
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;
    int j;
    int k = 0;
    int found;
    char str[256];

    if (argc == 3)
    {
        while (argv[1][i])
        {
            j = 0;
            found = 0;
            while (j < k)
            {
                if (str[j] == argv[1][i])
                    found = 1;
                j++;
            }
            if (found == 0)
            {
                str[k] = argv[1][i];
                k++;
            }
            i++;
        }
        i = 0;
        while (argv[2][i])
        {
            j = 0;
            found = 0;
            while (j < k)
            {
                if (str[j] == argv[2][i])
                    found = 1;
                j++;
            }
            if (found == 0)
            {
                str[k] = argv[2][i];
                k++;
            }
            i++;
        }
        write(1, str, k);
    }
    write(1, "\n", 1);
    return (0);
}
