/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/24 12:42:17 by mcygan           ###   ########.fr       */
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

# define PT		"\x1b[38;5;195m"	// pale turquoise
# define AG		"\x1b[38;5;191m"	// apple green
# define VM		"\x1b[38;5;1m"		// vermillion
# define RESET	"\x1b[0m"			// reset

typedef struct s_token
{
	int				type;
	char			*str;
	char			*backup;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	t_token			*token_array;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	char	**env;
	char	*buf;
	t_cmd	*cmd_array;
	int		exit_status;
}	t_shell;

// prompt.c
void		prompt(t_shell *shell);

// sig_handler.c
void		init_signals(void);

#endif
