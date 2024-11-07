/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:14:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/11/07 18:53:40 by dzapata          ###   ########.fr       */
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
# include <sys/wait.h>
# include "../libft/inc/libft.h"
# include "constants.h"

extern int	g_exitstatus;

typedef enum e_type
{
	COMMAND,
	ARGUMENT,
	OPERATOR,
	HEREDOC,
	RED_IN,
	RED_OUT,
	RED_APP,
	RED_ARG,
}	t_type;

typedef struct s_err_msg
{
	char	*str;
	char	*str_err;
	char	*cmd;
	char	*arg;
	int		cmd_len;
	int		arg_len;
	int		quotes;
}	t_err_msg;

typedef struct s_token
{
	char			*str;
	char			*old_str;
	int				len;
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
	int		expand;
	int		status;
	int		spaces_skipped;
	int		temp;
	int		red_limit;
	char	quotes;
	char	**env;
}	t_format;

typedef struct s_expand
{
	int		i;
	int		j;
	int		expand;
	int		status;
	char	quotes;
	char	**env;
}	t_expand;

typedef struct s_shell
{
	char	*buf;
	t_token	*tokens;
	t_env	*env;
	char	**env_var;
	int		*fd;
	int		n_commands;
	char	cwd[PATH_MAX];
}	t_shell;

// main.c
void		free_table(void **table);
void		free_shell(t_shell *shell);
void		close_files(int *fd, int n);

// prompt.c
char		*find_env(char **env, char *var);
void		prompt(t_shell *shell);

// expander.c
int			handle_expansions(t_token *t, char **env, int status, int expand);
int			expand_commands(t_shell *shell);

// expander_len.c
int			valid_name(char c);
int			valid_expand(char c, char quotes, char next);
int			calculate_len(t_token *t, t_format *f, int *len);
void		init_format(t_format *f, char **env, int expand, int status);

// heredoc.c
int			red_heredoc(t_shell *shell);

// redirect.c
int			get_pipes(t_shell *shell);
int			redirect(t_token *token, int *fd, int cmd);

// parser.c
int			skip_spaces(char *str);
int			parse(char *buf, t_shell *shell);
void		destroy_list(t_token **head);

// parser_utils.c
int			skip_spaces(char *str);
int			is_special_token(char c);
int			is_operator(char c);
int			verify_redirect(char *str, int *i);

// parse_strings.c
int			quotes_closed(char *s);
char		*get_token(char *str, int *i);
int			check_string(char *str);

// classifier.c
void		classify_count(t_token *head, int *n);

// env.c
int			print_var(t_env *stack);
int			ft_export(t_env *stack, t_env_node *env, t_token *token);
int			ft_unset(t_env *env, t_token *token);
int			ft_env(t_env *env, t_token *token);

// env_utils.c
int			addenv(t_env *stack, t_env_node *env, char *str);
void		remove_env(t_env *env, t_token *token);
void		destroy_env(t_env **env);
t_env		*copy_env(char **envp);

// env_string.c
char		**get_env(t_env *env);
char		*find_env(char **env, char *var);
int			set_level(t_env_node *head);

// builtins.c
int			ft_echo(t_token *token);
int			ft_pwd(char *cwd);
int			ft_exit(unsigned int ret, t_shell *shell, t_token *token);

// cd.c
int			ft_cd(t_shell *shell, t_token *token);

// builtins_utils.c
t_token		*get_cmd_token(t_token *t, t_type type);
int			has_echo_flag(char *s);
int			write_rest(t_token *token, char *temp, int flag);

// signals.c
void		sigint_handler(int sig);
void		init_signals(void);

// execute.c
void		execute(t_shell *shell);

// execute_waits.c
void		wait_processes(pid_t *pid, t_shell *shell);

// execute_utils.c
int			is_builtin(char *str);
int			affects_environment(char *str);
void		jump_to_next(t_token **t);
int			path_access(char *str);
int			perform_redirections(t_token *t, int *fd, int n);

// command.c
char		**get_args(t_token *t);
char		*get_cmd_path(int *code, char **paths, char *str);
char		*find_command(t_shell *shell, t_token *cmd, int *code);
void		find_error(char *str, int *code);

// errors.c
void		print_err(int err);
void		print_arg_err(char *cmd, char *arg, int err, int quotes);

//utilc.c
int			ft_isnumber(char *str);
int			has_quotes(char *str);
long		ft_atol(const char *nptr, int *overflow);

#endif
