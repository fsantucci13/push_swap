
#include <unistd.h>

int main(int argc, char **argv)
{
    int n = 0;
    int i = 2;
    int j;
    int prime;
    int sum = 0;
    int k;
    char result[12];

    if (argc == 2)
    {
        j = 0;
        while (argv[1][j] >= '0' && argv[1][j] <= '9')
        {
            n = n * 10 + argv[1][j] - '0';
            j++;
        }

        if (argv[1][j] != '\0' || j == 0)
            n = 0;
    }

    if (argc != 2)
        n = 0;

    while (i <= n)
    {
        prime = 1;
        j = 2;

        while (j < i)
        {
            if (i % j == 0)
            {
                prime = 0;
                break;
            }
            j++;
        }

        if (prime == 1)
            sum += i;

        i++;
    }

    if (sum == 0)
    {
        write(1, "0", 1);
    }
    else
    {
        k = 0;
        while (sum > 0)
        {
            result[k] = sum % 10 + '0';
            sum /= 10;
            k++;
        }

        while (k > 0)
        {
            k--;
            write(1, &result[k], 1);
        }
    }

    write(1, "\n", 1);
    return (0);
}
