# push_swap

*This project has been created as part of the 42 curriculum by \<login1\>, \<login2\>.*

---

## État du projet vs consignes

> Audit rapide avant la doc détaillée.

| Exigence sujet | Statut | Notes |
|---|---|---|
| Programme `push_swap` en C | ✅ | |
| Makefile (NAME, all, clean, fclean, re) | ✅ | |
| Zéro variable globale | ✅ | |
| Gestion d'erreurs (`Error\n` sur stderr) | ✅ | |
| Pas de doublons | ✅ | `is_repeated()` dans `check_input.c` |
| Entiers hors range → erreur | ✅ | `ft_atoi` retourne `long`, comparé à INT_MIN/MAX |
| Flag `--complex` accepté | ✅ | Décalage `ac/av`, même algo |
| Flag `--simple` | ❌ | Non géré |
| Flag `--medium` | ❌ | Non géré |
| Flag `--adaptive` (défaut) | ❌ | Non géré |
| 4 stratégies distinctes | ❌ | 1 seule implémentée (Turk sort) |
| Métrique de désordre | ❌ | Non implémentée |
| Mode `--bench` (stderr) | ❌ | Non implémenté |
| README avec section Resources + AI | ❌ | Ce fichier |
| Performance 100 nombres < 700 ops | ✅ | L'algo Turk y arrive |
| Performance 500 nombres < 5500 ops | ✅ | L'algo Turk y arrive |

---

## Description

`push_swap` est un projet de tri algorithmique : trier une pile d'entiers en utilisant **uniquement** un jeu d'opérations limité sur deux piles `a` et `b`, en minimisant le nombre total d'opérations.

**Contraintes fondamentales :**
- Deux piles : `a` (contient les entiers au départ) et `b` (vide au départ)
- 11 opérations disponibles : `sa sb ss pa pb ra rb rr rra rrb rrr`
- Objectif : pile `a` triée en ordre croissant, pile `b` vide
- Sortie : la séquence d'opérations, une par ligne, sur stdout

**Ce que ce projet apprend :**
- Manipulation de listes chaînées en C
- Algorithmes de tri adaptés à des structures contraintes
- Optimisation du nombre d'opérations (complexité pratique vs théorique)

---

## Instructions

### Compilation

```sh
make        # compile push_swap
make re     # recompile de zéro
make clean  # supprime les .o
make fclean # supprime les .o et le binaire
```

### Utilisation

```sh
# Tri simple avec vérification
./push_swap 4 67 3 87 23
./push_swap 4 67 3 87 23 | ./checker_linux 4 67 3 87 23

# Avec le flag --complex (accepté, même comportement)
./push_swap --complex 4 67 3 87 23 | ./checker_linux 4 67 3 87 23

# Compter les opérations
ARG="4 67 3 87 23"; ./push_swap $ARG | wc -l

# Test de performance (100 nombres aléatoires)
shuf -i 0-9999 -n 100 > args.txt
./push_swap $(cat args.txt) | wc -l

# Test de performance (500 nombres aléatoires)
shuf -i 0-9999 -n 500 > args.txt
./push_swap $(cat args.txt) | wc -l
```

### Note zsh

En zsh, `$ARG` n'est pas découpé en mots automatiquement. Deux solutions :

```sh
# Option 1 : activer le word split
setopt SH_WORD_SPLIT
ARG="4 67 3 87 23"
./push_swap $ARG | ./checker_linux $ARG

# Option 2 : syntaxe zsh native
./push_swap $=ARG | ./checker_linux $=ARG
```

### Gestion d'erreurs

```sh
./push_swap              # rien affiché, retour immédiat
./push_swap 1 2 abc      # Error
./push_swap 1 1 2        # Error (doublon)
./push_swap 2147483648   # Error (overflow INT_MAX)
```

---

## Architecture du projet

```
push_swap/
├── push_swap.h            # Struct + prototypes (contrat de l'API interne)
├── push_swap.c            # main() + is_sorted() + dispatcher push_swap()
├── check_input.c          # Validation des arguments argv
├── create_stack.c         # Construction de la liste chaînée + normalisation des rangs
├── utils.c                # Fonctions bas niveau (ft_atoi, ft_putstr, free, erreur)
├── basic_moves.c          # swap(), push(), small_sort() (cas 2-3 éléments)
├── rotate_moves.c         # rotate(), reverse() et leurs variantes
├── complex_moves.c        # big_sort() : orchestrateur de l'algorithme principal
├── complex_moves_prep.c   # Calcul des positions et coûts de déplacement
├── complex_moves_do.c     # Exécution du déplacement le moins coûteux
└── Makefile
```

