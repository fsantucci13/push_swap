/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:24:18 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	put_counts(t_data *d)
{
	int	i;

	bench_putstr("[bench] sa sb ss pa pb ra rb rr rra rrb rrr: ");
	i = 0;
	while (i < 11)
	{
		bench_putnbr(d->count[i++]);
		if (i < 11)
			write(2, " ", 1);
	}
	write(2, "\n", 1);
}

static int	get_total(t_data *d)
{
	int	i;
	int	total;

	total = 0;
	i = 0;
	while (i < 11)
		total += d->count[i++];
	return (total);
}

static void	put_strategy(t_data *d)
{
	char	*names[4];
	char	*classes[4];

	names[0] = "Simple";
	names[1] = "Medium";
	names[2] = "Complex";
	names[3] = "Adaptive";
	classes[0] = "O(n^2)";
	classes[1] = "O(n*sqrt(n))";
	classes[2] = "O(n log n)";
	classes[3] = "adaptive";
	bench_putstr("[bench] Strategy: ");
	bench_putstr(names[d->strategy]);
	bench_putstr(" (");
	if (d->strategy == 3 && d->method != 3)
	{
		bench_putstr(names[d->method]);
		bench_putstr(" / ");
	}
	bench_putstr(classes[d->method]);
	bench_putstr(")\n");
}

void	print_bench(t_data *d)
{
	bench_putstr("[bench] Disorder: ");
	bench_put_percent(d->disorder * 100);
	put_strategy(d);
	bench_putstr("[bench] Total operations: ");
	bench_putnbr(get_total(d));
	write(2, "\n", 1);
	put_counts(d);
}
