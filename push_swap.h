<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:08:08 by fsantucc          #+#    #+#             */
/*   Updated: 2026/10/30 16:09:07 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

>>>>>>> b6bc14bf6583108bd44ccbd403cd94bc8e7a1256
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

<<<<<<< HEAD
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
=======
int			is_correct_input(char **arg);
int			get_stack_size(t_stack *st);
t_stack		*create_stack(int ac, char **av);
void		get_main_index(t_stack *sa, int size);
int			is_sorted(t_stack *st);

void		swap_move(t_stack **sa, t_stack **sb, char *choice);
void		small_sort(t_stack **st);
void		rotate_move(t_stack **sa, t_stack **sb, char *choice);
void		big_sort(t_stack **sa, t_stack **sb);

void		less_moves_sort(t_stack **sa, t_stack **sb);
int			get_lower_position(t_stack **st);
void		where_fit_in_a(t_stack **sa, t_stack **sb);
void		get_stack_positions(t_stack **st);
void		calculate_moves(t_stack **sa, t_stack **sb);

int			ft_strcmp(char *s1, char *s2);
int			absolute(int n);
long int	ft_atoi(const char *s);
void		ft_putstr(char *s);
void		free_stack(t_stack **st);
void		print_error_and_exit(t_stack **sa, t_stack **sb);

>>>>>>> b6bc14bf6583108bd44ccbd403cd94bc8e7a1256
#endif
