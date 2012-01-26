//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_EXCEPTION_HPP
#define SAGA_EXCEPTION_HPP

// include stl
#include <exception>
#include <iostream>
#include <string>
#include <vector>

// include saga dependencies
#include <saga/saga/base.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/error.hpp>
#include <saga/saga/object.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

/////////////////////////////////////////////////////////////////////////////
//
namespace saga 
{
    /// We introduce a exception hierarchy here, which maps onto the exceptions
    /// as defined by the SAGA specification:
    ///
    ///   saga::exception<br>
    ///   &nbsp;saga::not_implemented<br>
    ///   &nbsp;saga::parameter_exception<br>
    ///   &nbsp;&nbsp;saga::incorrect_url<br>
    ///   &nbsp;&nbsp;saga::bad_parameter<br>
    ///   &nbsp;saga::state_exception<br>
    ///   &nbsp;&nbsp;saga::already_exists<br>
    ///   &nbsp;&nbsp;saga::does_not_exist<br>
    ///   &nbsp;&nbsp;saga::incorrect_state<br>
    ///   &nbsp;&nbsp;saga::timeout<br>
    ///   &nbsp;saga::security_exception<br>
    ///   &nbsp;&nbsp;saga::permission_denied<br>
    ///   &nbsp;&nbsp;saga::authorization_failed<br>
    ///   &nbsp;&nbsp;saga::authentication_failed<br>
    ///   &nbsp;saga::no_success<br>
    ///
    class SAGA_EXCEPTION_EXPORT exception : public std::exception
    {
    private:
        /// @cond
        TR1::shared_ptr<saga::impl::exception> impl_;
        saga::impl::exception* get_impl() const { return impl_.get(); }
        friend struct saga::impl::runtime;
        /// @endcond

    public:
        // default constructor
        //       exception() {}

        /*! \brief Creates the exception.
         * \param obj The object associated with the exception. 
         * \param message The message to be associated with the new exception.
         * \return The newly created exception.
         */
        exception (saga::object obj, std::string const& m, error e = NoSuccess);

        /*! \brief Creates the exception.
         * \param obj The object associated with the exception. 
         * \param message The message to be associated with the new exception.
         * \return The newly created exception.
         */
        exception (saga::object obj, std::vector<saga::exception> const& l);

        /*! \brief Creates the exception.
         * \param obj The object associated with the exception. 
         * \param message The message to be associated with the new exception.
         * \param e Use this error code for the generated exception
         * \return The newly created exception.
         */
        exception (saga::object obj, std::vector<saga::exception> const& l, error e);

        /*! \brief Creates the exception, without associating a SAGA object 
         *         instance
         * \param message The message to be associated with the new exception.
         * \return The newly created exception.
         */
        explicit exception (std::string const& m, error e = NoSuccess);

        /*! \brief Creates the exception, without associating a SAGA object 
         *         instance
         * \param message The message to be associated with the new exception.
         * \return The newly created exception.
         */
        explicit exception (std::vector<saga::exception> const& l);

        /*! \brief Destroys the exception.
         */
        ~exception() throw() {}

        /*! \brief Brief %description starts here
         */
        char const* what() const throw();

        /*! \brief Gets the message associated with the exception 
         * \return The error message.
         */
        char const* get_message() const throw();

        /*! \brief Brief %description starts here
         */
        saga::error get_error () const throw();

        /*! \brief Gets the SAGA object associated with exception. 
         * \return The object associated with the exception.
         */
        saga::object get_object () const throw();

        /*! \brief return list of all exceptions encapsulated in this instance
         */
        std::vector<saga::exception> const& get_all_exceptions() const throw();

        /*! \brief return list of all messages
         * 
         *  \note may throw std::bad_alloc, use with care
         */
        std::vector<std::string> get_all_messages() const;
    }; 

    /////////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT not_implemented : public saga::exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        not_implemented (saga::object obj, std::string const& m) 
          : saga::exception(obj, m, NotImplemented)
        {}

        not_implemented (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit not_implemented (std::string const& m) 
          : saga::exception(m, NotImplemented)
        {}
    };

