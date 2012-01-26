
#include <iostream>
#include <sstream>
#include <set>

#include <errno.h>
#include <strings.h>
#include <pthread.h>

#include <saga/saga.hpp>

#define THREADS  100
#define URLS     100
#define ITER     100

#define URL_BASE "file://localhost/tmp/data_"

std::set <saga::url> set;
pthread_mutex_t      mtx;

void * worker (void * data)
{
  for ( int i = 0; i < ITER; i++ )
  {
    for ( int j = 0; j < URLS; j++ )
    {
      std::stringstream ss;
      ss << URL_BASE << j;
      saga::url u (ss.str ());

      // if ( ::pthread_mutex_lock (&mtx) )
      // {
      //   std::cerr << "mtx lock error: " << ::strerror (errno) << std::endl;
      //   ::exit (-3);
      // }

      if ( set.find (u) == set.end () )
      {
        std::cout << "-" << std::flush;
      }
      else
      {
        std::cout << "+" << std::flush;
      }

      set.insert (u);

      if ( set.find (u) == set.end () )
      {
        std::cout << "?" << std::flush;
      }
      else
      {
        std::cout << "!" << std::flush;
      }

      // if ( ::pthread_mutex_unlock (&mtx) )
      // {
      //   std::cerr << "mtx unlock error: " << ::strerror (errno) << std::endl;
      //   ::exit (-4);
      // }
    }
  }

  return data;
}

int main ()
{
  pthread_t threads[THREADS];

  if ( 0 != ::pthread_mutex_init (&mtx, NULL) )
  {
    std::cerr << "mtx init error: " << ::strerror (errno) << std::endl;
    return (-1);
  }


  for ( int i = 0; i < THREADS; i++ )
  {
    if ( 0 != ::pthread_create (&threads[i], NULL, worker, NULL) )
    {
      std::cerr << "pthread create error: " << ::strerror (errno) << std::endl;
      return (-2);
    }
  }


  for ( int i = 0; i < THREADS; i++ )
  {
    if ( 0 != ::pthread_join (threads[i], NULL) )
    {
      std::cerr << "pthread join error: " << ::strerror (errno) << std::endl;
      return (-3);
    }
  }



}

