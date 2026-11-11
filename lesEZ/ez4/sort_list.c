#include <stdlib.h>
#include "list.h"

t_list	*sort_list(t_list *lst, int (*cmp)(int, int))
{
	int	swap;
	t_list	*tmp;

	tmp = lst;
	while(lst && lst->next != NULL)
	{
		if (((*cmp)(lst->data, lst->next->data)) == 0)
		{
			swap = lst->data;
			lst->data = lst->next->data;
			lst->next->data = swap;
			lst = tmp;
		}
		else
			lst = lst->next;
	}
	lst = tmp;
	return (lst);
}
Trier une liste chaînée selon l'ordre défini par la fonction `cmp`.

1. Parcourir la liste.
2. Comparer les valeurs de deux éléments avec `cmp`.
3. Échanger les éléments s'ils sont dans le mauvais ordre.
4. Continuer jusqu'à ce que toute la liste soit triée.
5. Retourner le pointeur vers le premier élément.

Attention : conserver les doublons et utiliser `cmp` pour déterminer l'ordre du tri.

Fonctions autorisées : aucune.