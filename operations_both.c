/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_both.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:24:50 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ss(t_data *d)
{
	int	t;

	if (d->na > 1)
	{
		t = d->a[0];
		d->a[0] = d->a[1];
		d->a[1] = t;

	}
	if (d->nb > 1)
	{
		t = d->b[0];
		d->b[0] = d->b[1];
		d->b[1] = t;

	}
	d->count[2]++;
	write(1, "ss\n", 3);
}

void	rr(t_data *d)
{
	int	t;
	int	i;

	if (d->na > 1)
	{
		t = d->a[0];
		i = 0;
		while (++i < d->na)
			d->a[i - 1] = d->a[i];
		d->a[i - 1] = t;

	}
	if (d->nb > 1)
	{
		rb_silent(d);
		d->count[6]--;
	}
	d->count[7]++;
	write(1, "rr\n", 3);
}

void	rrr(t_data *d)
{
	int	t;
	int	i;

	if (d->na > 1)
	{
		t = d->a[d->na - 1];
		i = d->na - 1;
		while (i > 0)
		{
			d->a[i] = d->a[i - 1];
			i--;
		}
		d->a[0] = t;

	}
	if (d->nb > 1)
	{
		rrb_silent(d);
		d->count[9]--;
	}
	d->count[10]++;
	write(1, "rrr\n", 4);
}
