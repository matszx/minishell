/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:20:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/19 18:08:34 by dzapata          ###   ########.fr       */
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

int	skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

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

int		is_token(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	start_token(char *str, int *i, int *len)
{
	if (str[*i] == '<' || str[*i] == '>')
	{
		(*len)++;
		(*i)++;
		if (str[*i] == str[(*i) - 1])
		{
			(*i)++;
			(*len)++;
		}
		return (1);
	}
	else if (str[*i] == '|')
	{
		(*i)++;
		(*len)++;
		return (1);
	}
	return (0);
}

int		token_len(char *str, int *i)
{
	char	quotes;
	int		len;

	quotes = '\0';
	len = 0;
	if (start_token(str, i, &len))
		return (len);
	while (str[*i])
	{
		if ((str[*i] == SQUOTE || str[*i] == DQUOTE) && !quotes)
			quotes = str[*i];
		else if (str[*i] == quotes && quotes)
			quotes = '\0';
		else if ((ft_isspace(str[*i]) || is_token(str[*i])) && !quotes)
			break ;	
		len++;
		(*i)++;
	}
	return (len);
}

char	*get_token(char *str, int *i)
{
	int		len;
	char	*new_str;
	int		prev_i;

	prev_i = *i;
	len = token_len(str, i);
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, &str[prev_i], len + 1);
	new_str[len] = '\0';
	return (new_str);
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
	i = 0;
	while (str[i])
	{
		token_str = get_token(str, &i);
		temp->next = new_node(token_str);
		if (!token_str || !temp)
			return (free(token_str), destroy_list(&head), NULL);
		temp = temp->next;
		i += skip_spaces(&str[i]);
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
		if (str[i] == '|' && !command)
			return (SYNTAX_ERR);
		else if (str[i] == '|')
			command = 0;
		else if (verify_redirect(str, &i))
			return (SYNTAX_ERR);
		else if (!str[i])
			break ;
		else
			command = 1;
	}
	return (0);
}

void	classify_operator(t_token *head, int *cmd, int *n)
{
	if (!ft_strncmp("<<", head->str, 3))
		head->type = HEREDOC;
	else if (!ft_strncmp(">>", head->str, 3))
		head->type = RED_APP;
	else if (!ft_strncmp("<", head->str, 2))
		head->type = RED_IN;
	else if (!ft_strncmp(">", head->str, 2))
		head->type = RED_OUT;
	else
		head->type = OPERATOR;
	if (head->type == OPERATOR)
	{
		*cmd = 1;
		(*n)++;
	}
	else if (*cmd == 1)
		*cmd = 2;
	else
		*cmd = 3;
}

void	classify_count(t_token *head, int *n)
{
	int	cmd;

	cmd = 1;
	while (head)
	{
		if (is_token(head->str[0]))
			classify_operator(head, &cmd, n);
		else if (cmd == 2 || cmd == 3)
		{
			head->type = RED_ARG;
			if (cmd == 2)
				cmd = 1;
			else
				cmd = 0;
		}
		else if (!cmd)
			head->type = ARGUMENT;
		else
		{
			head->type = COMMAND;
			cmd = 0;
		}
		head = head->next;
	}
}

int	parse(char	*buf, t_shell *shell)
{
	shell->tokens = NULL;
	if (!buf[0] || !buf[skip_spaces(buf)])
		return (EMPTY_INPUT);
	if (!quotes_closed(buf))
		return (QUOTES_ERR);
	else if (check_string(buf))
		return (SYNTAX_ERR);
	shell->tokens = get_arguments(buf);
	if (!(shell->tokens))
		return (ERRNO_ERR);
	shell->n_commands = 1;
	classify_count(shell->tokens, &shell->n_commands);
	return (0);
}
