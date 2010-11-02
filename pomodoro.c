/* pomodoro-applet: timer for the Pomodoro Technique
 * Copyright (C) 2010 John Stumpo
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
#include <gst/gst.h>

#include <string.h>

#define WORK_SECONDS 25*60
#define BREAK_SECONDS 5*60

struct pom_state {
  GtkWidget* label;
  enum {
    POM_STOPPED = 0,
    POM_WORK = 1,
    POM_BREAK = 2
  } state;
  int seconds;
  GstElement* playbin;
  GTimer* timer;
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
  NotifyNotification* note = notify_notification_new(summary, body, NULL, NULL);
  notify_notification_show(note, NULL);
  g_object_unref(note);

  /* Play the alarm tone if we need to. */
  if (sound) {
    gchar* alarm_tone_filename = g_build_filename(PKGDATADIR, "timerexpired.ogg", NULL);
    GFile* tonefile = g_file_new_for_path(alarm_tone_filename);
    g_free(alarm_tone_filename);

    g_object_set(G_OBJECT(state->playbin), "uri", g_file_get_uri(tonefile), NULL);
    g_object_unref(tonefile);
    gst_element_set_state(state->playbin, GST_STATE_PLAYING);
  }
}

static void pom_gst_message(GstBus* bus, GstMessage* msg, gpointer data)
{
  struct pom_state* state = data;
  GError* err = NULL;
  gchar* debuginfo = NULL;
  (void) bus;

  switch (msg->type) {
    case GST_MESSAGE_EOS:
      /* We reached the end of the file; reset the stream. */
      gst_element_set_state(state->playbin, GST_STATE_NULL);
      break;

    case GST_MESSAGE_ERROR:
      /* Report an error playing the file. */
      gst_element_set_state(state->playbin, GST_STATE_NULL);
      gst_message_parse_error(msg, &err, &debuginfo);
      g_printerr("GStreamer error: %s\n", err->message);
      g_printerr("Debug info: %s\n", debuginfo);
      g_error_free(err);
      g_free(debuginfo);
      break;

    default:
      break;
  }
}

/* Set the applet text according to the current timer state. */
static void pom_update_label(struct pom_state* state)
{
  gchar* new_text;
  int seconds_left = state->seconds - g_timer_elapsed(state->timer, NULL);
  switch (state->state) {
    case POM_STOPPED:
      new_text = g_strdup("Pomodoro");
      break;
    case POM_WORK:
      new_text = g_strdup_printf("Work: %02d:%02d", seconds_left / 60, seconds_left % 60);
      break;
    case POM_BREAK:
      new_text = g_strdup_printf("Break: %02d:%02d", seconds_left / 60, seconds_left % 60);
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
        pom_notify(state, "Pomodoro", "Break time!", TRUE);
        break;

      case POM_BREAK:
        /* The break period is over; go into idle mode. */
        state->state = POM_STOPPED;
        pom_notify(state, "Pomodoro", "The break period is over.", TRUE);
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
        pom_notify(state, "Pomodoro", "The current pomodoro was aborted.", FALSE);
        break;
      case POM_BREAK:
        state->state = POM_STOPPED;
        break;
    }
  }
  pom_update_label(state);
  return FALSE;
}

static void pom_about(BonoboUIComponent* component, gpointer data, const gchar* cname)
{
  const gchar* authors[] = {"John Stumpo", NULL};
  gchar* logo_filename = g_build_filename(PIXMAPDIR, "pomodoro.svg", NULL);
  GdkPixbuf* logo = gdk_pixbuf_new_from_file(logo_filename, NULL);
  g_free(logo_filename);

  (void) component;
  (void) data;
  (void) cname;

  gtk_show_about_dialog(NULL,
    "authors", authors,
    "comments", "Timer for the Pomodoro Technique",
    "copyright", "Copyright (C) 2010 John Stumpo",
    "license", "GNU GPL version 3 or later",
    "logo", logo,
    "version", PACKAGE_VERSION,
    NULL
  );

  g_object_unref(G_OBJECT(logo));
}

static const gchar* menu_xml =
  "<popup name=\"button3\">\n"
  "  <menuitem name=\"About Item\" verb=\"About\" _label=\"_About\"\n"
  "            pixtype=\"stock\" pixname=\"gtk-about\" />\n"
  "</popup>\n"
;

static const BonoboUIVerb menu_verbs[] = {
  BONOBO_UI_VERB("About", pom_about),
  BONOBO_UI_VERB_END
};

static gboolean pomodoro_applet_fill(PanelApplet* applet, const gchar* iid, gpointer data)
{
  struct pom_state* state;
  GstBus* bus;
  (void) data;

  if (strcmp(iid, "OAFIID:PomodoroApplet") != 0)
    return FALSE;

  if (!notify_is_initted())
    notify_init("Pomodoro");

  gst_init(NULL, NULL);

  /* Build the widget structure. */
  state = g_malloc0(sizeof(struct pom_state));
  state->label = gtk_label_new("Pomodoro");
  gtk_container_add(GTK_CONTAINER(applet), state->label);

  g_signal_connect(G_OBJECT(applet), "button-press-event", G_CALLBACK(pom_button_pressed), state);

  panel_applet_setup_menu(applet, menu_xml, menu_verbs, NULL);
  panel_applet_set_flags(applet, PANEL_APPLET_EXPAND_MINOR);
  gtk_widget_show_all(GTK_WIDGET(applet));

  /* Prepare GStreamer for playing the alarm tone. */
  state->playbin = gst_element_factory_make("playbin", NULL);
  bus = gst_element_get_bus(state->playbin);
  gst_bus_add_signal_watch(bus);
  g_signal_connect(G_OBJECT(bus), "message", G_CALLBACK(pom_gst_message), state);

  state->timer = g_timer_new();

  return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY("OAFIID:PomodoroApplet_Factory", PANEL_TYPE_APPLET, "PomodoroApplet", "0", pomodoro_applet_fill, NULL);
