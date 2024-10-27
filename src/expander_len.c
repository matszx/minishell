/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:33:56 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/27 00:25:01 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	valid_name(char c)
{
	return (c && (c == '_' || ft_isalnum(c)));
}

int	valid_expand(char c, char quotes, char next)
{
	return ((c == '$' && quotes != SQUOTE && next != '\0' && next != ' '
			&& next != '$' && next != DQUOTE));
}

int	env_var_len(t_format *f, char *str)
{
	int		i;
	char	*sbstr;

	i = 0;
	if (str[++i] == '(')
		f->err = PARENTHESIS_ERR;
	else if (str[i] == '?')
	{
		printf("%i\n", f->status);
		if (f->status > 100)
			f->len_val++;
	}
	else
	{
		while (valid_name(str[i]))
			i++;
		sbstr = ft_substr(str, 1, --i);
		f->len_var += i + 1;
		if (!sbstr)
			f->err = ERRNO_ERR;
		else
			f->len_val += ft_strlen(find_env(f->env, sbstr));
		free(sbstr);
	}
	return (f->err);
}

void	init_format(t_format *f, char **env, int expand, int status)
{
	f->quotes = '\0';
	f->rem_quotes = 0;
	f->len_var = 0;
	f->rem_slash = 0;
	f->len_val = 0;
	f->i = -1;
	f->env = env;
	f->err = 0;
	f->expand = expand;
	f->status = status;
	f->spaces_skipped = 0;
	f->temp = 0;
	f->red_limit = 0;
}

int	calculate_len(t_token *t, t_format *f, int *len)
{
	while (t->str[++f->i])
	{
		if ((t->str[f->i] == SQUOTE || t->str[f->i] == DQUOTE)
			&& t->str[f->i + 1] && f->quotes == '\0')
		{
			f->quotes = t->str[f->i++];
			f->rem_quotes++;
		}
		if (t->str[f->i] == f->quotes && f->quotes != '\0')
			f->quotes = '\0';
		else if (valid_expand(t->str[f->i], f->quotes, t->str[f->i + 1])
			&& f->expand)
		{
			if (env_var_len(f, &t->str[f->i]))
				return (f->err);
		}
		else if (t->str[f->i] == '\\' && t->str[f->i + 1] && f->quotes == '\0')
		{
			f->rem_slash++;
			f->i++;
		}
	}
	(*len) = f->i - (f->rem_quotes * 2) - f->rem_slash - f->len_var
		- f->spaces_skipped + (f->len_val) + f->red_limit;
	return (0);
}
