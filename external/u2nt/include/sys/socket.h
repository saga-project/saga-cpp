/*****************************************************************************
 *                                                                           *
 * sys/socket.h                                                              *
 *                                                                           *
 * Freely redistributable and modifiable.  Use at your own risk.             *
 *                                                                           *
 * Copyright 1994 The Downhill Project                                       *
 *                                                                           *
 *****************************************************************************/
/*
 * HISTORY
 * 	Allow selection of macro definitions only.
 * 	That is, provide a compatibility layer that requires no library
 * 	functions only macro definitions. This is enabled by defining the
 * 	macro DOWNHILL_NO_FUNCTIONS.
 * 	[1997/06/23  12:12:33  tbartley]
 *
 * 	Allow provision of an external system socket header file.
 * 	[1997/05/26  07:42:23  tbartley]
 */


#if !defined(_DOWNHILL_SYS_SOCKET_H)
#define _DOWNHILL_SYS_SOCKET_H

/* Include stuff *************************************************************/
#include "DOWNHILL/DH_POSIX.H"
#ifdef DOWNHILL_INCLUDE_SYS_SOCKET
#include DOWNHILL_INCLUDE_SYS_SOCKET
#else
#ifdef DOWNHILL_USE_WINSOCK2
#include <winsock2.h>
#else
#include <winsock.h>
#endif
#endif

struct msghdr
{
  void  * msg_name;         /* Socket name      */
  int   msg_namelen;        /* Length of name   */
  struct iovec *  msg_iov;  /* Data blocks      */
  int   msg_iovlen;         /* Number of blocks   */
  void  * msg_accrights;    /* Per protocol magic (eg BSD file descriptor passing) */
  int   msg_accrightslen;   /* Length of rights list */
};

/* Define stuff **************************************************************/
#if !defined(EWOULDBLOCK)
#define EWOULDBLOCK  WSAEWOULDBLOCK
#endif
#if !defined(ECONNRESET)
#define ECONNRESET   WSAECONNRESET
#endif
#if !defined(ETIMEDOUT)
#define ETIMEDOUT    WSAETIMEDOUT
#endif
#if !defined(EADDRINUSE)
#define EADDRINUSE   WSAEADDRINUSE
#endif
#if !defined(ECONNREFUSED)
#define ECONNREFUSED WSAECONNREFUSED
#endif
#if !defined(EINPROGRESS)
#define EINPROGRESS  WSAEINPROGRESS
#endif
#if !defined(EALREADY)
#define EALREADY     WSAEALREADY
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	DOWNHILL_NO_FUNCTIONS

/* Prototype stuff ***********************************************************/
#if (_DOWNHILL_POSIX_SOURCE == 0)
int  ruserok(char* hostname_Client,int root_Want,char* user_Client,
      char* user_Server);
int  rresvport(unsigned short* client_Port);
int  rcmd(char** server_Host,unsigned short server_Port,char* client_User,
     char* server_User,char* server_Command,int* client_Stderr);
void ruserpass(char* host_Name,char** user_Name,char** user_Password);
int  rexec(char** server_Host,unsigned short server_Port,char* server_User,
      char* server_Password,char* server_Command,int* client_Stderr);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif	/* DOWNHILL_NO_FUNCTIONS */

#endif /* _DOWNHILL_SYS_SOCKET_H */
