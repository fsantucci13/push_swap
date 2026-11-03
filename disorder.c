#include "push_swap.h"

double	compute_disorder(t_data *d)
{
	long long	bad;
	long long	total;
	int			i;
	int			j;

	bad = 0;
	total = (long long)d->na * (d->na - 1) / 2;
	i = 0;
	while (i < d->na)
	{
		j = i + 1;
		while (j < d->na)
		{
			if (d->a[i] > d->a[j])
				bad++;
			j++;
		}
		i++;
	}
	if (!total)
		return (0.0);
	return ((double)bad / (double)total);
}

int	is_sorted(t_data *d)
{
	int	i;

	i = 1;
	while (i < d->na)
	{
		if (d->a[i - 1] > d->a[i])
			return (0);
		i++;
	}
	return (1);
}

void	free_data(t_data *d)
{
	free(d->a);
	free(d->b);
}
