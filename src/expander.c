/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:48:56 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/04 15:22:04 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*ft_getenv(t_env *env, char *s)
{
	env = env->next;
	while (env)
	{
		if (!ft_strcmp(env->var, s))
			return (env->value);
		env = env->next;
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
	value = ft_getenv(env, var);
	free(var);
	return (value);
}
