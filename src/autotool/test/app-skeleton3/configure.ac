
AC_INIT([App Skeleton], [2.0.0], [philip.chimento@gmail.com])
AC_CONFIG_SRCDIR([src/hello-world.c])

AM_INIT_AUTOMAKE([-Wall foreign])
AM_SILENT_RULES([yes])

AC_PROG_CC

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTK], [gtk+-3.0])

AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 pixmaps/Makefile
])

AC_OUTPUT
