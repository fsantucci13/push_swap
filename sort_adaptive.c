/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 14:56:40 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 14:56:41 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
