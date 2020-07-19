/* pomodoro-applet: timer for the Pomodoro Technique
 * Copyright (C) 2010-2012, 2014, 2020 John Stumpo
 * Copyright (C) 2013 José Luis Segura Lucas
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

#include "applet-common.h"

#include <panel-applet.h>

#include <string.h>

static void about_wrap(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  (void) action;
  (void) parameter;
  pom_about(data);
}

static const gchar* menu_xml =
  "<section>\
     <item>\
       <attribute name=\"label\" translatable=\"yes\">_About</attribute>\
       <attribute name=\"action\">pomodoro.about</attribute>\
     </item>\
   </section>"
;

static const GActionEntry menu_actions[] = {
  {"about", about_wrap, NULL, NULL, NULL, NULL},
};

static gboolean pomodoro_applet_fill(PanelApplet* applet, const gchar* iid, gpointer data)
{
  struct pom_state* state;
  GSimpleActionGroup* action_group;
  (void) data;

  if (strcmp(iid, "PomodoroApplet") != 0)
    return FALSE;

  state = pom_common_fill(GTK_BIN(applet));

  /* Set up the action group and menu. */
  action_group = g_simple_action_group_new();
  g_action_map_add_action_entries(G_ACTION_MAP(action_group), menu_actions, G_N_ELEMENTS(menu_actions), state);
  panel_applet_setup_menu(applet, menu_xml, action_group, GETTEXT_PACKAGE);
  gtk_widget_insert_action_group(GTK_WIDGET(applet), "pomodoro", G_ACTION_GROUP(action_group));
  g_object_unref(action_group);
  panel_applet_set_flags(applet, PANEL_APPLET_EXPAND_MINOR);

  gtk_widget_show_all(GTK_WIDGET(applet));

  return TRUE;
}

PANEL_APPLET_IN_PROCESS_FACTORY("PomodoroAppletFactory", PANEL_TYPE_APPLET, pomodoro_applet_fill, NULL);
