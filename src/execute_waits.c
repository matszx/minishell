/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_waits.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:14:59 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/05 16:32:20 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	wait_processes(pid_t *pid, t_shell *shell)
{
	int	i;

	i = -1;
	while (++i < shell->n_commands)
	{
		if (pid[i] != -1 && waitpid(pid[i], &g_exitstatus, 0) == -1)
			print_err(ERRNO_ERR);
	}
	if (pid[shell->n_commands - 1] == -1)
		g_exitstatus = 1;
	if (!WIFSIGNALED(g_exitstatus))
		g_exitstatus = WEXITSTATUS(g_exitstatus);
	else
		g_exitstatus = 128 + WTERMSIG(g_exitstatus);
}
