//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_EXCEPTION_HPP
#define SAGA_IMPL_EXCEPTION_HPP

// filesystem will be in TR2
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/preprocessor/stringize.hpp>

#include <saga/saga/base.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/error.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/saga/uuid.hpp>
#include <saga/saga/exception.hpp>
#include <saga/impl/exception_list.hpp>
#include <saga/impl/runtime.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
    namespace detail
    {
        // return the most significant error from the list
        SAGA_EXPORT error get_error(std::vector<saga::exception> const&);
        SAGA_EXPORT error get_error(std::vector<saga::exception> const&, 
            saga::object obj);

        // return the full hierarchy of messages corresponding to the exception 
        // in the list
        SAGA_EXPORT std::string get_message(std::vector<saga::exception> const&);

        // return the message associated with the most significant error in 
        // the list
        SAGA_EXPORT std::string get_top_message(std::vector<saga::exception> const&);
        SAGA_EXPORT std::string get_top_message(std::vector<saga::exception> const&, 
            saga::object obj);

        // return a list of (fully hierarchical) messages for all errors in the
        // list
        SAGA_EXPORT std::vector<std::string> get_all_messages(std::vector<saga::exception> const&);
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace impl 
    {
        ///////////////////////////////////////////////////////////////////////
        /// @cond
        class exception
        {
        private:
            std::string msg_;
            std::string fullmsg_;
            mutable saga::error err_;
            saga::object object_;
            std::vector<saga::exception> exceptions_;
            saga::uuid failed_cpi_;

            friend class saga::impl::exception_list;

        public:
            exception (saga::object obj, std::string const& m, error e) 
              : msg_(""), fullmsg_(""), err_(e), object_(obj)
            {
                BOOST_ASSERT(err_ >= (saga::error)saga::adaptors::Success && 
                             err_ <= (saga::error)saga::adaptors::Unexpected);

                // prefix the message with the SAGA exception name (only if it's not 
                // already there)
                std::string::size_type p = m.find("SAGA(");
                if (std::string::npos == p || 0 != p)
                    msg_ = std::string("SAGA(") + error_names[e] + "): ";

                if (err_ != (saga::error)saga::adaptors::Success)
                {
                    SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
                        << "Created exception: " << m;
                }

                msg_ += m + '\n';
            }

            exception (saga::object obj, std::vector<saga::exception> const& l) 
              : msg_(detail::get_top_message(l)), fullmsg_(detail::get_message(l)), 
                err_(detail::get_error(l, obj)), object_(obj), 
                exceptions_(l)
            {
                BOOST_ASSERT(err_ >= (saga::error)saga::adaptors::Success && 
                             err_ <= (saga::error)saga::adaptors::Unexpected);

                if (err_ != (saga::error)saga::adaptors::Success)
                {
                    SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
                        << "Created exception: " << fullmsg_;
                }
            }

            exception (saga::object obj, std::vector<saga::exception> const& l, error e) 
              : msg_(detail::get_top_message(l)), fullmsg_(detail::get_message(l)), 
                err_(e), object_(obj), exceptions_(l)
            {
                BOOST_ASSERT(err_ >= (saga::error)saga::adaptors::Success && 
                             err_ <= (saga::error)saga::adaptors::Unexpected);

                if (err_ != (saga::error)saga::adaptors::Success)
                {
                    SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
                        << "Created exception: " << fullmsg_;
                }
            }

            explicit exception (std::string const& m, error e) 
              : msg_(""), err_(e)
            {
                BOOST_ASSERT(err_ >= (saga::error)saga::adaptors::Success && 
                             err_ <= (saga::error)saga::adaptors::Unexpected);

                // prefix the message with the SAGA exception name (only if it's not 
                // already there)
                std::string::size_type p = m.find("SAGA(");
                if (std::string::npos == p || 0 != p)
                    msg_ = std::string("SAGA(") + error_names[e] + "): ";

                if (err_ != (saga::error)saga::adaptors::Success)
                {
                    SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
                        << "Created exception: " << m;
                }

                msg_ += m + '\n';
            }

            explicit exception (std::vector<saga::exception> const& l) 
              : msg_(detail::get_top_message(l)), fullmsg_(detail::get_message(l)), 
                err_(detail::get_error(l)), exceptions_(l)
            {
                BOOST_ASSERT(err_ >= (saga::error)saga::adaptors::Success && 
                             err_ <= (saga::error)saga::adaptors::Unexpected);

                if (err_ != (saga::error)saga::adaptors::Success)
                {
                    SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
                        << "Created exception: " << fullmsg_;
                }
            }

            ~exception() throw() {}

            char const* what() const throw() 
            { 
                return fullmsg_.empty() ? msg_.c_str() : fullmsg_.c_str();
            }

            char const* get_message() const throw() 
            { 
                return msg_.c_str();    // top level message only
            }

            saga::error get_error () const throw() 
            { 
                return err_; 
            }
            void set_error (saga::error err) throw() 
            { 
                err_ = err; 
            }

            saga::object get_object () const throw() 
            { 
                return object_; 
            }

            std::vector<saga::exception> const& get_all_exceptions() const throw()
            {
                return exceptions_;
            }

            std::vector<std::string> get_all_messages() const
            {
                if (exceptions_.empty()) {
                    std::vector<std::string> msgs;
                    msgs.push_back(what());
                    return msgs;
                }
                return detail::get_all_messages(exceptions_);
            }

            saga::uuid const& get_failed_cpi() const
            {
                return failed_cpi_;
            }
            void set_failed_cpi(saga::uuid const& u)
            {
                failed_cpi_ = u;
            }
        };
        /// @endcond

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception 
        SAGA_EXPORT void throw_exception(saga::object const& obj, 
            std::string const& m, error e);

        SAGA_EXPORT void throw_exception(saga::object const& obj, 
            exception_list const& l);

        SAGA_EXPORT void throw_exception(saga::object const& obj, 
            exception_list const& l, error e);

        SAGA_EXPORT void throw_exception(saga::object const& obj, 
            std::vector<saga::exception> const& l, error e);

        SAGA_EXPORT void throw_exception(saga::object const& obj, 
            exception_list const& l, error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from a saga object member function
        SAGA_EXPORT void throw_exception(saga::impl::object const *this_, 
            std::string const& m, error e);

        SAGA_EXPORT void throw_exception(saga::impl::object const *this_, 
            exception_list const& l);

        SAGA_EXPORT void throw_exception(saga::impl::object const *this_, 
            exception_list const& l, error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from an adaptor member function
        SAGA_EXPORT void throw_exception(saga::impl::v1_0::cpi const *this_, 
            std::string const& m, error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from any other function
        SAGA_EXPORT void throw_exception(void const *this_, 
            std::string const& m, error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::adaptors::exception 
        SAGA_EXPORT void throw_exception(saga::object const& obj, 
            std::string const& m, adaptors::error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from a saga object member function
        SAGA_EXPORT void throw_exception(saga::impl::object const *this_, 
            std::string const& m, adaptors::error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from an adaptor member function
        SAGA_EXPORT void throw_exception(saga::impl::v1_0::cpi const *this_, 
            std::string const& m, adaptors::error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from any other function
        SAGA_EXPORT void throw_exception(void const *this_, 
            std::string const& m, adaptors::error e);

    ///////////////////////////////////////////////////////////////////////////
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace adaptors 
    {
        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::adaptors::exception
        SAGA_EXPORT void throw_adaptor_exception(
            saga::impl::v1_0::cpi const *this_, 
            std::string const& adaptor_name, std::string const& m, 
            saga::error e);

        SAGA_EXPORT void throw_adaptor_exception(
            saga::impl::v1_0::cpi const *this_, impl::exception_list const& l);

        SAGA_EXPORT void throw_adaptor_exception(
            std::string const& adaptor_name, std::string const& m, 
            saga::error e);

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::adaptors::exception
        SAGA_EXPORT void throw_adaptor_exception(
            saga::impl::v1_0::cpi const *this_, 
            std::string const& adaptor_name, std::string const& m, 
            saga::adaptors::error e);

        SAGA_EXPORT void throw_adaptor_exception(
            std::string const& adaptor_name, std::string const& m, 
            saga::adaptors::error e);

    ///////////////////////////////////////////////////////////////////////////
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

///////////////////////////////////////////////////////////////////////////////
#define SAGA_THROW_PLAIN(obj, msg, errcode)                                   \
        saga::impl::throw_exception(obj, msg, errcode);                       \
    /**/

#define SAGA_THROW_PLAIN_LIST(obj, list)                                      \
        saga::impl::throw_exception(obj, list);                               \
    /**/

#define SAGA_THROW_PLAIN_LIST_EX(obj, list, e)                                \
        saga::impl::throw_exception(obj, list, e);                            \
    /**/

#if BOOST_FILESYSTEM_VERSION == 3
///////////////////////////////////////////////////////////////////////////////
#define SAGA_THROW_VERBATIM(obj, msg, errcode)                                \
    {                                                                         \
        std::string __s("");                                                  \
        SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)                               \
        {                                                                     \
            boost::filesystem::path __p(__FILE__);                            \
            __s = saga::detail::leaf(__p);                                    \
            __s += std::string("(") + BOOST_PP_STRINGIZE(__LINE__) + "): ";   \
        }                                                                     \
        __s += std::string("") + msg;                                         \
        SAGA_THROW_PLAIN(obj, __s, errcode);                                  \
    }                                                                         \
    /**/
#else
///////////////////////////////////////////////////////////////////////////////
#define SAGA_THROW_VERBATIM(obj, msg, errcode)                                \
    {                                                                         \
        std::string __s("");                                                  \
        SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)                               \
        {                                                                     \
            boost::filesystem::path __p(__FILE__, boost::filesystem::native); \
            __s = saga::detail::leaf(__p);                                    \
            __s += std::string("(") + BOOST_PP_STRINGIZE(__LINE__) + "): ";   \
        }                                                                     \
        __s += std::string("") + msg;                                         \
        SAGA_THROW_PLAIN(obj, __s, errcode);                                  \
    }                                                                         \
/**/
#endif

///////////////////////////////////////////////////////////////////////////////
#define SAGA_THROW(msg, errcode)                                              \
    SAGA_THROW_VERBATIM(this, msg, errcode)                                   \
    /**/
    
///////////////////////////////////////////////////////////////////////////////
#define SAGA_THROW_NO_OBJECT(msg, errcode)                                    \
    SAGA_THROW_VERBATIM(saga::object(), msg, errcode)                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_ADAPTOR_THROW_PLAIN(obj, msg, errcode)                           \
        saga::adaptors::throw_adaptor_exception(obj,                          \
            BOOST_PP_STRINGIZE(SAGA_ADAPTOR_NAME), msg, errcode);             \
    /**/

#define SAGA_ADAPTOR_THROW_PLAIN_LIST(obj, list)                              \
        saga::adaptors::throw_adaptor_exception(obj, list);                   \
    /**/

#if BOOST_FILESYSTEM_VERSION == 3
///////////////////////////////////////////////////////////////////////////////
#define SAGA_ADAPTOR_THROW_VERBATIM(cpi, msg, errcode)                        \
    {                                                                         \
        std::string __s("");                                                  \
        SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)                               \
        {                                                                     \
            boost::filesystem::path __p(__FILE__);                            \
            __s = saga::detail::leaf(__p);                                    \
            __s += std::string("(") + BOOST_PP_STRINGIZE(__LINE__) + "): ";   \
        }                                                                     \
        __s += std::string("") + msg;                                         \
        SAGA_ADAPTOR_THROW_PLAIN(cpi, __s, errcode);                          \
    }                                                                         \
    /**/
#else
///////////////////////////////////////////////////////////////////////////////
#define SAGA_ADAPTOR_THROW_VERBATIM(cpi, msg, errcode)                        \
{                                                                             \
        std::string __s("");                                                  \
        SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)                               \
        {                                                                     \
            boost::filesystem::path __p(__FILE__, boost::filesystem::native); \
            __s = saga::detail::leaf(__p);                                    \
            __s += std::string("(") + BOOST_PP_STRINGIZE(__LINE__) + "): ";   \
        }                                                                     \
        __s += std::string("") + msg;                                         \
        SAGA_ADAPTOR_THROW_PLAIN(cpi, __s, errcode);                          \
    }                                                                         \
    /**/
#endif

///////////////////////////////////////////////////////////////////////////////
#define SAGA_ADAPTOR_THROW(msg, errcode)                                      \
    SAGA_ADAPTOR_THROW_VERBATIM(this, msg, errcode)                           \
    /**/
    
#if BOOST_FILESYSTEM_VERSION == 3
///////////////////////////////////////////////////////////////////////////////
#define SAGA_ADAPTOR_THROW_NO_CONTEXT(msg, errcode)                           \
    {                                                                         \
        std::string __s("");                                                  \
        SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)                               \
        {                                                                     \
            boost::filesystem::path __p(__FILE__);                            \
            __s = saga::detail::leaf(__p);                                    \
            __s += std::string("(") + BOOST_PP_STRINGIZE(__LINE__) + "): ";   \
        }                                                                     \
        __s += std::string("") + msg;                                         \
        saga::adaptors::throw_adaptor_exception(                              \
              BOOST_PP_STRINGIZE(SAGA_ADAPTOR_NAME), __s, errcode);           \
    }                                                                         \
    /**/
#else
#define SAGA_ADAPTOR_THROW_NO_CONTEXT(msg, errcode)                           \
    {                                                                         \
        std::string __s("");                                                  \
        SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)                               \
        {                                                                     \
            boost::filesystem::path __p(__FILE__, boost::filesystem::native); \
            __s = saga::detail::leaf(__p);                                    \
            __s += std::string("(") + BOOST_PP_STRINGIZE(__LINE__) + "): ";   \
        }                                                                     \
        __s += std::string("") + msg;                                         \
        saga::adaptors::throw_adaptor_exception(                              \
              BOOST_PP_STRINGIZE(SAGA_ADAPTOR_NAME), __s, errcode);           \
    }                                                                         \
    /**/
#endif

#endif // !SAGA_IMPL_EXCEPTION_HPP

