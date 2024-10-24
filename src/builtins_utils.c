/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:55:20 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/24 19:09:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	has_args(t_token *t)
{
	t_token	*temp;

	temp = t;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type == ARGUMENT)
			return (1);
		temp = temp->next;
	}
	return (0);
}

t_token	*get_cmd_token(t_token *t, t_type type)
{
	t_token	*temp;

	temp = t;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type == type)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

int	ft_isnumber(char *str)
{
	if (str && (*str == '-' || *str == '+'))
		str++;
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit((int) *str))
			return (0);
		str++;
	}
	return (1);
}
