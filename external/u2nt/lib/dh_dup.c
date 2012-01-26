#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int Downhill_dup (int fh)
{
  int fd = _dup(fh);
  if (-1 == fd && EBADF == errno)
  {
    /* it's probably not a file handle */
	  if ((fd = _open_osfhandle(fh, _O_BINARY)) >= 0)
	  {
	    int fd1 = _dup(fd);
      if (fd1 < 0)
      {
	      close(fd);
        return -1;
      }
      fd = fd1;
    }
  }
  return fd;
}


