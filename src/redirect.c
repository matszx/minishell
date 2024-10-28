/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:49:32 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/28 00:24:31 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_pipes(t_shell *shell)
{
	int	i;

	shell->fd = malloc(sizeof(int) * shell->n_commands * 2);
	if (!shell->fd)
		return (ERRNO_ERR);
	i = -1;
	while (++i < shell->n_commands * 2)
		shell->fd[i] = -1;
	shell->fd[0] = STDIN_FILENO;
	i = 2;
	while (i < shell->n_commands * 2)
	{
		if (pipe(&shell->fd[i]) == -1)
			return (close_files(shell->fd, i), free(shell->fd),
				shell->fd = NULL, ERRNO_ERR);
		i += 2;
	}
	i = 0;
	while (++i < shell->n_commands)
		shell->fd[((i - 1) * 2) + 1] = shell->fd[(i * 2) + 1];
	shell->fd[((i - 1) * 2) + 1] = STDOUT_FILENO;
	return (0);
}

int	input(char *str, int *fd, int cmd)
{
	int	new_fd;

	close(fd[cmd * 2]);
	new_fd = open(str, O_RDONLY);
	fd[cmd * 2] = new_fd;
	if (new_fd == -1)
		return (ERRNO_ERR);
	return (0);
}

int	output(char *str, int *fd, int cmd)
{
	int	new_fd;

	close(fd[(cmd * 2) + 1]);
	new_fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, FILE_CREAT);
	fd[(cmd * 2) + 1] = new_fd;
	if (new_fd == -1)
		return (ERRNO_ERR);
	return (0);
}

int	append_output(char *str, int *fd, int cmd)
{
	int	new_fd;

	close(fd[(cmd * 2) + 1]);
	new_fd = open(str, O_WRONLY | O_CREAT | O_APPEND, FILE_CREAT);
	fd[(cmd * 2) + 1] = new_fd;
	if (new_fd == -1)
		return (ERRNO_ERR);
	return (0);
}

int	redirect(t_token *token, int *fd, int cmd)
{
	if (token->type == RED_IN)
		return (input(token->next->str, fd, cmd));
	else if (token->type == RED_OUT)
		return (output(token->next->str, fd, cmd));
	else if (token->type == RED_APP)
		return (append_output(token->next->str, fd, cmd));
	return (0);
}
