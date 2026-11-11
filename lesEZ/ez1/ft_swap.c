void	ft_swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
Échanger les valeurs de deux entiers.

1. Utiliser une variable temporaire.
2. Stocker la valeur de `*a`.
3. Mettre `*b` dans `*a`.
4. Mettre l'ancienne valeur de `*a` dans `*b`.

Fonctions autorisées : aucune.