    /////////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT parameter_exception : public saga::exception
    {
    protected:
        /*! \brief Brief %description starts here
         */
        parameter_exception (saga::object obj, std::string const& m, saga::error e) 
          : saga::exception(obj, m, e)
        {}

        parameter_exception (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        parameter_exception (std::string const& m, saga::error e) 
          : saga::exception(m, e)
        {}
    };
    
    /////////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT incorrect_url : public saga::parameter_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        incorrect_url (saga::object obj, std::string const& m) 
          : saga::parameter_exception(obj, m, IncorrectURL)
        {}

        incorrect_url (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::parameter_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit incorrect_url (std::string const& m) 
          : saga::parameter_exception(m, IncorrectURL)
        {}
    };

    /////////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT bad_parameter : public saga::parameter_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        bad_parameter (saga::object obj, std::string const& m) 
          : saga::parameter_exception(obj, m, BadParameter)
        {}

        bad_parameter (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::parameter_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit bad_parameter (std::string const& m) 
          : saga::parameter_exception(m, BadParameter)
        {}
    };

    /////////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT state_exception : public saga::exception
    {
    protected:
        /*! \brief Brief %description starts here
         */
        state_exception (saga::object obj, std::string const& m, saga::error e) 
          : saga::exception(obj, m, e)
        {}

        state_exception (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        state_exception (std::string const& m, saga::error e) 
          : saga::exception(m, e)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT already_exists : public saga::state_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        already_exists (saga::object obj, std::string const& m) 
          : saga::state_exception(obj, m, AlreadyExists)
        {}

        already_exists (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::state_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit already_exists (std::string const& m) 
          : saga::state_exception(m, AlreadyExists)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT does_not_exist : public saga::state_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        does_not_exist (saga::object obj, std::string const& m) 
          : saga::state_exception(obj, m, DoesNotExist)
        {}

        does_not_exist (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::state_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit does_not_exist (std::string const& m) 
        : saga::state_exception(m, DoesNotExist)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT incorrect_state : public saga::state_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        incorrect_state (saga::object obj, std::string const& m) 
          : saga::state_exception(obj, m, IncorrectState)
        {}

        incorrect_state (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::state_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit incorrect_state (std::string const& m) 
        : saga::state_exception(m, IncorrectState)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT security_exception : public saga::exception
    {
    protected:
        /*! \brief Brief %description starts here
         */
        security_exception (saga::object obj, std::string const& m, saga::error e) 
          : saga::exception(obj, m, e)
        {}

        security_exception (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        security_exception (std::string const& m, saga::error e) 
        : saga::exception(m, e)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT permission_denied
    : public saga::security_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        permission_denied (saga::object obj, std::string const& m) 
          : saga::security_exception(obj, m, PermissionDenied)
        {}

        permission_denied (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::security_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit permission_denied (std::string const& m) 
          : saga::security_exception(m, PermissionDenied)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT authorization_failed
    : public saga::security_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        authorization_failed (saga::object obj, std::string const& m) 
          : saga::security_exception(obj, m, AuthorizationFailed)
        {}

        authorization_failed (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::security_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit authorization_failed (std::string const& m) 
          : saga::security_exception(m, AuthorizationFailed)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT authentication_failed
      : public saga::security_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        authentication_failed (saga::object obj, std::string const& m) 
          : saga::security_exception(obj, m, AuthenticationFailed)
        {}

        authentication_failed (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::security_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit authentication_failed (std::string const& m) 
          : saga::security_exception(m, AuthenticationFailed)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT timeout : public saga::state_exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        timeout (saga::object obj, std::string const& m) 
          : saga::state_exception(obj, m, Timeout)
        {}

        timeout (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::state_exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit timeout (std::string const& m) 
          : saga::state_exception(m, Timeout)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXCEPTION_EXPORT no_success : public saga::exception
    {
    public:
        /*! \brief Brief %description starts here
         */
        no_success (saga::object obj, std::string const& m) 
          : saga::exception(obj, m, NoSuccess)
        {}

        no_success (saga::object obj, std::vector<saga::exception> const& l) 
          : saga::exception(obj, l)
        {}

        /*! \brief Brief %description starts here
         */
        explicit no_success (std::string const& m) 
          : saga::exception(m, NoSuccess)
        {}
    };

    ///////////////////////////////////////////////////////////////////////////
    //  adaptors::exception is not in the spec, but to be used by adaptors
    /// @cond
    /** These methods are not within API scope */
    namespace adaptors 
    {
        ///////////////////////////////////////////////////////////////////////
        class SAGA_EXCEPTION_EXPORT exception : public saga::exception 
        {
        public:
            exception (saga::object obj, 
                       std::string const& name, std::string const& m, 
                       saga::error e = NoSuccess) 
              : saga::exception (obj, name + ": " + m, e)
            {
            }

            exception (saga::object obj, std::vector<saga::exception> const& l)
              : saga::exception (obj, l)
            {
            }

            ~exception (void) throw () {}
        };

    ///////////////////////////////////////////////////////////////////////////
    }   // namespace adaptors
    ///@endcond

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_EXCEPTION_HPP

