# pomodoro-applet: timer for the Pomodoro Technique
# Copyright (C) 2010-2012, 2014, 2020 John Stumpo
# Copyright (C) 2013 José Luis Segura Lucas
# Copyright (C) 2015, 2017 Balló György
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
AC_INIT([pomodoro-applet], [1.0], [projects at stump dot io])
AC_CONFIG_SRCDIR([src/applet-common.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11 foreign])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT([dlopen disable-static])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
CFLAGS="$CFLAGS -W -Wall"
gl_VISIBILITY
CFLAGS="$CFLAGS $CFLAG_VISIBILITY"

# Checks for libraries.
AC_ARG_WITH([gnome], AS_HELP_STRING([--without-gnome],
  [ignore presence of GNOME libraries and do not build GNOME support (default
   is autodetect; alternatively, pass --with-gnome to refuse to build without
   GNOME support)]))
AS_IF([test x"$with_gnome" != x"no"], [
  AC_ARG_WITH([libgnome-panel], AS_HELP_STRING([--without-libgnome-panel],
    [ignore presence of libgnome-panel and only look for libpanel-applet to use
     for GNOME support (default is to prefer libgnome-panel and fall back to
     libpanel-applet; alternatively, pass --with-libgnome-panel to refuse to
     build a libpanel-applet based GNOME applet)]))
  AS_IF([test x"$with_libgnome_panel" != x"no"], [
    PKG_CHECK_MODULES([LIBGNOMEPANEL], [libgnome-panel], [
      have_gnome=yes
      have_libgnome_panel=yes
      AC_SUBST([LIBGNOMEPANEL_CFLAGS])
      AC_SUBST([LIBGNOMEPANEL_LIBS])
    ], [
      have_libgnome_panel=no
    ])
  ], [
    have_libgnome_panel=no
  ])
  AS_IF([test x"$have_libgnome_panel" = x"no"], [
    AS_IF([test x"$with_libgnome_panel" = x"yes"], [
      AC_MSG_ERROR([libgnome-panel not found but support was explicitly requested])
    ])
    PKG_CHECK_MODULES([LIBPANELAPPLET], [libpanel-applet], [
      have_gnome=yes
      AC_SUBST([LIBPANELAPPLET_CFLAGS])
      AC_SUBST([LIBPANELAPPLET_LIBS])
    ], [
      have_gnome=no
    ])
  ])
], [
  have_gnome=no
  have_libgnome_panel=no
])
AS_IF([test x"$have_gnome" = x"no"], [
  AS_IF([test x"$with_gnome" = x"yes"], [
    AC_MSG_ERROR([GNOME not found but support was explicitly requested])
  ])
])
AM_CONDITIONAL([HAVE_GNOME], [test x"$have_gnome" = x"yes"])
AM_CONDITIONAL([HAVE_LIBGNOMEPANEL], [test x"$have_libgnome_panel" = x"yes"])

AC_ARG_WITH([mate], AS_HELP_STRING([--without-mate],
  [ignore presence of MATE libraries and do not build MATE support (default
   is autodetect; alternatively, pass --with-mate to refuse to build without
   MATE support)]))
AS_IF([test x"$with_mate" != x"no"], [
  PKG_CHECK_MODULES([LIBMATEPANELAPPLET], [libmatepanelapplet-4.0], [
    have_mate=yes
    AC_SUBST([LIBMATEPANELAPPLET_CFLAGS])
    AC_SUBST([LIBMATEPANELAPPLET_LIBS])
  ], [
    have_mate=no
  ])
], [
  have_mate=no
])
AS_IF([test x"$have_mate" = x"no"], [
  AS_IF([test x"$with_mate" = x"yes"], [
    AC_MSG_ERROR([MATE not found but support was explicitly requested])
  ])
])
AM_CONDITIONAL([HAVE_MATE], [test x"$have_mate" = x"yes"])

