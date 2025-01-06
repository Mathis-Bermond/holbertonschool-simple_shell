#include "simple_shell.h"

#define MAX_INPUT 1024


/**
* execute_command - Forks a child process to execute a command.
* @info: The shell info structure containing input and arguments.
*/
void execute_command(shell_info_t *info)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Processus enfant : exécuter la commande */
        if (execvp(info->cmd_path, info->args) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Processus parent : attendre la fin du processus enfant */
        do {
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        } while (!((status & 0x7F) == 0 || (status & 0x7F) == 0x7F));
        info->status = status;  /* Mettre à jour le statut de la commande */
    }
}

/**
* parse_input - Split the input string into arguments and find the command path.
* @info: The shell info structure containing input.
*/
void parse_input(shell_info_t *info)
{
    char *token;
    int i = 0;

    /* Allouer de la mémoire pour les arguments */
    info->args = malloc(MAX_INPUT * sizeof(char *));
    if (!info->args)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* Séparer la commande et les arguments */
    token = strtok(info->input, " ");
    while (token != NULL)
    {
        info->args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    info->args[i] = NULL;  /* Terminer le tableau des arguments par NULL */

    /* Définir le chemin de la commande (ici, on suppose qu'elle est dans le PATH) */
    info->cmd_path = info->args[0];
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

    while (1)
    {
        /* Afficher le prompt */
        write(STDOUT_FILENO, "prompt> ", 9);

        /* Lire l'entrée utilisateur */
        nread = read(STDIN_FILENO, input, MAX_INPUT);
        if (nread == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        /* Supprimer le caractère de nouvelle ligne si présent */
        if (input[nread - 1] == '\n')
            input[nread - 1] = '\0';

        /* Vérifier si l'entrée est vide */
        if (strlen(input) == 0)
            continue;

        /* Comparer pour "exit" et quitter si nécessaire */
        if (strcmp(input, "exit") == 0)
            break;

        /* Initialiser la structure */
        info.input = strdup(input);  /* Allouer une copie de l'entrée */
        if (!info.input)
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }

        /* Analyser l'entrée pour obtenir les arguments et le chemin de la commande */
        parse_input(&info);

        /* Exécuter la commande entrée par l'utilisateur */
        execute_command(&info);

        /* Libérer la mémoire allouée pour l'entrée */
        free(info.input);
        free(info.args);
    }

    return 0;
}
