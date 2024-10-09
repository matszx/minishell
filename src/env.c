/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:30 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/09 19:49:33 by mcygan           ###   ########.fr       */
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
