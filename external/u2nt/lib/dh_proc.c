/*****************************************************************************
 *                                                                           *
 * DH_PROC.C                                                                 *
 *                                                                           *
 * Freely redistributable and modifiable.  Use at your own risk.             *
 *                                                                           *
 * Copyright 1994 The Downhill Project                                       *
 *                                                                           *
 *****************************************************************************/
/*
 * HISTORY
 * 	Fix the environment for downhill_Process_Forkexec.
 * 	[1997/06/10  01:09:17  patience]
 *
 * 	Add Downhill_Process_ForkexecEnv procedure.
 * 	[1997/06/05  02:40:43  gpilz]
 */
#if !defined(lint) && !defined(_NOIDENT)
static const char* rcsid = "@(#)$RCSfile: dh_proc.c,v $ $Revision: 1.1 $ (DOWNHILL) $Date: 2006/02/07 17:25:06 $";
#endif


/* Include stuff *************************************************************/
#include "INCLUDE/DH.H"
#include <TlHelp32.h>

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/param.h>

#ifdef __cplusplus
extern "C" {
#endif

void * __cdecl _alloca(size_t);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* Define stuff **************************************************************/
#define DOWNHILL_PROCESS_MAX 1024
#define INVALID_HANDLE (HANDLE)(-1)

/* Struct stuff **************************************************************/
struct downhill_Process_Struct
{
    pid_t pid[DOWNHILL_PROCESS_MAX];
    int   sigged[DOWNHILL_PROCESS_MAX];
};


/* Global stuff **************************************************************/
static struct downhill_Process_Struct* downhill_Process_Info = NULL;


/* Function stuff ************************************************************/

static void cleanup(void)
{
  free(downhill_Process_Info);
  downhill_Process_Info = NULL;
}

/* Init the process table ================================================== */
static int downhill_Process_Init(void)
{
    int process_Index;

    /* Skip this if it's already been done */
    if (downhill_Process_Info == NULL)
    {
        /* Get the memory for the process table */
        downhill_Process_Info = (struct downhill_Process_Struct *)
         calloc(sizeof(struct downhill_Process_Struct),1);
        if (downhill_Process_Info == NULL)
        {
            errno = ENOMEM;
            return FALSE;
        }

        /* And set it up */
        for (process_Index = 0;process_Index < DOWNHILL_PROCESS_MAX;
         process_Index++)
        {
            downhill_Process_Info->pid[process_Index] = INVALID_HANDLE;
        }
        
    atexit(cleanup);
    }

    /* Return that we're ready */
    return TRUE;
}

/* Find the index of a pid in the table ==================================== */
static int downhill_Process_GetIndex(pid_t process_Id)
{
    int process_Index;

    for (process_Index = 0;process_Index < DOWNHILL_PROCESS_MAX;
     process_Index++)
    {
        if (process_Id == downhill_Process_Info->pid[process_Index])
        {
            return process_Index;
        }
    }

    return -1;
}

/* Clear a pid entry from the table ======================================== */
static void downhill_Process_ClearPid(pid_t process_Id)
{
    int process_Index = downhill_Process_GetIndex(process_Id);

    if (process_Index != -1)
    {
        downhill_Process_Info->pid[process_Index] = INVALID_HANDLE;
        CloseHandle((HANDLE)process_Id);
    }
}

/* Check if a process is signaled ========================================== */
int downhill_Process_IsSignaled(pid_t process_Id)
{
    int process_Index = downhill_Process_GetIndex(process_Id);

    if (process_Index != -1)
    {
        return downhill_Process_Info->sigged[process_Index];
    }

    return FALSE;
}

/* Signal a pid entry in the table ========================================= */
void downhill_Process_Signal(pid_t process_Id)
{
    int process_Index = downhill_Process_GetIndex(process_Id);

    if (process_Index != -1)
    {
        downhill_Process_Info->sigged[process_Index] = TRUE;
    }
}

/* Get the status of a process ============================================= */
static pid_t downhill_Process_GetStatus(pid_t process_Id,int* process_Status)
{
    DWORD process_StatusTemp;

    /* Get the status of the process */
    if (GetExitCodeProcess((HANDLE)process_Id,&process_StatusTemp) ==
     FALSE)
    {
        errno = ECHILD;
        return INVALID_HANDLE;
    }

    /* Still living? */
    if (process_StatusTemp == STILL_ACTIVE)
    {
        return 0;
    }

    /* Return OK */
    if (process_Status != NULL)
    {
        *process_Status = (int)(((unsigned char)
         process_StatusTemp)<<8);
    }
    return process_Id;
}

/* Get the status of any process =========================================== */
pid_t downhill_Process_GetStatusAny(int* process_Status)
{
    int          loop_Index;
    static int   process_Index = 0;
    pid_t        process_Id;

    /* Make sure the info exists */
    if (downhill_Process_Init() == FALSE)
    {
        return INVALID_HANDLE;
    }

    /* Check any processes */
    for (loop_Index = 0;loop_Index < DOWNHILL_PROCESS_MAX;loop_Index++)
    {
        /* Check this process */
        if (downhill_Process_Info->pid[process_Index] != INVALID_HANDLE)
        {
            process_Id = downhill_Process_GetStatus(
             downhill_Process_Info->pid[process_Index],
             process_Status);

            /* Handle how things came out */
            if (process_Id == INVALID_HANDLE)
            {
                downhill_Process_Info->pid[process_Index] = INVALID_HANDLE;
            }
            else if (process_Id > 0)
            {
                return process_Id;
            }
        }

        process_Index++;
        if (process_Index == DOWNHILL_PROCESS_MAX)
        {
            process_Index = 0;
        }
    }

    return 0;
}

/* Count the number of active children ===================================== */
static int downhill_Process_CountChildren(void)
{
    int process_Index;
    int child_Count = 0;

    /* Make sure we're inited */
    if (downhill_Process_Init() == FALSE)
    {
        return 0;
    }

    /* Count the number of active children */
    for (process_Index = 0;process_Index < DOWNHILL_PROCESS_MAX;
     process_Index++)
    {
        if (downhill_Process_Info->pid[process_Index] != INVALID_HANDLE)
        {
            child_Count++;
        }
    }

    /* And return it */
    return child_Count;
}

/* Wait for any process to terminate ======================================= */
pid_t wait(int* process_Status)
{
    pid_t process_Id;

    /* Run through all processes */
    for (;;)
    {
        /* Check for interrupts */
        if (downhill_Signal_IsInterrupted())
        {
            errno = EINTR;
            return INVALID_HANDLE;
        }

        /* Make sure there are children */
        if (downhill_Process_CountChildren() == 0)
        {
            return INVALID_HANDLE;
        }

        /* Get any dead processes */
        process_Id = downhill_Process_GetStatusAny(process_Status);
        if (process_Id == INVALID_HANDLE)
        {
            return INVALID_HANDLE;
        }
        else if (process_Id > 0)
        {
            downhill_Process_ClearPid(process_Id);
            return process_Id;
        }

        /* Don't busy-loop */
        Sleep(DOWNHILL_SLEEP_TIME);
    }
}

/* Wait for a process to terminate ========================================= */
pid_t waitpid(pid_t process_Id,int* process_Status,int wait_Flags)
{
    pid_t wait_Pid;

    /* Waiting on more than one? */
    if (process_Id == 0)
    {
        /* Non-blocking */
        if (wait_Flags&WNOHANG)
        {
            if (downhill_Signal_IsInterrupted() > 0)
            {
                errno = EINTR;
                return INVALID_HANDLE;
            }
            wait_Pid = downhill_Process_GetStatusAny(
             process_Status);
            if (wait_Pid != 0)
            {
                downhill_Process_ClearPid(wait_Pid);
            }
            return wait_Pid;
        }

        /* Blocking */
        return wait(process_Status);
    }
    else
    {
        /* Non-blocking */
        if (wait_Flags&WNOHANG)
        {
            if (downhill_Signal_IsInterrupted() > 0)
            {
                errno = EINTR;
                return INVALID_HANDLE;
            }
            wait_Pid = downhill_Process_GetStatus(process_Id,
             process_Status);
            if (wait_Pid != 0)
            {
                downhill_Process_ClearPid(wait_Pid);
            }
            return wait_Pid;
        }

        /* Blocking */
        for (;;)
        {
            /* Check for interrupts */
            if (downhill_Signal_IsInterrupted() > 0)
            {
                errno = EINTR;
                return INVALID_HANDLE;
            }

            /* Should we return? */
            wait_Pid = downhill_Process_GetStatus(process_Id,
             process_Status);
            if (wait_Pid != 0)
            {
                if (wait_Pid > 0)
                {
                    downhill_Process_ClearPid(wait_Pid);
                }
                return wait_Pid;
            }

            /* Don't busy-loop */
            Sleep(DOWNHILL_SLEEP_TIME);
        }
    }
}

/* Wait for a process to terminate ========================================= */
#if (_DOWNHILL_POSIX_SOURCE == 0)
pid_t wait4(pid_t process_Id, int* process_Status,int wait_Flags,
       struct rusage* process_Usage)
{
    /* Just call waitpid() */
    process_Id = waitpid(process_Id,process_Status,wait_Flags);

    /* Dummy up the rusage entry */
    if (process_Usage != NULL)
    {
        memset(process_Usage,0,sizeof(struct rusage));
    }

    return process_Id;
}
#endif

/* Wait for any process to terminate ======================================= */
#if (_DOWNHILL_POSIX_SOURCE == 0)
pid_t wait3(int* process_Status,int wait_Flags,struct rusage* process_Usage)
{
    /* Just call wait4() */
    return wait4(0,process_Status,wait_Flags,process_Usage);
}
#endif

/* Add a process to the list =============================================== */
pid_t Downhill_Process_Add(HANDLE process_Handle)
{
    int process_Index;

    /* Make sure the table is there */
    if (downhill_Process_Init() == FALSE)
    {
        return INVALID_HANDLE;
    }

    /* Run through each pid */
    for (process_Index = 0;process_Index < DOWNHILL_PROCESS_MAX;
     process_Index++)
    {
        /* If we find an empty slot (or they're re-adding the pid),
           add the info */
        if ((downhill_Process_Info->pid[process_Index] == INVALID_HANDLE)
         || (downhill_Process_Info->pid[process_Index] ==
         (pid_t)process_Handle))
        {
            downhill_Process_Info->pid[process_Index] =
             (pid_t)process_Handle;
            downhill_Process_Info->sigged[process_Index] = FALSE;

            return (pid_t)process_Handle;
        }
    }

    return 0;
}

/* Run a program in the background ========================================= */
static HANDLE
downhill_Process_Forkexec(char* exec_Name,
              char* exec_Argv[],
              HANDLE file_Handle[],
              DWORD exec_Flags,
              char* exec_Env[])
{
    char                exec_Command[DOWNHILL_ARG_MAX];
    STARTUPINFO         exec_Startup;
    PROCESS_INFORMATION exec_Info;
    int                 exec_Index;
    int                 exec_Inherit = FALSE;
        char                *exec_Envp = NULL;
    
    /* Make sure the table is there */
    if (downhill_Process_Init() == FALSE)
    {
        return INVALID_HANDLE_VALUE;
    }

    /* Build the command */
    strcpy(exec_Command, exec_Name);
    if (exec_Argv != NULL)
    {
        /*
         * If we were asked to, check every arguement for spaces
         * and place single quotes around it if there are.
         */
        for (exec_Index = 1; exec_Argv[exec_Index] != NULL; exec_Index++)
        {
        BOOLEAN end_quote = FALSE;
        
        if (strchr(exec_Argv[exec_Index], ' ') != NULL)
        {
            strcat(exec_Command," '");
            end_quote = TRUE;
        }
        else
            strcat(exec_Command," ");

        strcat(exec_Command,exec_Argv[exec_Index]);
        
        if (end_quote == TRUE)
            strcat(exec_Command,"'");
        }
    }

    /* Run the command */
    memset(&exec_Startup,0,sizeof(STARTUPINFO));
    exec_Startup.cb = sizeof(STARTUPINFO);
    if (file_Handle != NULL)
    {
        if (file_Handle[0] != INVALID_HANDLE_VALUE)
        {
            exec_Startup.hStdInput = file_Handle[0];
            exec_Inherit = TRUE;
            exec_Startup.dwFlags = STARTF_USESTDHANDLES;
        }
        if (file_Handle[1] != INVALID_HANDLE_VALUE)
        {
            exec_Startup.hStdOutput = file_Handle[1];
            exec_Inherit = TRUE;
            exec_Startup.dwFlags = STARTF_USESTDHANDLES;
        }
        if (file_Handle[2] != INVALID_HANDLE_VALUE)
        {
            exec_Startup.hStdError = file_Handle[2];
            exec_Inherit = TRUE;
            exec_Startup.dwFlags = STARTF_USESTDHANDLES;
        }
    }
        /* PW need to make the environment a list of null terminated
         * strings.
         */
        if (exec_Env != NULL) {
           char **envp, *tmpp;
           int  length = 1;

           for (envp = exec_Env; *envp != NULL; envp++) {
               length += (int)strlen(*envp) + 1;
           } 
           if ((exec_Envp = (char *)_alloca(length)) != NULL) { 
               for (envp = exec_Env, tmpp=exec_Envp; *envp != NULL; envp++) {
                   strcpy(tmpp, *envp); 
                   tmpp += strlen(*envp) + 1;
               }
               *tmpp = '\0';
           }
        }
    if (!CreateProcess(NULL,      /* application name */
               exec_Command,      /* command line string */
               NULL,              /* process security attribs */
               NULL,              /* thread security attribs */
               exec_Inherit,      /* handle inheritance flag */
               exec_Flags,        /* creation flags */
               exec_Envp,         /* new environment block */
               NULL,              /* current directory name */
               &exec_Startup,     /* startup information */
               &exec_Info))       /* process information */
    {
        return INVALID_HANDLE_VALUE;
    }

    /* Clean up the thread */
    CloseHandle(exec_Info.hThread);

    /* Return the result */
    return exec_Info.hProcess;
}

/* Run a file in the background ============================================ */
pid_t Downhill_Process_Forkexec(char* exec_Name,
                char* exec_Argv[],
                HANDLE file_Handle[],
                DWORD exec_Flags)
{
    HANDLE exec_Handle = downhill_Process_Forkexec(exec_Name,
                               exec_Argv,
                               file_Handle,
                               exec_Flags,
                               NULL);

    if (exec_Handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE;
    }
    return Downhill_Process_Add(exec_Handle);
}

pid_t Downhill_Process_ForkexecEnv(char* exec_Name,
                   char* exec_Argv[],
                   HANDLE file_Handle[],
                   DWORD exec_Flags,
                   char* exec_Env[])
{
    HANDLE exec_Handle = downhill_Process_Forkexec(exec_Name,
                               exec_Argv,
                               file_Handle,
                               exec_Flags,
                               exec_Env);

    if (exec_Handle == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE;
    }
    return Downhill_Process_Add(exec_Handle);
}

/* Run a file and return the results ======================================= */
char* Downhill_Process_System(char* exec_Name,char* exec_Argv[],
       int* exec_Return,int file_Return)
{
    HANDLE              exec_Handle;
    HANDLE              dump_Handle[3];
    static char         dump_File[MAXPATHLEN+1];
    char*               dump_Dir;
    SECURITY_ATTRIBUTES dump_Security;
    int                 result_Status;
    struct stat         result_Stat;
    char*               result_Mem;
    int                 result_Fd;

    /* Create the dump file */
    dump_Dir = getenv("TEMP");
    if (dump_Dir == NULL)
    {
        dump_Dir = getenv("TMP");
        if (dump_Dir == NULL)
        {
            dump_Dir = ".";
        }
    }
    strcpy(dump_File,dump_Dir);
    if ((dump_File[strlen(dump_File)-1] != '/') && (dump_File[strlen(
     dump_File)-1] != '\\'))
    {
        strcat(dump_File,"/");
    }
    strcat(dump_File,"DHXXXXXX");
    _mktemp(dump_File);
    strcat(dump_File,".TMP");
    dump_Security.nLength = sizeof(SECURITY_ATTRIBUTES);
    dump_Security.lpSecurityDescriptor = NULL;
    dump_Security.bInheritHandle = TRUE;
    dump_Handle[0] = INVALID_HANDLE_VALUE;
    dump_Handle[1] = dump_Handle[2] = CreateFile(dump_File,
     GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
     &dump_Security,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,
     INVALID_HANDLE_VALUE);
    if (dump_Handle[1] == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    /* Run the process */
    exec_Handle = downhill_Process_Forkexec(exec_Name,
                        exec_Argv,
                        dump_Handle,
                        0,
                        NULL);
    CloseHandle(dump_Handle[1]);
    if (exec_Handle == INVALID_HANDLE_VALUE)
    {
        errno = EINVAL;
        unlink(dump_File);
        return NULL;
    }
    for (;;)
    {
        Downhill_Signal_Check();
        if (GetExitCodeProcess(exec_Handle, (DWORD *)&result_Status) == FALSE)
        {
            CloseHandle(exec_Handle);
            errno = EINVAL;
            unlink(dump_File);
            return NULL;
        }
        if (result_Status != STILL_ACTIVE)
        {
            CloseHandle(exec_Handle);
            break;
        }
        Sleep(DOWNHILL_SLEEP_TIME);
    }

    /* Return the name if that's what they want */
    if (file_Return != 0)
    {
        return dump_File;
    }

    /* Read the file */
    if (stat(dump_File,&result_Stat) != 0)
    {
        errno = EINVAL;
        unlink(dump_File);
        return NULL;
    }
    result_Mem = (char *)calloc(result_Stat.st_size+1,1);
    if (result_Mem == NULL)
    {
        errno = ENOMEM;
        unlink(dump_File);
        return NULL;
    }
    result_Fd = open(dump_File,O_RDONLY|O_BINARY);
    if (read(result_Fd,result_Mem,result_Stat.st_size) < 0)
    {
        close(result_Fd);
        unlink(dump_File);
        return NULL;
    }
    close(result_Fd);
    unlink(dump_File);

    /* And return */
    return result_Mem;
}

/* ------------------------------------------------------------------------- */
int Downhill_getppid(void)
{
HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
PROCESSENTRY32 pi;
int parent_id = -1;

  memset(&pi, 0, sizeof(PROCESSENTRY32));
  pi.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hSnapShot, &pi))
  {
    do {
      if (pi.th32ProcessID == GetCurrentProcessId()) 
      {
        parent_id = pi.th32ParentProcessID;
        break;
      }
    } while (Process32Next(hSnapShot, &pi));
  }
  CloseHandle(hSnapShot);
  return parent_id;
}

