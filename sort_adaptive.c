/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxim76 <maxim76@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:40 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/03 10:39:22 by maxim76          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_three(t_data *d)
{
	int	a;
	int	b;
	int	c;

	if (d->na <= 1)
		return ;
	if (d->na == 2)
	{
		if (d->a[0] > d->a[1])
			apply_op(d, SA);
		return ;
	}
	a = d->a[0];
	b = d->a[1];
	c = d->a[2];
	if (a < b && b < c)
		return ;
	if (a > b && b > c)
	{
		apply_op(d, SA);
		apply_op(d, RRA);
		return ;
	}
	if (a > b && a < c)
		apply_op(d, SA);
	else if (a > b)
		apply_op(d, RA);
	else if (a < c)
	{
		apply_op(d, RRA);
		apply_op(d, SA);
	}
	else
		apply_op(d, RRA);
}

static void	insert_b0(t_data *d)
{
	int	i;
	int	at;
	int	best_pos;
	int	best_val;
	int	max_pos;

	best_pos = -1;
	best_val = INT_MIN;
	max_pos = 0;
	i = 0;
	while (i < d->na)
	{
		if (d->a[i] > d->a[max_pos])
			max_pos = i;
		if (d->a[i] < d->b[0] && d->a[i] > best_val)
		{
			best_val = d->a[i];
			best_pos = i;
		}
		i++;
	}
	if (best_pos == -1)
		at = (max_pos + 1) % d->na;
	else
		at = (best_pos + 1) % d->na;
	if (at <= d->na / 2)
		while (at-- > 0)
			apply_op(d, RA);
	else
		while (at++ < d->na)
			apply_op(d, RRA);
	apply_op(d, PA);
}

static void	sort_five(t_data *d)
{
	int	pos;

	apply_op(d, PB);
	apply_op(d, PB);
	sort_three(d);
	insert_b0(d);
	insert_b0(d);
	pos = find_min(d);
	if (pos <= d->na / 2)
		while (pos-- > 0)
			apply_op(d, RA);
	else
		while (pos++ < d->na)
			apply_op(d, RRA);
}

int	sort_adaptive(t_data *d)
{
	if (d->n <= 3)
	{
		d->used_strategy = 0;
		sort_three(d);
		return (1);
	}
	if (d->n <= 5)
	{
		d->used_strategy = 0;
		sort_five(d);
		return (1);
	}
	if (d->disorder < 0.2)
	{
		d->used_strategy = 0;
		sort_simple(d);
		return (1);
	}
	d->used_strategy = 3;
	return (sort_turk(d));
}
