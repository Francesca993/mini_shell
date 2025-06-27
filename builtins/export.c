/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:03:02 by francesca         #+#    #+#             */
/*   Updated: 2025/06/27 16:00:14 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
Testa questi casi:
export
export VAR1=ciao
export VAR1=nuovo
export VAR2
export 1INVALID=fail
export VAR3=ok VAR4 VAR5=123
*/

/*
export VAR=VAL	Aggiunge alla fine se non esiste
export VAR=NUOVO	Sovrascrive valore, mantiene posizione
export	Mostra tutte, nell'ordine corrente
env         # stampa tutte le variabili "vive" (quelle con =valore)
export      # stampa tutte, anche quelle senza valore
✅ In sintesi
Sintassi VAR=VAL	OK
Aggiunta nuovi	OK
Sovrascrittura esistenti	OK
Nome valido solo	OK
Comando senza argomenti (export)
*/

/*
** Restituisce l'indice della variabile in my_env se esiste (VAR= o VAR),
** altrimenti -1
*/
static int	find_env_index(char **my_env, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (my_env[i])
	{
		if (ft_strncmp(my_env[i], key, len) == 0 &&
			(my_env[i][len] == '=' || my_env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

/*
** Verifica se una stringa è un identificatore valido per export/unset.
** Regole (come in Bash):
** - Deve iniziare con una lettera (a-z, A-Z) o underscore '_'
** - Può contenere lettere, numeri e underscore
** - Il controllo si ferma prima del primo '=' (se presente)
** Esempi validi:   VAR, _HOME, VAR123
** Esempi invalidi: 1ABC, VAR-TEST, =INVALID
** Ritorna:
** - 1 se valido
** - 0 se invalido
** ============================================
*/
int	is_valid_identifier(const char *str)
{
	int	i;

	i = 1;
	// Primo carattere: deve essere una lettera o '_'
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	// Scorri il resto finché non trovi '=' o la fine
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0); // carattere non valido
		i++;
	}
	return (1); // tutto valido
}

/*
** Aggiunge una nuova variabile a my_env (in fondo)
*/
static char	**add_new_var(char **my_env, const char *entry)
{
	int		len;
	char	**new_env;
	int		i;

	len = 0;
	i = 0;
	while (my_env && my_env[len])
		len++;
	new_env = ft_calloc(len + 2, sizeof(char *)); // +1 per nuova, +1 per NULL
	if (!new_env)
		return (my_env); // fallback se malloc fallisce
	while (i < len)
	{
		new_env[i] = my_env[i]; // riciclo i vecchi puntatori
		i++;
	}
	new_env[len] = ft_strdup(entry);
	new_env[len + 1] = NULL;
	free(my_env); // libera solo il contenitore, non le stringhe
	return (new_env);
}

/*
** Trova la chiave (prima di '=' se presente)
*/
char	*find_key(char *str)
{
	char	*equal;
	char	*key;

	equal = ft_strchr(str, '=');
	// ricava il nome della variabile (prima di '=' se presente)
	if (equal)
		key = ft_substr(str, 0, equal - str);
	else
		key = ft_strdup(str);
	return (key);
}

int	export_variable(char ***my_envp, char **args)
{
	int		i;
	char	*key;
	char	*equal;
	int		index;
	char	**my_env;
	char	*tmp;

	i = 1;
	my_env = *my_envp;
	g_exit_status = 0;
	if (!args[1])
	{
		print_export_buildin_sorted(my_env);
		return (1);
	}
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal)
			key = ft_substr(args[i], 0, equal - args[i]);
		else
			key = ft_strdup(args[i]);
		if (!is_valid_identifier(key))
		{
			fprintf(stderr, "export: `%s`: not a valid identifier\n",
					args[i]);
			g_exit_status = 1;
			free(key);
			i++;
			continue ;
		}
		index = find_env_index(my_env, key);
		if (index >= 0 && equal)
		{
			free(my_env[index]);
			my_env[index] = ft_strdup(args[i]);
		}
		else if (index == -1)
		{
			if (equal)
				my_env = add_new_var(my_env, args[i]);
			else
			{
				tmp = ft_strjoin(args[i], "=");
				my_env = add_new_var(my_env, tmp);
				free(tmp);
			}
		}
		free(key);
		i++;
	}
	*my_envp = my_env; // aggiorna il chiamante
	return (1);
}
