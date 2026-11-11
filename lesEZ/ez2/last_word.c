#include <unistd.h>

void	last_word(char *str)
{
	int	j = 0;
	int i = 0;

	while (str[i])
	{
		if (str[i] == ' ' && str[i + 1] >= 33 && str[i + 1] <= 126)
			j = i + 1;
		i++;
	}
	while (str[j] >= 33 && str[j] <= 127)
	{
		write(1, &str[j], 1);
		j++;
	}
}

int		main(int argc, char **argv)
{
	if (argc == 2)
		last_word(argv[1]);
	write(1, "\n", 1);
	return (0);
}
Afficher le dernier mot de la chaîne.

1. Vérifier qu'il y a exactement 1 argument.
2. Ignorer les espaces et tabulations à la fin.
3. Remonter jusqu'au début du dernier mot.
4. Afficher le dernier mot.
5. Ajouter `\n`.

Fonction autorisée : `write`.