/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 15:21:03 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/03 15:21:04 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	number(char *s, int *out)
{
	long long	v;
	long long	limit;
	int			sign;

	v = 0;
	sign = 1;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign = -1;
	if (!*s)
		return (0);
	limit = (long long)INT_MAX + (sign < 0);
	while (*s >= '0' && *s <= '9')
	{
		v = v * 10 + (*s++ - '0');
		if (v > limit)
			return (0);
	}
	if (*s)
		return (0);
	*out = (int)(v * sign);
	return (1);
}

static int	append_value(t_data *d, char *arg)
{
	int	value;
	int	i;

	if (!number(arg, &value))
		return (0);
	i = 0;
	while (i < d->na)
		if (d->a[i++] == value)
			return (0);
	d->a[d->na++] = value;
	return (1);
}

static int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static int	parse_token(t_data *d, char **p)
{
	char	*start;
	char	end;

	start = *p;
	while (**p && !is_space(**p))
		(*p)++;
	end = **p;
	**p = '\0';
	if (!append_value(d, start))
	{
		**p = end;
		return (0);
	}
	**p = end;
	return (1);
}

int	parse_arg(t_data *d, char *arg)
{
	char	*p;
	int		seen;

	p = arg;
	seen = 0;
	while (*p)
	{
		while (*p && is_space(*p))
			p++;
		if (*p)
		{
			if (!parse_token(d, &p))
				return (0);
			seen = 1;
		}
	}
	return (seen);
}
