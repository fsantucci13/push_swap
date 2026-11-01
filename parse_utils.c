/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:15 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:24:56 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f');
}

int	streq(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return (a[i] == b[i]);
}

int	parse_number(char *s, int *out)
{
	long long	v;
	int			sign;

	v = 0;
	sign = 1 - 2 * (*s == '-');
	if (*s == '-' || *s == '+')
		s++;
	if (!*s)
		return (0);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		v = v * 10 + (*s++ - '0');
		if (v > 2147483648LL)
			return (0);
	}
	if (v * sign < INT_MIN || v * sign > INT_MAX)
		return (0);
	*out = (int)(v * sign);
	return (1);
}

int	count_words(char *s)
{
	int	n;

	n = 0;
	while (*s)
	{
		while (*s && is_space(*s))
			s++;
		if (*s)
			n++;
		while (*s && !is_space(*s))
			s++;
	}
	return (n);
}

