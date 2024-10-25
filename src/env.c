/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:30 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/25 15:50:22 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Checks if the environment identifier is valid
static int	is_valid_identifier(char *s)
{
	if (!(*s) || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

void	print_var(t_env *stack)
{
	t_env_node	*temp;

	temp = stack->head->next;
	while (temp)
	{
		if (temp->value)
			printf("declare -x %s=\"%s\"\n", temp->var, temp->value);
		else
			printf("declare -x %s\n", temp->var);
		temp = temp->next;
	}
}

// Loops addenv for every argument given
int	ft_export(t_env *stack, t_env_node *env, t_token *token)
{
	int		ret;
	t_token	*arg;

	ret = 0;
	arg = get_cmd_token(token, ARGUMENT);
	if (!arg)
		return (print_var(stack), 0);
	while (arg)
	{
		if (is_valid_identifier(arg->str))
		{
			if (addenv(stack, env, arg->str))
				ret = 1;
		}
		else
			print_arg_err("export", arg->str, IDENTIFIER_ERR, 1);
		arg = get_cmd_token(arg->next, ARGUMENT);
	}
	return (ret);
}

// Deletes an environment variable
int	ft_unset(t_env *env, t_token *token)
{
	int			ret;

	token = get_cmd_token(token, ARGUMENT);
	ret = 0;
	while (token)
	{
		if (is_valid_identifier(token->str))
			remove_env(env, token);
		else
		{
			ret = 1;
			print_arg_err("unset", token->str, IDENTIFIER_ERR, 1);
		}
		token = get_cmd_token(token->next, ARGUMENT);
	}
	return (ret);
}

// Prints the environment
int	ft_env(t_env *env, t_token *token)
{
	t_env_node	*temp;

	if (token && token->type == ARGUMENT)
		return (print_arg_err("env", NULL, ARGS_ERR, 0), 1);
	temp = env->head->next;
	while (temp)
	{
		if (temp->value)
			printf("%s=%s\n", temp->var, temp->value);
		temp = temp->next;
	}
	return (0);
}
