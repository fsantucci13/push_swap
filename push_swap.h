#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_data
{
	int	*a;
	int	*b;
	int	n;
	int	na;
	int	nb;
	int	count[11];
	double	disorder;
	int	strategy;
	int	used_strategy;
	int	bench;
}	t_data;

enum e_op {SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR};

int		ft_strcmp(const char *a, const char *b);
void	ft_putstr_fd(const char *s, int fd);
void	ft_putendl_fd(const char *s, int fd);
void	ft_putnbr_fd(long n, int fd);
int		is_option(const char *s);
int		set_option(t_data *d, char *s, int *selected);
int		parse_input(t_data *d, int argc, char **argv);
void	free_data(t_data *d);
double	compute_disorder(t_data *d);
int		is_sorted(t_data *d);
int		normalize(t_data *d);
void	apply_op(t_data *d, int op);
void	sort_simple(t_data *d);
int		sort_medium(t_data *d);
int		sort_complex(t_data *d);
int		sort_adaptive(t_data *d);
void	sort_b_to_a(t_data *d);
int		find_min(t_data *d);
int		find_max_b(t_data *d);
void	print_bench(t_data *d);
#endif
