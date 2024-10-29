/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:42:13 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/29 14:16:40 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	addenv2(t_env_node *env, char *str, size_t idx, char *equal)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (1);
	new->value = NULL;
	new->next = NULL;
	if ((equal[0]))
		new->value = ft_substr(str, idx + 1, ft_strlen(str) - idx);
	new->var = ft_substr(str, 0, idx);
	if (!new->var || (!new->value && equal[0]))
		return (free(new->value), free(new->var), free(new), 1);
	while (env->next)
		env = env->next;
	env->next = new;
	return (0);
}

// Sets a new environment variable or replaces an existing one
int	addenv(t_env *stack, t_env_node *env, char *str)
{
	size_t		idx;
	char		*equal;

	equal = ft_strchr(str, '=');
	if (!equal)
		equal = &str[ft_strlen(str)];
	idx = equal - str;
	while (env && env->next && ft_strncmp(env->var, str, idx))
		env = env->next;
	if (!ft_strncmp(env->var, str, idx) && !str[idx])
		return (0);
	else if (!ft_strncmp(env->var, str, idx) && !env->var[idx])
	{
		equal = env->value;
		env->value = ft_substr(str, idx + 1, ft_strlen(str) - idx);
		if (!env->value)
			return (env->value = equal, print_err(ERRNO_ERR), 1);
		free(equal);
		return (0);
	}
	if (addenv2(env, str, idx, equal))
		return (1);
	return (stack->amount++, 0);
}

void	remove_env(t_env *env, t_token *token)
{
	t_env_node	*prev;
	t_env_node	*temp;

	prev = env->head;
	temp = env->head->next;
	while (temp && ft_strcmp(temp->var, token->str))
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp)
	{
		prev->next = temp->next;
		free(temp->var);
		free(temp->value);
		free(temp);
		env->amount--;
	}
}

void	destroy_env(t_env **env)
{
	t_env_node	*temp;
	t_env_node	*next;

	if (*env)
	{
		temp = (*env)->head->next;
		while (temp)
		{
			next = temp->next;
			free(temp->var);
			free(temp->value);
			free(temp);
			temp = next;
		}
		free((*env)->head);
		free(*env);
		*env = NULL;
	}
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
	env->head->var = "";
	env->head->value = "";
	env->head->next = NULL;
	env->amount = 0;
	while (*envp)
		if (addenv(env, env->head, *(envp++)))
			return (destroy_env(&env), NULL);
	if (set_level(env->head))
		destroy_env(&env);
	return (env);
}
