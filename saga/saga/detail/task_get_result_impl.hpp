//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <vector>
#include <string>

#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

#include <saga/saga/task.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/util.hpp>

#include <saga/impl/task.hpp>    // include the task implementation we want to use

namespace saga
{
    /// @cond 
    namespace detail
    {
        template <typename T>
        struct disable_reconvert : is_saga_object<T> {};

        template <typename T>
        struct disable_reconvert<std::vector<T> > : boost::mpl::true_ {};

        template <typename Retval>
        struct reconvert_result
        {
            template <typename Task>
            static Retval& call(Task& t, boost::mpl::false_)
            {
                // if the type we're interested in isn't directly supported, try to 
                // get a std::string assuming it can be converted explicitly
                std::string* str_retval = saga::detail::any_cast<std::string>(
                    &get_task_result(t));

                if (0 != str_retval)
                {
                    // now try to convert
                    try {
                        // re-assign stored value
                        get_task_result(t).assign(
                            boost::lexical_cast<Retval>(*str_retval));

                        // re-fetch the sored value
                        Retval* retval = saga::detail::any_cast<Retval>(
                                    &get_task_result(t));
                        if (0 != retval)
                            return *retval;
                    }
                    catch (boost::bad_lexical_cast const&) {
                        ;   // just fall through
                    }
                }

                // give up
                SAGA_THROW_VERBATIM(t, 
                    "Wrong data type requested while calling get_result", 
                    saga::NoSuccess);

                static Retval static_retval;
                return static_retval;
            }

            template <typename Task>
            static Retval& call(Task& t, boost::mpl::true_)
            {
                // just give up
                SAGA_THROW_VERBATIM(t, 
                    "Wrong data type requested while calling get_result", 
                    saga::NoSuccess);

                static Retval static_retval;
                return static_retval;
            }

            template <typename Task>
            static Retval& call(Task& t)
            {
                typedef boost::mpl::bool_<disable_reconvert<Retval>::value> 
                    predicate;
                return call(t, predicate());
            }
       };
    }

    // implement the get_result functions for the saga::task object
    template <typename Retval>
    Retval& task::get_result()
    {
        if (saga::task_base::Failed == get_task_if()->get_state())
        {
            get_task_if()->rethrow();     // must throw
        }

        Retval* retval = saga::detail::any_cast<Retval>(
            &get_task_result(*this));
        if (0 == retval)
            return detail::reconvert_result<Retval>::call(*this);

        return *retval;
    }

    template <typename Retval>
    Retval const& task::get_result() const
    {
        if (saga::task_base::Failed == get_task_if()->get_state())
        {
            get_task_if()->rethrow();     // must throw
        }

        Retval const* retval = saga::detail::any_cast<Retval const>(
            &get_task_result(*this));
        if (0 == retval)
            return detail::reconvert_result<Retval>::call(*this);

        return *retval;
    }
    /// @endcond 
}

