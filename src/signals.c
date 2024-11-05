/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:34:56 by mcygan            #+#    #+#             */
/*   Updated: 2024/11/05 13:27:23 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	sigint_handler(int sig)
{
	g_exitstatus = 128 + sig;
	write(STDOUT_FILENO, "\n", 1);
	readline(PT "minishell" VM " > " RESET);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sigchld_handler(int sig)
{
	(void)sig;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGCHLD, sigchld_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
