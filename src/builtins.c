/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/25 15:49:55 by dzapata          ###   ########.fr       */
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
			else if (*s == 'e' || *s == 'E')
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

	if (!token)
		return (printf("\n"), 0);
	newline = n_flag(token->str);
	if (!newline || newline == 2)
		token = token->next;
	while (token && token->type != OPERATOR)
	{
		if (token->type == ARGUMENT)
		{
			printf("%s", token->str);
			token = token->next;
			if (get_cmd_token(token, ARGUMENT))
				printf(" ");
		}
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_cd(char **env, t_token *token)
{
	char	*tmp;
	int		ret;
	t_token	*arg;

	arg = get_cmd_token(token, ARGUMENT);
	if (arg && has_args(arg->next))
		return (print_arg_err("cd", NULL, ARGS_ERR, 0), 1);
	if (!arg)
		ret = chdir(find_env(env, "HOME"));
	else
	{
		if (*(arg->str) == '~')
		{
			tmp = ft_strjoin(find_env(env, "HOME"), arg->str + 1);
			if (!tmp)
				return (print_err(ERRNO_ERR), 1);
			ret = chdir(tmp);
			free(tmp);
		}
		else
			ret = chdir(arg->str);
	}
	if (ret == -1)
		return (print_arg_err("cd", NULL, ERRNO_ERR, 0), 1);
	return (ret);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (print_arg_err("pwd", NULL, ERRNO_ERR, 0), 1);
	return (printf("%s\n", cwd), 0);
}

int	ft_exit(unsigned int ret, t_shell *shell, t_token *token)
{
	t_token	*arg;
	int		out;
	int		overflow;

	printf("exit\n");
	if (!token || !has_args(token))
		return (free_shell(shell), exit(ret), 0);
	arg = get_cmd_token(token, ARGUMENT);
	if (!ft_isnumber(arg->str))
		return (print_arg_err("exit", arg->str, NAN_ERR, 1),
			free_shell(shell), exit(2), 0);
	if (arg->next && has_args(arg->next))
	{
		print_arg_err("exit", NULL, ARGS_ERR, 0);
		return (free_shell(shell), exit(2), 0);
	}
	overflow = 0;
	out = ft_atol(arg->str, &overflow);
	if (overflow)
		return (print_arg_err("exit", arg->str, NAN_ERR, 1),
			free_shell(shell), exit(2), 0);
	return (free_shell(shell), exit(out), 0);
}
