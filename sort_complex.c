/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:43 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:44 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	max_bits(int n)
{
	int	bits;

	bits = 0;
	while (n > 1)
	{
		bits++;
		n = (n + 1) / 2;
	}
	return (bits);
}

static void	radix_pass(t_data *d, int bit)
{
	int	i;

	i = 0;
	while (i < d->n)
	{
		if ((d->a[0] >> bit) & 1)
			apply_op(d, RA);
		else
			apply_op(d, PB);
		i++;
	}
	while (d->nb)
		apply_op(d, PA);
}

int	sort_complex(t_data *d)
{
	int	bit;
	int	bits;

	if (!normalize(d))
		return (0);
	bits = max_bits(d->n);
	bit = 0;
	while (bit < bits)
		radix_pass(d, bit++);
	return (1);
}
