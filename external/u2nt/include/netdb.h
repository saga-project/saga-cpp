/*****************************************************************************
 *                                                                           *
 * netdb.h                                                                   *
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
 * 	[1997/06/23  12:11:56  tbartley]
 */


#if !defined(_DOWNHILL_NETDB_H)
#define _DOWNHILL_NETDB_H


/* Include stuff *************************************************************/
#include "DOWNHILL/DH_POSIX.H"
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DOWNHILL_NO_FUNCTIONS

/* Prototype stuff ***********************************************************/
#if (_DOWNHILL_POSIX_SOURCE == 0)
int sethostent(int host_Stayopen);
int endhostent(void);
#endif

#endif	/* DOWNHILL_NO_FUNCTIONS */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _DOWNHILL_NETDB_H */
