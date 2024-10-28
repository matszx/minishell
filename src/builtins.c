/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/28 01:16:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	n_flag(char *s)
{
	int	flag;

	flag = 1;
	if (s && *s == '-' && *(++s))
	{
		while (*s)
		{
			if (*s == 'n')
				flag = 0;
			else if ((*s == 'e' || *s == 'E') && flag)
				flag = 2;
			else
				return (1);
			s++;
		}
	}
	return (flag);
}

int	ft_echo(t_token *token)
{
	int	newline;
	int	err;

	newline = 1;
	err = 0;
	if (token)
	{
		newline = n_flag(token->str);
		if (!newline || newline == 2)
			token = get_cmd_token(token->next, ARGUMENT);
		while (token)
		{
			err = write(STDOUT_FILENO, token->str, token->len);
			token = get_cmd_token(token->next, ARGUMENT);
			if (token && err != -1)
				err = write(STDOUT_FILENO, " ", 1);
			if (err == -1)
				break ;
		}
	}
	if (newline && err != -1)
		err = write(STDOUT_FILENO, "\n", 1);
	if (err == -1)
		return (print_arg_err("echo", "write error", ERRNO_ERR, 0), 1);
	return (0);
}

int	ft_cd(char **env, t_token *token)
{
	char	*tmp;
	int		ret;
	t_token	*arg;

	arg = get_cmd_token(token, ARGUMENT);
	if (arg && get_cmd_token(arg->next, ARGUMENT))
		return (print_arg_err("cd", NULL, ARGS_ERR, 0), 1);
	if (!arg)
	{
		tmp = find_env(env, "HOME");
		if (!tmp[0])
			return (print_arg_err("cd", NULL, HOME_ERR, 0), 1);
	}
	else
		tmp = arg->str;
	ret = chdir(tmp);
	if (ret == -1)
		return (print_arg_err("cd", tmp, ERRNO_ERR, 0), 1);
	return (ret);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (print_arg_err("pwd", NULL, ERRNO_ERR, 0), 1);
	if (write(STDOUT_FILENO, cwd, ft_strlen(cwd)) == -1)
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
