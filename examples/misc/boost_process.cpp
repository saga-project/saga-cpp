
#include <iostream>
#include <boost/process.hpp>

int main ()
{
  try 
  {
    boost::process::command_line cl ("/usr/bin/ssh");

    cl.argument ("-o");
    cl.argument ("StrictHostKeyChecking=no");
    cl.argument ("-i");
    cl.argument ("/home/amerzky/.ec2_keys/saga.aws_private_ec2.pem");
    cl.argument ("root@ec2-174-129-180-113.compute-1.amazonaws.com");
    cl.argument ("/tmp/saga-ec2-image-pre");

    boost::process::launcher l;

    boost::process::child c       = l.start (cl);
    boost::process::status status = c.wait ();

    if ( ! status.exited () || status.exit_status () )
    {
      std::cout << "exit status != 0\n";
    }
    else
    {
      std::cout << "exit status == 0\n";
    }
  }
  catch ( const boost::process::not_found_error <std::string> & e )
  {
    std::cerr << "exe not found\n";
  }
  catch ( const boost::process::system_error & e )
  {
    std::cerr << "exe failed\n";
  }

  return 0;
}

