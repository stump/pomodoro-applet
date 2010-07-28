#include <gtk/gtk.h>
#include <panel-applet.h>

#include <string.h>

static gboolean pomodoro_applet_fill(PanelApplet* applet, const gchar* iid, gpointer data)
{
  (void) data;

  if (strcmp(iid, "OAFIID:PomodoroApplet") != 0)
    return FALSE;

  gtk_container_add(GTK_CONTAINER(applet), gtk_label_new("Pomodoro"));

  gtk_widget_show_all(GTK_WIDGET(applet));
  return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY("OAFIID:PomodoroApplet_Factory", PANEL_TYPE_APPLET, "PomodoroApplet", "0", pomodoro_applet_fill, NULL);
