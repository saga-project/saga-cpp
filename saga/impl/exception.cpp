//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/base.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/saga/exception.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/exception.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { 

    ///////////////////////////////////////////////////////////////////////////
    namespace impl {

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception 
        void throw_exception(saga::object const& obj, std::string const& m, 
            error e)
        {
            switch (e) {
            case NotImplemented:
                throw saga::not_implemented(obj, m);
            case IncorrectURL:
                throw saga::incorrect_url(obj, m);
            case BadParameter:
                throw saga::bad_parameter(obj, m);
            case AlreadyExists:
                throw saga::already_exists(obj, m);
            case DoesNotExist:
                throw saga::does_not_exist(obj, m);
            case IncorrectState:
                throw saga::incorrect_state(obj, m);
            case PermissionDenied:
                throw saga::permission_denied(obj, m);
            case AuthorizationFailed:
                throw saga::authorization_failed(obj, m);
            case AuthenticationFailed:
                throw saga::authentication_failed(obj, m);
            case Timeout:
                throw saga::timeout(obj, m);
            case (saga::error)adaptors::AdaptorDeclined:
            case (saga::error)adaptors::NoAdaptor:
            case (saga::error)adaptors::NoAdaptorInfo:
            case (saga::error)adaptors::Unexpected:
                throw saga::exception(obj, m, e);
            case NoSuccess:
                throw saga::no_success(obj, m);
            default:
                throw saga::exception(obj, m, e);
            }
        }

        void throw_exception(saga::object const& obj, 
            std::vector<saga::exception> const& l, saga::error e)
        {
            switch (e) {
            case NotImplemented:
                throw saga::not_implemented(obj, l);
            case IncorrectURL:
                throw saga::incorrect_url(obj, l);
            case BadParameter:
                throw saga::bad_parameter(obj, l);
            case AlreadyExists:
                throw saga::already_exists(obj, l);
            case DoesNotExist:
                throw saga::does_not_exist(obj, l);
            case IncorrectState:
                throw saga::incorrect_state(obj, l);
            case PermissionDenied:
                throw saga::permission_denied(obj, l);
            case AuthorizationFailed:
                throw saga::authorization_failed(obj, l);
            case AuthenticationFailed:
                throw saga::authentication_failed(obj, l);
            case Timeout:
                throw saga::timeout(obj, l);
            case (saga::error)adaptors::AdaptorDeclined:
            case (saga::error)adaptors::NoAdaptor:
            case (saga::error)adaptors::NoAdaptorInfo:
            case (saga::error)adaptors::Unexpected:
                throw saga::exception(obj, l, e);
            case NoSuccess:
                throw saga::no_success(obj, l);
            default:
                throw saga::exception(obj, l);
            }
        }

        void throw_exception(saga::object const& obj, exception_list const& l)
        {
            throw_exception(obj, l.get_all_exceptions(), l.get_error());
        }

        void throw_exception(saga::object const& obj, exception_list const& l,
            saga::error e)
        {
            throw_exception(obj, l.get_all_exceptions(), e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception 
        void throw_exception(saga::object const& obj, std::string const& m, 
            adaptors::error e)
        {
            throw_exception(obj, m, (saga::error)e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from a saga object member function
        void throw_exception(saga::impl::object const *this_, 
            std::string const& m, error e)
        {
            throw_exception(
                runtime::get_object(
                    TR1::const_pointer_cast<saga::impl::object>(
                        this_->shared_from_this())
                ), 
                m, e);
        }

        void throw_exception(saga::impl::object const *this_, 
            exception_list const& l)
        {
            throw_exception(
                runtime::get_object(
                    TR1::const_pointer_cast<saga::impl::object>(
                        this_->shared_from_this())
                ), 
                l);
        }

        void throw_exception(saga::impl::object const *this_, 
            exception_list const& l, saga::error e)
        {
            throw_exception(
                runtime::get_object(
                    TR1::const_pointer_cast<saga::impl::object>(
                        this_->shared_from_this())
                ), 
                l, e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from a saga object member function
        void throw_exception(saga::impl::object const *this_, 
            std::string const& m, adaptors::error e)
        {
            throw_exception(this_, m, (saga::error)e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from an adaptor member function
        void throw_exception(saga::impl::v1_0::cpi const *this_, 
            std::string const& m, error e)
        {
            TR1::shared_ptr<saga::impl::object> impl(
                TR1::static_pointer_cast<saga::impl::object>(
                    TR1::const_pointer_cast<saga::impl::proxy>(
                        this_->get_proxy()->shared_from_this())));

            throw_exception(runtime::get_object(impl), m, e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from an adaptor member function
        void throw_exception(saga::impl::v1_0::cpi const *this_, 
            std::string const& m, adaptors::error e)
        {
            throw_exception(this_, m, (saga::error)e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from any other function
        void throw_exception(void const *this_, std::string const& m, error e)
        {
            throw_exception(saga::object(), m, e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::exception from any other function
        void throw_exception(void const *this_, std::string const& m, 
            adaptors::error e)
        {
            throw_exception(saga::object(), m, e);
        }

    ///////////////////////////////////////////////////////////////////////////
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace adaptors 
    {
        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::adaptors::exception
        void throw_adaptor_exception(saga::impl::v1_0::cpi const *this_, 
            std::string const& adaptor_name, std::string const& m, 
            saga::error e)
        {
            TR1::shared_ptr<saga::impl::object> impl(
                TR1::static_pointer_cast<saga::impl::object>(
                    TR1::const_pointer_cast<saga::impl::proxy>(
                        this_->get_proxy()->shared_from_this())));

            throw saga::adaptors::exception(
                saga::impl::runtime::get_object(impl), adaptor_name, m, e);
        }

        void throw_adaptor_exception(saga::impl::v1_0::cpi const *this_, 
            saga::impl::exception_list const& l)
        {
            TR1::shared_ptr<saga::impl::object> impl(
                TR1::static_pointer_cast<saga::impl::object>(
                    TR1::const_pointer_cast<saga::impl::proxy>(
                        this_->get_proxy()->shared_from_this())));

            throw saga::adaptors::exception(
                saga::impl::runtime::get_object(impl), l.get_all_exceptions());
        }

        void throw_adaptor_exception(saga::impl::v1_0::cpi const *this_, 
            std::string const& adaptor_name, std::string const& m, 
            saga::adaptors::error e)
        {
            throw_adaptor_exception(this_, adaptor_name, m, (saga::error)e);
        }

        ///////////////////////////////////////////////////////////////////////
        //  throw a saga::adaptors::exception
        void throw_adaptor_exception(std::string const& adaptor_name, 
            std::string const& m, saga::error e)
        {
            throw saga::adaptors::exception(saga::object(), adaptor_name, m, e);
        }

        void throw_adaptor_exception(std::string const& adaptor_name, 
            std::string const& m, saga::adaptors::error e)
        {
            throw saga::adaptors::exception(saga::object(), adaptor_name, m, 
                (saga::error)e);
        }

    ///////////////////////////////////////////////////////////////////////////
    }

///////////////////////////////////////////////////////////////////////////////
}  // namespace saga::impl
