#include "push_swap.h"

static int	number(char *s, int *out)
{
	long long	v;
	long long	limit;
	int			sign;

	v = 0;
	sign = 1;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sign = -1;
	if (!*s)
		return (0);
	limit = (long long)INT_MAX + (sign < 0);
	while (*s >= '0' && *s <= '9')
	{
		v = v * 10 + (*s++ - '0');
		if (v > limit)
			return (0);
	}
	if (*s)
		return (0);
	*out = (int)(v * sign);
	return (1);
}

static int	append_value(t_data *d, char *arg)
{
	int	value;
	int	i;

	if (!number(arg, &value))
		return (0);
	i = 0;
	while (i < d->na)
		if (d->a[i++] == value)
			return (0);
	d->a[d->na++] = value;
	return (1);
}

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
		if (!is_option(argv[i]) && !append_value(d, argv[i]))
			return (0);
		i++;
	}
	return (d->na > 0);
}

int	parse_input(t_data *d, int argc, char **argv)
{
	d->a = malloc(sizeof(int) * argc);
	if (!d->a || !read_values(d, argc, argv))
		return (0);
	d->n = d->na;
	d->b = malloc(sizeof(int) * d->n);
	if (!d->b)
		return (0);
	d->disorder = compute_disorder(d);
	return (1);
}
