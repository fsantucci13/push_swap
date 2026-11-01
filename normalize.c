/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:31 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:32 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sift_down(int *a, int root, int end)
{
	int	child;
	int	tmp;

	while (root * 2 + 1 <= end)
	{
		child = root * 2 + 1;
		if (child + 1 <= end && a[child] < a[child + 1])
			child++;
		if (a[root] >= a[child])
			return ;
		tmp = a[root];
		a[root] = a[child];
		a[child] = tmp;
		root = child;
	}
}

static void	heap_sort(int *a, int n)
{
	int	i;
	int	tmp;

	i = n / 2 - 1;
	while (i >= 0)
	{
		sift_down(a, i, n - 1);
		i--;
	}
	i = n - 1;
	while (i > 0)
	{
		tmp = a[0];
		a[0] = a[i];
		a[i] = tmp;
		sift_down(a, 0, --i);
	}
}

static int	get_rank(int *sorted, int n, int value)
{
	int	lo;
	int	hi;
	int	mid;

	lo = 0;
	hi = n - 1;
	while (lo <= hi)
	{
		mid = lo + (hi - lo) / 2;
		if (sorted[mid] == value)
			return (mid);
		if (sorted[mid] < value)
			lo = mid + 1;
		else
			hi = mid - 1;
	}
	return (-1);
}

int	normalize(t_data *d)
{
	int	*sorted;
	int	i;

	sorted = malloc(sizeof(int) * d->n);
	if (!sorted)
		return (0);
	i = 0;
	while (i < d->n)
	{
		sorted[i] = d->a[i];
		i++;
	}
	heap_sort(sorted, d->n);
	i = 0;
	while (i < d->n)
	{
		d->a[i] = get_rank(sorted, d->n, d->a[i]);
		i++;
	}
	free(sorted);
	return (1);
}
