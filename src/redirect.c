/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:49:32 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/14 01:52:04 by dzapata          ###   ########.fr       */
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

int	heredoc(char *str, int *fd, int cmd)
{
	int		i;
	char	*input;
	int		new_fd[2];

	if (pipe(new_fd) == -1)
		return (ERRNO_ERR);
	i = skip_spaces(str);
	while (str[i] && str[i] != ' ' && str[i] != '|')
		i++;
	while (1)
	{
		input = readline(">");
		if (!input)
			return (close_files(new_fd, 2), ERRNO_ERR);
		if (write(new_fd[1], input, ft_strlen(input)) == -1
		||	write(new_fd[1], "\n", 1) == -1)
			return (free(input), close_files(new_fd, 2), ERRNO_ERR);
		free(input);
		if (ft_strncmp(str, input, i))
			break ;
	}
	close(new_fd[1]);
	close(fd[cmd]);
	fd[cmd] = new_fd[0];
	return (0);
}

int	input(char *str, int *fd, int cmd)
{
	int	fd;
	int	i;

	i = skip_spaces(str);
	while (str[i] && str[i] != ' ' && str[i] != '|')
		i++;
	
}

int	nth_pipe(t_token *token, int *fd, int cmd)
{
	int		i;
	int		err;
	char	quotes;

	i = -1;
	err = 0;
	quotes = '\0';
	while (token->str[++i])
	{
		if (token->str[i] == SQUOTE || token->str[i] == DQUOTE && !quotes)
			quotes = token->str[i];
		else if (token->str[i] == quotes)
			quotes = '\0';
		else if (token->str[i] == '<' && !quotes)
		{
			if (token->str[++i] == '<')
				err = heredoc(&token->str[++i], fd, cmd);
			else
				err = input(&token->str[i], fd, cmd);
		}
		else if (token->str[i] == '>' && !quotes)
		{
			if (token->str[++i] == '>')
				err = append_output(&token->str[++i], fd, cmd);
			else
				err = output(&token->str[i], fd, cmd);
		}
		if (err)
			return (err);
	}
	return (0);
}

int	redirect(t_shell *shell)
{
	t_token		*temp;
	int			err;
	int			cmd;

	err = get_pipes(shell);
	if (err)
		return (err);
	temp = shell->tokens;
	cmd = 0;
	while (temp)
	{
		err = nth_pipe(temp, shell->fd, cmd);
		if (err)
			return (err);
		temp = temp->next;
		cmd++;
	}
	return (0);
}
