
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;

    if (argc == 2)
    {
        while (argv[1][i])
        {
            if ((argv[1][i] >= 'a' && argv[1][i] <= 'm')
                || (argv[1][i] >= 'A' && argv[1][i] <= 'M'))
                argv[1][i] += 13;
            else if ((argv[1][i] >= 'n' && argv[1][i] <= 'z')
                || (argv[1][i] >= 'N' && argv[1][i] <= 'Z'))
                argv[1][i] -= 13;

            write(1, &argv[1][i], 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}
Afficher la chaîne en remplaçant chaque lettre par celle située 13 positions après dans l'alphabet.

1. Vérifier qu'il y a exactement 1 argument.
2. Pour chaque lettre, avancer de 13 positions dans l'alphabet.
3. Recommencer au début après `z` ou `Z`.
4. Garder les majuscules et minuscules.
5. Ne pas modifier les autres caractères.
6. Ajouter `\n`.

Fonction autorisée : `write`.