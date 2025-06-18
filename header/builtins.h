/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:22:20 by francesca         #+#    #+#             */
/*   Updated: 2025/06/18 14:10:15 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int     execute_builtin(t_cmd *cmd, char ***envp, t_pipeline *pipeline);
int     ft_exit(char **args);
void	init_signals(void);
int     ft_pwd(void);
int     ft_cd(char **args, char **my_env);
void    update_env_value(char **my_env, char *new_path);
void    unset_variable(char **my_env, char **args);
void    export_variable(char ***my_env, char **args);
void    print_export_buildin_sorted(char **my_env);
void    ft_echo(char **args);

#endif
