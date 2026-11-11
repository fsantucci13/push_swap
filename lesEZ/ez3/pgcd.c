#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int nbr1;
	int nbr2;

	if (argc == 3)
	{
		if ((nbr1 = atoi(argv[1])) > 0 && (nbr2 = atoi(argv[2])) > 0)
		{
			while (nbr1 != nbr2)
			{
				if (nbr1 > nbr2)
					nbr1 -= nbr2;
				else
					nbr2 -= nbr1;
			}
			printf("%d", nbr1);
		}
	}
	printf("\n");
	return (0);
}
Afficher le PGCD (plus grand diviseur commun) de deux nombres.

1. Vérifier qu'il y a exactement 2 arguments.
2. Convertir les deux nombres avec `atoi`.
3. Calculer leur PGCD avec l'algorithme d'Euclide.
4. Afficher le résultat suivi de `\n`.

Fonctions autorisées : `printf`, `atoi`, `malloc`, `free`.