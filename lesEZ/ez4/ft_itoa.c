#include <stdio.h>
#include <stdlib.h>

char *ft_itoa(int nbr) 
{
	if(nbr == -2147483648)
		return("-2147483648\0");
	int n = nbr;
	int len = 0;
	if (nbr <= 0)
	{
		len++;
    	}
	while (n) 
	{
		n /= 10;
		len++;
	}
	char *result = (char *)malloc(sizeof(char) * (len + 1));
	if (result == NULL) 
		return NULL;
	result[len] = '\0';
	if (nbr == 0)
	{
		result[0] = '0';
		return(result);
	}
	if (nbr < 0) 
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr) 
	{
		result[--len] = nbr % 10 + '0';
		nbr /= 10;
	}
	return result;
}

Convertir un entier (`int`) en chaîne de caractères (`char *`).

1. Compter le nombre de chiffres.
2. Gérer le signe négatif (`-`).
3. Allouer la mémoire avec `malloc`.
4. Convertir chaque chiffre en caractère.
5. Ajouter le `'\0'` final.
6. Retourner la chaîne.

Cas particulier : gérer `0` et `INT_MIN` (valeur entière minimale).

Fonction autorisée : `malloc`.