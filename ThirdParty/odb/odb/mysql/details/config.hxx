// file      : odb/mysql/details/config.hxx
// copyright : Copyright (c) 2005-2015 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_DETAILS_CONFIG_HXX
#define ODB_MYSQL_DETAILS_CONFIG_HXX

// no pre

#ifdef _MSC_VER
#  if !defined(LIBODB_MYSQL_INCLUDE_SHORT) && !defined (LIBODB_MYSQL_INCLUDE_LONG)
#    define LIBODB_MYSQL_INCLUDE_SHORT 1
#  endif
#elif defined(ODB_COMPILER)
#  error libodb-mysql header included in odb-compiled header
#else
#  include <odb/mysql/details/config.h>
#endif

// no post

#endif // ODB_MYSQL_DETAILS_CONFIG_HXX
