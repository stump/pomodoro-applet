/* pomodoro-applet: timer for the Pomodoro Technique
 * Copyright (C) 2010-2012, 2014 John Stumpo
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <panel-applet.h>
#include <libnotify/notify.h>
#include <canberra-gtk.h>
#include <librsvg/rsvg.h>
#include <glib/gi18n.h>

#include <string.h>

#define WORK_SECONDS 25*60
#define BREAK_SECONDS 5*60

/* The stock applets hard-code 24x24 for images they load for this
   purpose, so I don't feel bad doing it too.  */
#define MINITOMATO_WIDTH 24
#define MINITOMATO_HEIGHT 24

struct pom_state {
  GtkWidget* label;
  enum {
    POM_STOPPED = 0,
    POM_WORK = 1,
    POM_BREAK = 2
  } state;
  int seconds;
  GTimer* timer;
  RsvgHandle* tomato_svg;
};

/* Set the timer to a new state and time. */
static void pom_set_timer(struct pom_state* state, int new_state, int seconds)
{
  state->state = new_state;
  state->seconds = seconds;
  g_timer_reset(state->timer);
}

static void pom_notify(struct pom_state* state, const gchar* summary, const gchar* body, gboolean sound)
{
  /* Show the libnotify notification. */
  NotifyNotification* note;
  note = notify_notification_new(summary, body, NULL);
  notify_notification_show(note, NULL);
  g_object_unref(note);

  /* Play the alarm tone if we need to. */
  if (sound) {
    gchar* alarm_tone_filename = g_build_filename(PKGDATADIR, "timerexpired.ogg", NULL);
    ca_gtk_play_for_widget(state->label, 0,
      CA_PROP_MEDIA_FILENAME, alarm_tone_filename,
      CA_PROP_CANBERRA_ENABLE, "1",  /* force override disabling of event sounds */
      NULL);
    g_free(alarm_tone_filename);
  }
}

/* Set the applet text according to the current timer state. */
static void pom_update_label(struct pom_state* state)
{
  gchar* new_text;
  int seconds_left = state->seconds - g_timer_elapsed(state->timer, NULL);
  switch (state->state) {
    case POM_STOPPED:
      new_text = g_strdup(_("Pomodoro"));
      break;
    case POM_WORK:
      new_text = g_strdup_printf(_("Work: %02d:%02d"), seconds_left / 60, seconds_left % 60);
      break;
    case POM_BREAK:
      new_text = g_strdup_printf(_("Break: %02d:%02d"), seconds_left / 60, seconds_left % 60);
      break;
    default:
      g_assert_not_reached();
      break;
  }
  gtk_label_set_text(GTK_LABEL(state->label), new_text);
  g_free(new_text);
}

static gboolean pom_update(gpointer data)
{
  struct pom_state* state = data;
  if (g_timer_elapsed(state->timer, NULL) >= state->seconds) {
    /* The current timer has expired. Advance to the next state. */
    switch (state->state) {
      case POM_WORK:
        /* The work period is over; go into break mode. */
        pom_set_timer(state, POM_BREAK, BREAK_SECONDS);
        pom_notify(state, _("Pomodoro"), _("Break time!"), TRUE);
        break;

      case POM_BREAK:
        /* The break period is over; go into idle mode. */
        state->state = POM_STOPPED;
        pom_notify(state, _("Pomodoro"), _("The break period is over."), TRUE);
        break;

      case POM_STOPPED:
        break;
    }
  }
  pom_update_label(state);
  return (state->state != POM_STOPPED);
}

static gboolean pom_button_pressed(GtkWidget* ebox, GdkEventButton* event, struct pom_state* state)
{
  (void) ebox;

  if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
    switch (state->state) {
      case POM_STOPPED:
        pom_set_timer(state, POM_WORK, WORK_SECONDS);
        g_timeout_add(100, pom_update, state);
        break;
      case POM_WORK:
        pom_set_timer(state, POM_BREAK, BREAK_SECONDS);
        pom_notify(state, _("Pomodoro"), _("The current pomodoro was aborted."), FALSE);
        break;
      case POM_BREAK:
        state->state = POM_STOPPED;
        break;
    }
  }
  pom_update_label(state);
  return FALSE;
}

