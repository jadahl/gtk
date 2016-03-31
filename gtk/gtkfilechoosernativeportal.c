/* -*- Mode: C; c-file-style: "gnu"; tab-width: 8 -*- */
/* GTK - The GIMP Toolkit
 * gtkfilechoosernativeportal.c: Portal File selector dialog
 * Copyright (C) 2015, Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gtkfilechoosernativeprivate.h"
#include "gtknativedialogprivate.h"

#include "gtkprivate.h"
#include "gtkfilechooserdialog.h"
#include "gtkfilechooserprivate.h"
#include "gtkfilechooserwidget.h"
#include "gtkfilechooserwidgetprivate.h"
#include "gtkfilechooserutils.h"
#include "gtkfilechooserembed.h"
#include "gtkfilesystem.h"
#include "gtksizerequest.h"
#include "gtktypebuiltins.h"
#include "gtkintl.h"
#include "gtksettings.h"
#include "gtktogglebutton.h"
#include "gtkstylecontext.h"
#include "gtkheaderbar.h"
#include "gtklabel.h"
#include "gtkmain.h"
#include "gtkinvisible.h"
#include "gtkfilechooserentry.h"
#include "gtkfilefilterprivate.h"

#ifdef GDK_WINDOWING_X11
#include "x11/gdkx.h"
#endif

typedef struct {
  GtkFileChooserNative *self;

  GtkWidget *grab_widget;

  GDBusConnection *connection;
  char *portal_handle;
  guint portal_response_signal_id;
  gboolean modal;

  gboolean hidden;

} FilechooserPortalData;


static void
filechooser_portal_data_free (FilechooserPortalData *data)
{
  if (data->portal_response_signal_id != 0)
    g_dbus_connection_signal_unsubscribe (data->connection,
                                          data->portal_response_signal_id);

  //TODO: This causes a crash in other code. Do we double unref somewhere?
  // g_object_unref (data->connection);

  if (data->grab_widget)
    {
      gtk_grab_remove (data->grab_widget);
      gtk_widget_destroy (data->grab_widget);
    }

  if (data->self)
    g_object_unref (data->self);

  g_free (data->portal_handle);

  g_free (data);
}

static void
open_file_response (GDBusConnection  *connection,
                    const gchar      *sender_name,
                    const gchar      *object_path,
                    const gchar      *interface_name,
                    const gchar      *signal_name,
                    GVariant         *parameters,
                    gpointer          user_data)
{
  GtkFileChooserNative *self = user_data;
  FilechooserPortalData *data = self->mode_data;
  guint32 portal_response;
  int gtk_response;
  char *uri, *handle;;
  GVariant *response_data;

  g_variant_get (parameters, "(&ou&s@a{sv})", &handle, &portal_response, &uri, &response_data);

  if (data->portal_handle == NULL ||
      strcmp (handle, data->portal_handle) != 0)
    return;

  g_slist_free_full (self->custom_files, g_object_unref);
  self->custom_files = g_slist_prepend (NULL, g_file_new_for_uri (uri));

  switch (portal_response)
    {
    case 0:
      gtk_response = GTK_RESPONSE_OK;
      break;
    case 1:
      gtk_response = GTK_RESPONSE_CANCEL;
      break;
    case 2:
    default:
      gtk_response = GTK_RESPONSE_DELETE_EVENT;
      break;
    }

  filechooser_portal_data_free (data);
  self->mode_data = NULL;

  _gtk_native_dialog_emit_response (GTK_NATIVE_DIALOG (self), gtk_response);
}

static void
send_close (FilechooserPortalData *data)
{
  GDBusMessage *message;
  GError *error = NULL;

  message = g_dbus_message_new_method_call ("org.freedesktop.portal.Desktop",
                                            "/org/freedesktop/portal/desktop",
                                            "org.freedesktop.portal.FileChooser",
                                            "Close");
  g_dbus_message_set_body (message,
                           g_variant_new ("(o)", data->portal_handle));

  if (!g_dbus_connection_send_message (data->connection,
                                       message,
                                       G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                       NULL, &error))
    {
      g_warning ("unable to send FileChooser Close message: %s", error->message);
      g_error_free (error);
    }
}

static void
open_file_msg_cb (GObject *source_object,
                  GAsyncResult *res,
                  gpointer user_data)
{
  FilechooserPortalData *data = user_data;
  GtkFileChooserNative *self = data->self;
  GDBusMessage *reply;
  GError *error = NULL;

  reply = g_dbus_connection_send_message_with_reply_finish (data->connection, res, &error);
  if (reply == NULL)
    {
      if (!data->hidden)
        _gtk_native_dialog_emit_response (GTK_NATIVE_DIALOG (self), GTK_RESPONSE_DELETE_EVENT);
      g_warning ("Can't open portal file chooser: %s\n", error->message);
      filechooser_portal_data_free (data);
      return;
    }

  g_variant_get_child (g_dbus_message_get_body (reply), 0, "o",
                       &data->portal_handle);

  if (data->hidden)
    {
      /* The dialog was hidden before we got the handle, close it now */
      send_close (data);
      filechooser_portal_data_free (data);
      return;
    }
}

