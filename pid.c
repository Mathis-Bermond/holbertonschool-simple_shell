#include "simple_shell.h"

/**
* pid - get the process id
* Return: void
*/

void pid(void)
{
	pid_t pid;

	pid = getpid();
	printf("%d\n", pid);
}
