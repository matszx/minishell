/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/02 15:39:50 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*prompt(void)
{
	char	*user_input;

	while (1)
	{
		user_input = readline("minishell$ ");
		if (!user_input)
			eof_handler();
		if (!ft_strncmp(user_input, "exit", 5))
		{
			write(1, "exit\n", 5);
			exit(EXIT_SUCCESS);
		}
		add_history(user_input);
		free(user_input);
	}
	return (user_input);
}
