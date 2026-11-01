*This project has been created as part of the 42 curriculum by <login1>, <login2>.*

# Push_swap

## Description

`push_swap` is a C program that prints a sequence of operations capable of sorting
integers in ascending order in stack A, using only two stacks (A and B) and the
Push_swap instruction set. Stack B starts empty. The first supplied integer is the
top of stack A. The program prints one operation per line and sends diagnostics
only to standard error.

This version of the subject requires four sorting strategies, an inversion-based
disorder metric, runtime strategy selection and an optional benchmark mode.

## Instructions

### Compilation

```sh
make
```

The Makefile compiles with `cc -Wall -Wextra -Werror`. Other supported targets:

```sh
make clean   # remove object files
make fclean  # remove object files and executable
make re      # clean and rebuild
```

### Execution

```sh
./push_swap 4 67 3 87 23
./push_swap --simple 4 67 3 87 23
./push_swap --medium 4 67 3 87 23
./push_swap --complex 4 67 3 87 23
./push_swap --adaptive 4 67 3 87 23
./push_swap --bench --adaptive 4 67 3 87 23
```

The default strategy is `--adaptive`. The strategy selector and `--bench` can be
placed before or after the integer arguments. Arguments may be separate integers
or a quoted, whitespace-separated string:

```sh
./push_swap "4 67 3 87 23"
```

No arguments produce no output. Already sorted input also produces no operations.
Invalid integers, overflow, empty arguments and duplicates produce `Error` on
standard error.

### Checking a result

The provided Linux checker was used for validation; it is not part of this
submission because `checker` is the bonus program, which was explicitly excluded.

```sh
ARG="4 67 3 87 23"
./push_swap --complex $ARG | ./checker_linux $ARG
```

Expected checker result: `OK`.

### Benchmark mode

`--bench` keeps the operation stream on standard output and writes metrics to
standard error. It reports disorder as a percentage with two decimals, the
selected strategy and its operation-model complexity, the total operation count,
and the count of all eleven instruction types.

## Strategies and algorithm justification

### Disorder

Before any stack operation, the program counts inversions in the original A:

`disorder = inversions / (n * (n - 1) / 2)`.

An inversion is a pair `(i, j)` for which `i < j` and `A[i] > A[j]`. The value is
between 0 and 1. Zero means sorted; one means reverse-sorted. For zero or one
integer, disorder is defined as zero.

### Simple — O(n²) operations

The algorithm repeatedly finds the smallest value in A, rotates it to the top
using the shorter direction (`ra` or `rra`), and pushes it to B (`pb`). Once A is
empty, it pushes every value back with `pa`. Since minima are pushed in ascending
order, B contains them in reverse order; pushing them back produces ascending A.

Finding each minimum examines the remaining stack, and bringing it to the top can
require a linear number of rotations. The upper bound in generated stack
operations is O(n²).

### Medium — O(n√n) operations

The algorithm divides the normalized ranks into consecutive chunks of width
`ceil(sqrt(n))`. For each chunk, it scans the current A once. Values inside the
chunk are pushed to B; values outside it are rotated to the bottom of A. Values
from the lower half of a chunk are rotated in B to keep chunk groups arranged for
the return phase. Finally, the largest remaining value in B is brought to the top and pushed to A.
The partition phase scans at most n elements for each of about √n chunks. During
that phase, values are grouped in B: the upper half of a chunk remains near its
top while the lower half is rotated toward the bottom. This grouping is what keeps
the maximum-selection rotations localized to a chunk-sized region during the
return phase. Thus the intended operation bound is O(n√n), rather than the O(n²)
bound of an ungrouped repeated-maximum extraction.

### Complex — O(n log n) operations

The program first replaces each input integer with its rank from 0 to n-1. This
preserves numerical order while avoiding signed-integer bit complications. The
binary LSD Radix Sort then processes one rank bit per pass:

- If the current top rank has a 1 in the current bit, rotate A (`ra`).
- Otherwise push it to B (`pb`).
- After exactly the original number of A elements has been processed, push B
  back to A (`pa`).

There are `ceil(log2(n))` bit passes and each pass processes n elements, so the
number of generated operations is O(n log n).

### Adaptive

The program measures disorder once, before sorting, and applies the required
thresholds:

| Disorder | Internal method | Target operation complexity |
|---|---|---|
| `< 0.20` | Simple | O(n²) |
| `0.20` to `< 0.50` | Medium | O(n√n) |
| `>= 0.50` | Complex | O(n log n) |

The thresholds divide nearly sorted, partly disordered and highly disordered
inputs. The benchmark identifies the requested strategy as `Adaptive` and also
shows which internal method was selected.

The algorithms are practical stack-operation heuristics. They do not prove a
globally shortest possible instruction sequence for every input permutation.

## Source files

| File | Responsibility |
|---|---|
| `push_swap.h` | Shared structure, constants from system headers and prototypes. |
| `main.c` | Initializes state, parses, measures disorder, selects and runs the strategy, frees memory. |
| `parse.c` | Recognizes options, allocates stacks, fills them and rejects duplicates. |
| `parse_utils.c` | Whitespace, string comparison, integer conversion and token counting. |
| `parse_read.c` | Reads each numeric token safely from an argument. |
| `errors.c` | Prints the required error and checks whether A is sorted. |
| `ranks.c` | Computes disorder and maps values to order-preserving ranks. |
| `operations_a.c` | `sa`, `pa`, `ra`, `rra`. |
| `operations_b.c` | `sb`, `pb`, `rb`, `rrb`. |
| `operations_both.c` | `ss`, `rr`, `rrr`. |
| `operations_helpers.c` | Silent B rotations used by combined instructions. |
| `sort_simple.c` | Minimum-extraction strategy. |
| `sort_medium.c` | Chunk partitioning phase. |
| `sort_medium_back.c` | Rebuilds A from B in descending-maximum order. |
| `sort_complex.c` | Binary Radix passes. |
| `sort_adaptive.c` | Disorder thresholds and internal method selection. |
| `bench.c`, `bench_helpers.c` | Benchmark report on standard error. |
| `Makefile` | Compilation and cleanup rules. |

## Norm and memory

The source is organized with no more than five functions per C file. Function
bodies are kept within the requested 25-line limit. The program uses no global variables. Stack arrays and temporary
rank storage are heap-allocated and freed. The implementation uses only
`write`, `malloc`, `free` and `exit` from the permitted system-call set; the
benchmark formatter is implemented locally.

Run `norminette` in the school environment for the authoritative style check.
The Norminette executable was not installed in the build environment used for
these tests, so a successful official Norminette run cannot be claimed here.

## Resources

- The Push_swap subject supplied with this project.
- Donald E. Knuth, *The Art of Computer Programming*, sorting and searching.
- C language reference: integer ranges, pointers, arrays and dynamic allocation.
- Algorithm references: inversion count, binary LSD Radix Sort and chunk/bucket
  partitioning.

### Use of AI

AI assistance was used to interpret the supplied subject, organize the required
modules, explain the data flow and algorithms, and propose test cases. The
resulting implementation was compiled and tested against the supplied checker.
Both learners must review the code, understand it and be able to explain it
before submission.

## Group contributions — complete before submission

- `<login1>`: replace this line with the first learner's actual contribution.
- `<login2>`: replace this line with the second learner's actual contribution.

Both learners must be listed as repository contributors and must be able to
explain every submitted part during the defense.
