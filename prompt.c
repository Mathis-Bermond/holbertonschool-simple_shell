	#include "simple_shell.h"

	/**
	 * prompt - Displays the prompt and reads user input.
	 * description: This function displays the prompt, reads user input,
	 * and processes it.
	 * @info: Pointer to the shell_info structure.
	*/

	void prompt(shell_info_t *info)
	{
		char *line = NULL;
		size_t len = 0;
		ssize_t nread;
		char *token;
		int i = 0;

		if (isatty(STDIN_FILENO))  /* Afficher le prompt si mode interactif */
			write(STDOUT_FILENO, "$ ", 2);

		nread = getline(&line, &len, stdin);  /* Lire l'entrée de l'utilisateur */
		if (nread == -1)  /* End-of-file condition (Ctrl+D) */
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);  /* retour à la ligne avant de quitter */
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
		info->args[i] = NULL;  /* Terminer le tableau des arguments */

		execute_command(info);
		free(info->args);
		free(line);  /* Libérer la mémoire allouée */
	}
