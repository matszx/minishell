/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:22:35 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/04 19:49:49 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_shell(t_shell *shell)
{
	t_env	*prev;

	while (shell->env)
	{
		prev = shell->env;
		shell->env = shell->env->next;
		destroy_list(&shell->tokens);
		free(prev->var);
		free(prev->value);
		free(prev);
	}
	free(shell);
}

static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(*shell));
	if (!shell)
		return (NULL);
	shell->env = copy_env(envp);
	shell->exit_status = 0;
	init_signals();
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	if (argc > 1)
		return (printf("error: does not accept arguments\n"), EXIT_FAILURE);
	shell = init_shell(envp);
	if (!shell)
		return (printf("error: could not allocate memory\n"), EXIT_FAILURE);
	prompt(shell);
	return (free_shell(shell), EXIT_SUCCESS);
}
