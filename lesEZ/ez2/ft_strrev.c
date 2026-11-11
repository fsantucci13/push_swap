
char    *ft_strrev(char *str)
{
    int     i;
    int     j;
    char    temp;

    i = 0;
    j = 0;
    while (str[j])
        j++;
    j--;

    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
    return (str);
}
Inverser une chaîne directement en mémoire (sans créer de nouvelle chaîne).

1. Trouver la longueur de la chaîne.
2. Échanger le premier caractère avec le dernier.
3. Continuer vers le centre.
4. Retourner la chaîne inversée.

Fonctions autorisées : aucune.