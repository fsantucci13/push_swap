#include "push_swap.h"

static int	read_values(t_data *d, int argc, char **argv)
{
	int	i;
	int	selected;

	i = 1;
	selected = 0;
	while (i < argc)
	{
		if (is_option(argv[i]) && !set_option(d, argv[i], &selected))
			return (0);
		if (!is_option(argv[i]) && !parse_arg(d, argv[i]))
			return (0);
		i++;
	}
	return (d->na > 0);
}

static size_t	get_capacity(int argc, char **argv)
{
	size_t	capacity;
	int		i;
	char	*p;

	capacity = 1;
	i = 1;
	while (i < argc)
	{
		p = argv[i];
		while (*p++)
			capacity++;
		i++;
	}
	return (capacity);
}

int	parse_input(t_data *d, int argc, char **argv)
{
	size_t	capacity;

	capacity = get_capacity(argc, argv);
	d->a = malloc(sizeof(int) * capacity);
	if (!d->a || !read_values(d, argc, argv))
		return (0);
	d->n = d->na;
	d->b = malloc(sizeof(int) * d->n);
	if (!d->b)
		return (0);
	d->disorder = compute_disorder(d);
	return (1);
}
