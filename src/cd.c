/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:29:13 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/07 19:47:38 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	update_oldpwd(t_shell *shell, char *new)
{
	char		*temp;
	t_env_node	*oldpwd;

	oldpwd = shell->env->head->next;
	while (oldpwd && ft_strcmp(oldpwd->var, "OLDPWD"))
		oldpwd = oldpwd->next;
	if (oldpwd)
	{
		free(oldpwd->value);
		oldpwd->value = NULL;
	}
	if (shell->cwd[0])
	{
		temp = ft_strjoin("OLDPWD=", new);
		if (!temp || addenv(shell->env, shell->env->head, temp))
			return (free(temp), print_err(ERRNO_ERR), 1);
		free(temp);
	}
	return (0);
}

int	update_pwd(t_shell *shell, char *old)
{
	char		*temp;
	t_env_node	*oldpwd;

	oldpwd = shell->env->head->next;
	while (oldpwd && ft_strcmp(oldpwd->var, "PWD"))
		oldpwd = oldpwd->next;
	if (oldpwd)
	{
		free(oldpwd->value);
		oldpwd->value = NULL;
	}
	if (shell->cwd[0])
	{
		temp = ft_strjoin("PWD=", old);
		if (!temp || addenv(shell->env, shell->env->head, temp))
			return (free(temp), print_err(ERRNO_ERR), 1);
		free(temp);
	}
	return (0);
}

int	manage_cd_arg(t_shell *shell, t_token *arg)
{
	char	*old;
	int		ret;

	if (arg->str[1])
		return (print_arg_err("cd", NULL, CD_FLAGS_ERR, 0), 2);
	old = find_env(shell->env_var, "OLDPWD");
	if (!old[0])
		return (print_arg_err("cd", NULL, OLDPWD_ERR, 0), 1);
	if (chdir(old) == -1)
		return (print_arg_err("cd", NULL, ERRNO_ERR, 0), 1);
	ret = update_oldpwd(shell, shell->cwd);
	if (!getcwd(shell->cwd, PATH_MAX))
		return (print_arg_err("cd", "", ERRNO_ERR, 0), 1);
	ret = ret || update_pwd(shell, shell->cwd);
	if (write(STDOUT_FILENO, old, ft_strlen(old)) == -1
		|| write(STDOUT_FILENO, "\n", 1) == -1)
	{
		print_arg_err("cd", "write error", ERRNO_ERR, 0);
		ret = 1;
	}
	return (ret);
}

int	home(t_shell *shell)
{
	char	*tmp;

	tmp = find_env(shell->env_var, "HOME");
	if (!tmp[0])
		return (print_arg_err("cd", NULL, HOME_ERR, 0), 1);
	if (chdir(tmp) == -1)
		return (print_arg_err("cd", tmp, ERRNO_ERR, 0), 1);
	if (!getcwd(shell->cwd, PATH_MAX))
		return (print_arg_err("cd", "", ERRNO_ERR, 0), 1);
	return (0);
}

int	ft_cd(t_shell *shell, t_token *token)
{
	char	*tmp;
	int		ret;
	t_token	*arg;

	arg = get_cmd_token(token, ARGUMENT);
	if (arg && get_cmd_token(arg->next, ARGUMENT))
		return (print_arg_err("cd", NULL, ARGS_ERR, 0), 1);
	if (!arg)
		return (home(shell));
	else if (arg->str[0] == '-')
		return (manage_cd_arg(shell, arg));
	else
		tmp = arg->str;
	ret = chdir(tmp);
	if (ret == -1)
		return (print_arg_err("cd", tmp, ERRNO_ERR, 0), 1);
	ret = update_oldpwd(shell, shell->cwd);
	if (!getcwd(shell->cwd, PATH_MAX))
		return (print_arg_err("cd", "", ERRNO_ERR, 0), 1);
	return (ret || update_pwd(shell, shell->cwd));
}
