/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:52:15 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/26 17:18:54 by dzapata          ###   ########.fr       */
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
# define HOME_ERR			12

# define MINISHELL			"minishell"
# define PIPE_END_MSG		"unclosed pipes are not supported"
# define QUOTES_MSG			"unclosed quotes"
# define SYNTAX_MSG			"syntax error near unexpected token"
# define PARENTHESIS_MSG	"parenthesis are not supported"
# define NO_ARGS_MSG		"arguments are not accepted"
# define ARGS_MSG			"too many arguments"
# define NAN_MSG			"numeric argument required"
# define IDENTIFIER_MSG		"not a valid identifier"
# define NO_CMD_MSG			"command not found"
# define HOME_MSG			"HOME not set"
# define UNKNOW_MSG			"unknow error"

#endif