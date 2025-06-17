/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:32:39 by francesca         #+#    #+#             */
/*   Updated: 2025/06/16 21:32:17 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"
#include <stdlib.h> // per malloc, free
#include <string.h> // per strcmp

/*
** Stampa tutte le variabili come `declare -x` (come Bash)
** Ordina alfabeticamente le righe (solo per stampa)
** Quando Bash stampa declare -x Bash non stampa semplicemente VAR=value,
** ma dice esplicitamente: “questa variabile è esportata” → e lo fa con declare -x
*/
static void sort_env(char **env)
{
    int i;
    int j;
    char *tmp;

    i = 0;
    while (env[i] && env[i + 1])
    {
        j = i + 1;
        while (env[j])
        {
            if (ft_strncmp(env[i], env[j], ft_strlen(env[i]) + 1) > 0)
            {
                tmp = env[i];
                env[i] = env[j];
                env[j] = tmp;
            }
            j++;
        }
        i++;
    }
}


static char **sort_env_copy(char **env)
{
    char **copy;

    copy = copy_env(env);
    if (!copy)
        return (NULL);

    sort_env(copy);
    return (copy);
}

void print_export_buildin_sorted(char **my_env)
{
    int i;
    char **sorted;
    char *equal;
    size_t len;

    sorted = sort_env_copy(my_env);
    if (!sorted)
        return;

    i = 0;
    while (sorted[i] != NULL)
    {
        // equal = ft_strchr(sorted[i], '=');
        // if (equal)
        // {
        //     len = equal - sorted[i];
        //     printf("declare -x %.*s=\"%s\"\n", (int)len, sorted[i], equal + 1);
        // }
        // else
        // {
        //     printf("declare -x %s\n", sorted[i]);
        // }
        printf("%s\n", sorted[i]);
        free(sorted[i]);
        i++;
    }
    free(sorted);
}

