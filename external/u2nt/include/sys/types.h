/*****************************************************************************
 *                                                                           *
 * sys/types.h                                                               *
 *                                                                           *
 * Freely redistributable and modifiable.  Use at your own risk.             *
 *                                                                           *
 * Copyright 1994 The Downhill Project                                       *
 *                                                                           *
 *****************************************************************************/

#if !defined(_DOWNHILL_SYS_TYPES_H)
#define _DOWNHILL_SYS_TYPES_H


/* Typedef stuff *************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Good typedefs =========================================================== */
typedef long           uid_t;
typedef long           gid_t;
typedef long           pid_t;
typedef unsigned short mode_t;
typedef short          nlink_t;
typedef char*          caddr_t;

/* Bad typedefs ============================================================ */
typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int   u_int;
typedef unsigned long  u_long;
typedef u_int          uint;
typedef u_short        ushort;

#ifdef __cplusplus
} /* extern "C" */
#endif

/* Include stuff *************************************************************/
/* Note: This stuff has to go after the typedefs because these files include
         files that use the new types. */
#define _WINSOCKAPI_    /* don't #include <winsock.h> */
#include "DOWNHILL/DH_INC.H"
#include _DOWNHILL_INCLUDE_DIR/sys/types.h>
#include _DOWNHILL_INCLUDE_DIR2/winsock2.h>
#include _DOWNHILL_INCLUDE_DIR2/windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_OFF_T_DEFINED)
typedef _off_t         off_t;
#define _OFF_T_DEFINED
#endif
#if !defined(_SSIZE_T_DEFINED)
# ifdef   WIN64
typedef signed __int64    ssize_t;
# else
typedef _W64 signed int   ssize_t;
# endif
#define _SSIZE_T_DEFINED
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _DOWNHILL_SYS_TYPES_H */
