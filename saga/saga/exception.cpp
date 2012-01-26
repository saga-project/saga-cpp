//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// include stl
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <boost/assert.hpp>

// include saga dependencies
#include <saga/saga/base.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/error.hpp>
#include <saga/saga/object.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/impl/exception.hpp>

/////////////////////////////////////////////////////////////////////////////
//
namespace saga 
{
    exception::exception (saga::object obj, std::string const& m, error e) 
      : impl_(new saga::impl::exception(obj, m, e))
    {}

    /*! \brief Creates the exception.
     * \param obj The object associated with the exception. 
     * \param message The message to be associated with the new exception.
     * \return The newly created exception.
     */
    exception::exception (saga::object obj, std::vector<saga::exception> const& l) 
      : impl_(new saga::impl::exception(obj, l))
    {}

    /*! \brief Creates the exception.
     * \param obj The object associated with the exception. 
     * \param message The message to be associated with the new exception.
     * \param e Use this error code for the generated exception
     * \return The newly created exception.
     */
    exception::exception (saga::object obj, 
            std::vector<saga::exception> const& l, error e) 
      : impl_(new saga::impl::exception(obj, l, e))
    {}

    /*! \brief Creates the exception, without associating a SAGA object 
     *         instance
     * \param message The message to be associated with the new exception.
     * \return The newly created exception.
     */
    exception::exception (std::string const& m, error e) 
      : impl_(new saga::impl::exception(m, e))
    {}

    /*! \brief Creates the exception, without associating a SAGA object 
     *         instance
     * \param message The message to be associated with the new exception.
     * \return The newly created exception.
     */
    exception::exception (std::vector<saga::exception> const& l) 
      : impl_(new saga::impl::exception(l))
    {}

    /*! \brief Brief %description starts here
     */
    char const* exception::what() const throw() 
    { 
        BOOST_ASSERT(impl_);
        return impl_->what();
    }

    /*! \brief Gets the message associated with the exception 
     * \return The error message.
     */
    char const* exception::get_message() const throw() 
    { 
        BOOST_ASSERT(impl_);
        return impl_->get_message();    // top level message only
    }

    /*! \brief Brief %description starts here
     */
    saga::error exception::get_error () const throw() 
    { 
        BOOST_ASSERT(impl_);
        return impl_->get_error(); 
    }

    /*! \brief Gets the SAGA object associated with exception. 
     * \return The object associated with the exception.
     */
    saga::object exception::get_object () const throw() 
    { 
        return impl_->get_object(); 
    }

    /*! \brief return list of all exceptions encapsulated in this instance
     */
    std::vector<saga::exception> const& exception::get_all_exceptions() const throw()
    {
        BOOST_ASSERT(impl_);
        return impl_->get_all_exceptions(); 
    }

    /*! \brief return list of all messages
     * 
     *  \note may throw std::bad_alloc, use with care
     */
    std::vector<std::string> exception::get_all_messages() const
    {
        BOOST_ASSERT(impl_);
        return impl_->get_all_messages(); 
    }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

