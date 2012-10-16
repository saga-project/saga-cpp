
#include <saga/saga.hpp>

int main ()
{
  saga::job::description jd;

  jd.set_attribute ("executable",   "/bin/sleep");

  std::vector <std::string> attribs = jd.list_attributes ();

  for ( unsigned int i = 0; i < attribs.size (); i++ )
  {
    std::cout << i << " : " << attribs[i] << std::endl;
  }

  std::cout << "E: " << jd.get_attribute ("Executable") << std::endl;
  std::cout << "e: " << jd.get_attribute ("executable") << std::endl;;

  return 0;
}

