/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:38:15 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 13:43:35 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <termcap.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "envp.h"
# include "parser.h"
# include "execution.h"
# include "builtins.h"

extern volatile sig_atomic_t	g_exit_status;

#endif