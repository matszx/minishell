/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/04 16:13:07 by dzapata          ###   ########.fr       */
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

# define SQUOTE	39
# define DQUOTE	34

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
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	char	*buf;
	t_token	*tokens;
	t_env	*env;
	int		exit_status;
}	t_shell;

// prompt.c
void		prompt(t_shell *shell);

// expander.c
char		*expander(t_env *env, char *s);

// lexer.c
char		*space_line(char *s);

// parser.c
char		*parse(t_env *env, char *s);

// env.c
int			ft_env(t_env *env);
int			ft_export(t_env *env, char *str);
int			ft_unset(t_env *env, char *str);
t_env		*copy_env(char **envp);

// signals.c
void		init_signals(void);

#endif
