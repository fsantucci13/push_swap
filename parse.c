/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:37:56 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	check_duplicates(t_data *d)
{
	int	i;
	int	j;

	i = 0;
	while (i < d->n)
	{
		j = i + 1;
		while (j < d->n)
			if (d->a[i] == d->a[j++])
				return (0);
		i++;
	}
	return (1);
}

static int	parse_flag(t_data *d, char *s)
{
	if (streq(s, "--simple"))
		d->strategy = 0;
	else if (streq(s, "--medium"))
		d->strategy = 1;
	else if (streq(s, "--complex"))
		d->strategy = 2;
	else if (streq(s, "--adaptive"))
		d->strategy = 3;
	else if (streq(s, "--bench"))
		d->bench = 1;
	else
		return (0);
	return (1);
}

static int	count_args(int ac, char **av)
{
	int	i;
	int	n;

	n = 0;
	i = 1;
	while (i < ac)
	{
		if (!streq(av[i], "--simple") && !streq(av[i], "--medium")
			&& !streq(av[i], "--complex") && !streq(av[i], "--adaptive")
			&& !streq(av[i], "--bench"))
			n += count_words(av[i]);
		i++;
	}
	return (n);
}

static int	fill_args(t_data *d, int ac, char **av)
{
	int	i;
	int	pos;

	pos = 0;
	i = 1;
	while (i < ac)
	{
		if (!av[i][0])
			return (0);
		if (!parse_flag(d, av[i]) && (!count_words(av[i])
				|| !read_words(d, av[i], &pos)))
			return (0);
		i++;
	}
	d->n = pos;
	d->na = pos;
	return (1);
}


int	parse_args(t_data *d, int ac, char **av)
{
	int	total;

	total = count_args(ac, av);
	d->a = malloc(sizeof(int) * (total + 1));
	d->b = malloc(sizeof(int) * (total + 1));
	if (!d->a || !d->b)
		return (0);
	if (!fill_args(d, ac, av))
		return (0);
	return (check_duplicates(d));
}
