
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int a;
    int b;

    if (argc == 4)
    {
        a = atoi(argv[1]);
        b = atoi(argv[3]);

        if (argv[2][0] == '+')
            printf("%d", a + b);
        else if (argv[2][0] == '-')
            printf("%d", a - b);
        else if (argv[2][0] == '*')
            printf("%d", a * b);
        else if (argv[2][0] == '/')
            printf("%d", a / b);
        else if (argv[2][0] == '%')
            printf("%d", a % b);
    }
    printf("\n");
    return (0);
}
Effectuer une opération mathématique entre deux entiers.

1. Vérifier qu'il y a exactement 3 arguments.
2. Convertir les 2 nombres avec `atoi`.
3. Lire l'opérateur (`+`, `-`, `*`, `/`, `%`).
4. Effectuer le calcul correspondant.
5. Afficher le résultat suivi de `\n`.

Fonctions autorisées : `atoi`, `printf`, `write`.