/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:30 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/10 14:35:10 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Loops addenv for every argument given
int	ft_export(t_env *stack, t_env_node *env, t_token *token)
{
	int		ret;

	ret = 0;
	while (token && token->type == ARGUMENT)
	{
		if (addenv(stack, env, token->str))
		{
			printf(
				"minishell: export: '%s': Not a valid identifier\n",
				token->str
				);
			ret = 1;	
		}
		token = token->next;
	}
	return (ret);
}

// Deletes an environment variable
int	ft_unset(t_env *env, t_token *token)
{
	t_env_node	*prev;
	t_env_node	*temp;

	while (token && token->type == ARGUMENT)
	{
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
		token = token->next;
	}
	return (0);
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

// Prints the environment
int	ft_env(t_env *env, t_token *token)
{
	t_env_node	*temp;

	if (token && token->type == ARGUMENT)
		return (printf("minishell: env: Too many arguments\n"), 1);
	temp = env->head->next;
	while (temp)
	{
		printf("%s=%s\n", temp->var, temp->value);
		temp = temp->next;
	}
	return (0);
}
