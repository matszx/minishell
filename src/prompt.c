/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/04 12:38:08 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

#define PT		"\x1b[38;5;195m"	// pale turquoise
#define AG		"\x1b[38;5;191m"	// apple green
#define RESET	"\x1b[0m"

char	*prompt(void)
{
	char	*user_input;

	while (1)
	{
		user_input = readline(PT "minishell" AG " > " RESET);
		if (!user_input)
			eof_handler();
		if (!ft_strncmp(user_input, "exit", 5))
		{
			printf("exit\n");
			exit(EXIT_SUCCESS);
		}
		add_history(user_input);
		free(user_input);
	}
	return (user_input);
}