AS_IF([test x"$have_gnome$have_mate" = x"nono"], [
  AC_MSG_ERROR([Neither GNOME nor MATE applets will be built; no point in continuing])
])

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
# First, a little error-checking wrapper around PKG_CHECK_VAR.
AC_DEFUN([POM_PKG_REQUIRE_VAR], [
  PKG_CHECK_VAR([$1], [$2], [$3], [
    $4
  ], [
    $5
    AC_MSG_ERROR([could not retrieve variable $3 for pkg-config package $2])
  ])
])

# Then, the real work.
AS_AC_EXPAND([APPLETDIR], ["${libdir}/pomodoro-applet"])
AC_SUBST([APPLETDIR])
AS_IF([test x"$have_gnome" = x"yes"], [
  AS_IF([test x"$have_libgnome_panel" = x"yes"], [
    AC_MSG_CHECKING([where gnome-panel modules go])
    POM_PKG_REQUIRE_VAR([MODULESDIR], [libgnome-panel], [modulesdir])
    AC_SUBST([MODULESDIR])
    AC_MSG_RESULT([$MODULESDIR])
  ], [
    AC_MSG_CHECKING([where panel-applet files go])
    POM_PKG_REQUIRE_VAR([PANELAPPLETDIR], [libpanel-applet], [libpanel_applet_dir])
    AC_SUBST([PANELAPPLETDIR])
    AC_MSG_RESULT([$PANELAPPLETDIR])
  ])
])
AS_IF([test x"$have_mate" = x"yes"], [
  AC_MSG_CHECKING([where mate-panel-applet files go])
  # mate-panel doesn't provide this in its .pc file, so we have to just assume...
  POM_PKG_REQUIRE_VAR([MATEPANELPREFIX], [libmatepanelapplet-4.0], [prefix])
  AS_AC_EXPAND([MATEPANELAPPLETDIR], ["${MATEPANELPREFIX}/share/mate-panel/applets"])
  AC_SUBST([MATEPANELAPPLETDIR])
  AC_MSG_RESULT([$MATEPANELAPPLETDIR])
])

# Let the MATE applet optionally be built to run in-process.
AC_ARG_ENABLE([mate-in-process], AS_HELP_STRING([--enable-mate-in-process],
  [build the MATE panel applet to run in-process]))
AS_IF([test x"$enable_mate_in_process" = x"yes"], [
  MATE_IN_PROCESS=true
  MATE_BINARY_SUFFIX=.so
], [
  MATE_IN_PROCESS=false
  MATE_BINARY_SUFFIX=
  # We only need to know this if we're building an out-of-process MATE applet.
  AC_MSG_CHECKING([where dbus service files go])
  POM_PKG_REQUIRE_VAR([SERVICEDIR], [dbus-1], [session_bus_services_dir])
  AC_SUBST([SERVICEDIR])
  AC_MSG_RESULT([$SERVICEDIR])
])
AC_SUBST([MATE_IN_PROCESS])
AC_SUBST([MATE_BINARY_SUFFIX])
AM_CONDITIONAL([BUILD_MATE_IN_PROCESS], [test x"$MATE_IN_PROCESS" = x"true"])

AC_CONFIG_FILES([Makefile
                 po/Makefile.in
                 src/Makefile
                 src/io.stump.PomodoroApplet.panel-applet.in
                 src/io.stump.PomodoroApplet.mate-panel-applet.in
                 src/org.mate.panel.applet.PomodoroAppletFactory.service])
AC_OUTPUT

AS_ECHO_N(["
        To be built
        ===========

        GNOME applet:      $have_gnome"])

AS_IF([test x"$have_gnome" = x"yes"], [
  AS_IF([test x"$have_libgnome_panel" = x"yes"], [
    AS_ECHO_N([', using libgnome-panel'])
  ], [
    AS_ECHO_N([', using libpanel-applet'])
  ])
])

AS_ECHO_N(["
        MATE applet:       $have_mate"])

AS_IF([test x"$have_mate" = x"yes"], [
  AS_IF([test x"$MATE_IN_PROCESS" = x"true"], [
    AS_ECHO_N([', to run in-process'])
  ], [
    AS_ECHO_N([', to run out-of-process'])
  ])
])

AS_ECHO(["
"])
