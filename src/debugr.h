#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>

static const pid_t ignored_pid;
static const void *ignored_ptr;

static void setup_inferior(const char *path, char *const argv[])
{
	ptrace(PTRACE_TRACEME, ignored_pid, ignored_ptr, ignored_ptr);
	execv(path, argv);
}

static const void *no_continue_signal = 0;

static void attach_to_inferior(pid_t pid)
{
	while(1)
	{
		int status;
		waitpid(pid, &status, 0);
		
		if(WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP)
		{
			printf("Inferior stopped on SIGTRAP - continuing...\n");
			ptrace(PTRACE_CONT, pid, ignored_ptr, no_continue_signal);
		} 
		else if(WIFEXITED(status))
		{
			printf("Inferior exited - debugger terminating...\n");
			return;
		}
	}
}

void dbg_inferior_exec(const char *path, char *const argv[])
{
	pid_t result;
	do {
		result = fork();
		switch(result)
		{
			case 0: // Child process
				setup_inferior(path, argv);
				break;
			case -1: // Error
				printf("Error in fork(), Trying again\n");
				break;
			default:
				attach_to_inferior(result);
				break;
		}
	} while(result == -1 && errno == EAGAIN);
}
