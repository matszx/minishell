/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:52:15 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/07 13:50:36 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define PT		"\x1b[1;38;5;195m"	// pale turquoise
# define AG		"\x1b[1;38;5;191m"	// apple green
# define VM		"\x1b[1;38;5;203m"	// vermillion
# define RESET	"\x1b[0m"			// reset

# define SQUOTE	39
# define DQUOTE	34
# define LVL_LIMIT 1000

# define EMPTY_INPUT 1
# define ERRNO_ERR 2
# define QUOTES_ERR 3
# define SPLIT_ERR 4
# define LIST_ERR 5
# define PARENTHESIS_ERR 6
# define FORMAT_ERR 7

#endif