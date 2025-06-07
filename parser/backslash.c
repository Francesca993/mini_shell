/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:50:39 by skayed            #+#    #+#             */
/*   Updated: 2025/06/07 21:11:01 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char *handle_backslash(char *str)
{
	int len;
	char *res;
	int i;
	int j;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			if (str[i + 1] == '$' || str[i+1] == '"' || str[i+1] == '\\' || str[i+1] == '\n')
			{
				i++;
				if (str[i])
					res[j++] = str[i++]; // salta e copia carattere successivo
			}
			else if (str[i + 1])// backslash non speciale
			{
				res[j++] = str[i++];
			}
			else // backslash finale
            {
                res[j++] = str[i++];
            }
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}