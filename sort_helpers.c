#include "push_swap.h"

int	find_max_b(t_data *d)
{
	int	i;
	int	pos;

	i = 1;
	pos = 0;
	while (i < d->nb)
	{
		if (d->b[i] > d->b[pos])
			pos = i;
		i++;
	}
	return (pos);
}

static void	bring_max(t_data *d, int pos)
{
	if (pos <= d->nb / 2)
		while (pos-- > 0)
			apply_op(d, RB);
	else
		while (pos++ < d->nb)
			apply_op(d, RRB);
}

void	sort_b_to_a(t_data *d)
{
	int	pos;

	while (d->nb)
	{
		pos = find_max_b(d);
		bring_max(d, pos);
		apply_op(d, PA);
	}
}
