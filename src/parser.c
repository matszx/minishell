/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:20:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/11/05 19:38:51 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*new_node(char *s)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->str = s;
	node->old_str = NULL;
	if (!s)
		node->len = 0;
	else
		node->len = ft_strlen(s);
	node->next = NULL;
	return (node);
}

void	destroy_list(t_token **head)
{
	t_token	*temp;
	t_token	*next;

	if (!head || !(*head))
		return ;
	temp = (*head);
	while (temp)
	{
		free(temp->str);
		free(temp->old_str);
		next = temp->next;
		free(temp);
		temp = next;
	}
	*head = NULL;
}

void	remove_dummy(t_token **token)
{
	t_token	*temp;

	if ((*token))
	{
		temp = *token;
		*token = (*token)->next;
		free(temp);
		temp = NULL;
	}
}

t_token	*get_arguments(char *str)
{
	t_token	*head;
	t_token	*temp;
	int		i;
	char	*token_str;

	head = new_node(NULL);
	if (!head)
		return (NULL);
	temp = head;
	i = skip_spaces(str);
	while (str[i])
	{
		token_str = get_token(str, &i);
		temp->next = new_node(token_str);
		if (!token_str || !temp->next)
			return (free(token_str), destroy_list(&head), NULL);
		temp = temp->next;
		i += skip_spaces(&str[i]);
	}
	remove_dummy(&head);
	return (head);
}

int	parse(char	*buf, t_shell *shell)
{
	shell->tokens = NULL;
	if (!buf[0] || !buf[skip_spaces(buf)])
		return (free(buf), EMPTY_INPUT);
	add_history(buf);
	if (!quotes_closed(buf))
		return (free(buf), QUOTES_ERR);
	else if (check_string(buf))
		return (free(buf), SYNTAX_ERR);
	shell->tokens = get_arguments(buf);
	free(buf);
	if (!(shell->tokens))
		return (ERRNO_ERR);
	shell->n_commands = 1;
	classify_count(shell->tokens, &shell->n_commands);
	return (0);
}
