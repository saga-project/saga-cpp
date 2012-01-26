
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
   namespace sja = saga::job::attributes;

   saga::job::description jd;
   jd.set_attribute (sja::description_executable, "/bin/date");

   saga::job::service js;
   saga::job::job j = js.create_job (jd);

   j.run ();

   std::cout << j.get_job_id () << std::endl;
   
   saga::job::state state = j.get_state ();
   while ( state != saga::job::Done     &&
           state != saga::job::Failed   &&
           state != saga::job::Canceled )
   {
     std::cout << "state: " << state << std::endl;

     ::usleep (10000);
     state = j.get_state ();
   }

   std::cout << "done" << std::endl;
}

