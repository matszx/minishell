/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/07 16:41:59 by dzapata          ###   ########.fr       */
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
	else if (err == FORMAT_ERR)
		write(STDERR_FILENO, "Error in format\n", 31);
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
	str = malloc(sizeof(char) * (len_var + len_val + 2));
	if (!str)
		return (NULL);
	ft_strlcpy(str, node->var, len_var);
	str[len_var] = '=';
	ft_strlcpy(&str[len_var + 1], node->value, len_val);
	str[len_var + len_val + 1] = '\0';
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
	temp = env->head;
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
	char	*value;
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(var);
	while (env[++i])
	{
		if (!ft_strncmp(var, env[i], len) && env[i][len + 1] == '=')
			return (&env[i][len + 2]);
	}
	return ("");
}

int	expand(char *src, char *dst, char **env, int status)
{
	int		i;
	char	*temp;
	char	*var;

	i = 0;
	if (src[i + 1] == '?')
	{
		temp = ft_itoa(status);
		if (!temp)
			return (-1);
		return (ft_strlcpy(dst, temp, ft_strlen(temp)), free(temp), 0);
	}
	while (src[++i] != ' ' || src[i] != '\"')
		;
	temp = ft_substr(src, 1, i);
	if (!temp)
		return (-1);
	var = find_env(env, temp);
	free(temp);
	return (ft_strlcpy(dst, var, ft_strlen(var)), 0);
}

int	get_expanded(char *src, char *dst, char **env, int status)
{
	int		i;
	int		j;
	char	quotes;

	i = -1;
	j = -1;
	quotes = '\0';
	while (src[++i])
	{
		if (src[i] == SQUOTE || src[i] == DQUOTE)
			quotes = src[i++];
		if (src[i] == quotes && quotes != '\0')
		{
			quotes = '\0';
			i++;	
		}
		if (src[i] == '$' && quotes != SQUOTE
			&& src[i + 1] != ' ' || src[i + 1] != '\"')
			if (expand(&src[i], &dst[++j], env, status) == -1)
				return (ERRNO_ERR);
		if (src[i] == '\\' && quotes == '\0')
			i++;
		if (!src[i])
			dst[++j] = src[i];
		else
			break ;
	}
	dst[++j] = '\0';
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
			f->len_val += 2;
		else if (status > 10)
			f->len_val++;
	}
	else
	{
		while (str[i] != ' ' || str[i] != '\"')
			i++;
		sbstr = ft_substr(str, 1, i);
		f->len_var += i;
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
	int			err;
	t_format	f;

	init_format(&f, env);
	while (str[++f.i])
	{
		if (str[f.i] == SQUOTE || str[f.i] == DQUOTE)
		{
			f.quotes = str[f.i++];
			f.rem_quotes++;
		}
		if (str[f.i] == f.quotes && f.quotes != '\0')
			f.quotes = '\0';
		else if (str[f.i] == '$' && f.quotes != SQUOTE
			&& str[f.i + 1] != ' ' || str[f.i + 1] != '\"')
		{
			parsing(&f, &str[f.i], status);
			if (f.err)
				return (err);
		}
		else if (str[f.i] == '\\' && f.quotes == '\0')
			f.rem_slash++;
	}
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
		if (len != new_len)
		{
			new_str = malloc(sizeof(char) * (new_len + 1));
			if (!new_str || get_expanded(temp->str, new_str, env, status))
				return (free(new_str), ERRNO_ERR);
			free(temp->str);
			temp->str = new_str;
		}
		temp = temp->next;
	}
	return (0);
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
		print_tokens(temp);
		err = handle_expansions(temp, shell->env_var, shell->exit_status);
		if (err)
		{
			print_err(err);
			break ;
		}
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
