/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:59:18 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/24 19:20:49 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_err_msg(int err)
{
	if (err == ARGS_ERR)
		return (": too many arguments\n");
	else if (err == NAN_ERR)
		return (": numeric argument required\n");
	else if (err == IDENTIFIER_ERR)
		return (": not a valid identifier\n");
	else if (err == CMD_NOT_FOUND)
		return (": command not found\n");
	else if (err == ERRNO_ERR)
		return (strerror(errno));
	else
		return (": unknow error\n");
}

void	print_custom_err(char *cmd, int err)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(MINISHELL ": ", cmd);
	if (!temp)
		return (perror(MINISHELL));
	temp2 = ft_strjoin(temp, get_err_msg(err));
	free(temp);
	if (!temp2)
		return (perror(MINISHELL));
	write(STDERR_FILENO, temp2, ft_strlen(temp2));
	free(temp2);
}

void	print_arg_err(char *cmd, char *str, int err)
{
	int		len1;
	int		len2;
	int		total_len;
	char	*err_str;
	char	*new_str;

	len1 = ft_strlen(cmd);
	len2 = ft_strlen(str);
	err_str = get_err_msg(err);
	total_len = len1 + len2 + ft_strlen(err_str);
	new_str = malloc(total_len + ft_strlen(MINISHELL) + 7);
	if (!new_str)
		return (perror(MINISHELL));
	total_len = ft_strlcpy(new_str, MINISHELL, ft_strlen(MINISHELL) + 1);
	total_len += ft_strlcpy(&new_str[total_len], ": ", 3);
	total_len += ft_strlcpy(&new_str[total_len], cmd, len1 + 1);
	total_len += ft_strlcpy(&new_str[total_len], ": '", 4);
	total_len += ft_strlcpy(&new_str[total_len], str, len2 + 1);
	total_len += ft_strlcpy(&new_str[total_len], "'", 2);
	total_len += ft_strlcpy(&new_str[total_len],
			err_str, ft_strlen(err_str) + 1);
	new_str[total_len] = '\0';
	write(STDERR_FILENO, new_str, ft_strlen(new_str));
	free(new_str);
}

void	write_err(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

void	print_err(int err)
{
	if (err == QUOTES_ERR)
		write_err(MINISHELL ": " QUOTES_MSG);
	else if (err == SYNTAX_ERR)
		write_err(MINISHELL ": " SYNTAX_MSG);
	else if (err == PIPE_END_ERR)
		write_err(MINISHELL ": " PIPE_END_MSG);
	else if (err == ARGS_ERR)
		write_err(MINISHELL ": " ARGS_MSG);
	else if (err == PARENTHESIS_ERR)
		write_err(MINISHELL ": " PARENTHESIS_MSG);
	else if (err != ERRNO_PRINTED)
		perror(MINISHELL);
}
