/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_turk.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 00:00:00 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/02 00:00:01 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** find_target_a: index in A to bring to top before PA so v lands correctly,
** keeping A circularly ascending.
*/
static int	find_target_a(t_data *d, int v)
{
	int	i;
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
		if (d->a[i] < v && d->a[i] > best_val)
		{
			best_val = d->a[i];
			best_pos = i;
		}
		i++;
	}
	if (best_pos == -1)
		return ((max_pos + 1) % d->na);
	return ((best_pos + 1) % d->na);
}

/*
** find_target_b: index in B to bring to top before PB so v lands correctly,
** keeping B circularly descending.
*/
static int	find_target_b(t_data *d, int v)
{
	int	i;
	int	best_pos;
	int	best_val;
	int	max_pos;

	if (d->nb == 0)
		return (0);
	best_pos = -1;
	best_val = INT_MIN;
	max_pos = 0;
	i = 0;
	while (i < d->nb)
	{
		if (d->b[i] > d->b[max_pos])
			max_pos = i;
		if (d->b[i] < v && d->b[i] > best_val)
		{
			best_val = d->b[i];
			best_pos = i;
		}
		i++;
	}
	if (best_pos == -1)
		return (max_pos);
	return (best_pos);
}

/*
** cost_pair: minimum ops to bring position p (in stack of size np) and
** position q (in stack of size nq) to their tops, then one final op.
** Considers all 4 combos (fwd/rev for each stack):
**   0: both fwd  → max(pf, qf)+1    (use simultaneous RR/RRR)
**   1: both rev  → max(pr, qr)+1
**   2: p-fwd q-rev → pf+qr+1       (sequential, no sharing)
**   3: p-rev q-fwd → pr+qf+1
*/
static int	cost_pair(int p, int np, int q, int nq)
{
	int	pf;
	int	pr;
	int	qf;
	int	qr;
	int	costs[4];
	int	min;
	int	i;

	pf = p;
	pr = np - p;
	qf = q;
	qr = nq - q;
	costs[0] = (pf > qf ? pf : qf) + 1;
	costs[1] = (pr > qr ? pr : qr) + 1;
	costs[2] = pf + qr + 1;
	costs[3] = pr + qf + 1;
	min = costs[0];
	i = 1;
	while (i < 4)
	{
		if (costs[i] < min)
			min = costs[i];
		i++;
	}
	return (min);
}

/*
** Apply min(ps,qs) simultaneous ops (op_both), then remaining ps as op_p
** and remaining qs as op_q.
*/
static void	apply_rot(t_data *d, int ps, int qs,
		int op_both, int op_p, int op_q)
{
	int	shared;

	shared = ps < qs ? ps : qs;
	ps -= shared;
	qs -= shared;
	while (shared-- > 0)
		apply_op(d, op_both);
	while (ps-- > 0)
		apply_op(d, op_p);
	while (qs-- > 0)
		apply_op(d, op_q);
}

/*
** Execute push A[ai] → B at position bt (B stays circularly descending).
** Picks the optimal direction combo (all 4 considered) then PB.
*/
static void	execute_push(t_data *d, int ai, int bt)
{
	int	af;
	int	ar;
	int	bf;
	int	br;
	int	costs[4];
	int	dir;
	int	i;

	af = ai;
	ar = d->na - ai;
	bf = bt;
	br = d->nb - bt;
	costs[0] = (af > bf ? af : bf) + 1;
	costs[1] = (ar > br ? ar : br) + 1;
	costs[2] = af + br + 1;
	costs[3] = ar + bf + 1;
	dir = 0;
	i = 1;
	while (i < 4)
	{
		if (costs[i] < costs[dir])
			dir = i;
		i++;
	}
	if (dir == 0)
		apply_rot(d, af, bf, RR, RA, RB);
	else if (dir == 1)
		apply_rot(d, ar, br, RRR, RRA, RRB);
	else if (dir == 2)
	{
		while (af-- > 0)
			apply_op(d, RA);
		while (br-- > 0)
			apply_op(d, RRB);
	}
	else
	{
		while (ar-- > 0)
			apply_op(d, RRA);
		while (bf-- > 0)
			apply_op(d, RB);
	}
	apply_op(d, PB);
}

