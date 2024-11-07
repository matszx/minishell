/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:02:01 by dzapata           #+#    #+#             */
/*   Updated: 2024/11/07 18:50:20 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <limits.h>

// Determines if a given string has only numbers, with optional sign
int	ft_isnumber(char *str)
{
	if (str && (*str == '-' || *str == '+'))
		str++;
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit((int) *str))
			return (0);
		str++;
	}
	return (1);
}

int	has_quotes(char *str)
{
	while (*str)
	{
		if (*str == SQUOTE || *str == DQUOTE)
			return (1);
		str++;
	}
	return (0);
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
