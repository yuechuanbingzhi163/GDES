// file      : odb/boost/exception.hxx
// copyright : Copyright (c) 2005-2015 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_BOOST_EXCEPTION_HXX
#define ODB_BOOST_EXCEPTION_HXX

#include <odb/pre.hxx>

#include <odb/exceptions.hxx>

#include <odb/boost/details/export.hxx>

namespace odb
{
  namespace boost
  {
    struct LIBODB_BOOST_EXPORT exception: odb::exception
    {
      virtual const char*
      what () const throw () = 0;
    };
  }
}

#include <odb/post.hxx>

#endif // ODB_BOOST_EXCEPTION_HXX
