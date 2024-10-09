/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/09 12:45:18 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_exit(t_shell *shell)
{
	printf("exit\n");
	free_shell(shell);
	exit (EXIT_SUCCESS);
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (1);
	printf("%s\n", cwd);
	return (0);
}

int	ft_cd(char **env, t_token *token)
{
	char	*tmp;
	int		res;

	if (token && token->next && token->next->type == ARGUMENT)
		return (1);
	else if (!token || token->type != ARGUMENT)
		return (chdir(find_env(env, "HOME")) != 0);
	else if (token && token->type == ARGUMENT)
	{
		if (*(token->str) == '~')
			tmp = ft_strjoin(find_env(env, "HOME"), token->str + 1);
		else
			tmp = ft_strjoin("", token->str);
		res = (chdir(tmp) != 0);
		free(tmp);
		return (res);
	}
	return (1);
}

int	ft_echo(t_token *token)
{
	int	newline;

	if (!token || token->type != ARGUMENT)
		return (printf("\n"), 0);
	newline = 1;
	if (token && !ft_strcmp(token->str, "-n"))
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
