/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:40:44 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/19 19:27:51 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_builting(char *str)
{
	return (!ft_strncmp(str, "echo", 5)
		|| !ft_strncmp(str, "cd", 3)
		|| !ft_strncmp(str, "pwd", 4)
		|| !ft_strncmp(str, "export", 6)
		|| !ft_strncmp(str, "unset", 5)
		|| !ft_strncmp(str, "env", 4)
		|| !ft_strncmp(str, "exit", 5));
}

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

int	build_command(t_token *t, int *fd, int n)
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
				return (err);
		}
		temp = temp->next;
	}
	return (0);
}

void	jump_to_next(t_token **t)
{
	while (*t && (*t)->type != OPERATOR)
		*t = (*t)->next;
	if (*t)
		*t = (*t)->next;
}

void	execute_process(t_shell *shell, t_token *t)
{
	int		i;
	pid_t	pid;

	i = -1;
	pid = -1;
}

int	execute(t_shell *shell)
{
	int		i;
	int		err;
	t_token	*temp;

	i = -1;
	temp = shell->tokens;
	while (++i < shell->n_commands)
	{
		err = build_command(temp, shell->fd, i);
		if (err)
		{
			print_err(err);
			jump_to_next(&temp);
			continue ;
		}
		execute_process(shell, temp);
		jump_to_next(&temp);
	}
	return (0);
}
