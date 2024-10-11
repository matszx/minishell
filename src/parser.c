/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:20:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/11 17:56:07 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	quotes_closed(char *s)
{
	char	status;

	status = 0;
	while (*s)
	{
		if ((*s == SQUOTE || *s == DQUOTE) && !status)
			status = *s;
		else if (*s == status)
			status = 0;
		s++;
	}
	return (!status);
}

void	replace_spaces(char *str)
{
	int		i;
	char	c;

	i = -1;
	while (str[++i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			str[i] = '\x1F';
		if (str[i] == DQUOTE || str[i] == SQUOTE)
		{
			c = str[i++];
			while (str[i] && str[i] != c)
				i++;
			if (!str[i])
				break ;
		}
	}
}

char *skip_spaces(char *str)
{
	char	*str2;

	str2 = str;
	while (str2 && ft_isspace(*str))
		str2++;
	return (str2);
}

void	classify(t_token *head)
{
	t_token	*temp;
	int		command;

	temp = head;
	command = 1;
	while (temp)
	{
		if (!ft_strncmp(temp->str, "|", 2))
		{
			temp->type = OPERATOR;
			command = 1;
		}
		else if (!ft_strncmp(temp->str, "<", 2)	|| !ft_strncmp(temp->str, ">", 2)
			|| !ft_strncmp(temp->str, "<<", 3) || !ft_strncmp(temp->str, ">>", 3))
			temp->type = REDIRECT;
		else if (command)
		{
			temp->type = COMMAND;
			command = 0;
		}
		else
			temp->type = ARGUMENT;
		temp = temp->next;
	}
}

t_token	*new_node(char *s, t_token *prev)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->str = s;
	node->prev = prev;
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

t_token	*get_arguments(char **splitted)
{
	t_token	*head;
	t_token	*temp;
	t_token	*prev;
	int		i;

	head = new_node(NULL, NULL);
	if (!head)
		return (NULL);
	temp = head;
	prev = NULL;
	i = -1;
	while (splitted[++i])
	{
		temp->next = new_node(splitted[i], prev);
		if (!temp->next)
			return (destroy_list(&head), NULL);
		prev = temp->next;
		temp = temp->next;
	}
	remove_dummy(&head);
	classify(head);
	return (head);
}

int	parse(char	*buf, t_token **head)
{
	char	**splitted;
	char	*jumped;

	*head = NULL;
	if (!quotes_closed(buf))
		return (QUOTES_ERR);
	jumped = skip_spaces(buf);
	if (!jumped)
		return (EMPTY_INPUT);
	else if(jumped[0] == '|')
		return (SYNTAX_ERR);
	splitted = ft_split(jumped, '|');
	if (!splitted)
		return (SPLIT_ERR);
	*head = get_arguments(splitted);
	free(splitted);
	if (!(*head))
		return (LIST_ERR);
	return (0);
}
