/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/07 14:33:44 by mcygan           ###   ########.fr       */
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
	char	*cwd;

	cwd = NULL;
	getcwd(cwd, 256);
	if (!cwd)
		return (1);
	printf("%s\n", cwd);
	return (0);
}

int	ft_cd(t_token *token)
{
	if (!token || token->type != ARGUMENT|| chdir(token->str))
		return (1);
	return (0);
}

int	ft_echo(t_token *token)
{
	int	newline;

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
