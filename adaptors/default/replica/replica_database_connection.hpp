//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_DEFAULT_REPLICA_DATABASE_CONNECTION_HPP
#define SAGA_ADAPTORS_DEFAULT_REPLICA_DATABASE_CONNECTION_HPP

#include <soci.h>

#include <boost/assert.hpp>
#include <boost/random/linear_congruential.hpp>

#include <saga/saga/url.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace replica
{
    ///////////////////////////////////////////////////////////////////////////
    class database_connection
    {
    public:
        enum db_type {
            db_unknown = -1,
            db_sqlite = 0,
            db_postgresql = 1,
            db_mysql = 2
        };

        database_connection(db_type type, std::string const & connectString);
        ~database_connection() { close(false); }

        void close(bool verify = true);
        bool is_open() const { return opened; }
        void verify_status();

        // detect, whether replica entry already exists
        bool exists(std::string lfn) 
            {  bool found_dir = false; return find(lfn, found_dir) != 0; }
        bool exists_entry(std::string lfn, bool throw_on_error = true) 
            { return find_entry(lfn, throw_on_error) != 0; }
        bool exists_directory(std::string lfn, bool throw_on_error = true) 
            { return find_directory(lfn, throw_on_error) != 0; }

        // create a new replica entry
        bool create_entry(std::string const& lfn, bool create_parents);
        bool create_directory(std::string const& lfn, bool create_parents);

        // manage nodes
        bool remove(std::string const& lfn);
        bool remove_directory(std::string const& lfn);
        bool list(std::string const& lfn, std::string const& pattern, 
            std::vector<std::string>& entries);

        // manage entries
        bool get_entries(std::string const& lfn, std::vector<std::string>& locations);
        bool add_entry(std::string const& lfn, std::string const& location);
        bool remove_entry(std::string const& lfn, std::string const& location);
        bool update_entry(std::string const& lfn, std::string const& oldlocation, 
            std::string const& newlocation);

        // manage attributes
        bool attribute_exists(std::string const& lfn, std::string const& key, 
            bool& val);
        bool get_attribute(std::string const& lfn, std::string const& key, 
            std::string& val, bool& is_vector);
        bool set_attribute(std::string const& lfn, std::string const& key, 
            std::string const& val, bool is_vector);
        bool remove_attribute(std::string const& lfn, std::string const& key);
        bool list_attributes(std::string const& lfn, 
            std::vector<std::string>& keys);
        bool find_attributes(std::string const& lfn, std::string const& kpat, 
            std::string const& vpat, std::vector<std::string>* keys, 
            bool count_only = false);
        bool find_attributes(std::string const& lfn, std::vector<std::string> const&,
            std::vector<std::string> const&, std::vector<std::string>*,
            bool count_only = false);

        void install_function(char const* name, void *data);
        void uninstall_function(char const* name);

        int find_entry(std::string url, bool throw_on_error = true);
        int find_directory(std::string url, bool throw_on_error = true);
        int find(std::string url, bool& found_dir, 
            bool create = false, bool create_parents = false, 
            bool create_dir = false);

        db_type get_dbtype() const { return type_; }
        std::string const& get_connectstring() const { return connect_string_; }

    protected:
        void init_database(std::string const& connect);
        bool init_tables();
        void init_backend();
        bool error_is_resolvable();

        // wrapped functions
        bool remove_db(std::string const& adname);
        bool remove_directory_db(std::string const& adname);
        bool create_entry_db(std::string const& adname, bool create_parents);
        bool create_directory_db(std::string const& adname, bool create_parents);

        bool list_db(std::string const& adname, std::string pattern, 
            std::vector<std::string>& entries);
        bool list_attributes_db(std::string const& adname, 
            std::vector<std::string>& keys);
        bool find_attributes_db(std::string const& adname, 
            std::string kpat, std::string vpat, 
            std::vector<std::string>* keys, bool count_only = false);
        bool find_attributes_db_v(std::string const& adname, 
            std::vector<std::string> const&, std::vector<std::string> const&, 
            std::vector<std::string>*, bool count_only = false);
        bool attribute_exists_db(std::string const& adname, std::string key, 
            bool& val);
        bool get_attribute_db(std::string const& adname, std::string key, 
            std::string& val, bool& is_vector);
        bool set_attribute_db(std::string const& adname, std::string key, 
            std::string val, bool is_vector);
        bool remove_attribute_db(std::string const& adname, std::string key);

        bool get_entries_db(std::string const& lfn, std::vector<std::string>& locations);
        bool add_entry_db(std::string const& lfn, std::string location);
        bool remove_entry_db(std::string const& lfn, std::string location);
        bool update_entry_db(std::string const& lfn, std::string oldlocation, 
            std::string newlocation);

#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        void init_backend_sqlite3();
        void init_tables_sqlite3();
        void install_function_sqlite3(char const* name, void *data);
        void uninstall_function_sqlite3(char const* name);
        bool error_is_resolvable_sqlite3();
#endif

        bool get_node_id(std::string entry, int parent_id, int& node_id,
            bool& is_dir);
        bool remove_node(std::string const& lfn, int node_id, bool is_entry);

        template <typename F>
        bool execute(F f, std::string const& name);

    private:
        SOCI::Session sql_;
        db_type type_;
        std::string connect_string_;
        bool opened;
        boost::minstd_rand rand_;
    };

///////////////////////////////////////////////////////////////////////////////
#define SAGA_REPLICADB_VERSION_100    0x0100
#define SAGA_REPLICADB_SUBMINORMASK   0x00FF

#define SAGA_REPLICADB_LASTVERSION    SAGA_REPLICADB_VERSION_100

///////////////////////////////////////////////////////////////////////////////
}   // replica

#endif

