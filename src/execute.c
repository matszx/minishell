/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:40:44 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/05 19:26:21 by mcygan           ###   ########.fr       */
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
	else if (!ft_strncmp(head->str, "export", 7))
		return (ft_export(shell->env, shell->env->head, head->next));
	else if (!ft_strncmp(head->str, "unset", 6))
		return (ft_unset(shell->env, head->next));
	else if (!ft_strncmp(head->str, "env", 4))
		return (ft_env(shell->env, head->next));
	else if (!ft_strncmp(head->str, "exit", 5))
		return (ft_exit((unsigned int)g_exitstatus, shell, head->next));
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
	close_files(&shell->fd[n], (shell->n_commands - n) * 2);
}

void	child(t_shell *shell, t_token *t, int n)
{
	int		err;
	t_token	*cmd;
	char	*str;
	char	**args;

	if (perform_redirections(t, shell->fd, n))
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
	signal(SIGQUIT, SIG_DFL);
	execve(str, args, shell->env_var);
	signal(SIGQUIT, SIG_IGN);
	return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
}

pid_t	execute_process(t_shell *shell, t_token *t, int n)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		g_exitstatus = 1;
		print_err(ERRNO_ERR);
	}
	else if (pid == 0)
		child(shell, t, n);
	close_files(&shell->fd[n * 2], 2);
	return (pid);
}

void	execute(t_shell *shell)
{
	int		i;
	t_token	*temp;
	pid_t	*pid;

	i = -1;
	temp = get_cmd_token(shell->tokens, COMMAND);
	if (shell->n_commands == 1 && temp && affects_environment(temp->str))
		g_exitstatus = argument_manager(shell, temp);
	else
	{
		temp = shell->tokens;
		pid = malloc(sizeof(pid_t) * shell->n_commands);
		if (!pid)
			return (g_exitstatus = 1, print_err(ERRNO_ERR));
		while (++i < shell->n_commands)
		{
			pid[i] = execute_process(shell, temp, i);
			jump_to_next(&temp);
		}
		signal(SIGINT, SIG_IGN);
		wait_processes(pid, shell);
		signal(SIGINT, sigint_handler);
		free(pid);
	}
}
