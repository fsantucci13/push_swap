#include <unistd.h>

void    ft_putchar_n(char c, int i)
{
    while (i > 0)
    {
        write(1, &c, 1);
        --i;
    }
}

void    repeat_alpha(char *str)
{
    while (*str != '\0')
    {
        if (*str >= 'a' && *str <= 'z')
            ft_putchar_n(*str, *str + 1 - 'a');
        else if (*str >= 'A' && *str <= 'Z')
            ft_putchar_n(*str, *str + 1 - 'A');
        else
            write(1, str, 1);
        ++str;
    }
}

int main(int ac, char **av)
{
    if (ac == 2)
        repeat_alpha(av[1]);
    
    write(1, "\n", 1);
    return (0);
}


Afficher chaque lettre autant de fois que sa position dans l'alphabet.

1. Vérifier qu'il y a exactement 1 argument.
2. Pour chaque lettre, calculer sa position dans l'alphabet (a = 1, b = 2...).
3. Afficher la lettre autant de fois que sa position.
4. Garder les majuscules et minuscules inchangées.
5. Les caractères non alphabétiques sont affichés une seule fois.
6. Ajouter `\n`.

Fonction autorisée : `write`.