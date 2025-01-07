#include "simple_shell.h"

/**
 * wait_sys - wait for child process to finish
 * @status: status of the child process
 * Return: void
 */

void wait_sys(void)
{
	pid_t pid = fork();
	int status;

	if (pid == -1)
	{
		perror("Error:");
		return;
	}
	if (pid == 0)
	{
		printf("Child process working...");
		printf("Child process done");
	}
	else
	{
		wait(&status);
		printf("Parent process: child exited");
	}
}