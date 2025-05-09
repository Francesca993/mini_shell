/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:58:21 by francesca         #+#    #+#             */
/*   Updated: 2025/05/09 14:03:56 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header/minishell.h"

void minishell_loop(char **env)
{
    char    *line;
    t_cmd   *cmd;

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
        cmd = parse_line(line, env);
       /*
        if (cmd)
        {
            // ⬇️ ESECUZIONE
            execute_cmd(cmd, env);
            free_cmd(cmd);
        }
        */
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
    int i = 0;
    while (my_env[i])
    {
        printf("%s\n", my_env[i]);
        i++;
    }
    minishell_loop(my_env);
    /*
    free_envp(my_env);
    rl_clear_history(); // libera la history
    */
    return (0);
}