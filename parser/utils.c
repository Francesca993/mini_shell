/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:23:22 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 22:30:36 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"

//libera solo i token allocati fino a count.
void free_partial_tokens(char **tokens, t_token_type *types, int count)
{
	if (!tokens || !types)
        return;

    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
    free(types);
}
void exit_shell(int code, const char *msg)
{
	if (msg && *msg)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
	}
	g_exit_status = code;
	rl_clear_history(); // opzionale, ma buona pratica
	exit(code);
}