/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:55:20 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/20 21:36:29 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	has_args(t_token *t)
{
	t_token *temp;

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
	t_token *temp;

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
	if (!str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
