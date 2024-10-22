/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:40:44 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/22 17:14:47 by dzapata          ###   ########.fr       */
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
				return (err);
		}
		temp = temp->next;
	}
	return (0);
}

char	**get_args(t_token *t)
{
	t_token	*temp;
	int		len;
	char	**str;

	temp = t;
	len = 0;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type == ARGUMENT)
			len++;
		temp = temp->next;
	}
	str = malloc(sizeof(char *) * (len + 2));
	if (!str)
		return (NULL);
	temp = t;
	len = 1;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type == ARGUMENT)
			str[len++] = temp->str;
		temp = temp->next;
	}
	str[len] = NULL;
	return (str[0] = t->str, str);
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

char	*find_command(t_shell *shell, t_token *cmd, int *code)
{
	char	*str;
	char	**paths;
	char	*c_path;
	int		i;
	int		file;

	if (!cmd->str[0])
		return ((*code) = CMD_NOT_FOUND, NULL);
	file = path_access(cmd->str);
	if (!file)
		return (*code = 0, cmd->str);
	else if (file == 1)
		return (*code = 126, NULL);
	i = -1;
	c_path = find_env(shell->env_var, "PATH");
	if (!c_path)
		c_path = DEFAULT_PATH;
	paths = ft_split(c_path, ':');
	if (!paths)
		return (*code = 1, NULL);
	while (paths[++i])
	{
		str = ft_strjoin(paths[i], "/");
		if (!str)
			return (*code = 1, free_table((void **) paths), NULL);
		c_path = ft_strjoin(str, cmd->str);
		free(str);
		if (!c_path)
			return (*code = 1, free_table((void **) paths), NULL);
		file = path_access(c_path);
		if (!file)
			return (free_table((void **) paths), c_path);
		else if (file == 1)
			return (free(c_path), free_table((void **) paths), NULL);
		free(c_path);
	}
	free_table((void **) paths);
	return ((*code) = CMD_NOT_FOUND, NULL);
}

void	find_error(char *str, int *code)
{
	if (*code == 126 || *code == 127)
		return (print_errno(str));
	else if (*code == CMD_NOT_FOUND)
		return (*code = 127, print_custom_err(str, CMD_NOT_FOUND));
	return (print_err(ERRNO_ERR));
}

void	child(t_shell *shell, t_token *t, int n)
{
	int		err;
	t_token	*cmd;
	char	*str;
	char	**args;

	err = perform_redirections(t, shell->fd, n);
	if (err)
		return (print_err(err), exit(EXIT_FAILURE));
	cmd = get_cmd_token(t, COMMAND);
	if (!cmd)
		exit(EXIT_SUCCESS);
	if (is_builtin(cmd->str))
	{
		close(shell->fd[n * 2]);
		if (shell->fd[(n * 2) + 1] != STDOUT_FILENO && dup2(shell->fd[(n * 2) + 1], STDOUT_FILENO) == -1)
			return (print_err(ERRNO_ERR), exit(EXIT_FAILURE));
		if (shell->fd[(n * 2) + 1] != STDOUT_FILENO)
			close(shell->fd[(n * 2) + 1]);
		exit(argument_manager(shell, cmd));
	}
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
	if (waitpid(pid, &shell->exit_status, 0) == -1)
		print_err(ERRNO_ERR);
	return (0);
}

/*
	n1
	in	out
	n2
	in1	out1	in2	out2
*/