gboolean
gtk_file_chooser_native_portal_show (GtkFileChooserNative *self)
{
  FilechooserPortalData *data;
  GtkWindow *transient_for;
  guint update_preview_signal;
  GDBusConnection *connection;
  char *parent_window_str;
  GDBusMessage *message;
  GVariantBuilder opt_builder;

  if (g_getenv ("GTK_USE_PORTAL") == NULL)
    return FALSE;

  if (gtk_file_chooser_get_extra_widget (GTK_FILE_CHOOSER (self)) != NULL)
    return FALSE;

  update_preview_signal = g_signal_lookup ("update-preview", GTK_TYPE_FILE_CHOOSER);
  if (g_signal_has_handler_pending (self, update_preview_signal, 0, TRUE))
    return FALSE;

  connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL);
  if (connection == NULL)
    return FALSE;

  data = g_new0 (FilechooserPortalData, 1);
  data->self = g_object_ref (self);
  data->connection = connection;

  message = g_dbus_message_new_method_call ("org.freedesktop.portal.Desktop",
                                            "/org/freedesktop/portal/desktop",
                                            "org.freedesktop.portal.FileChooser",
                                            "OpenFile");

  parent_window_str = NULL;
  transient_for = gtk_native_dialog_get_transient_for (GTK_NATIVE_DIALOG (self));
  if (transient_for != NULL && gtk_widget_is_visible (GTK_WIDGET (transient_for)))
    {
      GdkWindow *window = gtk_widget_get_window (GTK_WIDGET (transient_for));
#ifdef GDK_WINDOWING_X11
      if (GDK_IS_X11_WINDOW(window))
        parent_window_str = g_strdup_printf ("x11:%x", (guint32)gdk_x11_window_get_xid (window));
#endif
    }

  if (gtk_native_dialog_get_modal (GTK_NATIVE_DIALOG (self)))
    data->modal = TRUE;

  if (data->modal && transient_for)
    {
      data->grab_widget = gtk_invisible_new_for_screen (gtk_widget_get_screen (GTK_WIDGET (transient_for)));
      gtk_grab_add (GTK_WIDGET (data->grab_widget));
    }

  g_variant_builder_init (&opt_builder, G_VARIANT_TYPE_VARDICT);
  g_variant_builder_add (&opt_builder, "{sv}", "accept_label",
                         g_variant_new_string (self->accept_label));
  g_variant_builder_add (&opt_builder, "{sv}", "cancel_label",
                         g_variant_new_string (self->cancel_label));
  g_variant_builder_add (&opt_builder, "{sv}", "modal",
                         g_variant_new_boolean (data->modal));

  g_dbus_message_set_body (message,
                           g_variant_new ("(ss@a{sv})",
                                          parent_window_str ? parent_window_str : "",
                                          gtk_native_dialog_get_title (GTK_NATIVE_DIALOG (self)),
                                          g_variant_builder_end (&opt_builder)));
  g_free (parent_window_str);

  data->portal_response_signal_id =
    g_dbus_connection_signal_subscribe (data->connection,
                                        "org.freedesktop.portal.Desktop",
                                        "org.freedesktop.portal.FileChooser",
                                        "OpenFileResponse",
                                        "/org/freedesktop/portal/desktop",
                                        NULL,
                                        G_DBUS_SIGNAL_FLAGS_NO_MATCH_RULE,
                                        open_file_response,
                                        self, NULL);

  g_dbus_connection_send_message_with_reply (data->connection,
                                             message,
                                             G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                             -1,
                                             NULL,
                                             NULL,
                                             open_file_msg_cb,
                                             data);


  self->mode_data = data;
  return TRUE;
}

void
gtk_file_chooser_native_portal_hide (GtkFileChooserNative *self)
{
  FilechooserPortalData *data = self->mode_data;

  /* This is always set while dialog visible */
  g_assert (data != NULL);

  data->hidden = TRUE;

  if (data->portal_handle)
    {
      send_close (data);
      filechooser_portal_data_free (data);
    }

  self->mode_data = NULL;
}
