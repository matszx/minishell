/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/02 16:43:56 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Returns the appropriate prompt line
static char	*prompt_msg(int exit_status)
{
	if (!exit_status)
		return (PT "minishell" AG " > " RESET);
	else
		return (PT "minishell" VM " > " RESET);
}

// Displays a prompt
void	prompt(t_shell *shell)
{
	char	*tmp;

	while (1)
	{
		shell->buf = readline(prompt_msg(shell->exit_status));
		if (!shell->buf || !ft_strncmp(shell->buf, "exit", 5))
		{
			free(shell->buf);
			printf("exit\n");
			return ;
		}
		else if (!ft_strncmp(shell->buf, "env", 4))
			shell->exit_status = ft_env(shell->env);
		else if (!ft_strncmp(shell->buf, "unset", 5))
			shell->exit_status = ft_unset(shell->env, shell->buf + 6);
		else if (!ft_strncmp(shell->buf, "export", 6))
			shell->exit_status = ft_export(shell->env, shell->buf + 7);
		else
			shell->exit_status = 1;
		tmp = space_line(shell->buf);
		printf("%s\n", tmp);
		free(tmp);
		add_history(shell->buf);
		free(shell->buf);
	}
}
