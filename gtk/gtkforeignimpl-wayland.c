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

#include "gtkforeignimpl-wayland.h"

#include "gtkforeignexported-wayland.h"
#include "gtkforeignprivate.h"
#include "gtkwidget.h"
#include "wayland/gdkwayland.h"

#include "xdg-foreign-unstable-v1-client-protocol.h"

struct _GtkForeignImplWayland
{
  GtkForeignImpl parent;

  struct zxdg_exporter_v1 *xdg_exporter;
  gboolean globals_discovered;
};

G_DEFINE_TYPE (GtkForeignImplWayland, gtk_foreign_impl_wayland,
               GTK_FOREIGN_TYPE_IMPL);

static void
registry_handle_global (void               *data,
                        struct wl_registry *registry,
                        uint32_t            name,
                        const char         *interface,
                        uint32_t            version)
{
  GtkForeignImplWayland *impl_wayland = data;

  if (g_str_equal (interface, "zxdg_exporter_v1"))
    impl_wayland->xdg_exporter = wl_registry_bind (registry,
                                                   name,
                                                   &zxdg_exporter_v1_interface,
                                                   1);
}

static void
registry_handle_global_remove (void               *data,
                               struct wl_registry *registry,
                               uint32_t            name)
{
}

static const struct wl_registry_listener registry_listener = {
  registry_handle_global,
  registry_handle_global_remove
};

static void
discover_globals (GtkForeignImplWayland *impl_wayland)
{
  GtkForeign *foreign;
  GdkDisplay *display;
  struct wl_display *wl_display;
  struct wl_registry *registry;

  if (impl_wayland->globals_discovered)
    return;

  foreign = _gtk_foreign_impl_get_foreign (GTK_FOREIGN_IMPL (impl_wayland));
  display = _gtk_foreign_get_gdk_display (foreign);
  wl_display = gdk_wayland_display_get_wl_display (display);

  registry = wl_display_get_registry (wl_display);
  wl_registry_add_listener (registry, &registry_listener, impl_wayland);

  wl_display_roundtrip (wl_display);

  impl_wayland->globals_discovered = TRUE;
}

static GtkForeignExported *
gtk_foreign_impl_wayland_export_window (GtkForeignImpl *impl,
                                        GdkWindow      *window)
{
  GtkForeign *foreign = _gtk_foreign_impl_get_foreign (impl);
  GtkForeignImplWayland *impl_wayland = GTK_FOREIGN_IMPL_WAYLAND (impl);
  struct wl_surface *wl_surface;
  struct zxdg_exported_v1 *xdg_exported;

  gtk_widget_realize (GTK_WIDGET (window));

  wl_surface = gdk_wayland_window_get_wl_surface (window);
  if (!wl_surface)
    {
      g_warning ("No wl_surface to export\n");
      return NULL;
    }

  discover_globals (impl_wayland);
  if (!impl_wayland->xdg_exporter)
    {
      g_warning ("Wayland server doesn't support exporting\n");
      return NULL;
    }

  xdg_exported = zxdg_exporter_v1_export (impl_wayland->xdg_exporter,
                                          wl_surface);
  return _gtk_foreign_exported_wayland_new (foreign, xdg_exported);
}

static void
gtk_foreign_impl_wayland_class_init (GtkForeignImplWaylandClass *klass)
{
  GtkForeignImplClass *impl_class = GTK_FOREIGN_IMPL_CLASS (klass);

  impl_class->export_window = gtk_foreign_impl_wayland_export_window;
}

static void
gtk_foreign_impl_wayland_init (GtkForeignImplWayland *impl_wayland)
{
}
