#include <unistd.h>

void	ft_putnbr(int n)
{
	char digit;

	if (n >= 10)
		ft_putnbr(n / 10);
	digit = (n % 10) + '0';
	write(1, &digit, 1);
}

int	main(int argc, char **argv)
{
	(void)argv;

	ft_putnbr(argc - 1);
	write(1, "\n", 1);
	return (0);
}
Afficher le nombre d'arguments passés au programme.

1. Récupérer `argc - 1`.
2. Convertir ce nombre en caractères.
3. Afficher le résultat suivi de `\n`.

Cas particulier : aucun argument → afficher `0`.

Fonction autorisée : `write`.