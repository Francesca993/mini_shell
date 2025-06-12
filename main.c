/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:58:21 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 10:45:40 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header/minishell.h"

/*
** Variabile globale usata per memorizzare lo stato di uscita della shell.
** - Aggiornata dopo l'esecuzione di ogni comando (per supportare $?)
** - Modificata anche dai signal handler per riflettere l'interruzione da segnali (es. Ctrl-C → 130)
**
** Conforme al subject:
** - È l'unica variabile globale usata per i segnali
** - Non accede né fornisce accesso ad altre strutture dati
** - Di tipo sig_atomic_t per garantire scrittura sicura nei signal handler
** - È lo stesso valore che l'utente potrà recuperare con $? in uno script o dopo un comando.
*/
//Dichiarazione globale (in un .c file, NON in un header .h per rispetto alla Norm)
volatile sig_atomic_t g_exit_status = 0;

void minishell_loop(char **env)
{
    char    *line;
    t_pipeline   *pipeline = NULL;
    int processing = 1;

    while (processing == 1)
    
    {
        line = readline("minishell$: ");
        // Ctrl-D premuto
        if (!line)
        {
            exit_shell(0, NULL);
            write(1, "exit\n", 5);
            break;
        }
        if (*line == '\0')
        {
            free(line);
            continue; // prompt nuovo senza crash
        }
        if (*line)
            add_history(line);
        // ⬇️ PARSING
        pipeline = parse_line(line, env, pipeline);
        if (!pipeline)
        {
            // lexer ha già stampato l’errore, salta solo l'esecuzione
            free_pipeline(pipeline);
        }
        if (pipeline && pipeline->cmds[0] != NULL)
        {
            // ⬇️ ESECUZIONE
            processing = process_pipeline(pipeline);
        }
        if (pipeline)
            free_pipeline(pipeline);
        free(line);
    }
}

int main(int argc, char **argv, char**envp)
{
    //so che esistono ma al momento non mi servono
    (void)argc;
    (void)argv;

    char **my_env;
    
    my_env = copy_env(envp);
    /*
    int i = 0;
    stampa env copiato
    while (my_env[i])
    {
        printf("%s\n", my_env[i]);
        i++;
    }
    */
    minishell_loop(my_env);
    free_myenvp(my_env);
    //rl_clear_history(); // libera la history ma lo fa gia exitshell per linux
    clear_history();
    //return (0);
    return (g_exit_status);
}