/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:33 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:34 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	swap(int *s, int n)
{
	int	tmp;

	if (n > 1)
	{
		tmp = s[0];
		s[0] = s[1];
		s[1] = tmp;
	}
}

static void	push(int *dst, int *nd, int *src, int *ns)
{
	int	i;

	if (!*ns)
		return ;
	i = *nd;
	while (i > 0)
	{
		dst[i] = dst[i - 1];
		i--;
	}
	dst[0] = src[0];
	(*nd)++;
	i = 0;
	while (i < *ns - 1)
	{
		src[i] = src[i + 1];
		i++;
	}
	(*ns)--;
}

static void	rotate(int *s, int n)
{
	int	first;
	int	i;

	if (n < 2)
		return ;
	first = s[0];
	i = 0;
	while (i < n - 1)
	{
		s[i] = s[i + 1];
		i++;
	}
	s[n - 1] = first;
}

static void	reverse(int *s, int n)
{
	int	last;
	int	i;

	if (n < 2)
		return ;
	last = s[n - 1];
	i = n - 1;
	while (i > 0)
	{
		s[i] = s[i - 1];
		i--;
	}
	s[0] = last;
}

void	apply_op(t_data *d, int op)
{
	static const char	*names[11] = {"sa", "sb", "ss", "pa", "pb",
		"ra", "rb", "rr", "rra", "rrb", "rrr"};

	if (op == SA || op == SS)
		swap(d->a, d->na);
	if (op == SB || op == SS)
		swap(d->b, d->nb);
	if (op == PA)
		push(d->a, &d->na, d->b, &d->nb);
	if (op == PB)
		push(d->b, &d->nb, d->a, &d->na);
	if (op == RA || op == RR)
		rotate(d->a, d->na);
	if (op == RB || op == RR)
		rotate(d->b, d->nb);
	if (op == RRA || op == RRR)
		reverse(d->a, d->na);
	if (op == RRB || op == RRR)
		reverse(d->b, d->nb);
	d->count[op]++;
	ft_putendl_fd(names[op], 1);
}
