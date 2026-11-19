/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:17 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:18 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static const char	*strategy_name(t_data *d)
{
	if (d->strategy == 3 && d->used_strategy == 0)
		return ("Adaptive -> Simple O(n^2)");
	if (d->strategy == 3 && d->used_strategy == 1)
		return ("Adaptive -> Medium O(n sqrt(n))");
	if (d->strategy == 3)
		return ("Adaptive -> Complex O(n log n)");
	if (d->used_strategy == 0)
		return ("Simple O(n^2)");
	if (d->used_strategy == 1)
		return ("Medium O(n sqrt(n))");
	return ("Complex O(n log n)");
}

static void	print_disorder(double disorder)
{
	long	value;

	value = (long)(disorder * 10000 + 0.5);
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putnbr_fd(value / 100, 2);
	ft_putstr_fd(".", 2);
	if (value % 100 < 10)
		ft_putstr_fd("0", 2);
	ft_putnbr_fd(value % 100, 2);
	ft_putendl_fd("%", 2);
}

static void	print_count(const char *name, int count)
{
	ft_putstr_fd("[bench] ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putnbr_fd(count, 2);
	ft_putstr_fd("\n", 2);
}

static int	total_ops(t_data *d)
{
	int	i;
	int	total;

	i = 0;
	total = 0;
	while (i < 11)
		total += d->count[i++];
	return (total);
}

void	print_bench(t_data *d)
{
	static const char	*names[11] = {"sa", "sb", "ss", "pa", "pb",
		"ra", "rb", "rr", "rra", "rrb", "rrr"};
	int					i;

	print_disorder(d->disorder);
	ft_putstr_fd("[bench] strategy: ", 2);
	ft_putendl_fd(strategy_name(d), 2);
	print_count("total operations", total_ops(d));
	i = 0;
	while (i < 11)
	{
		print_count(names[i], d->count[i]);
		i++;
	}
}
