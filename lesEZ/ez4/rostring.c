#include <unistd.h>

int main(int argc, char **argv)
{
    int i;
    int start;
    int end;
    int flag;

    flag = 0;
    if (argc > 1 && argv[1][0])
    {
        i = 0;
        while (argv[1][i] == ' ' || argv[1][i] == '\t')
            i++;
        start = i;
        while (argv[1][i] != '\0' && argv[1][i] != ' ' && argv[1][i] != '\t')
            i++;
        end = i;
        while (argv[1][i] == ' ' || argv[1][i] == '\t')
            i++;
        while(argv[1][i]) 
        {
            while ((argv[1][i] == ' ' && argv[1][i + 1] == ' ') || (argv[1][i] == '\t' && argv[1][i + 1] == '\t'))
                i++; 
            if (argv[1][i] == ' ' || argv[1][i] == '\t')
                flag = 1; 
            write(1, &argv[1][i], 1);
            i++;
        }
        if (flag)
            write(1, " ", 1);
        while (start < end)
        {
            write(1, &argv[1][start], 1);
            start++;
        }
    }
    write(1, "\n", 1);
    return(0);
}
Faire tourner les mots d'une phrase d'un cran vers la gauche.

1. Vérifier qu'il y a au moins 1 argument.
2. Récupérer le premier mot.
3. Afficher tous les autres mots dans leur ordre.
4. Afficher le premier mot à la fin.
5. Mettre exactement un espace entre les mots.
6. Ajouter `\n`.

Cas particulier : un seul mot → l'afficher tel quel.

Fonctions autorisées : `write`, `malloc`, `free`.