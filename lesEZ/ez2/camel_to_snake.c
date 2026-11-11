
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;
    char c;

    if (argc == 2)
    {
        while (argv[1][i] != '\0')
        {
            c = argv[1][i];

            if (c >= 'A' && c <= 'Z')
            {
                write(1, "_", 1);
                c = c + 32;
            }
            write(1, &c, 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}

Convertir une chaîne `lowerCamelCase` en `snake_case`.

1. Vérifier qu'il y a exactement 1 argument.
2. Parcourir la chaîne.
3. Avant chaque majuscule, ajouter `_`.
4. Transformer chaque majuscule en minuscule.
5. Garder les minuscules inchangées.
6. Afficher le résultat suivi de `\n`.

Fonctions autorisées : `malloc`, `realloc`, `write`.