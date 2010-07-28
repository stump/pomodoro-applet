#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([pomodoro-applet], [0.0], [stump@jstump.com])
AC_CONFIG_SRCDIR([pomodoro.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([LIBPANELAPPLET], [libpanelapplet-2.0])
AC_SUBST([LIBPANELAPPLET_CFLAGS])
AC_SUBST([LIBPANELAPPLET_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
