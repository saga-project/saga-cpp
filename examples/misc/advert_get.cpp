
#include <saga/saga.hpp>

int main()
{
  try
  {
    saga::advert::entry ad ("advert://localhost/home/merzky/master_worker/0/0",
                            saga::advert::Read);

    if ( ad.attribute_exists ("state") )
    {
      std::cout << "state: " << ad.get_attribute ("state") << std::endl;
    }
    else
    {
      std::cerr << "no state found" << std::endl;
    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << "SAGA Exception: " << e.what ();
    return -1;
  }

  try
  {
    saga::advert::entry ad ("advert://localhost/home/merzky/master_worker/0/0",
                            saga::advert::Read);

    if ( ad.attribute_exists ("state") )
    {
      std::cout << "state: " << ad.get_attribute ("state") << std::endl;
    }
    else
    {
      std::cerr << "no state found" << std::endl;
    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << "SAGA Exception: " << e.what ();
    return -1;
  }

  return 0;
}

