/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:46:33 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/02 14:45:03 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static int	ft_numlen(int n)
{
	int	res;

	res = -1;
	if (n > 0)
		n = -n;
	else
		res++;
	while (res++, n)
		n /= 10;
	return (res);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*res;

	len = ft_numlen(n);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len--] = '\0';
	if (n == 0)
		res[len] = '0';
	else if (n >= 0)
		n = -n;
	else
		res[0] = '-';
	while (n)
	{
		res[len--] = -(n % 10) + '0';
		n /= 10;
	}
	return (res);
}
