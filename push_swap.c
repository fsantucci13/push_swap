/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   push_swap.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: fsantucc <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2026/10/30 16:08:01 by fsantucc       #+#    #+#                */
/*   Updated: 2026/10/30 16:08:02 by fsantucc       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_sorted(t_stack *st)
{
	t_stack	*s;

	s = st;
	while (s && s->next)
	{
		if (s->n > s->next->n)
			return (0);
		s = s->next;
	}
	return (1);
}

static void	push_swap(t_stack **sa, t_stack **sb, int size)
{
	if (size == 2 && !is_sorted(*sa))
		swap_move(sa, NULL, "sa");
	else if (size == 3)
		small_sort(sa);
	else if (size > 3 && !is_sorted(*sa))
		big_sort(sa, sb);
}

int	main(int ac, char **av)
{
	int		size;
	t_stack	*sa;
	t_stack	*sb;

	if (ac > 1 && ft_strcmp(av[1], "--complex") == 0)
	{
		ac--;
		av++;
	}
	if (ac < 2)
		return (0);
	if (!is_correct_input(av))
		print_error_and_exit(NULL, NULL);
	sa = create_stack(ac, av);
	sb = NULL;
	size = get_stack_size(sa);
	get_main_index(sa, size);
	push_swap(&sa, &sb, size);
	free_stack(&sa);
	free_stack(&sb);
	return (0);
}
