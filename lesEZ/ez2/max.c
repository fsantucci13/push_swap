int max(int *tab, unsigned int len)
{
	if (len == 0)
		return (0);
	
	unsigned int result;
	unsigned int i = 0;
	
	result = tab[i];
	while(i < len)
	{
		if (result <  tab[i])
		{
			result = tab[i];
		}
		i++;
	}
	return result;
}
Trouver le plus grand nombre dans un tableau.

1. Si le tableau est vide (`len == 0`), retourner `0`.
2. Prendre le premier élément comme maximum.
3. Parcourir le reste du tableau.
4. Remplacer le maximum si un nombre plus grand est trouvé.
5. Retourner le maximum.

Fonctions autorisées : aucune.