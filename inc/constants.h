/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:52:15 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/24 18:55:05 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define PT		"\x1b[1;38;5;195m"	// pale turquoise
# define AG		"\x1b[1;38;5;191m"	// apple green
# define VM		"\x1b[1;38;5;203m"	// vermillion
# define RESET	"\x1b[0m"			// reset

# define SQUOTE				39
# define DQUOTE				34
# define LVL_LIMIT 			1000
# define PATH_MAX 			1024
# define FILE_CREAT 		0644
# define DEFAULT_PATH		"/usr/local/sbin:/usr/local/bin:\
								/usr/sbin:/usr/bin:/sbin:/bin"

# define EMPTY_INPUT 		1
# define ERRNO_ERR 			2
# define QUOTES_ERR 		3
# define PIPE_END_ERR		4
# define PARENTHESIS_ERR 	5
# define SYNTAX_ERR			6
# define ERRNO_PRINTED		7
# define ARGS_ERR			8
# define NAN_ERR			9
# define IDENTIFIER_ERR		10
# define CMD_NOT_FOUND		11

# define MINISHELL			"minishell"
# define PIPE_END_MSG		"unclosed pipes are not supported\n"
# define QUOTES_MSG			"unclosed quotes\n"
# define SYNTAX_MSG			"syntax error near unexpected token\n"
# define PARENTHESIS_MSG	"parenthesis are not supported\n"
# define ARGS_MSG			"arguments are not accepted\n"

#endif