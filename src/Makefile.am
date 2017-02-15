# pomodoro-applet: timer for the Pomodoro Technique
# Copyright (C) 2010-2012 John Stumpo
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

pomodoro_applet_libdir=${pkglibdir}/${LIBPANEL_APPLET_API_VERSION}
pomodoro_applet_lib_LTLIBRARIES=libpomodoro-applet.la

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = pomodoro.svg

libpomodoro_applet_la_SOURCES = pomodoro.c
libpomodoro_applet_la_CPPFLAGS = @LIBPANELAPPLET_CFLAGS@ @LIBNOTIFY_CFLAGS@ @GSTREAMER_CFLAGS@ @LIBRSVG_CFLAGS@ -DPKGDATADIR='"$(pkgdatadir)"' -DPIXMAPDIR='"$(pixmapdir)"' -DGNOMELOCALEDIR='"$(localedir)"'
libpomodoro_applet_la_LIBADD = @LIBPANELAPPLET_LIBS@ @LIBNOTIFY_LIBS@ @GSTREAMER_LIBS@ @LIBRSVG_LIBS@

panelappletdir = $(PANELAPPLETDIR)
panelapplet_DATA = com.jstump.PomodoroApplet.panel-applet

# Adapted from similar rules in intltool.m4; hopefully it'll get
# included in intltool itself soon enough...
%.panel-applet:   %.panel-applet.in   $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; $(INTLTOOL_V_MERGE)LC_ALL=C $(INTLTOOL_MERGE) $(INTLTOOL_V_MERGE_OPTIONS) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po $< $@

pkgdata_DATA = timerexpired.ogg

EXTRA_DIST = timerexpired.ogg pomodoro.svg
CLEANFILES = com.jstump.PomodoroApplet.panel-applet
