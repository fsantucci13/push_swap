#include "push_swap.h"

void	error_exit(t_data *d)
{
	write(2, "Error\n", 6);
	free(d->a);
	free(d->b);
	exit(1);
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
