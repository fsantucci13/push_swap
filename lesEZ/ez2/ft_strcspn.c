#include <stdio.h>

size_t  ft_strcspn(const char *s, const char *reject)
{
    int i = 0;
    int j;

    while (s[i] != '\0')
    {
		j = 0;
        while (reject[j] != '\0')
        {
            if(s[i] == reject[j])
                return (i);
            j++;
        }
        i++;
    }
    return (i);
}
Compter le nombre de caractères avant le premier caractère présent dans `reject`.

1. Parcourir la chaîne `s`.
2. Vérifier si chaque caractère existe dans `reject`.
3. S'arrêter dès qu'un caractère est trouvé.
4. Retourner le nombre de caractères parcourus.

Fonctions autorisées : aucune.