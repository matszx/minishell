/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/03 14:51:23 by mcygan           ###   ########.fr       */
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
			printf("exit\n");
			exit(EXIT_SUCCESS);
		}
		add_history(user_input);
		free(user_input);
	}
	return (user_input);
}
