//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_JOB_HPP)
#define SAGA_TEST_JOB_HPP

#include <boost/test/unit_test.hpp>
#include "test_api.hpp"
#include "../test_functions.hpp"
#include "../test_attribute.hpp"

namespace fs = boost::filesystem;

namespace test_job
{
  ///////////////////////////////////////////////////////////////////////////
  void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
    std::string test_helper, std::string scheme, std::string host,
    std::vector<std::string> const& arguments);

  ///////////////////////////////////////////////////////////////////////////
  struct create_job
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);

          saga::job::description jd;
          jd.set_attribute(saga::job::attributes::description_executable, 
              equipment.test_helper.get_path());
          jd.set_vector_attribute(saga::job::attributes::description_arguments, 
              equipment.arguments);

          saga::task t = js.create_job<Tag>(jd);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          saga::job::job job = t.get_result<saga::job::job>();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::New);
          return 0;
      }
      template <typename Error>
      static int test(test_job::api::PlainSync t, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);

          saga::job::description jd;
          jd.set_attribute(saga::job::attributes::description_executable, 
              equipment.test_helper.get_path());
          jd.set_vector_attribute(saga::job::attributes::description_arguments, 
              equipment.arguments);

          saga::job::job job = js.create_job(jd);
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::New);
          return 0;
      }
  }; //create_job
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct list
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
      > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);
          std::vector<std::string> jobids;
          saga::job::job job1, job2, job3, job4;
          saga::job::ostream in1, in2, in3, in4;
          saga::job::istream out1, out2, out3, out4;
          saga::job::istream err1, err2, err3, err4;

          std::string command_line(equipment.get_command_line());

          job1 = js.run_job(command_line, equipment.host, in1, out1, err1);
          job2 = js.run_job(command_line, equipment.host, in2, out2, err2);
          job3 = js.run_job(command_line, equipment.host, in3, out3, err3);
          job4 = js.run_job(command_line, equipment.host, in4, out4, err4);

          SAGA_CHECK_EQUAL(job1.get_state(), saga::job::Running);
          SAGA_CHECK_EQUAL(job2.get_state(), saga::job::Running);
          SAGA_CHECK_EQUAL(job3.get_state(), saga::job::Running);
          SAGA_CHECK_EQUAL(job4.get_state(), saga::job::Running);

          std::string jobid1 = job1.get_job_id();
          std::string jobid2 = job2.get_job_id();
          std::string jobid3 = job3.get_job_id();
          std::string jobid4 = job4.get_job_id();

          saga::task t = js.list<Tag>();
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);
          jobids = t.get_result<std::vector<std::string> >();
          SAGA_CHECK_EQUAL(jobids.size(), 4u);

          SAGA_CHECK(jobids.end() != std::find(jobids.begin(), jobids.end(), jobid1));
          SAGA_CHECK(jobids.end() != std::find(jobids.begin(), jobids.end(), jobid2));
          SAGA_CHECK(jobids.end() != std::find(jobids.begin(), jobids.end(), jobid3));
          SAGA_CHECK(jobids.end() != std::find(jobids.begin(), jobids.end(), jobid4));
          in1 << "<exit>" << std::endl;
          in2 << "<exit>" << std::endl;
          in3 << "<exit>" << std::endl;
          in4 << "<exit>" << std::endl;

          SAGA_CHECK_EQUAL(job1.get_state(), saga::job::Done);
          SAGA_CHECK_EQUAL(job2.get_state(), saga::job::Done);
          SAGA_CHECK_EQUAL(job3.get_state(), saga::job::Done);
          SAGA_CHECK_EQUAL(job4.get_state(), saga::job::Done);

          t = js.list<Tag>();
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          jobids = t.get_result<std::vector<std::string> >();
          SAGA_CHECK_EQUAL(jobids.size(), 0u);
          return 0;
      }
      template <typename Error>
      static int test(test_job::api::PlainSync t, Error e, test_job::api::equipment equipment)
      {
          return 0;
      }
  }; //list
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct run_job
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
      > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);
          saga::job::job job;
          saga::task t;
          saga::job::ostream in;
          saga::job::istream out;
          saga::job::istream err;
  
          t = js.run_job<Tag>(equipment.get_command_line(), equipment.host, in, out, err);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          job = t.get_result<saga::job::job>();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Running);

          in << "<exit>" << std::endl;
          job.wait();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Done);
          return 0;
      }
      template <typename Error>
      static int test(test_job::api::PlainSync t, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);

          saga::job::ostream in;
          saga::job::istream out;
          saga::job::istream err;
          saga::job::job job = js.run_job(equipment.get_command_line(), equipment.host, in, out, err);
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Running);

          in << "<exit>" << std::endl;
          job.wait();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Done);
          return 0;
      }
  }; //run_job
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct get_job
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
      > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);

          saga::job::ostream in;
          saga::job::istream out, err;
          saga::job::job job = js.run_job( equipment.get_command_line(), 
                                           equipment.host, in, out, err);

          sleep(2);
          SAGA_CHECK_EQUAL (job.get_state(), saga::job::Running);
          std::string jobid = job.get_job_id();

          // reconnect to the running applications
          saga::task t = js.get_job<Tag>(jobid);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          saga::job::job job1 = t.get_result<saga::job::job>();
          SAGA_CHECK_EQUAL (job1.get_state(), saga::job::Running);
          SAGA_CHECK_EQUAL (jobid, job1.get_job_id());

          in << "<exit>" << std::endl;
          job1.wait();
          SAGA_CHECK_EQUAL(job1.get_state(), saga::job::Done);
          job.wait();
          SAGA_CHECK_EQUAL (job.get_state(), saga::job::Done);
          return 0;
      }
      template <typename Error>
      static int test(test_job::api::PlainSync t, Error e, test_job::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);

          saga::job::ostream in;
          saga::job::istream out, err;
          saga::job::job job = js.run_job( equipment.get_command_line(), 
                                           equipment.host, in, out, err);
          SAGA_CHECK_EQUAL (job.get_state(), saga::job::Running);
          std::string jobid = job.get_job_id();

          // reconnect to the running applications
          saga::job::job job1 = js.get_job(jobid);
          SAGA_CHECK_EQUAL (job1.get_state(), saga::job::Running);
          SAGA_CHECK_EQUAL (jobid, job1.get_job_id());

          in << "<exit>" << std::endl;
          job1.wait();
          SAGA_CHECK_EQUAL(job1.get_state(), saga::job::Done);
          job.wait();
          SAGA_CHECK_EQUAL (job.get_state(), saga::job::Done);
          return 0;
      }
  }; //get_job
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct suspend_resume
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success/*,
              saga::IncorrectState*/
      > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_job::api::equipment equipment)
      {
          SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
          {
            std::cout << " -> suspend_resume 1" << std::endl;
          }
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);
          saga::job::description jd;
          saga::job::job job;
          saga::task t;
          saga::job::ostream in;
          saga::job::istream out, err;
             
          if(saga::IncorrectState == Error::value) {
              jd.set_attribute(saga::job::attributes::description_executable, 
                               equipment.test_helper.get_path());
              jd.set_vector_attribute(saga::job::attributes::description_arguments, 
                                      equipment.arguments);
              SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
              {
                std::cout << " create: " << equipment.test_helper.get_path ();
                for ( unsigned int i = 0; i < equipment.arguments.size (); i++ )
                {
                  std::cout << " " << equipment.arguments[i];
                }
                std::cout << std::endl;
              }
              job = js.create_job(jd);
              SAGA_CHECK_EQUAL(job.get_state(), saga::job::New);
          }
          else {
              SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
              {
                std::cout << " run   : " << equipment.get_command_line () << std::endl;
              }
              job = js.run_job(equipment.get_command_line(), equipment.host, in, out, err);
              SAGA_CHECK_EQUAL(job.get_state(), saga::job::Running);
          }

          t = job.suspend<Tag>();
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Suspended);

          t = job.resume<Tag>();
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Running);

          in << "<exit>" << std::endl;
          job.wait();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Done);
          SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
          {
            std::cout << job.get_state() << std::endl;
            std::cout << " <- suspend_resume 1" << std::endl;
          }
          return 0;
      }
      template <typename Error>
      static int test(test_job::api::PlainSync t, Error e, test_job::api::equipment equipment)
      {
          SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
          {
            std::cout << " -> suspend_resume 2" << std::endl;
          }
          saga::session s = saga::get_default_session();
          saga::job::service js(s, equipment.contact);
          saga::job::description jd;
          saga::job::job job;
          saga::job::ostream in;
          saga::job::istream out, err;
 
          if(saga::IncorrectState == Error::value) {
              jd.set_attribute(saga::job::attributes::description_executable, 
                  equipment.test_helper.get_path());
              jd.set_vector_attribute(saga::job::attributes::description_arguments, 
                                      equipment.arguments);
              SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
              {
                std::cout << " create: " << equipment.test_helper.get_path ();
                for ( unsigned int i = 0; i < equipment.arguments.size (); i++ )
                {
                  std::cout << " " << equipment.arguments[i];
                }
                std::cout << std::endl;
              }
              job = js.create_job(jd);
              SAGA_CHECK_EQUAL(job.get_state(), saga::job::New);
          }
          else {
              SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
              {
                std::cout << " run   : " << equipment.get_command_line () << std::endl;
              }
              job = js.run_job(equipment.get_command_line(), equipment.host, in, out, err);
              SAGA_CHECK_EQUAL(job.get_state(), saga::job::Running);
          }

          job.suspend();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Suspended);

          job.resume();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Running);

          ::sleep (3);

          in << "<exit>" << std::endl;
          job.wait();
          SAGA_CHECK_EQUAL(job.get_state(), saga::job::Done);
          SAGA_VERBOSE (SAGA_VERBOSE_LEVEL_DEBUG)
          {
            std::cout << job.get_state() << std::endl;
            std::cout << " <- suspend_resume 2" << std::endl;
          }
          return 0;
      }
  }; //suspend_resume
  ///////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  inline void test_scalar_attributes(std::vector<std::string> const& attrs)
  {
      typedef std::vector<std::string>::const_iterator iterator;

      iterator end = attrs.end();
      for (iterator it = attrs.begin(); it != end; ++it) 
      {
          saga::job::description jd;
          test_attributes::check_attrib(jd, *it, "VALUE");
      }
  }

  inline void test_vector_attributes(std::vector<std::string> const& attrs)
  {
      typedef std::vector<std::string>::const_iterator iterator;

      iterator end = attrs.end();
      for (iterator it = attrs.begin(); it != end; ++it) 
      {
          saga::job::description jd;
          test_attributes::check_attrib_vector(jd, *it, "VALUE1", "VALUE2");
      }
  }

  inline void test_jd_attributes()
  {
      std::vector<std::string> vector_attrs;
      vector_attrs.push_back(saga::job::attributes::description_candidate_hosts);
      vector_attrs.push_back(saga::job::attributes::description_file_transfer);
      vector_attrs.push_back(saga::job::attributes::description_environment);
      vector_attrs.push_back(saga::job::attributes::description_arguments);
      vector_attrs.push_back(saga::job::attributes::description_candidate_hosts);

      test_vector_attributes(vector_attrs);

      std::vector<std::string> scalar_attrs;
      scalar_attrs.push_back(saga::job::attributes::description_executable);
      vector_attrs.push_back(saga::job::attributes::description_operating_system_type);
      vector_attrs.push_back(saga::job::attributes::description_cpu_architecture);
      scalar_attrs.push_back(saga::job::attributes::description_total_cpu_count);
      scalar_attrs.push_back(saga::job::attributes::description_spmd_variation);
      scalar_attrs.push_back(saga::job::attributes::description_number_of_processes);
      scalar_attrs.push_back(saga::job::attributes::description_processes_per_host);
      scalar_attrs.push_back(saga::job::attributes::description_threads_per_process);
      scalar_attrs.push_back(saga::job::attributes::description_working_directory);
      scalar_attrs.push_back(saga::job::attributes::description_interactive);
      scalar_attrs.push_back(saga::job::attributes::description_input);
      scalar_attrs.push_back(saga::job::attributes::description_output);
      scalar_attrs.push_back(saga::job::attributes::description_error);
      scalar_attrs.push_back(saga::job::attributes::description_cleanup);
      scalar_attrs.push_back(saga::job::attributes::description_job_start_time);
      scalar_attrs.push_back(saga::job::attributes::description_total_cpu_time);
      scalar_attrs.push_back(saga::job::attributes::description_total_physical_memory);
      scalar_attrs.push_back(saga::job::attributes::description_queue);

      test_scalar_attributes(scalar_attrs);

      saga::job::description jd;
      test_attributes::check_attrib_invalid(jd, "something_else");
  }

  ///////////////////////////////////////////////////////////////////////////
  namespace all
  {
    class suite : public boost::unit_test_framework::test_suite
    {
    private:
        typedef boost::unit_test_framework::test_suite base_type;

    public:
        struct add_tests
        {
            add_tests(suite& outer, test_job::api::equipment equipment) 
              : outer_(outer), equipment_(equipment) 
            {}

            template<typename Test>
            void operator()(Test)
            {
                outer_.add(
                    SAGA_TEST_CASE(
                        boost::bind(&api::run_test<Test>::execute, equipment_),
                        Test::name.c_str()
                    ), 0, 40); // timeout in sec.
            }

            suite& outer_;
            test_job::api::equipment equipment_;
        };

        suite (std::string test_helper, std::string scheme, std::string host,
               std::vector<std::string> const& arguments) 
          : base_type("saga::job::job test suite")
        {
            typedef boost::mpl::vector<
                create_job, 
                suspend_resume,
                run_job, 
                list, 
                get_job
            > apitests;

            test_job::api::equipment equipment_;
            saga::url test_helper_url(test_helper);
            test_helper_url.set_scheme(scheme);
            test_helper_url.set_host  (host);

            equipment_.test_helper = test_helper_url;
            equipment_.host = host;
            equipment_.contact = scheme + "://" + host + "/";
            equipment_.arguments = arguments;

            boost::mpl::for_each<apitests>(add_tests(*this, equipment_));

            // add attribute tests
            this->base_type::add(BOOST_TEST_CASE(&test_jd_attributes), 0, 40); 
       }

    };  //end suite for unit tests

  }//End namespace all
}//End namespace test_job
#endif // !SAGA_TEST_JOB_HPP
