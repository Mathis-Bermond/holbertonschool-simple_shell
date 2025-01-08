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
