#include "simple_shell.h"

#define MAX_INPUT 1024

/**
* get_command_path - Retrieves the full path of a command from PATH.
* @command: The command to search for.
*
* Return: A malloc'd string with the full path or NULL if not found.
*/
char *get_command_path(char *command)
{
	char *path_env = _getenv("PATH");
	char *path, *token, *full_path;
	size_t len;

	if (!path_env || !command)
		return (NULL);

	path = _strdup(path_env); /* Créer une copie de PATH */
	if (!path)
	{
		perror("strdup");
		return (NULL);
	}
	token = strtok(path, ":");
	while (token != NULL)
	{
		len = _strlen(token) + _strlen(command) + 2; /* Taille "token/command\0" */
		full_path = malloc(len);
		if (!full_path)
		{
			perror("malloc");
			free(path);
			return (NULL);
		}

		/* Construction du chemin avec sprintf */
		sprintf(full_path, "%s/%s", token, command);

		/* Vérifier si le chemin est exécutable */
		if (access(full_path, X_OK) == 0)
		{
			free(path); /* Libérer PATH car on a trouvé le chemin valide */
			return (full_path);
		}

		free(full_path); /* Libérer full_path si non valide */
		token = strtok(NULL, ":");
	}
	free(path); /* Libérer PATH si aucun chemin valide n'est trouvé */
	return (NULL);
}

/**
* child_process - Executes the command in a child process.
* @info: Pointer to the shell information structure.
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
* execute_command - Forks a process to execute a command.
* @info: Pointer to the shell information structure.
*/
void execute_command(shell_info_t *info)
{
	pid_t pid;
	int status;

	if (!info->cmd_path)
	{
		perror("Command not found");
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
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		info->status = status;
	}
}

/**
* parse_input - Parses the input string into command and arguments.
* @info: Pointer to the shell information structure.
*/
void parse_input(shell_info_t *info)
{
	char *token;

	int i = 0;

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
	info->args[i] = NULL;

	if (i > 0)
	{
		info->cmd_path = get_command_path(info->args[0]); /* Rechercher dans PATH */
		if (!info->cmd_path)
			info->cmd_path = _strdup(info->args[0]); /* Utiliser si PATH échoue */
	}
	else
	{
		info->cmd_path = NULL;
	}
}

/**
* main - Entry point of the shell program.
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
		if (info.cmd_path && info.cmd_path != info.args[0])
			free(info.cmd_path);
	}
	return (0);
}
