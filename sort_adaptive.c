#include "push_swap.h"

int	sort_adaptive(t_data *d)
{
	if (d->disorder < 0.2)
	{
		d->used_strategy = 0;
		sort_simple(d);
		return (1);
	}
	if (d->disorder < 0.5)
		d->used_strategy = 1;
	else
		d->used_strategy = 2;
	if (d->used_strategy == 1)
		return (sort_medium(d));
	return (sort_complex(d));
}
