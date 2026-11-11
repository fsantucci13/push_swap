#include <unistd.h>

void print_bits(unsigned char octet)
{
	int	i = 8;
	unsigned char 	bit;

	while (i--)
	{
		bit = (octet >> i & 1) + '0';
		write(1, &bit, 1);
	}
}
Afficher les 8 bits d'un nombre en binaire.

1. Parcourir les 8 bits du plus important au moins important.
2. Vérifier si chaque bit vaut `0` ou `1`.
3. Afficher chaque bit avec `write`.
4. Ne pas ajouter de `\n`.

Fonction autorisée : `write`.