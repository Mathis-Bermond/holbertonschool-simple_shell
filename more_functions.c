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
