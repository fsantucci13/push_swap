#include "ft_list.h"

int	ft_list_size(t_list *begin_list)
{
	if (begin_list == 0)
		return (0);
	else
		return (1 + ft_list_size(begin_list->next));
}
Compter le nombre d'éléments dans une liste chaînée.

1. Parcourir la liste avec le pointeur `next`.
2. Incrémenter un compteur pour chaque élément.
3. S'arrêter quand `next == NULL`.
4. Retourner le compteur.

Cas particulier : liste vide → retourner `0`.

Fonctions autorisées : aucune.