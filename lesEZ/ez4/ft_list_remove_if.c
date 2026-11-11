// Passed Moulinette 2019.09.01

#include <stdlib.h>
#include "ft_list.h"

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
	if (begin_list == NULL || *begin_list == NULL)
		return;

	t_list *cur = *begin_list;

	if (cmp(cur->data, data_ref) == 0)
	{
		*begin_list = cur->next;
		free(cur);
		ft_list_remove_if(begin_list, data_ref, cmp);
	}
	else // if there is a no else, you cant pass the Moulinette, tryed 2023.09.08
	{
		cur = *begin_list;
		ft_list_remove_if(&cur->next, data_ref, cmp);
	}
}
Supprimer de la liste chaînée tous les éléments dont `data` est égal à `data_ref`.

1. Parcourir la liste.
2. Comparer chaque `data` avec `data_ref` grâce à `cmp`.
3. Si `cmp` retourne `0`, supprimer l'élément avec `free`.
4. Mettre à jour les pointeurs `next`.
5. Continuer jusqu'à la fin de la liste.

Attention : gérer aussi la suppression du premier élément et de plusieurs éléments consécutifs.

Fonction autorisée : `free`.