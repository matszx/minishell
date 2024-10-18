/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/18 16:58:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_flag(char *s)
{
	if (s && *s == '-' && *(++s))
	{
		while (*s)
		{
			if (*s != 'n' && *s != 'e' && *s != 'E')
				return (0);
			s++;
		}
		return (1);
	}
	return (0);
}

int	ft_echo(t_token *token)
{
	int	newline;

	if (!token || token->type != ARGUMENT)
		return (printf("\n"), 0);
	newline = 1;
	if (token && is_flag(token->str))
	{
		newline = 0;
		token = token->next;
	}
	while (token && token->type == ARGUMENT)
	{
		printf("%s", token->str);
		token = token->next;
		if (token)
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_cd(char **env, t_token *token)
{
	char	*tmp;
	int		ret;

	if (token && token->next && token->next->type == ARGUMENT)
		return (printf("minishell: cd: Too many arguments\n"), 1);
	ret = 1;
	if (!token || token->type != ARGUMENT)
		ret = (chdir(find_env(env, "HOME")) != 0);
	else if (token && token->type == ARGUMENT)
	{
		if (*(token->str) == '~')
			tmp = ft_strjoin(find_env(env, "HOME"), token->str + 1);
		else
			tmp = ft_strjoin("", token->str);
		ret = (chdir(tmp) != 0);
		free(tmp);
	}
	if (ret)
		printf("minishell: cd: %s: No such file or directory\n", token->str);
	return (ret);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (1);
	return (printf("%s\n", cwd), 0);
}

int	ft_exit(unsigned int ret, t_shell *shell, t_token *token)
{
	int				i;

	if (token && token->next)
		return (printf("exit\nminishell: exit: Too many arguments\n"), \
			free_shell(shell), exit(1), 0);
	else if (token && token->type == ARGUMENT)
	{
		i = 0;
		if (*token->str == '-' || *token->str == '+')
			i++;
		if (!token->str[i])
			return (printf(\
				"exit\nminishell: exit: %s: Numeric argument required\n", \
				token->str), free_shell(shell), exit(2), 0);
		while (token->str[i])
		{
			if (!ft_isdigit(token->str[i++]))
				return (printf(\
					"exit\nminishell: exit: %s: Numeric argument required\n", \
					token->str), free_shell(shell), exit(2), 0);
		}
		ret = (unsigned char)ft_atoi(token->str);
	}
	return (printf("exit\n"), free_shell(shell), exit(ret), 0);
}
