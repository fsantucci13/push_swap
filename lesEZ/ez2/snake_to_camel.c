#include <unistd.h>

int main(int argc, char **argv)
{
	int i = 0;
	
	if (argc ==2 )
	{
		while(argv[1][i] != '\0')
		{
			if (argv[1][i] == '_')
			{
				i++;
				argv[1][i] = argv[1][i]-32;
			}
			write (1, &argv[1][i], 1);
			i++;
		}
	}
	write(1, "\n", 1);
}
Convertir une chaîne `snake_case` en `lowerCamelCase`.

1. Vérifier qu'il y a exactement 1 argument.
2. Parcourir la chaîne.
3. Supprimer chaque `_`.
4. Mettre en majuscule la lettre après chaque `_`.
5. Garder la première lettre en minuscule.
6. Afficher le résultat suivi de `\n`.

Fonctions autorisées : `malloc`, `free`, `realloc`, `write`.