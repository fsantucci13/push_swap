int is_power_of_2(unsigned int n)
{
	if (n == 0)
		return (0);
	while (n % 2 == 0)
		n /= 2;
	return ((n == 1) ? 1 : 0);
}
Vérifier si un nombre est une puissance de 2.

1. Si `n` vaut 0, retourner `0`.
2. Diviser `n` par 2 tant qu'il est divisible par 2.
3. Si le résultat final vaut 1, retourner `1`.
4. Sinon, retourner `0`.

Fonctions autorisées : aucune.