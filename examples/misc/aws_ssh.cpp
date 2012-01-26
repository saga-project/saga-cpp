//  Copyright (c) 2005-2007 Andre Merzky <andre@merzky.net>
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE file or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>

void print_io (saga::job::job j)
{
  saga::job::istream out = j.get_stdout ();

  std::string line;

  while ( out.good () ) 
  {
    char c;

    out.read (&c, 1);

    if ( out.fail () ) 
      break; 

    std::cout << c;
  }

  return;
}

///////////////////////////////////////////////////////////////////////////////

void test_cloud (saga::job::description jd, 
                 std::string            type, 
                 std::string            instance = "")
{
  saga::url js_url (type + "://" + instance);

  std::cout << " ----------------------- (" << type << " - " << js_url << ") \n";


  // create a context for that cloud type
  saga::context c (type);
  std::cout << " =========== created context" << std::endl;
  
  // attach it to the session
  saga::session s;
  std::cout << " =========== created session" << std::endl;

  s.add_context (c);
  std::cout << " =========== added context" << std::endl;

  // create a job service in that session
  saga::job::service js (s, js_url);
  std::cout << " =========== created job service" << std::endl;

//  // we got the VM running - now use it as an ssh host
//  saga::url rm = js.get_url ();
//  rm.set_scheme ("ssh");
//  rm.set_path   ("");
//  rm.set_port   (22);
//  std::cout << " =========== ssh url:" << rm << std::endl;
//
//  // create ssh job service
//  saga::job::service sjs (rm);
//  std::cout << " =========== created ssh job service" << std::endl;

  // // // create and run a job on that job service (i.e. on that VM)
  // saga::job::job j = js.create_job (jd);
  // std::cout << " =========== created job " << std::endl;

  // j.run ();
  // std::cout << " =========== run job " << std::endl;

  // // // print job output
  // print_io (j);
  // std::cout << " =========== done job " << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
  try
  {
    // job description is shared by all sessions
    saga::job::description jd;

    std::vector <std::string> args;
    std::vector <std::string> env;

    // std::string exe ("/usr/local/saga/bin/saga-job");

    // args.push_back ("run");
    // args.push_back ("fork://localhost");
    // args.push_back ("/bin/date");

    // env.push_back  ("SAGA_VERBOSE=0");

    std::string exe ("uname");
    
    args.push_back ("-a");
    
    jd.set_attribute (saga::job::attributes::description_executable,         exe);
    jd.set_vector_attribute (saga::job::attributes::description_arguments,   args);
    jd.set_vector_attribute (saga::job::attributes::description_environment, env );
    jd.set_attribute (saga::job::attributes::description_interactive,        saga::attributes::common_true);

    //  FIXME: the job service contact should point to the ec2 service instance.
    //  Problem: what protocol to use?  We need to denotify the context type (do
    //  we?), _and_ the ec2 service protocol (http/https).  Anyway, this needs to 
    //  be fixed in the adaptor.
    //
    // "https://ec2.amazonaws.com/"
    // "http://mayhem9.cs.ucsb.edu:8773/services/Eucalyptus"
    // "https://tp-vm1.ci.uchicago.edu:8445/wsrf/services/ElasticNimbusService"
    // "https://vm02.cct.lsu.edu:8443/"

    // create a new instance on ec2, and run a test job
 // test_cloud (jd, "ec2");
 // test_cloud (jd, "ec2",        "i-2863e141");

    // same for eucalyptus
 // test_cloud (jd, "eucalyptus");
 // test_cloud (jd, "eucalyptus", "i-4C1308D3");

    // same for nimbus
 // test_cloud (jd, "nimbus");
 // test_cloud (jd, "nimbus",     "i-3247A527");
  
    // same for gumbocloud
 // test_cloud (jd, "gumbocloud");
 // test_cloud (jd, "gumbocloud", "i-43AC0847");

    // same for  eucalytptus on futuregrid
    test_cloud (jd, "fgeuca");
 // test_cloud (jd, "fgeuca", "i-331905B9");

  }
  catch ( saga::exception const & e )
  {
    std::cerr << e.what ();
  }

  return 0;
}

