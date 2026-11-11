
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;
    int j;

    if (argc == 3)
    {
        while (argv[1][i])
        {
            j = 0;
            while (j < i && argv[1][j] != argv[1][i])
                j++;
            if (j == i)
            {
                j = 0;
                while (argv[2][j] && argv[2][j] != argv[1][i])
                    j++;
                if (argv[2][j])
                    write(1, &argv[1][i], 1);
            }
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}
Afficher les caractères présents dans les deux chaînes, sans doublons.

1. Vérifier qu'il y a exactement 2 arguments.
2. Parcourir les deux chaînes dans l'ordre.
3. Afficher chaque caractère uniquement à sa première apparition.
4. Ne pas réafficher les caractères déjà rencontrés.
5. Ajouter `\n`.

Fonction autorisée : `write`.