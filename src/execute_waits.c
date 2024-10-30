/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_wais.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:14:59 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/30 18:15:17 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	wait_processes(pid_t *pid, t_shell *shell)
{
	int	i;

	i = -1;
	while (++i < shell->n_commands)
	{
		if (pid[i] != -1 && waitpid(pid[i], &shell->exit_status, 0) == -1)
			print_err(ERRNO_ERR);
	}
	if (pid[shell->n_commands - 1] == -1)
		shell->exit_status = 1;
	else
		shell->exit_status = WEXITSTATUS(shell->exit_status);
}
