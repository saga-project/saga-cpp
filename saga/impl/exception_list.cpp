//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <set>
#include <map>
#include <vector>

#include <saga/saga/base.hpp>
#include <saga/impl/object.hpp>
#include <saga/impl/exception.hpp>
#include <saga/impl/exception_list.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    inline bool consider_adaptor_exceptions() 
    {
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO)
        {
            return true;
        }
        return false;
    }

    error get_error(std::vector<saga::exception> const& exceptions)
    {
        if (exceptions.empty())
            return saga::NoSuccess;

        // find the first exception having a valid stored saga::object
        saga::object obj;
        std::vector<saga::exception>::const_iterator end = exceptions.end();
        for (std::vector<saga::exception>::const_iterator it = exceptions.begin(); 
            it != end; ++it)
        {
            obj = (*it).get_object();
            if (obj.get_type() != saga::object::Unknown)
                break;
        }

        return get_error(exceptions, obj);
    }

    error get_error(std::vector<saga::exception> const& exceptions,
        saga::object obj)
    {
        bool keep_adaptor_exceptions = consider_adaptor_exceptions();

        // we ignore NotImplemented while looking for the most significant 
        // error as long as there are other errors. NotImplemented will be
        // reported only if there are 'only' NotImplemented errors
        std::set<saga::error> errors;

        // at the same time we look for exceptions thrown from the adaptor 
        // being the last known good adaptor for the stored API object
        saga::uuid last_known_good;
        TR1::shared_ptr<impl::object> impl(impl::runtime::get_impl_object_sp(obj));
        if (impl) 
            last_known_good = impl->get_last_known_good();

        std::vector<saga::exception>::const_iterator end = exceptions.end();
        for (std::vector<saga::exception>::const_iterator it = exceptions.begin(); 
            it != end; ++it)
        {
            saga::error err = (*it).get_error();
            if (err == saga::error(saga::adaptors::AdaptorDeclined) && 
                !keep_adaptor_exceptions)
            {
                continue;     // ignore AdaptorDeclined
            }

            if (err != saga::NotImplemented) {
                saga::uuid adaptor_id = impl::runtime::get_impl(*it)->get_failed_cpi();
                if (adaptor_id == last_known_good)
                    return err;   // we found an adaptor which previously succeeded

                // just store this error in the sorted list
                errors.insert(err);
            }
        }

        return errors.empty() ? saga::NotImplemented : *errors.begin();
    }

    std::string indent_message(char const* msg_) 
    {
        std::string result;
        std::string msg(msg_);
        std::string::size_type pos = msg.find_first_of("\n");
        std::string::size_type first_non_ws = msg.find_first_not_of(" \n");
        std::string::size_type pos1 = 0;

        while (std::string::npos != pos) {
            if (pos > first_non_ws) {   // skip leading newline
                result += msg.substr(pos1, pos-pos1+1); 
                do {
                    // skip consecutive newlines
                    pos = msg.find_first_of("\n", pos1 = pos+1);
                } while (pos == pos1);
                if (std::string::npos != pos)
                    result += "  ";
            }
            else {
                pos = msg.find_first_of("\n", pos1 = pos+1);
            }
        }

        result += msg.substr(pos1); 
        return result;
    }

    ///////////////////////////////////////////////////////////////////////////
    std::string get_message(std::vector<saga::exception> const& exceptions)
    {
        if (exceptions.empty())
            return "";

        if (1 == exceptions.size())
            return exceptions.front().get_message();

        std::string result("SAGA(");
        saga::error err = get_error(exceptions);
        if (err >= (saga::error)adaptors::FirstAdaptorSpecificException)
            err = NoSuccess;
        result += error_names[err];
        result += "):\n";

        bool keep_adaptor_exceptions = consider_adaptor_exceptions();

        std::vector<saga::exception>::const_iterator end = exceptions.end();
        for (std::vector<saga::exception>::const_iterator it = exceptions.begin(); 
            it != end; ++it)
        {
            saga::error err = (*it).get_error();
            if (err == saga::error(saga::adaptors::AdaptorDeclined) && 
                !keep_adaptor_exceptions)
            {
                continue;       // ignore AdaptorDeclined
            }

            result += "  ";
            result += indent_message((*it).what());
            if (result.find_last_of("\n") < result.size()-1)
                result += "\n";
        }
        return result;
    }

    ///////////////////////////////////////////////////////////////////////////
    std::string get_top_message(std::vector<saga::exception> const& exceptions)
    {
        if (exceptions.empty()) 
            return "";

        // find the first exception having a valid stored saga::object
        saga::object obj;
        std::vector<saga::exception>::const_iterator end = exceptions.end();
        for (std::vector<saga::exception>::const_iterator it = exceptions.begin(); 
            it != end; ++it)
        {
            obj = (*it).get_object();
            if (obj.get_type() != saga::object::Unknown)
                break;
        }

        return get_top_message(exceptions, obj);
    }

    std::string get_top_message(std::vector<saga::exception> const& exceptions,
        saga::object obj)
    {
        if (exceptions.empty())
            return "";

        bool keep_adaptor_exceptions = consider_adaptor_exceptions();

        // we ignore NotImplemented while looking for the most significant 
        // error as long as there are other errors. NotImplemented will be
        // reported only if there are 'only' NotImplemented errors
        std::map<saga::error, std::string> errors;
        std::string notimpl(error_names[saga::NotImplemented]);

        // at the same time we look for exceptions thrown from the adaptor 
        // being the last know good adaptor for the stored API object
        saga::uuid last_known_good;
        TR1::shared_ptr<impl::object> impl(impl::runtime::get_impl_object_sp(obj));
        if (impl) 
            last_known_good = impl->get_last_known_good();

        std::vector<saga::exception>::const_iterator end = exceptions.end();
        for (std::vector<saga::exception>::const_iterator it = exceptions.begin(); 
            it != end; ++it)
        {
            saga::error err = (*it).get_error();
            if (err == saga::error(saga::adaptors::AdaptorDeclined) && 
                !keep_adaptor_exceptions)
            {
                continue;       // ignore AdaptorDeclined
            }

            if (err != saga::NotImplemented) {
                saga::uuid adaptor_id = impl::runtime::get_impl(*it)->get_failed_cpi();
                if (adaptor_id == last_known_good)
                    return (*it).get_message();   // we found an adaptor which previously succeeded

                // just store this error in the sorted list
                typedef std::map<saga::error, std::string>::value_type value_type;
                errors.insert(value_type(err, (*it).get_message()));
            }
            else if (notimpl.empty()) {
                notimpl = (*it).get_message();
            }
        }
        return errors.empty() ? notimpl : (*errors.begin()).second;
    }

    std::vector<std::string> 
    get_all_messages(std::vector<saga::exception> const& exceptions)
    {
        std::vector<std::string> messages;
        if (!exceptions.empty()) {
            bool keep_adaptor_exceptions = consider_adaptor_exceptions();

            std::vector<saga::exception>::const_iterator end = exceptions.end();
            for (std::vector<saga::exception>::const_iterator it = exceptions.begin(); 
                it != end; ++it)
            {
                saga::error err = (*it).get_error();
                if (err == saga::error(saga::adaptors::AdaptorDeclined) && 
                    !keep_adaptor_exceptions)
                {
                    continue;       // ignore AdaptorDeclined
                }
                messages.push_back((*it).get_message());
            }
        }
        return messages;
    }

}}

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    exception_list::exception_list()
    {
    }

    exception_list::exception_list(saga::exception const& e)
    {
        add(e);
    }

    void exception_list::add(saga::exception const& e)
    {
        exceptions_.push_back(e);
    }

    saga::error exception_list::get_error() const
    {
        return detail::get_error(exceptions_);
    }

    std::string exception_list::get_message() const
    {
        return detail::get_message(exceptions_);
    }

    std::vector<std::string> exception_list::get_all_messages() const
    {
        return detail::get_all_messages(exceptions_);
    }

    std::string exception_list::get_top_message() const
    {
        return detail::get_top_message(exceptions_);
    }

    std::size_t exception_list::get_error_count() const
    {
        return exceptions_.size();
    }

    std::vector<saga::exception> const& exception_list::get_all_exceptions() const
    {
        return exceptions_;
    }

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::impl
