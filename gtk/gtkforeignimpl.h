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

#ifndef GTKFOREIGN_IMPL_H
#define GTKFOREIGN_IMPL_H

#include "gtkforeign.h"

#define GTK_FOREIGN_TYPE_IMPL (_gtk_foreign_impl_get_type ())
G_DECLARE_DERIVABLE_TYPE (GtkForeignImpl, _gtk_foreign_impl,
                          GTK, FOREIGN_IMPL, GObject)

struct _GtkForeignImplClass
{
  GObjectClass parent_class;

  GtkForeignExported * (* export_window) (GtkForeignImpl   *impl,
                                          GdkWindow        *window);
};

GtkForeignImpl *     _gtk_foreign_create_impl        (GtkForeign       *foreign);

GtkForeignExported * _gtk_foreign_impl_export_window (GtkForeignImpl   *impl,
                                                      GdkWindow        *window);

GtkForeign *         _gtk_foreign_impl_get_foreign   (GtkForeignImpl   *impl);

#endif /* GTKFOREIGN_IMPL_H */
