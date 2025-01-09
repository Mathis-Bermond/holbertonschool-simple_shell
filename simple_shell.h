#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#define MAX_INPUT 1024

extern char **environ;

/* Déclaration des fonctions utilitaires */
int _strlen(char *s);

char *_strdup(char *str);

int _strcmp(char *s1, char *s2);

char *_strcat(char *dest, char *src);

char *_strcpy(char *dest, char *src);

char *_strchr(char *s, char c);

void trim_whitespace(char *str);

char *find_command_in_path(char *cmd);

/**
* struct shell_info - Structure contenant les informations du shell
* @input: L'entrée de l'utilisateur
* @args: Les arguments séparés par des espaces
* @cmd_path: Le chemin de la commande à exécuter
* @status: Le statut de la commande
*/
typedef struct shell_info
{
	char *input;

	char **args;

	char *cmd_path;

	int status;

} shell_info_t;

/* Déclaration de la fonction pour afficher l'environnement */
void print_env(shell_info_t *info);

/* Autres fonctions */
void prompt(shell_info_t *info);
void process_input(shell_info_t *info);
void execute_command(shell_info_t *info);
void free_shell_info(shell_info_t *info);
void get_pid(void);
void ppid(void);

/* Variable globale représentant l'environnement */
extern char **environ;

#endif /* SIMPLE_SHELL_H */
