bin_PROGRAMS = pomodoro-applet

pomodoro_applet_SOURCES = pomodoro.c
pomodoro_applet_CFLAGS = @LIBPANELAPPLET_CFLAGS@ @LIBNOTIFY_CFLAGS@
pomodoro_applet_LDADD = @LIBPANELAPPLET_LIBS@ @LIBNOTIFY_LIBS@

serverdir = $(libdir)/bonobo/servers
server_DATA = PomodoroApplet.server
EXTRA_DIST = PomodoroApplet.server
