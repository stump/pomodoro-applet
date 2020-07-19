/* pomodoro-applet: timer for the Pomodoro Technique
 * Copyright (C) 2014 John Stumpo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POM_APPLET_COMMON_H
#define POM_APPLET_COMMON_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

extern const gchar* pom_menu_xml_gnome;
extern const gchar* pom_menu_xml;

struct pom_state;
struct pom_state* pom_common_fill(GtkBin* applet);
GSimpleActionGroup* pom_make_action_group_gnome(struct pom_state* state);
GtkActionGroup* pom_make_action_group(struct pom_state* state);

G_END_DECLS

#endif
