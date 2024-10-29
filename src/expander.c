/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:48:56 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/29 14:59:22 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
			dst[e->j++] = t->str[++e->i];
		else if (!t->str[e->i])
			break ;
		else
			dst[e->j++] = t->str[e->i];
	}
	dst[e->j] = '\0';
	return (0);
}

int	handle_expansions(t_token *t, char **env, int status, int expand)
{
	int			new_len;
	int			err;
	char		*new_str;
	t_format	f;
	t_expand	e;

	new_len = 0;
	if (t->type != COMMAND && t->type != ARGUMENT && t->type != RED_ARG)
		return (0);
	init_format(&f, env, expand, status);
	err = calculate_len(t, &f, &new_len);
	if (err)
		return (err);
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
	return (0);
}
