/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:21:10 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/02 13:38:49 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Checks if a space needs to be inserted after s[i]
static int	sep_check(char *s, int i)
{
	if ((s[i + 1] == '|' || s[i + 1] == '<' || s[i + 1] == '>')
		&& s[i] != s[i + 1])
		return (1);
	if (s[i] == '|' && s[i + 1] != ' ')
		return (1);
	if ((s[i] == '<' || s[i] == '>') && s[i] != s[i + 1] && s[i + 1] != ' ')
		return (1);
	if (i > 0)
	{
		if ((s[i] == '<' || s[i] == '>') && s[i] == s[i - 1])
			return (1);
	}
	return (0);
}

//	Returns the line with spaces around the separators
char	*space_line(char *s)
{
	char	*line;
	int		spaces;
	int		i;
	int		j;

	spaces = 0;
	i = 0;
	while (s[i] && s[i + 1])
		spaces += sep_check(s, i++);
	line = malloc(sizeof(char) * (ft_strlen(s) + spaces + 1));
	if (!line)
		return (printf("error: malloc fail, can't parse line\n"), NULL);
	i = 0;
	j = 0;
	while (s[i] && s[i + 1])
	{
		line[j++] = s[i];
		if (sep_check(s, i++))
			line[j++] = ' ';
	}
	line[j] = '\0';
	return (line);
}
