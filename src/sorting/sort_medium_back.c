/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:16 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:25:11 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	max_pos(t_data *d)
{
	int	i;
	int	pos;

	i = 1;
	pos = 0;
	while (i < d->nb)
	{
		if (d->b[i] > d->b[pos])
			pos = i;
		i++;
	}
	return (pos);
}

void	push_back_sorted(t_data *d)
{
	int	pos;

	while (d->nb)
	{
		pos = max_pos(d);
		if (pos <= d->nb / 2)
			while (pos-- > 0)
				rb(d);
		else
		{
			pos = d->nb - pos;
			while (pos-- > 0)
				rrb(d);
		}
		pa(d);
	}
}
