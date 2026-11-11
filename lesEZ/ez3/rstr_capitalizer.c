#include <unistd.h>

void    rstr_capitalizer(char *str)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
        if ((str[i] >= 'a' && str[i] <= 'z') && (str[i + 1] == ' ' \
                    || str[i + 1] == '\t' || str[i + 1] == '\0'))
            str[i] -= 32;
        write(1, &str[i++], 1);
    }
}

int main(int argc, char **argv)
{
    int i;

    if (argc == 1)
        write(1, "\n", 1);
    else
    {
        i = 1;
        while (i < argc)
        {
            rstr_capitalizer(argv[i]);
            write(1, "\n", 1);
            i += 1;
        }
    }
    return (0);
}
Pour chaque mot, mettre la dernière lettre en majuscule et toutes les autres en minuscule.

1. Parcourir chaque argument.
2. Mettre toutes les lettres en minuscules.
3. Mettre la dernière lettre de chaque mot en majuscule.
4. Un mot se termine par un espace, une tabulation ou la fin de chaîne.
5. Conserver les espaces et tabulations.
6. Ajouter `\n` après chaque argument.

Cas particulier : un mot d'une seule lettre devient majuscule.

Fonction autorisée : `write`.