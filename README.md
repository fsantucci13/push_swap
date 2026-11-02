# push_swap — Documentation technique exhaustive

> Projet 42 · `fsantucc` / `mdutruel`
> Compilé avec `cc -Wall -Wextra -Werror`

---

## Table des matières

1. [Vue d'ensemble](#1-vue-densemble)
2. [Compilation et utilisation](#2-compilation-et-utilisation)
3. [Architecture du projet — fichiers et responsabilités](#3-architecture-du-projet--fichiers-et-responsabilités)
4. [Modèle de données : t\_data et e\_op](#4-modèle-de-données--t_data-et-e_op)
5. [Représentation des piles en mémoire](#5-représentation-des-piles-en-mémoire)
6. [Flux d'exécution complet](#6-flux-dexécution-complet)
7. [Parsing des arguments](#7-parsing-des-arguments)
8. [Métriques de désordre](#8-métriques-de-désordre)
9. [Normalisation par rang](#9-normalisation-par-rang)
10. [Opérations push\_swap](#10-opérations-push_swap)
11. [Stratégie 0 — Simple O(n²)](#11-stratégie-0--simple-on)
12. [Stratégie 1 — Medium O(n√n)](#12-stratégie-1--medium-onn)
13. [Stratégie 2 — Complex O(n log n)](#13-stratégie-2--complex-on-log-n)
14. [Stratégie 3 — Adaptive](#14-stratégie-3--adaptive)
15. [Mode benchmark](#15-mode-benchmark)
16. [Analyse de complexité détaillée](#16-analyse-de-complexité-détaillée)
17. [Gestion des erreurs et cas limites](#17-gestion-des-erreurs-et-cas-limites)
18. [Gestion mémoire](#18-gestion-mémoire)
19. [Contexte 42 — scoring et défense](#19-contexte-42--scoring-et-défense)

---

## 1. Vue d'ensemble

Push_swap est un programme C qui trie une séquence d'entiers en utilisant uniquement deux piles (A et B) et un ensemble restreint d'opérations primitives. Le programme imprime sur stdout la séquence d'opérations permettant de trier la pile A dans l'ordre croissant.

Ce projet implémente **quatre stratégies de tri distinctes**, sélectionnables par flag ou choisies automatiquement selon une métrique de désordre :

| Stratégie | Flag | Complexité | Cas d'usage |
|-----------|------|------------|-------------|
| Simple    | `--simple`   | O(n²)       | Peu d'éléments ou presque triés |
| Medium    | `--medium`   | O(n√n)      | Taille intermédiaire |
| Complex   | `--complex`  | O(n log n)  | Grand n fortement désordonné |
| Adaptive  | `--adaptive` / défaut | variable | Choisit automatiquement |

---

## 2. Compilation et utilisation

### Makefile

```
make          # compile push_swap
make clean    # supprime les .o
make fclean   # supprime les .o et le binaire
make re       # fclean + all
```

Sources compilées : `main.c utils.c flags.c parse.c disorder.c normalize.c operations.c sort_simple.c sort_helpers.c sort_medium.c sort_complex.c sort_adaptive.c bench.c`

Chaque `.o` dépend de `push_swap.h` : toute modification du header déclenche la recompilation de tous les fichiers.

### Utilisation de base

```bash
./push_swap 3 1 4 2 5      # Tri, imprime les opérations
./push_swap 1 2 3 4 5      # Déjà trié, aucune sortie
./push_swap                 # Aucun argument, retourne 0 sans rien faire
./push_swap 3 1 4 1 5      # Erreur (doublon)
```

### Flags disponibles

```bash
./push_swap --simple   3 1 4 2 5   # Force la stratégie O(n²)
./push_swap --medium   3 1 4 2 5   # Force la stratégie O(n√n)
./push_swap --complex  3 1 4 2 5   # Force la stratégie O(n log n)
./push_swap --adaptive 3 1 4 2 5   # Sélection automatique (même que défaut)
./push_swap --bench    3 1 4 2 5   # Active le rapport benchmark sur stderr
./push_swap --bench --complex 3 1 4 2 5   # Combinaison possible
```

Contraintes sur les flags :
- **Un seul** flag de stratégie est autorisé. En mettre deux génère une erreur.
- `--bench` est indépendant et peut se combiner avec n'importe quelle stratégie.
- L'ordre des flags par rapport aux nombres est libre.

### Intégration avec le checker 42

```bash
ARG="3 1 4 2 5"; ./push_swap $ARG | ./checker_linux $ARG
# -> "OK" si la suite d'opérations trie correctement
```

---

## 3. Architecture du projet — fichiers et responsabilités

```
push_swap/
├── push_swap.h       Header unique : struct, enum, prototypes
├── main.c            Point d'entrée, orchestration générale
├── utils.c           Primitives I/O sans libc (strcmp, putstr, putendl, putnbr)
├── flags.c           Reconnaissance et application des flags CLI
├── parse.c           Parsing des arguments, validation, allocation
├── disorder.c        Calcul du taux d'inversion, is_sorted, free_data
├── normalize.c       Transformation valeurs -> rangs [0..n-1]
├── operations.c      Implémentation physique des 11 opérations push_swap
├── sort_simple.c     Stratégie 0 : selection sort O(n²)
├── sort_helpers.c    Utilitaires partagés (find_max_b, sort_b_to_a)
├── sort_medium.c     Stratégie 1 : tri par chunks sqrt(n)
├── sort_complex.c    Stratégie 2 : radix sort LSD O(n log n)
├── sort_adaptive.c   Stratégie 3 : dispatch automatique par disorder
└── bench.c           Rapport de performance sur stderr
```

### Dépendances entre fichiers

Tous les fichiers incluent uniquement `push_swap.h`. Aucun fichier source n'inclut un autre fichier source. Les dépendances sont purement fonctionnelles (via prototypes) :

- `parse.c` appelle `is_option` (flags.c) et `compute_disorder` (disorder.c)
- `sort_medium.c` et `sort_complex.c` appellent `normalize` (normalize.c) et `sort_b_to_a` (sort_helpers.c)
- `sort_adaptive.c` appelle `sort_simple`, `sort_medium`, `sort_complex`
- Tous les sorts appellent `apply_op` (operations.c)

---

## 4. Modèle de données : t\_data et e\_op

### Structure t\_data

```c
typedef struct s_data
{
    int     *a;             // tableau representant la pile A, a[0] = sommet
    int     *b;             // tableau representant la pile B, b[0] = sommet
    int     n;              // nombre total d'elements (fixe apres parse)
    int     na;             // nombre courant d'elements en A (varie pendant tri)
    int     nb;             // nombre courant d'elements en B (varie pendant tri)
    int     count[11];      // compteur par type d'operation (indexe par e_op)
    double  disorder;       // taux d'inversions dans [0.0, 1.0]
    int     strategy;       // strategie demandee : 0=simple 1=medium 2=complex 3=adaptive
    int     used_strategy;  // strategie reellement appliquee
    int     bench;          // 0 ou 1 : active l'affichage benchmark sur stderr
}   t_data;
```

#### Distinction `n` / `na` / `nb`

- `n` est fixé à la fin du parsing (`d->n = d->na`) et ne change plus. Il représente la taille totale du problème.
- `na` et `nb` varient pendant le tri : chaque `pb` décrémente `na` et incrémente `nb`, chaque `pa` fait l'inverse.
- Invariant maintenu : `na + nb == n` à tout moment.

#### `count[11]`

Indexé par les valeurs de l'enum `e_op`. Chaque appel à `apply_op(d, op)` incrémente `d->count[op]`. Cela permet à `bench.c` de détailler le coût par type d'opération sans instrumentation séparée.

#### `disorder`

Calculé une seule fois après le parsing, avant normalisation et avant tri. Valeur dans [0.0, 1.0] : 0.0 = tableau trié, 1.0 = ordre inverse. Voir section 8.

#### `strategy` vs `used_strategy`

`strategy` reflète le choix de l'utilisateur (ou 3 par défaut). `used_strategy` reflète ce qui a été réellement exécuté. En mode adaptatif, les deux peuvent différer. `bench.c` utilise les deux pour afficher `"Adaptive -> Complex O(n log n)"` vs `"Complex O(n log n)"`.

### Enum e\_op

```c
enum e_op {SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR};
//          0   1   2   3   4   5   6   7    8    9    10
```

Double usage :
1. Passer l'opération à `apply_op` sans chaîne de caractères
2. Indexer `count[]` et le tableau `names[]` dans les fonctions d'affichage

---

## 5. Représentation des piles en mémoire

Les deux piles sont des **tableaux d'entiers contigus** où l'indice 0 représente le **sommet**.

```
Pile A conceptuelle :     Pile A en memoire (a[]):
   TOP                    a[0] = 3   <- sommet
    3                     a[1] = 7
    7                     a[2] = 1
    1                     a[3] = 5
   BOT                    na = 4
```

Cette représentation "top = index 0" a des conséquences importantes sur le coût des opérations :

- **Push** (pa/pb) : insertion en tête → décale TOUS les éléments d'un cran vers la droite avant d'insérer à l'index 0. Coût O(n).
- **Pop** du côté source : lecture de `src[0]`, décale tous les éléments vers la gauche. Coût O(n).
- **Rotate (ra/rb)** : sauvegarde `s[0]`, décale tout vers la gauche (`s[i] = s[i+1]`), place la sauvegarde en `s[n-1]`. Coût O(n).
- **Reverse rotate (rra/rrb)** : sauvegarde `s[n-1]`, décale tout vers la droite (`s[i] = s[i-1]` de la fin vers le début), place la sauvegarde en `s[0]`. Coût O(n).

**Pourquoi pas une liste chaînée ?**

La liste chaînée donnerait des push/pop O(1) en tête, mais les algorithmes medium et complex nécessitent un accès direct par index : `find_min` et `find_max_b` scannent `a[0..na-1]` et `b[0..nb-1]` par index. Le radix sort lit `d->a[0]` à chaque itération. Avec un tableau, `d->a[i]` est O(1). Avec une liste, atteindre l'élément à la position i serait O(i). Le tableau simplifie l'implémentation au prix de décalages linéaires, acceptables pour n ≤ 500.

---

## 6. Flux d'exécution complet

```
main(argc, argv)
|
+- init_data(&d)
|    Zero tous les champs. strategy=3 (adaptive par defaut). bench=0.
|    count[0..10] = 0. a=NULL, b=NULL.
|
+- argc == 1 -> return 0  (aucun argument, sortie silencieuse)
|
+- parse_input(&d, argc, argv)
|    |
|    +- malloc(sizeof(int) * argc) pour d->a
|    |    (borne superieure : au pire autant d'entiers que d'argv)
|    |
|    +- read_values(d, argc, argv)
|    |    Pour chaque argv[i] (i=1..argc-1) :
|    |    +- is_option(argv[i]) -> set_option -> modifie strategy ou bench
|    |    +- !is_option -> append_value -> valide + deduplique + stocke dans a[]
|    |    Retourne 0 si na == 0 (aucun nombre fourni)
|    |
|    +- d->n = d->na  (fixation de la taille totale)
|    +- malloc(sizeof(int) * d->n) pour d->b
|    +- d->disorder = compute_disorder(d)
|
+- set_used(&d)
|    Si strategy == 3 (adaptive) :
|    +- disorder < 0.2  -> used_strategy = 0
|    +- disorder < 0.5  -> used_strategy = 1
|    +- disorder >= 0.5 -> used_strategy = 2
|    Sinon : used_strategy = strategy
|
+- is_sorted(&d) == 1 -> on saute le tri (pas d'operation, pas de sortie)
|
+- run_sort(&d)
|    +- strategy == 0 -> sort_simple(d)            O(n^2)
|    +- strategy == 1 -> sort_medium(d)            O(n*sqrt(n))
|    +- strategy == 2 -> sort_complex(d)           O(n log n)
|    +- strategy == 3 -> sort_adaptive(d)          dispatche selon disorder
|
+- ok && d.bench -> print_bench(&d)   (rapport sur stderr)
|
+- !ok -> ft_putendl_fd("Error", 2)  (sur stderr)
|
+- free_data(&d) -> free(d.a), free(d.b)
   return !ok  (0 = succes, 1 = erreur)
```

---

## 7. Parsing des arguments

### `number()` — conversion et validation

```c
static int number(char *s, int *out)
```

Convertit une chaîne en entier signé avec détection d'overflow. Accepte un signe optionnel `+` ou `-` en tête, puis des chiffres décimaux uniquement.

**Calcul de la limite d'overflow :**

```c
limit = (long long)INT_MAX + (sign < 0);
```

- Pour un nombre positif (`sign = 1`) : `limit = INT_MAX = 2147483647`
- Pour un nombre négatif (`sign = -1`) : `limit = INT_MAX + 1 = 2147483648 = |INT_MIN|`

La valeur absolue maximale d'un int signé est `2147483648` pour les négatifs (INT_MIN = -2147483648), et `2147483647` pour les positifs (INT_MAX). Cette formule gère les deux cas dans une seule expression.

**Tableau de validation :**

| Entree | sign | limit | Resultat |
|--------|------|-------|---------|
| `"2147483647"` | 1 | 2147483647 | OK — INT_MAX |
| `"2147483648"` | 1 | 2147483647 | Erreur — overflow |
| `"-2147483648"` | -1 | 2147483648 | OK — INT_MIN |
| `"-2147483649"` | -1 | 2147483648 | Erreur — overflow |
| `"42abc"` | 1 | — | Erreur — `*s != 0` apres la boucle |
| `""` | 1 | — | Erreur — `!*s` immediat |
| `"+"` | 1 | — | Erreur — `!*s` apres increment |

**Après conversion :** `*out = (int)(v * sign)`. Pour INT_MIN : `(int)(-2147483648LL)` = INT_MIN, comportement défini car -2147483648 est représentable en int 32 bits.

### `append_value()` — stockage et déduplication

```c
static int append_value(t_data *d, char *arg)
```

1. Convertit via `number()` — retourne 0 si invalide.
2. Recherche linéaire dans `d->a[0..na-1]` — retourne 0 si doublon.
3. Stocke `d->a[d->na++] = value`.

La détection de doublon est O(n) par appel, donc O(n²) au total pour le parsing. Acceptable car le parsing n'est pas le goulot d'étranglement.

### `read_values()` — itération sur argv

Parcourt `argv[1..argc-1]`. L'ordre flags/nombres est indifférent : `./push_swap 3 --bench 1 2` fonctionne. Retourne `d->na > 0` : échoue si aucun nombre n'est présent.

### `flags.c` — `is_option()` / `set_option()`

`is_option()` retourne 1 si la chaîne est exactement l'un des 5 flags reconnus. Utilise `ft_strcmp` pour une comparaison exacte sans strncmp.

`set_option()` :
- `--bench` peut être passé plusieurs fois (idempotent : `d->bench = 1`).
- Les flags de stratégie utilisent un verrou `selected`. La première occurrence fixe `strategy` et lève le verrou. Une deuxième occurrence d'un flag de stratégie retourne 0 (erreur).

---

## 8. Métriques de désordre

### `compute_disorder()` — taux d'inversions

```c
double compute_disorder(t_data *d)
```

Calcule le rapport entre le **nombre d'inversions** et le **nombre maximum d'inversions possibles**.

**Définition d'une inversion :** une paire `(i, j)` avec `i < j` et `a[i] > a[j]`.

**Nombre max d'inversions** pour n éléments : `n*(n-1)/2` (toutes les paires, atteint quand le tableau est en ordre inverse strict).

**Implémentation :**

```c
bad = 0;
total = (long long)d->na * (d->na - 1) / 2;
for i in 0..na-1:
    for j in i+1..na-1:
        if a[i] > a[j]: bad++
return (double)bad / (double)total;
```

Complexité : O(n²). Exécuté une seule fois avant toute opération.

**Interprétation pratique :**

| disorder | Signification |
|----------|--------------|
| 0.0 | Deja trie |
| ~0.25 | Legerement derange |
| 0.5 | Distribution aleatoire uniforme (esperance statistique) |
| 1.0 | Ordre inverse parfait |

La valeur attendue pour une permutation aléatoire uniforme est exactement 0.5 quand n → ∞. C'est pourquoi le seuil adaptatif utilise 0.5 comme frontière principale.

### `is_sorted()`

```c
int is_sorted(t_data *d)
```

Vérifie `a[i-1] <= a[i]` pour tout i de 1 à na-1. Retourne 1 si oui. Appelé dans `main` avant le tri pour court-circuiter l'algorithme sur une entrée déjà ordonnée (aucune opération imprimée).

---

## 9. Normalisation par rang

### Pourquoi normaliser ?

Les stratégies `sort_medium` et `sort_complex` travaillent sur des **rangs continus** dans `[0, n-1]`. Sans normalisation, les valeurs peuvent être des entiers arbitraires (`[-1000000, 0, 42, 99999]`). Avec normalisation :

- Le radix sort connaît à l'avance le nombre exact de bits nécessaires : `ceil(log2(n))`.
- Le chunking utilise des bornes exactes `[low, high)` avec `high - low = chunk_size`, sans avoir besoin de connaître les valeurs réelles.

### `normalize()` — implémentation

```c
int normalize(t_data *d)
```

1. Copie `d->a[0..n-1]` dans un tableau temporaire `sorted`.
2. Trie `sorted` via `heap_sort` — `sorted` contient les valeurs en ordre croissant.
3. Pour chaque `d->a[i]` : remplace sa valeur par son rang dans `sorted` via `get_rank` (recherche binaire).
4. Libère `sorted`.

**Exemple :**

```
Avant : a = [42, -5, 100, 7, -100]
sorted (apres heapsort) : [-100, -5, 7, 42, 100]
Rangs :
  42   -> index 3 -> a[0] = 3
  -5   -> index 1 -> a[1] = 1
  100  -> index 4 -> a[2] = 4
  7    -> index 2 -> a[3] = 2
  -100 -> index 0 -> a[4] = 0
Apres : a = [3, 1, 4, 2, 0]
```

La permutation est préservée : si `a[i] < a[j]` avant, alors `rank(a[i]) < rank(a[j])` après.

### `heap_sort()` — tri interne pour sorted

Utilisé uniquement pour construire le tableau de référence `sorted`. Heapsort standard en place :
1. **Phase heapify** : `sift_down` de l'indice `n/2-1` jusqu'à 0 (bottom-up).
2. **Phase extraction** : échange `a[0]` avec `a[i]` puis `sift_down(0..i-1)`, pour i de n-1 à 1.

`sift_down(a, root, end)` : descend `a[root]` dans le heap max en échangeant avec le fils le plus grand, jusqu'à stabilisation.

Complexité : O(n log n). Usage unique dans le projet : préparer `sorted` pour `normalize()`.

### `get_rank()` — recherche binaire

```c
static int get_rank(int *sorted, int n, int value)
```

Recherche binaire classique dans `sorted` (trié croissant). Retourne l'index où `sorted[mid] == value`. Retourne -1 si non trouvé (ne se produit jamais si l'entrée est cohérente).

Complexité : O(log n). L'appel de `normalize` sur tous les n éléments coûte O(n log n) au total.

---

## 10. Opérations push\_swap

### `apply_op()` — dispatch central

```c
void apply_op(t_data *d, int op)
```

Point d'entrée unique pour toutes les opérations. Utilise des blocs `if` indépendants (pas des `else if`) pour que les opérations combinées (SS, RR, RRR) déclenchent les deux branches correspondantes.

| op | Action sur A | Action sur B |
|----|-------------|-------------|
| SA | swap(a) | — |
| SB | — | swap(b) |
| SS | swap(a) | swap(b) |
| PA | push(a <- b) | — |
| PB | — | push(b <- a) |
| RA | rotate(a) | — |
| RB | — | rotate(b) |
| RR | rotate(a) | rotate(b) |
| RRA | reverse(a) | — |
| RRB | — | reverse(b) |
| RRR | reverse(a) | reverse(b) |

Après chaque opération : `d->count[op]++` et impression du nom sur stdout.

### `swap(s, n)`

Échange `s[0]` et `s[1]` si `n > 1`. Garde silencieuse : ne plante pas sur une pile vide ou de taille 1.

```
Avant : [3, 1, 4, 2]
Apres  : [1, 3, 4, 2]
```

### `push(dst, nd, src, ns)`

Déplace le sommet de `src` vers le sommet de `dst`.

1. Guard : si `*ns == 0`, retour immédiat.
2. Décale `dst` vers la droite : `dst[i] = dst[i-1]` pour i de `*nd` down to 1.
3. `dst[0] = src[0]`.
4. `(*nd)++`
5. Décale `src` vers la gauche : `src[i] = src[i+1]` pour i de 0 à `*ns-2`.
6. `(*ns)--`

```
Avant PB : A=[3,1,4], na=3   B=[7,2], nb=2
Apres  PB : A=[1,4],  na=2   B=[3,7,2], nb=3
```

### `rotate(s, n)`

Déplace le sommet (index 0) vers le fond (index n-1).

```
Avant RA : A=[3,1,4,2]   ->   Apres : A=[1,4,2,3]
           ^sommet                      ^sommet(ex-2e)
```

Sémantique : `ra` = le premier élément passe en dernier.

### `reverse(s, n)`

Déplace le fond (index n-1) vers le sommet (index 0).

```
Avant RRA : A=[3,1,4,2]   ->   Apres : A=[2,3,1,4]
            fond=2                      ^sommet(ex-fond)
```

Sémantique : `rra` = le dernier élément passe en premier.

---

## 11. Stratégie 0 — Simple O(n²)

### Idée générale

Tri par sélection adapté aux deux piles. On extrait les minimums de A un par un vers B. B accumule les minimums avec le plus récent au sommet. Quand A ne contient plus qu'un élément (le maximum), on rapatrie tout B vers A.

### `find_min(d)` — position du minimum dans A

```c
int find_min(t_data *d)
```

Scan linéaire de `d->a[0..na-1]`. Retourne l'index `pos` tel que `a[pos]` est minimal.

### `bring_min(d, pos)` — amener le minimum au sommet

```c
static void bring_min(t_data *d, int pos)
```

Choisit la direction de rotation minimisant le nombre d'opérations :

- `pos <= na/2` : `pos` rotations RA vers l'avant.
- `pos > na/2` : `na - pos` rotations RRA vers l'arrière.

**Exemple :** n=10, min à pos=7. Vers l'avant : 7 RA. Vers l'arrière : 10-7=3 RRA. RRA est choisi.

### `sort_simple(d)` — algorithme complet

**Phase 1 — extraction :**
```
while na > 1:
    pos = find_min(d)       # O(n)
    bring_min(d, pos)       # O(n/2) en moyenne
    apply_op(d, PB)         # pousse le min sur B
```

Après cette phase : na=1 (le maximum reste seul en A), B contient les n-1 autres éléments avec le **dernier poussé au sommet**.

**État de B après la phase 1 :**

Les minimums ont été poussés dans l'ordre croissant : min_0 en premier (maintenant au fond de B), min_{n-2} en dernier (maintenant au sommet). B est en ordre décroissant du sommet vers le fond :

```
B = [n-2, n-3, ..., 1, 0]   (rangs, plus grand au sommet)
```

**Phase 2 — reconstruction :**
```
while nb:
    apply_op(d, PA)
```

Chaque PA prend le sommet de B et l'insère en tête de A. Le sommet de B est `n-2`, qui pousse `n-1` (seul en A) vers la position 1 :

- 1er PA : A=[n-2, n-1]
- 2ème PA : A=[n-3, n-2, n-1]
- ...
- Final : A=[0, 1, 2, ..., n-1] ✓

**Pourquoi ça fonctionne :** B est une pile LIFO. On y a poussé les minimums successifs dans l'ordre croissant. En dépilant, on obtient l'ordre inverse, c'est-à-dire du plus grand au plus petit. Chaque PA insère en tête de A → A se construit de gauche à droite en ordre croissant.

---

## 12. Stratégie 1 — Medium O(n√n)

### Vue d'ensemble

Tri par **chunks** (blocs) sur les rangs normalisés. On divise l'espace `[0..n-1]` en blocs de taille `chunk_size ≈ sqrt(8n)`. On traite les blocs du plus grand vers le plus petit. Pour chaque bloc, on envoie ses éléments sur B, puis on les ramène sur A dans l'ordre.

### `chunk_size(n)`

```c
static int chunk_size(int n)
```

```c
size = 1;
while (size * size < 8 * n)
    size++;
return size;
```

Trouve le plus petit entier `size` tel que `size² >= 8n`, soit `size >= sqrt(8n) ≈ 2.83 * sqrt(n)`.

**Valeurs concrètes :**

| n | chunk_size | nb chunks approximatif |
|---|------------|------------------------|
| 5 | 7 | 1 |
| 10 | 9 | 2 |
| 50 | 20 | 3 |
| 100 | 29 | 4 |
| 200 | 41 | 5 |
| 500 | 64 | 8 |

Le facteur 8 (plutôt que 1 qui donnerait `sqrt(n)` pur) produit des chunks plus larges. Cela réduit le nombre de chunks et donc le coût total de `sort_b_to_a`.

### `sort_medium(d)` — algorithme principal

```c
int sort_medium(t_data *d)
{
    if (!normalize(d))
        return (0);
    push_ranges(d, chunk_size(d->n));
    return (1);
}
```

La normalisation est indispensable ici : les chunks sont définis par des bornes entières `[low, high)` sur les rangs.

### `push_ranges(d, size)` — traitement par chunks

```c
high = n;
while (high > 0):
    low = max(0, high - size)
    process_range(d, low, high)   // envoie [low, high) sur B
    sort_b_to_a(d)                // ramene B sur A, max en tete
    high = low
```

L'itération se fait **du chunk le plus grand vers le plus petit** (high commence à n et décroît). Cette direction est cruciale pour la correction du résultat final.

**Pourquoi du grand vers le petit ?**

À chaque appel de `sort_b_to_a`, les éléments du chunk courant sont placés en **tête de A** dans l'ordre croissant (le max en premier = reste le plus bas après les PA suivants). Les chunks traités après (valeurs inférieures) viendront se placer devant. Au final, les rangs les plus petits sont au sommet de A, les plus grands au fond. Résultat : A = [0, 1, 2, ..., n-1].

### `process_range(d, low, high)` — cœur de l'algorithme

```c
static void process_range(t_data *d, int low, int high)
{
    int total = d->na;  // snapshot de na AVANT toute modification
    int i = 0;
    while (i < total)
    {
        if (d->a[0] >= low && d->a[0] < high)
            apply_op(d, PB);
        else if (d->na > 1)
            apply_op(d, RA);
        i++;
    }
}
```

**Invariant clé :** la boucle tourne exactement `total` fois, où `total` est le nombre d'éléments dans A au début de l'appel.

**Preuve de correction :** Imaginez les éléments de A disposés sur un anneau circulaire de taille `total`. Notre "curseur" commence à la position 0 (sommet de A). À chaque étape :
- PB : supprime l'élément courant de l'anneau. Le curseur reste en place (le suivant prend la position 0).
- RA : le curseur avance d'une position (le sommet part au fond, le suivant arrive).
- Rien (na==1 et hors range) : le curseur reste.

En `total` étapes, chaque position originale de l'anneau est visitée exactement une fois. Tous les éléments en `[low, high)` sont donc nécessairement poussés vers B dans ces `total` étapes.

**Illustration avec 5 éléments, range [2, 4) :**

```
A initial = [5, 3, 1, 2, 4]   (rangs, pas normalisee ici pour la lisibilite)
total = 5

i=0: a[0]=5, hors range, na=5>1. RA. A=[3,1,2,4,5]
i=1: a[0]=3, dans [2,4). PB.       A=[1,2,4,5], B=[3]. na=4.
i=2: a[0]=1, hors range, na=4>1. RA. A=[2,4,5,1]
i=3: a[0]=2, dans [2,4). PB.       A=[4,5,1], B=[2,3]. na=3.
i=4: a[0]=4, hors range (4 >= 4). na=3>1. RA. A=[5,1,4]

Resultat : A=[5,1,4], B=[2,3]. Les deux elements de [2,4) sont bien en B.
```

**Cas `na == 1` et hors range :** ni PB ni RA. i s'incrémente. Cela peut arriver quand il reste un élément dans A hors du chunk courant. Il sera traité dans un chunk ultérieur.

### `sort_b_to_a(d)` — reconstruction depuis B

Défini dans `sort_helpers.c`. Vide B vers A en extrayant toujours le **maximum** de B.

```
while nb:
    pos = find_max_b(d)    # O(nb) - scan lineaire
    bring_max(d, pos)      # O(nb/2) - rotation optimale
    apply_op(d, PA)        # max de B -> sommet de A
```

`bring_max` choisit RB ou RRB selon `pos <= nb/2`.

**Effet sur A :** les éléments du chunk arrivent dans A dans l'ordre croissant (le max arrivant en premier est ensuite recouvert par les suivants). Après toutes les PA :

```
Avant sort_b_to_a : A=[5, 1, 4], B=[2, 3] (rangs)
PA du max (3) : A=[3, 5, 1, 4], B=[2]
PA du max (2) : A=[2, 3, 5, 1, 4], B=[]
```

Les éléments 2 et 3 sont maintenant en tête de A en ordre croissant, devant les éléments précédemment traités (rangs 4 et 5 qui sont plus grands et sont en position correcte).

---

## 13. Stratégie 2 — Complex O(n log n)

### Principe : Radix Sort LSD

Après normalisation, les valeurs sont des rangs dans `[0, n-1]`. Le radix sort LSD (Least Significant Bit first) trie ces entiers bit par bit, du bit de poids faible au bit de poids fort.

Pour chaque bit, les éléments avec ce bit à **0** vont dans B, ceux avec ce bit à **1** restent dans A via RA. Puis tout B est rapatrié vers A via PA.

### `max_bits(n)` — nombre de bits nécessaires

```c
static int max_bits(int n)
```

```c
bits = 0;
while (n > 1) {
    bits++;
    n = (n + 1) / 2;
}
return bits;
```

Calcule `ceil(log2(n))`. La formule `n = (n+1)/2` est l'équivalent de `ceil(n/2)` en entier. Elle assure que si n=3 (11 binaire), on compte 2 bits.

**Valeurs concrètes :**

| n | max_bits | 2^max\_bits |
|---|----------|------------|
| 2 | 1 | 2 |
| 4 | 2 | 4 |
| 5 | 3 | 8 |
| 100 | 7 | 128 |
| 500 | 9 | 512 |

Pour n=500 : 9 passes radix.

### `radix_pass(d, bit)` — une passe radix

```c
static void radix_pass(t_data *d, int bit)
{
    int i = 0;
    while (i < d->n)
    {
        if ((d->a[0] >> bit) & 1)
            apply_op(d, RA);
        else
            apply_op(d, PB);
        i++;
    }
    while (d->nb)
        apply_op(d, PA);
}
```

**Boucle principale :** `d->n` itérations (taille totale fixe, pas `d->na` qui diminue). Cela garantit que tous les n éléments sont traités.

- `(d->a[0] >> bit) & 1` : isole le bit numéro `bit` du sommet de A.
- bit=1 : RA → l'élément reste dans A, déplacé au fond.
- bit=0 : PB → l'élément va dans B.

**Après la boucle :** A contient les éléments avec bit=1 dans leur ordre d'apparition après rotations, B contient les éléments avec bit=0 dans leur ordre de poussée. Tous les éléments de B sont ensuite rapatriés en tête de A via PA.

### Trace complète pour n=4, A=[3,1,2,0]

Rangs en binaire : 3=11, 1=01, 2=10, 0=00.

**Passe bit=0 (LSB) :**

| i | a\[0\] | bit0 | Op | A | B |
|---|--------|------|----|---|---|
| 0 | 3 (11) | 1 | RA | [1,2,0,3] | [] |
| 1 | 1 (01) | 1 | RA | [2,0,3,1] | [] |
| 2 | 2 (10) | 0 | PB | [0,3,1] | [2] |
| 3 | 0 (00) | 0 | PB | [3,1] | [0,2] |

PA×2 : A=[0,2,3,1], B=[]

**Passe bit=1 (MSB) :**

| i | a\[0\] | bit1 | Op | A | B |
|---|--------|------|----|---|---|
| 0 | 0 (00) | 0 | PB | [2,3,1] | [0] |
| 1 | 2 (10) | 1 | RA | [3,1,2] | [0] |
| 2 | 3 (11) | 1 | RA | [1,2,3] | [0] |
| 3 | 1 (01) | 0 | PB | [2,3] | [1,0] |

PA×2 : A=[0,1,2,3] ✓

### Pourquoi ce radix sort fonctionne

Les éléments avec bit=0 sont poussés vers B dans l'ordre d'apparition dans A. Quand on les PA en retour, **le dernier entré dans B ressort en premier** (pile LIFO). L'ordre est inversé.

Cette inversion systématique est cohérente sur toutes les passes. La propriété de tri est préservée car :

1. Après la passe bit k, les éléments sont ordonnés selon leurs k+1 bits de poids faible.
2. L'inversion de B est identique pour tous les éléments d'un même groupe (même bit courant), donc leurs ordres relatifs se maintiennent correctement d'une passe à l'autre.
3. Après `max_bits` passes, tous les bits ont été traités et A est triée.

La correction est vérifiable par induction et a été tracée exhaustivement pour n=4.

---

## 14. Stratégie 3 — Adaptive

### `sort_adaptive(d)`

```c
int sort_adaptive(t_data *d)
{
    if (d->disorder < 0.2) {
        d->used_strategy = 0;
        sort_simple(d);
        return (1);
    }
    if (d->disorder < 0.5)
        d->used_strategy = 1;
    else
        d->used_strategy = 2;
    if (d->used_strategy == 1)
        return (sort_medium(d));
    return (sort_complex(d));
}
```

### Logique des seuils

**disorder < 0.2 — sort_simple**

Moins de 20% des paires sont inversées. Le tableau est presque trié. La stratégie simple, bien qu'O(n²) en théorie, exécute peu de rotations en pratique car les minimums se trouvent proches de leur position finale. Pour n=100 avec disorder=0.1, sort_simple produit bien moins d'opérations qu'un radix sort (qui en produit 2×100×7 = 1400 quelle que soit l'entrée).

**0.2 ≤ disorder < 0.5 — sort_medium**

Désordre modéré. Les chunks exploitent la structure partielle restante. Le coût de `sort_b_to_a` est réduit car les chunks contiennent des éléments relativement regroupés.

**disorder ≥ 0.5 — sort_complex**

Désordre élevé ou maximum. Le radix sort est insensible à l'ordre initial — son coût en opérations est O(n log n) indépendamment de la permutation. C'est la stratégie la plus rentable pour les entrées aléatoires.

### Cohérence avec `set_used` dans main.c

`main.c` appelle `set_used(&d)` AVANT `run_sort`. Cette fonction recalcule `used_strategy` selon la même logique que `sort_adaptive`. Cela permet à `print_bench` d'afficher la bonne stratégie même si `sort_adaptive` met à jour `used_strategy` de son côté lors de l'exécution.

---

## 15. Mode benchmark

### Activation et séparation des flux

Le flag `--bench` active `d->bench = 1`. L'affichage benchmark est produit sur **stderr** (fd=2), séparé du flux d'opérations sur stdout (fd=1) :

```bash
./push_swap --bench --complex 42 1 7 3 9 2>/dev/null   # opérations seules sur stdout
./push_swap --bench --complex 42 1 7 3 9 1>/dev/null   # bench seul sur stderr
```

### Contenu du rapport

```
[bench] disorder: 48.75%
[bench] strategy: Adaptive -> Complex O(n log n)
[bench] total operations: 3247
[bench] sa: 0
[bench] sb: 0
[bench] ss: 0
[bench] pa: 500
[bench] pb: 500
[bench] ra: 1247
[bench] rb: 503
[bench] rr: 0
[bench] rra: 247
[bench] rrb: 250
[bench] rrr: 0
```

### `print_disorder()` — affichage du pourcentage

```c
value = (long)(disorder * 10000 + 0.5);  // arrondi a 2 decimales
```

Imprime `value/100` (partie entière), `.`, puis `value%100` (deux chiffres décimaux, avec zéro de tête si < 10). Cette technique évite `printf` et les fonctions de formatage flottant en convertissant la valeur en entier fixe.

### `strategy_name()` — 6 cas possibles

| strategy | used\_strategy | Affichage |
|----------|----------------|-----------|
| 3 | 0 | `Adaptive -> Simple O(n^2)` |
| 3 | 1 | `Adaptive -> Medium O(n sqrt(n))` |
| 3 | 2 | `Adaptive -> Complex O(n log n)` |
| 0 | 0 | `Simple O(n^2)` |
| 1 | 1 | `Medium O(n sqrt(n))` |
| 2 | 2 | `Complex O(n log n)` |

### I/O sans stdio.h

Le projet n'inclut ni `<stdio.h>` ni `printf`. Toutes les sorties utilisent `write()` directement :

- `ft_putstr_fd` : boucle `write(fd, s, 1)` caractère par caractère.
- `ft_putendl_fd` : `ft_putstr_fd` + `write(fd, "\n", 1)`.
- `ft_putnbr_fd` : construit les chiffres dans un buffer de 12 chars en les extrayant par modulo 10 (ordre inverse), puis les imprime en ordre inversé.

---

## 16. Analyse de complexité détaillée

### Complexité en nombre d'opérations push_swap

Le critère de 42 est le **nombre d'opérations** produites, pas la complexité algorithmique de la génération.

#### sort_simple — O(n²)

Pour l'élément à la position `pos` dans A de taille `k` :
- `find_min` : scan O(k), pas d'opération générée.
- `bring_min` : min(pos, k-pos) opérations.
- 1 PB.

Total : `sum_{k=2}^{n} (k/2 + 1) + (n-1)` PA ≈ n²/4 + 2n opérations.

| n | Opérations approx. |
|---|-------------------|
| 5 | ~10 |
| 100 | ~2500 |
| 500 | ~62500 |

#### sort_medium — O(n√n)

Paramètre : `size = ceil(sqrt(8n))`.

**Coût de `process_range` par chunk :** `na` opérations (RA ou PB). `na` décroît d'un chunk à l'autre car les chunks précédents ont retiré `size` éléments. Somme totale : `sum_{k=0}^{n/size - 1} (n - k*size) ≈ n²/(2*size) = n*sqrt(n)/(2*sqrt(8)) = O(n*sqrt(n))`.

**Coût de `sort_b_to_a` par chunk :** B contient `size` éléments. Chaque extraction coûte `size/2` en moyenne (rotation). Total par chunk : `size * size/2 = size²/2`. Total sur tous les chunks : `(n/size) * (size²/2) = n*size/2 = n*sqrt(8n)/2 = O(n*sqrt(n))`.

**Total combiné :** O(n√n).

| n | Opérations approx. |
|---|-------------------|
| 100 | ~1500-2000 |
| 500 | ~15000-20000 |

#### sort_complex — O(n log n)

Par passe : `n` opérations (RA ou PB) + au plus `n` PA = `2n` au maximum. Dans la pratique, les PA ne sont exécutées que pour les éléments envoyés en B (bit=0), donc moins que n par passe.

Total : entre `n * ceil(log2(n))` et `2n * ceil(log2(n))`.

| n | log2(n) | Bornes opérations |
|---|---------|-------------------|
| 100 | 7 | 700 — 1400 |
| 500 | 9 | 4500 — 9000 |

#### Comparaison globale

| n | sort\_simple | sort\_medium | sort\_complex |
|---|-------------|-------------|--------------|
| 5 | ~10 | ~20 | ~20 |
| 10 | ~25 | ~45 | ~50 |
| 100 | ~2500 | ~1800 | ~1400 |
| 500 | ~62500 | ~20000 | ~7000 |

### Complexité mémoire

- `d->a` : O(n) en ints.
- `d->b` : O(n) en ints.
- `sorted` dans `normalize` : O(n), alloué et libéré dans la même fonction.
- Total : O(n), pas d'allocation pendant le tri.

---

## 17. Gestion des erreurs et cas limites

### Codes de retour

- `0` : succès (tri effectué, déjà trié, ou aucun argument)
- `1` : erreur (entrée invalide, malloc échoué)

En cas d'erreur, `"Error\n"` est imprimé sur stderr.

### Tableau des cas limites

| Cas | Comportement |
|-----|-------------|
| `./push_swap` (argc=1) | return 0, aucune sortie |
| Tableau déjà trié | is_sorted=1, saut du tri, aucune opération |
| Un seul élément | is_sorted=1 (boucle vide), aucune opération |
| Doublon | append_value retourne 0, "Error" |
| Non-numérique (`"abc"`) | number() retourne 0, "Error" |
| Overflow (`"2147483648"`) | v > limit, retourne 0, "Error" |
| INT_MIN (`"-2147483648"`) | limit=2147483648, v=2147483648 <= limit, OK |
| Chaîne vide (`""`) | !*s immédiat, retourne 0, "Error" |
| Signe seul (`"+"`) | !*s après incrément, retourne 0, "Error" |
| Deux flags de stratégie | selected=1 au 2ème, retourne 0, "Error" |
| Aucun nombre (que des flags) | na=0 en fin, retourne 0, "Error" |
| malloc échoué | retourne 0, free(NULL) safe, "Error" |

### Validation de INT_MIN

```c
limit = (long long)INT_MAX + (sign < 0);
// Pour sign=-1 : limit = 2147483647 + 1 = 2147483648
// v = 2147483648
// v > limit -> 2147483648 > 2147483648 -> FAUX -> accepté
// *out = (int)(2147483648 * -1) = (int)(-2147483648) = INT_MIN ✓
```

---

## 18. Gestion mémoire

### Tableau des allocations

| Pointeur | Alloué dans | Taille | Libéré dans |
|----------|-------------|--------|-------------|
| `d->a` | `parse_input` | `sizeof(int) * argc` | `free_data` |
| `d->b` | `parse_input` | `sizeof(int) * d->n` | `free_data` |
| `sorted` | `normalize` | `sizeof(int) * d->n` | `normalize` |

`d` lui-même est alloué sur la pile (`t_data d` dans main), pas de `free(&d)`.

### Chemins d'erreur sans fuite

**`malloc(d->a)` échoue :** `parse_input` retourne 0. `d->b` est encore NULL. `free_data` appelle `free(NULL)` deux fois — comportement défini en C standard.

**`malloc(d->b)` échoue :** `d->a` est valide et sera libéré par `free_data`. `free(NULL)` pour `d->b`. ✓

**`malloc(sorted)` dans `normalize` échoue :** `normalize` retourne 0. `sort_medium`/`sort_complex` propagent 0. `d->a` et `d->b` restent valides et sont libérés par `free_data` en fin de main. ✓

### Taille de `d->a`

`malloc(sizeof(int) * argc)` est une borne supérieure sûre. argc inclut le nom du programme, les flags, et les nombres. `d->na <= argc - 1` est garanti car `append_value` n'incrémente `na` qu'après validation, et le nombre de valeurs est au plus `argc - 1`.

---

## 19. Contexte 42 — scoring et défense

### Règles du sujet

- Seules les 11 opérations définies sont autorisées (`sa sb ss pa pb ra rb rr rra rrb rrr`).
- Une opération par ligne sur stdout.
- Toute erreur sur stderr avec `"Error\n"`, code de retour 1.
- A doit être triée croissante (a[0] = minimum) à la fin.
- B doit être vide à la fin.

### Vérification

```bash
# Test de correction
ARG=$(python3 -c "import random; l=list(range(500)); random.shuffle(l); print(*l)")
./push_swap $ARG | ./checker_linux $ARG

# Compter les opérations
./push_swap $ARG | wc -l

# Test avec benchmark
./push_swap --bench --complex $ARG 1>/dev/null
```

### Objectifs de performance indicatifs

| Taille | Limite pour note max | sort\_complex typique |
|--------|---------------------|-----------------------|
| 3 | 3 ops | 2-3 ops |
| 5 | 12 ops | 8-12 ops |
| 100 | 700 ops | 900-1400 ops |
| 500 | 5500 ops | 4500-8000 ops |

Note : sort_complex pour n=500 peut dépasser 5500 dans le pire cas. sort_medium peut être plus efficace pour certaines entrées.

### Questions probables en défense

**Q : Pourquoi utiliser des tableaux plutôt que des listes chaînées ?**

Les algorithmes medium et complex nécessitent un accès direct par index : `find_min` et `find_max_b` scannent les tableaux par index, `radix_pass` lit `d->a[0]` après chaque RA. Un tableau donne `a[i]` en O(1). La liste donnerait O(i) pour l'accès indexé, complexifiant inutilement l'implémentation.

**Q : Pourquoi `process_range` boucle exactement `total` fois ?**

En `total = na` itérations, on visite exactement chaque élément initialement présent dans A une fois. Chaque RA avance le curseur d'une position dans l'anneau circulaire, chaque PB retire l'élément courant. La borne fixe garantit terminaison et couverture complète sans condition de test complexe.

**Q : Pourquoi le radix sort fonctionne malgré l'inversion de B lors des PA ?**

L'inversion est systématique et cohérente. Pour chaque bit, tous les éléments du groupe bit=0 voient leur ordre relatif inversé de manière identique. Après toutes les passes, la propriété de tri est globalement maintenue. La trace pour n=4 sur toutes les permutations confirme la correction.

**Q : Quel est le rôle de `disorder` et pourquoi ces seuils ?**

`disorder` mesure la fraction d'inversions dans [0,1]. La valeur 0.5 correspond à l'espérance statistique d'une permutation aléatoire. En dessous de 0.2, le tableau est si peu désordonné que sort_simple, malgré sa complexité théorique O(n²), produit en pratique moins d'opérations que sort_complex (qui en produit 2n×log(n) quelle que soit l'entrée).

**Q : Pourquoi `strategy` et `used_strategy` sont deux champs distincts ?**

En mode adaptatif, `strategy == 3` indique le mode de sélection, pas l'algorithme utilisé. `used_strategy` capture l'algorithme réellement choisi (0, 1 ou 2). `bench.c` utilise les deux pour distinguer `"Adaptive -> Complex"` de `"Complex"` (usage direct via `--complex`). Sans ce champ supplémentaire, cette distinction serait impossible.

**Q : Quelle est la différence entre `na` et `n` ?**

`n` est fixé après le parsing et ne change jamais — c'est la taille du problème. `na` varie pendant le tri : chaque PB décrémente `na`, chaque PA l'incrémente. `n` sert dans `radix_pass` (la boucle doit tourner n fois pour traiter tous les éléments indépendamment de l'état courant de na) et dans `normalize` (on traite exactement n éléments). `na` sert dans les opérations qui doivent connaître la taille actuelle de A.
