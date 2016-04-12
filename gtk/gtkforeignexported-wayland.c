/*
 * Copyright (C) 2015 Red Hat Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by:
 *     Jonas Ådahl <jadahl@gmail.com>
 */

#include "config.h"

#include "gtkforeignexported-wayland.h"

#include "gtkforeignprivate.h"
#include "gtkforeignexported.h"
#include "wayland/gdkwayland.h"

#include "xdg-foreign-unstable-v1-client-protocol.h"

struct _GtkForeignExportedWayland
{
  GtkForeignExported parent;

  struct zxdg_exported_v1 *xdg_exported;
  char *handle_str;
};

G_DEFINE_TYPE (GtkForeignExportedWayland,
               gtk_foreign_exported_wayland,
               GTK_FOREIGN_TYPE_EXPORTED);

static char *
gtk_foreign_exported_wayland_get_handle_str (GtkForeignExported *exported)
{
  GtkForeignExportedWayland *exported_wayland =
    GTK_FOREIGN_EXPORTED_WAYLAND (exported);

  return g_strdup (exported_wayland->handle_str);
}

static void
gtk_foreign_exported_wayland_handle_handle (void                    *data,
                                            struct zxdg_exported_v1 *xdg_exported,
                                            const char              *handle)
{
  GtkForeignExportedWayland *exported_wayland = data;

  exported_wayland->handle_str = g_strdup_printf ("wayland:%s", handle);

  g_signal_emit_by_name (GTK_FOREIGN_EXPORTED (exported_wayland),
                         "window-exported",
                         0);
}

static const struct zxdg_exported_v1_listener gtk_foreign_xdg_exported_listener = {
  gtk_foreign_exported_wayland_handle_handle,
};

GtkForeignExported *
_gtk_foreign_exported_wayland_new (GtkForeign              *foreign,
                                   struct zxdg_exported_v1 *xdg_exported)
{
  GtkForeignExportedWayland *exported_wayland;

  exported_wayland = g_object_new (GTK_FOREIGN_TYPE_EXPORTED_WAYLAND,
                                   "foreign", foreign,
                                   NULL);
  exported_wayland->xdg_exported = xdg_exported;


  zxdg_exported_v1_add_listener (xdg_exported,
                                 &gtk_foreign_xdg_exported_listener,
                                 exported_wayland);

  return GTK_FOREIGN_EXPORTED (exported_wayland);
}

static void
gtk_foreign_exported_wayland_finalize (GObject *gobject)
{
  GtkForeignExportedWayland *exported_wayland =
    GTK_FOREIGN_EXPORTED_WAYLAND (gobject);

  zxdg_exported_v1_destroy (exported_wayland->xdg_exported);

  G_OBJECT_CLASS (gtk_foreign_exported_wayland_parent_class)->finalize (gobject);
}

static void
gtk_foreign_exported_wayland_class_init (GtkForeignExportedWaylandClass *klass)
{
  GtkForeignExportedClass *exported_class = GTK_FOREIGN_EXPORTED_CLASS (klass);
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  exported_class->get_handle_str = gtk_foreign_exported_wayland_get_handle_str;

  gobject_class->finalize = gtk_foreign_exported_wayland_finalize;
}

static void
gtk_foreign_exported_wayland_init (GtkForeignExportedWayland *exported_wayland)
{
}
