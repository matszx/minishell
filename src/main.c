/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:22:35 by mcygan            #+#    #+#             */
/*   Updated: 2024/11/07 16:11:53 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_table(void **table)
{
	int	i;

	if (!table)
		return ;
	i = -1;
	while (table[++i])
	{
		free(table[i]);
		table[i] = NULL;
	}
	free(table);
}

void	close_files(int *fd, int n)
{
	int	i;
	int	err;

	err = errno;
	i = -1;
	if (!fd)
		return ;
	while (++i < n)
		if (fd[i] > -1 && fd[i] != STDOUT_FILENO
			&& fd[i] != STDIN_FILENO && fd[i] != STDERR_FILENO)
			close(fd[i]);
	errno = err;
}

void	free_shell(t_shell *shell)
{
	while (shell->env)
	{
		close_files(shell->fd, shell->n_commands * 2);
		free(shell->fd);
		shell->fd = NULL;
		destroy_list(&shell->tokens);
		destroy_env(&shell->env);
		free_table((void **) shell->env_var);
	}
	free(shell);
}

static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;
	char	*tmp;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = copy_env(envp);
	if (!shell->env)
		return (free_shell(shell), NULL);
	shell->env_var = NULL;
	shell->fd = NULL;
	shell->n_commands = 0;
	shell->tokens = NULL;
	shell->cwd[0] = '\0';
	if (!getcwd(shell->cwd, PATH_MAX))
		print_arg_err(NULL, NO_PWD_MSG, ERRNO_ERR, 0);
	if (shell->cwd[0])
	{
		tmp = ft_strjoin("PWD=", shell->cwd);
		if (!tmp || addenv(shell->env, shell->env->head, tmp))
			print_err(ERRNO_ERR);
		free(tmp);
	}
	init_signals();
	return (shell);
}

int		g_exitstatus = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	if (argc > 1)
		return (print_err(ARGS_ERR), EXIT_FAILURE);
	g_exitstatus = 0;
	shell = init_shell(envp);
	if (!shell)
		return (print_err(ERRNO_ERR), EXIT_FAILURE);
	prompt(shell);
	return (free_shell(shell), EXIT_SUCCESS);
}
