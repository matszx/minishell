/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:59:18 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/26 17:43:38 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_err_msg(int err)
{
	if (err == ARGS_ERR)
		return (ARGS_MSG);
	else if (err == NAN_ERR)
		return (NAN_MSG);
	else if (err == IDENTIFIER_ERR)
		return (IDENTIFIER_MSG);
	else if (err == CMD_NOT_FOUND)
		return (NO_CMD_MSG);
	else if (err == HOME_ERR)
		return (HOME_MSG);
	else if (err == ERRNO_ERR)
		return (strerror(errno));
	else
		return (UNKNOW_MSG);
}

void	write_err(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

void	write_strerr(t_err_msg *err)
{
	int	i;

	i = ft_strlcpy(err->str, MINISHELL ": ", ft_strlen(MINISHELL) + 3);
	if (err->cmd)
	{
		i += ft_strlcpy(&err->str[i], err->cmd, err->cmd_len + 1);
		i += ft_strlcpy(&err->str[i], ": ", 3);
	}
	if (err->arg)
	{
		if (err->quotes)
			i += ft_strlcpy(&err->str[i], "'", 2);
		i += ft_strlcpy(&err->str[i], err->arg, err->arg_len + 1);
		if (err->quotes)
			i += ft_strlcpy(&err->str[i], "'", 2);
		i += ft_strlcpy(&err->str[i], ": ", 3);
	}
	i += ft_strlcpy(&err->str[i], err->str_err, ft_strlen(err->str_err) + 1);
	err->str[i++] = '\n';
	err->str[i] = '\0';
}

void	print_arg_err(char *cmd, char *arg, int err, int quotes)
{
	t_err_msg	err_msg;
	size_t		total_len;

	err_msg.cmd = cmd;
	err_msg.arg = arg;
	err_msg.quotes = quotes;
	if (cmd)
		err_msg.cmd_len = ft_strlen(cmd) + 2;
	else
		err_msg.cmd_len = 0;
	if (arg)
		err_msg.arg_len = ft_strlen(arg) + 2;
	else
		err_msg.arg_len = 0;
	err_msg.str_err = get_err_msg(err);
	total_len = err_msg.cmd_len + err_msg.arg_len + ft_strlen(err_msg.str_err);
	if (quotes)
		total_len += 2;
	err_msg.str = malloc(total_len + ft_strlen(MINISHELL) + 4);
	if (!err_msg.str)
		return (perror(MINISHELL));
	write_strerr(&err_msg);
	write_err(err_msg.str);
	free(err_msg.str);
}

void	print_err(int err)
{
	if (err == QUOTES_ERR)
		write_err(MINISHELL ": " QUOTES_MSG "\n");
	else if (err == SYNTAX_ERR)
		write_err(MINISHELL ": " SYNTAX_MSG "\n");
	else if (err == PIPE_END_ERR)
		write_err(MINISHELL ": " PIPE_END_MSG "\n");
	else if (err == ARGS_ERR)
		write_err(MINISHELL ": " NO_ARGS_MSG "\n");
	else if (err == PARENTHESIS_ERR)
		write_err(MINISHELL ": " PARENTHESIS_MSG "\n");
	else if (err != ERRNO_PRINTED)
		perror(MINISHELL);
}
