//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_ADVERT_DEFAULT_ADAPTOR_HPP
#define ADAPTORS_DEFAULT_ADVERT_DEFAULT_ADAPTOR_HPP

#include <saga/saga/adaptors/adaptor.hpp>
#include "advert_database_connection_cache.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace advert
{
    ///////////////////////////////////////////////////////////////////////////
    struct adaptor : public saga::adaptor
    {
        typedef saga::impl::v1_0::op_info         op_info;  
        typedef saga::impl::v1_0::cpi_info        cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;

        // This function registers the adaptor with the factory
        // @param factory the factory where the adaptor registers
        //        its maker function and description table
        saga::impl::adaptor_selector::adaptor_info_list_type 
                adaptor_register (saga::impl::session *s);

        std::string get_name (void) const
        { 
            return BOOST_PP_STRINGIZE(SAGA_ADAPTOR_NAME);
        }

        TR1::shared_ptr<database_connection> aquire_db_connection(
                database_connection::db_type type, std::string const& connect)
        {
            return cache_.aquire(type, connect);
        }
        void release_db_connection(TR1::shared_ptr<database_connection> db)
        {
            cache_.release(db);
        }

    private:
        connection_cache cache_;
    };

///////////////////////////////////////////////////////////////////////////////
}   // namespace advert

#endif // ADAPTORS_DEFAULT_ADVERT_DEFAULT_ADAPTOR_HPP

