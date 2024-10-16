/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/16 13:41:32 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/inc/libft.h"
# include "constants.h"

typedef enum e_type
{
	COMMAND,
	ARGUMENT,
	OPERATOR,
	REDIRECT,
}	t_type;

typedef struct s_token
{
	char			*str;
	t_type			type;
	struct s_token	*next;
}	t_token;

typedef struct s_env_node
{
	char				*var;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

typedef struct s_env
{
	int			amount;
	t_env_node	*head;
}	t_env;

typedef struct s_format
{
	int		rem_quotes;
	int		rem_slash;
	int		len_var;
	int		len_val;
	int		i;
	int		err;
	int		heredoc;
	int		spaces_skipted;
	int		temp;
	char	quotes;
	char	**env;
}	t_format;

typedef struct s_expand
{
	int		i;
	int		j;
	int		heredoc;
	char	quotes;
	char	**env;
}	t_expand;

typedef struct s_shell
{
	char	*buf;
	t_token	*tokens;
	t_env	*env;
	char	**env_var;
	int		exit_status;
	int		*fd;
	int		n_commands;
}	t_shell;

// main.c
void		free_table(void **table);
void		free_shell(t_shell *shell);
void		close_files(int *fd, int n);

// prompt.c
void		print_err(int err);
char		*find_env(char **env, char *var);
void		prompt(t_shell *shell);

// expander.c
void		expand_commands(t_shell *shell);

// redirect.c
int			redirect_delimiter(char c);
int			redirect(t_shell *shell);

// parser.c
int			skip_spaces(char *str);
int			parse(char *buf, t_shell *shell);
void		destroy_list(t_token **head);

// env.c
char		**get_env(t_env *env);
int			ft_export(t_env *stack, t_env_node *env, t_token *token);
int			ft_unset(t_env *env, t_token *token);
int			ft_env(t_env *env, t_token *token);

// env_utils.c
int			addenv(t_env *stack, t_env_node *env, char *str);
void		destroy_env(t_env **env);
t_env		*copy_env(char **envp);

// builtins.c
int			ft_echo(t_token *token);
int			ft_cd(char **env, t_token *token);
int			ft_pwd(void);
int			ft_exit(unsigned int ret, t_shell *shell, t_token *token);

// signals.c
void		init_signals(void);

//execute.c
int			execute(t_shell *shell);

#endif
