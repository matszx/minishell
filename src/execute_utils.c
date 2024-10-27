/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:01:06 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/27 20:04:26 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_builtin(char *str)
{
	return (!ft_strncmp(str, "echo", 5)
		|| !ft_strncmp(str, "cd", 3)
		|| !ft_strncmp(str, "pwd", 4)
		|| !ft_strncmp(str, "export", 6)
		|| !ft_strncmp(str, "unset", 5)
		|| !ft_strncmp(str, "env", 4)
		|| !ft_strncmp(str, "exit", 5));
}

int	affects_environtment(char *str)
{
	return (!ft_strncmp(str, "cd", 3)
		|| !ft_strncmp(str, "export", 6)
		|| !ft_strncmp(str, "unset", 5)
		|| !ft_strncmp(str, "exit", 5));
}

void	jump_to_next(t_token **t)
{
	while (*t && (*t)->type != OPERATOR)
		*t = (*t)->next;
	if (*t)
		*t = (*t)->next;
}

int	path_access(char *str)
{
	if (!access(str, F_OK))
	{
		if (!access(str, X_OK))
			return (0);
		return (1);
	}
	return (2);
}

int	perform_redirections(t_token *t, int *fd, int n)
{
	t_token	*temp;
	int		err;

	temp = t;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type != COMMAND && temp->type != ARGUMENT)
		{
			err = redirect(temp, fd, n);
			if (err)
				return (print_arg_err(temp->next->str, NULL, ERRNO_ERR, 0), 1);
		}
		temp = temp->next;
	}
	return (0);
}
