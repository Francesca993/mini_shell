/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:02:00 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 11:21:44 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

char	**copy_env(char **envp);
void	free_myenvp(char **my_env);
int		export_myenvp(char **my_env);

#endif
