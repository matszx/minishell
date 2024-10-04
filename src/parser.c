/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:20:49 by mcygan            #+#    #+#             */
/*   Updated: 2024/10/04 15:59:00 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	quotes_closed(char *s)
{
	char	status;

	status = 0;
	while (*s)
	{
		if ((*s == SQUOTE || *s == DQUOTE) && !status)
			status = *s;
		else if (*s == status)
			status = 0;
		s++;
	}
	return (!status);
}

char	*parse(t_env *env, char *s)
{
	char	*line;
	char	status;
	int		i;
	int		j;
	char	*value;

	if (!quotes_closed(s))
		return (printf("minishell error:\texpected quote"), NULL);
	line = malloc(sizeof(char) * 2000);
	if (!line)
		return (printf("minishell error:\tmalloc fail in parse()"), NULL);
	status = 0;
	i = 0;
	while (*s)
	{
		if ((*s == SQUOTE || *s == DQUOTE) && !status)
		{
			status = *(s++);
			continue ;
		}
		if (*s == status)
		{
			status = !status;
			s++;
			continue ;
		}
		if (status != SQUOTE && *s == '$')
		{
			s++;
			value = expander(env, s);
			j = 0;
			while (value[j])
				line[i++] = value[j++];
			while (ft_isalnum(*s) || *s == '_')
				s++;
		}
		else
			line[i++] = *(s++);
	}
	line[i] = '\0';
	return (line);
}
