/*
 * HISTORY
 * 	Provide wrapper for <fcntl.h>.
 * 	This defined macros for O_ACCMODE, the mask for extracting the access
 * 	mode in the flags parameter of the open(2) call.
 * 	[1997/05/26  07:49:31  tbartley]
 */
#ifndef _DOWNHILL_FCNTL_H
#define _DOWNHILL_FCNTL_H

#include "DOWNHILL/DH_INC.H"
#include _DOWNHILL_INCLUDE_DIR/fcntl.h>

#ifndef _O_ACCMODE
#define _O_ACCMODE (_O_RDONLY|_O_WRONLY|_O_RDWR)
#endif
#ifndef O_ACCMODE
#define O_ACCMODE _O_ACCMODE
#endif

#define O_NOCTTY        0           // we don't need this on Windows
#define O_NONBLOCK      0

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#define F_SETFL         0

// we don't implement this
#if defined(__cplusplus)
inline int fcntl(int fildes, int cmd, ...) { return 0; }
#endif

#endif /* _DOWNHILL_FCNTL_H */
