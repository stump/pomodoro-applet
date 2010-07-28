#include <gtk/gtk.h>
#include <panel-applet.h>
#include <libnotify/notify.h>

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
};

static void pom_notify(const gchar* summary, const gchar* body)
{
  NotifyNotification* note = notify_notification_new(summary, body, NULL, NULL);
  notify_notification_show(note, NULL);
  g_object_unref(note);
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
        state->state = POM_BREAK;
        state->seconds_left = BREAK_SECONDS;
        pom_notify("Pomodoro", "Break time!");
        break;
      case POM_BREAK:
        state->state = POM_STOPPED;
        pom_notify("Pomodoro", "The break period is over.");
        break;
    }
  }
  pom_update_label(state);
  return (state->state != POM_STOPPED);
}

static void pom_button_pressed(GtkWidget* ebox, GdkEventButton* event, struct pom_state* state)
{
  switch (state->state) {
    case POM_STOPPED:
      state->state = POM_WORK;
      state->seconds_left = WORK_SECONDS;
      g_timeout_add(1000, pom_second, state);
      break;
    case POM_WORK:
      state->state = POM_BREAK;
      state->seconds_left = BREAK_SECONDS;
      pom_notify("Pomodoro", "The current pomodoro was aborted.");
      break;
    case POM_BREAK:
      state->state = POM_STOPPED;
      break;
  }
  pom_update_label(state);
}

static gboolean pomodoro_applet_fill(PanelApplet* applet, const gchar* iid, gpointer data)
{
  struct pom_state* state;
  GtkWidget* ebox;
  (void) data;

  if (strcmp(iid, "OAFIID:PomodoroApplet") != 0)
    return FALSE;

  if (!notify_is_initted())
    notify_init("Pomodoro");

  state = g_malloc0(sizeof(struct pom_state));
  state->label = gtk_label_new("Pomodoro");
  ebox = gtk_event_box_new();
  gtk_container_add(GTK_CONTAINER(ebox), state->label);
  gtk_container_add(GTK_CONTAINER(applet), ebox);

  g_signal_connect(G_OBJECT(ebox), "button-press-event", G_CALLBACK(pom_button_pressed), state);
  gtk_widget_show_all(GTK_WIDGET(applet));
  return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY("OAFIID:PomodoroApplet_Factory", PANEL_TYPE_APPLET, "PomodoroApplet", "0", pomodoro_applet_fill, NULL);
