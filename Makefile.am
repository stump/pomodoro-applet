bin_PROGRAMS = pomodoro-applet

pomodoro_applet_SOURCES = pomodoro.c
pomodoro_applet_CFLAGS = @LIBPANELAPPLET_CFLAGS@ @LIBNOTIFY_CFLAGS@ @GSTREAMER_CFLAGS@
pomodoro_applet_LDADD = @LIBPANELAPPLET_LIBS@ @LIBNOTIFY_LIBS@ @GSTREAMER_LIBS@

serverdir = $(libdir)/bonobo/servers
server_DATA = PomodoroApplet.server

pkgdata_DATA = timerexpired.ogg

EXTRA_DIST = PomodoroApplet.server timerexpired.ogg
