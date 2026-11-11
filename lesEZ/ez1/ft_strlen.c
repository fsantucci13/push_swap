int ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}
Retourner la longueur d'une chaîne.
1. Parcourir la chaîne caractère par caractère.
2. Compter chaque caractère jusqu'à '\0'.
3. Retourner le nombre de caractères.
Fonctions autorisées : aucune.