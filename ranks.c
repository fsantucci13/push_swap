/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ranks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:16 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:25:03 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	rank_of(t_data *d, int value)
{
	int	i;
	int	rank;

	i = 0;
	rank = 0;
	while (i < d->n)
	{
		if (d->a[i] < value)
			rank++;
		i++;
	}
	return (rank);
}

int	assign_ranks(t_data *d)
{
	int	*tmp;
	int	i;

	tmp = malloc(sizeof(int) * d->n);
	if (!tmp)
		return (0);
	i = 0;
	while (i < d->n)
	{
		tmp[i] = rank_of(d, d->a[i]);
		i++;
	}
	i = 0;
	while (i < d->n)
	{
		d->a[i] = tmp[i];
		i++;
	}
	free(tmp);
	return (1);
}

double	get_disorder(t_data *d)
{
	long long	bad;
	long long	pairs;
	int			i;
	int			j;

	bad = 0;
	pairs = (long long)d->n * (d->n - 1) / 2;
	i = 0;
	while (i < d->n)
	{
		j = i + 1;
		while (j < d->n)
			if (d->a[i] > d->a[j++])
				bad++;
		i++;
	}
	if (!pairs)
		return (0.0);
	return ((double)bad / (double)pairs);
}
