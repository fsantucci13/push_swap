/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:26:38 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	bench_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	write(2, s, i);
}

void	bench_putnbr(int n)
{
	char	buf[12];
	int		i;

	i = 0;
	if (n < 0)
	{
		write(2, "-", 1);
		n = -n;
	}
	if (!n)
		buf[i++] = '0';
	while (n)
	{
		buf[i++] = '0' + n % 10;
		n /= 10;
	}
	while (i-- > 0)
		write(2, &buf[i], 1);
}

void	bench_put_percent(double value)
{
	int	whole;
	int	decimal;

	whole = (int)value;
	decimal = (int)((value - whole) * 100 + 0.5);
	if (decimal == 100)
	{
		whole++;
		decimal = 0;
	}
	bench_putnbr(whole);
	write(2, ".", 1);
	if (decimal < 10)
		write(2, "0", 1);
	bench_putnbr(decimal);
	bench_putstr("%\n");
}