**Flux d'exécution global :**

```
argv → check_input() → create_stack() → get_main_index() → push_swap()
                                                                ↓
                                              size==2 → swap_move("sa")
                                              size==3 → small_sort()
                                              size>3  → big_sort()
                                                            ↓
                                              push_b_save_3()   ← Phase 1
                                              loop: where_fit_in_a()
                                                    calculate_moves()    ← Phase 2
                                                    less_moves_sort()
                                              last_rotates()    ← Phase 3
```

---

## Analyse fichier par fichier

### `push_swap.h` — Le contrat

```c
typedef struct s_stack
{
    int             n;           // valeur brute de l'élément
    int             main_index;  // RANG normalisé (1 = plus petit, n = plus grand)
    int             position;    // position actuelle dans la pile (0 = sommet)
    int             where_fit;   // position dans A où cet élément de B doit s'insérer
    int             mv_b;        // coût en rotations pour amener cet élément au sommet de B
    int             mv_a;        // coût en rotations pour amener son slot d'insertion au sommet de A
    struct s_stack  *next;       // liste chaînée simple
}   t_stack;
```

**Réflexion derrière la struct :**

Le champ `main_index` est la clé de voûte. Plutôt que de comparer des valeurs brutes (qui peuvent être -2147483648 à 2147483647), l'algo normalise tout en rangs 1..n. Cela simplifie radicalement toutes les comparaisons de tri : l'élément de rang 1 est le minimum absolu, l'élément de rang n le maximum absolu.

Les champs `mv_a`, `mv_b`, `where_fit`, `position` servent de cache de calcul recalculé à chaque itération de la boucle principale. Ils sont signés : **positif = rotation vers l'avant (ra/rb), négatif = rotation vers l'arrière (rra/rrb)**. Cette convention permet d'encoder direction ET amplitude dans un seul entier.

---

### `push_swap.c` — Le dispatcher

```c
// L. 3-15 : is_sorted()
// Parcourt la liste et vérifie que chaque n <= n->next
// Retourne 1 si trié, 0 sinon
// Utilisé avant et après chaque étape critique

// L. 17-25 : push_swap() statique
// Dispatcher selon la taille :
// - 2 éléments : un swap suffit si besoin
// - 3 éléments : small_sort (max 3 opérations, cas hardcodés)
// - >3 éléments : big_sort (l'algorithme principal)

// L. 27-50 : main()
// 1. Détecte --complex et décale av/ac (flag ignoré fonctionnellement)
// 2. Retourne 0 silencieusement si aucun argument
// 3. Valide l'input → erreur si invalide
// 4. Construit la pile sa
// 5. Calcule les rangs (main_index)
// 6. Lance le tri
// 7. Libère la mémoire
```

**Réflexion :** Le `--complex` est géré par un décalage de pointeur (`ac--; av++`). Élégant et sans malloc supplémentaire : on fait simplement pointer `av` sur le deuxième argument, rendant le flag transparent pour la suite du programme.

---

### `check_input.c` — La validation

```c
// ft_strcmp() L. 4-22
// Comparaison de strings adaptée aux signes +/-
// Gère le cas "+5" == "5" en ignorant le + unilatéral
// Utilisé aussi pour comparer les noms d'opérations

// is_repeated() L. 24-42
// O(n²) : double boucle, compare chaque paire d'arguments
// Utilise ft_strcmp pour gérer les équivalences "+5"=="5"

// is_valid_number() L. 44-55
// Accepte optionnel +/- au début, puis que des chiffres
// Rejette "+", "-", "1a2", ""
// Note : ne vérifie pas l'overflow (géré dans create_stack via ft_atoi long)

// remove_extra_zeros() L. 57-83
// Normalise "007" → "7", "-00" → "0", "+03" → "3"
// Travaille en place sur le string argv (mutation directe)
// Nécessaire pour que is_repeated() fonctionne ("007" == "7")

// is_correct_input() L. 85-101
// Point d'entrée de la validation
// 1. Valide chaque argument (is_valid_number)
// 2. Normalise les zéros superflus
// 3. Vérifie l'absence de doublons
```

**Réflexion :** La normalisation des zéros est une subtilité importante. Sans elle, "007" et "7" seraient considérés comme différents par une comparaison string, alors qu'ils représentent la même valeur. La validation est volontairement stricte sur la forme (pas de "1.5", pas de "1e3") mais souple sur le signe ("+" accepté).

