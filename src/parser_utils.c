/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:40:11 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/24 16:41:38 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	is_token(char c)
{
	return (c == '|' || c == '<' || c == '>');
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
