# pomodoro-applet: timer for the Pomodoro Technique
# Copyright (C) 2010-2012, 2014, 2020 John Stumpo
# Copyright (C) 2013 José Luis Segura Lucas
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

appletdir = $(APPLETDIR)
applet_PROGRAMS =
if HAVE_GNOME
applet_PROGRAMS += pomodoro-applet-gnome
endif
if HAVE_MATE
applet_PROGRAMS += pomodoro-applet-mate
endif

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = pomodoro.svg

if HAVE_GNOME
pomodoro_applet_gnome_SOURCES = applet-common.c applet-common.h applet-gnome.c
pomodoro_applet_gnome_CPPFLAGS = @LIBPANELAPPLET_CFLAGS@ @LIBNOTIFY_CFLAGS@ @LIBCANBERRA_GTK_CFLAGS@ @LIBRSVG_CFLAGS@ -DPKGDATADIR='"$(pkgdatadir)"' -DPIXMAPDIR='"$(pixmapdir)"' -DGNOMELOCALEDIR='"$(localedir)"'
pomodoro_applet_gnome_LDADD = @LIBPANELAPPLET_LIBS@ @LIBNOTIFY_LIBS@ @LIBCANBERRA_GTK_LIBS@ @LIBRSVG_LIBS@

panelappletdir = $(PANELAPPLETDIR)
panelapplet_DATA = io.stump.PomodoroApplet.panel-applet
endif

if HAVE_MATE
pomodoro_applet_mate_SOURCES = applet-common.c applet-common.h applet-mate.c
pomodoro_applet_mate_CPPFLAGS = @LIBMATEPANELAPPLET_CFLAGS@ @LIBNOTIFY_CFLAGS@ @LIBCANBERRA_GTK_CFLAGS@ @LIBRSVG_CFLAGS@ -DPKGDATADIR='"$(pkgdatadir)"' -DPIXMAPDIR='"$(pixmapdir)"' -DMATELOCALEDIR='"$(localedir)"'
pomodoro_applet_mate_LDADD = @LIBMATEPANELAPPLET_LIBS@ @LIBNOTIFY_LIBS@ @LIBCANBERRA_GTK_LIBS@ @LIBRSVG_LIBS@

matepanelappletdir = $(MATEPANELAPPLETDIR)
matepanelapplet_DATA = io.stump.PomodoroApplet.mate-panel-applet
endif

# Adapted from similar rules in intltool.m4; hopefully it'll get
# included in intltool itself soon enough...
%.panel-applet:   %.panel-applet.in   $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; $(INTLTOOL_V_MERGE)LC_ALL=C $(INTLTOOL_MERGE) $(INTLTOOL_V_MERGE_OPTIONS) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po $< $@
%.mate-panel-applet:   %.mate-panel-applet.in   $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; $(INTLTOOL_V_MERGE)LC_ALL=C $(INTLTOOL_MERGE) $(INTLTOOL_V_MERGE_OPTIONS) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po $< $@

servicedir = $(SERVICEDIR)
service_DATA =
if HAVE_GNOME
service_DATA += org.gnome.panel.applet.PomodoroAppletFactory.service
endif
if HAVE_MATE
service_DATA += org.mate.panel.applet.PomodoroAppletFactory.service
endif

pkgdata_DATA = timerexpired.ogg

EXTRA_DIST = timerexpired.ogg pomodoro.svg
CLEANFILES = io.stump.PomodoroApplet.panel-applet io.stump.PomodoroApplet.mate-panel-applet
