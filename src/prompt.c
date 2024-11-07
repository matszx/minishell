/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/11/07 18:55:35 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Returns the appropriate prompt line
static char	*prompt_msg(int exitstatus)
{
	if (!exitstatus)
		return (PT "minishell" AG " > " RESET);
	else
		return (PT "minishell" VM " > " RESET);
}

int	verify_order(t_token *t)
{
	while (t)
	{
		if (t->type == OPERATOR && !(t->next))
			return (PIPE_END_ERR);
		t = t->next;
	}
	return (0);
}

void	clean(t_shell *shell)
{
	close_files(shell->fd, shell->n_commands * 2);
	free(shell->fd);
	shell->fd = NULL;
	destroy_list(&shell->tokens);
	free_table((void **) shell->env_var);
	shell->env_var = NULL;
	shell->n_commands = 0;
}

void	minishell(t_shell *shell)
{
	int	err;

	err = verify_order(shell->tokens);
	if (err)
		return (g_exitstatus = 2, print_err(err));
	err = expand_commands(shell);
	if (err)
		return (g_exitstatus = 1, print_err(err));
	err = get_pipes(shell);
	if (err)
		return (g_exitstatus = 1, print_err(err));
	err = red_heredoc(shell);
	if (err)
		return (g_exitstatus = 1, print_err(err));
	execute(shell);
}

// Displays a prompt
void	prompt(t_shell *shell)
{
	int	err;

	while (1)
	{
		err = errno;
		shell->buf = readline(prompt_msg(g_exitstatus));
		if (!shell->buf && err && errno && err != errno && errno != ENOENT)
			return (perror(MINISHELL));
		else if (!shell->buf)
			ft_exit((unsigned int)g_exitstatus, shell, NULL);
		err = parse(shell->buf, shell);
		if (err)
		{
			if (err != EMPTY_INPUT)
			{
				print_err(err);
				g_exitstatus = 2;
			}
			continue ;
		}
		minishell(shell);
		clean(shell);
	}
}
