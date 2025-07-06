/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:00:39 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 23:02:26 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Verifica se un carattere è uno spazio bianco valido.
 * Sono considerati spazi:
 *   - spazio ' '
 *   - tabulazione '\t'
 *   - newline '\n'
 *   - vertical tab '\v'
 *   - form feed '\f'
 *   - carriage return '\r'
 *
 * Ritorna:
 * - 1 se il carattere è uno spazio
 * - 0 altrimenti
 */
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/*
 * Verifica se un carattere è un metacarattere della shell.
 * Sono considerati metacaratteri:
 *   - '|': pipe
 *   - '<': redirezione input
 *   - '>': redirezione output
 *
 * Ritorna:
 * - 1 se il carattere è un metacarattere
 * - 0 altrimenti
 */
int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
