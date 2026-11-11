#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;

    if (argc == 4 && argv[2][1] == '\0' && argv[3][1] == '\0')
    {
        while (argv[1][i]!='\0')
        {
            if (argv[1][i]==argv[2][0])
            {
                argv[1][i] = argv[3][0];
            }
            write (1, &argv[1][i], 1);
            i++;
        }
    }
    write (1, "\n", 1);
}
Remplacer un caractère précis par un autre dans une chaîne.

1. Vérifier qu'il y a exactement 3 arguments.
2. Parcourir la première chaîne.
3. Remplacer chaque occurrence du 2e argument par le 3e.
4. Si le caractère n'existe pas, afficher la chaîne sans modification.
5. Ajouter `\n`.

Fonctions autorisées : `write`, `exit`.