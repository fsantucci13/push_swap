char	*ft_strcpy(char *s1, char *s2)
{
	int i = 0;
	
	while (s2[i] != '\0')
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}
Copier la chaîne s2 dans s1.
1. Copier chaque caractère de s2 dans s1.
2. Copier également le '\0' final.
3. Retourner le pointeur s1.
Fonctions autorisées : aucune