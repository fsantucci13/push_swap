/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_stack.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:07:44 by fsantucc          #+#    #+#             */
/*   Updated: 2026/10/30 19:08:26 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	get_main_index(t_stack *sa, int size)
{
	t_stack	*a;
	t_stack	*b;
	int		rank;

	(void)size;
	a = sa;
	while (a)
	{
		rank = 1;
		b = sa;
		while (b)
		{
			if (b->n < a->n)
				rank++;
			b = b->next;
		}
		a->main_index = rank;
		a = a->next;
	}
}

int	get_stack_size(t_stack *st)
{
	int		size;
	t_stack	*s;

	size = 0;
	s = st;
	while (s)
	{
		size++;
		s = s->next;
	}
	return (size);
}

static t_stack	*new_node(int n)
{
	t_stack	*s;

	s = (t_stack *)malloc(sizeof(t_stack));
	if (!s)
		return (NULL);
	s->n = n;
	s->main_index = 0;
	s->position = -1;
	s->where_fit = -1;
	s->mv_a = -1;
	s->mv_b = -1;
	s->next = NULL;
	return (s);
}

static int	add_at_end(t_stack **st, t_stack *node)
{
	t_stack	*s;

	if (!st || !node)
		return (0);
	if (!*st)
		*st = node;
	else
	{
		s = *st;
		while (s->next)
			s = s->next;
		s->next = node;
	}
	return (1);
}

t_stack	*create_stack(int ac, char **av)
{
	t_stack		*sa;
	t_stack		*node;
	long int	n;
	int			i;

	sa = NULL;
	i = 1;
	while (i < ac)
	{
		n = ft_atoi(av[i]);
		if (n < INT_MIN || n > INT_MAX)
			print_error_and_exit(&sa, NULL);
		node = new_node((int)n);
		if (!node)
			print_error_and_exit(&sa, NULL);
		if (!add_at_end(&sa, node))
		{
			free(node);
			print_error_and_exit(&sa, NULL);
		}
		i++;
	}
	return (sa);
}
