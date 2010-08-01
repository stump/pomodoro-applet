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
  int seconds_left;
  GstElement* playbin;
};

static void pom_set_timer(struct pom_state* state, int new_state, int seconds)
{
  state->state = new_state;
  state->seconds_left = seconds;
}

static void pom_notify(struct pom_state* state, const gchar* summary, const gchar* body, gboolean sound)
{
  NotifyNotification* note = notify_notification_new(summary, body, NULL, NULL);
  notify_notification_show(note, NULL);
  g_object_unref(note);

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
      gst_element_set_state(state->playbin, GST_STATE_NULL);
      break;
    case GST_MESSAGE_ERROR:
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

static void pom_update_label(struct pom_state* state)
{
  gchar* new_text;
  switch (state->state) {
    case POM_STOPPED:
      new_text = g_strdup("Pomodoro");
      break;
    case POM_WORK:
      new_text = g_strdup_printf("Work: %02d:%02d", state->seconds_left / 60, state->seconds_left % 60);
      break;
    case POM_BREAK:
      new_text = g_strdup_printf("Break: %02d:%02d", state->seconds_left / 60, state->seconds_left % 60);
      break;
  }
  gtk_label_set_text(GTK_LABEL(state->label), new_text);
  g_free(new_text);
}

static gboolean pom_second(gpointer data)
{
  struct pom_state* state = data;
  if (--state->seconds_left == 0) {
    switch (state->state) {
      case POM_WORK:
        pom_set_timer(state, POM_BREAK, BREAK_SECONDS);
        pom_notify(state, "Pomodoro", "Break time!", TRUE);
        break;
      case POM_BREAK:
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
        g_timeout_add(1000, pom_second, state);
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

static gboolean pomodoro_applet_fill(PanelApplet* applet, const gchar* iid, gpointer data)
{
  struct pom_state* state;
  GtkWidget* ebox;
  GstBus* bus;
  (void) data;

  if (strcmp(iid, "OAFIID:PomodoroApplet") != 0)
    return FALSE;

  if (!notify_is_initted())
    notify_init("Pomodoro");

  gst_init(NULL, NULL);

  state = g_malloc0(sizeof(struct pom_state));
  state->label = gtk_label_new("Pomodoro");
  ebox = gtk_event_box_new();
  gtk_container_add(GTK_CONTAINER(ebox), state->label);
  gtk_container_add(GTK_CONTAINER(applet), ebox);

  g_signal_connect(G_OBJECT(ebox), "button-press-event", G_CALLBACK(pom_button_pressed), state);
  gtk_widget_show_all(GTK_WIDGET(applet));

  state->playbin = gst_element_factory_make("playbin", NULL);
  bus = gst_element_get_bus(state->playbin);
  gst_bus_add_signal_watch(bus);
  g_signal_connect(G_OBJECT(bus), "message", G_CALLBACK(pom_gst_message), state);

  return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY("OAFIID:PomodoroApplet_Factory", PANEL_TYPE_APPLET, "PomodoroApplet", "0", pomodoro_applet_fill, NULL);
