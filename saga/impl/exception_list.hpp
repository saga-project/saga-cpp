//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_EXCEPTION_LIST_HPP
#define SAGA_IMPL_EXCEPTION_LIST_HPP

#include <vector>
#include <algorithm>

#include <saga/saga/base.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/error.hpp>
#include <saga/saga/exception.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    class exception_list
    {
    private:
        typedef std::vector<saga::exception> exception_list_type;
        exception_list_type exceptions_;

    public:
        SAGA_EXPORT exception_list();
        explicit SAGA_EXPORT exception_list(saga::exception const& e);

        SAGA_EXPORT void add(saga::exception const& e);
        SAGA_EXPORT saga::error get_error() const;
        SAGA_EXPORT std::string get_message() const;
        SAGA_EXPORT std::string get_top_message() const;
        SAGA_EXPORT std::vector<std::string> get_all_messages() const;

        SAGA_EXPORT std::size_t get_error_count() const;

        SAGA_EXPORT std::vector<saga::exception> const& get_all_exceptions() const;
    };

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl

#endif // !SAGA_IMPL_EXCEPTION_LIST_HPP


