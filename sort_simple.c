/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:49 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:50 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	find_min(t_data *d)
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
			apply_op(d, RA);
	else
		while (pos++ < d->na)
			apply_op(d, RRA);
}

void	sort_simple(t_data *d)
{
	int	pos;

	while (d->na > 1)
	{
		pos = find_min(d);
		bring_min(d, pos);
		apply_op(d, PB);
	}
	while (d->nb)
		apply_op(d, PA);
}
