/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:29:05 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/28 00:15:01 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

size_t	get_env_len(t_env *env)
{
	size_t		len;
	t_env_node	*node;

	len = 0;
	node = env->head->next;
	while (node)
	{
		if (node->value)
			len++;
		node = node->next;
	}
	return (len);
}

char	**get_env(t_env *env)
{
	char		**env_var;
	int			i;
	t_env_node	*temp;

	env_var = malloc(sizeof(char *) * (get_env_len(env) + 1));
	if (!env_var)
		return (NULL);
	i = -1;
	temp = env->head->next;
	while (temp)
	{
		if (temp->value)
		{
			env_var[++i] = get_var(temp);
			if (!env_var[i])
			{
				while (--i > -1)
					free(env_var[i]);
				return (free(env_var), NULL);
			}
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
