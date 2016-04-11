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

#include "gtkforeignimpl-x11.h"

#include "gtkforeignexported-x11.h"
#include "gtkforeignprivate.h"

struct _GtkForeignImplX11
{
  GtkForeignImpl parent;
};

G_DEFINE_TYPE (GtkForeignImplX11, gtk_foreign_impl_x11,
               GTK_FOREIGN_TYPE_IMPL);

static GtkForeignExported *
gtk_foreign_impl_x11_export_window (GtkForeignImpl *impl,
                                    GdkWindow      *window)
{
  GtkForeign *foreign = _gtk_foreign_impl_get_foreign (impl);

  return _gtk_foreign_exported_x11_new (foreign, window);
}

static void
gtk_foreign_impl_x11_class_init (GtkForeignImplX11Class *klass)
{
  GtkForeignImplClass *impl_class = GTK_FOREIGN_IMPL_CLASS (klass);

  impl_class->export_window = gtk_foreign_impl_x11_export_window;
}

static void
gtk_foreign_impl_x11_init (GtkForeignImplX11 *impl_x11)
{
}
