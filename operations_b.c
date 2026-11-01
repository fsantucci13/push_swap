#include "push_swap.h"

void	sb(t_data *d)
{
	int	t;

	if (d->nb < 2)
		return ;
	t = d->b[0];
	d->b[0] = d->b[1];
	d->b[1] = t;
	d->count[1]++;
	write(1, "sb\n", 3);
}

void	pb(t_data *d)
{
	int	i;

	if (!d->na)
		return ;
	i = d->nb++;
	while (i > 0)
	{
		d->b[i] = d->b[i - 1];
		i--;
	}
	d->b[0] = d->a[0];
	d->na--;
	i = 0;
	while (i < d->na)
	{
		d->a[i] = d->a[i + 1];
		i++;
	}
	d->count[4]++;
	write(1, "pb\n", 3);
}

void	rb(t_data *d)
{
	int	i;
	int	t;

	if (d->nb < 2)
		return ;
	t = d->b[0];
	i = 0;
	while (i < d->nb - 1)
	{
		d->b[i] = d->b[i + 1];
		i++;
	}
	d->b[i] = t;
	d->count[6]++;
	write(1, "rb\n", 3);
}

void	rrb(t_data *d)
{
	int	i;
	int	t;

	if (d->nb < 2)
		return ;
	t = d->b[d->nb - 1];
	i = d->nb - 1;
	while (i > 0)
	{
		d->b[i] = d->b[i - 1];
		i--;
	}
	d->b[0] = t;
	d->count[9]++;
	write(1, "rrb\n", 4);
}
