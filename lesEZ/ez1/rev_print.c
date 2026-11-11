#include <unistd.h>

int	main(int argc, char **argv)
{
	int	i = 0;

	if (argc == 2)
	{
		while (argv[1][i])
			i += 1;
		while (i != 0)
			write(1, &argv[1][--i], 1);
	}
	write(1, "\n", 1);
	return (0);
}
Afficher la chaîne à l'envers.

1. Vérifier qu'il y a exactement 1 argument.
2. Trouver la longueur de la chaîne.
3. Parcourir la chaîne de la fin vers le début.
4. Afficher chaque caractère.
5. Ajouter `\n`.

Fonction autorisée : `write`.