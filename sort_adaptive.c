/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsantucc <fsantucc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:12:16 by fsantucc          #+#    #+#             */
/*   Updated: 2026/11/01 12:25:06 by fsantucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_adaptive(t_data *d)
{
	if (d->disorder < 0.20)
	{
		d->method = 0;
		sort_simple(d);
	}
	else if (d->disorder < 0.50)
	{
		d->method = 1;
		sort_medium(d);
	}
	else
	{
		d->method = 2;
		sort_complex(d);
	}
}
