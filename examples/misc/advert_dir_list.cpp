
#include <saga/saga.hpp>

int main ()
{

  try
  {
    saga::advert::directory d1 ("/tmp/", saga::advert::ReadWrite);
    saga::advert::directory d2 = d1.open_dir ("100", saga::advert::Create    | 
                                                     saga::advert::Exclusive | 
                                                     saga::advert::ReadWrite );

    unsigned int n = 0;

    while ( true )
    {
      n++;

      std::vector <saga::url> entries = d2.list ();
      std::cout << "list # " << n << ": " << entries.size () << std::endl;
    }
  }
  catch (const saga::exception & e )
  {
    std::cerr << "saga exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

