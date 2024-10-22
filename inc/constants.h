/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:52:15 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/22 04:17:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define PT		"\x1b[1;38;5;195m"	// pale turquoise
# define AG		"\x1b[1;38;5;191m"	// apple green
# define VM		"\x1b[1;38;5;203m"	// vermillion
# define RESET	"\x1b[0m"			// reset

# define SQUOTE			39
# define DQUOTE			34
# define LVL_LIMIT 		1000
# define PATH_MAX 		1024
# define FILE_CREAT 	0644.
# define DEFAULT_PATH	"/usr/local/sbin:/usr/local/bin:\
							/usr/sbin:/usr/bin:/sbin:/bin"

# define EMPTY_INPUT 		1
# define ERRNO_ERR 			2
# define QUOTES_ERR 		3
# define PIPE_END_ERR		4
# define PARENTHESIS_ERR 	5
# define FORMAT_ERR 		6
# define SYNTAX_ERR			7
# define ERRNO_PRINTED		8
# define ARGS_ERR			9
# define NAN_ERR			10
# define IDENTIFIER_ERR		11
# define CMD_NOT_FOUND		12

#endif