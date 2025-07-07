/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:50:39 by skayed            #+#    #+#             */
/*   Updated: 2025/07/07 09:16:25 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char	*handle_backslash(char *str, int *dollar)
{
	int		len;
	char	*res;
	int		i;
	int		j;

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
				if (dollar)
					*dollar = 0;
				i++;
				res[j++] = str[i++];
			}
			else if (str[i + 1] == '"' || str[i + 1] == '\\' || str[i
					+ 1] == '\n')
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

char	*handle_backslash_outside_quotes(const char *str)
{
	size_t	len;
	char	*res;
	size_t	i;
	size_t	j;
	size_t	count;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	res = ft_calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			count = 0;
			while (str[i] == '\\')
			{
				count++;
				i++;
			}
			if (count > 1)
				res[j++] = '\\';
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}
