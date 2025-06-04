/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:58:21 by francesca         #+#    #+#             */
/*   Updated: 2025/06/04 16:09:39 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header/minishell.h"

int g_exit_status = 0;

void minishell_loop(char **env)
{
    char    *line;
    t_pipeline   *pipeline = NULL;

    while (1)
    {
        line = readline("minishell$: ");
        if (!line)
        {
            write(1, "exit\n", 5);
            break;
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
       /*
        if (cmd)
        {
            // ⬇️ ESECUZIONE
            execute_cmd(cmd, env);
            free_cmd(cmd);
        }
        */
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
    /*
    free_envp(my_env);
    rl_clear_history(); // libera la history
    */
    return (0);
}