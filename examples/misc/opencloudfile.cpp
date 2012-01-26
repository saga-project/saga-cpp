
#include <saga/saga.hpp>
#include <cstring>
#include <istream>

int main ()
{
  try 
  {
 
    /********** Setup ************/
    saga::session s;
    saga::context c ("opencloud");
    s.add_context  (c) ; 

    /********** Write File************/
    std::cout << std::endl ; 
    std::cout << std::endl ; 
    saga::filesystem::file f (s, "sector://test", saga::filesystem::ReadWrite ) ; 
    std::string towrite ("hello world !\n") ; 
    saga::const_buffer buf ( towrite.c_str(), towrite.length()) ; 
    f.write( buf, towrite.length()) ;  
    std::cout << "Written data: " << towrite ; 


    /********** Read File************/
    saga::mutable_buffer bufread ( towrite.length()) ; 
    f.read( bufread, towrite.length()) ; 
    std::cout << "Data read: " << (char*) bufread.get_data() ;  


    /********* Seek File ************/
    std::cout << "Seek to the end .. " << std::endl ; 
    f.seek( towrite.length(), saga::filesystem::Start ) ; 
    std::string w2 (" clouds!") ; 
    std::cout << "Write some more: " << w2 << std::endl ; 
    saga::const_buffer buf2( w2.c_str(), w2.length()) ; 
    f.write( buf2, w2.length()) ; 

    /********* Read new data ************/
    f.seek( 0, saga::filesystem::Start ) ; 
    saga::mutable_buffer bufread2( towrite.length() + w2.length() + 1 ) ; 
    f.read( bufread2, towrite.length() + w2.length() + 1 ) ; 
    std::cout << "Data read after seek and write: " << (char*) bufread2.get_data() << std::endl ; 

    f.close() ; 

    
    /********* Directory ***************/

    /*Create directories 
     */
    std::cout << std::endl ; 
    std::cout << "Create directory my_dir..." << std::endl ; 
    std::cout << "Create directory my_dir/another_dir..." << std::endl ; 
    saga::url dir ("sector://my_dir") ;
    saga::url dir2 ("sector://my_dir/another_dir") ;
    saga::filesystem::directory d( s, dir ) ; 
    saga::filesystem::directory d2( s, dir2 ) ; 

    /* Change working directory
     */
    std::cout << "Changing cwd to  my_dir/another_dir..." << std::endl ; 
    d.change_dir("another_dir") ; 
    std::cout << "Creating my_dir/another_dir/nested_dir ... " << std::endl ; 
    d.make_dir("nested_dir") ; 

    /* Populate Directories
     */
    std::cout << "Populate files in nested_dir ... " << std::endl ; 
    saga::filesystem::file f2 (s, "my_dir/another_dir/nested_dir/file1", saga::filesystem::ReadWrite ) ; 
    saga::filesystem::file f3 (s, "my_dir/another_dir/nested_dir/file2", saga::filesystem::ReadWrite ) ; 
    saga::filesystem::file f4 (s, "my_dir/another_dir/nested_dir/file3", saga::filesystem::ReadWrite ) ; 
    saga::filesystem::file f5 (s, "/makeme/file3", saga::filesystem::ReadWrite ) ; 
    f2.close() ; 
    f3.close() ; 
    f4.close() ; 
    f5.close() ; 

    /* List directory
     */
    d.change_dir("nested_dir") ; 
    std::vector <saga::url> listing = d.list() ; 
    std::cout << "List nested_dir ..." << std::endl ; 
    for ( std::vector<saga::url>::iterator i = listing.begin(); i != listing.end(); ++ i) 
    {
       std::cout << i->get_string() << std::endl ;  
    }

    /* Copy Files */
    d.copy( "file1", "sector:///" )  ; 

    return 0 ; 
   
  }
  catch ( const saga::exception & e )
  {
    std::cerr << e.what ();
  }
}

