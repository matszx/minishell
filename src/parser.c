/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:20:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/16 16:37:00 by dzapata          ###   ########.fr       */
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

int	skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

/*void	classify(t_token *head)
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
		else if (command)
		{
			temp->type = COMMAND;
			command = 0;
		}
		else
			temp->type = ARGUMENT;
		temp = temp->next;
	}
}*/

t_token	*new_node(char *s)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->str = s;
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

t_token	*get_arguments(char **splitted, int *n_commands)
{
	t_token	*head;
	t_token	*temp;

	head = new_node(NULL);
	if (!head)
		return (NULL);
	temp = head;
	*n_commands = -1;
	while (splitted[++(*n_commands)])
	{
		temp->next = new_node(splitted[*n_commands]);
		if (!temp->next)
			return (destroy_list(&head), NULL);
		temp = temp->next;
	}
	remove_dummy(&head);
	return (head);
}

int	is_operator(char c)
{
	return (c == '|' || c == ';' || c == '&');
}

int	valid_redirect_input(char *str, int *i, char c)
{
	(*i)++;
	if (str[(*i)] == c)
		(*i)++;
	*i += skip_spaces(&str[*i]);
	return (str[*i] == '<' || str[*i] == '>'
		|| is_operator(str[*i]) || !str[*i]);
}

int	verify_redirect(char *str, int *i)
{
	if (str[*i] == '<')
		return (valid_redirect_input(str, i, '<'));
	else if (str[*i] == '>')
		return (valid_redirect_input(str, i, '>'));
	return (0);
}

int	check_string(char *str)
{
	int		i;
	int		command;

	command = 0;
	i = -1;
	while (str[++i])
	{
		i += skip_spaces(&str[i]);
		if (str[i] == '|'&& !command)
			return (SYNTAX_ERR);
		else if (str[i] == '|')
			command = 0;
		else if (verify_redirect(str, &i))
			return (SYNTAX_ERR);
		else if (!str[i])
			break;
		else
			command = 1;
	}
	return (0);
}

int	parse(char	*buf, t_shell *shell)
{
	char	**splitted;

	shell->tokens = NULL;
	if (!buf[0] || !buf[skip_spaces(buf)])
		return (EMPTY_INPUT);
	if (!quotes_closed(buf))
		return (QUOTES_ERR);
	else if(check_string(buf))
		return (SYNTAX_ERR);
	splitted = ft_split(buf, '|');
	if (!splitted)
		return (SPLIT_ERR);
	shell->tokens = get_arguments(splitted, &shell->n_commands);
	free(splitted);
	if (!(shell->tokens))
		return (LIST_ERR);
	return (0);
}
