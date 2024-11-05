/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:07:50 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/05 17:45:02 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**get_args(t_token *t)
{
	t_token	*temp;
	int		len;
	char	**str;

	temp = t;
	len = 0;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type == ARGUMENT)
			len++;
		temp = temp->next;
	}
	str = malloc(sizeof(char *) * (len + 2));
	if (!str)
		return (NULL);
	temp = t;
	len = 1;
	while (temp && temp->type != OPERATOR)
	{
		if (temp->type == ARGUMENT)
			str[len++] = temp->str;
		temp = temp->next;
	}
	str[len] = NULL;
	return (str[0] = t->str, str);
}

char	*get_cmd_path(int *code, char **paths, char *str)
{
	char	*temp;
	char	*c_path;
	int		i;
	int		file;

	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (*code = 1, NULL);
		c_path = ft_strjoin(temp, str);
		free(temp);
		if (!c_path)
			return (*code = 1, NULL);
		file = path_access(c_path);
		if (!file)
			return (c_path);
		free(c_path);
		if (file == 1)
			return (NULL);
	}
	return ((*code) = CMD_NOT_FOUND, NULL);
}

char	*find_command(t_shell *shell, t_token *cmd, int *code)
{
	char	**paths;
	char	*c_path;
	int		file;

	if (!cmd->str[0])
		return ((*code) = CMD_NOT_FOUND, NULL);
	else if (!ft_strncmp("./", cmd->str, 2) || !ft_strncmp("/", cmd->str, 1))
	{
		file = path_access(cmd->str);
		if (!file)
			return (*code = 0, cmd->str);
		else if (file == 1)
			return (*code = 126, NULL);
		return (*code = CMD_NOT_FOUND, NULL);
	}
	c_path = find_env(shell->env_var, "PATH");
	if (!c_path)
		return ((*code) = CMD_NOT_FOUND, NULL);
	paths = ft_split(c_path, ':');
	if (!paths)
		return (*code = 1, NULL);
	c_path = get_cmd_path(code, paths, cmd->str);
	free_table((void **) paths);
	if (!c_path)
		return (NULL);
	return (c_path);
}

void	find_error(char *str, int *code)
{
	if (*code == 126 || *code == 127)
		return (print_arg_err(str, NULL, ERRNO_ERR, 0));
	else if (*code == CMD_NOT_FOUND)
		return (*code = 127, print_arg_err(str, NULL, CMD_NOT_FOUND, 0));
	return (print_err(ERRNO_ERR));
}
