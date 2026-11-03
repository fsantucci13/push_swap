*This project has been created as part of the 42 curriculum by fsantucc, mdutruel.*

# push\_swap — Documentation technique exhaustive

> Compilé avec `cc -Wall -Wextra -Werror`

---

## Table des matières

1. [Description](#description)
2. [Instructions](#instructions)
3. [Algorithmes — justification et choix](#algorithmes--justification-et-choix)
4. [Modèle de données central — t\_data](#modèle-de-données-central--t_data)
5. [Fichier par fichier — référence complète](#fichier-par-fichier--référence-complète)
   - [push\_swap.h](#push_swaph)
   - [main.c](#mainc)
   - [utils.c](#utilsc)
   - [flags.c](#flagsc)
   - [parse.c](#parsec)
   - [disorder.c](#disorderc)
   - [normalize.c](#normalizec)
   - [operations.c](#operationsc)
   - [sort\_simple.c](#sort_simplec)
   - [sort\_helpers.c](#sort_helpersc)
   - [sort\_medium.c](#sort_mediumc)
   - [sort\_complex.c](#sort_complexc)
   - [sort\_adaptive.c](#sort_adaptivec)
   - [sort\_turk.c](#sort_turkc)
   - [bench.c](#benchc)
6. [Flux d'exécution global](#flux-dexécution-global)
7. [Analyse de complexité](#analyse-de-complexité)
8. [Gestion des erreurs et cas limites](#gestion-des-erreurs-et-cas-limites)
9. [Gestion mémoire](#gestion-mémoire)
10. [Défense technique — questions 42](#défense-technique--questions-42)
11. [Resources](#resources)
12. [Contributors](#contributors)

---

## Description

### C'est quoi push\_swap en termes simples ?

Imagine que tu as une pile de cartes numérotées posées sur une table (pile A). Le but : les mettre dans l'ordre croissant (la plus petite en haut) en utilisant une deuxième pile vide (pile B) et seulement 11 opérations autorisées. Tu dois y arriver **en le moins d'opérations possible**.

Le programme imprime sur la sortie standard la liste des opérations à effectuer, une par ligne.

### Les 11 opérations autorisées

| Opération | Ce qu'elle fait |
|-----------|-----------------|
| `sa` | Échange les 2 premiers éléments de A |
| `sb` | Échange les 2 premiers éléments de B |
| `ss` | `sa` et `sb` en même temps |
| `pa` | Prend le premier élément de B, le met en haut de A |
| `pb` | Prend le premier élément de A, le met en haut de B |
| `ra` | Fait tourner A vers le haut : le premier devient le dernier |
| `rb` | Fait tourner B vers le haut : le premier devient le dernier |
| `rr` | `ra` et `rb` en même temps |
| `rra` | Fait tourner A vers le bas : le dernier devient le premier |
| `rrb` | Fait tourner B vers le bas : le dernier devient le premier |
| `rrr` | `rra` et `rrb` en même temps |

### Limites de la moulinette 42 (barème officiel)

| Taille | Excellent (5 étoiles) | Bien (3 étoiles) | Passable |
|--------|----------------------|-----------------|---------|
| n = 3  | ≤ 3 opérations | — | — |
| n = 5  | ≤ 12 opérations | — | — |
| n = 100 | ≤ 700 ops | ≤ 900 ops | ≤ 1500 ops |
| n = 500 | ≤ 5 500 ops | ≤ 8 000 ops | ≤ 12 000 ops |

Notre implémentation : **n=500 → max 5 465 ops, moyenne ~5 106 ops, 0 dépassement sur 15 000 tests.**

### Quatre stratégies implémentées

| Stratégie | Flag CLI | Utilisée quand |
|-----------|----------|----------------|
| Simple    | `--simple`   | Basique O(n²), petit n ou quasi-trié |
| Medium    | `--medium`   | Chunk sort O(n√n) |
| Complex   | `--complex`  | Tri par bits O(n log n) |
| **Adaptive** | *(défaut)* | Choisit automatiquement la meilleure |

---

## Instructions

### Compiler le projet

```bash
make          # compile → génère le binaire ./push_swap
make clean    # supprime les fichiers .o (objets intermédiaires)
make fclean   # clean + supprime le binaire push_swap
make re       # recompile tout depuis zéro (fclean puis all)
```

### Exécuter

```bash
# Trier 5 nombres
./push_swap 3 1 4 1 5

# Trier une séquence aléatoire de 500 nombres
ARG=$(shuf -i 1-500 -n 500 | tr '\n' ' ') && ./push_swap $ARG

# Compter le nombre d'opérations
ARG=$(shuf -i 1-500 -n 500 | tr '\n' ' ') && ./push_swap $ARG | wc -l

# Vérifier que le résultat est bien trié (avec un checker)
ARG=$(shuf -i 1-500 -n 500 | tr '\n' ' ')
./push_swap $ARG | ./checker $ARG    # doit afficher "OK"

# Voir les statistiques internes
./push_swap --bench 3 1 4 1 5 9 2 6
```

### Options CLI disponibles

| Option | Effet |
|--------|-------|
| `--bench` | Affiche les stats sur stderr (disorder, stratégie, nb ops) |
| `--simple` | Force la stratégie simple |
| `--medium` | Force la stratégie medium (chunk) |
| `--complex` | Force la stratégie complex (bitshift) |
| `--adaptive` | Force la stratégie adaptive (défaut) |

### Cas d'erreur → "Error\n" sur stderr

- Argument non entier (`./push_swap abc`)
- Entier hors plage INT (`./push_swap 99999999999`)
- Doublons (`./push_swap 1 2 1`)

---

## Algorithmes — justification et choix

### Pourquoi plusieurs algorithmes ?

Il n'existe pas un seul algorithme parfait pour toutes les tailles. Un algorithme optimal pour n=3 est inutilisable pour n=500. Le programme choisit donc automatiquement selon la taille et le désordre de l'entrée.

### Concept clé : le désordre (Kendall tau)

Avant de trier, on mesure **à quel point c'est déjà dans le désordre**. On compte le nombre de paires (i, j) où i < j mais a[i] > a[j] (une **inversion**). Plus il y a d'inversions, plus c'est désordonné. On divise par le nombre total de paires possible pour obtenir un score entre 0 (parfaitement trié) et 1 (ordre inverse).

```
disorder = nombre_inversions / (n * (n-1) / 2)
```

Si `disorder < 0.2`, l'entrée est quasi-triée → on utilise la stratégie simple qui est très efficace dans ce cas. Sinon, on utilise l'algorithme Turk.

### Stratégie Simple (O(n²))

**Idée :** Trouve le minimum dans A, amène-le en haut par rotations, pousse vers B. Répète. Puis ramène tout de B vers A dans l'ordre.

**Pourquoi O(n²) ?** Pour chaque élément (n fois), on peut faire jusqu'à n/2 rotations = n²/2 opérations au total.

**Quand utilisée :** n ≤ 3, n ≤ 5, ou disorder < 0.2.

### Stratégie Medium — Chunk Sort (O(n√n))

**Idée :** Normalise d'abord les valeurs (remplace les vraies valeurs par leurs rangs 0..n-1). Puis divise la plage [0..n[ en tranches ("chunks") de taille √(8n). Traite les tranches de la plus grande à la plus petite : pousse tous les éléments de la tranche en B, ramène de B vers A (en cherchant le max à chaque fois). Répète.

**Pourquoi O(n√n) ?** n/√n = √n chunks. Pour chaque chunk, on fait O(n) opérations (scan de A) + O(√n) opérations (ramener de B). Total : √n × n + n × √n = O(n√n).

### Stratégie Complex — Tri par bits (O(n log n))

**Idée :** Normalise les valeurs. Pousse vers B tous les éléments dont le bit k vaut 0, ramène ensuite. Répète pour chaque bit k de 0 à log₂(n). C'est un radix sort adapté aux piles.

**Pourquoi O(n log n) ?** log₂(n) passes × n opérations par passe.

### Stratégie Adaptive — Turk Algorithm (principal pour n=500)

C'est l'algorithme principal, inspiré du "Turkish sort" ou "greedy cost sort". Il se décompose en deux phases.

#### Phase 1 : turk\_push — pousser vers B

On pousse tous les éléments de A vers B **sauf les 2 derniers** (qu'on trie directement avec SA si nécessaire).

**Invariant maintenu dans B :** B est toujours **décroissant circulairement**. C'est-à-dire que si on "déroule" B comme un cercle, les valeurs descendent. Exemple : B = [8, 5, 3, 1] ou B = [3, 1, 9, 7] (9 est "après" 3 dans le sens circulaire).

**Pourquoi garder B décroissant ?** Parce que quand on va tout ramener en A, on pourra insérer chaque élément au bon endroit sans avoir à retrier B.

**Choix de quel élément pousser :** Pour chaque élément de A, on calcule le **coût minimal** pour l'amener en tête de A ET amener sa position cible en tête de B, puis faire PB. On choisit l'élément dont ce coût est minimum.

**Calcul du coût — les 4 combinaisons :**

Pour amener l'élément en position `p` dans A (taille `na`) et la cible en position `q` dans B (taille `nb`), il y a 4 façons :

| Combo | Opérations | Formule |
|-------|-----------|---------|
| Les 2 vers l'avant | RR simultané | max(p, q) + 1 |
| Les 2 vers l'arrière | RRR simultané | max(na-p, nb-q) + 1 |
| A avant, B arrière | séquentiel | p + (nb-q) + 1 |
| A arrière, B avant | séquentiel | (na-p) + q + 1 |

Le `+ 1` est pour l'opération PB finale. On prend le **minimum des 4**.

**Pourquoi les combos simultanés (RR/RRR) coûtent max(p,q) ?** Parce qu'un RR fait avancer les deux piles d'un cran en même temps. Si A a besoin de 3 rotations et B de 5, avec RR on fait 3 RR (avance les deux) + 2 RB (finit B seul) = 5 ops au lieu de 3+5 = 8.

#### Phase 2 : turk\_pull — ramener de B vers A

On ramène tous les éléments de B vers A. Pour chaque élément de B, on calcule la position cible dans A (où il doit s'insérer pour que A reste trié circulairement ascendant) et le coût. On choisit le moins cher, on effectue les rotations et PA.

**Invariant dans A :** A est toujours **croissant circulairement**. Exemple : A = [3, 5, 8, 1, 2] (c'est trié circulairement car 1 vient "après" 8).

#### Rotation initiale optimale

Pour n=500, la position de départ (quel élément est en tête de A) peut changer significativement le coût total. `sort_turk` simule d'abord la rotation 0. Si le résultat dépasse 5 400 ops (~2.6% des cas), il teste 3 autres rotations (n/4, n/2, 3n/4) en mode silencieux (sans rien imprimer) et choisit la meilleure.

**Mode silencieux (`d->silent`) :** Pendant les simulations, `apply_op` ne print rien sur stdout. Seulement `d->count[op]++` est fait. Après la simulation, on remet le tableau A dans son état initial (copie sauvegardée dans `orig`).

#### Résultat garanti

- n = 3 : max **2 ops** (optimal)
- n = 5 : max **11 ops** (bonus 42 = ≤ 12 ✓)
- n = 500 : max **5 465 ops** sur 15 000 tests (excellent 42 = ≤ 5 500 ✓)

---

## Modèle de données central — t\_data

Toute l'information du programme vit dans une seule structure :

```c
typedef struct s_data
{
    int    *a;            // tableau représentant la pile A (index 0 = sommet)
    int    *b;            // tableau représentant la pile B (index 0 = sommet)
    int    n;             // nombre total d'éléments (fixe)
    int    na;            // nombre d'éléments actuellement dans A
    int    nb;            // nombre d'éléments actuellement dans B
    int    count[11];     // compteur par opération (sa=0, sb=1, ..., rrr=10)
    double disorder;      // taux de désordre Kendall tau ∈ [0.0, 1.0]
    int    strategy;      // stratégie demandée (0=simple 1=medium 2=complex 3=adaptive)
    int    used_strategy; // stratégie réellement utilisée
    int    bench;         // 1 si --bench actif
    int    silent;        // 1 pendant les simulations (pas d'affichage)
}   t_data;
```

**Pourquoi des tableaux et pas de vrais "stacks" ?** Les piles sont simulées par des tableaux avec un compteur de taille. L'index 0 représente **le sommet** de la pile. Pousser = décaler tout d'un cran vers la droite et mettre le nouvel élément en [0]. Dépiler = lire [0] et décaler tout d'un cran vers la gauche.

**Pourquoi allouer avec malloc ?** La taille n'est connue qu'à l'exécution (dépend des arguments). On ne peut pas déclarer `int a[n]` dans la structure (taille variable → interdit en C89/C90).

---

## Fichier par fichier — référence complète

---

### push\_swap.h

**Rôle :** Fichier d'en-tête. Contient tous les types, constantes et déclarations de fonctions partagées par tous les fichiers .c.

**Inclusions système :**
- `<unistd.h>` : fonction `write` (pour afficher sans printf)
- `<stdlib.h>` : `malloc`, `free`, `INT_MAX`, `INT_MIN`
- `<limits.h>` : constantes `INT_MAX` et `INT_MIN`

**L'énumération des opérations :**
```c
enum e_op {SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR};
```
C assigne automatiquement SA=0, SB=1, ..., RRR=10. Ça permet d'utiliser l'opération comme index dans `count[11]` et dans le tableau de noms `names[11]`.

**Guard `#ifndef PUSH_SWAP_H` :** Évite que le fichier soit inclus deux fois dans la même unité de compilation (ce qui causerait des erreurs de redéfinition).

---

### main.c

**Rôle :** Point d'entrée du programme. Coordonne les 4 étapes : init → parse → sort → bench.

#### `init_data(t_data *d)`

Initialise tous les champs de `d` à zéro/NULL avant usage. **Obligatoire** : sans ça, les pointeurs `a` et `b` auraient des valeurs aléatoires et `free_data` plante.

```c
d->a = NULL; d->b = NULL; d->n = 0; d->na = 0; d->nb = 0;
d->disorder = 0; d->strategy = 3; d->bench = 0; d->silent = 0;
// + boucle pour mettre count[0..10] = 0
```

#### `set_used(t_data *d)`

Détermine `used_strategy` (stratégie réellement utilisée) en fonction de `strategy` demandée et du `disorder` calculé. En mode adaptive (strategy=3) : si disorder < 0.2 → simple (0), sinon → turk (3).

#### `run_sort(t_data *d)`

Dispatch selon `d->strategy` :
- 0 → `sort_simple(d)`
- 1 → `sort_medium(d)`
- 2 → `sort_complex(d)`
- 3 → `sort_adaptive(d)`

Retourne 1 si succès, 0 si erreur (échec malloc interne).

#### `main(int argc, char **argv)`

1. `init_data` — tout à zéro
2. Si `argc == 1` (aucun argument) → retourne 0 immédiatement (comportement normal selon sujet)
3. `parse_input` — lit et valide les arguments
4. `set_used` — détermine la stratégie
5. Si pas déjà trié → `run_sort`
6. Si `--bench` → `print_bench`
7. Si erreur → affiche "Error" sur stderr, retourne 1
8. `free_data` — libère la mémoire

**Pourquoi vérifier `is_sorted` avant `run_sort` ?** Pour ne rien afficher si la séquence est déjà triée (le sujet l'exige : zéro opération = solution valide).

---

### utils.c

**Rôle :** Fonctions de base d'affichage et de comparaison de chaînes. Elles remplacent les fonctions de la libc standard (non autorisées selon le sujet 42, qui interdit d'inclure `<string.h>` ou `<stdio.h>`).

#### `ft_strcmp(const char *a, const char *b)`

Compare deux chaînes caractère par caractère. Retourne 0 si identiques, une valeur ≠ 0 sinon. Utilisé pour reconnaître les flags `--bench`, `--simple`, etc.

**Pourquoi `(unsigned char)` ?** Pour éviter les comportements définis par l'implémentation avec les caractères ≥ 128 (accents, etc.).

#### `ft_putstr_fd(const char *s, int fd)`

Écrit une chaîne sur le descripteur de fichier `fd` (1 = stdout, 2 = stderr). Utilise `write` caractère par caractère pour rester simple et conforme.

#### `ft_putendl_fd(const char *s, int fd)`

Comme `ft_putstr_fd` mais ajoute `\n` à la fin. Utilisé pour imprimer chaque opération (ex : `pb\n`).

#### `ft_putnbr_fd(long n, int fd)`

Convertit un entier `long` en chaîne et l'écrit sur `fd`. Fonctionne par remplissage d'un buffer de 12 caractères en extrayant les chiffres de droite à gauche (modulo 10), puis en les écrivant à l'envers.

---

### flags.c

**Rôle :** Reconnaît et traite les options CLI (`--simple`, `--bench`, etc.).

#### `is_option(const char *s)`

Retourne 1 si `s` est un des 5 flags reconnus, 0 sinon. Utilisé dans `parse.c` pour savoir si un argument est une option ou un nombre.

#### `set_option(t_data *d, char *s, int *selected)`

Applique l'option sur `d`. La variable `selected` empêche de donner deux stratégies contradictoires (`--simple --medium` → Error). `--bench` est autorisé en combinaison avec n'importe quelle stratégie.

---

### parse.c

**Rôle :** Transforme les arguments texte en entiers valides dans `d->a`, avec toutes les validations requises.

#### `number(char *s, int *out)`

Parse un entier depuis une chaîne avec validation complète :
- Signe optionnel `+` ou `-`
- Uniquement des chiffres `0-9`
- Overflow : compare avec `INT_MAX` (ou `INT_MAX+1` pour les négatifs) en utilisant `long long` pour éviter l'overflow pendant la conversion
- Retourne 0 si invalide, 1 et remplit `*out` si valide

**Exemple :** `"2147483648"` retourne 0 (> INT_MAX). `"-2147483648"` retourne 1.

#### `append_value(t_data *d, char *arg)`

Parse un argument et l'ajoute à `d->a`. Vérifie l'absence de doublons par scan linéaire de `d->a[0..na-1]`. Retourne 0 si doublon ou nombre invalide.

#### `read_values(t_data *d, int argc, char **argv)`

Parcourt `argv[1..argc-1]`. Chaque argument est soit une option (traité par `set_option`) soit un nombre (traité par `append_value`).

#### `parse_input(t_data *d, int argc, char **argv)`

- Alloue `d->a` (taille `argc` entiers — assez large pour tous les arguments)
- Appelle `read_values`
- Fixe `d->n = d->na`
- Alloue `d->b` (même taille que A)
- Calcule `d->disorder` avec `compute_disorder`
- Retourne 1 si tout OK, 0 sinon

---

### disorder.c

**Rôle :** Mesure le désordre de A et vérifie si A est trié.

#### `compute_disorder(t_data *d)`

Compte les **inversions** (paires (i,j) avec i<j et a[i]>a[j]) en O(n²). Divise par `n*(n-1)/2` (nombre total de paires) pour normaliser en [0, 1].

- `disorder = 0.0` → déjà trié
- `disorder = 1.0` → ordre inverse (totalement inversé)
- `disorder < 0.2` → moins de 20% des paires sont inversées → quasi-trié

**Pourquoi `long long bad` ?** Pour n=500 : 500×499/2 = 124 750 paires. Tient dans un `int`, mais avec n=100 000 ce serait ~5 milliards → hors de portée de `int`. On utilise `long long` par sécurité et bonne pratique.

#### `is_sorted(t_data *d)`

Parcourt A de gauche à droite. Retourne 0 dès qu'on trouve a[i-1] > a[i]. Si on arrive à la fin sans problème, retourne 1.

#### `free_data(t_data *d)`

Libère `d->a` et `d->b` avec `free`. Pas de null-check nécessaire car `free(NULL)` est valide en C.

---

### normalize.c

**Rôle :** Remplace les valeurs réelles (ex: 42, -7, 1000000) par leurs **rangs relatifs** (0, 1, 2, ..., n-1). Nécessaire pour les stratégies medium et complex qui travaillent sur des plages [0..n[.

**Exemple :** A = [-7, 1000000, 42] → triés : [-7, 42, 1000000] → rangs : 0, 2, 1 → A devient [0, 2, 1].

**Pourquoi normaliser ?** Les algorithmes medium et complex découpent la plage de valeurs en tranches ou travaillent bit à bit. Ça n'a de sens que si les valeurs sont dans [0, n[.

#### `sift_down(int *a, int root, int end)`

Fonction interne du heap sort. Fait "descendre" un élément dans le tas jusqu'à sa position correcte (tant que l'enfant le plus grand est plus grand que lui, on échange).

#### `heap_sort(int *a, int n)`

Trie le tableau `a` en place par tas (heap sort) en O(n log n). En deux phases : construction du tas max (de n/2-1 à 0), puis extraction répétée du max.

**Pourquoi heap sort ?** O(n log n) garanti, pas de cas worst-case comme quicksort, implémentable sans bibliothèque externe.

#### `get_rank(int *sorted, int n, int value)`

Recherche binaire pour trouver l'index de `value` dans le tableau trié. Retourne l'index (qui est le rang). O(log n) par appel.

#### `normalize(t_data *d)`

1. Alloue une copie de A → `sorted`
2. Heap sort sur `sorted`
3. Remplace chaque `d->a[i]` par son rang via `get_rank`
4. Libère `sorted`
5. Retourne 0 si malloc échoue, 1 sinon

---

### operations.c

**Rôle :** Implémente les 11 opérations sur les piles. C'est le cœur mécanique du programme.

#### `swap(int *s, int n)`

Échange `s[0]` et `s[1]` via une variable temporaire `tmp`. Ne fait rien si n ≤ 1 (stack vide ou un seul élément).

#### `push(int *dst, int *nd, int *src, int *ns)`

Déplace le sommet de `src` vers le sommet de `dst` :
1. Décale tous les éléments de `dst` d'une position vers la droite (de l'index `*nd` jusqu'à 1)
2. Place `src[0]` en `dst[0]`
3. Incrémente `*nd`, décrémente `*ns`

Ne fait rien si `*ns == 0` (source vide).

**Complexité O(n) par push.** Pour n=500, 500 pushes = 250 000 décalages. Acceptable pour ce projet.

#### `rotate(int *s, int n)`

Sauvegarde `s[0]` dans `first`, décale tout vers la gauche d'une position, remet `first` en `s[n-1]`.

#### `reverse(int *s, int n)`

Sauvegarde `s[n-1]` dans `last`, décale tout vers la droite d'une position, remet `last` en `s[0]`.

#### `apply_op(t_data *d, int op)`

Fonction centrale appelée par tous les algorithmes. Applique l'opération `op` sur les piles :

```c
if (op == SA || op == SS) swap(d->a, d->na);
if (op == SB || op == SS) swap(d->b, d->nb);
if (op == PA) push(d->a, &d->na, d->b, &d->nb);
if (op == PB) push(d->b, &d->nb, d->a, &d->na);
if (op == RA || op == RR)  rotate(d->a, d->na);
if (op == RB || op == RR)  rotate(d->b, d->nb);
if (op == RRA || op == RRR) reverse(d->a, d->na);
if (op == RRB || op == RRR) reverse(d->b, d->nb);
d->count[op]++;
if (!d->silent)
    ft_putendl_fd(names[op], 1);
```

**Le flag `d->silent` :** Quand 1, l'opération est exécutée mécaniquement (modifie les tableaux, incrémente le compteur) mais rien n'est affiché. Utilisé pendant les simulations dans `sort_turk`. Quand 0 (mode normal), chaque opération est imprimée sur stdout.

**Pourquoi `count[op]++` même en mode silencieux ?** Pour pouvoir compter le coût total de la simulation et choisir la meilleure rotation initiale.

---

### sort\_simple.c

**Rôle :** Stratégie simple O(n²) et utilitaires de recherche.

#### `find_min(t_data *d)`

Parcourt A de 0 à na-1, retourne l'**index** (position) du minimum. Ne retourne pas la valeur mais la position, car on en a besoin pour calculer le nombre de rotations.

#### `bring_min(t_data *d, int pos)`

Amène l'élément à la position `pos` en haut de A par le chemin le plus court :
- Si `pos ≤ na/2` → rotations avant (RA) : `pos` rotations
- Sinon → rotations arrière (RRA) : `na - pos` rotations

**Exemple :** na=10, pos=3 → 3 RA. pos=8 → 2 RRA (10-8=2).

#### `sort_simple(t_data *d)`

1. Tant qu'il reste plus d'1 élément dans A : trouve le min, l'amène en haut, PB
2. Tant que B n'est pas vide : PA

Résultat : A contient les éléments dans l'ordre croissant.

**Pourquoi pousser les minima vers B et les récupérer ensuite ?** On pousse dans l'ordre croissant (min d'abord), donc B[0] est le min actuel, B[1] le deuxième min, etc. En faisant PA pour chacun, on remet d'abord le dernier min poussé, donc on remet dans l'ordre croissant de haut en bas.

---

### sort\_helpers.c

**Rôle :** Fonctions symétriques à `sort_simple.c` mais pour la pile B.

#### `find_max_b(t_data *d)`

Retourne l'index du maximum dans B (même logique que `find_min` mais cherche le max dans B).

#### `bring_max(t_data *d, int pos)`

Amène le max de B en tête de B par le chemin le plus court (RB ou RRB).

#### `sort_b_to_a(t_data *d)`

Vide B vers A en ramenant toujours le maximum en tête de B avant PA. Résultat : A reçoit les éléments de B dans l'ordre croissant de haut en bas (le max arrivé en dernier dans A = en haut = le plus grand).

Utilisé dans `sort_medium` pour ramener chaque chunk de B vers A.

---

### sort\_medium.c

**Rôle :** Stratégie medium — chunk sort O(n√n).

#### `chunk_size(int n)`

Calcule la taille optimale d'un chunk. Trouve le plus petit entier `size` tel que `size² ≥ 8n`. Pour n=500 : size=63. Intuition : on veut environ √(8n) ≈ 63 éléments par chunk pour n=500.

**Pourquoi 8n et pas n ?** C'est un facteur empiriquement optimal pour minimiser le nombre total d'opérations.

#### `process_range(t_data *d, int low, int high)`

Parcourt A (exactement `total = d->na` fois) et pour chaque élément :
- Si sa valeur est dans [low, high[ → PB
- Sinon → RA (continue à tourner)

Résultat : tous les éléments du range [low, high[ sont en B (dans l'ordre où ils étaient dans A), le reste est toujours dans A.

#### `push_ranges(t_data *d, int size)`

Traite les chunks de la **plus grande valeur vers la plus petite** (high = n, puis n-size, n-2×size, ...). Pour chaque chunk :
1. `process_range` → pousse le chunk en B
2. `sort_b_to_a` → ramène le chunk en A dans l'ordre croissant (max d'abord = en haut de A)

Résultat : après tous les chunks, A est trié.

#### `sort_medium(t_data *d)`

Normalise (`normalize`) puis appelle `push_ranges`. Retourne 0 si échec malloc dans normalize.

---

### sort\_complex.c

**Rôle :** Stratégie complex — radix sort par bits O(n log n).

**Principe :** Après normalisation (valeurs dans [0, n[), tri bit à bit de LSB à MSB. Pour chaque bit k (de 0 à log₂(n)) : si le bit k de a[0] vaut 0 → PB, sinon → RA. Après avoir parcouru tout A, ramène tout de B vers A avec PA. Répète pour le bit suivant.

**Pourquoi ça marche ?** C'est la propriété fondamentale du radix sort : trier de LSB à MSB produit un tri stable au total.

---

### sort\_adaptive.c

**Rôle :** Stratégie adaptive — choisit le meilleur algorithme selon la taille et le désordre.

#### `sort_three(t_data *d)`

Trie A quand na ≤ 3. Cas par cas :
- na ≤ 1 : déjà trié, rien
- na = 2 : si a[0] > a[1] → SA, sinon rien
- na = 3 : 6 cas possibles (6 permutations de 3 éléments) :

| Ordre | a b c | Opérations |
|-------|-------|-----------|
| ✓ | a<b<c | rien |
| ✗ | a>b>c | SA + RRA |
| ✗ | a>b, a<c | SA |
| ✗ | a>b, a≥c | RA |
| ✗ | a<b, a≥c | RRA + SA |
| ✗ | a<b, a<c, b≥c | RRA |

Maximum **2 opérations** pour n=3. Optimal.

**Pourquoi ces cas couvrent tout ?** Il y a exactement 6 permutations de 3 éléments distincts et chaque condition `if/else` correspond à exactement une ou plusieurs d'entre elles. Les conditions sont mutuellement exclusives et exhaustives.

#### `insert_b0(t_data *d)`

Insère `b[0]` (sommet de B) dans A à la bonne position pour maintenir A circulairement croissant. Algorithme :

1. Parcourt A pour trouver :
   - `best_pos` : position du plus grand élément de A **strictement inférieur** à b[0]
   - `max_pos` : position du maximum absolu de A
2. Si aucun élément de A n'est inférieur à b[0] (b[0] est le nouveau minimum) → insérer après le maximum (`at = (max_pos + 1) % na`)
3. Sinon → insérer après `best_pos` (`at = (best_pos + 1) % na`)
4. Amener `at` en tête de A par le chemin le plus court
5. PA

#### `sort_five(t_data *d)`

Trie A quand na ≤ 5 :
1. PB PB → pousse 2 éléments en B, il reste 3 dans A
2. `sort_three(d)` → trie les 3 restants (max 2 ops)
3. `insert_b0(d)` × 2 → réinsère les 2 éléments de B dans A au bon endroit
4. Fait pivoter A pour amener le minimum en tête

Maximum **11 opérations** pour n=5. En dessous de la limite bonus 42 (12 ops).

#### `sort_adaptive(t_data *d)`

Routing function — choisit l'algorithme :

```c
if (d->n <= 3) → sort_three
if (d->n <= 5) → sort_five
if (d->disorder < 0.2) → sort_simple  // quasi-trié
→ sort_turk                            // cas général
```

---

### sort\_turk.c

**Rôle :** Implémente l'algorithme de tri "Turk" — greedy cost-based insertion. C'est l'algorithme principal pour n > 5 avec disorder ≥ 0.2, notamment pour n=500.

#### `find_target_a(t_data *d, int v)`

Pour un élément `v` qui vient de B et va être inséré dans A, trouve la position cible dans A (l'index où `v` doit atterrir pour que A reste circulairement croissant).

Logique :
- Trouve le plus grand élément de A **strictement inférieur** à `v` → `best_pos`
- Si aucun (v est nouveau minimum) → retourne `(max_pos + 1) % na` (juste après le max = position "wrap-around")
- Sinon → retourne `(best_pos + 1) % na`

**Intuition :** Dans une liste circulaire croissante [3, 5, 8, 1, 2], pour insérer 6 : le plus grand élément < 6 est 5 en position 1, donc on insère en position 2 (juste après 5). Pour insérer 0 (nouveau min) : aucun élément < 0, donc on insère juste après le max (8, position 2) → position 3.

#### `find_target_b(t_data *d, int v)`

Pour un élément `v` qui va être poussé de A vers B, trouve la position cible dans B (pour que B reste circulairement décroissant).

Logique symétrique à `find_target_a` mais pour maintenir l'ordre décroissant :
- Si B vide → retourne 0
- Trouve le plus grand élément de B **strictement inférieur** à `v` → `best_pos`
- Si aucun (v est le nouveau minimum de B) → retourne `max_pos` (v sera inséré devant le max = ça devient le nouveau "wrap-around")
- Sinon → retourne `best_pos`

#### `cost_pair(int p, int np, int q, int nq)`

Calcule le coût minimum pour amener la position `p` (dans un stack de taille `np`) et la position `q` (dans un stack de taille `nq`) en tête de leurs stacks respectifs, **plus 1** pour l'opération finale (PB ou PA).

Les 4 combinaisons :
- `pf = p` (rotations forward pour p), `pr = np - p` (reverse), `qf = q`, `qr = nq - q`
- Combo 0 (les 2 forward, RR) : `max(pf, qf) + 1`
- Combo 1 (les 2 reverse, RRR) : `max(pr, qr) + 1`
- Combo 2 (p forward, q reverse) : `pf + qr + 1`
- Combo 3 (p reverse, q forward) : `pr + qf + 1`

Retourne le minimum des 4.

**Exemple :** p=3 dans na=10, q=7 dans nb=10 :
- pf=3, pr=7, qf=7, qr=3
- Combo 0 : max(3,7)+1 = 8
- Combo 1 : max(7,3)+1 = 8
- Combo 2 : 3+3+1 = 7 ← meilleur
- Combo 3 : 7+7+1 = 15

#### `apply_rot(t_data *d, int ps, int qs, int op_both, int op_p, int op_q)`

Applique les rotations de manière optimale quand les deux stacks tournent dans le même sens :
1. `shared = min(ps, qs)` → nombre de rotations simultanées (op_both = RR ou RRR)
2. Applique `shared` fois `op_both`
3. Applique le reste de `ps` fois `op_p`
4. Applique le reste de `qs` fois `op_q`

**Exemple :** ps=3, qs=5, op_both=RR, op_p=RA, op_q=RB → 3 RR + 0 RA + 2 RB = 5 ops (contre 3+5=8 sans partage).

#### `execute_push(t_data *d, int ai, int bt)`

Exécute le push optimal de l'élément en position `ai` de A vers la position `bt` de B :
1. Recalcule les 4 coûts (comme `cost_pair`)
2. Choisit la direction minimale (`dir`)
3. Applique les rotations selon `dir` :
   - dir=0 : `apply_rot(af, bf, RR, RA, RB)` (les 2 forward)
   - dir=1 : `apply_rot(ar, br, RRR, RRA, RRB)` (les 2 reverse)
   - dir=2 : `af` fois RA + `br` fois RRB (A forward, B reverse — séquentiel)
   - dir=3 : `ar` fois RRA + `bf` fois RB (A reverse, B forward — séquentiel)
4. PB

#### `execute_pull(t_data *d, int bi, int at)`

Symmétrique à `execute_push` pour le pull (B → A) :
1. Mêmes 4 combos mais pour bi dans B et at dans A
2. Applique les rotations optimales
3. PA

#### `turk_push(t_data *d)`

Phase 1 — pousse tous les éléments de A vers B (sauf les 2 derniers) :

```
tant que na > 2:
    pour chaque ai dans A:
        bt = find_target_b(d, a[ai])
        cost = cost_pair(ai, na, bt, nb)
        si cost < best_cost: mémoriser best_ai, best_bt, best_cost
        si best_cost == 1: break (optimal, pas mieux possible)
    execute_push(d, best_ai, best_bt)

si na==2 et a[0] > a[1]: SA
```

**Early exit `best_cost == 1` :** Si on trouve un élément qui coûte 1 op (déjà en tête de A et B est vide ou cible est en tête de B), on arrête de chercher — impossible de faire mieux.

#### `turk_pull(t_data *d)`

Phase 2 — ramène tous les éléments de B vers A :

```
tant que nb > 0:
    pour chaque bi dans B:
        at = find_target_a(d, b[bi])
        cost = cost_pair(bi, nb, at, na)
        si cost < best_cost: mémoriser best_bi, best_at, best_cost
        si best_cost == 1: break
    execute_pull(d, best_bi, best_at)
```

#### `arr_dup(int *dst, int *src, int n)`

Copie simple de tableau : `dst[i] = src[i]` pour i in [0, n[. Utilisé pour sauvegarder et restaurer l'état de A.

#### `sim_cost(t_data *d, int *orig, int rot)`

Simule un run complet en mode silencieux (`d->silent = 1`) à partir de la rotation initiale `rot` :
1. Restaure A depuis `orig`
2. Remet na=n, nb=0, count à zéro
3. Active `d->silent = 1`
4. Applique `rot` fois RA
5. `turk_push` + `turk_pull`
6. Amène le minimum en tête (rotations finales)
7. Somme tous les `count[0..10]`
8. Retourne le total

**Rien n'est imprimé.** C'est une simulation pure pour évaluer le coût.

#### `do_turk(t_data *d, int rot)`

Run réel (mode normal, `d->silent = 0`) avec rotation initiale `rot`. Même séquence que `sim_cost` mais imprime les opérations.

#### `sort_turk(t_data *d)`

Fonction principale — optimise la rotation initiale avant de lancer le tri réel :

```
1. Si déjà trié → retourne 1
2. Alloue orig = copie de A
3. Simule rotation 0 → best_ops, best_rot = 0
4. Si best_ops > 5400 (environ 2.6% des cas) :
   step = n/4
   Pour k = 1, 2, 3 :
     ops = sim_cost(d, orig, k * step)
     Si ops < best_ops → mémoriser new best
5. Restaure A depuis orig, libère orig
6. Remet na, nb, count à zéro, silent=0
7. do_turk(d, best_rot) → run réel
```

**Pourquoi seulement 3 rotations supplémentaires ?** Équilibre performance/qualité. Plus de simulations = meilleur résultat mais plus lent. Avec K=4 (rotation 0 + 3 supplémentaires), ~160ms pour n=500. Avec K=20, ~1.56s — trop lent pour la moulinette.

**Pourquoi le seuil 5400 ops ?** La limite "excellent" est 5500. Si la rotation 0 donne déjà < 5400, on est bien en dessous du seuil et les simulations supplémentaires ne sont pas nécessaires. On ne les déclenche que quand il y a un risque réel de dépasser 5500.

**Résultat mesuré :** 5 seeds × 3000 permutations = 15 000 tests → 0 dépassement de 5500, max = 5465, moyenne = 5106.

---

### bench.c

**Rôle :** Affiche des statistiques de performance sur stderr quand `--bench` est activé.

#### `strategy_name(t_data *d)`

Retourne une chaîne humainement lisible décrivant la stratégie utilisée :
- "Adaptive -> Simple O(n^2)" si adaptive a choisi simple (disorder < 0.2)
- "Adaptive -> Turk O(n^2)" si adaptive a choisi Turk
- etc.

#### `print_disorder(double disorder)`

Affiche le taux de désordre en pourcentage avec 2 décimales. Exemple : disorder=0.4567 → "45.67%". Implémenté sans `printf` : multiplie par 10000, arrondit, divise manuellement pour extraire les parties entière et décimale.

#### `print_count(const char *name, int count)`

Affiche `[bench] <name>: <count>\n` sur stderr.

#### `total_ops(t_data *d)`

Somme `count[0..10]` pour obtenir le nombre total d'opérations.

#### `print_bench(t_data *d)`

Affiche tout : disorder, stratégie, total ops, puis le détail par opération (sa, sb, ss, pa, pb, ra, rb, rr, rra, rrb, rrr).

**Exemple de sortie `--bench` :**
```
[bench] disorder: 49.87%
[bench] strategy: Adaptive -> Turk O(n^2)
[bench] total operations: 5113
[bench] sa: 0
[bench] pb: 498
[bench] pa: 498
[bench] ra: 847
[bench] rr: 623
...
```

---

## Flux d'exécution global

```
main()
 ├─ init_data()           → tout à zéro
 ├─ parse_input()
 │   ├─ malloc a[], b[]
 │   ├─ read_values()     → valide et stocke les entiers
 │   └─ compute_disorder()→ Kendall tau
 ├─ is_sorted()           → si déjà trié, fin (rien à imprimer)
 └─ run_sort()
     └─ sort_adaptive()
         ├─ n ≤ 3  → sort_three()          ← max 2 ops
         ├─ n ≤ 5  → sort_five()           ← max 11 ops
         ├─ disorder < 0.2 → sort_simple() ← O(n²) mais entrée quasi-triée
         └─ else   → sort_turk()
             ├─ sim_cost(rot=0)             ← simulation silencieuse
             ├─ si > 5400 : sim_cost(n/4, n/2, 3n/4) ← 3 simulations supp.
             └─ do_turk(best_rot)           ← run réel avec affichage
                 ├─ turk_push()             ← tous → B sauf 2 (greedy min-cost)
                 └─ turk_pull()             ← tous → A (greedy min-cost)
```

---

## Analyse de complexité

| Algorithme | Complexité opérations | Complexité mémoire | n=500 en pratique |
|------------|----------------------|-------------------|-------------------|
| sort_three | O(1) ≤ 2 ops | O(1) | — |
| sort_five  | O(1) ≤ 11 ops | O(1) | — |
| sort_simple | O(n²) | O(n) | ~125 000 ops — inutilisable |
| sort_medium | O(n√n) | O(n) | ~35 000 ops — too many |
| sort_complex | O(n log n) | O(n) | ~4 500 ops en théorie |
| sort_turk | O(n²) pire cas | O(n) | **~5 100 ops en pratique** |

**Pourquoi sort_turk (théoriquement O(n²)) bat-il sort_complex (O(n log n)) en pratique ?**

La complexité asymptotique ne dit pas tout. Pour n=500 :
- Sort_turk fait n-2 pushes + n pulls = ~1000 opérations de base, chacune optimisée avec les rotations simultanées (RR/RRR). Le coefficient constant est très petit.
- Sort_complex (radix) fait log₂(500) ≈ 9 passes de n opérations chacune = ~4500 opérations minimum, mais sans optimisation des rotations simultanées ni recherche du chemin minimal.
- Pour n=100 000, sort_complex serait largement meilleur. Pour n=500, sort_turk est empiriquement optimal.

---

## Gestion des erreurs et cas limites

| Cas | Comportement |
|-----|-------------|
| Aucun argument | Fin silencieuse (retour 0) |
| Argument non numérique | "Error\n" sur stderr, retour 1 |
| Entier hors int32 | "Error\n" (ex: 2147483648 > INT\_MAX) |
| Doublon | "Error\n" |
| Déjà trié | Aucune opération imprimée |
| n=1 | Aucune opération |
| n=2 déjà trié | Aucune opération |
| n=2 inversé | "sa\n" |
| Échec malloc | "Error\n" sur stderr |
| Deux stratégies contradictoires | "Error\n" (ex: `--simple --medium`) |

**Cas INT\_MIN = -2147483648 :** Géré correctement car le parsing utilise `long long` avec `limit = INT_MAX + 1 = 2147483648LL` pour les nombres négatifs.

---

## Gestion mémoire

Deux allocations principales dans `parse_input` :
- `d->a = malloc(sizeof(int) * argc)` — alloué avant de savoir le nombre exact de valeurs, taille conservative (argc = nbre d'args max)
- `d->b = malloc(sizeof(int) * d->n)` — alloué après avoir compté les valeurs réelles

`d->a` et `d->b` restent alloués pendant toute l'exécution (les piles changent de contenu mais pas de taille).

Une allocation temporaire dans `sort_turk` :
- `orig = malloc(d->n * sizeof(int))` — sauvegarde de A pour les simulations, libéré avant `do_turk`

Une allocation temporaire dans `normalize` :
- `sorted = malloc(sizeof(int) * d->n)` — copie de A à trier, libérée à la fin de `normalize`

`free_data` appelé dans `main` quoi qu'il arrive (chemin succès et chemin erreur).

**Fuites mémoire possibles ?** Si `malloc(b)` échoue après que `malloc(a)` ait réussi, `a` n'est pas libéré avant de retourner 0. `free_data` s'en occupe car `free(NULL)` est no-op.

---

## Défense technique — questions 42

**Q : Pourquoi deux piles et pas une seule ?**
R : La pile B est le buffer temporaire. Sans B, on ne peut que faire des rotations et des swaps sur A — insuffisant pour trier efficacement. B permet de stocker les éléments pendant qu'on réorganise A.

**Q : Pourquoi normaliser les valeurs ?**
R : Les algorithmes medium et complex découpent la plage de valeurs en tranches. Si les valeurs sont -10^9 à 10^9, les tranches seraient énormes. Après normalisation, les valeurs sont dans [0, n[, donc on peut faire des tranches de taille sqrt(n) ou travailler bit à bit sur log₂(n) bits.

**Q : Qu'est-ce que la complexité O(n²) ?**
R : Pour n éléments, dans le pire cas on effectue de l'ordre de n² opérations. Si on double n, on quadruple le nombre d'opérations. Pour n=500 : ~250 000 ops en théorie. En pratique Turk fait ~5100 car les rotations simultanées (RR) et le choix greedy réduisent drastiquement la constante.

**Q : Pourquoi sort_turk garde-t-il B circulairement décroissant ?**
R : Parce que lors du pull (B→A), on doit insérer chaque élément de B dans A. Si B est circulairement décroissant, l'ordre d'insertion naturel (depuis le sommet) insérera les éléments du plus grand au plus petit — ce qui minimise le déplacement nécessaire dans A à chaque étape.

**Q : Pourquoi les rotations simultanées (RR/RRR) font-elles des économies ?**
R : RR coûte 1 opération et avance les deux piles. Sans RR, amener position 3 dans A et position 5 dans B coûterait 3 RA + 5 RB = 8 ops. Avec RR : 3 RR + 2 RB = 5 ops. Économie maximale quand les deux positions sont proches.

**Q : Le programme peut-il sortir "Error" et quand même avoir écrit des opérations sur stdout ?**
R : Non. La validation se fait entièrement dans `parse_input` avant tout tri. Si `parse_input` retourne 0, `run_sort` n'est jamais appelé, donc aucune opération n'est imprimée.

**Q : Que se passe-t-il si malloc retourne NULL pendant sort_turk ?**
R : `sort_turk` alloue `orig` pour les simulations. Si malloc échoue, il appelle directement `do_turk(d, 0)` sans simulation — trie quand même avec rotation 0, sans optimisation. La qualité peut être légèrement moindre mais le programme ne crash pas.

**Q : Pourquoi le checker dit "KO" si les opérations trient correctement ?**
R : Le checker vérifie aussi que les opérations sont bien formées (une par ligne, sans espace, en minuscules). Toutes nos opérations sont imprimées via `ft_putendl_fd(names[op], 1)` où `names` est `{"sa", "sb", "ss", "pa", "pb", "ra", "rb", "rr", "rra", "rrb", "rrr"}` — donc format parfait.

**Q : La mémoire est-elle libérée en cas d'erreur ?**
R : Oui. `free_data` est appelé à la fin de `main` quelle que soit la branche prise. `free(NULL)` est sûr (no-op en C).

**Q : Quel est le pire cas pour n=500 ?**
R : 5465 opérations (mesuré sur 15 000 tests). Toujours en dessous de la limite "excellent" de 5500.

**Q : Comment vérifier que sort_three fait au plus 2 opérations pour n=3 ?**
R : Les 6 permutations de [a,b,c] distincts et leurs coûts :
- a<b<c : 0 ops (return immédiat)
- a<b, b>c, a>c : RRA (1 op) — car a<b, a≥c → else → RRA
- a<b, b>c, a<c : RRA+SA (2 ops)
- a>b, a<c : SA (1 op)
- a>b, b<c, a>c (⇔ a>b>c) : SA+RRA (2 ops)
- a>b, b<c, a≥c (⇔ a>b, a≥c, b<c) : RA (1 op)

Maximum : 2 opérations. ✓

---

## Resources

- **Sujet officiel push_swap 42 :** documentation interne 42 (PDF fourni par l'école)
- **Kendall tau distance :** [Wikipedia — Kendall rank correlation coefficient](https://en.wikipedia.org/wiki/Kendall_rank_correlation_coefficient)
- **Heap sort :** [Wikipedia — Heapsort](https://en.wikipedia.org/wiki/Heapsort)
- **Radix sort :** [Wikipedia — Radix sort](https://en.wikipedia.org/wiki/Radix_sort)
- **Turk algorithm (greedy stack sort) :** inspiré de l'approche décrite par la communauté 42 (Medium articles sur "Turkish sort" pour push_swap)
- **Claude (Anthropic)** a été utilisé pour la conception et l'optimisation de l'algorithme Turk (`sort_turk.c`), notamment pour la gestion des 4 combinaisons de rotations simultanées, le modèle de coût `cost_pair`, et la stratégie de simulation multi-rotation pour garantir le seuil de 5500 opérations. Les fichiers `sort_turk.c`, `sort_adaptive.c`, `push_swap.h`, `main.c`, et `operations.c` ont été développés ou significativement modifiés avec l'assistance de l'IA.

---

## Contributors

| Login | Rôle |
|-------|------|
| **fsantucc** | Conception initiale, architecture, algorithmes simple/medium/complex, parse, operations, normalize, disorder, bench |
| **mdutruel** | Co-développement, revue, tests, algorithme Turk |
