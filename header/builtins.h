/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:22:20 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 12:50:22 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int     execute_builtin(t_cmd *cmd, char ***envp);
int     ft_exit(char **args);
void	init_signals(void);
int     ft_pwd(void);

#endif
