//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_DEFAULT_REPLICA_DATABASE_HPP
#define SAGA_ADAPTORS_DEFAULT_REPLICA_DATABASE_HPP

#include <boost/assert.hpp>
#include <saga/saga/url.hpp>

#include "default_replica_adaptor.hpp"
#include "replica_database_connection.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace replica
{
    ///////////////////////////////////////////////////////////////////////////
    // forward declaration only
    class database_scope;

    ///////////////////////////////////////////////////////////////////////////
    class database
    {
    public:
        database(database_connection::db_type type, std::string const & connect_string)
          : type_(type), connect_string_(connect_string)
        {}
        ~database() 
        {}

        void verify_status() { connection_->verify_status(); }
        void close(bool verify = true)
            { if (verify) verify_status(); }

        // detect, whether advert already exists
        bool exists(std::string const& lfn) 
            { return connection_->exists(lfn); }
        bool exists_entry(std::string const& lfn, bool throw_on_error = true) 
            { return connection_->exists_entry(lfn, throw_on_error); }
        bool exists_directory(std::string const& lfn, bool throw_on_error = true) 
            { return connection_->exists_directory(lfn, throw_on_error); }

        int find(std::string const& url, bool& found_dir)
            { return connection_->find(url, found_dir); }

        // create a new advert
        bool create_entry(std::string const& lfn, bool create_parents)
            { return connection_->create_entry(lfn, create_parents); }
        bool create_directory(std::string const& lfn, bool create_parents)
            { return connection_->create_directory(lfn, create_parents); }

        // manage nodes
        bool remove(std::string const& lfn)
            { return connection_->remove(lfn); }
        bool remove_directory(std::string const& lfn)
            { return connection_->remove_directory(lfn); }
        bool list(std::string const& lfn, std::string const& pattern, 
                std::vector<std::string>& entries)
            { return connection_->list(lfn, pattern, entries); }

        // manage attributes
        bool attribute_exists(std::string const& lfn, std::string const& key, 
                bool& val)
            { return connection_->attribute_exists(lfn, key, val); }
        bool get_attribute(std::string const& lfn, std::string const& key, 
                std::string& val, bool& is_vector)
            { return connection_->get_attribute(lfn, key, val, is_vector); }
        bool set_attribute(std::string const& lfn, std::string const& key, 
                std::string const& val, bool is_vector)
            { return connection_->set_attribute(lfn, key, val, is_vector); }
        bool remove_attribute(std::string const& lfn, std::string const& key)
            { return connection_->remove_attribute(lfn, key); }
        bool list_attributes(std::string const& lfn, 
                std::vector<std::string>& keys)
            { return connection_->list_attributes(lfn, keys); }
        bool find_attributes(std::string const& lfn, std::string const& kpat, 
                std::string const& vpat, std::vector<std::string>* keys, 
                bool count_only = false)
            { return connection_->find_attributes(lfn, kpat, vpat, keys, count_only); }
        bool find_attributes(std::string const& lfn, std::vector<std::string> const& kpat,
                std::vector<std::string> const& vpat, std::vector<std::string>* keys,
                bool count_only = false)
            { return connection_->find_attributes(lfn, kpat, vpat, keys, count_only); }

        // manage entries
        bool get_entries(std::string const& lfn, std::vector<std::string>& locations)
            { return connection_->get_entries(lfn, locations); }
        bool add_entry(std::string const& lfn, std::string const& location)
            { return connection_->add_entry(lfn, location); }
        bool remove_entry(std::string const& lfn, std::string const& location)
            { return connection_->remove_entry(lfn, location); }
        bool update_entry(std::string const& lfn, std::string const& oldlocation, 
            std::string const& newlocation)
            { return connection_->update_entry(lfn, oldlocation, newlocation); }

        database_connection::db_type get_dbtype() const { return type_; }
        std::string const& get_connectstring() const { return connect_string_; }

    protected:
        friend class database_scope;

        void set_connection(TR1::shared_ptr<database_connection> connection)
        {
            BOOST_ASSERT(!connection_);
            connection_ = connection;
        }
        TR1::shared_ptr<database_connection> get_connection() const
        {
            return connection_;
        }
        void reset_connection()
        {
            connection_.reset();
        }

    private:
        database_connection::db_type type_;
        std::string connect_string_;
        TR1::shared_ptr<database_connection> connection_;
    };

    ///////////////////////////////////////////////////////////////////////////
    class database_scope
    {
    public:
        template <typename Cpi>
        database_scope(Cpi *cpi_instance, database& db)
          : adaptor_ (TR1::static_pointer_cast<replica::adaptor>(
                cpi_instance->get_adaptor())),
            db_(db)
        {
            db.set_connection(adaptor_->aquire_db_connection(
                db.get_dbtype(), db.get_connectstring()));
        }
        ~database_scope()
        {
            adaptor_->release_db_connection(db_.get_connection());
            db_.reset_connection();
        }

    private:
        TR1::shared_ptr<replica::adaptor> adaptor_;
        database& db_;
    };

///////////////////////////////////////////////////////////////////////////////
}   // replica

#endif // !SAGA_ADAPTORS_DEFAULT_REPLICA_DATABASE_HPP

