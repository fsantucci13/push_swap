#include <stdio.h>

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while(s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{	
		i++;
	}
	return (s1[i] - s2[i]);
}	
Comparer deux chaînes de caractères.

1. Parcourir les deux chaînes caractère par caractère.
2. Comparer les caractères jusqu'à une différence ou la fin d'une chaîne.
3. Retourner la différence entre les deux caractères (`s1 - s2`).

Fonctions autorisées : aucune.