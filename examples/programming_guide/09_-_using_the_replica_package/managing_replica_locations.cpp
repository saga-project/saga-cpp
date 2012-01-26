
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  // open a logical file
  saga::url u ("/replica_1");
  saga::replica::logical_file lf (u, saga::replica::Create
                                  |  saga::replica::ReadWrite);

  // Add a replica location, replicate the file
  lf.add_location    (saga::url ("file://localhost//etc/passwd"));
  lf.replicate       (saga::url ("file://localhost//tmp/passwd"),
                      saga::replica::Overwrite);

  // list all locations 
  std::vector <saga::url> replicas = lf.list_locations ();

  for ( unsigned int i = 0; i < replicas.size (); i++ )
  {
    std::cout << "replica: " << replicas[i] << std::endl;
  }

  // remove the first location
  lf.remove_location (replicas[0]);

  return (0);
}

