#include "simple_shell.h"

/**
* _strlen - return the length
* description: return the length of a string
* @s: string to check
* Return: the length
*/

int _strlen(char *s)
{
	int length = 0;

	while (*s != '\0')
	{
		length++;
		s++;
	}
	return (length);
}

/**
 * *_strdup - return a pointer
 * Description: return pointer, which contain a cp of the str given as a param
 * @str: string
 * Return: NULL or pointer to the duplicate string
*/

char *_strdup(char *str)
{
	char *str_bis;
	int length = 0;
	int i = 0;

	if (str == NULL)
	{
		return (NULL);
	}
	while (str[length] != '\0')
	{
		length++;
	}

	str_bis = malloc((length + 1) * sizeof(char));

	if (str_bis == NULL)
	{
		return (NULL);
	}
	while (str[i] != '\0')
	{
		str_bis[i] = str[i];
		i++;
	}
	str_bis[i] = '\0';
	return (str_bis);
}

/**
* _strcmp - compare
* description: compare two strings
* @s1: string 1
* @s2: string 2
* Return: value
*/

int _strcmp(char *s1, char *s2)
{
	int i = 0;

	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}

	return (s1[i] - s2[i]);
}
