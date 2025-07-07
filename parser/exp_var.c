/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:33:23 by skayed            #+#    #+#             */
/*   Updated: 2025/07/07 09:14:37 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// Funzione per ottenere il nome della variabile da una stringa
char	*get_var_name(const char *str, int *len)
{
	int		i;
	char	*var_name;

	i = 0;
	if (str[0] == '$' && str[1] == '{')
	{
		i = 2;
		while (str[i] && str[i] != '}')
			i++;
		if (str[i] == '}')
		{
			*len = i + 1;
			return (ft_substr(str, 2, i - 2)); // nome tra { e }
		}
		// Parentesi non chiusa, tratta come $ normale
		*len = 1;
		return (ft_strdup(""));
	}
	// Caso $VAR, $?, $0
	if (str[1] == '?' || str[1] == '0')
	{
		*len = 2;
		return (ft_substr(str, 1, 1)); // estrae ? o 0
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
static char	*get_env_value(const char *var_name, char **env)
{
	int		i;
	int		len;
	char	*value;

	if (!var_name || !env)
		return (NULL);
	if (ft_strlen(var_name) == 0)
		return (ft_strdup(""));
	// Gestione casi speciali
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_strdup(ft_itoa(g_exit_status)));
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

// Funzione per ottenere la lunghezza del valore di una variabile dall'ambiente
static int	get_env_value_len(const char *var_name, char **env)
{
	char	*tmp;
	int		len;

	int i, len;
	if (!var_name || !env)
		return (0);
	if (ft_strlen(var_name) == 0)
		return (0);
	if (ft_strcmp(var_name, "?") == 0)
	{
		tmp = ft_itoa(g_exit_status);
		len = ft_strlen(tmp);
		free(tmp);
		return (len);
	}
	if (ft_strcmp(var_name, "0") == 0)
		return (ft_strlen("minishell"));
	if (ft_strcmp(var_name, "$") == 0)
		return (1);
	len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (ft_strlen(env[i] + len + 1));
		i++;
	}
	return (0); // Variabile non trovata
}

// Funzione per calcolare la lunghezza della stringa espansa
static int	compute_expanded_length(const char *str, char **env)
{
	int		len;
	char	*var_name;

	len = 0, i = 0, var_len;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] &&
			(ft_isalnum(str[i + 1]) || str[i + 1] == '?' ||
				str[i + 1] == '_' || str[i + 1] == '$'))
		{
			var_name = get_var_name(str + i, &var_len);
			len += get_env_value_len(var_name, env);
			free(var_name);
			i += var_len;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*expand_variables(const char *str, char **env)
{
	int		i;
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		out_len;
	char	*result;
	int		res_i;
	int		v;

	i = 0;
	out_len = compute_expanded_length(str, env);
	result = ft_calloc(out_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	res_i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] &&
			(ft_isalnum(str[i + 1]) || str[i + 1] == '?' ||
				str[i + 1] == '_' || str[i + 1] == '$' || str[i + 1] == '{'))
		{
			var_name = get_var_name(str + i, &var_len);
			var_value = get_env_value(var_name, env);
			free(var_name);
			if (var_value)
			{
				v = 0;
				while (var_value[v])
					result[res_i++] = var_value[v++];
				free(var_value);
			}
			i += var_len;
		}
		else
		{
			result[res_i++] = str[i++];
		}
	}
	result[res_i] = '\0';
	return (result);
}

// Funzione per espandere le variabili in tutti gli argomenti di un comando
void	expand_cmd_variables(t_cmd *cmd, char **env)
{
	int		i;
	char	*expanded;

	if (!cmd || !env)
		return ;
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
void	expand_pipeline_variables(t_pipeline *pipeline)
{
	int	i;

	if (!pipeline)
		return ;
	i = 0;
	while (i < pipeline->n_cmds)
	{
		if (pipeline->cmds[i]->dollar)
			expand_cmd_variables(pipeline->cmds[i], pipeline->my_env);
		i++;
	}
}

void	check_var(t_pipeline *pipeline)
{
	int		i;
	int		j;
	char	*expanded;

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