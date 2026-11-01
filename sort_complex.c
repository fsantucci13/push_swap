#include "push_swap.h"

static void	radix_pass(t_data *d, int bit)
{
	int	i;
	int	n;

	n = d->na;
	i = 0;
	while (i++ < n)
	{
		if ((d->a[0] >> bit) & 1)
			ra(d);
		else
			pb(d);
	}
	while (d->nb)
		pa(d);
}

void	sort_complex(t_data *d)
{
	int	max_bits;
	int	bits;

	max_bits = 0;
	while ((d->n - 1) >> max_bits)
		max_bits++;
	bits = 0;
	while (bits < max_bits)
		radix_pass(d, bits++);
}
