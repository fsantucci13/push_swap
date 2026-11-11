#include <unistd.h>

int main(int argc, char const *argv[])
{
    int i;
    int flag;

    if (argc == 2)
    {
        i = 0;
        while (argv[1][i] == ' ' || argv[1][i] == '\t')
            i++;
        while (argv[1][i])
        {
            if (argv[1][i] == ' ' || argv[1][i] == '\t')
                flag = 1;
            if (!(argv[1][i] == ' ' || argv[1][i] == '\t'))
            {
                if (flag)
                    write(1, "   ", 3);
                flag = 0;
                write(1, &argv[1][i], 1);
            }
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}
Afficher la chaîne avec exactement 3 espaces entre chaque mot.

1. Vérifier qu'il y a exactement 1 argument.
2. Ignorer les espaces et tabulations au début et à la fin.
3. Remplacer les espaces multiples et tabulations par 3 espaces.
4. Garder les mots dans leur ordre.
5. Ajouter `\n`.

Fonction autorisée : `write`.