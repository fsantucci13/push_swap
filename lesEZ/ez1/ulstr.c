#include <unistd.h>

int		main(int	argc, char **argv)
{
	int	i = 0;

	if (argc == 2)
	{
		while (argv[1][i] != '\0')
		{
			if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
				argv[1][i] -= 32;
			else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
				argv[1][i] += 32;
			write(1, &argv[1][i], 1);
			i++;	
		}
	}
	write(1, "\n", 1);
}
Inverser la casse de chaque lettre (minuscule ↔ majuscule).

1. Vérifier qu'il y a exactement 1 argument.
2. Parcourir la chaîne.
3. Transformer les minuscules en majuscules.
4. Transformer les majuscules en minuscules.
5. Ne pas modifier les autres caractères.
6. Ajouter `\n`.

Fonction autorisée : `write`.