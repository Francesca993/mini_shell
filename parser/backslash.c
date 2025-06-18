/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:50:39 by skayed            #+#    #+#             */
/*   Updated: 2025/06/18 17:42:50 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char *handle_backslash(char *str, int *dollar)
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
			if (str[i + 1] == '$')
			{
				// $ è escape-ato → non espandere
				if (dollar)
					*dollar = 0;
				i++;
				res[j++] = str[i++]; // copia $
			}
			else if (str[i + 1] == '"' || str[i + 1] == '\\' || str[i + 1] == '\n')
			{
				i++;
				if (str[i])
					res[j++] = str[i++];
			}
			else if (str[i + 1])
			{
				res[j++] = str[i++];
			}
			else
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


char *handle_backslash_outside_quotes(const char *str)
{
    size_t len = ft_strlen(str);
    char *res = ft_calloc((len + 1), sizeof(char)); // risultato al massimo lungo quanto l'originale
    if (!res)
        return NULL;

    size_t i = 0, j = 0;
    while (str[i])
    {
        if (str[i] == '\\')
        {
            size_t count = 0;

            // Conta backslash consecutive
            while (str[i] == '\\')
            {
                count++;
                i++;
            }

            if (count > 1)
                res[j++] = '\\'; // più di una → ne mantieni una
            // altrimenti (count == 1) → salta, non copia nulla
        }
        else
        {
            res[j++] = str[i++];
        }
    }

    res[j] = '\0';
    return res;
}

