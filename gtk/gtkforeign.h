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

#ifndef GTKFOREIGN_H
#define GTKFOREIGN_H

#include <gdk/gdk.h>

typedef struct _GtkForeign GtkForeign;
typedef struct _GtkForeignExported GtkForeignExported;

GtkForeign *         _gtk_foreign_from_display          (GdkDisplay           *display);

GtkForeignExported * _gtk_foreign_export_window         (GtkForeign           *foreign,
                                                         GdkWindow            *window);

char *               _gtk_foreign_exported_get_handle_str (GtkForeignExported *exported);

#endif /* GTKFOREIGN_H */
