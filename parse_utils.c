/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:24:47 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/03 14:24:51 by fsantucc         ###   ########.fr       */
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

int	append_value(t_data *d, char *arg)
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
