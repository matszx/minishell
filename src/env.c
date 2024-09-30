/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:30 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/30 16:07:31 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Prints the environment
*/
int	put_env(t_env *env)
{
	env = env->next;
	while (env)
	{
		printf("%s=%s\n", env->var, env->value);
		env = env->next;
	}
	return (0);
}

/*
Deletes an environment variable

env: environment pointer
str: variable name
*/
int	del_env(t_env *env, char *str)
{
	t_env	*prev;

	prev = env;
	env = env->next;
	while (env && ft_strcmp(env->var, str))
	{
		prev = env;
		env = env->next;
	}
	if (env)
	{
		prev->next = env->next;
		free(env->var);
		free(env);
	}
	return (0);
}

/*
Sets a new environment variable or replaces an existing one

env: environment pointer
str: string formatted as "variable=value"
*/
int	add_env(t_env *env, char *str)
{
	t_env	*new;
	size_t	idx;

	if (!str || *str == '=')
		return (1);
	new = malloc(sizeof(*new));
	if (!new)
		return (1);
	idx = 0;
	while (str[idx] && str[idx] != '=')
		idx++;
	while (env && env->next && ft_strncmp(env->var, str, idx))
		env = env->next;
	new->next = NULL;
	if (!env->next)
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
Copies envp into a list of t_env
*/
t_env	*copy_env(char **envp)
{
	t_env	*env;

	env = malloc(sizeof(*env));
	if (!env)
		return (NULL);
	env->var = ft_substr("___INIT___", 0, 10);
	env->value = ft_substr("___INIT___", 0, 10);
	env->next = NULL;
	while (*envp)
		add_env(env, *(envp++));
	return (env);
}
