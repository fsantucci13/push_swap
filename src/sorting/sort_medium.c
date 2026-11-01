/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:16 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:25:13 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	push_one_chunk(t_data *d, int low, int high, int size)
{
	int	steps;

	steps = d->na;
	while (steps-- > 0)
	{
		if (d->a[0] >= low && d->a[0] < high)
		{
			pb(d);
			if (d->b[0] < low + size / 2)
				rb(d);
		}
		else
			ra(d);
	}
}

void	sort_medium(t_data *d)
{
	int	size;
	int	low;

	size = 1;
	while ((long long)size * size < d->n)
		size++;
	low = 0;
	while (low < d->n)
	{
		push_one_chunk(d, low, low + size, size);
		low += size;
	}
	push_back_sorted(d);
}
