/*****************************************************************************
 *                                                                           *
 * downhill.h                                                                *
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
 * 	[1997/06/23  12:11:46  tbartley]
 *
 * 	Add Downhill_Process_ForkexecEnv procedure.
 * 	[1997/06/05  02:40:44  gpilz]
 */


#if !defined(_DOWNHILL_DOWNHILL_H)
#define _DOWNHILL_DOWNHILL_H


/* Include stuff *************************************************************/
#if !defined(_WINSOCKAPI_)
#define _WINSOCKAPI_      /* avoid inclusion of winsock.h from windows.h */
#endif
#include <windows.h>
#include <io.h>
#include <process.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defined constants */
#define DOWNHILL_ARG_MAX 4096

/* Extern stuff **************************************************************/

#ifndef DOWNHILL_NO_FUNCTIONS

/* User stuff ============================================================== */
extern char* Downhill_User_Name;
extern uid_t Downhill_User_IdReal;
extern uid_t Downhill_User_IdEffective;
#if defined(_DOWNHILL_UNIX_SYSV)
extern uid_t Downhill_User_IdSaved;
#endif
extern char* Downhill_User_Password;
extern char* Downhill_User_Gecos;
extern char* Downhill_User_Dir;
extern char* Downhill_User_Shell;

/* Group stuff ============================================================= */
extern char* Downhill_Group_Name;
extern gid_t Downhill_Group_IdReal;
extern gid_t Downhill_Group_IdEffective;
#if defined(_DOWNHILL_UNIX_SYSV)
extern gid_t Downhill_Group_IdSaved;
#endif
extern char* Downhill_Group_Password;


/* Prototype stuff ***********************************************************/

/* Process stuff =========================================================== */
char* Downhill_Process_System(char* exec_Name,char* exec_Argv[],
       int* exec_Return,int file_Return);
pid_t Downhill_Process_Forkexec(char* exec_Name,char* exec_Argv[],
       HANDLE exec_Handle[],DWORD exec_Flags);
pid_t Downhill_Process_ForkexecEnv(char *exec_name,
				   char* exec_Argv[],
				   HANDLE exec_Handle[],
				   DWORD exec_Flags,
				   char* exec_Env[]);
pid_t Downhill_Process_Add(HANDLE process_Id);

/* Signal stuff ============================================================ */
void Downhill_Signal_Check(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif	/* DOWNHILL_NO_FUNCTIONS */

#endif /* _DOWNHILL_DOWNHILL_H */
