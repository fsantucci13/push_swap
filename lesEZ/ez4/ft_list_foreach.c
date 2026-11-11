#include <stdlib.h>
#include "ft_list.h"

void	ft_list_foreach(t_list *begin_list, void (*f)(void *))
{
	t_list *list_ptr;

	list_ptr = begin_list;
	while (list_ptr)
	{
		(*f)(list_ptr->data);
		list_ptr = list_ptr->next;
	}
}
Appliquer une fonction à chaque élément d'une liste chaînée.

1. Parcourir la liste avec `next`.
2. Pour chaque élément, appeler `f` avec `data`.
3. Continuer jusqu'à `NULL`.

Fonctions autorisées : aucune.