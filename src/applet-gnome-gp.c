/* pomodoro-applet: timer for the Pomodoro Technique
 * Copyright (C) 2010-2012, 2014, 2020 John Stumpo
 * Copyright (C) 2013 José Luis Segura Lucas
 * Copyright (C) 2015, 2017 Balló György
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

#include <libgnome-panel/gp-module.h>
#include <libgnome-panel/gp-applet.h>
#include <glib/gi18n-lib.h>

/* For this, we need to define our own subclass of GpApplet.
   Just do it right in this file.  */
#define POMODORO_TYPE_APPLET pomodoro_applet_get_type()
G_DECLARE_FINAL_TYPE(PomodoroApplet, pomodoro_applet, POMODORO, APPLET, GpApplet)

struct _PomodoroApplet {
  GpApplet parent_instance;
  struct pom_state* state;
};

G_DEFINE_TYPE(PomodoroApplet, pomodoro_applet, GP_TYPE_APPLET)

static void about_wrap(GSimpleAction *action, GVariant *parameter, gpointer data)
{
  (void) action;
  (void) parameter;
  pom_about(POMODORO_APPLET(data)->state);
}

static const gchar* menu_xml =
  "<interface>\
     <menu id=\"pomodoro-applet-menu\">\
       <section>\
         <item>\
           <attribute name=\"label\" translatable=\"yes\">_About</attribute>\
           <attribute name=\"action\">pomodoro-applet.about</attribute>\
         </item>\
       </section>\
     </menu>\
   </interface>"
;

static const GActionEntry menu_actions[] = {
  {"about", about_wrap, NULL, NULL, NULL, {0}},
  {NULL},
};

/* Fill the applet in after all other object setup has been done. */
static void pomodoro_applet_constructed(GObject* object)
{
  GpApplet* applet = GP_APPLET(object);

  G_OBJECT_CLASS(pomodoro_applet_parent_class)->constructed(object);

  POMODORO_APPLET(applet)->state = pom_common_fill(GTK_BIN(applet));

  gp_applet_setup_menu(applet, menu_xml, menu_actions);
  gp_applet_set_flags(applet, GP_APPLET_FLAGS_EXPAND_MINOR);

  gtk_widget_show_all(GTK_WIDGET(applet));
}

static void pomodoro_applet_class_init(PomodoroAppletClass* class_)
{
  G_OBJECT_CLASS(class_)->constructed = pomodoro_applet_constructed;
}

static void pomodoro_applet_init(PomodoroApplet* applet)
{
  (void) applet;
}

/* And now, the gnome-panel module glue.
   First, the info factory...  */
static GpAppletInfo* pom_get_applet_info(const gchar* id)
{
  g_assert_cmpstr(id, ==, "pomodoro-applet");

  return gp_applet_info_new(pomodoro_applet_get_type,
                            _("Pomodoro Applet"),
                            _("Time the intervals used in the Pomodoro time management technique."),
                            "pomodoro");
}

/* ...then our upgrade path from the libpanel-applet based applet... */
static const gchar* pom_get_applet_id_from_iid(const gchar* iid)
{
  if (g_strcmp0(iid, "PomodoroAppletFactory::PomodoroApplet") == 0)
    return "pomodoro-applet";

  return NULL;
}

/* ...and finally, the module load function to tie everything together. */
G_MODULE_EXPORT void gp_module_load(GpModule* module)
{
  bindtextdomain(GETTEXT_PACKAGE, GNOMELOCALEDIR);
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  gp_module_set_gettext_domain(module, GETTEXT_PACKAGE);

  gp_module_set_abi_version(module, GP_MODULE_ABI_VERSION);

  gp_module_set_id(module, "io.stump.pomodoro-applet");
  gp_module_set_version(module, PACKAGE_VERSION);

  gp_module_set_applet_ids(module, "pomodoro-applet", NULL);

  gp_module_set_get_applet_info(module, pom_get_applet_info);
  gp_module_set_compatibility(module, pom_get_applet_id_from_iid);
}
