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

#include <mate-panel-applet.h>
#include <glib/gi18n-lib.h>

#include <string.h>

static void about_wrap(GtkAction* action, gpointer data)
{
  (void) action;
  pom_about(data);
}

static const gchar* menu_xml =
  "<menuitem name=\"About\" action=\"About\" />"
;

static const GtkActionEntry menu_actions[] = {
  {"About", "help-about", N_("_About"), NULL, NULL, G_CALLBACK(about_wrap)},
};

static gboolean pomodoro_applet_fill(MatePanelApplet* applet, const gchar* iid, gpointer data)
{
  struct pom_state* state;
  GtkActionGroup* action_group;
  (void) data;

  if (strcmp(iid, "PomodoroApplet") != 0)
    return FALSE;

  state = pom_common_fill(GTK_BIN(applet));

  /* Set up the action group and menu. */
  /* mate-panel-applet has not yet been updated to avoid using the deprecated GtkActionGroup type. */
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  action_group = gtk_action_group_new("Pomodoro Applet Actions");
  gtk_action_group_set_translation_domain(action_group, GETTEXT_PACKAGE);
  gtk_action_group_add_actions(action_group, menu_actions, G_N_ELEMENTS(menu_actions), state);
G_GNUC_END_IGNORE_DEPRECATIONS
  mate_panel_applet_setup_menu(applet, menu_xml, action_group);
  g_object_unref(action_group);
  mate_panel_applet_set_flags(applet, MATE_PANEL_APPLET_EXPAND_MINOR);

  gtk_widget_show_all(GTK_WIDGET(applet));

  return TRUE;
}

#ifdef BUILD_MATE_IN_PROCESS
MATE_PANEL_APPLET_IN_PROCESS_FACTORY("PomodoroAppletFactory", PANEL_TYPE_APPLET, "", pomodoro_applet_fill, NULL);
#else
MATE_PANEL_APPLET_OUT_PROCESS_FACTORY("PomodoroAppletFactory", PANEL_TYPE_APPLET, "", pomodoro_applet_fill, NULL);
#endif
