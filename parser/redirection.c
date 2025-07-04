/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:07 by skayed            #+#    #+#             */
/*   Updated: 2025/07/04 15:34:44 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	setup_redir_in(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
		return (perror("open <"), -1);
	cmd->fd_in = fd;
	cmd->redir_in = 1;
	return (0);
}

int	setup_redir_out(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("open >"), -1);
	cmd->fd_out = fd;
	cmd->redir_out = 1;
	return (0);
}

int	setup_redir_append(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("open >>"), -1);
	cmd->fd_out = fd;
	cmd->append = 1;
	return (0);
}

int	setup_heredoc(t_cmd *cmd, char *delimiter)
{
	int pipe_fd[2];
	char *new_del = NULL;
	new_del = strip_outer_quotes(delimiter);
	//cmd->infile = ft_strdup(new_del);
	
	if (pipe(pipe_fd) < 0)
		return (perror("pipe heredoc"), -1);

	pid_t pid = fork();

	if (pid == 0)
	{
		char *line;
		close(pipe_fd[0]); // chiudi la lettura
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, new_del) == 0)
				break ;
			write(pipe_fd[1], line, ft_strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		free(line);
		close(pipe_fd[1]);
		exit(0);
	}
	free(new_del);
	wait(NULL);
	close(pipe_fd[1]); // chiudi la scrittura nel padre
	cmd->fd_in = pipe_fd[0];
	cmd->heredoc = 1;
	return (0);
}
