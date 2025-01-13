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
		/*execute la commande dans un processus enfant*/
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
	if (waitpid(pid, status, 0) == -1) /*Attend la fin du processus enfant*/
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	}

/**
* execute_command - Executes a command in a child process.
* @info: Pointer to the shell information structure.
*
* This function forks a new process to execute the command specified
* in the shell information structure. It handles the execution of the
* command and any necessary cleanup in the child process.
*/
void execute_command(shell_info_t *info)
{
	char *cmd_path; /*chemin de la commande a executer*/
	pid_t pid;
	int status; /* statut de sortie du processus enfant*/

	char *cmd = info->args[0]; /* commande de base extraite des arguments*/

	/* Si la commande contient un chemin absolu, on l'utilise tel quel */
	if (cmd[0] == '/')
	{
		cmd_path = cmd;
	}
	else /*sinon on appelle la fonction pour chercher la commande dans la path*/
	{
		cmd_path = find_command_in_path(cmd);
	}

	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", cmd);
		return;
	}

	/*on crée un nouveau processus enfant avec un pid de 0*/
	pid = fork();
	if (pid == 0)  /* Processus fils */
	{
		if (execve(cmd_path, info->args, environ) == -1)
		{
			perror("execve");
			if (cmd[0] != '/')
				free(cmd_path);
			exit(errno);
		}
	}
	else if (pid > 0)  /* Processus père */
	{
		wait(&status);  /* Attend la fin du processus fils */
		if (cmd[0] != '/')
			free(cmd_path);
	}
	else
	{
		perror("fork");
		if (cmd[0] != '/')
			free(cmd_path);
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

	/* Allocation de la mémoire pour les arguments */
	info->args = malloc(MAX_INPUT * sizeof(char *));
	if (!info->args)
	{
		perror("malloc");
		free(info->input);
		exit(EXIT_FAILURE);
	}
	/* découpe info->input en tokens séparés par des espaces*/
	token = strtok(info->input, " ");
	while (token != NULL)
	{
		info->args[i] = token; /*chaque token est ajouté a info->args*/
		token = strtok(NULL, " ");
		i++;
	}
	info->args[i] = NULL; /* Termine le tableau d'arguments par un pointeur null*/

	/* si au moins un token est trouvé, le 1er argument est défini */
	/* comme commande a executer */
	if (i > 0)
	{
		info->cmd_path = info->args[0];
	}
	else
	{
		info->cmd_path = NULL;
	}

	/* Si la commande est 'env', afficher l'environnement */
	if (info->cmd_path != NULL && _strcmp(info->cmd_path, "env") == 0)
	{
		print_env();
		return;
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
	/*taille initiale ajusté automatiquement si buffer doit etre redimensionné*/
	size_t len = 0;
	ssize_t nread;
	/*defini si le shell est en mode interactif ou non*/
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive) /*si interactif, affiche le prompt*/
			write(STDOUT_FILENO, "$ ", 2);
		/*lit une ligne d'entrée et la stocke dans line*/
		nread = getline(&line, &len, stdin);
		if (nread == -1) /* Gestion de Ctrl+D */
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);/*ecrit une nouvelle ligne sur la stdout*/
			break;
		}
		/* Retirer le saut de ligne de l'entrée */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		cut_whitespace(line); /*retire les espaces en début et fin*/

		/* Ignorer les lignes vides */
		if (_strlen(line) == 0)
			continue;

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
