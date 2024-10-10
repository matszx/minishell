/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:49:32 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/10 14:44:20 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	valid_sequence(t_)
{
	
}

int	redirect(t_shell *shell)
{
	int			n_commands;
	t_token		*temp;
	int			err;

	temp = shell->tokens;
	while (temp)
	{
		while (temp && temp->type != OPERATOR)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
}
