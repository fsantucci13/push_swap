#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stddef.h>

/* All arrays use index 0 as the top of the corresponding stack. */
typedef struct s_data
{
	int	*a;			/* Stack A. */
	int	*b;			/* Stack B. */
	int	n;			/* Initial number of values. */
	int	na;			/* Current size of A. */
	int	nb;			/* Current size of B. */
	int	count[11];	/* Number of each printed instruction. */
	double	disorder;	/* Initial inversion ratio. */
	int	strategy;	/* Requested strategy. */
	int	method;		/* Actual method used by adaptive. */
	int	bench;		/* Enables the benchmark report. */
} t_data;

void	error_exit(t_data *d);
int		parse_args(t_data *d, int ac, char **av);
int		is_space(char c);
int		streq(char *a, char *b);
int		parse_number(char *s, int *out);
int		count_words(char *s);
int		read_words(t_data *d, char *s, int *pos);
int		assign_ranks(t_data *d);
double	get_disorder(t_data *d);
void	sort_simple(t_data *d);
void	sort_medium(t_data *d);
void	push_back_sorted(t_data *d);
void	sort_complex(t_data *d);
void	sort_adaptive(t_data *d);
void	sa(t_data *d);
void	sb(t_data *d);
void	ss(t_data *d);
void	pa(t_data *d);
void	pb(t_data *d);
void	ra(t_data *d);
void	rb(t_data *d);
void	rr(t_data *d);
void	rra(t_data *d);
void	rrb(t_data *d);
void	rrr(t_data *d);
void	rb_silent(t_data *d);
void	rrb_silent(t_data *d);
void	print_bench(t_data *d);
void	bench_putstr(char *s);
void	bench_putnbr(int n);
void	bench_put_percent(double value);
int		is_sorted(t_data *d);
#endif
