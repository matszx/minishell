/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/09 16:52:06 by dzapata          ###   ########.fr       */
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
	struct s_token	*prev;
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
	char	quotes;
	char	**env;
}	t_format;

typedef struct s_expand
{
	int		i;
	int		j;
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
	int		fd_in;
	int		fd_out;
}	t_shell;

// prompt.c
char		*find_env(char **env, char *var);
void		prompt(t_shell *shell);

// expander.c
char		*expander(t_env *env, char *s);

// lexer.c
char		*space_line(char *s);

// parser.c
int			parse(char *buf, t_token **head);
void		destroy_list(t_token **head);

// env.c
int			addenv(t_env_node *env, char *str);
t_env		*copy_env(char **envp);
void		destroy_env(t_env **env);
void		free_table(void **table);
void		free_shell(t_shell *shell);

// builtins1.c
int			ft_echo(t_token *token);
int			ft_cd(char **env, t_token *token);
int			ft_pwd();
int			ft_exit(t_shell *shell);

// env_utils.c

int			ft_export(t_env_node *env, t_token *token);
int			ft_unset(t_env *env, t_token *token);
int			ft_env(t_env *env);

// signals.c
void		init_signals(void);

#endif
