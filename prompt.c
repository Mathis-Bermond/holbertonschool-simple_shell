#include "simple_shell.h"

/**
* prompt - Displays a prompt and reads user input.
* @info: Pointer to the shell information structure.
*/
void prompt(shell_info_t *info)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *token;
	int i = 0;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		exit(EXIT_SUCCESS);
	}

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	info->input = line;

	info->args = malloc(MAX_INPUT * sizeof(char *));
	if (!info->args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(info->input, " ");
	while (token != NULL)
	{
		info->args[i++] = token;
		token = strtok(NULL, " ");
	}
	info->args[i] = NULL;
	execute_command(info);
	free(info->args);
	free(line);
}
