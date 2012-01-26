
#include <saga/saga.hpp>
#include <saga/saga/adaptors/utils.hpp>

int main (int argc, char** argv)
{
  try
  {
    saga::adaptors::utils::process p ("/bin/sleep 10");
    p.run_sync ();

    std::cout << "out: " << p.get_out_s () << std::endl;
    std::cout << "err: " << p.get_err_s () << std::endl;
  }
  catch ( const std::exception & e )
  {
    std::cerr << "exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

