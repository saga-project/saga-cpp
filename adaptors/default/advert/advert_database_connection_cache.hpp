//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_CONNECTION_CACHE_HPP
#define SAGA_ADAPTORS_DEFAULT_ADVERT_ADVERT_DATABASE_CONNECTION_CACHE_HPP

#include <map>
#include <string>

#include <boost/thread.hpp>

#include "advert_database_connection.hpp"

namespace advert
{
    ///////////////////////////////////////////////////////////////////////////
    struct cache_key
    {
        cache_key(database_connection::db_type type, std::string const& connect_string)
          : type_(type), connect_string_(connect_string)
        {}

        database_connection::db_type type_;
        std::string connect_string_;
    };

    inline bool operator< (cache_key const& lhs, cache_key const& rhs)
    {
        if (lhs.type_ < rhs.type_)
            return true;
        if (lhs.type_ > rhs.type_)
            return false;
        return lhs.connect_string_ < rhs.connect_string_;
    }

    ///////////////////////////////////////////////////////////////////////////
    #define CACHE_ENTRY_TTL 60      // old entries live 60 seconds

    struct cache_data
    {
        cache_data(TR1::shared_ptr<database_connection> connection)
          : connection_(connection)
        {
            boost::xtime_get(&creation_time_, boost::TIME_UTC);
        }

        bool is_old() const
        {
            boost::xtime now;
            boost::xtime_get(&now, boost::TIME_UTC);
            return now.sec - creation_time_.sec > CACHE_ENTRY_TTL;
        }

        TR1::shared_ptr<database_connection> connection_;
        boost::xtime creation_time_;
    };

    ///////////////////////////////////////////////////////////////////////////
    class connection_cache
    {
        typedef boost::recursive_mutex mutex_type;
        typedef std::multimap<cache_key, cache_data> cache_type;

    public:
        connection_cache() {}
        ~connection_cache() {}

        TR1::shared_ptr<database_connection> aquire(
            database_connection::db_type type, std::string const& connect_string) 
        {
            mutex_type::scoped_lock lock(mtx_);
            cache_key key(type, connect_string);
            cache_type::iterator it = cache_.find(key);
            if (it != cache_.end()) {
            // reuse existing connection
                TR1::shared_ptr<database_connection> db((*it).second.connection_);
                cache_.erase(it);
                return db;
            }

            // create a new connection
            TR1::shared_ptr<database_connection> db (
                new database_connection(type, connect_string));
            return db;
        }

        void release(TR1::shared_ptr<database_connection> db)
        {
            mutex_type::scoped_lock lock(mtx_);

            // find old entries
            cache_type::iterator end = cache_.end();
            for (cache_type::iterator it = cache_.begin(); it != end; /**/)
            {
                if ((*it).second.is_old()) {
                    cache_type::iterator old = it++;
                    cache_.erase(old);
                }
                else {
                    ++it;
                }
            }

            // insert the new entry
            cache_.insert(cache_type::value_type(
                cache_key(db->get_dbtype(), db->get_connectstring()), db));
        }

    private:
        mutex_type mtx_;
        cache_type cache_;
    };

}

#endif

