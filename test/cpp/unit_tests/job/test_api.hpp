//  Copyright (c) 2005-2007 Hartmut.kaiser@gmail.com)
//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#if !defined(SAGA_TEST_JOB_API_HPP)
#define SAGA_TEST_JOB_API_HPP

#include <boost/bind.hpp>
#include <boost/futures.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include "../test_saga.hpp"
#include "../macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_job
{
 namespace api
 {
    struct equipment
    {
        saga::url test_helper;
        std::string host;
        std::string contact;
        std::vector<std::string> arguments;

        std::string get_command_line() const
        {
            std::string result(test_helper.get_path());
            typedef std::vector<std::string>::const_iterator iterator;

            iterator end = arguments.end();
            for (iterator it = arguments.begin(); it != arguments.end(); ++it)
            {
                result += " " + *it;
            }
            return result;
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    struct PlainSync {};
    typedef boost::mpl::vector<
//             saga::task_base::Sync, 
//             saga::task_base::Async, 
//             saga::task_base::Task,
            PlainSync
        > apitypes;

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    inline void wait_on_futures(boost::futures::future<int> current)
    {
        current();
    }

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    template <typename TestFunctor, typename E, typename Threaded>
    struct call_test_exception
    {
        call_test_exception(equipment equipment)
          : equipment_(equipment) {}

        typedef int result_type;
        template <typename Tag>
        int operator()(Tag)
        {
            saga::error err = (saga::error)E::value;
            SAGA_REQUIRE_THROW(TestFunctor::test(Tag(), E(), equipment_), err);
            if(err != (saga::error)E::value)
            {
              std::string message;
              message += "Test " + TestFunctor::name + " failed with exception " +
                         saga::error_names[err] + " expecting " + 
                         saga::error_names[E::value] + " with threading ";
              if(boost::is_same<Threaded, boost::mpl::true_>::value)
                message += "enabled";
              else
                message += "disabled";
              message += " with api_type ";
              if(boost::is_same<Tag, saga::task_base::Sync>::value)
                message += "Sync";
              else if(boost::is_same<Tag, saga::task_base::Async>::value)
                message += "Async";
              else if(boost::is_same<Tag, saga::task_base::Task>::value)
                message += "Task";
              else if(boost::is_same<Tag, PlainSync>::value)
                message += "Normal";
              BOOST_ERROR(message.c_str());
            }
            return 0;
        }
        equipment equipment_;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    // call_test that is instantiated with the test to be run and
    // a generic error code including success
    template <typename TestFunctor, typename E, typename Threaded>
    struct call_test;
   
    template <typename TestFunctor, typename E>
    struct call_test<TestFunctor, E, boost::mpl::true_>
    {
        call_test(equipment equipment, 
                std::vector<boost::futures::future<int> > &futures)
          : equipment_(equipment), futures_(futures){}

        template <typename Tag>
        void operator()(Tag)
        {
            boost::futures::simple_future<int> f(
                boost::bind(call_test_exception<TestFunctor, E, boost::mpl::true_>(equipment_), Tag()));
            futures_.push_back(f);
        }
        equipment equipment_;
        std::vector<boost::futures::future<int> > &futures_;
    };

    template <typename TestFunctor, typename E>
    struct call_test<TestFunctor, E, boost::mpl::false_>
    {
        call_test(equipment equipment)
          : equipment_(equipment) {}

        template <typename Tag>
        void operator()(Tag)
        {
            boost::bind(call_test_exception<TestFunctor, E, boost::mpl::false_>(equipment_), Tag())();
        }
        equipment equipment_;
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename TestFunctor, typename Threaded>
    struct api_test;
    
    template <typename TestFunctor>
    struct api_test<TestFunctor, boost::mpl::true_>
    {
        api_test(equipment equipment)
          : equipment_(equipment) {}

        // The error is from the list of error_codes defined in the test 
        // structure the api_test is instantiated with
        template <typename Error>
        void operator()(Error)
        {
            std::vector<boost::futures::future<int> > futures;
            boost::mpl::for_each<apitypes>(
                call_test<TestFunctor, Error, boost::mpl::true_>(
                 equipment_, futures));
            std::for_each(futures.begin(), futures.end(), wait_on_futures);
        }
        equipment equipment_;
    };
    
    template <typename TestFunctor>
    struct api_test<TestFunctor, boost::mpl::false_>
    {
        api_test(equipment equipment)
          : equipment_(equipment) {}

        // The error is from the list of error_codes defined in the test 
        // structure the api_test is instantiated with
        template <typename Error>
        void operator()(Error)
        {
            boost::mpl::for_each<apitypes>(
                call_test<TestFunctor, Error, boost::mpl::false_>(
                 equipment_));
        }
        equipment equipment_;
    };
    
    // run_test gets instantiated with test to be run from test packages
    // passed from the package a utils for testing
    template <typename TestFunctor>
    struct run_test
    {
        static void execute(equipment equipment_)
        {
            // run tests in a non-futurized manner
            boost::mpl::for_each<typename TestFunctor::error_codes>(
                 api_test<TestFunctor, boost::mpl::false_>(equipment_));
                
            // run tests in a futurized manner (highly parallel)
            boost::mpl::for_each<typename TestFunctor::error_codes>(
                api_test<TestFunctor, boost::mpl::true_>(equipment_));
        }
    };
 } // api   
} // test_job

#endif // !SAGA_TEST_JOB_API_HPP
