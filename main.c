#include "push_swap.h"

static void	init_data(t_data *d)
{
	int	i;

	d->a = NULL;
	d->b = NULL;
	d->n = 0;
	d->na = 0;
	d->nb = 0;
	d->disorder = 0;
	d->strategy = 3;
	d->used_strategy = 3;
	d->bench = 0;
	i = 0;
	while (i < 11)
	{
		d->count[i] = 0;
		i++;
	}
}

static void	set_used(t_data *d)
{
	d->used_strategy = d->strategy;
	if (d->strategy == 3 && d->disorder < 0.2)
		d->used_strategy = 0;
	else if (d->strategy == 3 && d->disorder < 0.5)
		d->used_strategy = 1;
	else if (d->strategy == 3)
		d->used_strategy = 2;
}

static int	run_sort(t_data *d)
{
	if (d->strategy == 0)
	{
		d->used_strategy = 0;
		sort_simple(d);
		return (1);
	}
	if (d->strategy == 1)
	{
		d->used_strategy = 1;
		return (sort_medium(d));
	}
	if (d->strategy == 2)
	{
		d->used_strategy = 2;
		return (sort_complex(d));
	}
	return (sort_adaptive(d));
}

int	main(int argc, char **argv)
{
	t_data	d;
	int		ok;

	init_data(&d);
	if (argc == 1)
		return (0);
	ok = parse_input(&d, argc, argv);
	if (ok)
	{
		set_used(&d);
		if (!is_sorted(&d))
			ok = run_sort(&d);
	}
	if (ok && d.bench)
		print_bench(&d);
	if (!ok)
		ft_putendl_fd("Error", 2);
	free_data(&d);
	return (!ok);
}
