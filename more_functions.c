#include "simple_shell.h"

/**
* _getenv - Gets the value of an environment variable
* @name: The name of the environment variable
*
* Return: Pointer to the value of the env variable, or NULL if not found
*/
char *_getenv(const char *name)
{
	int i = 0, j;

	if (!name || !environ)
		return (NULL);

	while (environ[i])
	{
		j = 0;
		while (name[j] && environ[i][j] && name[j] == environ[i][j])
			j++;

		if (name[j] == '\0' && environ[i][j] == '=')
			return (environ[i] + j + 1);

		i++;
	}

	return (NULL);
}

/**
* trim_whitespace - Removes leading and trailing spaces from a string
* @str: The string to be trimmed
*/
void trim_whitespace(char *str)
{
	int start = 0;

	int i;

	int end = _strlen(str) - 1;

	/* Remove leading spaces */
	while (str[start] == ' ' && str[start] != '\0')
		start++;

	/* Remove trailing spaces */
	while (end > start && str[end] == ' ')
		end--;

	/* Shift the string to the left */
	if (start > 0)
	{
		for (i = 0; str[start + i] != '\0'; i++)

		{
			str[i] = str[start + i];
		}
		str[end - start + 1] = '\0';  /* Null-terminate the string */
	}

	/* Now we also handle the case where the string was only spaces */
	if (end < 0)
	{
		str[0] = '\0';  /* Empty string case */
	}
}

/**
* print_env - Prints the environment variables
* @info: Pointer to the shell information structure
*/
void print_env(shell_info_t *info)
{
	int i = 0;

	(void)info;  /* Évite l'avertissement si info n'est pas utilisé */

	/* Parcours de la liste d'environnement et affichage */
	while (environ[i] != NULL)
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * find_command_in_path - Finds the full path of a command
 * @cmd: Command to find
 *
 * Return: Full path to the command if found, NULL otherwise
 */
char *find_command_in_path(char *cmd)
{
    char *path = NULL;
    char *path_copy = NULL;
    char *dir = NULL;
    char *cmd_path = NULL;
    int i = 0;

    /* Locate the PATH variable in the environment */
    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path = environ[i] + 5;
            break;
        }
        i++;
    }

    if (!path)
        return (NULL);

    /* Duplicate the PATH to tokenize it */
    path_copy = _strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        cmd_path = malloc(_strlen(dir) + _strlen(cmd) + 2);
        if (!cmd_path)
        {
            free(path_copy);
            return (NULL);
        }

        /* Construct the full path: dir + "/" + cmd */
        _strcpy(cmd_path, dir);
        _strcat(cmd_path, "/");
        _strcat(cmd_path, cmd);

        if (access(cmd_path, X_OK) == 0) /* Check if command is executable */
        {
            free(path_copy);
            return (cmd_path);
        }

        free(cmd_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL); /* Command not found in PATH */
}
