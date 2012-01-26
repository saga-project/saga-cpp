
#include <iostream>
#include <sstream>
#include <set>

#include <errno.h>
#include <strings.h>
#include <pthread.h>

#include <saga/saga.hpp>

class stream_container
{
  private:
    saga::stream::stream s_;

  public:
    stream_container (saga::stream::stream s)
      : s_ (s)
    {
    }

    stream_container (const stream_container * sc)
      : s_ (sc->get_stream ())
    {
    }

    saga::stream::stream get_stream (void) const
    {
      return s_;
    }
};

bool work;

// manage a stream client
void * worker (void * data)
{
  pthread_t self = pthread_self ();
  std::cout << self << " thread created connect " << std::endl;

  stream_container     * sc (static_cast <stream_container*> (data));
  saga::stream::stream   s = sc->get_stream ();
  delete (sc);

  std::stringstream ss;
  ss << "HELO " << self << "\n";
  s.write (saga::buffer ((void*) (ss.str ().c_str ()), 
                         ss.str ().size ()));

  while ( work )
  {
    char buff[255];
    saga::ssize_t read_bytes = s.read (saga::buffer (buff));

    if ( read_bytes > 0 )
    {
      std::cout << self << " received " << std::string (buff, read_bytes) << std::endl;
    }

    ::sleep (1);
  }
  
  std::cout << "OLEH world";

  return data;
}



int main ()
{
  try 
  {
    saga::stream::server ss ("any://localhost:1234/");

    work = true;

    while ( true )
    {
      stream_container * sc = new stream_container (ss.serve ());

      std::cout << " client connect " << std::endl;

      pthread_t tid;

      if ( 0 != ::pthread_create (&tid, NULL, worker, static_cast <void*> (sc)) )
      {
        std::cerr << "pthread create error: " << ::strerror (errno) << std::endl;
        return (-1);
      }
    }

  }
  catch ( const saga::exception & e )
  {
    std::cerr << "exception: " << e.what () << std::endl;
  }

  ::sleep (10);

  return 0;

}

