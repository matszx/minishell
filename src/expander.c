/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:48:56 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/16 13:41:32 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	valid_name(char c)
{
	return (c && (c == '_'|| ft_isalnum(c)));
}

int	valid_expand(char c, char quotes, char next)
{
	return (c == '$' && quotes != SQUOTE && next != '\0' && next != ' '
		&& next != '$' && next != DQUOTE);
}

int	expand(t_expand *e, char *src, char *dst, int status)
{
	char	*temp;
	char	*var;
	int		start;

	start = ++e->i;
	if (src[e->i] == '?')
	{
		temp = ft_itoa(status);
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

void	init_expand(t_expand *expand, char **env)
{
	expand->i = -1;
	expand->j = 0;
	expand->quotes = '\0';
	expand->env = env;
	expand->heredoc = 0;
}

void	redirect_expand(t_expand *e, char *src, char *dst)
{
	dst[e->j++] = src[e->i++];
	if (src[e->i] == '<')
	{
		e->heredoc = 1;
		dst[e->j++] = src[e->i];
	}
	else if (src[e->i] == '>')
		dst[e->j++] = src[e->i];
	while (ft_isspace(src[e->i + 1]))
		e->i++;
}

int	get_expanded(char *src, char *dst, char **env, int status)
{
	t_expand	e;

	init_expand(&e, env);
	while (src[++e.i])
	{
		if (src[e.i] && valid_expand(src[e.i], e.quotes, src[e.i + 1]) && !e.heredoc)
		{
			if (expand(&e, src, dst, status) == -1)
				return (ERRNO_ERR);
			continue ;
		}
		if ((src[e.i] == SQUOTE || src[e.i] == DQUOTE) && e.quotes == '\0')
			e.quotes = src[e.i];
		else if (src[e.i] == e.quotes && e.quotes != '\0')
			e.quotes = '\0';
		else if (src[e.i] == '\\' && src[e.i + 1] && e.quotes == '\0')
			e.i++;
		else if (src[e.i] == '<' || src[e.i] == '>')
			redirect_expand(&e, src, dst);
		else if (redirect_delimiter(src[e.i]) && e.heredoc)
		{
			e.heredoc = 0;
			dst[e.j++] = src[e.i];
		}
		else if (!src[e.i])
			break ;
		else
			dst[e.j++] = src[e.i];
	}
	dst[e.j] = '\0';
	return (0);
}

void	parsing(t_format *f, char *str, int status)
{
	int		i;
	char	*sbstr;

	i = 0;
	if (str[++i] == '(')
		f->err = PARENTHESIS_ERR;
	else if (str[i] == '?')
	{
		if (status > 100)
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
}

void	init_format(t_format *f, char **env)
{
	f->quotes = '\0';
	f->rem_quotes = 0;
	f->len_var = 0;
	f->rem_slash = 0;
	f->len_val = 0;
	f->i = -1;
	f->env = env;
	f->err = 0;
	f->heredoc = 0;
	f->spaces_skipted = 0;
	f->temp = 0;
}

void	redirect_len(t_format *f, char *str)
{
	int	skip_red;

	if (str[f->i + 1] == '>')
	{
		f->heredoc = 1;
		skip_red = 2;	
	}
	else if (str[f->i + 1] == '<')
		skip_red = 2;
	else
		skip_red = 1;
	f->temp = skip_spaces(&str[f->i + skip_red]);
	f->spaces_skipted += f->temp;
	f->i += f->temp + skip_red;
}

int	calculate_len(char *str, char **env, int *len, int status)
{
	t_format	f;

	init_format(&f, env);
	while (str[++f.i])
	{
		if ((str[f.i] == SQUOTE || str[f.i] == DQUOTE) && str[f.i + 1] && f.quotes == '\0')
		{
			f.quotes = str[f.i++];
			f.rem_quotes++;
		}
		if (str[f.i] == f.quotes && f.quotes != '\0')
			f.quotes = '\0';
		else if (str[f.i] == '<' || str[f.i] == '>')
			redirect_len(&f, str);
		else if (redirect_delimiter(str[f.i]) && f.heredoc)
			f.heredoc = 0;
		else if (valid_expand(str[f.i], f.quotes, str[f.i + 1]) && !f.heredoc)
		{
			parsing(&f, &str[f.i], status);
			if (f.err)
				return (f.err);
		}
		else if (str[f.i] == '\\' && str[f.i + 1] && f.quotes == '\0')
		{
			f.rem_slash++;
			f.i++;	
		}
	}
	//printf("Quotes: %i\nslash: %i\nvar: %i\nval: %i\n", f.rem_quotes, f.rem_slash, f.len_var, f.len_val);
	(*len) = f.i - (f.rem_quotes * 2) - f.rem_slash - f.len_var - f.spaces_skipted + (f.len_val);
	return (0);
}

int	handle_expansions(t_token *cmd_head, char **env, int status)
{
	t_token	*temp;
	int		new_len;
	char	*new_str;

	temp = cmd_head;
	new_len = 0;
	while (temp && temp->type != OPERATOR)
	{
		if (calculate_len(temp->str, env, &new_len, status))
			return (FORMAT_ERR);
		printf("len: %i\n", new_len);
		new_str = malloc(sizeof(char) * (new_len + 1));
		if (!new_str || get_expanded(temp->str, new_str, env, status))
			return (free(new_str), ERRNO_ERR);
		free(temp->str);
		temp->str = new_str;
		temp = temp->next;
	}
	return (0);
}

void	expand_commands(t_shell *shell)
{
	t_token	*temp;
	int		err;

	temp = shell->tokens;
	shell->env_var = get_env(shell->env);
	if (!shell->env_var)
		return ;
	while (temp)
	{
		err = handle_expansions(temp, shell->env_var, shell->exit_status);
		if (err)
		{
			print_err(err);
			free_shell(shell);
			exit(1);
		}
		while (temp && temp->type != OPERATOR)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
}
