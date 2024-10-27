/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:33:09 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/26 23:33:57 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	write_heredoc_line(t_shell *shell, char *buf, int fd)
{
	t_token	temp;
	int		err;

	temp.type = ARGUMENT;
	temp.str = buf;
	temp.next = NULL;
	temp.len = ft_strlen(buf);
	err = handle_expansions(&temp, shell->env_var, shell->exit_status, 1);
	if (err)
		return (free(buf), err);
	if (write(fd, temp.str, ft_strlen(temp.str)) == -1
		|| write(fd, "\n", 1) == -1)
		return (free(temp.str), ERRNO_ERR);
	return (free(temp.str), 0);
}

int	heredoc(t_shell *shell, t_token *t, int *fd, int cmd)
{
	char	*input;
	int		new_fd[2];

	if (pipe(new_fd) == -1)
		return (ERRNO_ERR);
	while (1)
	{
		input = readline("> ");
		if (!input)
			return (close_files(new_fd, 2), ERRNO_ERR);
		if (!ft_strncmp(t->str, input, t->len))
			break ;
		if (write_heredoc_line(shell, input, new_fd[1]))
			return (close_files(new_fd, 2), ERRNO_ERR);
	}
	close(new_fd[1]);
	close(fd[cmd * 2]);
	fd[cmd * 2] = new_fd[0];
	return (free(input), 0);
}

int	red_heredoc(t_shell *shell)
{
	t_token		*temp;
	int			err;
	int			cmd;

	temp = shell->tokens;
	cmd = 0;
	while (temp)
	{
		if (temp->type == HEREDOC)
		{
			err = heredoc(shell, temp->next, shell->fd, cmd);
			if (err)
				return (err);
		}
		temp = temp->next;
		if (temp && temp->type == OPERATOR)
			cmd++;
	}
	return (0);
}

int	sort_io(int *fd, int n)
{
	int	i;

	i = 0;
	while (++i < n)
		fd[((i - 1) * 2) + 1] = fd[(i * 2) + 1];
	fd[((i - 1) * 2) + 1] = STDOUT_FILENO;
	return (0);
}
