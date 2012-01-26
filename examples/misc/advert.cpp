
#include <sstream>

#include <saga/saga.hpp>

class my_cb : public saga::callback
{
  public:
    bool cb (saga::monitorable o, 
             saga::metric      m, 
             saga::context     c)
    {
      std::cout << "cb was called\n";

      return true;
    }
};

int main()
{
  try
  {
    saga::advert::directory d ("/", 
                               saga::advert::Create    | 
                               saga::advert::ReadWrite );

    std::cout << "creating advert\n";

    saga::name_space::entry ns = d.open     ("tmp", saga::advert::ReadWrite );
    std::cout << "entry"<< std::endl;
    saga::advert::directory ad = d.open_dir ("tmp", saga::advert::ReadWrite );

    if ( ns.is_dir () )
    {
      std::cout << " d " << std::endl;
    }
    else
    {
      std::cout << " a " << std::endl;
    }

    my_cb cb;
    ad.add_callback (saga::advert::metrics::advert_modified, cb);

    int i = 0;
    while ( i < 10 )
    {
      ad.set_attribute ("key", boost::lexical_cast <std::string> (i++));
      ::getchar ();
    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << "SAGA Exception: " << e.what ();
    return -1;
  }

  return 0;
}

