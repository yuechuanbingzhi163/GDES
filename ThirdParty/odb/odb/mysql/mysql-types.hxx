// file      : odb/mysql/mysql-types.hxx
// copyright : Copyright (c) 2005-2015 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_MYSQL_TYPES_HXX
#define ODB_MYSQL_MYSQL_TYPES_HXX

#include <odb/mysql/details/config.hxx>

typedef char my_bool;
typedef struct st_mysql_bind MYSQL_BIND;

#ifdef LIBODB_MYSQL_INCLUDE_SHORT
#  include <mysql_time.h>
#else
#  include <mysql/mysql_time.h>
#endif

#endif // ODB_MYSQL_MYSQL_TYPES_HXX
