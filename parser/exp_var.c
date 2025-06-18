/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:33:23 by skayed            #+#    #+#             */
/*   Updated: 2025/06/18 09:58:00 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// Funzione per ottenere il nome della variabile da una stringa
static char *get_var_name(const char *str, int *len)
{
    int i;
    char *var_name;

    if (!str || str[0] != '$')
        return (NULL);
    
    // Caso speciale: $?
    if (str[1] == '?')
    {
        *len = 2;
        return (ft_strdup("?"));
    }

    // Caso speciale: $0
    if (str[1] == '0')
    {
        *len = 2;
        return (ft_strdup("0"));
    }

    // Caso normale: $VAR
    i = 1;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    
    if (i == 1) // Solo $ senza nome variabile
    {
        *len = 1;
        return (ft_strdup("$"));
    }

    *len = i;
    var_name = ft_substr(str, 1, i - 1);
    return (var_name);
}

// Funzione per ottenere il valore di una variabile dall'ambiente
static char *get_env_value(const char *var_name, char **env)
{
    int i;
    int len;
    char *value;

    if (!var_name || !env)
        return (NULL);

    // Gestione casi speciali
    if (ft_strcmp(var_name, "?") == 0)
        return (ft_itoa(g_exit_status));
    if (ft_strcmp(var_name, "0") == 0)
        return (ft_strdup("minishell"));
    if (ft_strcmp(var_name, "$") == 0)
        return (ft_strdup("$"));

    len = ft_strlen(var_name);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
        {
            value = ft_strdup(env[i] + len + 1);
            return (value);
        }
        i++;
    }
    return (ft_strdup("")); // Variabile non trovata
}

// Funzione per espandere una singola variabile in una stringa
static char *expand_single_variable(const char *str, char **env)
{
    char *var_name;
    char *var_value;
    char *result;
    int var_len;
    int total_len;

    var_name = get_var_name(str, &var_len);
    if (!var_name)
        return (ft_strdup(str));

    var_value = get_env_value(var_name, env);
    free(var_name);

    total_len = ft_strlen(str) - var_len + ft_strlen(var_value);
    result = ft_calloc(total_len + 1, sizeof(char));
    if (!result)
    {
        free(var_value);
        return (NULL);
    }

    ft_strlcat(result, var_value, total_len + 1);
    ft_strlcat(result, str + var_len, total_len + 1);
    free(var_value);
    return (result);
}

// Funzione principale per espandere tutte le variabili in un comando
char *expand_variables(const char *str, char **env)
{
    char *result;
    char *temp;
    int i;

    if (!str || !env)
        return (NULL);

    result = ft_strdup(str);
    if (!result)
        return (NULL);

    i = 0;
    while (result[i])
    {
        if (result[i] == '$' && result[i + 1] && 
            (ft_isalnum(result[i + 1]) || result[i + 1] == '?' || 
             result[i + 1] == '_' || result[i + 1] == '$'))
        {
            temp = expand_single_variable(result + i, env);
            if (!temp)
            {
                free(result);
                return (NULL);
            }
            free(result);
            result = temp;
            i = 0; // Ricomincia da capo per gestire variabili multiple
        }
        else
            i++;
    }
    return (result);
}

// Funzione per espandere le variabili in tutti gli argomenti di un comando
void expand_cmd_variables(t_cmd *cmd, char **env)
{
    int i;
    char *expanded;

    if (!cmd || !env)
        return;

    i = 0;
    while (cmd->args && cmd->args[i])
    {
        if (ft_strchr(cmd->args[i], '$') && !cmd->quote_single)
        {
            expanded = expand_variables(cmd->args[i], env);
            if (expanded)
            {
                free(cmd->args[i]);
                cmd->args[i] = expanded;
            }
        }
        i++;
    }
}

// Funzione per espandere le variabili in tutta la pipeline
void expand_pipeline_variables(t_pipeline *pipeline)
{
    int i;

    if (!pipeline)
        return;

    i = 0;
    while (i < pipeline->n_cmds)
    {
        if (pipeline->cmds[i]->dollar)
            expand_cmd_variables(pipeline->cmds[i], pipeline->my_env);
        i++;
    }
}

void check_var(t_pipeline *pipeline)
{
	int i;
	int j;
	char *expanded;
	i = 0;
    while (i < pipeline->n_cmds)
    {
        if (pipeline->cmds[i]->dollar)
        {
            j = 0;
            while (pipeline->cmds[i]->args[j] != NULL)
            {
                if (ft_strchr(pipeline->cmds[i]->args[j], '$') && 
                    !pipeline->cmds[i]->quote_single)
                {
                    expanded = expand_variables(pipeline->cmds[i]->args[j], 
                                                    pipeline->my_env);
                    free(pipeline->cmds[i]->args[j]);
                    pipeline->cmds[i]->args[j] = expanded;
                }
                j++;
            }
        }
        i++;
    }
}
