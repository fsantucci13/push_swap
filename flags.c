#include "push_swap.h"

int	is_option(const char *s)
{
	return (!ft_strcmp(s, "--simple") || !ft_strcmp(s, "--medium")
		|| !ft_strcmp(s, "--complex") || !ft_strcmp(s, "--adaptive")
		|| !ft_strcmp(s, "--bench"));
}

int	set_option(t_data *d, char *s, int *selected)
{
	if (!ft_strcmp(s, "--bench"))
	{
		d->bench = 1;
		return (1);
	}
	if (*selected)
		return (0);
	*selected = 1;
	if (!ft_strcmp(s, "--simple"))
		d->strategy = 0;
	else if (!ft_strcmp(s, "--medium"))
		d->strategy = 1;
	else if (!ft_strcmp(s, "--complex"))
		d->strategy = 2;
	else
		d->strategy = 3;
	return (1);
}

