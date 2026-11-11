
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;

    if (argc == 2)
    {
        while (argv[1][i] != '\0')
        {
            if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
                argv[1][i] = 'z' - (argv[1][i] - 'a');

            else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
                argv[1][i] = 'Z' - (argv[1][i] - 'A');

            write(1, &argv[1][i], 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}
Remplacer chaque lettre par son opposée dans l'alphabet.

1. Vérifier qu'il y a exactement 1 argument.
2. `a` devient `z`, `b` devient `y`, etc.
3. Faire pareil pour les majuscules.
4. Garder la casse et les autres caractères inchangés.
5. Ajouter `\n`.

Fonction autorisée : `write`.