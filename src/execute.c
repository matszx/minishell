/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:40:44 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/29 12:55:38 by dzapata          ###   ########.fr       */
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

void	files(t_shell *shell, int n)
{
	if (shell->fd[(n * 2)] != STDIN_FILENO
		&& dup2(shell->fd[(n * 2)], STDIN_FILENO) == -1)
		return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
	if (shell->fd[(n * 2) + 1] != STDOUT_FILENO
		&& dup2(shell->fd[(n * 2) + 1], STDOUT_FILENO) == -1)
		return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
	if (shell->fd[(n * 2)] != STDIN_FILENO)
		close(shell->fd[(n * 2)]);
	if (shell->fd[(n * 2) + 1] != STDOUT_FILENO)
		close(shell->fd[(n * 2) + 1]);
}

void	child(t_shell *shell, t_token *t, int n)
{
	int		err;
	t_token	*cmd;
	char	*str;
	char	**args;

	err = perform_redirections(t, shell->fd, n);
	if (err)
		return (exit(EXIT_FAILURE));
	cmd = get_cmd_token(t, COMMAND);
	if (!cmd)
		exit(EXIT_SUCCESS);
	files(shell, n);
	if (is_builtin(cmd->str))
		exit(argument_manager(shell, cmd));
	err = 0;
	str = find_command(shell, cmd, &err);
	if (!str)
		return (find_error(cmd->str, &err), exit(err));
	args = get_args(cmd);
	if (!args)
		return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
	execve(str, args, shell->env_var);
	return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
}

pid_t	execute_process(t_shell *shell, t_token *t, int n)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		shell->exit_status = 1;
		print_err(ERRNO_ERR);
	}
	else if (pid == 0)
		child(shell, t, n);
	else
	{
		if (shell->fd[(n * 2)] != STDIN_FILENO)
			close(shell->fd[n * 2]);
		if (shell->fd[(n * 2) + 1] != STDOUT_FILENO)
			close (shell->fd[(n * 2) + 1]);
	}
	return (pid);
}

int	execute(t_shell *shell)
{
	int		i;
	t_token	*temp;
	pid_t	pid;

	i = -1;
	temp = get_cmd_token(shell->tokens, COMMAND);
	pid = -1;
	if (shell->n_commands == 1 && temp && affects_environtment(temp->str))
		return (argument_manager(shell, temp));
	temp = shell->tokens;
	while (++i < shell->n_commands)
	{
		pid = execute_process(shell, temp, i);
		jump_to_next(&temp);
	}
	signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &shell->exit_status, 0) == -1)
	{
		shell->exit_status = 1;
		print_err(ERRNO_ERR);
	}
	signal(SIGINT, sigint_handler);
	shell->exit_status = WEXITSTATUS(shell->exit_status);
	return (0);
}
