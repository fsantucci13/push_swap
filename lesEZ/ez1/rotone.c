
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;

    if (argc == 2)
    {
        while (argv[1][i])
        {
            if ((argv[1][i] >= 'a' && argv[1][i] <= 'y')
                || (argv[1][i] >= 'A' && argv[1][i] <= 'Y'))
                argv[1][i]++;
            else if (argv[1][i] == 'z')
                argv[1][i] = 'a';
            else if (argv[1][i] == 'Z')
                argv[1][i] = 'A';

            write(1, &argv[1][i], 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}
Afficher la chaîne en remplaçant chaque lettre par la suivante dans l'alphabet.

1. Vérifier qu'il y a exactement 1 argument.
2. Remplacer chaque lettre par la suivante.
3. `z` devient `a` et `Z` devient `A`.
4. Garder les majuscules et minuscules.
5. Ne pas modifier les autres caractères.
6. Ajouter `\n`.

Fonction autorisée : `write`.