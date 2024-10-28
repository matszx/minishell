/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:22:35 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/28 00:27:55 by dzapata          ###   ########.fr       */
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

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_var = NULL;
	shell->tokens = NULL;
	shell->fd = NULL;
	shell->n_commands = 0;
	shell->env = copy_env(envp);
	if (!shell->env)
		return (free_shell(shell), NULL);
	shell->exit_status = 0;
	init_signals();
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	if (argc > 1)
		return (print_err(ARGS_ERR), EXIT_FAILURE);
	shell = init_shell(envp);
	if (!shell)
		return (print_err(ERRNO_ERR), EXIT_FAILURE);
	prompt(shell);
	return (free_shell(shell), EXIT_SUCCESS);
}
