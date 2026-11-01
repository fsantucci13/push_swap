#include "push_swap.h"

static void	push_one_chunk(t_data *d, int low, int high, int size)
{
	int	steps;

	steps = d->na;
	while (steps-- > 0)
	{
		if (d->a[0] >= low && d->a[0] < high)
		{
			pb(d);
			if (d->b[0] < low + size / 2)
				rb(d);
		}
		else
			ra(d);
	}
}

void	sort_medium(t_data *d)
{
	int	size;
	int	low;

	size = 1;
	while ((long long)size * size < d->n)
		size++;
	low = 0;
	while (low < d->n)
	{
		push_one_chunk(d, low, low + size, size);
		low += size;
	}
	push_back_sorted(d);
}
