#include <saga/saga.hpp>
#include <boost/thread/thread.hpp>

std::string url ("any://localhost/");
int exit_code = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////
//
void stream_server() 
{
  try
  {
	std::cout << "Serving " << url << std::endl;

	// actual functionality
	saga::stream::server service (url);

  saga::stream::stream strm = service.serve ();

  std::cout << "Established connection from: " 
			<< strm.get_url () 
			<< std::endl;

  char buff[255];
  saga::ssize_t read_bytes = strm.read (saga::buffer(buff));
  strm.write (saga::buffer (buff, read_bytes));


  }
  catch ( saga::exception const & e )
  {
	std::cerr << "Exception: " << e.what () << std::endl;
	++exit_code;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
void stream_client()
{
  try 
  {
    std::cout << "Connecting to " << url << std::endl;

    saga::stream::stream strm (url);
    strm.connect ();

    std::cout << "Connected!" << std::endl;

    std::string msg ("test from client\n");
    std::cout << "Sending : " << msg;
    strm.write (saga::buffer (msg));

    char buff[255];
    saga::ssize_t read_bytes = strm.read (saga::buffer (buff));

    std::cout << "Received: " << std::string (buff, read_bytes) << std::endl;
  } 
  catch ( saga::exception const & e )
  {
    std::cerr << "saga::exception caught: " << e.what () << std::endl;
	++exit_code;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
int main (int argc, char** argv)
{
	boost::thread server_thrd(&stream_server);
	sleep(2);
	boost::thread client_thrd(&stream_client);
  	
  	server_thrd.join();
  	client_thrd.join();
  	
  	return exit_code;
}

