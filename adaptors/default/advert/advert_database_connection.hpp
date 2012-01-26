//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_CONNECTION_HPP
#define SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_CONNECTION_HPP

#include <soci.h>

#include <boost/assert.hpp>
#include <boost/random/linear_congruential.hpp>

#include <saga/saga/url.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace advert
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

        // detect, whether advert already exists
        bool exists(std::string adname, int& node, bool& found_dir) 
        {
            node = find(adname, found_dir); 
            return node != 0;
        }
        bool exists_entry(std::string adname, int& node, bool throw_on_error = true) 
        {
            node = find_entry(adname, throw_on_error);
            return node != 0;
        }
        bool exists_directory(std::string adname, int& node, bool throw_on_error = true) 
        {
            node = find_directory(adname, throw_on_error); 
            return node != 0;
        }

        // create a new advert
        bool create_entry(std::string const& adname, bool create_parents, int& node);
        bool create_directory(std::string const& adname, bool create_parents, int& node);

        // manage nodes
        bool remove(std::string const& adname, int& node);
        bool remove_directory(std::string const& adname, int& node);
        bool list(std::string const& adname, std::string const& pattern, 
            std::vector<std::string>& entries, int& node);

        // manage attributes
        bool attribute_exists(std::string const& adname, std::string const& key, 
            bool& val, int& node, bool& isdir);
        bool get_attribute(std::string const& adname, std::string const& key, 
            std::string& val, bool& is_vector, int& node, bool& isdir);
        bool set_attribute(std::string const& adname, std::string const& key, 
            std::string const& val, bool is_vector, int& node, bool& isdir);
        bool remove_attribute(std::string const& adname, std::string const& key,
            int& node, bool& isdir);
        bool list_attributes(std::string const& adname, 
            std::vector<std::string>& keys, int& node, bool& isdir);
        bool find_attributes(std::string const& adname, std::string const& kpat, 
            std::string const& vpat, std::vector<std::string>* keys, 
            int& node, bool& isdir, bool count_only = false);
        bool find_attributes(std::string const& adname, std::vector<std::string> const&,
            std::vector<std::string> const&, std::vector<std::string>*,
            int& node, bool& isdir, bool count_only = false);

        void install_function(char const* name, void *data);
        void uninstall_function(char const* name);

        int find_node(std::string const& url);
        int find_entry(std::string const& url, bool throw_on_error = true);
        int find_directory(std::string const& url, bool throw_on_error = true);
        int find(std::string const& url, bool& found_dir, 
            bool create = false, bool create_parents = false, 
            bool create_dir = false);

        bool verify_node_exists(std::string const& adname, int node_id);

        // store/retrieve data
        bool store_data(std::string const& url, std::string const& data, int& node);
        bool retrieve_data(std::string const& url, std::string& data, int& node);

        db_type get_dbtype() const { return type_; }
        std::string const& get_connectstring() const { return connect_string_; }

    protected:
        void init_database(std::string const& connect);
        bool init_tables();
        void init_backend();
        bool error_is_resolvable();

        // wrapped functions
        bool remove_db(std::string const& adname, int& node);
        bool remove_directory_db(std::string const& adname, int& node);
        bool create_entry_db(std::string const& adname, bool create_parents, int& node);
        bool create_directory_db(std::string const& adname, bool create_parents, int& node);

        bool list_db(std::string const& adname, std::string pattern, 
            std::vector<std::string>& entries, int& node);

        bool list_attributes_db(std::string const& adname, 
            std::vector<std::string>& keys, int& node, bool& isdir);
        bool find_attributes_db(std::string const& adname, 
            std::string kpat, std::string vpat, 
            std::vector<std::string>* keys, int& node, bool& isdir,
            bool count_only = false);
        bool find_attributes_db_v(std::string const& adname, 
            std::vector<std::string> const&, std::vector<std::string> const&, 
            std::vector<std::string>*, int& node, bool& isdir,
            bool count_only = false);
        bool attribute_exists_db(std::string const& adname, std::string key, 
            bool& val, int& node, bool& isdir);
        bool get_attribute_db(std::string const& adname, std::string key, 
            std::string& val, bool& is_vector, int& node, bool& isdir);
        bool set_attribute_db(std::string const& adname, std::string key, 
            std::string val, bool is_vector, int& node, bool& isdir);
        bool remove_attribute_db(std::string const& adname, std::string key, 
            int& node, bool& isdir);
        bool store_data_db(std::string const& url, std::string data, int& node);
        bool retrieve_data_db(std::string const& url, std::string& data, int& node);

#if defined(SAGA_DEFAULT_ADVERT_ADAPTOR_USE_SQLITE3)
        void init_backend_sqlite3();
        void init_tables_sqlite3();
        void install_function_sqlite3(char const* name, void *data);
        void uninstall_function_sqlite3(char const* name);
        bool error_is_resolvable_sqlite3();
#endif

        bool get_node_id(std::string entry, int parent_id, int& node_id,
            bool& is_dir);
        bool remove_node(std::string const& adname, int node_id, bool is_entry);

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
#define SAGA_ADVERTDB_VERSION_100    0x0100
#define SAGA_ADVERTDB_SUBMINORMASK   0x00FF

#define SAGA_ADVERTDB_LASTVERSION    SAGA_ADVERTDB_VERSION_100

///////////////////////////////////////////////////////////////////////////////
}   // advert

#endif // !SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_CONNECTION_HPP

