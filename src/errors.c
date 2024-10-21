/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:59:18 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/21 16:03:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_errno(char *str)
{
	char	*temp;

	temp = ft_strjoin("Minishell: ", str);
	if (!temp)
		return (perror("Minishell: "));
	perror(temp);
	free(temp);
}

char	*get_err_msg(int err)
{
	if (err == ARGS_ERR)
		return (": Too many arguments\n");
	else if (err == NAN_ERR)
		return (": Numeric argument required\n");
	else if (err == IDENTIFIER_ERR)
		return (": Not a valid identifier\n");
	else if (err == ERRNO_ERR)
		return (strerror(errno));
	else
		return (": Unknow error\n");
}

void	print_custom_err(char *cmd, int err)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin("Minishell: ", cmd);
	if (!temp)
		return (perror("Minishell: "));
	temp2 = ft_strjoin(temp, get_err_msg(err));
	free(temp);
	if (!temp2)
		return (perror("Minishell: "));
	write(STDERR_FILENO, temp2, ft_strlen(temp2));
	free(temp2);
}

void	print_err(int err)
{
	if (err == QUOTES_ERR)
		write(STDERR_FILENO,
			"Minishell: Unclosed quotes\n", 28);
	else if (err == SYNTAX_ERR)
		write(STDERR_FILENO,
			"Minishell: Syntax error near unexpected token\n", 47);
	else if (err == PIPE_END_ERR)
		write(STDERR_FILENO,
			"Minishell: Unclosed pipes are not allowed\n", 43);
	else if (err != ERRNO_PRINTED)
		perror("Minishell: ");
}
