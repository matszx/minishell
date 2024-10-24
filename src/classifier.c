/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:44:51 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/24 16:45:19 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	classify_operator(t_token *head, int *cmd, int *n)
{
	if (!ft_strncmp("<<", head->str, 3))
		head->type = HEREDOC;
	else if (!ft_strncmp(">>", head->str, 3))
		head->type = RED_APP;
	else if (!ft_strncmp("<", head->str, 2))
		head->type = RED_IN;
	else if (!ft_strncmp(">", head->str, 2))
		head->type = RED_OUT;
	else
		head->type = OPERATOR;
	if (head->type == OPERATOR)
	{
		*cmd = 1;
		(*n)++;
	}
	else if (*cmd == 1)
		*cmd = 2;
	else
		*cmd = 3;
}

void	classify_count(t_token *head, int *n)
{
	int	cmd;

	cmd = 1;
	while (head)
	{
		if (is_token(head->str[0]))
			classify_operator(head, &cmd, n);
		else if (cmd == 2 || cmd == 3)
		{
			head->type = RED_ARG;
			if (cmd == 2)
				cmd = 1;
			else
				cmd = 0;
		}
		else if (!cmd)
			head->type = ARGUMENT;
		else
		{
			head->type = COMMAND;
			cmd = 0;
		}
		head = head->next;
	}
}
