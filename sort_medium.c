/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:47 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:48 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	chunk_size(int n)
{
	int	size;

	size = 1;
	while (size * size < 8 * n)
		size++;
	return (size);
}

static void	process_range(t_data *d, int low, int high)
{
	int	i;
	int	total;

	total = d->na;
	i = 0;
	while (i < total)
	{
		if (d->a[0] >= low && d->a[0] < high)
			apply_op(d, PB);
		else if (d->na > 1)
			apply_op(d, RA);
		i++;
	}
}

static void	push_ranges(t_data *d, int size)
{
	int	low;
	int	high;

	high = d->n;
	while (high > 0)
	{
		low = high - size;
		if (low < 0)
			low = 0;
		process_range(d, low, high);
		sort_b_to_a(d);
		high = low;
	}
}

int	sort_medium(t_data *d)
{
	if (!normalize(d))
		return (0);
	push_ranges(d, chunk_size(d->n));
	return (1);
}
