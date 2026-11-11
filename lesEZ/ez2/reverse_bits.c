unsigned char	reverse_bits(unsigned char octet)
{
	int		i = 8;
	unsigned char	res = 0;

	while (i > 0)
	{
		res = res * 2 + (octet % 2);
		octet = octet / 2;
		i--;
	}
	return (res);
}
Inverser l'ordre des 8 bits d'un octet.

1. Lire les bits de droite à gauche.
2. Construire un nouvel octet avec les bits dans l'ordre inverse.
3. Retourner le nouvel octet.

Exemple : `00100110` devient `01100100`.

Fonctions autorisées : aucune.