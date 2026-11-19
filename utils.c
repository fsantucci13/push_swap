/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:51 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:52 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_strcmp(const char *a, const char *b)
{
	while (*a && *a == *b)
	{
		a++;
		b++;
	}
	return ((unsigned char)*a - (unsigned char)*b);
}

void	ft_putstr_fd(const char *s, int fd)
{
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

void	ft_putendl_fd(const char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

void	ft_putnbr_fd(long n, int fd)
{
	char	buf[12];
	int		i;

	i = 0;
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (!n)
		buf[i++] = '0';
	while (n && i < 11)
	{
		buf[i++] = '0' + n % 10;
		n /= 10;
	}
	while (i)
		write(fd, &buf[--i], 1);
}
