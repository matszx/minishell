/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/24 12:47:42 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*prompt_msg(int	exit_status)
{
	if (!exit_status)
		return (PT "minishell" AG " > " RESET);
	else
		return (PT "minishell" VM " > " RESET);
}

void	prompt(t_shell *shell)
{
	while (1)
	{
		shell->buf = readline(prompt_msg(shell->exit_status));
		if (!shell->buf || !ft_strncmp(shell->buf, "exit", 5))
		{
			free(shell->buf);
			printf("exit\n");
			return ;
		}
		add_history(shell->buf);
		free(shell->buf);
	}
}
