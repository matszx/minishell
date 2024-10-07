/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:02 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/07 14:33:51 by mcygan           ###   ########.fr       */
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
	else if (!ft_strncmp(head->str, "cd", 3))
		return (ft_cd(head->next));
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

char	*find_env(char **env, char* var)
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

int	handle_expansions(t_token *cmd_head, char **env)
{
	t_token	*temp;
	char	*quotes;

	temp = cmd_head;
	while (temp && temp->type != OPERATOR)
	{
		temp = temp->next;
	}
}

void	execute(t_shell *shell)
{
	t_token	*temp;

	temp = shell->tokens;
	shell->env_var = get_env(shell->env);
	if (!shell->env_var)
		return ;
	while (temp)
	{
		print_tokens(temp);
		handle_expansions(temp, shell->env_var);
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
				return (perror ("Minishell: "));	
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
