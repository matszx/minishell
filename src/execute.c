/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:40:44 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/16 23:04:04 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	argument_manager(t_shell *shell, t_token *head)
{
	if (!ft_strncmp(head->str, "echo", 5))
		return (ft_echo(head->next));
	else if (!ft_strncmp(head->str, "cd", 3))
		return (ft_cd(shell->env_var, head->next));
	else if (!ft_strncmp(head->str, "pwd", 4))
		return (ft_pwd());
	else if (!ft_strncmp(head->str, "export", 6))
		return (ft_export(shell->env, shell->env->head, head->next));
	else if (!ft_strncmp(head->str, "unset", 5))
		return (ft_unset(shell->env, head->next));
	else if (!ft_strncmp(head->str, "env", 4))
		return (ft_env(shell->env, head->next));
	else if (!ft_strncmp(head->str, "exit", 5))
		return (ft_exit((unsigned int)shell->exit_status, shell, head->next));
	else
		return (1);
}

int	execute(t_shell *shell)
{
	t_token		*temp;
	int			err;

	temp = shell->tokens;
	while (temp)
	{
		shell->exit_status = argument_manager(shell, temp);
		while (temp && temp->type != OPERATOR)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
	return (0);
}
