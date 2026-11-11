unsigned char swap_bits(unsigned char octet)
{
	return ((octet >> 4) | (octet << 4));
}
Échanger les deux moitiés d'un octet (4 bits de gauche ↔ 4 bits de droite).

1. Récupérer les 4 bits de gauche.
2. Récupérer les 4 bits de droite.
3. Inverser leurs positions.
4. Retourner le nouvel octet.

Exemple : `01000001` devient `00010100`.

Fonctions autorisées : aucune.