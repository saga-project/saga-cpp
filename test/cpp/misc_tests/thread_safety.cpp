//
//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <saga/saga.hpp>

#include <iostream>

#define N 1000


int main (int argc, char** argv)
{
  try
  {
    saga::task_container tc;

    saga::filesystem::file f ("./thread_safety_test.src", saga::filesystem::Create);

    for ( unsigned int n = 0; n < N; n++ )
    {
      saga::task t = f.copy <saga::task_base::Async> ("./thread_safety_test.tgt",
                                                      saga::filesystem::Overwrite);
      tc.add_task (t);
    }


    unsigned int n_done = 0;
    while ( n_done < N )
    {
      std::vector <saga::task> done = tc.wait (saga::task_container::Any);
      std::cerr << "finished: " << done.size() << std::endl;
      n_done += done.size();
//       for ( unsigned int i = 0; i < done.size (); i++ )
//       {
//         tc.remove_task (done[i]);
//         n_done ++;
//       }
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "unexpected exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