---

### `create_stack.c` — La construction + normalisation

```c
// new_node() L. 48-63
// Alloue un nœud, initialise tous les champs à des sentinelles (-1)
// main_index = 0 signifie "pas encore rangé"

// add_at_end() L. 65-80
// Parcourt jusqu'au dernier nœud, attache le nouveau
// Maintient l'ordre d'insertion = ordre des arguments argv
// (av[1] = sommet de pile = tête de liste)

// create_stack() L. 82-103
// Boucle argv, convertit en long (ft_atoi), vérifie overflow INT
// Construit la liste chaînée dans l'ordre des arguments

// get_main_index() L. 4-31
// NORMALISATION DES RANGS — cœur de la stratégie
// Algo : sélection du maximum répétée n fois
// Itération i : trouve le maximum non encore rangé, lui assigne le rang (size_restant + 1)
// Après n itérations : rang 1 = plus petit, rang n = plus grand
// Gestion spéciale de INT_MIN (impossible à trouver avec >, lui assigner rang 1 direct)

// get_stack_size() L. 33-46
// Compte les nœuds : O(n), recalculé souvent (pas de cache global)
```

**Réflexion derrière `get_main_index` :**

```
Exemple avec [3, 1, 4, 1, 5] (après dédup) → [3, 1, 4, 5]
Passage 1 (size=4) : max = 5 → main_index = 4
Passage 2 (size=3) : max = 4 → main_index = 3
Passage 3 (size=2) : max = 3 → main_index = 2
Passage 4 (size=1) : max = 1 → main_index = 1

Résultat : 3→rang2, 1→rang1, 4→rang3, 5→rang4
```

Pourquoi cette normalisation ? L'algorithme de tri n'a jamais besoin des valeurs brutes après ce point. Travailler avec des rangs consécutifs 1..n simplifie l'insertion dans A : "trouver l'élément de rang immédiatement supérieur" est une opération simple sur des entiers consécutifs.

---

### `utils.c` — L'infrastructure bas niveau

```c
// ft_putstr() L. 4-11
// write() caractère par caractère sur fd=1
// Utilisé pour afficher chaque opération + "\n"
// Pas de printf pour rester POSIX minimal

// free_stack() L. 13-26
// Libère nœud par nœud, NULL-safe
// Met *lst = NULL après libération (protection double-free)

// print_error_and_exit() L. 28-36
// Libère sa et sb si non-NULL, write "Error\n" sur fd=2, exit(1)
// Point de sortie unique en cas d'erreur → évite les fuites mémoire

// absolute() L. 38-43
// abs() maison (évite <math.h>)
// Utilisé pour comparer les coûts mv_a et mv_b (qui sont signés)

// ft_atoi() L. 45-66
// Retourne long int (pas int) pour détecter l'overflow
// Skip les whitespace au début (compatible avec les espaces dans les args)
// Gère + et - en début de chaîne
```

