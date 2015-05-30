# file      : configure.ac
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

AC_PREREQ(2.60)
AC_INIT([libodb], [2.4.0], [odb-users@codesynthesis.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([odb/version.hxx])

AM_INIT_AUTOMAKE([-Wall -Werror foreign nostdinc subdir-objects dist-bzip2 dist-zip tar-ustar])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR]) # Required by automake 1.12.

LT_INIT([win32-dll])

AC_CANONICAL_HOST

# Check for C++ compiler and use it to compile the tests.
#
AC_PROG_CXX
AC_LANG(C++)

# Create the libtool executable so that we can use it in further tests.
#
LT_OUTPUT

# Check for Windows.
#
win32=no
case $host_os in
  windows* | mingw*)
    win32=yes
    ;;
esac

AM_CONDITIONAL([ODB_WIN32], [test x$win32 = xyes])

# Check for threads.
#
THREADS

AM_CONDITIONAL([ODB_THREADS_NONE], [test x$threads = xnone])
AM_CONDITIONAL([ODB_THREADS_WIN32], [test x$threads = xwin32])
AM_CONDITIONAL([ODB_THREADS_POSIX], [test x$threads = xposix])

AS_IF([test x$threads = xnone], AC_DEFINE([ODB_THREADS_NONE], [1], [Have no threads.]))
AS_IF([test x$threads = xwin32], AC_DEFINE([ODB_THREADS_WIN32], [1], [Have Win32 threads.]))
AS_IF([test x$threads = xposix], AC_DEFINE([ODB_THREADS_POSIX], [1], [Have POSIX threads.]))

AS_IF([test x$threads_thread_keyword = xyes], AC_DEFINE([ODB_THREADS_TLS_KEYWORD], [1], [Have __thread keyword.]))

# Define LIBODB_STATIC_LIB if we are build static library on certain platforms.
#
STATIC_LIB([LIBODB_STATIC_LIB], [Static library interface.])

# Allow the user to specify the pkgconfig directory.
#
PKGCONFIG

# Check if we should disable rpath.
#
DISABLE_RPATH

# Output.
#
AC_CONFIG_HEADERS([odb/config.h odb/details/config.h])
AC_CONFIG_FILES([libodb.pc Makefile odb/Makefile])
AC_OUTPUT
