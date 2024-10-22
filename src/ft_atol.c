/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:02:01 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/22 01:28:43 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <limits.h>

long	ft_atol(const char *nptr, int *overflow)
{
	int		i;
	long	number;
	char	sign;

	i = 0;
	sign = '+';
	number = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		sign = nptr[i++];
	while (ft_isdigit(nptr[i]) && number > -1)
		number = (number * 10) + (nptr[i++] - '0');
	if (number < 0)
	{
		if (sign == '-')
			return ((*overflow) = 1, LONG_MIN);
		return ((*overflow) = 1, LONG_MAX);
	}
	if (sign == '-')
		return ((*overflow) = 0, -number);
	return ((*overflow) = 0, number);
}