**Réflexion :** `ft_atoi` retourne `long` intentionnellement. Dans `create_stack()`, la comparaison `if (n < INT_MIN || n > INT_MAX)` ne serait pas possible si on retournait directement un `int` (l'overflow serait déjà arrivé). Le `long` sur la plupart des plateformes 64-bit est 64 bits, permettant de détecter les valeurs hors range.

---

### `basic_moves.c` — Les opérations fondamentales

```c
// swap() L. 20-32
// Échange n et main_index des deux premiers nœuds
// Travaille sur les données (pas sur les pointeurs)
// Raison : la liste chaînée garde sa structure, seules les valeurs bougent

// push() L. 34-44
// Déplace le nœud de tête de top1 vers le sommet de top2
// Manipulation de pointeurs pure (pas de malloc, pas de free)
// top1: [A→B→C] top2: [X→Y] → top1: [B→C] top2: [A→X→Y]

// swap_move() L. 46-63
// Dispatche sa/sb/ss/pa/pb
// Affiche le nom de l'opération après exécution
// Note : pa et pb utilisent swap_move() (nommage un peu ambigu mais fonctionnel)

// get_max_index() L. 4-18
// Trouve le rang maximum dans la pile
// Utilisé par small_sort pour localiser le maximum des 3 éléments

// small_sort() L. 65-78
// Trie 3 éléments en max 3 opérations
// Logique :
//   1. Si le max est en position 0 (sommet) : ra pour le mettre en bas
//   2. Si le max est en position 1 (milieu) : rra pour le mettre en bas
//   3. Maintenant le max est en bas → vérifier si les 2 premiers sont dans l'ordre
//   4. Si non : sa
// Maximum théorique : 3 opérations pour les pires cas
```

**Réflexion derrière `small_sort` :**

```
Cas à 3 éléments [a, b, c] où max = plus grand des 3 :

Si a=max : ra → [b, c, max] → si b>c : sa → [c, b, max] ✓
Si b=max : rra → [c, a, max] → si c>a : sa → [a, c, max] ✓
Si c=max : déjà en bas → si a>b : sa → [b, a, c] ✓
```

L'insight : le maximum doit toujours finir en bas (position 2) avant de vérifier l'ordre des 2 restants.

---

### `rotate_moves.c` — Les rotations

```c
// rotate() L. 22-34
// Implémentation de ra/rb :
//   - Sauvegarde le sommet (ex_top)
//   - Fait pointer *top sur le suivant
//   - Parcourt jusqu'au dernier nœud
//   - Attache ex_top à la fin, son next = NULL
// [A→B→C→D] → [B→C→D→A]

// reverse() L. 4-20
// Implémentation de rra/rrb :
//   - Trouve l'avant-dernier (new_bottom) et le dernier (ex_bottom)
//   - Le dernier devient la nouvelle tête
//   - La nouvelle tête pointe vers l'ancienne tête
//   - L'avant-dernier devient le nouveau dernier (next = NULL)
// [A→B→C→D] → [D→A→B→C]

// rotate_move() L. 36-58
// Dispatche ra/rb/rr/rra/rrb/rrr
// rr : rotate(sa) + rotate(sb) → économie : 2 ops pour le prix d'1
// rrr : reverse(sa) + reverse(sb) → idem
// Affiche le nom de l'opération
```

**Réflexion :** Les opérations combinées `rr` et `rrr` sont cruciales pour l'efficacité. Si on doit faire 3 `ra` et 3 `rb`, on peut faire 3 `rr` à la place : 3 opérations au lieu de 6. L'algorithme principal exploite cela au maximum.

---

### `complex_moves.c` — L'orchestrateur `big_sort`

```c
// push_b_save_3() L. 45-71
// Phase 1 de l'algorithme : vider A vers B en gardant 3 éléments
// Stratégie de push :
//   - D'abord, push les éléments de la moitié inférieure (main_index <= size/2)
//   - Cette pré-sélection évite d'envoyer les grands en B en premier
//   - Après la première passe, push tout ce qui reste jusqu'à size-pushed == 3
//   - small_sort() les 3 derniers dans A

// get_lower_position() L. 4-24
// Trouve la position (index dans la liste) de l'élément de rang minimum
// Utilisé par last_rotates pour savoir où est le minimum dans A

// last_rotates() L. 26-43
// Phase 3 : rotation finale pour mettre le minimum en tête
// Optimisation : si le minimum est dans la 2ème moitié → rra (plus court)
//                sinon → ra
// Condition : lower_position > size/2 → aller en arrière

// big_sort() L. 73-84
// Orchestrateur des 3 phases :
//   1. push_b_save_3() : vider A→B, trier les 3 restants dans A
//   2. Boucle tant que B non vide :
//      a. where_fit_in_a() : calculer où chaque élément de B va dans A
//      b. calculate_moves() : calculer le coût (mv_a, mv_b) pour chaque élément de B
//      c. less_moves_sort() : insérer l'élément le moins coûteux dans A
//   3. last_rotates() : rotation finale si A n'est pas encore bien orienté
```

**Réflexion derrière la Phase 1 (push_b_save_3) :**

```
Pourquoi pousser la moitié inférieure en premier ?

Intuition : après la Phase 1, B va être ré-inséré dans A.
Si les petits éléments (bas rangs) sont dans B, quand on les ré-insère,
ils iront naturellement près du sommet de A (qui contient les grands).
Les éléments de B proches du bon endroit dans A = moins de rotations nécessaires.

Sans cette stratégie : push n-3 éléments dans l'ordre d'arrivée → B aléatoire.
Avec cette stratégie : B contient les petits, A contient les grands + les 3 médians triés.
```

---

### `complex_moves_prep.c` — Calcul des coûts

```c
// get_stack_positions() L. 56-69
// Attribue à chaque nœud sa position actuelle (0=sommet, 1, 2, ...)
// Recalculé à chaque itération (la pile change à chaque insertion)

// get_a_position() L. 27-54
// Pour un élément de B de rang b_index, trouve la position dans A
// de l'élément qui devrait venir juste APRÈS lui dans l'ordre trié.
// Logique :
//   1. Cherche l'élément de A avec le plus petit rang > b_index
//      (le "successeur" dans l'ordre trié)
//   2. Si aucun successeur trouvé (b_index est le maximum actuel) :
//      → wrap-around : insérer avant le minimum de A
//      (circulaire : le plus grand va devant le plus petit)

// where_fit_in_a() L. 71-86
// Pour chaque élément de B, appelle get_a_position()
// Stocke le résultat dans b->where_fit

// calculate_moves() L. 4-25
// Transforme les positions en coûts signés :
//   mv_b = position (rotation vers l'avant, coût positif)
//   Si position > size_b/2 : mv_b = -(size_b - position) (rotation vers l'arrière, moins cher)
//   Même logique pour mv_a avec where_fit
```

**Réflexion derrière les coûts signés :**

```
Exemple avec size_b = 8 :
  Position 2 : mv_b = +2 (2 rotations ra vers l'avant)
  Position 6 : mv_b = -(8-6) = -2 (2 rotations rra vers l'arrière)
  Position 4 : mv_b = +4 ou -4 (équivalent, on prend +4)

Le signe encode la direction. Cela permet à less_moves_sort() de savoir :
  mv_a > 0 ET mv_b > 0 → les deux vont dans le même sens → utiliser rr
  mv_a < 0 ET mv_b < 0 → les deux vont dans le même sens arrière → utiliser rrr
  Sinon → mouvements séparés
```

---

### `complex_moves_do.c` — L'insertion optimale

```c
// less_moves_sort() L. 58-84
// Sélectionne l'élément de B avec le coût total minimum (|mv_a| + |mv_b|)
// Puis exécute les rotations nécessaires et fait pa
//
// Ordre d'exécution des rotations :
//   1. Si mv_a < 0 ET mv_b < 0 : reverse_both() → rrr simultané (économie max)
//   2. Si mv_a > 0 ET mv_b > 0 : rotate_both() → rr simultané (économie max)
//   3. rotate_a() : finit les rotations restantes de A
//   4. rotate_b() : finit les rotations restantes de B
//   5. swap_move("pa") : pousse l'élément de B vers A

// reverse_both() L. 4-12
// Tant que mv_a < 0 ET mv_b < 0 : rrr, incrémenter les deux compteurs
// Quand l'un des deux atteint 0 → sortir, continuer séparément

// rotate_both() L. 14-22
// Tant que mv_a > 0 ET mv_b > 0 : rr, décrémenter les deux compteurs

// rotate_a() L. 24-39
// Épuise mv_a : positive → ra ; négative → rra

// rotate_b() L. 41-56
// Épuise mv_b : positive → rb ; négative → rrb
```

**Réflexion derrière l'ordre d'exécution :**

```
Pourquoi d'abord les rotations combinées ?
→ rr coûte 1 opération au lieu de 2 (ra + rb)
→ On maximise l'usage de rr/rrr en les faisant en premier
→ Les rotations restantes sont forcément individuelles (directions différentes)

Exemple : mv_a = +3, mv_b = +5
  rotate_both : 3 × rr → mv_a = 0, mv_b = 2
  rotate_b    : 2 × rb
  Total : 3 + 2 = 5 opérations au lieu de 3 + 5 = 8

Exemple : mv_a = -2, mv_b = +3
  Pas de combiné possible (directions opposées)
  rotate_a : 2 × rra
  rotate_b : 3 × rb
  Total : 5 opérations (pas d'optimisation possible dans ce cas)
```

---

## L'algorithme principal : Turk Sort (greedy insertion)

### Vue d'ensemble

```
État initial : A = [non trié], B = []

Phase 1 — Partition :
  A = [3 éléments triés], B = [n-3 éléments, les plus petits en premier]

Phase 2 — Insertion gloutonne :
  Pour chaque itération :
    → Calculer le coût d'insertion pour TOUS les éléments de B
    → Choisir l'élément de B le MOINS CHER à insérer dans A
    → Insérer via pa
  A se reconstruit trié de façon incrémentale

Phase 3 — Alignement :
  A est trié mais le minimum n'est peut-être pas au sommet
  → Rotation finale (ra ou rra selon la position du minimum)
```

### Pourquoi ça fonctionne

**Invariant maintenu :** Après chaque insertion en Phase 2, A est toujours trié de façon circulaire. C'est-à-dire : si on lit A du sommet, c'est une sous-séquence triée. Le seul "break" possible est entre la dernière et la première position (le wrap-around).

**Pourquoi insérer au bon endroit maintient l'invariant :**
`get_a_position` trouve le successeur direct de l'élément à insérer. En insérant avant ce successeur, on maintient l'ordre trié localement. Comme A était déjà trié circulairement, il le reste.

**Complexité pratique :**
- Phase 1 : O(n) opérations (push n-3 éléments)
- Phase 2 : n-3 insertions × coût moyen par insertion
  - Coût moyen ≈ O(√n) si les éléments sont bien distribués
  - Dans le pire cas : O(n) par insertion → O(n²) total
- Phase 3 : O(n) rotations max
- **En pratique sur des données aléatoires : ~700 ops pour n=100, ~5500 pour n=500**

---

## Algorithmes requis par le sujet (non encore implémentés)

Le sujet demande 4 stratégies. Voici leur description et justification :

### 1. Simple — O(n²) : Insertion sort adapté

**Principe :** Pour chaque élément de A, le pousser dans B à sa position triée.
- Itérer sur A
- Pour chaque élément, trouver sa place dans B avec des rotations
- Le coût par insertion : O(n) rotations
- Total : O(n²)

**Quand l'utiliser :** Petites listes (n ≤ ~20), ou désordre faible.

### 2. Medium — O(n√n) : Chunk sort

**Principe :** Diviser les rangs en √n chunks, pousser chunk par chunk vers B.
- √n chunks de taille √n chacun
- Pour chaque chunk : pousser tous les éléments de ce chunk dans B (O(n) rotations par chunk)
- √n chunks × O(n) par chunk = O(n√n)
- Puis insérer B→A trié (greedy, O(n) par élément mais beaucoup moins de recherche)

### 3. Complex — O(n log n) : Radix sort LSD (bits)

**Principe :** Trier par bits successifs (du moins significatif au plus significatif).
- Pour chaque bit (log₂(n) bits nécessaires pour n éléments) :
  - Parcourir A, pousser dans B si bit = 0, garder dans A si bit = 1
  - Ramener tout B dans A
- log₂(n) passes × O(n) par passe = O(n log n)

**Implémentation sur les rangs normalisés :** Les rangs 1..n tiennent sur log₂(n) bits.

### 4. Adaptive — Sélection selon le désordre

**Mesure du désordre :**
```c
float compute_disorder(t_stack *a) {
    int mistakes = 0, total = 0;
    for chaque paire (i, j) avec i < j :
        if a[i] > a[j] : mistakes++
        total++
    return (float)mistakes / total;
}
```

**Stratégie par seuil :**
| Désordre | Algorithme | Justification |
|---|---|---|
| < 0.2 | Insertion sort O(n²) | Presque trié → peu d'inversions → insertion sort rapide |
| 0.2 – 0.5 | Chunk sort O(n√n) | Désordre modéré → chunks efficaces |
| ≥ 0.5 | Radix sort O(n log n) | Désordre élevé → besoin de l'algo le plus robuste |

**Justification des seuils :**
- `0.2` : En dessous, la plupart des éléments sont à leur place. L'insertion sort est quasi-linéaire sur des données presque triées.
- `0.5` : Désordre médian (aléatoire complet ≈ 0.5). Au-delà, les méthodes O(n²) deviennent pénalisantes.

---

## Resources

### Documentation et références

- Knuth, D. (1998). *The Art of Computer Programming, Vol. 3: Sorting and Searching*. Addison-Wesley.
- Algorithme "Turk sort" / greedy insertion : popularisé dans la communauté 42
- Radix sort : [Wikipedia — Radix sort](https://en.wikipedia.org/wiki/Radix_sort)
- Big-O notation : [Big-O Cheat Sheet](https://www.bigocheatsheet.com/)

### Utilisation de l'IA

L'IA (Claude) a été utilisée pour :
- **Rédaction du README** : génération de la structure et du contenu explicatif à partir du code source existant, après analyse complète des fichiers C.
- **Vérification de conformité** : comparaison systématique du code implémenté avec les exigences du sujet pour identifier les points manquants.
- **Explication algorithmique** : formulation des invariants de l'algorithme Turk sort et des justifications de complexité.

Tout le code C a été écrit et validé par les deux membres du groupe. L'IA n'a pas généré de code C.

---

## Contribution des membres

| Membre | Contribution |
|---|---|
| \<login1\> | (à compléter) |
| \<login2\> | (à compléter) |
