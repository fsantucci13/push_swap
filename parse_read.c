#include "push_swap.h"

static int	read_token(t_data *d, char **s, int *pos)
{
	char	*end;
	char	saved;
	int		len;
	int		v;

	len = 0;
	while ((*s)[len] && !is_space((*s)[len]))
		len++;
	end = *s + len;
	saved = *end;
	*end = 0;
	if (!parse_number(*s, &v))
	{
		*end = saved;
		return (0);
	}
	*end = saved;
	d->a[(*pos)++] = v;
	*s += len;
	return (1);
}

int	read_words(t_data *d, char *s, int *pos)
{
	while (*s)
	{
		while (*s && is_space(*s))
			s++;
		if (*s && !read_token(d, &s, pos))
			return (0);
	}
	return (1);
}
