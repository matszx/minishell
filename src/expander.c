/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:48:56 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/05 23:37:10 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*ft_getenv(t_env_node *env, char *s)
{
	t_env_node	*temp;

	temp = env->next;
	while (temp)
	{
		if (!ft_strcmp(temp->var, s))
			return (temp->value);
		temp = temp->next;
	}
	return ("\0");
}

char	*expander(t_env *env, char *s)
{
	int		i;
	char	*var;
	char	*value;

	i = 0;
	while (ft_isalnum(s[i]) || s[i] == '_')
		i++;
	var = ft_substr(s, 0, i);
	value = ft_getenv(env->head, var);
	free(var);
	return (value);
}
