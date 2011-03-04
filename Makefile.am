# pomodoro-applet: timer for the Pomodoro Technique
# Copyright (C) 2010-2011 John Stumpo
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

appletdir = $(libdir)/pomodoro-applet
applet_PROGRAMS = pomodoro-applet

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = pomodoro.svg

pomodoro_applet_SOURCES = pomodoro.c
pomodoro_applet_CPPFLAGS = @LIBPANELAPPLET_CFLAGS@ @LIBNOTIFY_CFLAGS@ @GSTREAMER_CFLAGS@ @LIBRSVG_CFLAGS@ -DPKGDATADIR='"$(pkgdatadir)"' -DPIXMAPDIR='"$(pixmapdir)"'
pomodoro_applet_LDADD = @LIBPANELAPPLET_LIBS@ @LIBNOTIFY_LIBS@ @GSTREAMER_LIBS@ @LIBRSVG_LIBS@

serverdir = $(libdir)/bonobo/servers
server_DATA = PomodoroApplet.server

PomodoroApplet.server : $(srcdir)/PomodoroApplet.server.in Makefile
	$(AM_V_GEN)sed -e "s|\@APPLETDIR\@|$(appletdir)|" $< >$@

pkgdata_DATA = timerexpired.ogg

EXTRA_DIST = PomodoroApplet.server.in timerexpired.ogg pomodoro.svg
CLEANFILES = PomodoroApplet.server
