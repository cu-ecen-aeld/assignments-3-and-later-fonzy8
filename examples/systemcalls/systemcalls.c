#include "systemcalls.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include "fcntl.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int istatus;
    bool iretval; 

    istatus = system(cmd);

    if ( WIFEXITED(istatus) ) {
	// normal program termination
	// This would give process exit status iretval = EXITSTATUS(istatus);
	iretval = true;
    } else {
	// stopped by signal or abnormal stop
	iretval = false;
    }

    return iretval;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    pid_t parent_pid = getpid(); // this is parents pid

    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
	printf("DBG : Command %d is %s\n", i, command[i]);
    }
    command[count] = NULL;

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    pid_t child_pid = fork();
    printf("DBG - Child process from parent %d created is %d\n",parent_pid, child_pid);

    if (child_pid == -1) {
	// failed to fork
	printf("DBG - Error in forking\n");
        return false;
    } else if ( child_pid > 0 ) {
	// we are the parent
	int status;
	waitpid (child_pid, &status, 0);
	if (WIFEXITED(status)) {
	    printf ("DBG - Child exit with status %d\n", WEXITSTATUS(status));
	    return (WEXITSTATUS(status) == 0);
	}
	if (WIFSIGNALED(status)) {
	    printf ("DBG - Child signal with status %d\n", WTERMSIG(status));
	    return false;
	}
    } else {
	// we are the child
        execv(command[0], command);   // this should never return
	printf("DBG - Unknown command %s in child process\n",command[0]);
  	exit(5);    // I've decided error 5 is unknown command. Just needs to be non-zero !
    }

    va_end(args);

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    pid_t parent_pid = getpid(); // this is parents pid

    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (fd < 0) { printf("DBG - Redirect file not opened\n"); return false; }
    
    pid_t child_pid = fork();
    printf("DBG - Child process from parent %d created is %d\n",parent_pid, child_pid);

    if (child_pid == -1) {
	// failed to fork
	printf("DBG - Error in forking\n");
        return false;
    } else if ( child_pid > 0 ) {
	// we are the parent
	close(fd);
	int status;
	waitpid (child_pid, &status, 0);
	if (WIFEXITED(status)) {
	    printf ("DBG - Child exit with status %d\n", WEXITSTATUS(status));
	    return (WEXITSTATUS(status) == 0);
	}
	if (WIFSIGNALED(status)) {
	    printf ("DBG - Child signal with status %d\n", WTERMSIG(status));
	    return false;
	}
    } else {
	// we are the child
	if (dup2(fd, 1) < 0) { printf("DBG - Dup2 failed\n"); return false; }
	close(fd);
	
        execv(command[0], command);   // this should never return
	printf("DBG - Unknown command %s in child process\n",command[0]);
  	exit(5);    // I've decided error 5 is unknown command. Just needs to be non-zero !
    }

    va_end(args);

    return true;
}
