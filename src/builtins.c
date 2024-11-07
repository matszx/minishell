/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/07 18:29:50 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	n_flag(t_token *token)
{
	int		i;
	int		flag;
	char	*temp;

	i = -1;
	while (token->old_str[++i])
		if (ft_isspace(token->old_str[i]))
			return (0);
	temp = ft_strchr(token->str, '-');
	flag = 0;
	if (temp)
	{
		flag = has_echo_flag(&temp[1]);
		i = 0;
		while (temp[++i])
			if (ft_isspace(temp[i]))
				break ;
		if (temp[i])
		{
			temp = &temp[i + skip_spaces(&temp[i])];
			if (temp[0])
				flag = write_rest(token, temp, flag);
		}
	}
	return (flag);
}

int	ft_echo(t_token *token)
{
	int	newline;
	int	err;

	newline = 0;
	err = 0;
	token = get_cmd_token(token, ARGUMENT);
	if (token)
	{
		newline = n_flag(token);
		if (newline == 1 || newline == 2)
			token = get_cmd_token(token->next, ARGUMENT);
		while (token && newline != 3 && err != -1)
		{
			err = write(STDOUT_FILENO, token->str, token->len);
			token = get_cmd_token(token->next, ARGUMENT);
			if (token && err != -1)
				err = write(STDOUT_FILENO, " ", 1);
		}
	}
	if (newline == 0 && newline != 3 && err != -1)
		err = write(STDOUT_FILENO, "\n", 1);
	if (err == -1 || newline == 3)
		return (print_arg_err("echo", "write error", ERRNO_ERR, 0), 1);
	return (0);
}

int	ft_pwd(char *cwd)
{
	if (!cwd[0])
		return (errno = ENOENT,
			print_arg_err("pwd", NO_PWD_MSG, ERRNO_ERR, 0), 1);
	if (write(STDOUT_FILENO, cwd, ft_strlen(cwd)) == -1
		|| write(STDOUT_FILENO, "\n", 1) == -1)
		return (print_arg_err("pwd", "write error", ERRNO_ERR, 0), 1);
	return (0);
}

int	ft_exit(unsigned int ret, t_shell *shell, t_token *token)
{
	t_token	*arg;
	int		out;
	int		overflow;

	write(STDERR_FILENO, "exit\n", 5);
	if (!token || !get_cmd_token(token, ARGUMENT))
		return (free_shell(shell), exit(ret), 0);
	arg = get_cmd_token(token, ARGUMENT);
	if (!ft_isnumber(arg->str))
		return (print_arg_err("exit", arg->str, NAN_ERR, 1),
			free_shell(shell), exit(2), 0);
	if (arg->next && get_cmd_token(arg->next, ARGUMENT))
	{
		print_arg_err("exit", NULL, ARGS_ERR, 0);
		return (free_shell(shell), exit(2), 0);
	}
	overflow = 0;
	out = ft_atol(arg->str, &overflow);
	if (overflow)
	{
		print_arg_err("exit", arg->str, NAN_ERR, 1);
		out = 2;
	}
	return (free_shell(shell), exit(out), 0);
}
