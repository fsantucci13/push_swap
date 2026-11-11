unsigned int lcm(unsigned int a, unsigned int b)
{
	unsigned int n;	

	if (a == 0 || b == 0)
		return (0);
	if (a > b)
		n = a;
	else
		n = b;
	while (1)
	{
		if (n % a == 0 && n % b == 0)
			return (n);
		++n;
	}
}
Calculer le plus petit multiple commun (PPCM) de deux nombres.

1. Si `a` ou `b` vaut `0`, retourner `0`.
2. Calculer le PGCD des deux nombres (avec l'algorithme d'Euclide).
3. Calculer le PPCM à partir du PGCD.
4. Retourner le résultat.

Formule : PPCM = `(a / PGCD) * b`

Fonctions autorisées : aucune.