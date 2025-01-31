#include "simple_shell.h"

/**
* *_strchr - locate character
* description: locate a character in a string
* @s: represent a given string.
* @c: refers to specific character to be searched in a given string.
* Return: pointer to the first occurrence or NULL if the character is not found
*/

char *_strchr(char *s, char c)
{
	while (*s != '\0')
	{
		if (*s == c)
		{
			return (s);
		}
		s++;
	}

	if (c == '\0')
	{
		return (s);
	}

	return (NULL);
}

/**
* cut_whitespace - Removes leading and trailing spaces from a string
* @str: The string to be trimmed
*/
void cut_whitespace(char *str)
{
	int start = 0;

	int i;

	int end = _strlen(str) - 1;

	/* Avance start tant que c'est un espace et s'arrête a la fin de la chaine*/
	while (str[start] == ' ' && str[start] != '\0')
		start++;

	/* recule end tant qu'il y a un espace, s'arrête quand un non espace est trouvé*/
	while (end > start && str[end] == ' ')
		end--;

	/* Déplace les caractères restant vers la gauche */
	if (start > 0)
	{
		for (i = 0; str[start + i] != '\0'; i++)

		{
			str[i] = str[start + i];
		}
		str[end - start + 1] = '\0';  /* on marque la fin de la chaine nettoyée*/
	}

	/* on gère le cas où la chaîne ne contient que des espaces */
	if (end < 0)
	{
		str[0] = '\0'; 
	}
}

/**
* print_env - Prints the environment variables
* @info: Pointer to the shell information structure
*/
void print_env(void)
{
	int i = 0;

	if (!environ)
		return;
	
	/* Parcours de la liste d'environnement et affichage */
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);  /* Affiche chaque variable d'environnement */
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
	char *path = NULL; /*stock le contenu de path*/
	char *path_copy = NULL; /*duplique le contenu de path*/
	char *dir = NULL; /*stocke chaque repertoire trouvé dans path*/
	char *cmd_path = NULL; /*stocke le chemin complet de la commande*/
	int i = 0;

	/* Localiser la variable PATH dans l'environnement sans utiliser strncmp */
	while (environ[i])
	{
		if (environ[i][0] == 'P' && environ[i][1] == 'A' && environ[i][2] == 'T' &&
			environ[i][3] == 'H' && environ[i][4] == '=')
		{
			path = environ[i] + 5;  /* Ignorer "PATH=" */
			break;
		}
		i++;
	}

	if (!path)
		return (NULL);

	/* Dupliquer la variable PATH pour pouvoir la tokeniser */
	path_copy = _strdup(path);
	if (!path_copy)
		return (NULL);

	/* Tokeniser la variable PATH en utilisant ":" comme séparateur */
	dir = strtok(path_copy, ":");
	while (dir)
	{
		/* Allouer de la mémoire pour le chemin complet de la commande */
		cmd_path = malloc(_strlen(dir) + _strlen(cmd) + 2);
		if (!cmd_path)
		{
			free(path_copy);
			return (NULL);
		}

		/* Construire le chemin complet : répertoire + "/" + commande */
		_strcpy(cmd_path, dir);
		_strcat(cmd_path, "/");
		_strcat(cmd_path, cmd);

		/* Vérifier si la commande existe et est exécutable */
		if (access(cmd_path, X_OK) == 0)
		{
			free(path_copy);
			return (cmd_path);
		}

		free(cmd_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}



