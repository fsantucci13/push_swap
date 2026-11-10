
#include <stddef.h>

size_t ft_strspn(const char *s, const char *accept)
{
    size_t i = 0;
    size_t j;
    int found;

    while (s[i])
    {
        j = 0;
        found = 0;
        while (accept[j])
        {
            if (s[i] == accept[j])
            {
                found = 1;
                break;
            }
            j++;
        }
        if (found == 0)
            return (i);
        i++;
    }
    return (i);
}
