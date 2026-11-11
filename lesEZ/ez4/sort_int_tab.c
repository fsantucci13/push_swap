void	sort_int_tab(int *tab, unsigned int size)
{
	unsigned int	i = 0;
	int	temp;

	while (i < (size - 1))
	{
		if (tab[i] > tab[i + 1])
		{
			temp = tab[i];
			tab[i] = tab[i+ 1];
			tab[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
}
Trier un tableau d'entiers dans l'ordre croissant.

1. Parcourir le tableau.
2. Comparer les éléments.
3. Échanger ceux qui sont dans le mauvais ordre.
4. Continuer jusqu'à ce que le tableau soit trié.
5. Conserver les doublons.

Attention : modifier directement le tableau, sans en créer un nouveau.

Fonctions autorisées : aucune.