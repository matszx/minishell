/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:55:20 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/07 18:54:13 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Finds a certain type of token within a command
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

// Determines if a string (after the '-') has only echo flags. The verification
// is performed until the string ends or he finds a space
int	has_echo_flag(char *s)
{
	int	flag;

	flag = 0;
	while (*s && !ft_isspace(*s))
	{
		if (*s == 'n')
			flag = 1;
		else if ((*s == 'e' || *s == 'E') && flag)
			flag = 2;
		else
			return (0);
		s++;
	}
	return (flag);
}

int	write_rest(t_token *token, char *temp, int flag)
{
	if (write(STDOUT_FILENO, temp, ft_strlen(temp)) == -1)
		return (3);
	if (get_cmd_token(token->next, ARGUMENT))
		if (write(STDOUT_FILENO, " ", 1) == -1)
			return (3);
	return (flag);
}
