/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:24:44 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sa(t_data *d)
{
	int	t;

	if (d->na < 2)
		return ;
	t = d->a[0];
	d->a[0] = d->a[1];
	d->a[1] = t;
	d->count[0]++;
	write(1, "sa\n", 3);
}

void	pa(t_data *d)
{
	int	i;

	if (!d->nb)
		return ;
	i = d->na++;
	while (i > 0)
	{
		d->a[i] = d->a[i - 1];
		i--;
	}
	d->a[0] = d->b[0];
	d->nb--;
	i = 0;
	while (i < d->nb)
	{
		d->b[i] = d->b[i + 1];
		i++;
	}
	d->count[3]++;
	write(1, "pa\n", 3);
}

void	ra(t_data *d)
{
	int	i;
	int	t;

	if (d->na < 2)
		return ;
	t = d->a[0];
	i = 0;
	while (i < d->na - 1)
	{
		d->a[i] = d->a[i + 1];
		i++;
	}
	d->a[i] = t;
	d->count[5]++;
	write(1, "ra\n", 3);
}

void	rra(t_data *d)
{
	int	i;
	int	t;

	if (d->na < 2)
		return ;
	t = d->a[d->na - 1];
	i = d->na - 1;
	while (i > 0)
	{
		d->a[i] = d->a[i - 1];
		i--;
	}
	d->a[0] = t;
	d->count[8]++;
	write(1, "rra\n", 4);
}
