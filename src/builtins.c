/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/21 16:08:43 by dzapata          ###   ########.fr       */
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
		return (print_custom_err("cd", ARGS_ERR), 1);
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
		return (print_errno("cd"), 1);
	return (ret);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (print_errno("pwd"), 1);
	return (printf("%s\n", cwd), 0);
}

int	ft_exit(unsigned int ret, t_shell *shell, t_token *token)
{
	t_token	*arg;
	char	*temp;

	printf("exit\n");
	if (!token || !has_args(token))
		return (free_shell(shell), exit(ret), 0);
	arg = get_cmd_token(token, ARGUMENT);
	if (!ft_isnumber(arg->str))
	{
		temp = ft_strjoin("exit: ", arg->str);
		if (!temp)
			return (print_err(ERRNO_ERR), 1);
		return (print_custom_err(temp, NAN_ERR), free(temp), 2);
	}
	if (arg->next && has_args(arg->next))
		return (print_custom_err("exit", ARGS_ERR), 2);
	return (free_shell(shell), exit(ft_atoi(arg->str)), 0);
}
