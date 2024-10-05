/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 22:20:39 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/05 23:41:22 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_exit(t_shell *shell)
{
	printf("exit\n");
	free_shell(shell);
	exit (EXIT_SUCCESS);
	return (0);
}

int	ft_pwd()
{
	return (0);
}

int	ft_echo(t_token *token)
{
	return (0);
}
