/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:13:09 by skayed            #+#    #+#             */
/*   Updated: 2025/06/24 12:14:15 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	set_redirections(t_cmd *cmd)
{
	if (cmd->redir_in && cmd->fd_in != -1)
		dup2(cmd->fd_in, STDIN_FILENO);
	if (cmd->redir_out && cmd->fd_out != -1)
		dup2(cmd->fd_out, STDOUT_FILENO);
	if (cmd->heredoc && cmd->fd_in != -1)
		dup2(cmd->fd_in, STDIN_FILENO);
	if (cmd->append && cmd->fd_out != -1)
		dup2(cmd->fd_out, STDOUT_FILENO);
}

static void	ft_free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
}

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (perror("PATH not found"), NULL);
}

char	*check_path(char *env_path, char *cmd)
{
	char	**matrix;
	char	*full_path;
	char	*tmp;
	int		i;

	matrix = ft_split(env_path, ':');
	if (!matrix)
		return (perror("Split failed"), NULL);
	i = 0;
	while (matrix[i])
	{
		tmp = ft_strjoin(matrix[i], "/");
		if (!tmp)
			return (ft_free_matrix(matrix), NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (ft_free_matrix(matrix), NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (ft_free_matrix(matrix), full_path);
		i++;
		free(full_path);
	}
	ft_free_matrix(matrix);
	return (perror("Command not found"), NULL);
}

