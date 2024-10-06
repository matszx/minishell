/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/06 21:44:15 by dzapata          ###   ########.fr       */
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

typedef struct s_shell
{
	char	*buf;
	t_token	*tokens;
	t_env	*env;
	char	**env_var;
	int		exit_status;
}	t_shell;

// prompt.c
void		prompt(t_shell *shell);

// expander.c
char		*expander(t_env *env, char *s);

// lexer.c
char		*space_line(char *s);

// parser.c
int			parse(char *buf, t_token **head);
void		destroy_list(t_token **head);

// env.c
void		destroy_env(t_env **env);
int			ft_env(t_env *env);
int			ft_export(t_env_node *env, char *str);
int			ft_unset(t_env *env, char *str);
int			ft_exit(t_shell *shell);
int			ft_pwd();
int			ft_echo(t_token *token);

t_env		*copy_env(char **envp);
void		free_shell(t_shell *shell);
void		free_table(void **table);

// signals.c
void		init_signals(void);

#endif
