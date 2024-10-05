/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:30 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/05 23:54:03 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Sets a new environment variable or replaces an existing one

env: environment pointer
str: string formatted as "variable=value"
*/
int	ft_export(t_env_node *env, char *str)
{
	t_env_node	*new;
	size_t		idx;

	if (!str || (!ft_isalpha(*str) && *str != '_') || !ft_strchr(str, '='))
		return (printf("error: invalid format\n"), 1);
	new = malloc(sizeof(t_env_node));
	if (!new)
		return (printf("error: malloc fail for %s\n", str), 1);
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
	return (new->var = ft_substr(str, 0, idx), 0);
}

/*
Deletes an environment variable

env: environment pointer
str: variable name
*/
int	ft_unset(t_env *env, char *str)
{
	t_env_node	*prev;
	t_env_node	*temp;

	prev = env->head;
	temp = env->head->next;
	while (temp && ft_strcmp(temp->var, str))
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
	return (0);
}

/*
Prints the environment
*/
int	ft_env(t_env *env)
{
	t_env_node	*temp;

	temp = env->head->next;
	while (temp)
	{
		printf("%s=%s\n", temp->var, temp->value);
		temp = temp->next;
	}
	return (0);
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

/*
Copies envp into a list of t_env
*/
t_env	*copy_env(char **envp)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->head = malloc(sizeof(t_env_node));
	if (!env->head)
		return (free(env), NULL);
	env->head->var = ft_substr("___INIT___", 0, 10);
	env->head->value = ft_substr("___INIT___", 0, 10);
	env->head->next = NULL;
	if (!env->head->var || !env->head->value)
		return (destroy_env(&env), NULL);
	env->amount = 0;
	while (*envp)
	{
		ft_export(env->head, *(envp++));
		env->amount++;
	}
	if (set_level(env->head))
		destroy_env(&env);
	return (env);
}
