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

AC_PREREQ([2.65])
AC_INIT([pomodoro-applet], [1.0], [stump@stump.io])
AC_CONFIG_SRCDIR([src/applet-common.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11 foreign])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
CFLAGS="$CFLAGS -W -Wall"

# Checks for libraries.
PKG_CHECK_MODULES([LIBMATEPANELAPPLET], [libmatepanelapplet-4.0])
AC_SUBST([LIBMATEPANELAPPLET_CFLAGS])
AC_SUBST([LIBMATEPANELAPPLET_LIBS])
PKG_CHECK_MODULES([LIBNOTIFY], [libnotify >= 0.7])
AC_SUBST([LIBNOTIFY_CFLAGS])
AC_SUBST([LIBNOTIFY_LIBS])
PKG_CHECK_MODULES([LIBCANBERRA_GTK], [libcanberra-gtk3])
AC_SUBST([LIBCANBERRA_GTK_CFLAGS])
AC_SUBST([LIBCANBERRA_GTK_LIBS])
PKG_CHECK_MODULES([LIBRSVG], [librsvg-2.0 >= 2.36])
AC_SUBST([LIBRSVG_CFLAGS])
AC_SUBST([LIBRSVG_LIBS])

# i18n stuff
IT_PROG_INTLTOOL
GETTEXT_PACKAGE=pomodoro-applet
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [The gettext domain name])
AC_SUBST([GETTEXT_PACKAGE])
AM_GLIB_GNU_GETTEXT

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Set some extra paths.
AS_AC_EXPAND([APPLETDIR], ["${libdir}/pomodoro-applet"])
AC_SUBST([APPLETDIR])
AC_MSG_CHECKING([where mate-panel-applet files go])
# mate-panel doesn't provide this in its .pc file, so we have to just assume...
AS_AC_EXPAND([MATEPANELAPPLETDIR], ["`${PKG_CONFIG} --variable=prefix libmatepanelapplet-4.0`/share/mate-panel/applets"])
AC_SUBST([MATEPANELAPPLETDIR])
AC_MSG_RESULT([$MATEPANELAPPLETDIR])
AC_MSG_CHECKING([where dbus service files go])
AS_AC_EXPAND([SERVICEDIR], ["`${PKG_CONFIG} --variable=session_bus_services_dir dbus-1`"])
AC_SUBST([SERVICEDIR])
AC_MSG_RESULT([$SERVICEDIR])

AC_CONFIG_FILES([Makefile
                 po/Makefile.in
                 src/Makefile
                 src/io.stump.PomodoroApplet.mate-panel-applet.in
                 src/org.mate.panel.applet.PomodoroAppletFactory.service])
AC_OUTPUT
