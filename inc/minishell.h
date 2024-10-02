/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/02 13:42:51 by mcygan           ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/inc/libft.h"

# define PT		"\x1b[1;38;5;195m"	// pale turquoise
# define AG		"\x1b[1;38;5;191m"	// apple green
# define VM		"\x1b[1;38;5;203m"	// vermillion
# define RESET	"\x1b[0m"			// reset

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			*str;
	t_token			*token_array;
	int				flags;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	char	*buf;
	char	*cmd_line;
	t_cmd	*cmd_array;
	t_env	*env;
	int		exit_status;
}	t_shell;

// prompt.c
void		prompt(t_shell *shell);

// lexer.c
char		*space_line(char *s);

// env.c
int			ft_env(t_env *env);
int			ft_export(t_env *env, char *str);
int			ft_unset(t_env *env, char *str);
t_env		*copy_env(char **envp);

// signals.c
void		init_signals(void);

#endif
