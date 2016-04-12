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

#include "gtkforeignimpl.h"

#include "gtkforeignprivate.h"

#ifdef GDK_WINDOWING_WAYLAND
#include "gtkforeignimpl-wayland.h"
#include "wayland/gdkwayland.h"
#endif
#ifdef GDK_WINDOWING_X11
#include "gtkforeignimpl-x11.h"
#include "x11/gdkx.h"
#endif

typedef struct _GtkForeignImplPrivate
{
  GtkForeign *foreign;
} GtkForeignImplPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GtkForeignImpl, _gtk_foreign_impl, G_TYPE_OBJECT);

GtkForeignImpl *
_gtk_foreign_create_impl (GtkForeign *foreign)
{
  GtkForeignImpl *impl = NULL;
  GtkForeignImplPrivate *priv;

#ifdef GDK_WINDOWING_WAYLAND
  if (GDK_IS_WAYLAND_DISPLAY (_gtk_foreign_get_gdk_display (foreign)))
    impl = g_object_new (GTK_FOREIGN_TYPE_IMPL_WAYLAND, NULL);
#endif
#ifdef GDK_WINDOWING_X11
  if (GDK_IS_X11_DISPLAY (_gtk_foreign_get_gdk_display (foreign)))
    impl = g_object_new (GTK_FOREIGN_TYPE_IMPL_X11, NULL);
#endif

  if (!impl)
    {
      g_error ("Unsupported display type\n");
      return NULL;
    }

  priv = _gtk_foreign_impl_get_instance_private (impl);
  priv->foreign = foreign;

  return impl;
}

GtkForeignExported *
_gtk_foreign_impl_export_window (GtkForeignImpl   *impl,
                                 GdkWindow        *window)
{
  g_return_val_if_fail (gdk_window_get_window_type (window) == GDK_WINDOW_TOPLEVEL,
                        NULL);

  return GTK_FOREIGN_IMPL_GET_CLASS (impl)->export_window (impl, window);
}

GtkForeign *
_gtk_foreign_impl_get_foreign (GtkForeignImpl *impl)
{
  GtkForeignImplPrivate *priv = _gtk_foreign_impl_get_instance_private (impl);

  return priv->foreign;
}

static void
_gtk_foreign_impl_class_init (GtkForeignImplClass *klass)
{
}

static void
_gtk_foreign_impl_init (GtkForeignImpl *impl)
{
}
