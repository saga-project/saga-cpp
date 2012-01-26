//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_HPP
#define SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_HPP

#include <boost/assert.hpp>
#include <saga/saga/url.hpp>

#include "default_advert_adaptor.hpp"
#include "advert_database_connection.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace advert
{
    ///////////////////////////////////////////////////////////////////////////
    // forward declaration only
    class database_scope;

    enum entry_type
    {
        unknown,
        entry,
        directory
    };

    ///////////////////////////////////////////////////////////////////////////
    class database
    {
    public:
        database(database_connection::db_type type, std::string const & connect_string)
          : type_(type), connect_string_(connect_string), node_(0), isdir_(unknown)
        {}
        ~database() 
        {}

        void verify_status() { connection_->verify_status(); }
        void close(bool verify = true)
        { 
            if (verify) 
                verify_status(); 
            node_ = 0;
            isdir_ = unknown;
        }

        bool exists_other(std::string const& adname) 
        { 
            int node = 0;
            bool found_dir = false;
            return connection_->exists(adname, node, found_dir);
        }
        bool exists_entry_other(std::string const& adname, bool throw_on_error = true) 
        { 
            int node = 0;
            return connection_->exists_entry(adname, node, throw_on_error);
        }
        bool exists_directory_other(std::string const& adname, bool throw_on_error = true) 
        { 
            int node = 0;
            return connection_->exists_directory(adname, node, throw_on_error);
        }

        // detect, whether advert already exists
        bool exists(std::string const& adname) 
        { 
            if (0 != node_)
            {
                BOOST_ASSERT(isdir_ != unknown);
                return true;
            }

            bool found_dir = false;
            if (connection_->exists(adname, node_, found_dir))
            {
                isdir_ = found_dir ? directory : entry;
                return true;
            }
            return false;
        }
        bool exists_entry(std::string const& adname, bool throw_on_error = true) 
        { 
            if (0 != node_) 
            {
                BOOST_ASSERT(isdir_ == entry);
                return true;
            }

            if (connection_->exists_entry(adname, node_, throw_on_error))
            {
                BOOST_ASSERT(isdir_ == unknown || isdir_ == entry);
                isdir_ = entry;
                return true;
            }
            return false;
        }
        bool exists_directory(std::string const& adname, bool throw_on_error = true) 
        { 
            if (0 != node_)
            {
                BOOST_ASSERT(isdir_ == directory);
                return true;
            }

            if (connection_->exists_directory(adname, node_, throw_on_error))
            {
                BOOST_ASSERT(isdir_ == unknown || isdir_ == directory);
                isdir_ = directory;
                return true;
            }
            return false;
        }

        int find(std::string const& url, bool& found_dir, bool force = false)
        { 
            if (!force && 0 != node_)
            {
                BOOST_ASSERT(isdir_ != unknown);
                found_dir = (isdir_ == directory) ? true : false;
                return true;
            }
            int node = connection_->find(url, found_dir);
            if (0 == node)
                return false;

            if (!force && 0 == node_)
                node_ = node;

            isdir_ = found_dir ? directory : entry;
            return true;
        }

        // create a new advert
        bool create_entry(std::string const& adname, bool create_parents)
        { 
            if (connection_->create_entry(adname, create_parents, node_)) 
            {
                isdir_ = entry;
                return true;
            }
            return false;
        }
        bool create_directory(std::string const& adname, bool create_parents)
        { 
            if (connection_->create_directory(adname, create_parents, node_)) 
            {
                isdir_ = directory;
                return true;
            }
            return false;
        }
        bool create_new_directory(std::string const& adname, bool create_parents)
        { 
            int node = 0;
            return connection_->create_directory(adname, create_parents, node); 
        }

        // manage nodes
        bool remove_other(std::string const& adname)
        { 
            int node = 0;
            return connection_->remove(adname, node);
        }
        bool remove_directory_other(std::string const& adname)
        { 
            int node = 0;
            return connection_->remove_directory(adname, node);
        }

        bool remove(std::string const& adname)
        { 
            if (connection_->remove(adname, node_))
            {
                node_ = 0;
                isdir_ = unknown;
                return true;
            }
            return false;
        }
        bool remove_directory(std::string const& adname)
        { 
            if (connection_->remove_directory(adname, node_))
            {
                node_ = 0;
                isdir_ = unknown;
                return true;
            }
            return false;
        }
        bool list(std::string const& adname, std::string const& pattern, 
                std::vector<std::string>& entries, bool force = false)
        { 
            int node = force ? 0 : node_;
            if (connection_->list(adname, pattern, entries, node))
            {
                BOOST_ASSERT(isdir_ == unknown || isdir_ == directory);
                isdir_ = directory;
                if (!force && 0 == node_)
                    node_ = node;
                return true;
            }
            return false;
        }

        // manage attributes
        bool attribute_exists(std::string const& adname, std::string const& key, 
                bool& val)
        { 
            bool found_dir = false;
            if (connection_->attribute_exists(adname, key, val, node_, found_dir))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                return true;
            }
            return false;
        }
        bool get_attribute(std::string const& adname, std::string const& key, 
                std::string& val, bool& is_vector)
        {
            bool found_dir = false;
            if (connection_->get_attribute(adname, key, val, is_vector, node_, found_dir))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                return true;
            }
            return false;
        }
        bool set_attribute(std::string const& adname, std::string const& key, 
                std::string const& val, bool is_vector)
        { 
            bool found_dir = false;
            if (connection_->set_attribute(adname, key, val, is_vector, node_, found_dir))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                return true;
            }
            return false;
        }
        bool remove_attribute(std::string const& adname, std::string const& key)
        { 
            bool found_dir = false;
            if (connection_->remove_attribute(adname, key, node_, found_dir))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                return true;
            }
            return false;
        }
        bool list_attributes(std::string const& adname, 
                std::vector<std::string>& keys)
        { 
            bool found_dir = false;
            if (connection_->list_attributes(adname, keys, node_, found_dir))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                return true;
            }
            return false;
        }
        bool find_attributes(std::string const& adname, std::string const& kpat, 
                std::string const& vpat, std::vector<std::string>* keys, 
                bool count_only = false, bool force = false)
        { 
            bool found_dir = false;
            int node = force ? 0 : node_;
            if (connection_->find_attributes(adname, kpat, vpat, keys, node, found_dir, count_only))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                if (!force && 0 == node_)
                    node_ = node;
                return true;
            }
            return false;
        }
        bool find_attributes(std::string const& adname, std::vector<std::string> const& kpat,
                std::vector<std::string> const& vpat, std::vector<std::string>* keys,
                bool count_only = false, bool force = false)
        { 
            bool found_dir = false;
            int node = force ? 0 : node_;
            if (connection_->find_attributes(adname, kpat, vpat, keys, node, found_dir, count_only))
            {
                if (unknown == isdir_)
                    isdir_ = found_dir ? directory : entry;
                if (!force && 0 == node_)
                    node_ = node;
                return true;
            }
            return false;
        }

        bool change_directory(std::string const& adname, bool throw_on_error = true)
        {
            int node = 0;
            if (connection_->exists_directory(adname, node, throw_on_error))
            {
                node_ = node;
                isdir_ = directory;
                return true;
            }
            return false;
        }

        // store/retrieve data
        bool store_data(std::string const& url, std::string const& data)
        { 
            if (connection_->store_data(url, data, node_))
            { 
                isdir_ = entry;
                return true;
            }
            return false;
        }
        bool retrieve_data(std::string const& url, std::string& data)
        { 
            if (connection_->retrieve_data(url, data, node_))
            { 
                isdir_ = entry;
                return true;
            }
            return false;
        }

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
        int node_;
        entry_type isdir_;
    };

    ///////////////////////////////////////////////////////////////////////////
    class database_scope
    {
    public:
        template <typename Cpi>
        database_scope(Cpi *cpi_instance, database& db)
          : adaptor_ (TR1::static_pointer_cast<advert::adaptor>(
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
        TR1::shared_ptr<advert::adaptor> adaptor_;
        database& db_;
    };

///////////////////////////////////////////////////////////////////////////////
}   // advert

#endif // !SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_HPP

