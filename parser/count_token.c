/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 09:29:24 by francesca         #+#    #+#             */
/*   Updated: 2025/07/08 09:40:54 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	skip_spaces(const char *line, int i)
{
	while (ft_isspace(line[i]))
		i++;
	return (i);
}

int	handle_pipe_token(const char *line, int *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_redirect_token(const char *line, int *i)
{
	char	current;

	if (line[*i] == '<' || line[*i] == '>')
	{
		current = line[*i];
		(*i)++;
		if (line[*i] == current)
			(*i)++;
		return (1);
	}
	return (0);
}

int	handle_word_token(const char *line, int *i)
{
	int	quote;

	quote = 0;
	while (line[*i] && (quote || (!is_metachar(line[*i])
				&& !ft_isspace(line[*i]))))
	{
		if ((line[*i] == '\'' || line[*i] == '"') && !quote)
			quote = line[*i];
		else if (line[*i] == quote)
			quote = 0;
		(*i)++;
	}
	return (1);
}

/*
 * Conta quanti token sono presenti nella linea di input.
 * I token includono:
 *   - parole (comandi, argomenti)
 *   - redirezioni (<, >, <<, >>)
 *   - pipe (|)
 *
 * Gestisce anche le quote:
 *   - quote singole e doppie vengono considerate parte del token
 *   - se una quote non viene chiusa,
	il lexer si interrompe (resta in uno stato errato)
 *
 * Ritorna:
 * - Numero totale di token nella linea
 */
// int	count_token(const char *line)
// {
// 	int	i;
// 	int	count;
// 	int	quote;

// 	count = 0;
// 	quote = 0;
// 	i = 0;
// 	while (line[i])
// 	{
// 		while (ft_isspace(line[i]))
// 			i++;
// 		if (!line[i])
// 			break ;
// 		if (line[i] == '|')
// 		{
// 			count++;
// 			i++;
// 		}
// 		else if (line[i] == '<' || line[i] == '>')
// 		{
// 			i++;
// 			if (line[i] == line[i - 1])
// 				i++;
// 			count++;
// 		}
// 		else
// 		{
// 			count++;
// 			while (line[i] && (quote || (!is_metachar(line[i])
// 						&& !ft_isspace(line[i]))))
// 			{
// 				if ((line[i] == '\'' || line[i] == '"') && !quote)
// 					quote = line[i];
// 				else if (line[i] == quote)
// 					quote = 0;
// 				i++;
// 			}
// 		}
// 	}
// 	return (count);
// }
int	count_token(const char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		i = skip_spaces(line, i);
		if (!line[i])
			break ;
		if (handle_pipe_token(line, &i))
			count++;
		else if (handle_redirect_token(line, &i))
			count++;
		else
		{
			count += handle_word_token(line, &i);
		}
	}
	return (count);
}
