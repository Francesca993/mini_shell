/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:50:39 by skayed            #+#    #+#             */
/*   Updated: 2025/07/08 10:44:07 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// static int	handle_common_escapes(const char *str, int *i, int *j,
//		char *res)
// {
// 	if (str[*i + 1] == '"' || str[*i + 1] == '\\' || str[*i + 1] == '\n')
// 	{
// 		(*i)++; // salta il backslash
// 		if (str[*i])
// 			res[(*j)++] = str[(*i)++];
// 		return (1);
// 	}
// 	return (0);
// }
// static void	handle_other_backslashes(const char *str, int *i, int *j,
//		char *res)
// {
// 	if (str[*i + 1])
// 		res[(*j)++] = str[(*i)++];
// 	else
// 		res[(*j)++] = str[(*i)++];
// }
// char	*handle_backslash(char *str, int *dollar)
// {
// 	int		len = ft_strlen(str);
// 	char	*res = ft_calloc(len + 1, sizeof(char));
// 	int		i = 0;
// 	int		j = 0;
// 	if (!res)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] == '\\')
// 		{
// 			if (str[i + 1] == '$')
// 			{
// 				if (dollar)
// 					*dollar = 0;
// 				i++; // salta il backslash
// 				res[j++] = str[i++]; // copia il '$'
// 			}
// 			else if (!handle_common_escapes(str, &i, &j, res))
// 				handle_other_backslashes(str, &i, &j, res);
// 		}
// 		else
// 			res[j++] = str[i++];
// 	}
// 	res[j] = '\0';
// 	return (res);
// }
// char	*handle_backslash(char *str, int *dollar)
// {
// 	int		len;
// 	char	*res;
// 	int		i;
// 	int		j;
// 	i = 0;
// 	j = 0;
// 	len = ft_strlen(str);
// 	res = ft_calloc(len + 1, sizeof(char));
// 	if (!res)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] == '\\')
// 		{
// 			if (str[i + 1] == '$')
// 			{
// 				if (dollar)
// 					*dollar = 0;
// 				i++;
// 				res[j++] = str[i++];
// 			}
// 			else if (str[i + 1] == '"' || str[i + 1] == '\\' || str[i
// 					+ 1] == '\n')
// 			{
// 				i++;
// 				if (str[i])
// 					res[j++] = str[i++];
// 			}
// 			else if (str[i + 1])
// 				res[j++] = str[i++];
// 			else
// 				res[j++] = str[i++];
// 		}
// 		else
// 			res[j++] = str[i++];
// 	}
// 	res[j] = '\0';
// 	return (res);
// }
// char	*handle_backslash_outside_quotes(const char *str)
// {
// 	size_t	len;
// 	char	*res;
// 	size_t	i;
// 	size_t	j;
// 	size_t	count;make fclean

// 	i = 0;
// 	j = 0;
// 	len = ft_strlen(str);
// 	res = ft_calloc((len + 1), sizeof(char));
// 	if (!res)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] == '\\')
// 		{
// 			count = 0;
// 			while (str[i] == '\\')
// 			{
// 				count++;
// 				i++;
// 			}
// 			if (count > 1)
// 				res[j++] = '\\';
// 		}
// 		else
// 			res[j++] = str[i++];
// 	}
// 	res[j] = '\0';
// 	return (res);
// }
static size_t	count_consecutive_backslashes(const char *str, size_t *i)
{
	size_t	count;

	count = 0;
	while (str[*i] == '\\')
	{
		count++;
		(*i)++;
	}
	return (count);
}

// conserva un solo backslash
static void	handle_backslashes_outside_quotes(size_t count, char *res,
		size_t *j)
{
	if (count > 1)
		res[(*j)++] = '\\';
}

char	*handle_backslash_outside_quotes(const char *str)
{
	size_t	len;
	char	*res;
	size_t	i;
	size_t	j;
	size_t	count;

	len = ft_strlen(str);
	res = ft_calloc(len + 1, sizeof(char));
	i = 0;
	j = 0;
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			count = count_consecutive_backslashes(str, &i);
			handle_backslashes_outside_quotes(count, res, &j);
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}
