/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/08 17:10:42 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Returns the appropriate prompt line
static char	*prompt_msg(int exit_status)
{
	if (!exit_status)
		return (PT "minishell" AG " > " RESET);
	else
		return (PT "minishell" VM " > " RESET);
}

void	print_tokens(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		printf("STR: %s\n", temp->str);
		printf("TYPE: %i\n", temp->type);
		printf("\n");
		temp = temp->next;
	}
}

void	print_err(int err)
{
	write(STDERR_FILENO, "Minishell: ", 12);
	if (err == QUOTES_ERR)
		write(STDERR_FILENO, "Unclosed quotes\n", 17);
	else if (err == SPLIT_ERR)
		write(STDERR_FILENO, "Error splitting command line\n", 30);
	else if (err == LIST_ERR)
		write(STDERR_FILENO, "Error initializing list\n", 25);
	else
		perror(NULL);
}

int	argument_manager(t_shell *shell, t_token *head)
{
	if (!ft_strncmp(head->str, "exit", 5))
		return (ft_exit(shell));
	else if (!ft_strncmp(head->str, "env", 4))
		return (ft_env(shell->env));
	else if (!ft_strncmp(head->str, "unset", 5))
		return (ft_unset(shell->env, head->next->str));
	else if (!ft_strncmp(head->str, "export", 6))
		return (ft_export(shell->env->head, head->next->str));
	else if (!ft_strncmp(head->str, "pwd", 4))
		return (ft_pwd());
	else if (!ft_strncmp(head->str, "echo", 5))
		return (ft_echo(head->next));
	else
		return (1);
}

char	*get_var(t_env_node *node)
{
	int		len_var;
	int		len_val;
	char	*str;

	len_var = ft_strlen(node->var);
	len_val = ft_strlen(node->value);
	str = malloc(sizeof(char) * (len_var + len_val + 3));
	if (!str)
		return (NULL);
	ft_strlcpy(str, node->var, len_var + 1);
	str[len_var] = '=';
	ft_strlcpy(&str[len_var + 1], node->value, len_val + 1);
	str[len_var + len_val + 2] = '\0';
	return (str);
}

char	**get_env(t_env *env)
{
	char		**env_var;
	int			i;
	t_env_node	*temp;

	env_var = malloc(sizeof(char *) * env->amount + 1);
	if (!env_var)
		return (NULL);
	i = -1;
	temp = env->head->next;
	while (temp)
	{
		env_var[++i] = get_var(temp);
		if (!env_var)
		{
			while (i-- > -1)
				free(env_var[i]);
			return (free(env_var), NULL);
		}
		temp = temp->next;
	}
	env_var[++i] = NULL;
	return (env_var);
}

char	*find_env(char **env, char *var)
{
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(var);
	while (env[++i])
	{
		if (!ft_strncmp(var, env[i], len) && env[i][len] == '=')
			return (&env[i][len + 1]);
	}
	return ("");
}

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
}

int	get_expanded(char *src, char *dst, char **env, int status)
{
	t_expand	e;

	init_expand(&e, env);
	while (src[++e.i])
	{
		if (src[e.i] && valid_expand(src[e.i], e.quotes, src[e.i + 1]))
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
		else if (valid_expand(str[f.i], f.quotes, str[f.i + 1]))
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
	printf("Quotes: %i\nslash: %i\nvar: %i\nval: %i\n", f.rem_quotes, f.rem_slash, f.len_var, f.len_val);
	(*len) = f.i - (f.rem_quotes * 2) - f.rem_slash - f.len_var + (f.len_val);
	return (0);
}

int	handle_expansions(t_token *cmd_head, char **env, int status)
{
	t_token	*temp;
	int		len;
	int		new_len;
	char	*new_str;

	temp = cmd_head;
	new_len = 0;
	while (temp && temp->type != OPERATOR)
	{
		len = ft_strlen(temp->str);
		if (calculate_len(temp->str, env, &new_len, status))
			return (FORMAT_ERR);
		new_str = malloc(sizeof(char) * (new_len + 1));
		if (!new_str || get_expanded(temp->str, new_str, env, status))
			return (free(new_str), ERRNO_ERR);
		free(temp->str);
		temp->str = new_str;
		temp = temp->next;
	}
	return (0);
}

void	print_env(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	printf("\n");
}

void	execute(t_shell *shell)
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
		print_tokens(temp);
		shell->exit_status = argument_manager(shell, temp);
		while (temp && temp->type != OPERATOR)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
	free_table((void **) shell->env_var);
	shell->env_var = NULL;
}

// Displays a prompt
void	prompt(t_shell *shell)
{
	int	err;

	while (1)
	{
		err = errno;
		shell->buf = readline(prompt_msg(shell->exit_status));
		if (!shell->buf)
		{
			if (err != errno)
				return (perror("Minishell: "));
			ft_exit(shell);
		}
		add_history(shell->buf);
		err = parse(shell->buf, &shell->tokens);
		free(shell->buf);
		if (err)
		{
			if (err != EMPTY_INPUT)
				print_err(err);
			continue ;
		}
		execute(shell);
		destroy_list(&shell->tokens);
	}
}
