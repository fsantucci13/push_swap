/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:35 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/03 14:24:58 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "push_swap.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

static int	parse_arg(t_data *d, char *arg)
{
	char	*p;
	char	*start;
	char	end;

	p = arg;
	while (*p)
	{
		while (is_space(*p))
			p++;
		if (!*p)
			break ;
		start = p;
		while (*p && !is_space(*p))
			p++;
		end = *p;
		*p = '\0';
		if (!append_value(d, start))
		{
			*p = end;
			return (0);
		}
		*p = end;
	}
	return (1);
}

static int	read_values(t_data *d, int argc, char **argv)
{
	int	i;
	int	selected;

	i = 1;
	selected = 0;
	while (i < argc)
	{
		if (is_option(argv[i]))
		{
			if (!set_option(d, argv[i], &selected))
				return (0);
		}
		else if (!parse_arg(d, argv[i]))
			return (0);
		i++;
	}
	return (d->na > 0);
}

int	parse_input(t_data *d, int argc, char **argv)
{
	size_t	capacity;
	int		i;
	char	*p;

	capacity = 1;
	i = 1;
	while (i < argc)
	{
		p = argv[i];
		while (*p++)
			capacity++;
		i++;
	}
	d->a = malloc(sizeof(int) * capacity);
	if (!d->a || !read_values(d, argc, argv))
		return (0);
	d->n = d->na;
	d->b = malloc(sizeof(int) * d->n);
	if (!d->b)
		return (0);
	d->disorder = compute_disorder(d);
	return (1);
}
