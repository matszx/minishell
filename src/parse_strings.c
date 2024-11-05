/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:47:19 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/05 16:27:57 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	quotes_closed(char *s)
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

int	token_len(char *str, int *i)
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
		else if ((ft_isspace(str[*i]) || is_special_token(str[*i])) && !quotes)
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

int	check_string(char *str)
{
	int		i;
	int		command;
	char	quotes;

	command = 0;
	i = -1;
	while (str[++i])
	{
		i += skip_spaces(&str[i]);
		if (str[i] == SQUOTE || str[i] == DQUOTE)
		{
			quotes = str[i++];
			while (str[i] && str[i] != quotes)
				i++;
		}
		if (str[i] == '|' && !command)
			return (SYNTAX_ERR);
		else if (str[i] == '|')
			command = 0;
		else if (verify_redirect(str, &i))
			return (SYNTAX_ERR);
		if (!str[i])
			break ;
		else
			command = 1;
	}
	return (0);
}
