/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:24:52 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rb_silent(t_data *d)
{
	int	t;
	int	i;

	t = d->b[0];
	i = 0;
	while (i < d->nb - 1)
	{
		d->b[i] = d->b[i + 1];
		i++;
	}
	d->b[i] = t;
	d->count[6]++;
}

void	rrb_silent(t_data *d)
{
	int	t;
	int	i;

	t = d->b[d->nb - 1];
	i = d->nb - 1;
	while (i > 0)
	{
		d->b[i] = d->b[i - 1];
		i--;
	}
	d->b[0] = t;
	d->count[9]++;
}
