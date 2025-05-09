/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:58:21 by francesca         #+#    #+#             */
/*   Updated: 2025/05/09 10:47:33 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header/minishell.h"

void    minishell_loop(void)
{
    char *line;
    
    while (1)
   {
    line = readline("minishell$: ");
    if (!line)
    {
        printf("exit\n");
        break;
    }
    if (*line)
        add_history(line);
    
    printf("Comando ricevuto: %s\n", line);
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
    minishell_loop();
    /*
    free_envp(my_env);
    rl_clear_history(); // libera la history
    */
    return (0);
}