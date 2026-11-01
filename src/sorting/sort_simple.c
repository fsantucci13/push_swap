/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:16 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:25:15 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	min_pos(t_data *d)
{
	int	i;
	int	pos;

	i = 1;
	pos = 0;
	while (i < d->na)
	{
		if (d->a[i] < d->a[pos])
			pos = i;
		i++;
	}
	return (pos);
}

static void	bring_min(t_data *d, int pos)
{
	if (pos <= d->na / 2)
		while (pos-- > 0)
			ra(d);
	else
	{
		pos = d->na - pos;
		while (pos-- > 0)
			rra(d);
	}
}

void	sort_simple(t_data *d)
{
	while (d->na > 0)
	{
		bring_min(d, min_pos(d));
		pb(d);
	}
	while (d->nb > 0)
		pa(d);
}
