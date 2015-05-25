// file      : odb/mysql/connection.hxx
// copyright : Copyright (c) 2005-2015 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_MYSQL_CONNECTION_HXX
#define ODB_MYSQL_CONNECTION_HXX

#include <odb/pre.hxx>

#include <vector>

#include <odb/connection.hxx>

#include <odb/mysql/mysql.hxx>
#include <odb/mysql/version.hxx>
#include <odb/mysql/forward.hxx>
#include <odb/mysql/query.hxx>
#include <odb/mysql/tracer.hxx>
#include <odb/mysql/transaction-impl.hxx>
#include <odb/mysql/auto-handle.hxx>

#include <odb/details/shared-ptr.hxx>
#include <odb/details/unique-ptr.hxx>

#include <odb/mysql/details/export.hxx>

namespace odb
{
  namespace mysql
  {
    class statement_cache;

    class connection;
    typedef details::shared_ptr<connection> connection_ptr;

    class LIBODB_MYSQL_EXPORT connection: public odb::connection
    {
    public:
      typedef mysql::statement_cache statement_cache_type;
      typedef mysql::database database_type;

      virtual
      ~connection ();

      connection (database_type&);
      connection (database_type&, MYSQL* handle);

      database_type&
      database ()
      {
        return db_;
      }

    public:
      virtual transaction_impl*
      begin ();

    public:
      using odb::connection::execute;

      virtual unsigned long long
      execute (const char* statement, std::size_t length);

      // Query preparation.
      //
    public:
      template <typename T>
      prepared_query<T>
      prepare_query (const char* name, const char*);

      template <typename T>
      prepared_query<T>
      prepare_query (const char* name, const std::string&);

      template <typename T>
      prepared_query<T>
      prepare_query (const char* name, const mysql::query_base&);

      template <typename T>
      prepared_query<T>
      prepare_query (const char* name, const odb::query_base&);

      // SQL statement tracing.
      //
    public:
      typedef mysql::tracer tracer_type;

      void
      tracer (tracer_type& t)
      {
        odb::connection::tracer (t);
      }

      void
      tracer (tracer_type* t)
      {
        odb::connection::tracer (t);
      }

      using odb::connection::tracer;

    public:
      bool
      failed () const
      {
        return failed_;
      }

      void
      mark_failed ()
      {
        failed_ = true;
      }

      // Ping the server to make sure the connection is still alive. Return
      // true if successful, mark the connection as failed and return false
      // otherwise. This function can also throw database_exception.
      //
      bool
      ping ();

    public:
      MYSQL*
      handle ()
      {
        return handle_;
      }

      statement_cache_type&
      statement_cache ()
      {
        return *statement_cache_;
      }

    public:
      statement*
      active ()
      {
        return active_;
      }

      void
      active (statement* s)
      {
        active_ = s;

        if (s == 0 && stmt_handles_.size () > 0)
          free_stmt_handles ();
      }

      // Cancel and clear the active statement, if any.
      //
      void
      clear ()
      {
        if (active_ != 0)
          clear_ ();
      }

    public:
      MYSQL_STMT*
      alloc_stmt_handle ();

      void
      free_stmt_handle (auto_handle<MYSQL_STMT>&);

    private:
      connection (const connection&);
      connection& operator= (const connection&);

    private:
      void
      free_stmt_handles ();

      void
      clear_ ();

    private:
      friend class transaction_impl; // invalidate_results()

    private:
      // Needed to break the circular connection-database dependency
      // (odb::connection has the odb::database member).
      //
      database_type& db_;

      bool failed_;

      MYSQL mysql_;
      auto_handle<MYSQL> handle_;

      statement* active_;

      // Keep statement_cache_ after handle_ so that it is destroyed before
      // the connection is closed.
      //
      details::unique_ptr<statement_cache_type> statement_cache_;

      // List of "delayed" statement handles to be freed next time there
      // is no active statement.
      //
      typedef std::vector<MYSQL_STMT*> stmt_handles;
      stmt_handles stmt_handles_;
    };
  }
}

#include <odb/mysql/connection.ixx>

#include <odb/post.hxx>

#endif // ODB_MYSQL_CONNECTION_HXX
