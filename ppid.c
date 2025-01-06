#include "simple_shell.h"

/**
* ppid - get the parent process id
* Return: void
*/

void ppid(void)
{
	pid_t ppid;

	ppid = getppid();
	printf("%d\n", ppid);
}
