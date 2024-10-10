/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/10 14:43:35 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Returns the appropriate prompt line
static char	*prompt_msg(int exit_status)
{
	if (!exit_status)
		return (PT "minishell" AG " > " RESET);
	else
		return (PT "minishell" VM " > " RESET);
}

void	print_tokens(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		printf("STR: %s\n", temp->str);
		printf("TYPE: %i\n", temp->type);
		printf("\n");
		temp = temp->next;
	}
}

void	print_err(int err)
{
	write(STDERR_FILENO, "Minishell: ", 12);
	if (err == QUOTES_ERR)
		write(STDERR_FILENO, "Unclosed quotes\n", 17);
	else if (err == SPLIT_ERR)
		write(STDERR_FILENO, "Error splitting command line\n", 30);
	else if (err == LIST_ERR)
		write(STDERR_FILENO, "Error initializing list\n", 25);
	else
		perror(NULL);
}

// Displays a prompt
void	prompt(t_shell *shell)
{
	int	err;

	while (1)
	{
		err = errno;
		shell->buf = readline(prompt_msg(shell->exit_status));
		if (!shell->buf)
		{
			if (err != errno)
				return (perror("Minishell: "));
			ft_exit((unsigned int)shell->exit_status, shell, NULL);
		}
		add_history(shell->buf);
		err = parse(shell->buf, &shell->tokens);
		free(shell->buf);
		if (err)
		{
			if (err != EMPTY_INPUT)
				print_err(err);
			continue ;
		}
		expand_commands(shell);
		redirect(shell);
		execute(shell);
		destroy_list(&shell->tokens);
	}
}
