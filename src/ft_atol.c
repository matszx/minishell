/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:02:01 by dzapata           #+#    #+#             */
/*   Updated: 2024/10/22 17:27:56 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <limits.h>

char	*ft_strjoin2(const char *s, ...)
{
	int		print_c;
	va_list	args;

	va_start(args, s);
	print_c = ft_printf_funct(1, s, args);
	va_end(args);
	return (print_c);
}

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
