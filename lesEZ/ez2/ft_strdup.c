#include <stdlib.h>

char	*ft_strdup(char *src)
{
	int	i = 0;
	int	length = 0;
	char	*strcpy;

	while (src[length])
		length++;
	strcpy = malloc(sizeof(*strcpy) * (length + 1));
	if (strcpy != NULL)
	{
		while (src[i])
		{
			strcpy[i] = src[i];
			i++;
		}
		strcpy[i] = '\0';
	}
	return (strcpy);
}
Dupliquer une chaîne dans une nouvelle zone mémoire.

1. Calculer la longueur de `src`.
2. Allouer de la mémoire avec `malloc` (`longueur + 1`).
3. Copier tous les caractères de `src`.
4. Copier le `'\0'` final.
5. Retourner le nouveau pointeur.

Fonction autorisée : `malloc`.