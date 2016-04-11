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

#include <glib.h>

#include "gtkforeignexported.h"

G_DEFINE_TYPE (GtkForeignExported, _gtk_foreign_exported, G_TYPE_OBJECT)

char *
_gtk_foreign_exported_get_handle_str (GtkForeignExported *exported)
{
  return GTK_FOREIGN_EXPORTED_GET_CLASS (exported)->get_handle_str (exported);
}

static void
_gtk_foreign_exported_class_init (GtkForeignExportedClass *klass)
{
}

static void
_gtk_foreign_exported_init (GtkForeignExported *exported)
{
}