/*
** Execute pull B[bi] → A at position at (A stays circularly ascending).
** Picks the optimal direction combo (all 4 considered) then PA.
*/
static void	execute_pull(t_data *d, int bi, int at)
{
	int	bf;
	int	br;
	int	af;
	int	ar;
	int	costs[4];
	int	dir;
	int	i;

	bf = bi;
	br = d->nb - bi;
	af = at;
	ar = d->na - at;
	costs[0] = (bf > af ? bf : af) + 1;
	costs[1] = (br > ar ? br : ar) + 1;
	costs[2] = bf + ar + 1;
	costs[3] = br + af + 1;
	dir = 0;
	i = 1;
	while (i < 4)
	{
		if (costs[i] < costs[dir])
			dir = i;
		i++;
	}
	if (dir == 0)
		apply_rot(d, bf, af, RR, RB, RA);
	else if (dir == 1)
		apply_rot(d, br, ar, RRR, RRB, RRA);
	else if (dir == 2)
	{
		while (bf-- > 0)
			apply_op(d, RB);
		while (ar-- > 0)
			apply_op(d, RRA);
	}
	else
	{
		while (br-- > 0)
			apply_op(d, RRB);
		while (af-- > 0)
			apply_op(d, RA);
	}
	apply_op(d, PA);
}

static void	turk_push(t_data *d)
{
	int	ai;
	int	bt;
	int	cost;
	int	best_ai;
	int	best_bt;
	int	best_cost;

	while (d->na > 2)
	{
		best_cost = INT_MAX;
		best_ai = 0;
		best_bt = 0;
		ai = 0;
		while (ai < d->na)
		{
			bt = find_target_b(d, d->a[ai]);
			cost = cost_pair(ai, d->na, bt, d->nb);
			if (cost < best_cost)
			{
				best_cost = cost;
				best_ai = ai;
				best_bt = bt;
			}
			if (best_cost == 1)
				break ;
			ai++;
		}
		execute_push(d, best_ai, best_bt);
	}
	if (d->na == 2 && d->a[0] > d->a[1])
		apply_op(d, SA);
}

static void	turk_pull(t_data *d)
{
	int	bi;
	int	at;
	int	cost;
	int	best_bi;
	int	best_at;
	int	best_cost;

	while (d->nb)
	{
		best_cost = INT_MAX;
		best_bi = 0;
		best_at = 0;
		bi = 0;
		while (bi < d->nb)
		{
			at = find_target_a(d, d->b[bi]);
			cost = cost_pair(bi, d->nb, at, d->na);
			if (cost < best_cost)
			{
				best_cost = cost;
				best_bi = bi;
				best_at = at;
			}
			if (best_cost == 1)
				break ;
			bi++;
		}
		execute_pull(d, best_bi, best_at);
	}
}

static void	arr_dup(int *dst, int *src, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
}

static int	sim_cost(t_data *d, int *orig, int rot)
{
	int	i;
	int	pos;
	int	total;

	arr_dup(d->a, orig, d->n);
	d->na = d->n;
	d->nb = 0;
	i = 0;
	while (i < 11)
		d->count[i++] = 0;
	d->silent = 1;
	while (rot-- > 0)
		apply_op(d, RA);
	turk_push(d);
	turk_pull(d);
	pos = find_min(d);
	if (pos <= d->na / 2)
		while (pos-- > 0)
			apply_op(d, RA);
	else
		while (pos++ < d->na)
			apply_op(d, RRA);
	i = 0;
	total = 0;
	while (i < 11)
		total += d->count[i++];
	return (total);
}

static void	do_turk(t_data *d, int rot)
{
	int	pos;

	while (rot-- > 0)
		apply_op(d, RA);
	turk_push(d);
	turk_pull(d);
	pos = find_min(d);
	if (pos <= d->na / 2)
		while (pos-- > 0)
			apply_op(d, RA);
	else
		while (pos++ < d->na)
			apply_op(d, RRA);
}

int	sort_turk(t_data *d)
{
	int	*orig;
	int	best_ops;
	int	best_rot;
	int	ops;
	int	k;
	int	step;

	if (is_sorted(d))
		return (1);
	orig = malloc(d->n * sizeof(int));
	if (!orig)
	{
		do_turk(d, 0);
		return (1);
	}
	arr_dup(orig, d->a, d->n);
	best_ops = sim_cost(d, orig, 0);
	best_rot = 0;
	if (best_ops > 5400)
	{
		step = d->n / 4;
		if (!step)
			step = 1;
		k = 1;
		while (k < 4)
		{
			ops = sim_cost(d, orig, k * step);
			if (ops < best_ops)
			{
				best_ops = ops;
				best_rot = k * step;
			}
			k++;
		}
	}
	arr_dup(d->a, orig, d->n);
	d->na = d->n;
	d->nb = 0;
	k = 0;
	while (k < 11)
		d->count[k++] = 0;
	free(orig);
	d->silent = 0;
	do_turk(d, best_rot);
	return (1);
}
