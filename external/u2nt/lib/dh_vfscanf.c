/***
*vfprintf.c - fprintf from variable arg list
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vfprintf() - print formatted output, but take args from
*       a stdargs pointer.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int vfprintf(stream, format, ap) - print to file from varargs
*
*Purpose:
*       Performs formatted output to a file.  The arg list is a variable
*       argument list pointer.
*
*Entry:
*       FILE *stream - stream to write data to
*       char *format - format string containing data format
*       va_list ap - variable arg list pointer
*
*Exit:
*       returns number of correctly output characters
*       returns negative number if error occurred
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vfscanf (
        FILE *stream,
        const char *format,
        va_list ap
        )
/*
 * 'V'ariable argument 'F'ile (stream) 'SCAN', 'F'ormatted
 */
{
        int retval;

        _ASSERTE(stream != NULL);
        _ASSERTE(format != NULL);

#ifdef _MT
        _lock_str(stream);
        __try {
#endif  /* _MT */

        retval = (_input(stream,format,ap));

#ifdef _MT
        }
        __finally {
            _unlock_str(stream);
        }
#endif  /* _MT */

        return(retval);
}
