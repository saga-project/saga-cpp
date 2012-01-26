
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  try
  {
    saga::url u ("/path/which/does/not/exist");
    saga::filesystem::file f (u);
  }
  catch ( saga::state_exception const & e )
  {
    switch ( e.get_error () )
    {
      // handle does not exist
      case saga::DoesNotExist:
      {
        std::cout << "file does not exist"
                  << std::endl;
        exit (-1);
        break;
      }

      // generic handler for state related problems
      default:
      {
        std::cout << "some other saga state exception caught: "
                  << std::string (e.what ())
                  << std::endl;
        exit (0);
        break;
      }
    }
  }
}

