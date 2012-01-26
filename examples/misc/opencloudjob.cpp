
#include <saga/saga.hpp>
#include <cstring>
#include <istream>
#include <cstring>

int main ()
{
  try 
  {
 
    /********** Setup ************/
    saga::session s;
    saga::job::description jd ; 
    std::string intarg ; 

    saga::context c ("opencloud");
    s.add_context  (c) ; 

    std::ostringstream sso ; 
    int in = 100 ; 
    void * ptr = &in ; 
    sso << ptr ; 
    intarg = sso.str() ; 

    /********** Declare Job attributes *********/
    std::cout << std::endl ; 
    std::cout << std::endl ; 

     /* The Arguments. The following syntax holds:
      *     input_file_dir output_file_dir function_name num_rows param_address<optional> param_size<optional>
      */
    std::vector<std::string> args  ; 
    args.push_back( "./html") ; 
    args.push_back( "/html/rindex") ; 
    args.push_back( "wordbucket") ; 
    args.push_back( "0" ) ; 
    args.push_back( intarg ) ; 

    std::ostringstream so ; 
    so << sizeof( int ) ; 
    std::string argsize  = so.str() ; 
    args.push_back( argsize ) ; 

    jd.set_vector_attribute( saga::job::attributes::description_arguments, args ) ; 
    jd.set_attribute( saga::job::attributes::description_executable, "./wordbucket" ) ; 
    std::vector<std::string> ft ; 
    ft.push_back( "./html" ) ; 
    jd.set_vector_attribute( saga::job::attributes::description_file_transfer, ft ) ; 

    saga::job::service js ( s ) ; 
    saga::job::job j = js.create_job( jd ) ; 

    j.run() ; 
    j.wait() ; 
   
  }
  catch ( const saga::exception & e )
  {
    std::cerr << e.what ();
  }
}

