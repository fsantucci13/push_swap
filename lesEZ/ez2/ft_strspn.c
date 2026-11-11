
#include <stddef.h>

size_t ft_strspn(const char *s, const char *accept)
{
    size_t i = 0;
    size_t j;

    while (s[i])
    {
        j = 0;
        while (accept[j] && s[i] != accept[j])
            j++;
        if (accept[j] == '\0')
            return (i);
        i++;
    }
    return (i);
}
Compter le nombre de caractères consécutifs au début de `s` qui existent dans `accept`.

1. Parcourir `s` caractère par caractère.
2. Vérifier si chaque caractère existe dans `accept`.
3. S'arrêter dès qu'un caractère n'est pas accepté.
4. Retourner le nombre de caractères validés.

Fonctions autorisées : aucune.