
char *ft_strpbrk(const char *s1, const char *s2)
{
    int i = 0;

    if (!s1 || !s2)
        return (0);

    while (*s1)
    {
        i = 0;
        while (s2[i])
        {
            if (*s1 == s2[i])
                return ((char *)s1);
            i++;
        }
        s1++;
    }
    return (0);
}
Trouver le premier caractère de `s1` qui apparaît aussi dans `s2`.

1. Parcourir `s1` caractère par caractère.
2. Vérifier si le caractère existe dans `s2`.
3. Si trouvé, retourner son adresse dans `s1`.
4. Si aucun caractère ne correspond, retourner `NULL`.

Fonctions autorisées : aucune.