#include <unistd.h>

void	ft_putstr(char *str)
{
	int	i = 0;

	while (str[i])
		write(1, &str[i++], 1);
}
Afficher toute la chaîne de caractères.
1. Parcourir la chaîne caractère par caractère.
2. Afficher chaque caractère avec write.
3. S'arrêter au caractère '\0'.
Fonction autorisée : write.