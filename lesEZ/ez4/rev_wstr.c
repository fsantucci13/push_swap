#include <unistd.h>

int main(int argc, char **argv)
{
	int start;
	int end;
	int i = 0;
		
	if(argc == 2)
	{   
		while(argv[1][i] != '\0')
			i++;
		while(i >= 0)
		{
			while( argv[1][i] == '\0' || argv[1][i] == ' ' || argv[1][i] == '\t')
				i--;
			end = i;
			while(argv[1][i] && argv[1][i] != ' ' && argv[1][i] != '\t')
				i--;
			start = i + 1;
			int  flag;
			flag = start;
			while(start <= end)
			{
				write (1, &argv[1][start],1);
				start++;		
			}
			if (flag !=0)
			{
				write(1, " ", 1);
			}
		}
	}
	write(1, "\n", 1);
}
Afficher les mots de la phrase dans l'ordre inverse.

1. Vérifier qu'il y a exactement 1 argument.
2. Repérer les mots séparés par des espaces ou tabulations.
3. Parcourir les mots de la fin vers le début.
4. Afficher chaque mot séparé par un espace.
5. Ajouter `\n`.

Fonctions autorisées : `write`, `malloc`, `free`.