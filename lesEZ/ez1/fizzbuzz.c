#include <unistd.h>

void	putnum(int n)
{
	char	str[10] = "0123456789";

	if (n > 9)
		putnum(n / 10);
	write (1, &str[n % 10], 1);
}

int		main()
{
	int i = 1;

	while (i <= 100)
	{
		if (i % 15 == 0)
			write (1, "fizzbuzz", 8);
		else if (i % 3 == 0)
			write (1, "fizz", 4);
		else if (i % 5 == 0)
			write (1, "buzz", 4);
		else
			putnum(i);
		i++;
		write (1, "\n", 1);
	}
}
Afficher les nombres de 1 à 100, chacun sur une nouvelle ligne.
1. Si divisible par 3 → afficher fizz.
2. Si divisible par 5 → afficher buzz.
3. Si divisible par 3 et 5 → afficher fizzbuzz.
4. Sinon → afficher le nombre.
Fonction autorisée : write.