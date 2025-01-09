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
	}

	/**
	* execute_command - Forks a process to execute a command.
	* @info: Pointer to the shell information structure.
	* description: This function forks a new process to execute the command
	* specified in the shell information structure. It handles the execution
	* of the
	* command and any necessary cleanup in both the child and parent processes.
	*/
	void execute_command(shell_info_t *info)
	{
	pid_t pid;
	int status;

	if (info->cmd_path == NULL)
	{
		fprintf(stderr, "Command not found\n");
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
	* description: This function tokenizes the input string and stores the command
	* and arguments in the shell information structure.
	* @info: Pointer to the shell information structure.
	*/
	void parse_input(shell_info_t *info)
	{
	char *token;

	int i = 0;

	/* Allocate memory for arguments */
	info->args = malloc(MAX_INPUT * sizeof(char *));
	if (!info->args)
	{
		perror("malloc");
		free(info->input);
		exit(EXIT_FAILURE);
	}

	token = strtok(info->input, " ");
	while (token != NULL)
	{
		info->args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	info->args[i] = NULL; /* Terminate the arguments array */

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
	* description: This function initializes the shell information structure,
	* reads input from the user, parses the input, and executes
	* the command in a loop until the user exits.
	* Return: Always 0.
	*/
	int main(void)
{
	shell_info_t info;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "$ ", 2);

		nread = getline(&line, &len, stdin);

		trim_whitespace(line);

		/* Ignorer les lignes vides ou avec uniquement des espaces */
		if (_strlen(line) == 0)
			continue;

		if (nread == -1) /* Gestion de Ctrl+D */
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Retirer le saut de ligne de l'entrée */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (_strcmp(line, "exit") == 0)
			break;

		info.input = line;
		parse_input(&info);
		execute_command(&info);
		free(info.args);
	}
	free(line);
	return (0);
}
