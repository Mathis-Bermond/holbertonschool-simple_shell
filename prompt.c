#include "simple_shell.h"

/**
* prompt - Affiche le prompt et lit l'entrée utilisateur.
* @info: Pointeur vers la structure shell_info.
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

	line[nread - 1] = '\0'; /* Retirer le caractère de nouvelle ligne */
	info->input = line;

	/* Découper l'entrée en arguments ici, si nécessaire */
	token = strtok(info->input, " ");

	while (token != NULL)
	{
		info->args[i++] = token;
		token = strtok(NULL, " ");
	}
	info->args[i] = NULL;

	execute_command(info);

	free(line);
}

