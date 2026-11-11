#include <unistd.h>

int main(int argc, char **argv)
{
	int i = 0;
	int j = 0;
	
	if (argc == 3)
	{
		while (argv[2][j] && argv[1][i])
		{
			if (argv[2][j] == argv[1][i])
				i++;
			j++;
		}
		if (argv[1][i] == '\0')
			write(1, "1", 1);
		else
			write(1, "0", 1);
	}
	write(1, "\n", 1);
	return (0);
}
Vérifier si le premier mot est caché dans le deuxième, en respectant l'ordre des caractères.

1. Vérifier qu'il y a exactement 2 arguments.
2. Parcourir la deuxième chaîne.
3. Chercher les caractères du premier mot dans le même ordre.
4. Si tous sont trouvés, afficher `1`.
5. Sinon, afficher `0`.
6. Ajouter `\n`.

Cas particulier : chaîne vide → afficher `1`.

Fonction autorisée : `write`.