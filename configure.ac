#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([life], [1.6.0], [aiden.woodruff@gmail.com])
AC_CONFIG_SRCDIR([src/life.c])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CPP
AC_C_TYPEOF

# Checks for libraries.
AC_CHECK_LIB([ncurses], [initscr])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([memset strtol])

AC_CONFIG_FILES([docs/Makefile docs/life.6 test/Makefile src/Makefile Makefile])
AM_INIT_AUTOMAKE
AC_OUTPUT
