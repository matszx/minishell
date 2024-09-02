/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:51:42 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/02 14:45:03 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static void	*ft_free(char **strs, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n)
		free(strs[i++]);
	free(strs);
	return (NULL);
}

static unsigned int	ft_word_count(char *str, char c)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	while (str[i])
		i++;
	count = 0;
	while (i > 0)
	{
		if (str[i - 1] != c && (str[i] == c || !str[i]))
			count++;
		i--;
	}
	return (count);
}

static char	*ft_find_substr(char *str, char c)
{
	size_t			len;
	char			*res;
	unsigned int	i;

	len = 0;
	while (str[len] != c && str[len])
		len++;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	**ft_split(char const *s, char c)
{
	unsigned int	wc;
	char			**res;
	unsigned int	i;

	wc = ft_word_count((char *)s, c);
	res = malloc((wc + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			res[i++] = ft_find_substr((char *)s, c);
			if (!res[i - 1])
				return (ft_free(res, i));
		}
		while (*s != c && *s)
			s++;
	}
	res[i] = 0;
	return (res);
}
