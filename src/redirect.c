/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:49:32 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/11 17:21:28 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_commands(t_token *head)
{
	t_token	*temp;
	int		n;

	temp = head;
	n = 1;
	while (temp)
	{
		if (temp->type == OPERATOR)
			n++;
	}
	return (n);
}

int	get_pipes(t_shell *shell, int n_commands)
{
	int	i;

	shell->fd = malloc(sizeof(int) * n_commands * 2);
	if (!shell->fd)
		return (ERRNO_ERR);
	i = 0;
	while (i < n_commands)
	{
		if (pipe(&shell->fd[i]) == -1)
		{
			free(shell->fd);
			shell->fd = NULL;
			return (ERRNO_ERR);
		}
		i += 2;
	}
	return (0);
}

int	redirect(t_shell *shell)
{
	int			n_commands;
	t_token		*temp;
	int			err;

	n_commands = count_commands(shell->tokens);
	err = get_pipes(shell, n_commands);
	if (err)
		return (err);
	temp = shell->tokens;
	/*while (temp)
	{
		temp = temp->next;
	}*/
}
