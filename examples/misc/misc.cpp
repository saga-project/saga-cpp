

#include <saga/saga.hpp>

int test_1 (int x, int y)
{
  int i = x; 
  int j = y; 
  int k = i + j;
  return k;
}

int test_2 (int z)
{
  int l = z * z; 
  return l + z;
}

#define TEST_MACRO(x,y) int z = test_1 (x, y); \
                        int a = test_2 (z); \
                        std::cout << a << std::endl;


// int main (int argc, char** argv)
// {
//   saga::url js_url("fork://localhost");
// 
//   saga::job::description jd;
//   jd.set_attribute (saga::job::attributes::description_executable, "/bin/cat");
//   jd.set_attribute (saga::job::attributes::description_interactive, saga::attributes::common_false);
// 
//   std::vector<std::string> args;
//   args.push_back("/tmp/t");
// 
//   jd.set_vector_attribute(saga::job::attributes::description_arguments, args);
// 
//   saga::job::service js (js_url);
//   saga::job::job job = js.create_job (jd);
// 
//   job.run ();
// 
//   //// reconnect within the same instance
//   std::string jobID = job.get_job_id();
//   saga::job::job job2 = js.get_job (jobID);
//   std::cout << "Job State: " << saga::job::detail::get_state_name(job.get_state()) << std::endl;
// 
//   return 0;
// }

int main (int argc, char* argv[])
{
  TEST_MACRO (1,2);

  saga::url js_url("fork://localhost");

  saga::job::description jd;
  jd.set_attribute (saga::job::attributes::description_executable,
                    "/bin/date");
  jd.set_attribute (saga::job::attributes::description_interactive,
                    saga::attributes::common_false);

  // std::vector<std::string> args;
  // args.push_back("/tmp/ALOHA_SAGA");
  // jd.set_vector_attribute(saga::job::attributes::description_arguments, args);

  saga::job::service js (js_url);
  saga::job::job job = js.create_job (jd);

  job.run ();

  while(1) {
    std::cout << "Job State: " <<
      saga::job::detail::get_state_name(job.get_state()) << std::endl;
    sleep(1);
  }

  return 0;
}

