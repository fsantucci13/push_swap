/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_three(t_data *d)
{
	if (d->a[0] > d->a[1])
	{
		if (d->a[1] < d->a[2] && d->a[0] < d->a[2])
			apply_op(d, SA);
		else if (d->a[1] < d->a[2])
			apply_op(d, RA);
		else
		{
			apply_op(d, SA);
			apply_op(d, RRA);
		}
	}
	else if (d->a[1] > d->a[2])
	{
		if (d->a[0] < d->a[2])
		{
			apply_op(d, SA);
			apply_op(d, RA);
		}
		else
			apply_op(d, RRA);
	}
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

void	sort_small(t_data *d)
{
	int	pos;

	if (d->na == 2 && d->a[0] > d->a[1])
		apply_op(d, SA);
	if (d->na == 3)
		sort_three(d);
	while (d->na > 3)
	{
		pos = find_min(d);
		bring_min(d, pos);
		apply_op(d, PB);
	}
	if (d->na == 3 && d->nb > 0)
		sort_three(d);
	while (d->nb > 0)
		apply_op(d, PA);
}
