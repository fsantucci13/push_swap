*This project has been created as part of the 42 curriculum by fsantucc, mdutruel*

# push_swap

## Description

`push_swap` sorts a list of unique integers using two stacks (`a` and `b`) and the eleven operations permitted by the Push_swap subject. The first integer is the top of stack `a`. The program prints one operation per line; the objective is to keep the generated operation sequence as short as practical.

The project implements the four required strategies:

- **Simple** — selection-style minimum extraction, with the shorter rotation direction.
- **Medium** — rank normalization, range/chunk partitioning, then maximum extraction from stack `b`.
- **Complex** — rank normalization followed by LSD binary radix passes.
- **Adaptive** — chooses one of the three strategies from the initial disorder ratio.

The disorder is computed before any stack operation. It is the number of inverted pairs divided by the total number of pairs, so it ranges from `0` (already sorted) to `1` (reverse sorted).

## Instructions

### Build

The Makefile uses `cc` and the required `-Wall -Wextra -Werror` flags.

```sh
make
make clean
make fclean
make re
```

### Run

The default strategy is `--adaptive`.

```sh
./push_swap 4 67 3 87 23
./push_swap --simple 5 4 3 2 1
./push_swap --medium "4 67 3 87 23"
./push_swap --complex "4 67 3 87 23"
./push_swap --adaptive "4 67 3 87 23"
./push_swap --bench "4 67 3 87 23"
```

Numbers may be passed as separate arguments or as whitespace-separated tokens inside a quoted argument. Spaces, tabs, and line separators are recognized. A strategy selector may be combined with `--bench`; only one strategy selector may be supplied.

With the supplied Linux checker, a generated sequence can be verified as follows:

```sh
./push_swap --complex 4 67 3 87 23 | ./checker_linux 4 67 3 87 23
```

The checker should print `OK`. The checker binary in this archive is provided only as a local test aid; it is not part of the `push_swap` source implementation.

Operations are written to standard output. `--bench` statistics are written to standard error and are not mixed with the operation stream. Invalid input prints `Error` followed by a newline to standard error. Running the program without arguments produces no output.

## Algorithm design and complexity

The complexity classes below refer to the number of generated Push_swap operations, as required by the subject. The two stacks use `O(n)` storage. The rank-normalization helper allocates an additional `O(n)` integer array.

### Disorder metric

For every pair of positions `i < j`, the program counts one inversion when `a[i] > a[j]`. It divides this count by `n * (n - 1) / 2`; the empty-pair case returns `0`. This calculation is performed on the original stack before sorting. Its preprocessing cost is `O(n^2)` comparisons and `O(1)` auxiliary space; it does not itself generate Push_swap operations.

### Simple — `O(n^2)` operations

At each iteration, the algorithm scans stack `a` to find its minimum, rotates that minimum to the top using the shorter of `ra` and `rra`, and sends it to `b` with `pb`. Once one element remains in `a`, it moves the elements back with `pa`. There are at most `n` extraction rounds, and each round generates at most `O(n)` rotations and one push. The upper bound is therefore `O(n^2)` operations. It needs no additional rank array (`O(1)` auxiliary space beyond the two stacks).

### Medium — `O(n sqrt(n))` operations

The algorithm first replaces values with their sorted ranks. It divides the rank interval into chunks of size proportional to `sqrt(n)`, processes the chunks from high ranks to low ranks, and pushes each chunk's values from `a` to `b`. For each chunk, the remaining elements of `a` are scanned once, costing at most `O(n)` operations. The chunk placed in `b` contains at most `O(sqrt(n))` values; extracting its maximum repeatedly costs at most `O(n)` operations for that chunk. There are `O(sqrt(n))` chunks, giving an `O(n sqrt(n))` upper bound. Rank normalization uses `O(n)` auxiliary space.

### Complex — `O(n log(n))` operations

The program copies the values, heap-sorts the copy, and maps each original value to its rank using binary search. It then performs LSD radix passes over the ranks. Each bit pass examines `n` elements, using `ra` or `pb`, and returns the elements from `b` with `pa`. There are `O(log(n))` bit passes and at most `O(n)` operations per pass, giving `O(n log(n))` generated operations. The normalization array requires `O(n)` auxiliary space.

### Adaptive — disorder-based selection

The thresholds follow the subject:

- `disorder < 0.2`: Simple (`O(n^2)` operations).
- `0.2 <= disorder < 0.5`: Medium (`O(n sqrt(n))` operations).
- `disorder >= 0.5`: Complex (`O(n log(n))` operations).

The rationale is to use the baseline extraction method for nearly sorted inputs, range partitioning for intermediate disorder, and radix sorting for highly disordered inputs. The selected internal strategy is reported by `--bench`. The adaptive operation bound in each regime is the bound of the strategy selected for that regime. The disorder calculation itself is `O(n^2)` CPU comparisons, as noted above.

These strategies are heuristics for reducing the operation count; they do not claim to prove a globally minimal sequence for every possible input.

## Benchmark mode

`--bench` reports, on standard error:

- Initial disorder as a percentage with two decimal places.
- The selected strategy and its operation-model complexity class.
- The total number of operations.
- The count of each operation (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`).

The operation stream remains on standard output, so it can still be piped directly into a checker.

## Resources

- 42, *Push_swap subject*, version 1.2 (provided project subject).
- Donald E. Knuth, *The Art of Computer Programming*, Volume 3: *Sorting and Searching*.
- The 42 Norm and the C library function documentation for `malloc`, `free`, and `write`.

**Use of AI:** ChatGPT was used during this revision to review the input parser, design and check whitespace-token handling, inspect the implementation against the subject requirements, and prepare test cases and this technical explanation. The learners must review the implementation, understand it, and be able to explain and maintain every part of the submitted code.

## Contributors

| Login | Contributions |
|---|---|
| `fsantucc` | Initial architecture and implementation of the Simple, Medium, and Complex strategies; parsing, operations, normalization, disorder calculation, and benchmark code. |
| `mdutruel` | Co-development, peer review, and testing of the project. |
