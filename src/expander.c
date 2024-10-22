/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:48:56 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/21 21:34:56 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	valid_name(char c)
{
	return (c && (c == '_' || ft_isalnum(c)));
}

int	valid_expand(char c, char quotes, char next)
{
	return (c == '$' && quotes != SQUOTE && next != '\0' && next != ' '
		&& next != '$' && next != DQUOTE);
}

int	expand(t_expand *e, char *src, char *dst)
{
	char	*temp;
	char	*var;
	int		start;

	start = ++e->i;
	if (src[e->i] == '?')
	{
		temp = ft_itoa(e->status);
		if (!temp)
			return (-1);
		ft_strlcpy(&dst[e->j], temp, ft_strlen(temp) + 1);
		return (e->j += ft_strlen(temp), free(temp), 0);
	}
	while (valid_name(src[e->i]))
		e->i++;
	temp = ft_substr(src, start, e->i - start);
	e->i--;
	if (!temp)
		return (-1);
	var = find_env(e->env, temp);
	free(temp);
	start = ft_strlen(var);
	ft_strlcpy(&dst[e->j], var, start + 1);
	e->j += start;
	return (0);
}

void	init_expand(t_expand *e, char **env, int expand, int status)
{
	e->i = -1;
	e->j = 0;
	e->quotes = '\0';
	e->expand = expand;
	e->status = status;
	e->env = env;
}

int	get_expanded(t_token *t, t_expand *e, char *dst)
{
	while (t->str[++e->i])
	{
		if (t->str[e->i]
			&& valid_expand(t->str[e->i], e->quotes, t->str[e->i + 1])
			&& e->expand)
		{
			if (expand(e, t->str, dst) == -1)
				return (ERRNO_ERR);
			continue ;
		}
		if ((t->str[e->i] == SQUOTE || t->str[e->i] == DQUOTE)
			&& e->quotes == '\0')
			e->quotes = t->str[e->i];
		else if (t->str[e->i] == e->quotes && e->quotes != '\0')
			e->quotes = '\0';
		else if (t->str[e->i] == '\\' && t->str[e->i + 1] && e->quotes == '\0')
			e->i++;
		else if (!t->str[e->i])
			break ;
		else
			dst[e->j++] = t->str[e->i];
	}
	dst[e->j] = '\0';
	return (0);
}

int	parsing(t_format *f, char *str)
{
	int		i;
	char	*sbstr;

	i = 0;
	if (str[++i] == '(')
		f->err = PARENTHESIS_ERR;
	else if (str[i] == '?')
	{
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
			if (parsing(f, &t->str[f->i]))
				return (f->err);
		}
		else if (t->str[f->i] == '\\' && t->str[f->i + 1] && f->quotes == '\0')
		{
			f->rem_slash++;
			f->i++;
		}
	}
	//printf("Quotes: %i\nslash: %i\nvar: %i\nval: %i\nspaces: %i\nlimiter: %i\n",
	//	f.rem_quotes, f.rem_slash, f.len_var, f.len_val, f.spaces_skipped, f.red_limit);
	(*len) = f->i - (f->rem_quotes * 2) - f->rem_slash - f->len_var
		- f->spaces_skipped + (f->len_val) + f->red_limit;
	return (0);
}

int	handle_expansions(t_token *t, char **env, int status, int expand)
{
	int			new_len;
	char		*new_str;
	t_format	f;
	t_expand	e;

	new_len = 0;
	if (t->type != COMMAND && t->type != ARGUMENT && t->type != RED_ARG)
		return (0);
	init_format(&f, env, expand, status);
	if (calculate_len(t, &f, &new_len))
		return (FORMAT_ERR);
	new_str = malloc(sizeof(char) * (new_len + 1));
	init_expand(&e, env, expand, status);
	if (!new_str || get_expanded(t, &e, new_str))
		return (free(new_str), ERRNO_ERR);
	t->len = new_len;
	free(t->str);
	t->str = new_str;
	return (0);
}

int	expand_commands(t_shell *shell)
{
	t_token	*temp;
	int		err;
	int		expand;

	temp = shell->tokens;
	shell->env_var = get_env(shell->env);
	if (!shell->env_var)
		return (ERRNO_ERR);
	while (temp)
	{
		if (temp->type == HEREDOC)
		{
			expand = 0;
			temp = temp->next;
		}
		else
			expand = 1;
		err = handle_expansions(temp, shell->env_var,
			shell->exit_status, expand);
		if (err)
			return (err);
		temp = temp->next;
	}
}
