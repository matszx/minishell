/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:49:32 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/18 19:52:55 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_pipes(t_shell *shell)
{
	int	i;
	int	err;

	shell->fd = malloc(sizeof(int) * shell->n_commands * 2);
	if (!shell->fd)
		return (ERRNO_ERR);
	i = -1;
	while (++i < shell->n_commands * 2)
		shell->fd[i] = -1;
	i = 0;
	while (i < shell->n_commands)
	{
		if (pipe(&shell->fd[i]) == -1)
		{
			close_files(shell->fd, i);
			free(shell->fd);
			shell->fd = NULL;
			return (ERRNO_ERR);
		}
		i += 2;
	}
	err = close(shell->fd[i - 1]);
	shell->fd[i - 1] = STDOUT_FILENO;
	return (err);
}

int	heredoc(t_token *t, int *fd, int cmd)
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
		if (write(new_fd[1], input, ft_strlen(input)) == -1
			|| write(new_fd[1], "\n", 1) == -1)
			return (free(input), close_files(new_fd, 2), ERRNO_ERR);
		if (!ft_strncmp(t->str, input, t->len))
			break ;
		free(input);
	}
	close(new_fd[1]);
	close(fd[cmd * 2]);
	fd[cmd * 2] = new_fd[0];
	return (free(input), 0);
}

int	input(char *str, int *fd, int cmd)
{
	int	new_fd;

	close(fd[cmd * 2]);
	new_fd = open(str, O_RDONLY, FILE_CREAT);
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
			err = heredoc(temp->next, shell->fd, cmd);
			if (err)
				return (err);	
		}
		temp = temp->next;
		if (temp && temp->type == OPERATOR)
			cmd++;
	}
	return (0);
}

int	nth_pipe(t_token *token, int *fd, int cmd)
{
	int		err;

	err = 0;
	if (token->type == RED_IN)
		err = input(token->next->str, fd, cmd);
	else if (token->type == RED_OUT)
		err = output(token->next->str, fd, cmd);
	else if (token->type == RED_APP)
		err = append_output(token->next->str, fd, cmd);
	if (err)
		return (err);
	return (0);
}

int	redirect(t_shell *shell)
{
	t_token		*temp;
	int			err;
	int			cmd;

	temp = shell->tokens;
	cmd = 0;
	while (temp)
	{
		err = nth_pipe(temp, shell->fd, cmd);
		if (err)
			return (err);
		temp = temp->next;
		if (temp && temp->type == OPERATOR)
			cmd++;
	}
	return (0);
}
