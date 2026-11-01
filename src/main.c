/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:21:58 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	init_data(t_data *d)
{
	int	i;

	d->a = NULL;
	d->b = NULL;
	d->n = 0;
	d->na = 0;
	d->nb = 0;
	d->disorder = 0.0;
	d->strategy = 3;
	d->method = 3;
	d->bench = 0;
	i = 0;
	while (i < 11)
		d->count[i++] = 0;
}

static void	free_data(t_data *d)
{
	free(d->a);
	free(d->b);
}

static void	choose_method(t_data *d)
{
	d->method = d->strategy;
	if (d->strategy == 3 && d->disorder < 0.20)
		d->method = 0;
	else if (d->strategy == 3 && d->disorder < 0.50)
		d->method = 1;
	else if (d->strategy == 3)
		d->method = 2;
}

static void	select_sort(t_data *d)
{
	if (d->strategy == 0)
		sort_simple(d);
	else if (d->strategy == 1)
		sort_medium(d);
	else if (d->strategy == 2)
		sort_complex(d);
	else
		sort_adaptive(d);
}

int	main(int ac, char **av)
{
	t_data	d;

	init_data(&d);
	if (ac == 1)
		return (0);
	if (!parse_args(&d, ac, av))
		error_exit(&d);
	if (d.n > 1)
		d.disorder = get_disorder(&d);
	choose_method(&d);
	if (d.n > 1 && !is_sorted(&d))
	{
		if (!assign_ranks(&d))
			error_exit(&d);
		select_sort(&d);
	}
	if (d.bench)
		print_bench(&d);
	free_data(&d);
	return (0);
}
