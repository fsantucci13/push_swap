# Push_swap validation report

Validation performed against the supplied Push_swap subject v1.2 and the supplied `checker_linux` binary.

## Subject checklist

- [x] C program named `push_swap`.
- [x] Two-stack operations only in the generated operation stream.
- [x] Four distinct strategies: Simple, Medium, Complex, Adaptive.
- [x] Runtime selectors `--simple`, `--medium`, `--complex`, `--adaptive`; Adaptive is the default.
- [x] Initial disorder measured before sorting using inverted pairs / total pairs.
- [x] Adaptive thresholds: `< 0.2`, `[0.2, 0.5)`, `>= 0.5`.
- [x] `--bench` reports disorder, strategy and complexity, total operations, and all eleven operation counts on stderr.
- [x] Operations remain on stdout, one per line.
- [x] No-argument invocation produces no output.
- [x] Invalid input prints `Error\n` on stderr.
- [x] Makefile has `$(NAME)`, `all`, `clean`, `fclean`, `re`; default compiler is `cc` with `-Wall -Wextra -Werror`.
- [x] No global variables found in the submitted C sources.
- [x] README includes the required first line, Description, Instructions, Resources, AI-use disclosure, algorithm justifications, and contributor roles.

## Build checks

- GCC (`cc`) with `-Wall -Wextra -Werror`: PASS.
- Clang with `-Wall -Wextra -Werror`: PASS.
- UndefinedBehaviorSanitizer build and checker smoke test: PASS.
- Second `make` after a successful build: no relink observed.
- `norminette`: not installed in the test environment; must still be run in the 42 environment.
- `valgrind`: not installed in the test environment; must still be run in the 42 environment.

## Structural audit

- Every `.c` file has at most 5 functions.
- Every function has at most 25 physical source lines, including blank lines, signature, and braces.
- Every source line in `.c` and `.h` is at most 80 characters.

## Parser checks

Valid input cases: 7/7 PASS, including quoted whitespace-separated values, multiple quoted arguments, tabs/newlines, signed integers, `INT_MIN`, and `INT_MAX`.

Invalid input cases: 12/12 correctly rejected, including empty arguments, whitespace-only arguments, duplicates, overflow, non-numeric tokens, malformed signs, conflicting strategy flags, and flags without numbers.

## Correctness checks with checker_linux

- Exhaustive permutations of sizes 1 through 6 across default, Simple, Medium, Complex, and Adaptive: 4,365/4,365 returned `OK`.
- Random 100-value inputs: Adaptive 150/150, Medium 60/60, Complex 60/60, Simple 100/100 returned `OK`.
- Random 500-value inputs: Adaptive 150/150, Medium 60/60, Complex 60/60, Simple 10/10 returned `OK`.
- Additional edge tests: sorted, reverse-sorted, minimum/maximum integer values, and quoted whitespace inputs passed.

## Operation-count samples

These are measured samples, not guarantees for every possible input.

| Size | Strategy | Samples | Minimum | Median | Maximum |
|---:|---|---:|---:|---:|---:|
| 100 | Adaptive | 150 | 806 | 1,084 | 1,084 |
| 100 | Medium | 60 | 801 | 856 | 903 |
| 100 | Complex | 60 | 1,084 | 1,084 | 1,084 |
| 100 | Simple | 100 | 1,204 | 1,482 | 1,659 |
| 500 | Adaptive | 150 | 6,784 | 8,280 | 8,837 |
| 500 | Medium | 60 | 8,255 | 8,479 | 8,663 |
| 500 | Complex | 60 | 6,784 | 6,784 | 6,784 |
| 500 | Simple | 10 | 31,202 | 32,140 | 33,524 |

Against the subject's mandatory thresholds (<2,000 operations for 100 random values and <12,000 for 500 random values), all sampled Adaptive cases passed. The 100-value Adaptive samples also stayed below the 1,500-operation good-performance target. The 500-value Adaptive samples stayed below 12,000, but some exceeded the 8,000 good-performance target. Complex stayed below 8,000 in the sampled 500-value cases. Simple is a baseline quadratic strategy and is not intended to meet the 500-value performance target.

## Remaining manual checks

1. Run `norminette *.c *.h` in the 42 environment.
2. Run `valgrind --leak-check=full --show-leak-kinds=all ./push_swap ...` on valid and invalid inputs.
3. Confirm contributor roles and logins with both learners before submission.
4. Ensure both learners can explain every function and the complexity arguments during peer evaluation.
