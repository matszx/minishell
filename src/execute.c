/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:40:44 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/20 23:22:14 by dzapata          ###   ########.fr       */
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

void	child(t_shell *shell, t_token *t, int n)
{
	int	err;

	err = build_command(t, shell->fd, n);
	if (err)
		return (print_err(err), exit(EXIT_FAILURE));
	if (is_builting(t->str))
	{
		close(shell->fd[n * 2]);
		if (dup2(shell->fd[(n * 2) + 1], STDOUT_FILENO) == -1)
			return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
		exit(argument_manager(shell, t));
	}
	exit(EXIT_SUCCESS);
}

void	parent(t_shell *shell, t_token *t, int n)
{
	close(shell->fd[n * 2]);
	if (shell->fd[(n * 2) + 1] != STDOUT_FILENO)
		close (shell->fd[(n * 2) + 1]);
}

pid_t	execute_process(t_shell *shell, t_token *t, int n)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		print_err(ERRNO_ERR);
	else if (pid == 0)
		child(shell, t, n);
	else
		parent(shell, t, n);
	return (pid);
}

int	execute(t_shell *shell)
{
	int		i;
	int		err;
	t_token	*temp;
	pid_t	pid;

	i = -1;
	temp = get_cmd_token(shell->tokens, COMMAND);
	pid = -1;
	if (shell->n_commands == 1 && temp && !ft_strncmp(temp->str, "exit", 5))
		return (ft_exit((unsigned int) shell->exit_status, shell, temp->next));
	temp = shell->tokens;
	while (++i < shell->n_commands)
	{
		pid = execute_process(shell, temp, i);
		jump_to_next(&temp);
	}
	if (waitpid(pid, &shell->exit_status, 0) == -1)
		print_err(ERRNO_ERR);
	return (0);
}
