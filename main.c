#include "simple_shell.h"

#define MAX_INPUT 1024

/**
* child_process - Executes the command in a child process.
* @info: Pointer to the shell information structure.
*
* This function forks a new process to execute the command specified
* in the shell information structure. It handles the execution of the
* command and any necessary cleanup in the child process.
*/
void child_process(shell_info_t *info)
{
	if (execve(info->cmd_path, info->args, environ) == -1)
	{
		perror("./hsh");
		exit(EXIT_FAILURE);
	}
}

/**
* parent_process - Waits for the child process to finish.
* @pid: Process ID of the child process.
* @status: Pointer to the status variable to store the exit status.
*
* This function waits for the child process to finish and handles
* any necessary cleanup in the parent process.
*/
void parent_process(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}

	while (!WIFEXITED(*status) && !WIFSIGNALED(*status))
	{
		if (waitpid(pid, status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
}

/**
* execute_command - Forks a process to execute a command.
* @info: Pointer to the shell information structure.
*
* This function forks a new process to execute the command specified
* in the shell information structure. It handles the execution of the
* command and any necessary cleanup in both the child and parent processes.
*/
void execute_command(shell_info_t *info)
{
	pid_t pid;
	int status;

	if (info->cmd_path == NULL)
	{
		perror("No command to execute");
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		child_process(info);
	}
	else
	{
		parent_process(pid, &status);
		info->status = status;
	}
}

/**
* parse_input - Parses the input string into command and arguments.
* @info: Pointer to the shell information structure.
*
* This function tokenizes the input string and stores the command and
* arguments in the shell information structure.
*/
void parse_input(shell_info_t *info)
{
	char *token;

	int i = 0;

	/* Allouer de la mémoire pour les arguments */
	info->args = malloc(MAX_INPUT * sizeof(char *));
	if (!info->args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(info->input, " ");
	while (token != NULL)
	{
		info->args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	info->args[i] = NULL;  /* Terminer le tableau des arguments */

	if (i > 0)
	{
		info->cmd_path = info->args[0];
	}
	else
	{
		info->cmd_path = NULL;
	}
}

/**
* main - Entry point of the shell program.
*
* This function initializes the shell information structure,
* reads input from the user, parses the input, and executes
* the command in a loop until the user exits.
*
* Return: Always 0.
*/
int main(void)
{
	shell_info_t info;
	char input[MAX_INPUT];
	ssize_t nread;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "$ ", 3);

		nread = read(STDIN_FILENO, input, MAX_INPUT);
		if (nread == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (nread == 0)
			break;

		if (nread > 0 && input[nread - 1] == '\n')
			input[nread - 1] = '\0';

		if (_strlen(input) == 0)
			continue;

		if (_strcmp(input, "exit") == 0)
			break;

		info.input = _strdup(input);
		if (!info.input)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		parse_input(&info);
		execute_command(&info);
		free(info.input);
		free(info.args);
	}
	return (0);
}
