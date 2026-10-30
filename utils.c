/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:08:30 by fsantucc          #+#    #+#             */
/*   Updated: 2026/10/30 19:14:14 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_putstr(char *s)
{
	while (*s)
	{
		write(1, &*s, 1);
		s++;
	}
}

void	free_stack(t_stack **lst)
{
	t_stack	*c;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		c = (*lst)->next;
		free(*lst);
		*lst = c;
	}
	*lst = NULL;
}

void	print_error_and_exit(t_stack **a, t_stack **b)
{
	if (!a || *a)
		free_stack(a);
	if (!b || *b)
		free_stack(b);
	write(2, "Error\n", 6);
	exit(1);
}

int	absolute(int n)
{
	if (n == INT_MIN)
		return (INT_MAX);
	if (n < 0)
		return (-n);
	return (n);
}

long int	ft_atoi(const char *s)
{
	long int	result;
	long int	limit;
	int			sign;

	sign = 1;
	if (*s == '-' || *s == '+')
		if (*s == '-')
			sign = -1;
	s++;
	limit = INT_MAX;
	if (sign == -1)
		limit++;
	result = 0;
	while (*s >= '0' && *s <= '9')
	{
		if (result > (limit - (*s - '0')) / 10)
		{
			if (sign == -1)
				return ((long int)INT_MIN - 1);
			return ((long int)INT_MAX + 1);
		}
		result = result * 10 + (*s - '0');
		s++;
	}
	return (sign * result);
}
