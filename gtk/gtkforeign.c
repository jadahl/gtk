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

#include "gtkforeign.h"

#include "gtkforeignimpl.h"
#include "gtkforeignprivate.h"

static GQuark quark_gtk_foreign = 0;

struct _GtkForeign
{
  GdkDisplay *display;
  GtkForeignImpl *impl;
};

static GtkForeign *
gtk_foreign_new (GdkDisplay *display)
{
  GtkForeign *foreign;

  foreign = g_new0 (GtkForeign, 1);

  foreign->display = display;
  foreign->impl = _gtk_foreign_create_impl (foreign);

  return foreign;
}

static void
gtk_foreign_free (GtkForeign *foreign)
{
  g_object_unref (foreign->impl);
  g_free (foreign);
}

GtkForeign *
_gtk_foreign_from_display (GdkDisplay *display)
{
  GtkForeign *foreign;

  if (quark_gtk_foreign == 0)
    {
      foreign = gtk_foreign_new (display);

      quark_gtk_foreign = g_quark_from_static_string ("gtk-window-foreign");
      g_object_set_qdata_full (G_OBJECT (display), quark_gtk_foreign,
                               foreign, (GDestroyNotify) gtk_foreign_free);
    }
  else
    {
      foreign = g_object_get_qdata (G_OBJECT (display), quark_gtk_foreign);
    }

  return foreign;
}

GtkForeignExported *
_gtk_foreign_export_window (GtkForeign *foreign,
                            GdkWindow  *window)
{
  return _gtk_foreign_impl_export_window (foreign->impl, window);
}

GdkDisplay *
_gtk_foreign_get_gdk_display (GtkForeign *foreign)
{
  return foreign->display;
}
