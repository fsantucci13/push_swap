
int ft_atoi(const char *str)
{
    int i = 0;
    int signe = 1;
    int resultat = 0;

    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;

    if (str[i] == '-')
    {
        signe = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;

    while (str[i] >= '0' && str[i] <= '9')
    {
        resultat = resultat * 10 + (str[i] - '0');
        i++;
    }

    return (resultat * signe);
}
Convertir une chaîne de caractères en entier (`int`).

1. Ignorer les espaces au début.
2. Détecter le signe (`+` ou `-`).
3. Convertir les chiffres en nombre entier.
4. S'arrêter au premier caractère non numérique.
5. Retourner le résultat.

Fonctions autorisées : aucune.