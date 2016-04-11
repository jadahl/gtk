/*
 * Copyright (C) 2016 Red Hat Inc.
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

#include "gtkforeignexported-x11.h"

#include "gtkforeignprivate.h"
#include "gtkforeignexported.h"
#include "x11/gdkx.h"

struct _GtkForeignExportedX11
{
  GtkForeignExported parent;

  GdkWindow *window;
};

G_DEFINE_TYPE (GtkForeignExportedX11,
               gtk_foreign_exported_x11,
               GTK_FOREIGN_TYPE_EXPORTED)

static char *
gtk_foreign_exported_x11_get_handle_str (GtkForeignExported *exported)
{
  GtkForeignExportedX11 *exported_x11 = GTK_FOREIGN_EXPORTED_X11 (exported);
  GdkWindow *window = exported_x11->window;

  return g_strdup_printf ("x11:%x", (guint32) gdk_x11_window_get_xid (window));
}

GtkForeignExported *
_gtk_foreign_exported_x11_new (GtkForeign *foreign,
                               GdkWindow  *window)
{
  GtkForeignExportedX11 *exported_x11;

  exported_x11 = g_object_new (GTK_FOREIGN_TYPE_EXPORTED_X11, NULL);
  exported_x11->window = window;

  return GTK_FOREIGN_EXPORTED (exported_x11);
}

static void
gtk_foreign_exported_x11_class_init (GtkForeignExportedX11Class *klass)
{
  GtkForeignExportedClass *exported_class = GTK_FOREIGN_EXPORTED_CLASS (klass);

  exported_class->get_handle_str = gtk_foreign_exported_x11_get_handle_str;
}

static void
gtk_foreign_exported_x11_init (GtkForeignExportedX11 *exported_x11)
{
}

