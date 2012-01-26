
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  // open a logical file
  saga::url u ("/replica_1");
  saga::replica::logical_file lf (u, saga::replica::Create
                                  |  saga::replica::ReadWrite);

  // get all attributes
  std::vector <std::string> keys = lf.list_attributes ();


  // print the keys and values
  for ( unsigned int i = 0; i < keys.size (); i++ )
  {
    std::string key = keys[i];
    std::string val;

    if ( lf.attribute_is_vector (key) )
    {
      std::vector <std::string> vals = lf.get_vector_attribute (key);
      val = vals[0] + " ...";
    }
    else
    {
      val = lf.get_attribute (key);
    }
    std::cout << key << " -> " << val << std::endl;
  }

  return (0);

}

