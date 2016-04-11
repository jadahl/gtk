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

#ifndef GTKFOREIGN_EXPORTED_X11_H
#define GTKFOREIGN_EXPORTED_X11_H

#include "gtkforeignexported.h"

#define GTK_FOREIGN_TYPE_EXPORTED_X11 (gtk_foreign_exported_x11_get_type ())
G_DECLARE_FINAL_TYPE (GtkForeignExportedX11,
                      gtk_foreign_exported_x11,
                      GTK, FOREIGN_EXPORTED_X11,
                      GtkForeignExported)

GtkForeignExported * _gtk_foreign_exported_x11_new (GtkForeign *foreign,
                                                    GdkWindow  *window);

#endif /* GTKFOREIGN_EXPORTED_X11_H */

