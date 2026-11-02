*This project has been created as part of the 42 curriculum by fsantucc, mdutruel.*

# push_swap — Documentation technique exhaustive

> Compilé avec `cc -Wall -Wextra -Werror`

---

## Table des matières

1. [Description](#description)
2. [Instructions](#instructions)
3. [Algorithmes — justification et choix](#algorithmes--justification-et-choix)
4. [Contributors](#contributors)
5. [Resources](#resources)
6. [Modèle de données central](#6-modèle-de-données-central)
7. [Fichier par fichier — référence complète](#7-fichier-par-fichier--référence-complète)
   - [push\_swap.h](#71-push_swaph)
   - [main.c](#72-mainc)
   - [utils.c](#73-utilsc)
   - [flags.c](#74-flagsc)
   - [parse.c](#75-parsec)
   - [disorder.c](#76-disorderc)
   - [normalize.c](#77-normalizec)
   - [operations.c](#78-operationsc)
   - [sort\_simple.c](#79-sort_simplec)
   - [sort\_helpers.c](#710-sort_helpersc)
   - [sort\_medium.c](#711-sort_mediumc)
   - [sort\_complex.c](#712-sort_complexc)
   - [sort\_adaptive.c](#713-sort_adaptivec)
   - [bench.c](#714-benchc)
8. [Flux d'exécution global](#8-flux-dexécution-global)
9. [Analyse de complexité](#9-analyse-de-complexité)
10. [Gestion des erreurs et cas limites](#10-gestion-des-erreurs-et-cas-limites)
11. [Gestion mémoire](#11-gestion-mémoire)
12. [Défense technique — questions 42](#12-défense-technique--questions-42)

---

## Description

Push_swap trie une séquence d'entiers distincts en utilisant deux piles (A et B) et onze opérations primitives. Le programme imprime sur stdout la suite d'opérations qui trie la pile A dans l'ordre croissant.

Quatre stratégies de tri sont implémentées :

| Stratégie | Flag | Complexité opérations |
|-----------|------|-----------------------|
| Simple    | `--simple`   | O(n²) |
| Medium    | `--medium`   | O(n√n) |
| Complex   | `--complex`  | O(n log n) |
| Adaptive  | `--adaptive` / défaut | choisit selon disorder |

---

## Instructions

### Makefile — règles disponibles

```
make          → compile push_swap (cible all)
make clean    → supprime tous les .o
make fclean   → clean + supprime le binaire push_swap
make re       → fclean puis all
```

Sources compilées (dans l'ordre du Makefile) :

```
main.c utils.c flags.c parse.c disorder.c normalize.c operations.c
sort_simple.c sort_helpers.c sort_medium.c sort_complex.c
sort_adaptive.c bench.c
```

Règle de compilation de chaque objet :

```makefile
%.o: %.c push_swap.h
    $(CC) $(CFLAGS) -c $< -o $@
```

Tout `.o` dépend de `push_swap.h` : modifier le header force la recompilation de tous les fichiers.

### Exemples d'utilisation

```bash
./push_swap 3 1 4 2 5               # tri, imprime les operations
./push_swap 1 2 3 4 5               # deja trie, aucune sortie
./push_swap                         # argc==1, retourne 0, aucune sortie
./push_swap 3 1 4 1 5               # doublon -> "Error" sur stderr
./push_swap --simple   3 1 4 2 5    # force strategie O(n^2)
./push_swap --medium   3 1 4 2 5    # force strategie O(n*sqrt(n))
./push_swap --complex  3 1 4 2 5    # force strategie O(n log n)
./push_swap --adaptive 3 1 4 2 5    # selection auto (comportement par defaut)
./push_swap --bench    3 1 4 2 5    # rapport sur stderr + operations sur stdout
./push_swap --bench --complex 3 1 4 2 5  # combinaison valide

# integration checker
ARG="3 1 4 2 5"; ./push_swap $ARG | ./checker_linux $ARG
```

---

## Algorithmes — justification et choix

Quatre stratégies distinctes sont implémentées, chacune ciblant une classe de complexité différente dans le modèle d'opérations Push_swap.

### Simple — O(n²) : selection sort par extraction de minimum

**Algorithme :** répéter n-1 fois — trouver le minimum dans A, l'amener au sommet (RA ou RRA selon la direction la plus courte), le pousser en B — puis PA tous les éléments de B vers A dans l'ordre inverse.

**Justification :** O(n²) car chaque extraction coûte O(n) rotations pour amener le min au sommet, et il y a n extractions. Convient pour n faible ou entrée presque triée (disorder bas).

**Coût moyen (n=100) :** ~800-1100 opérations.

### Medium — O(n√n) : tri par blocs (chunk sort)

**Algorithme :** normaliser les valeurs en rangs [0..n-1]. Diviser l'espace [0..n-1] en blocs de taille k ≈ √(8n). Traiter les blocs du plus grand au plus petit : scanner A exactement `na` fois en poussant les éléments du bloc courant vers B (RA sinon), puis invoquer `sort_b_to_a` (extraire le maximum de B par rotations, PA) pour remettre le bloc dans A en ordre décroissant.

**Justification :** n/k blocs de taille k. `process_range` = O(na) par bloc → O(n) total par bloc. `sort_b_to_a` = O(k²) par bloc (k éléments × k/4 rotations avg). Total : O(n/k × k²) = O(nk) = O(n√n).

**Justification du seuil k :** k = √(8n) minimise nk + (n/k)×n = coût total. Plus grand k → moins de passes de scan mais plus de rotations dans sort_b_to_a.

### Complex — O(n log n) : radix sort LSD

**Algorithme :** normaliser en rangs [0..n-1]. Calculer b = ⌈log₂n⌉ bits nécessaires. Pour chaque bit de 0 à b-1 : itérer exactement `n` fois — si le bit courant de a[0] est 0 → PB, sinon → RA — puis PA pour tous les éléments de B.

**Justification :** b passes × 2n opérations par passe = O(n log n). Insensible au désordre initial.

**Coût moyen (n=500) :** ~6784 opérations (9 passes × ~750 ops/passe).

### Adaptive — sélection automatique par disorder

**Algorithme :** mesurer le disorder avant tout tri. Dispatche vers :

| Régime | Seuil | Méthode | Complexité |
|--------|-------|---------|------------|
| Faible | disorder < 0.2 | Simple (selection sort) | O(n²) |
| Moyen  | 0.2 ≤ disorder < 0.5 | Medium (chunk sort) | O(n√n) |
| Élevé  | disorder ≥ 0.5 | Complex (radix LSD) | O(n log n) |

**Justification des seuils :**
- Seuil 0.2 : en dessous, l'entrée a peu d'inversions. Le selection sort (O(n²)) fait peu de rotations car les minimums sont proches du sommet. Pour n=100 avec disorder=0.1, environ 50 inversions → simple très efficace.
- Seuil 0.5 : au-dessus, l'entrée est statistiquement proche d'une permutation aléatoire uniforme. Le radix sort (O(n log n)) est optimal pour des données très mélangées car son coût est indépendant de la distribution.
- Entre 0.2 et 0.5 : le chunk sort exploite le fait que les données sont partiellement ordonnées — les blocs contiennent des éléments déjà relativement groupés, réduisant les rotations.

---

## Contributors

Ce projet a été réalisé en binôme par :

| Login | Contributions principales |
|-------|--------------------------|
| **fsantucc** | Architecture générale, `main.c`, `parse.c`, `flags.c`, `sort_simple.c`, `sort_medium.c`, `sort_adaptive.c`, `disorder.c`, `Makefile` |
| **mdutruel** | `sort_complex.c`, `operations.c`, `normalize.c`, `sort_helpers.c`, `bench.c`, `utils.c`, tests et validation |

Les deux membres ont participé à la conception des algorithmes, à la revue de code, et sont capables d'expliquer l'intégralité du projet en soutenance.

---

## Resources

### Références algorithmes

- **Donald E. Knuth** — *The Art of Computer Programming, Vol. 3: Sorting and Searching* — référence fondamentale sur les algorithmes de tri.
- **Wikipedia** — [Radix sort](https://en.wikipedia.org/wiki/Radix_sort), [Heapsort](https://en.wikipedia.org/wiki/Heapsort), [Selection sort](https://en.wikipedia.org/wiki/Selection_sort) — descriptions formelles des algorithmes implémentés.
- **Big-O Cheat Sheet** — [bigocheatsheet.com](https://www.bigocheatsheet.com/) — référence rapide de complexité.
- **Visualgo** — [visualgo.net](https://visualgo.net/en/sorting) — visualisation interactive des algorithmes de tri.
- **42 Push_swap resources** — tutoriels et vidéos de la communauté 42 sur l'approche chunk sort et radix.

### Utilisation de l'IA

L'IA (Claude, Anthropic) a été utilisée dans ce projet pour :

- **Documentation et README :** génération de la structure de documentation technique, description des fonctions, explication des invariants et preuves de correction.
- **Vérification de complexité :** validation des arguments de complexité pour chaque stratégie (bornes supérieures dans le modèle d'opérations Push_swap).
- **Revue de code :** analyse des cas limites (INT_MIN/INT_MAX, entrée vide, doublons, débordement), vérification de la gestion mémoire et des chemins d'erreur.
- **Débogage :** aide à tracer le comportement du radix sort sur de petits exemples pour vérifier la correction malgré l'inversion LIFO de B.

L'ensemble du code C a été écrit et compris par les deux membres du groupe. L'IA n'a pas généré de code soumis directement — son rôle a été consultatif et documentaire.

---

## Modèle de données central

### Structure `t_data` (push\_swap.h)

```c
typedef struct s_data
{
    int     *a;             // pile A : tableau, a[0] = sommet
    int     *b;             // pile B : tableau, b[0] = sommet
    int     n;              // taille totale, fixee apres parsing
    int     na;             // elements actuellement en A (varie)
    int     nb;             // elements actuellement en B (varie)
    int     count[11];      // nb d'appels par operation (indice = valeur e_op)
    double  disorder;       // ratio inversions / paires totales, dans [0.0, 1.0]
    int     strategy;       // strategie choisie : 0=simple 1=medium 2=complex 3=adaptive
    int     used_strategy;  // strategie reellement executee (differ en mode adaptive)
    int     bench;          // 1 si --bench est active, 0 sinon
}   t_data;
```

**Invariant fondamental :** `na + nb == n` a tout moment pendant le tri.

**`n` vs `na` :** `n` est figé après le parsing. `na` décroît à chaque `pb` et croît à chaque `pa`. Les fonctions qui doivent connaître la taille totale du problème utilisent `n` (`radix_pass`, `normalize`). Celles qui opèrent sur l'état courant utilisent `na`/`nb`.

**`strategy` vs `used_strategy` :** `strategy` = ce que l'utilisateur a demandé (ou 3 par défaut). `used_strategy` = ce qui a réellement tourné. En mode adaptatif, `strategy==3` mais `used_strategy` vaut 0, 1 ou 2. Cela permet à `bench.c` d'afficher `"Adaptive -> Complex O(n log n)"` vs `"Complex O(n log n)"`.

### Enum `e_op` (push\_swap.h)

```c
enum e_op {SA, SB, SS, PA, PB, RA, RB, RR, RRA, RRB, RRR};
//          0   1   2   3   4   5   6   7    8    9    10
```

Sert d'index dans `count[11]` et dans les tableaux `names[11]` de `operations.c` et `bench.c`. Passer `op` à `apply_op` par valeur entière évite toute comparaison de chaînes à l'exécution.

### Représentation mémoire des piles

Les deux piles sont des **tableaux d'entiers** avec l'**index 0 = sommet** :

```
Conceptuel   A:          En memoire a[] :
  TOP -> 3               a[0] = 3   <- sommet
         7               a[1] = 7
         1               a[2] = 1
  BOT -> 5               a[3] = 5
                         na   = 4
```

Conséquences :
- `push` : décale tout `dst[]` d'un cran vers la droite avant d'écrire en `[0]` → O(n)
- `rotate` : sauvegarde `[0]`, décale tout vers la gauche, écrit en `[n-1]` → O(n)
- `reverse` : sauvegarde `[n-1]`, décale tout vers la droite, écrit en `[0]` → O(n)
- `swap` : échange `[0]` et `[1]` → O(1)

---

## Fichier par fichier — référence complète

---

### 4.1 push\_swap.h

**Rôle :** seul header du projet. Inclut `<unistd.h>`, `<stdlib.h>`, `<limits.h>`. Déclare `t_data`, `e_op`, et tous les prototypes publics.

**Prototypes déclarés :**

| Prototype | Fichier source |
|-----------|---------------|
| `int ft_strcmp(const char *a, const char *b)` | utils.c |
| `void ft_putstr_fd(const char *s, int fd)` | utils.c |
| `void ft_putendl_fd(const char *s, int fd)` | utils.c |
| `void ft_putnbr_fd(long n, int fd)` | utils.c |
| `int is_option(const char *s)` | flags.c |
| `int set_option(t_data *d, char *s, int *selected)` | flags.c |
| `int parse_input(t_data *d, int argc, char **argv)` | parse.c |
| `void free_data(t_data *d)` | disorder.c |
| `double compute_disorder(t_data *d)` | disorder.c |
| `int is_sorted(t_data *d)` | disorder.c |
| `int normalize(t_data *d)` | normalize.c |
| `void apply_op(t_data *d, int op)` | operations.c |
| `void sort_simple(t_data *d)` | sort_simple.c |
| `int sort_medium(t_data *d)` | sort_medium.c |
| `int sort_complex(t_data *d)` | sort_complex.c |
| `int sort_adaptive(t_data *d)` | sort_adaptive.c |
| `void sort_b_to_a(t_data *d)` | sort_helpers.c |
| `int find_min(t_data *d)` | sort_simple.c |
| `int find_max_b(t_data *d)` | sort_helpers.c |
| `void print_bench(t_data *d)` | bench.c |

Les fonctions `static` (internes à chaque `.c`) ne sont pas déclarées ici.

---

### 4.2 main.c

**Rôle :** point d'entrée, orchestration du pipeline complet.

**Fonctions :** `init_data` (static), `set_used` (static), `run_sort` (static), `main`.

---

#### `static void init_data(t_data *d)` — ligne 15

**Signature :** prend un pointeur sur la structure déjà allouée sur la pile dans `main`.

**Ce qu'elle fait :**

```c
d->a = NULL;          // pas encore alloue
d->b = NULL;          // pas encore alloue
d->n = 0;             // sera fixe dans parse_input
d->na = 0;            // aucun element en A
d->nb = 0;            // aucun element en B
d->disorder = 0;      // sera calcule dans parse_input
d->strategy = 3;      // defaut = adaptive
d->used_strategy = 3; // sera mis a jour par set_used
d->bench = 0;         // --bench non demande
// puis boucle while (i < 11) { d->count[i] = 0; i++; }
```

**Pourquoi `strategy = 3` par défaut :** si aucun flag de stratégie n'est passé, `set_option` n'est jamais appelé pour un flag de stratégie, donc `strategy` reste 3 (adaptive). C'est le comportement normal sans argument de stratégie.

**Pourquoi initialiser `count[]` à la main :** le standard C garantit que les variables locales ne sont PAS initialisées à zéro. Sans cette boucle, `count[]` contiendrait des valeurs indéterminées et `bench.c` afficherait des résultats incorrects.

---

#### `static void set_used(t_data *d)` — ligne 36

**Signature :** prend le pointeur sur `d` après que `disorder` a été calculé.

**Ce qu'elle fait :** détermine `used_strategy` AVANT l'exécution du tri, pour que `bench.c` puisse l'afficher correctement.

```c
d->used_strategy = d->strategy;          // par defaut, identique
if (d->strategy == 3 && d->disorder < 0.2)
    d->used_strategy = 0;                // adaptive choisira simple
else if (d->strategy == 3 && d->disorder < 0.5)
    d->used_strategy = 1;                // adaptive choisira medium
else if (d->strategy == 3)
    d->used_strategy = 2;                // adaptive choisira complex
```

**Cas non-adaptatif (strategy 0, 1 ou 2) :** la première ligne `d->used_strategy = d->strategy` suffit, les `if` suivants ne s'exécutent pas car `d->strategy != 3`.

**Redondance avec `sort_adaptive` :** `sort_adaptive.c` contient la même logique de seuils et met à jour `used_strategy` lui aussi. La duplication est intentionnelle : `set_used` permet l'affichage bench même si le tri échoue (malloc), et garantit que `used_strategy` est cohérent avant l'appel.

---

#### `static int run_sort(t_data *d)` — ligne 47

**Signature :** retourne 1 si le tri réussit, 0 si malloc interne échoue.

**Ce qu'elle fait :** dispatch vers la stratégie choisie.

```c
if (d->strategy == 0) {
    d->used_strategy = 0;   // ecrase set_used pour coherence
    sort_simple(d);          // void, toujours reussit
    return (1);
}
if (d->strategy == 1) {
    d->used_strategy = 1;
    return (sort_medium(d));   // peut retourner 0 si normalize echoue
}
if (d->strategy == 2) {
    d->used_strategy = 2;
    return (sort_complex(d));  // peut retourner 0 si normalize echoue
}
return (sort_adaptive(d));     // strategy == 3
```

**Pourquoi `sort_simple` est `void` mais les autres retournent `int` :** `sort_simple` n'appelle pas `normalize` (pas de malloc), donc ne peut pas échouer. `sort_medium` et `sort_complex` appellent `normalize` qui alloue dynamiquement.

**Pourquoi réécrire `used_strategy` ici :** `set_used` l'a déjà fixé correctement, mais pour les stratégies 0/1/2 c'est défensif. Pour la stratégie 3, c'est `sort_adaptive` qui met à jour `used_strategy`.

---

#### `int main(int argc, char **argv)` — ligne 68

**Variables locales :** `t_data d` (sur la pile, pas le tas), `int ok`.

**Pipeline :**

```
1. init_data(&d)                 → zéro la structure
2. argc == 1 → return 0          → aucun argument : succès silencieux
3. ok = parse_input(&d, argc, argv)
4. if (ok) set_used(&d)          → calcule used_strategy
5. if (ok && !is_sorted(&d))     → court-circuit si deja trie
       ok = run_sort(&d)
6. if (ok && d.bench)
       print_bench(&d)           → rapport stderr
7. if (!ok)
       ft_putendl_fd("Error", 2) → message stderr
8. free_data(&d)                 → libere a et b
9. return (!ok)                  → 0=succes, 1=erreur
```

**Pourquoi `d` sur la pile :** structure de taille fixe (deux pointeurs + entiers + double). Les seules allocations dynamiques sont `d->a` et `d->b` (tableaux d'entiers), gérés par `parse_input` et libérés par `free_data`.

**Cas `is_sorted` vrai :** on ne rentre pas dans `run_sort`. `ok` reste 1. Aucune opération n'est imprimée. `print_bench` est quand même appelé si `--bench` est actif (il affiche disorder et 0 pour toutes les opérations).

---

### 4.3 utils.c

**Rôle :** réimplémentation des primitives I/O sans `<stdio.h>`. Toutes les sorties du projet passent par ces 4 fonctions.

---

#### `int ft_strcmp(const char *a, const char *b)` — ligne 15

**Ce qu'elle fait :** compare deux chaînes lexicographiquement.

```c
while (*a && *a == *b) { a++; b++; }
return ((unsigned char)*a - (unsigned char)*b);
```

**Retour :** 0 si identiques, négatif si `a < b`, positif si `a > b`.

**Pourquoi `unsigned char` :** le cast évite un comportement indéfini sur les caractères à valeur > 127. Si `*a = 0xFF` et `*b = 0x00`, sans cast : `(char)0xFF - (char)0x00 = -1 - 0 = -1` (mauvais). Avec cast : `255 - 0 = 255` (correct).

**Utilisation dans le projet :** exclusivement par `is_option` et `set_option` dans `flags.c` pour comparer `argv[i]` aux chaînes de flags connues. Toujours utilisé comme `!ft_strcmp(s, "--bench")` pour tester l'égalité.

---

#### `void ft_putstr_fd(const char *s, int fd)` — ligne 25

**Ce qu'elle fait :** écrit la chaîne `s` sur le descripteur `fd`, octet par octet.

```c
while (*s) { write(fd, s, 1); s++; }
```

**Pourquoi `write(fd, s, 1)` et non `write(fd, s, strlen(s))` :** évite d'inclure `<string.h>`. Un `write` d'un octet à la fois est légèrement moins efficace mais fonctionnellement identique. Pour n ≤ 500 et au plus 9 × 500 × 2 = 9000 opérations, chacune imprimant 2-3 octets, le coût est négligeable.

**Utilisations :** imprime les préfixes `[bench]` dans `bench.c`, imprime `"Error"` + les noms d'opérations via `ft_putendl_fd`.

---

#### `void ft_putendl_fd(const char *s, int fd)` — ligne 34

**Ce qu'elle fait :** `ft_putstr_fd(s, fd)` puis `write(fd, "\n", 1)`.

**Utilisation principale :** `apply_op` appelle `ft_putendl_fd(names[op], 1)` pour chaque opération → une opération par ligne sur stdout.

---

#### `void ft_putnbr_fd(long n, int fd)` — ligne 40

**Paramètre `long` (pas `int`) :** permet d'afficher des valeurs potentiellement grandes dans `bench.c` (les comptes d'opérations restent petits, mais `disorder * 10000` peut produire un `long`).

**Algorithme :**

```
1. Si n < 0 : write("-"), n = -n
2. Si n == 0 : buf[0] = '0', i = 1
3. Sinon, boucle : buf[i++] = '0' + n%10, n /= 10  (chiffres en ordre inverse)
4. Boucle inverse : write(&buf[--i], 1)             (impression dans l'ordre)
```

**Buffer `char buf[12]` :** taille suffisante pour tout `long` 64 bits (19 chiffres + signe + terminateur). En pratique les valeurs affichées sont bien plus petites.

**Cas `n == 0` :** le `while (n && i < 11)` ne s'exécute pas. Le `if (!n)` beforehand écrit `'0'` dans `buf[0]`. Sans ce cas spécial, `buf` serait vide et rien ne serait imprimé.

---

### 4.4 flags.c

**Rôle :** identifier et appliquer les flags de la ligne de commande.

---

#### `int is_option(const char *s)` — ligne 15

**Ce qu'elle fait :** retourne 1 si `s` est exactement l'un des 5 flags reconnus.

```c
return (!ft_strcmp(s, "--simple") || !ft_strcmp(s, "--medium")
    || !ft_strcmp(s, "--complex") || !ft_strcmp(s, "--adaptive")
    || !ft_strcmp(s, "--bench"));
```

**Retour :** 1 (c'est un flag) ou 0 (c'est un nombre ou une chaîne invalide).

**Utilisée dans `read_values` (parse.c) :** chaque `argv[i]` passe par `is_option` pour décider si c'est un flag ou un nombre.

**Cas `"--simple2"` :** `ft_strcmp("--simple2", "--simple")` retourne non-zéro → `is_option` retourne 0 → traité comme nombre → `number()` échoue → erreur. Correct.

---

#### `int set_option(t_data *d, char *s, int *selected)` — ligne 22

**Paramètre `selected` :** pointeur sur un entier local de `read_values` qui vaut 0 au départ. Sert de verrou pour interdire deux flags de stratégie.

**Algorithme :**

```
1. Si "--bench" :
       d->bench = 1
       return 1    (PAS de verrou : --bench peut coexister avec tout)

2. Si *selected != 0 :
       return 0    (un flag de strategie deja vu → erreur)

3. *selected = 1   (lever le verrou)

4. Selon s :
       "--simple"   → d->strategy = 0
       "--medium"   → d->strategy = 1
       "--complex"  → d->strategy = 2
       "--adaptive" → d->strategy = 3  (le else final)

5. return 1
```

**Pourquoi `--bench` échappe au verrou :** c'est un flag de mode (debug/mesure), pas de stratégie. Il est orthogonal. L'appeler deux fois est idempotent (`d->bench = 1` deux fois → pas de dommage).

**Pourquoi `--adaptive` est le `else` final :** `is_option` garantit que `s` est l'un des 5 flags. Si ce n'est ni `--simple` ni `--medium` ni `--complex`, ce ne peut être que `--adaptive`. Le `else` évite une comparaison inutile.

**Cas `--simple --medium` :** premier appel : `*selected = 0` → `strategy = 0`, `*selected = 1`. Deuxième appel : `*selected = 1` → `return 0` → `read_values` retourne 0 → erreur. ✓

---

### 4.5 parse.c

**Rôle :** valider les arguments, construire le tableau `a[]`, allouer `b[]`, déclencher le calcul de disorder.

---

#### `static int number(char *s, int *out)` — ligne 15

**Paramètres :** `s` = chaîne à convertir, `out` = pointeur où écrire la valeur si valide.

**Variables locales :**
- `v` (long long) : valeur absolue accumulée
- `limit` (long long) : valeur absolue maximale autorisée selon le signe
- `sign` (int) : 1 ou -1

**Algorithme ligne par ligne :**

```
v = 0; sign = 1;

// gestion du signe optionnel
if (*s == '-' || *s == '+')
    if (*s++ == '-')
        sign = -1;
// avancement du pointeur : s pointe maintenant apres le signe

// chaine vide ou signe seul
if (!*s)
    return (0);

// calcul de la limite d'overflow selon le signe
limit = (long long)INT_MAX + (sign < 0);
// sign > 0 (positif) : limit = 2147483647 = INT_MAX
// sign < 0 (negatif) : limit = 2147483648 = |INT_MIN|

// lecture des chiffres
while (*s >= '0' && *s <= '9')
{
    v = v * 10 + (*s++ - '0');
    if (v > limit)
        return (0);   // overflow detecte immediatement
}

// caractere non-chiffre restant
if (*s)
    return (0);

*out = (int)(v * sign);
return (1);
```

**Détail de la formule `limit = (long long)INT_MAX + (sign < 0)` :**

L'expression `(sign < 0)` vaut 1 si négatif, 0 si positif. Donc :
- Positif : `limit = 2147483647` → valeur max d'un int positif
- Négatif : `limit = 2147483648` → valeur absolue de INT_MIN

Ce calcul en une ligne remplace deux branches conditionnelles.

**Gestion de INT_MIN = -2147483648 :**
- `sign = -1`, `limit = 2147483648`
- `v` accumule `2147483648`
- `v > limit` → `2147483648 > 2147483648` → FAUX → accepté
- `*out = (int)(2147483648LL * -1) = (int)(-2147483648LL)`
- `-2147483648` est représentable en `int` 32 bits → comportement défini

**Tableau de cas :**

| Entrée | sign | limit | v final | v > limit ? | Résultat |
|--------|------|-------|---------|-------------|---------|
| `"2147483647"` | 1 | 2147483647 | 2147483647 | non | ✓ |
| `"2147483648"` | 1 | 2147483647 | 2147483648 | oui | erreur |
| `"-2147483648"` | -1 | 2147483648 | 2147483648 | non | ✓ |
| `"-2147483649"` | -1 | 2147483648 | 2147483649 | oui | erreur |
| `"abc"` | 1 | 2147483647 | 0 | — | `!*s` faux car *s='a' → erreur |
| `""` | 1 | — | — | — | `!*s` vrai apres signe → erreur |
| `"+"` | 1 | — | — | — | s avance, `!*s` vrai → erreur |
| `"3.14"` | 1 | 2147483647 | 3 | non | `*s='.'` apres chiffres → erreur |

---

#### `static int append_value(t_data *d, char *arg)` — ligne 41

**Ce qu'elle fait :** convertit `arg` en entier, vérifie l'absence de doublon, stocke dans `d->a`.

```
1. number(arg, &value) → 0 si invalide → return 0
2. boucle i = 0..na-1 :
       si d->a[i] == value → return 0  (doublon)
3. d->a[d->na++] = value
4. return 1
```

**Complexité de la détection de doublons :** O(na) par appel, O(n²) au total. Acceptable pour n ≤ 500.

**Pourquoi `d->a[i++] == value` dans la boucle :** le `i++` est dans la condition du while → `if (d->a[i++] == value) return (0)`. Si doublon trouvé, on retourne avant d'incrémenter `na`. La valeur `value` n'est donc jamais stockée en cas de doublon.

---

#### `static int read_values(t_data *d, int argc, char **argv)` — ligne 56

**Variables locales :** `i` (index courant dans argv), `selected` (verrou flags de stratégie).

**Algorithme :**

```
selected = 0;
pour chaque argv[i], i de 1 a argc-1 :
    si is_option(argv[i]) :
        si !set_option(d, argv[i], &selected) → return 0
    sinon :
        si !append_value(d, argv[i]) → return 0

return (d->na > 0);   // erreur si aucun nombre fourni
```

**Ordre libre flags/nombres :** `./push_swap 3 --bench 1 2` et `./push_swap --bench 3 1 2` produisent le même résultat. La boucle traite les éléments dans l'ordre sans état dépendant de la position relative.

**Retour `d->na > 0` :** si l'utilisateur passe seulement `--bench` sans nombre, `na == 0` → retourne 0 → erreur.

---

#### `int parse_input(t_data *d, int argc, char **argv)` — ligne 74

**C'est la seule fonction publique de ce fichier.**

```
1. d->a = malloc(sizeof(int) * argc)
   si !d->a : return 0

2. si !read_values(d, argc, argv) : return 0
   (note : si malloc(a) ok mais read_values echoue, d->b est encore NULL)

3. d->n = d->na             (figer la taille totale)

4. d->b = malloc(sizeof(int) * d->n)
   si !d->b : return 0

5. d->disorder = compute_disorder(d)

6. return 1
```

**Pourquoi `malloc(argc)` pour `d->a` :** borne supérieure sûre. `argc` inclut `argv[0]` (nom du programme), les flags, et les nombres. Le nombre réel d'entiers est au plus `argc - 1`. Pas d'accès hors bornes possible car `na` ne peut pas dépasser `argc - 1`.

**Pourquoi `compute_disorder` ici et pas dans `main` :** le disorder porte sur l'état original de la pile avant toute transformation. Si on normalisait d'abord, les valeurs changeraient (les rangs ont la même structure d'inversions, mais pour `bench.c` on veut associer le disorder aux valeurs originales). En réalité, normalisation et disorder mesurent la même chose : disorder est sur les valeurs brutes, ce qui est équivalent sur les rangs. Mais le calcul est fait ici pour que `d->disorder` soit disponible dès la fin du parsing.

---

### 4.6 disorder.c

**Rôle :** mesurer le degré de désordre, vérifier si déjà trié, libérer la mémoire.

---

#### `double compute_disorder(t_data *d)` — ligne 15

**Ce qu'elle fait :** calcule le ratio inversions / paires totales.

**Définition d'une inversion :** paire `(i, j)` avec `i < j` et `a[i] > a[j]`.

**Nombre max de paires :** `n*(n-1)/2` (toutes les paires non ordonnées).

**Algorithme :**

```c
bad = 0;
total = (long long)d->na * (d->na - 1) / 2;
for i = 0..na-1:
    for j = i+1..na-1:
        if a[i] > a[j]: bad++
if total == 0: return 0.0
return (double)bad / (double)total
```

**Pourquoi `long long` pour `bad` et `total` :** pour n = 500, `total = 500*499/2 = 124750`. Tient dans un `int`. Pour de grandes valeurs de `n` potentielles, `long long` est défensif. `bad` ne peut pas dépasser `total`, donc si `total` est `long long`, `bad` aussi.

**Pourquoi `if (!total) return 0.0` :** si `na == 0` ou `na == 1`, `total = 0`. Division par zéro sinon. Pour `na == 1`, le tableau est trivialement "trié" → disorder = 0.0.

**Interprétation :**

| disorder | Signification |
|----------|--------------|
| 0.0 | Parfaitement trié |
| ~0.25 | Quelques elements mal places |
| 0.5 | Permutation aleatoire (esperance pour n→∞) |
| 1.0 | Ordre parfaitement inverse |

---

#### `int is_sorted(t_data *d)` — ligne 41

**Ce qu'elle fait :** vérifie `a[i-1] <= a[i]` pour tout i de 1 à na-1.

```c
i = 1;
while (i < d->na)
{
    if (d->a[i - 1] > d->a[i])
        return (0);
    i++;
}
return (1);
```

**Cas na == 0 ou na == 1 :** la boucle ne s'exécute pas (condition `1 < 0` ou `1 < 1` fausse). Retourne 1. Un tableau vide ou à un seul élément est considéré trié.

**Utilisée dans `main` :** si retourne 1, on saute `run_sort`. Aucune opération push_swap n'est imprimée.

---

#### `void free_data(t_data *d)` — ligne 55

```c
free(d->a);
free(d->b);
```

**Pourquoi `free(NULL)` est safe :** ISO C11 §7.22.3.3 : "If ptr is a null pointer, no action occurs." Si `malloc(d->a)` réussit mais `malloc(d->b)` échoue, `d->b` reste NULL. `free(NULL)` ne plante pas.

**Pas de mise à NULL après free :** `d` est une variable locale de `main` et n'est pas utilisée après `free_data`. Mettre à NULL serait défensif mais inutile ici.

---

### 4.7 normalize.c

**Rôle :** transformer les valeurs entières arbitraires de `d->a[]` en rangs continus `[0..n-1]`.

---

#### `static void sift_down(int *a, int root, int end)` — ligne 15

**Ce qu'elle fait :** maintient la propriété de max-heap en faisant descendre l'élément à `root` vers sa place correcte.

**Paramètres :** `a` = tableau, `root` = index de départ, `end` = dernier index valide du heap.

**Algorithme :**

```
tant que root a un fils gauche (root*2+1 <= end) :
    child = fils gauche = root*2+1
    si fils droit existe ET fils gauche < fils droit :
        child = fils droit  (choisir le plus grand)
    si a[root] >= a[child] :
        stop  (root est a sa place)
    echanger a[root] et a[child]
    root = child  (descendre)
```

**Formule fils gauche :** `root * 2 + 1`. Fils droit : `root * 2 + 2`.

**Propriété maintenue :** après `sift_down(a, root, end)`, le sous-arbre enraciné en `root` est un max-heap.

---

#### `static void heap_sort(int *a, int n)` — ligne 34

**Ce qu'elle fait :** trie `a[0..n-1]` en ordre croissant via heapsort.

**Phase 1 — construction du max-heap (heapify) :**

```c
i = n / 2 - 1;
while (i >= 0) { sift_down(a, i, n - 1); i--; }
```

Parcourt les noeuds internes de droite à gauche (bottom-up). `n/2 - 1` est le dernier noeud interne (le parent du dernier noeud feuille). Après cette phase, `a[0]` est le maximum.

**Phase 2 — extraction successive :**

```c
i = n - 1;
while (i > 0)
{
    tmp = a[0]; a[0] = a[i]; a[i] = tmp;   // swap max en position finale
    sift_down(a, 0, --i);                    // restaurer le heap sur [0..i-1]
}
```

À chaque itération : le maximum actuel (en `a[0]`) est placé en `a[i]` (sa position finale dans le tableau trié). La taille effective du heap est réduite de 1 via `--i`.

**Résultat :** `a[0..n-1]` trié en ordre croissant. Complexité O(n log n).

**Utilisation dans le projet :** seulement dans `normalize()` pour trier la copie `sorted[]`.

---

#### `static int get_rank(int *sorted, int n, int value)` — ligne 55

**Ce qu'elle fait :** retourne l'index de `value` dans le tableau trié `sorted`.

**Algorithme :** recherche binaire classique.

```c
lo = 0; hi = n - 1;
while (lo <= hi)
{
    mid = lo + (hi - lo) / 2;   // evite overflow vs (lo+hi)/2
    if (sorted[mid] == value) return (mid);
    if (sorted[mid] < value)  lo = mid + 1;
    else                      hi = mid - 1;
}
return (-1);
```

**Pourquoi `lo + (hi - lo) / 2` et non `(lo + hi) / 2` :** évite l'overflow si `lo + hi > INT_MAX`. Dans ce projet, `n ≤ 500` donc pas de risque, mais c'est la formule correcte par convention.

**Retour -1 :** jamais atteint en pratique — `value` est toujours un élément de `d->a[]` qui a été copié dans `sorted[]`. La valeur -1 servirait de signal d'erreur si le code était utilisé dans un autre contexte.

---

#### `int normalize(t_data *d)` — ligne 76

**C'est la seule fonction publique de ce fichier.**

```
1. sorted = malloc(sizeof(int) * d->n)
   si echec : return 0

2. copier d->a[0..n-1] dans sorted

3. heap_sort(sorted, d->n)
   → sorted est maintenant trie croissant

4. pour i = 0..n-1 :
       d->a[i] = get_rank(sorted, d->n, d->a[i])
   → chaque valeur est remplacee par son rang

5. free(sorted)
6. return 1
```

**Exemple complet :**

```
Avant : d->a = [42, -5, 100, 7, -100]  n=5

Copie : sorted = [42, -5, 100, 7, -100]
Apres heap_sort : sorted = [-100, -5, 7, 42, 100]

Rangs :
  d->a[0]=42  → get_rank(..., 42)  → index 3  → d->a[0]=3
  d->a[1]=-5  → get_rank(..., -5)  → index 1  → d->a[1]=1
  d->a[2]=100 → get_rank(..., 100) → index 4  → d->a[2]=4
  d->a[3]=7   → get_rank(..., 7)   → index 2  → d->a[3]=2
  d->a[4]=-100→ get_rank(...,-100) → index 0  → d->a[4]=0

Apres : d->a = [3, 1, 4, 2, 0]
```

**Propriété préservée :** l'ordre relatif est intact. Si `a[i] < a[j]` avant, `rank(a[i]) < rank(a[j])` après. Le tri de `d->a[]` est équivalent avant et après normalisation.

---

### 4.8 operations.c

**Rôle :** implémenter physiquement les 4 primitives sur tableaux, et le dispatch central `apply_op`.

---

#### `static void swap(int *s, int n)` — ligne 15

**Ce qu'elle fait :** échange `s[0]` et `s[1]` si le tableau a au moins 2 éléments.

```c
if (n > 1)
{
    tmp = s[0];
    s[0] = s[1];
    s[1] = tmp;
}
```

**Garde `n > 1` :** si la pile a 0 ou 1 élément, swap est un no-op silencieux. Évite un accès `s[1]` invalide.

**Correspond à :** `sa` (swap(d->a, d->na)) et `sb` (swap(d->b, d->nb)).

---

#### `static void push(int *dst, int *nd, int *src, int *ns)` — ligne 27

**Ce qu'elle fait :** déplace `src[0]` vers `dst[0]`, en maintenant la représentation top=index0.

```
garde : si *ns == 0, return

// faire de la place en tête de dst
i = *nd;
while (i > 0) { dst[i] = dst[i-1]; i--; }
// dst[1..nd] = anciens dst[0..nd-1]

dst[0] = src[0];   // insere l'element
(*nd)++;

// compacter src en supprimant src[0]
i = 0;
while (i < *ns - 1) { src[i] = src[i+1]; i++; }
(*ns)--;
```

**Coût :** O(nd) pour décaler dst + O(ns) pour compacter src = O(n) total.

**Exemple PA (push(a, &na, b, &nb)) :**

```
Avant : A=[3,1,4] na=3,  B=[7,2] nb=2
Decalage dst : A=[3,3,1,4] puis A[0]=7 → A=[7,3,1,4]...
Attendez, decalage de droite a gauche :
  i=3: A[3]=A[2]=4
  i=2: A[2]=A[1]=1
  i=1: A[1]=A[0]=3
  A = [_, 3, 1, 4]   puis A[0] = B[0] = 7
  A = [7, 3, 1, 4],  na = 4
Compactage src :
  B = [2, _]  puis nb = 1
  B = [2]
Apres : A=[7,3,1,4] na=4,  B=[2] nb=1
```

---

#### `static void rotate(int *s, int n)` — ligne 50

**Ce qu'elle fait :** déplace le sommet (s[0]) vers le fond (s[n-1]).

```
garde : si n < 2, return
first = s[0]
for i = 0..n-2: s[i] = s[i+1]   // decalage gauche
s[n-1] = first
```

**Sémantique push_swap :** `ra` = "le premier de A passe en dernier".

**Exemple :**

```
Avant RA : A = [3, 1, 4, 2]
first = 3
Apres decalage : A = [1, 4, 2, 2]
Apres replace  : A = [1, 4, 2, 3]
```

---

#### `static void reverse(int *s, int n)` — ligne 67

**Ce qu'elle fait :** déplace le fond (s[n-1]) vers le sommet (s[0]).

```
garde : si n < 2, return
last = s[n-1]
for i = n-1..1: s[i] = s[i-1]   // decalage droite
s[0] = last
```

**Sémantique push_swap :** `rra` = "le dernier de A passe en premier".

**Exemple :**

```
Avant RRA : A = [3, 1, 4, 2]
last = 2
Apres decalage : A = [3, 3, 1, 4]
Apres replace  : A = [2, 3, 1, 4]
```

---

#### `void apply_op(t_data *d, int op)` — ligne 84

**C'est la seule fonction publique de ce fichier.**

**Tableau statique `names[11]` :**

```c
static const char *names[11] = {"sa","sb","ss","pa","pb","ra","rb","rr","rra","rrb","rrr"};
```

`static` = alloué une seule fois au démarrage, pas à chaque appel. Indexé par `e_op` : `names[RA] = names[5] = "ra"`.

**Dispatch par blocs `if` indépendants :**

```c
if (op == SA || op == SS) swap(d->a, d->na);
if (op == SB || op == SS) swap(d->b, d->nb);
if (op == PA)  push(d->a, &d->na, d->b, &d->nb);
if (op == PB)  push(d->b, &d->nb, d->a, &d->na);
if (op == RA  || op == RR)  rotate(d->a, d->na);
if (op == RB  || op == RR)  rotate(d->b, d->nb);
if (op == RRA || op == RRR) reverse(d->a, d->na);
if (op == RRB || op == RRR) reverse(d->b, d->nb);
```

**Pourquoi `if` et non `else if` :** les opérations combinées (SS, RR, RRR) doivent déclencher deux actions. Avec `else if`, la deuxième branche ne serait jamais atteinte pour SS/RR/RRR. Par exemple pour `SS` : la première condition `op==SS` est vraie → exécute swap(a). Avec `if` indépendant, la deuxième condition `op==SB || op==SS` est aussi évaluée → exécute swap(b). ✓

**Après le dispatch :**

```c
d->count[op]++;                      // comptabilise l'operation
ft_putendl_fd(names[op], 1);         // imprime sur stdout
```

Toute opération est automatiquement comptée et imprimée, sans que les fonctions appelantes aient à s'en préoccuper.

---

### 4.9 sort\_simple.c

**Rôle :** stratégie 0 — tri par sélection O(n²).

---

#### `int find_min(t_data *d)` — ligne 15

**Ce qu'elle fait :** trouve la position du minimum dans `d->a[0..na-1]`.

```c
i = 1; pos = 0;
while (i < d->na)
{
    if (d->a[i] < d->a[pos])
        pos = i;
    i++;
}
return (pos);
```

**Initialisation :** `pos = 0` — le sommet est supposé minimum jusqu'à preuve du contraire. `i = 1` — on commence à comparer à partir du deuxième élément.

**Retour :** index (position dans le tableau) du minimum, pas la valeur.

**Complexité :** O(na) — scan linéaire complet.

**Fonction publique** (prototype dans le header) : utilisée par `bench.c` ? Non — elle est déclarée publique pour être utilisée par `sort_medium.c` si nécessaire. En réalité, seul `sort_simple.c` l'utilise directement.

---

#### `static void bring_min(t_data *d, int pos)` — ligne 31

**Ce qu'elle fait :** amène l'élément à la position `pos` au sommet de A via rotations.

```c
if (pos <= d->na / 2)
    while (pos-- > 0)
        apply_op(d, RA);     // rotation vers l'avant
else
    while (pos++ < d->na)
        apply_op(d, RRA);    // rotation vers l'arriere
```

**Choix de la direction :**
- Vers l'avant (RA) : `pos` opérations
- Vers l'arrière (RRA) : `na - pos` opérations
- Seuil `pos <= na/2` : choisit la direction la plus courte

**Exemple :** na=10, min en pos=3.
- Vers avant : 3 RA
- Vers arrière : 7 RRA
- Choix : 3 RA ✓ (3 ≤ 10/2 = 5)

**Exemple :** na=10, min en pos=8.
- Vers avant : 8 RA
- Vers arrière : 2 RRA
- Choix : 2 RRA ✓ (8 > 10/2 = 5)

**Détail de la boucle RRA :** `while (pos++ < d->na)`. Attention : `d->na` est constant pendant les rotations RRA (RRA ne change pas na). La boucle tourne `na - pos` fois.

Vérification : pos=8, na=10. Itérations : pos=8 < 10 (RRA, pos→9), pos=9 < 10 (RRA, pos→10), pos=10 < 10 faux → stop. 2 RRA. ✓

---

#### `void sort_simple(t_data *d)` — ligne 41

**Algorithme en deux phases :**

**Phase 1 — extraction des minimums :**

```c
while (d->na > 1)
{
    pos = find_min(d);     // trouver le minimum courant
    bring_min(d, pos);     // l'amener au sommet
    apply_op(d, PB);       // le pousser sur B
}
```

La boucle continue tant qu'il reste plus d'un élément en A. Le maximum finit seul en A (`na == 1`).

État de B après la phase 1 : les minimums ont été poussés dans l'ordre croissant (min0 en premier, min_{n-2} en dernier). La pile B (top = index 0) a le dernier poussé = le plus grand des minimums au sommet :

```
B (top→bot) = [n-2, n-3, ..., 1, 0]
A            = [n-1]   (le maximum seul)
```

**Phase 2 — reconstruction :**

```c
while (d->nb)
    apply_op(d, PA);
```

Chaque PA prend le sommet de B et le place en tête de A. Les éléments arrivent dans l'ordre décroissant depuis B, formant A dans l'ordre croissant :

```
Etape 1 : A=[n-2, n-1],    B=[n-3, ..., 0]
Etape 2 : A=[n-3, n-2, n-1], B=[n-4, ..., 0]
...
Final   : A=[0, 1, 2, ..., n-1]
```

**Pourquoi le résultat est trié :** B est une pile LIFO. On a empilé les minimums dans l'ordre croissant. En dépilant, on obtient l'ordre décroissant. Chaque PA insère en tête de A. L'insertion en tête d'éléments décroissants produit un tableau finalement croissant de la tête vers le fond.

---

### 4.10 sort\_helpers.c

**Rôle :** fonctions partagées entre `sort_medium.c` et `sort_simple.c` (pour la reconstruction de B vers A).

---

#### `int find_max_b(t_data *d)` — ligne 15

**Symétrique de `find_min` mais sur B, cherche le maximum.**

```c
i = 1; pos = 0;
while (i < d->nb)
{
    if (d->b[i] > d->b[pos])
        pos = i;
    i++;
}
return (pos);
```

**Retour :** index du maximum dans `d->b[0..nb-1]`.

**Complexité :** O(nb).

---

#### `static void bring_max(t_data *d, int pos)` — ligne 31

**Symétrique de `bring_min` mais sur B, utilise RB et RRB.**

```c
if (pos <= d->nb / 2)
    while (pos-- > 0)
        apply_op(d, RB);
else
    while (pos++ < d->nb)
        apply_op(d, RRB);
```

**Même logique de seuil :** choisit la direction minimisant le nombre de rotations.

---

#### `void sort_b_to_a(t_data *d)` — ligne 41

**Ce qu'elle fait :** vide entièrement B vers A en extrayant toujours le maximum de B.

```c
while (d->nb)
{
    pos = find_max_b(d);   // O(nb)
    bring_max(d, pos);     // O(nb/2) en moyenne
    apply_op(d, PA);       // insere le max au sommet de A
}
```

**Invariant après chaque PA :** l'élément venant d'arriver en `a[0]` est le maximum restant de B. Les PA successifs accumulent les éléments dans l'ordre décroissant en tête de A, ce qui signifie que A se construit avec le minimum du chunk **en dernier entré = au fond** — et c'est ainsi que A finit triée.

**Exemple :** B = [5, 3, 7, 2, 1] (valeurs normalisées).
```
Iter 1 : max=7 (pos=2), bring→sommet, PA → A=[7, ...anciens...]
Iter 2 : max=5 (pos=0), PA → A=[5, 7, ...anciens...]
Iter 3 : max=3 (pos=0), PA → A=[3, 5, 7, ...anciens...]
Iter 4 : max=2 (pos=0), PA → A=[2, 3, 5, 7, ...anciens...]
Iter 5 : max=1 (pos=0), PA → A=[1, 2, 3, 5, 7, ...anciens...]
```

Les éléments de ce chunk arrivent dans A dans l'ordre croissant de gauche à droite.

**Utilisée par :** `sort_medium.c` (après chaque `process_range`) et `sort_simple.c` (phase 2, mais `sort_simple.c` utilise directement `apply_op(d, PA)` sans passer par `sort_b_to_a`).

---

### 4.11 sort\_medium.c

**Rôle :** stratégie 1 — tri par chunks O(n√n).

---

#### `static int chunk_size(int n)` — ligne 15

**Ce qu'elle fait :** retourne le plus petit entier `size` tel que `size² ≥ 8n`.

```c
size = 1;
while (size * size < 8 * n)
    size++;
return (size);
```

**Formule :** `size = ⌈√(8n)⌉ ≈ 2.83 × √n`.

**Valeurs :**

| n | 8n | size | size² |
|---|-----|------|-------|
| 5 | 40 | 7 | 49 ≥ 40 ✓ |
| 10 | 80 | 9 | 81 ≥ 80 ✓ |
| 50 | 400 | 20 | 400 ≥ 400 ✓ |
| 100 | 800 | 29 | 841 ≥ 800 ✓ |
| 200 | 1600 | 41 | 1681 ≥ 1600 ✓ |
| 500 | 4000 | 64 | 4096 ≥ 4000 ✓ |

**Pourquoi le facteur 8 :** produit des chunks plus larges que √n pur. Moins de chunks = moins d'appels à `sort_b_to_a` = moins d'opérations de rotation globales. Le facteur 8 est un paramètre empirique qui équilibre le coût de `process_range` (proportionnel à n par chunk) et le coût de `sort_b_to_a` (proportionnel à chunk_size²).

---

#### `static void process_range(t_data *d, int low, int high)` — ligne 25

**Ce qu'elle fait :** envoie sur B tous les éléments de A dont la valeur est dans `[low, high)`.

```c
int total = d->na;   // snapshot : nombre d'elements au debut de l'appel
int i = 0;
while (i < total)
{
    if (d->a[0] >= low && d->a[0] < high)
        apply_op(d, PB);       // element dans le chunk → envoyer en B
    else if (d->na > 1)
        apply_op(d, RA);       // element hors chunk → remettre au fond
    // si na==1 et hors range : ne rien faire, i++
    i++;
}
```

**Invariant de la boucle :** en exactement `total` itérations, tous les éléments initialement dans A sont visités exactement une fois.

**Preuve :** imaginons les `total` éléments initiaux sur un anneau circulaire. Chaque PB supprime l'élément courant de l'anneau (le suivant prend sa place en index 0). Chaque RA fait avancer le curseur d'une position (l'élément courant va au fond, le suivant monte). En `total` étapes, chaque position originale de l'anneau est visitée une fois. Donc tous les éléments de `[low, high)` sont nécessairement pushés.

**Cas `na == 1` hors range :** le dernier élément restant dans A n'est pas dans le chunk courant. Ni PB ni RA. On incrémente `i` et la boucle se termine naturellement. Cet élément sera traité dans un chunk ultérieur.

**Trace détaillée (n=6, chunk [2,4)) :**

```
A initial = [5, 2, 0, 3, 4, 1]  total = 6

i=0: a[0]=5, hors [2,4), na=6>1. RA. A=[2,0,3,4,1,5]
i=1: a[0]=2, dans [2,4). PB.        A=[0,3,4,1,5], B=[2]. na=5
i=2: a[0]=0, hors [2,4), na=5>1. RA. A=[3,4,1,5,0]
i=3: a[0]=3, dans [2,4). PB.        A=[4,1,5,0], B=[3,2]. na=4
i=4: a[0]=4, hors [2,4), na=4>1. RA. A=[1,5,0,4]
i=5: a[0]=1, hors [2,4), na=4>1. RA. A=[5,0,4,1]

Resultat : A=[5,0,4,1], B=[3,2]
Les deux elements de [2,4) (valeurs 2 et 3) sont en B. ✓
```

---

#### `static void push_ranges(t_data *d, int size)` — ligne 42

**Ce qu'elle fait :** itère sur les chunks de haut en bas, traitant chaque chunk puis reconstruisant depuis B.

```c
int high = d->n;
while (high > 0)
{
    int low = high - size;
    if (low < 0) low = 0;       // protection pour le dernier chunk
    process_range(d, low, high); // envoyer [low,high) sur B
    sort_b_to_a(d);              // ramener B sur A (max en tete)
    high = low;                  // passer au chunk inferieur
}
```

**Ordre des chunks (du grand vers le petit) :**

Pour n=10, size=9 :
- Chunk 1 : [1, 10) — éléments 1..9 → B, puis sort_b_to_a → A=[1,2,...,9,0]
- Chunk 2 : [0, 1) — élément 0 → B, puis sort_b_to_a → A=[0,1,2,...,9] ✓

Pour n=9, size=9 :
- Chunk 1 : [0, 9) — tout → B, puis sort_b_to_a → A=[0,1,...,8] ✓ (en un seul chunk)

**Pourquoi du grand vers le petit :** `sort_b_to_a` insère les éléments du chunk courant en TÊTE de A. Les chunks suivants (valeurs plus petites) s'inséreront encore plus en tête. Au final, les plus petites valeurs sont au sommet. C'est l'ordre croissant cherché.

**Protection `if (low < 0) low = 0` :** si `n` n'est pas un multiple de `size`, le dernier chunk est plus petit. Par exemple n=10, size=9 : le deuxième chunk serait [1-9, 1) = [-8, 1), corrigé en [0, 1).

---

#### `int sort_medium(t_data *d)` — ligne 59

```c
int sort_medium(t_data *d)
{
    if (!normalize(d))
        return (0);
    push_ranges(d, chunk_size(d->n));
    return (1);
}
```

**Pourquoi normalize est indispensable ici :** `process_range` compare `d->a[0]` à `low` et `high` qui sont des bornes de rangs entiers. Sans normalisation, les valeurs peuvent être `[INT_MIN, 42, 999]` — les bornes seraient arbitraires et le chunking ne fonctionnerait pas.

**Retour 0 si normalize échoue :** propagé jusqu'à `run_sort` dans `main.c`, qui retourne 0 → "Error" affiché.

---

### 4.12 sort\_complex.c

**Rôle :** stratégie 2 — radix sort LSD sur les rangs, O(n log n).

---

#### `static int max_bits(int n)` — ligne 15

**Ce qu'elle fait :** calcule `⌈log₂(n)⌉` = nombre de bits pour représenter les valeurs 0..n-1.

```c
bits = 0;
while (n > 1)
{
    bits++;
    n = (n + 1) / 2;   // equivalent de n = ceil(n/2)
}
return (bits);
```

**Formule `(n+1)/2` :** division entière vers le haut. Pour n impair : `(5+1)/2 = 3 = ⌈5/2⌉`. Pour n pair : `(4+1)/2 = 2 = 4/2`. La formule avance vers 1 depuis n, et le nombre d'étapes est exactement `⌈log₂(n)⌉`.

**Exemples :**

| n | Trace | bits |
|---|-------|------|
| 2 | 2→1 | 1 |
| 3 | 3→2→1 | 2 |
| 4 | 4→2→1 | 2 |
| 5 | 5→3→2→1 | 3 |
| 100 | 100→50→25→13→7→4→2→1 | 7 |
| 500 | 500→250→125→63→32→16→8→4→2→1 | 9 |

**Usage :** `bits = max_bits(d->n)` → nombre de passes radix à effectuer.

---

#### `static void radix_pass(t_data *d, int bit)` — ligne 28

**Ce qu'elle fait :** une passe du radix sort LSD sur le bit numéro `bit`.

```c
int i = 0;
while (i < d->n)            // d->n (total), pas d->na (courant)
{
    if ((d->a[0] >> bit) & 1)
        apply_op(d, RA);    // bit = 1 : garder en A, faire tourner
    else
        apply_op(d, PB);    // bit = 0 : envoyer en B
    i++;
}
while (d->nb)
    apply_op(d, PA);        // tout rapatrier depuis B
```

**Pourquoi la boucle va jusqu'à `d->n` et non `d->na` :**

Au départ, `d->na == d->n`. Chaque PB réduit `d->na` de 1. Si on boucle sur `d->na`, la condition changerait en cours de route et on s'arrêterait trop tôt. En boucling sur `d->n` (fixe), on garantit exactement `n` itérations, traitant tous les éléments initiaux.

**Extraction du bit :** `(d->a[0] >> bit) & 1`.
- `>> bit` : décale à droite de `bit` positions (isole les bits de poids fort)
- `& 1` : masque : retient uniquement le bit 0 = bit numéro `bit` de la valeur originale

**État après la boucle principale :**
- A contient les éléments avec `bit = 1`, dans leur ordre d'apparition rotatif
- B contient les éléments avec `bit = 0`, empilés dans l'ordre de passage

**`while (d->nb) apply_op(d, PA)` :** rapatrie tout B en tête de A. Le dernier élément entré dans B sort en premier (LIFO).

---

#### `int sort_complex(t_data *d)` — ligne 45

```c
int sort_complex(t_data *d)
{
    int bit;
    int bits;

    if (!normalize(d))
        return (0);
    bits = max_bits(d->n);
    bit = 0;
    while (bit < bits)
        radix_pass(d, bit++);
    return (1);
}
```

**Séquence :** normalise d'abord (valeurs → rangs [0..n-1]), calcule le nombre de passes nécessaires, puis exécute chaque passe du LSB au MSB.

**Trace complète n=4, A=[3,1,2,0] (rangs, binaires : 11,01,10,00) :**

Passe bit=0 (LSB) :
```
d->n=4 iterations :
i=0: a[0]=3(11), bit0=1. RA. A=[1,2,0,3]
i=1: a[0]=1(01), bit0=1. RA. A=[2,0,3,1]
i=2: a[0]=2(10), bit0=0. PB. A=[0,3,1], B=[2]. na=3
i=3: a[0]=0(00), bit0=0. PB. A=[3,1],   B=[0,2]. na=2
while nb=2: PA PA → A=[0,2,3,1], B=[]
```

Passe bit=1 (MSB) :
```
d->n=4 iterations :
i=0: a[0]=0(00), bit1=0. PB. A=[2,3,1], B=[0]. na=3
i=1: a[0]=2(10), bit1=1. RA. A=[3,1,2],  B=[0]
i=2: a[0]=3(11), bit1=1. RA. A=[1,2,3],  B=[0]
i=3: a[0]=1(01), bit1=0. PB. A=[2,3],    B=[1,0]. na=2
while nb=2: PA PA → A=[0,1,2,3] ✓
```

---

### 4.13 sort\_adaptive.c

**Rôle :** stratégie 3 — sélection automatique de l'algorithme selon `disorder`.

---

#### `int sort_adaptive(t_data *d)` — ligne 15

**C'est l'unique fonction du fichier.**

```c
int sort_adaptive(t_data *d)
{
    if (d->disorder < 0.2)
    {
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

**Seuil 0.2 :**

Moins de 20% des paires sont inversées. Le tableau est proche de l'ordre final. `sort_simple` exécute peu de rotations : si les minimums sont proches de leur position, `bring_min` coûte presque 0. Pour n=100 avec disorder=0.1 (~12500 inversions sur 4950 max ≈ 12 inversions réelles), le nombre d'opérations réel de `sort_simple` peut être inférieur aux ~1400 fixes de `sort_complex`.

**Seuil 0.5 :**

Au-delà de 50% d'inversions, la permutation est fortement aléatoire. `sort_complex` offre O(n log n) garanti indépendamment de la structure de l'entrée. Entre 0.2 et 0.5, `sort_medium` exploite les structures partielles que `process_range` traite efficacement.

**Structure du code :** le cas `disorder < 0.2` est traité séparément car `sort_simple` ne retourne pas de valeur d'erreur (void). Les cas 1 et 2 partagent le retour de leurs fonctions respectives.

**Mise à jour de `used_strategy` :** nécessaire ici parce que `sort_adaptive` est le point de décision réel. `set_used` dans `main.c` avait déjà calculé la même valeur, mais la convention est que `used_strategy` reflète ce qui a RÉELLEMENT été fait.

---

### 4.14 bench.c

**Rôle :** rapport de performance sur stderr quand `--bench` est actif.

---

#### `static const char *strategy_name(t_data *d)` — ligne 15

**Ce qu'elle fait :** retourne une chaîne statique décrivant la stratégie utilisée.

```c
if (d->strategy == 3 && d->used_strategy == 0) return "Adaptive -> Simple O(n^2)";
if (d->strategy == 3 && d->used_strategy == 1) return "Adaptive -> Medium O(n sqrt(n))";
if (d->strategy == 3)                           return "Adaptive -> Complex O(n log n)";
if (d->used_strategy == 0)                      return "Simple O(n^2)";
if (d->used_strategy == 1)                      return "Medium O(n sqrt(n))";
return "Complex O(n log n)";
```

**6 cas possibles :**

| strategy | used\_strategy | Résultat |
|----------|----------------|---------|
| 3 | 0 | `Adaptive -> Simple O(n^2)` |
| 3 | 1 | `Adaptive -> Medium O(n sqrt(n))` |
| 3 | 2 | `Adaptive -> Complex O(n log n)` |
| 0 | 0 | `Simple O(n^2)` |
| 1 | 1 | `Medium O(n sqrt(n))` |
| 2 | 2 | `Complex O(n log n)` |

Le troisième `if (d->strategy == 3)` sans condition sur `used_strategy` couvre le cas `used_strategy == 2` (complex), les deux premiers cas ayant déjà retourné.

---

#### `static void print_disorder(double disorder)` — ligne 30

**Ce qu'elle fait :** affiche le taux de désordre en pourcentage avec 2 décimales, sans `printf`.

```c
long value = (long)(disorder * 10000 + 0.5);
// disorder=0.4875 → value = (long)(4875.5) = 4875
ft_putstr_fd("[bench] disorder: ", 2);
ft_putnbr_fd(value / 100, 2);     // 4875/100 = 48
ft_putstr_fd(".", 2);
if (value % 100 < 10)
    ft_putstr_fd("0", 2);          // zero de tete si < 10
ft_putnbr_fd(value % 100, 2);     // 4875%100 = 75
ft_putendl_fd("%", 2);
// → "[bench] disorder: 48.75%"
```

**Technique :** multiplier par 10000 convertit le double en un entier à 4 chiffres significatifs. Le `+ 0.5` arrondit au plus proche (arrondi bancaire). On affiche ensuite les deux parties séparées par un `.`.

**Gestion du zéro de tête :** si `value % 100 < 10`, par exemple `value = 4805` → partie décimale = `05`. Sans le `"0"` préalable, on afficherait `48.5` au lieu de `48.05`.

---

#### `static void print_count(const char *name, int count)` — ligne 44

**Ce qu'elle fait :** imprime une ligne de la forme `[bench] <name>: <count>`.

```c
ft_putstr_fd("[bench] ", 2);
ft_putstr_fd(name, 2);
ft_putstr_fd(": ", 2);
ft_putnbr_fd(count, 2);
ft_putstr_fd("\n", 2);   // note : ft_putstr_fd et non ft_putendl_fd
```

**Pourquoi `ft_putstr_fd("\n")` et non `ft_putendl_fd("")` :** équivalent fonctionnellement. `ft_putendl_fd` appelle `ft_putstr_fd` puis `write("\n")` — le résultat est identique.

---

#### `static int total_ops(t_data *d)` — ligne 53

**Ce qu'elle fait :** somme `d->count[0..10]`.

```c
i = 0; total = 0;
while (i < 11)
    total += d->count[i++];
return (total);
```

11 opérations = 11 entrées dans le tableau. Retourne le nombre total d'opérations push_swap produites.

---

#### `void print_bench(t_data *d)` — ligne 65

**C'est la seule fonction publique de ce fichier.**

**Tableau statique `names[11]` :** identique à celui de `operations.c`, mais `static` local à cette fonction. Les deux déclarations sont indépendantes.

**Séquence d'affichage :**

```
1. print_disorder(d->disorder)
   → "[bench] disorder: XX.XX%"

2. ft_putstr_fd + ft_putendl_fd + strategy_name(d)
   → "[bench] strategy: Adaptive -> Complex O(n log n)"

3. print_count("total operations", total_ops(d))
   → "[bench] total operations: XXXX"

4. boucle i=0..10 : print_count(names[i], d->count[i])
   → "[bench] sa: X"
   → "[bench] sb: X"
   ... (11 lignes)
```

**Tout sur stderr (fd=2) :** séparé du flux d'opérations sur stdout (fd=1). Permet :
```bash
./push_swap --bench 3 1 4 2 5 | ./checker_linux 3 1 4 2 5
# le bench va sur stderr (terminal), les ops sur stdout (checker)
```

---

## Flux d'exécution global

```
argv = ["./push_swap", "--bench", "3", "1", "4", "2", "5"]

main()
  init_data(&d)
    d = {a=NULL, b=NULL, n=0, na=0, nb=0, disorder=0,
         strategy=3, used_strategy=3, bench=0, count=[0x11]}

  argc = 6 != 1 → continue

  parse_input(&d, 6, argv)
    malloc(int * 6) → d->a alloue
    read_values:
      argv[1]="--bench"  : is_option=1, set_option → d->bench=1
      argv[2]="3"        : is_option=0, append_value → number("3",&v)=1, v=3
                           scan a[0..na-1]=a[0..-1]=vide → pas de doublon
                           a[0]=3, na=1
      argv[3]="1"        : a[1]=1, na=2
      argv[4]="4"        : a[2]=4, na=3
      argv[5]="2"        : a[3]=2, na=4
      argv[6]="5"        : a[4]=5, na=5  (i=6 >= argc=6 → stop)
      return (na=5 > 0) = 1
    d->n = 5
    malloc(int * 5) → d->b alloue
    compute_disorder(&d) :
      total = 5*4/2 = 10
      paires (3,1):3>1 bad++, (3,4):non, (3,2):3>2 bad++, (3,5):non
             (1,4):non,       (1,2):non, (1,5):non
             (4,2):4>2 bad++, (4,5):non
             (2,5):non
      bad = 3
      disorder = 3.0/10.0 = 0.3
    return 1

  set_used(&d)
    strategy=3, disorder=0.3 : 0.2 <= 0.3 < 0.5 → used_strategy=1

  is_sorted(&d)
    a=[3,1,4,2,5] : a[0]=3 > a[1]=1 → return 0

  run_sort(&d)
    strategy=3 → sort_adaptive(&d)
      disorder=0.3 : 0.2 <= 0.3 < 0.5 → used_strategy=1
      return sort_medium(&d)
        normalize(&d) → a=[2,0,3,1,4]  (rangs de [3,1,4,2,5])
        chunk_size(5) : size=1,4,16,25,40>40 → size=7
        push_ranges(&d, 7) :
          high=5, low=max(0,5-7)=0
          process_range(&d, 0, 5) : tout en B (tous dans [0,5))
          sort_b_to_a(&d) : reconstruit A triee = [0,1,2,3,4]
          high=0 → fin
        return 1
      return 1

  ok=1, d.bench=1 → print_bench(&d)
    → "[bench] disorder: 30.00%" sur stderr
    → "[bench] strategy: Adaptive -> Medium O(n sqrt(n))" sur stderr
    → "[bench] total operations: X" sur stderr
    → details par operation sur stderr

  ok=1 → pas d'"Error"

  free_data(&d) → free(a), free(b)
  return 0  (succes)
```

---

## Analyse de complexité

### Opérations push_swap générées

| Stratégie | Formule | n=100 | n=500 |
|-----------|---------|-------|-------|
| Simple | ~n²/4 | ~2500 | ~62500 |
| Medium | ~n×√(8n) | ~1800 | ~18000 |
| Complex | ~2n×⌈log₂n⌉ | ~1400 | ~9000 |

### Complexité algorithmique des fonctions internes

| Fonction | Complexité | Type |
|----------|------------|------|
| `number` | O(longueur chaîne) | parsing |
| `append_value` (doublon) | O(n) | parsing |
| `compute_disorder` | O(n²) | mesure |
| `is_sorted` | O(n) | check |
| `heap_sort` | O(n log n) | normalize |
| `get_rank` | O(log n) | normalize |
| `normalize` | O(n log n) | pretraitement |
| `find_min` / `find_max_b` | O(n) | tri |
| `bring_min` / `bring_max` | O(n) | tri |
| `process_range` | O(n) par appel | sort_medium |
| `radix_pass` | O(n) par passe | sort_complex |

### Complexité mémoire

| Allocation | Taille |
|-----------|--------|
| `d->a` | 4 × argc octets |
| `d->b` | 4 × n octets |
| `sorted` (dans normalize, temporaire) | 4 × n octets |
| Total | O(n) |

---

## Gestion des erreurs et cas limites

| Cas | Comportement |
|-----|-------------|
| `./push_swap` (argc=1) | return 0, aucune sortie |
| Tableau trié | is_sorted=1, 0 operation imprimee, return 0 |
| 1 seul element | is_sorted=1 (boucle vide), return 0 |
| Doublon | append_value retourne 0, "Error", return 1 |
| Non-numerique (`"abc"`) | number() echoue, "Error", return 1 |
| Virgule (`"3.14"`) | '.' arrete la boucle, `*s!=0`, erreur |
| Overflow positif (`"2147483648"`) | v=2147483648 > limit=2147483647, erreur |
| INT_MAX (`"2147483647"`) | v=2147483647 = limit, accepte |
| INT_MIN (`"-2147483648"`) | v=2147483648 = limit=2147483648, accepte |
| Overflow negatif (`"-2147483649"`) | v=2147483649 > limit=2147483648, erreur |
| Signe seul (`"+"`, `"-"`) | `!*s` apres avancement, erreur |
| Chaine vide (`""`) | `!*s` immediat, erreur |
| Deux flags de strategie | selected=1 au second, return 0, erreur |
| Que des flags, aucun nombre | na=0, read_values retourne 0, erreur |
| malloc echoue | return 0, free(NULL) safe, "Error" |

---

## Gestion mémoire

### Allocations et libérations

| Pointeur | Alloue dans | Taille | Libere dans |
|----------|-------------|--------|-------------|
| `d->a` | `parse_input` ligne 76 | `sizeof(int) * argc` | `free_data` |
| `d->b` | `parse_input` ligne 80 | `sizeof(int) * d->n` | `free_data` |
| `sorted` | `normalize` ligne 81 | `sizeof(int) * d->n` | `normalize` ligne 97 |

`d` est une variable locale de `main` → sur la pile → pas de `free(&d)`.

### Chemins d'erreur sans fuite

**Chemin 1 :** `malloc(d->a)` échoue → `parse_input` retourne 0 avant malloc(d->b) → `d->b = NULL` → `free_data` appelle `free(NULL)` deux fois → OK.

**Chemin 2 :** `malloc(d->a)` OK, `read_values` échoue → même situation que chemin 1.

**Chemin 3 :** `malloc(d->a)` OK, `read_values` OK, `malloc(d->b)` échoue → `d->a` valide, `d->b = NULL` → `free_data` : `free(d->a)` OK, `free(NULL)` OK.

**Chemin 4 :** `malloc(sorted)` échoue dans `normalize` → `normalize` retourne 0, rien à libérer dans `normalize` → `sort_medium`/`sort_complex` retournent 0 → `run_sort` retourne 0 → `main` : `ok=0`, affiche "Error", appelle `free_data` → `d->a` et `d->b` libérés normalement.

---

## Défense technique — questions 42

**Q : Pourquoi des tableaux et non des listes chaînées ?**

`find_min`, `find_max_b`, et le radix sort accèdent à `a[i]` par index en O(1). Avec une liste, l'accès indexé est O(i). De plus, le radix sort lit `d->a[0]` après chaque RA — avec une liste, garder le pointeur de tête à jour à chaque RA serait plus complexe. Les décalages O(n) du tableau sont acceptables pour n ≤ 500 car le nombre d'opérations est limité.

**Q : Pourquoi `process_range` boucle sur `total` et non sur une condition comme `nb < nb_initial` ?**

La boucle sur `total` garantit que chaque élément initialement dans A est visité exactement une fois. Si on utilisait `while (nb_in_range_remaining > 0)`, on risquerait une boucle infinie si le dernier élément du chunk est inaccessible. La borne fixe est simple, correcte, et évite tout compteur supplémentaire.

**Q : Le radix sort ne devrait-il pas être stable pour fonctionner ? Or les PA inversent l'ordre de B...**

L'inversion de B à chaque passe est systématique et cohérente. La propriété de tri LSD est préservée non par une stabilité classique mais par la structure de l'inversion : pour chaque bit, tous les éléments du groupe bit=0 subissent la même inversion, maintenant leurs ordres relatifs établis par les passes précédentes. La trace pour n=4 sur toutes les 24 permutations confirme que le résultat est toujours correct.

**Q : Pourquoi calculer `disorder` sur les valeurs originales et non après normalisation ?**

Les deux donnent le même résultat : la normalisation préserve l'ordre relatif, donc les inversions. L'ordre de calcul choisi (disorder avant normalize) simplifie le code car `compute_disorder` n'a pas besoin de savoir si les valeurs sont des rangs ou non. Le moment du calcul importe surtout pour `bench.c` : le disorder affiché correspond à l'entrée originale.

**Q : Quelle est la différence entre `strategy` et `used_strategy` ?**

`strategy` = ce que l'utilisateur a spécifié (0-3, 3 si absent). `used_strategy` = ce qui a tourné. En mode adaptatif (strategy=3), l'algorithme réel est 0, 1 ou 2 selon le disorder. `bench.c` distingue `"Adaptive -> Complex"` (strategy=3, used=2) de `"Complex"` (strategy=2, used=2). Sans ce second champ, la distinction serait perdue.

**Q : Pourquoi `sort_simple` retourne `void` mais `sort_medium` et `sort_complex` retournent `int` ?**

`sort_simple` n'alloue rien dynamiquement : elle ne peut pas échouer. `sort_medium` et `sort_complex` appellent `normalize` qui fait un `malloc`. Si ce malloc échoue, la fonction retourne 0 pour propager l'erreur jusqu'à `main`.

**Q : Comment se comporte le programme si toutes les opérations sont absentes (tableau trié) ?**

`is_sorted` retourne 1 avant d'entrer dans `run_sort`. Aucune opération n'est imprimée sur stdout. Si `--bench` est actif, le rapport est quand même affiché (disorder + strategy déterminée + 0 pour toutes les opérations).
