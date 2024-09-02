/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:58:07 by mcygan            #+#    #+#             */
/*   Updated: 2024/09/02 14:45:03 by mcygan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	mul;
	void	*ptr;

	if (!count || !size)
		return (malloc(0));
	mul = count * size;
	if (mul < count + size - 1)
		return (NULL);
	ptr = malloc(mul);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, mul);
	return (ptr);
}
