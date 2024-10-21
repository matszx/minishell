/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/20 21:53:09 by dzapata          ###   ########.fr       */
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

char	*get_type(t_token *token)
{
	if (token->type == COMMAND)
		return ("Command:\t\t");
	else if (token->type == ARGUMENT)
		return ("Argument:\t\t");
	else if (token->type == HEREDOC)
		return ("Heredoc:\t\t");
	else if (token->type == RED_IN)
		return ("Redirect Input:\t\t");
	else if (token->type == RED_OUT)
		return ("Redirect Output:\t");
	else if (token->type == RED_APP)
		return ("Redirect Append:\t");
	else if (token->type == RED_ARG)
		return ("Redirect Argument:\t");
	else if (token->type == OPERATOR)
		return ("Operator:\t\t");
	return ("\t\t\t");
}

void	print_tokens(t_token *head)
{
	t_token	*temp;
	char	*type;

	temp = head;
	while (temp)
	{
		type = get_type(temp);
		write(1, type, ft_strlen(type));
		write(1, temp->str, temp->len);
		write(1, "\n", 1);
		temp = temp->next;
	}
}

void	print_fd(int *fd, int n)
{
	int	i;

	i = -1;
	while(++i < n)
	{
		if (i % 2)
			printf("Out:\t%i\n", fd[i]);
		else
			printf("In:\t%i\n", fd[i]);
	}
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

	print_tokens(shell->tokens);
	err = verify_order(shell->tokens);
	if (err)
		return (print_err(err));
	expand_commands(shell);
	printf("Expanded =======================================\n");
	print_tokens(shell->tokens);
	err = get_pipes(shell);
	if (err)
		return (print_err(err));
	err = red_heredoc(shell);
	if (err)
		return (print_err(err));
	printf("Executing\n");
	execute(shell);
	printf("Executed\n");
	print_fd(shell->fd, shell->n_commands * 2);
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
				return (perror("Minishell"));
			ft_exit((unsigned int)shell->exit_status, shell, NULL);
		}
		add_history(shell->buf);
		err = parse(shell->buf, shell);
		free(shell->buf);
		if (err)
		{
			if (err != EMPTY_INPUT)
				print_err(err);
			continue ;
		}
		minishell(shell);
		clean(shell);
	}
}
