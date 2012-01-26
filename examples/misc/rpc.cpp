
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    // 'method' to call (aka exe)
    saga::rpc::rpc r ("shell-rpc://localhost/usr/bin/uname");

    // parameters: IN are command line params, OUT is buffer for result/stdout
    std::vector <saga::rpc::parameter> params;

    params.push_back (saga::rpc::parameter (::strdup ("-a"),   2, saga::rpc::In ));
    params.push_back (saga::rpc::parameter (new char[1024], 1024, saga::rpc::Out));

    // do the rpc call
    r.call (params);

    // and print result
    std::cout << (const char*) params[1].get_data () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "catched error: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

