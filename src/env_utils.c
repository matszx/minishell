/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:42:13 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/09 19:47:14 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Checks if the environment identifier is valid
static int	is_valid_identifier(char *s)
{
	if (!ft_isalpha(*s) && *s != '_')
		return (0);
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

// Sets a new environment variable or replaces an existing one
int	addenv(t_env *stack, t_env_node *env, char *str)
{
	t_env_node	*new;
	size_t		idx;

	if (!ft_strchr(str, '=') || !is_valid_identifier(str))
		return (1);
	new = malloc(sizeof(t_env_node));
	if (!new)
		return (1);
	idx = 0;
	while (str[idx] && str[idx] != '=')
		idx++;
	while (env && env->next && ft_strncmp(env->var, str, idx))
		env = env->next;
	new->next = NULL;
	if (!env->next && ft_strncmp(env->var, str, idx))
		env->next = new;
	else
	{
		free(new);
		new = env;
		free(new->var);
		free(new->value);
	}
	new->value = ft_substr(str, idx + (str[idx] == '='), ft_strlen(str) - idx);
	return (stack->amount++, new->var = ft_substr(str, 0, idx), 0);
}

void	destroy_env(t_env **env)
{
	t_env_node	*temp;

	if (*env)
	{
		while ((*env)->head)
		{
			temp = (*env)->head->next;
			free((*env)->head->var);
			free((*env)->head->value);
			free((*env)->head);
			(*env)->head = temp;
		}
		free(*env);
		*env = NULL;
	}
}

int	set_level(t_env_node *head)
{
	t_env_node	*tmp;
	int			level;
	char		*shlvl;

	tmp = head;
	shlvl = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->var, "SHLVL"))
		{
			level = ft_atoi(tmp->value) + 1;
			if (level < 0)
				level = 0;
			else if (level > LVL_LIMIT)
				level = 1;
			shlvl = ft_itoa(level);
			free(tmp->value);
			tmp->value = shlvl;
			if (!tmp->value)
				return (1);
			break ;
		}
		tmp = tmp->next;
	}
	return (0);
}

// Copies envp into a list of t_env
t_env	*copy_env(char **envp)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->head = malloc(sizeof(t_env_node));
	if (!env->head)
		return (free(env), NULL);
	env->head->var = ft_strdup("");
	env->head->value = ft_strdup("");
	env->head->next = NULL;
	env->amount = 0;
	while (*envp)
		addenv(env, env->head, *(envp++));
	if (set_level(env->head))
		destroy_env(&env);
	return (env);
}
