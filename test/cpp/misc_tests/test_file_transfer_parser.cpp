
#include <saga/saga.hpp>
#include <saga/saga/adaptors/file_transfer_spec.hpp>

#define LOCAL  "file://localhost/tmp/data_1"
#define REMOTE "ssh://remote.host.net/tmp/data_2"

namespace sa = saga::adaptors;

int main ()
{
  int                         err = 0;
  std::string                 spec;
  std::string                 left;
  std::string                 right;
  sa::file_transfer_operator  op;
    
  if (  true  != sa::parse_file_transfer_specification (LOCAL " > " REMOTE, left, op, right)
     || left  != LOCAL
     || op    != sa::copy_local_remote
     || right != REMOTE )
  {
    err++;
  }

  if (  true  != sa::parse_file_transfer_specification (LOCAL " >> " REMOTE, left, op, right)
     || left  != LOCAL
     || op    != sa::append_local_remote
     || right != REMOTE )
  {
    err++;
  }

  if (  true  != sa::parse_file_transfer_specification (LOCAL " < " REMOTE, left, op, right)
     || left  != LOCAL
     || op    != sa::copy_remote_local
     || right != REMOTE )
  {
    err++;
  }

  if (  true  != sa::parse_file_transfer_specification (LOCAL " << " REMOTE, left, op, right)
     || left  != LOCAL
     || op    != sa::append_remote_local
     || right != REMOTE )
  {
    err++;
  }

  return err;
}

