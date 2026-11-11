#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	int	i;
	int	number;

	if (argc == 2)
	{
		i = 1;
		number = atoi(argv[1]);
		if (number == 1)
			printf("1");
		while (number >= ++i)
		{
			if (number % i == 0)
			{
				printf("%d", i);
				if (number == i)
					break ;
				printf("*");
				number /= i;
				i = 1;
			}
		}
	}
	printf("\n");
	return (0);
}
Afficher tous les facteurs premiers d'un nombre, dans l'ordre croissant.

1. Vérifier qu'il y a exactement 1 argument.
2. Convertir l'argument avec `atoi`.
3. Tester les diviseurs à partir de `2`.
4. Si le nombre est divisible, afficher le diviseur.
5. Diviser le nombre et recommencer avec le même diviseur.
6. Séparer les facteurs avec `*`.
7. Ajouter `\n`.

Cas particulier : si le nombre vaut `1`, afficher `1`.

Fonctions autorisées : `printf`, `atoi`.