static void pom_about(GtkAction* action, gpointer data)
{
  const gchar* authors[] = {"John Stumpo", NULL};
  const gchar* artists[] = {"J\xc3\xa1nos Horv\xc3\xa1th (icon)", NULL};
  GdkPixbuf* logo = rsvg_handle_get_pixbuf(((struct pom_state*)data)->tomato_svg);

  (void) action;
  (void) data;

  gtk_show_about_dialog(NULL,
    "authors", authors,
    "artists", artists,
    "comments", _("Timer for the Pomodoro Technique"),
    "copyright", "Copyright \xc2\xa9 2010-2012 John Stumpo",
    "license", "GNU GPL version 3 or later",
    "logo", logo,
    "version", PACKAGE_VERSION,
    NULL
  );

  g_object_unref(G_OBJECT(logo));
}

const gchar* pom_menu_xml =
  "<menuitem name=\"About\" action=\"About\" />"
;

static const GtkActionEntry menu_actions[] = {
  {"About", GTK_STOCK_ABOUT, N_("_About"), NULL, NULL, G_CALLBACK(pom_about)},
};

struct pom_state* pom_common_fill(GtkBin* applet)
{
  struct pom_state* state;
  gchar* logo_filename;
  GdkPixbuf* minitomato;
  GtkWidget* hbox;

  if (!notify_is_initted())
    notify_init("Pomodoro");

  /* Build the widget structure. */
  state = g_malloc0(sizeof(struct pom_state));
  state->label = gtk_label_new(_("Pomodoro"));
  hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_container_add(GTK_CONTAINER(applet), hbox);

  g_signal_connect(G_OBJECT(applet), "button-press-event", G_CALLBACK(pom_button_pressed), state);

  /* Load the tomato icon for use in the about dialog. */
  logo_filename = g_build_filename(PIXMAPDIR, "pomodoro.svg", NULL);
  state->tomato_svg = rsvg_handle_new_from_file(logo_filename, NULL);
  if (state->tomato_svg != NULL) {
    RsvgDimensionData dims;
    int orig_width, orig_height;
    cairo_surface_t* surface;
    cairo_t* cr;

    rsvg_handle_get_dimensions(state->tomato_svg, &dims);
    orig_width = dims.width;
    orig_height = dims.height;
    if (dims.width > MINITOMATO_WIDTH) {
      dims.height = (dims.height * MINITOMATO_WIDTH) / dims.width;
      dims.width = MINITOMATO_WIDTH;
    }
    if (dims.height > MINITOMATO_HEIGHT) {
      dims.width = (dims.width * MINITOMATO_HEIGHT) / dims.height;
      dims.height = MINITOMATO_HEIGHT;
    }

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, dims.width, dims.height);
    cr = cairo_create(surface);
    cairo_scale(cr, dims.width/(double)orig_width, dims.height/(double)orig_height);
    rsvg_handle_render_cairo(state->tomato_svg, cr);
    cairo_destroy(cr);
    minitomato = gdk_pixbuf_get_from_surface(surface, 0, 0, dims.width, dims.height);
    gtk_box_pack_start(GTK_BOX(hbox), gtk_image_new_from_pixbuf(minitomato), FALSE, FALSE, 0);
    g_object_unref(minitomato);
    cairo_surface_destroy(surface);
  }
  g_free(logo_filename);

  gtk_box_pack_start(GTK_BOX(hbox), state->label, FALSE, FALSE, 0);
  state->timer = g_timer_new();

  return state;
}


GtkActionGroup* pom_make_action_group(struct pom_state* state)
{
  GtkActionGroup* action_group = action_group = gtk_action_group_new("Pomodoro Applet Actions");
  gtk_action_group_set_translation_domain(action_group, GETTEXT_PACKAGE);
  gtk_action_group_add_actions(action_group, menu_actions, G_N_ELEMENTS(menu_actions), state);
  return action_group;
}

static gboolean pomodoro_applet_fill(PanelApplet* applet, const gchar* iid, gpointer data)
{
  struct pom_state* state;
  GtkActionGroup* action_group;
  (void) data;

  if (strcmp(iid, "PomodoroApplet") != 0)
    return FALSE;

  state = pom_common_fill(GTK_BIN(applet));

  /* Set up the action group and menu. */
  action_group = pom_make_action_group(state);
  panel_applet_setup_menu(applet, pom_menu_xml, action_group);
  g_object_unref(action_group);
  panel_applet_set_flags(applet, PANEL_APPLET_EXPAND_MINOR);

  gtk_widget_show_all(GTK_WIDGET(applet));

  return TRUE;
}

PANEL_APPLET_OUT_PROCESS_FACTORY("PomodoroAppletFactory", PANEL_TYPE_APPLET, pomodoro_applet_fill, NULL);
