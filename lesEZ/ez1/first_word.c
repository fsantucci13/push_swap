#include <unistd.h>

int main (int argc, char **argv)
{
	int i = 0;
	
	if (argc == 2)
	{
		while (argv[1][i] == 32 || argv[1][i] == 9)
			i++;
		while ((argv[1][i] != 32 && argv[1][i] != 9) && argv[1][i])
		{
			write(1, &argv[1][i], 1);
			i++;
		}
	}
	write(1, "\n", 1);
}
Afficher le premier mot de l'argument.
1. Vérifier qu'il y a exactement 1 argument.
2. Ignorer les espaces et tabulations au début.
3. Afficher le premier mot jusqu'au prochain espace ou tabulation.
4. Ajouter \n.
Fonction autorisée : write.