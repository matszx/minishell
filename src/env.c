/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:30 by mcygan            #+#    #+#             */
/*   Updated: 2024/11/07 16:31:27 by dzapata          ###   ########.fr       */
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

int	print_var(t_env *stack)
{
	t_env_node	*temp;
	int			err;

	temp = stack->head->next;
	while (temp)
	{
		if (temp->value)
			err = write(STDOUT_FILENO, "declare -x ", 12) == -1
				|| write(STDOUT_FILENO, temp->var, ft_strlen(temp->var)) == -1
				|| write(STDOUT_FILENO, "=\"", 2) == -1
				|| write(STDOUT_FILENO, temp->value, ft_strlen(temp->value)) \
				== -1
				|| write(STDOUT_FILENO, "\"\n", 2) == -1;
		else
			err = write(STDOUT_FILENO, "declare -x ", 12) == -1
				|| write(STDOUT_FILENO, temp->var, ft_strlen(temp->var)) == -1
				|| write(STDOUT_FILENO, "\n", 1) == -1;
		if (err)
			return (print_arg_err("export", "write error", ERRNO_ERR, 0), 1);
		temp = temp->next;
	}
	return (0);
}

// Loops addenv for every argument given
int	ft_export(t_env *stack, t_env_node *env, t_token *token)
{
	int		ret;
	t_token	*arg;

	ret = 0;
	arg = get_cmd_token(token, ARGUMENT);
	if (!arg)
		return (print_var(stack));
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
	int	ret;

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
	t_token		*arg;

	arg = get_cmd_token(token, ARGUMENT);
	if (arg)
		return (print_arg_err("env", NULL, ARGS_ERR, 0), 1);
	temp = env->head->next;
	while (temp)
	{
		if (temp->value)
		{
			if (write(STDOUT_FILENO, temp->var, ft_strlen(temp->var)) == -1
				|| write(STDOUT_FILENO, "=", 1) == -1
				|| write(STDOUT_FILENO, temp->value, ft_strlen(temp->value)) \
				== -1
				|| write(STDOUT_FILENO, "\n", 1) == -1)
				return (print_arg_err("env", "write error", ERRNO_ERR, 0), 1);
		}
		temp = temp->next;
	}
	return (0);
}
