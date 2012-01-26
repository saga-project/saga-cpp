
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  // open a name space directory
  saga::url u (std::string ("file://localhost/") + getenv ("PWD"));
  saga::name_space::directory d (u);

  // list the contents
  std::vector <saga::url> entries = d.list ();

  // print the entries, and their type
  for ( unsigned int i = 0; i < entries.size (); i++ )
  {
    std::string type;
    // get some details for the entry
    if ( d.is_dir (entries[i]) )
    {
      type = "/";
    }
    // if a link (symbolic)
    else if ( d.is_link (entries[i]) )
    {
      type = " -> " + d.read_link (entries[i]).get_string();
    }
    // print the info
    std::cout << entries[i] << type << std::endl;
  }

  return (0);
}

