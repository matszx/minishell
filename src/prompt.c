/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/06 00:25:32 by dzapata          ###   ########.fr       */
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

int	argument_manager(t_shell *shell, t_token *head)
{
	if (!ft_strncmp(head->str, "exit", 5))
		return (ft_exit(shell));
	else if (!ft_strncmp(head->str, "env", 4))
		return (ft_env(shell->env));
	else if (!ft_strncmp(head->str, "unset", 5))
		return (ft_unset(shell->env, head->next->str));
	else if (!ft_strncmp(head->str, "export", 6))
		return (ft_export(shell->env->head, head->next->str));
	else if (!ft_strncmp(head->str, "pwd", 4))
		return (ft_pwd());
	else if (!ft_strncmp(head->str, "echo", 5))
		return (ft_echo(head->next));
	else
		return (1);
}

char	**get_env(t_env *env)
{
	return (NULL);
}

void	execute(t_shell *shell)
{
	t_token	*temp;
	char	**env;

	temp = shell->tokens;
	env = get_env(shell->env);
	while (temp)
	{
		print_tokens(temp);
		shell->exit_status = argument_manager(shell, temp);
		while (temp && temp->type != OPERATOR)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
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
				return (perror ("Minishell: "));	
			ft_exit(shell);
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
		execute(shell);
		destroy_list(&shell->tokens);
	}
}
