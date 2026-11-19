*This project has been created as part of the 42 curriculum by <fsantucc>, <mdutruel>*

# push_swap

> **À compléter avant rendu :** remplacer les deux identifiants fictifs par les logins réels et préciser les contributions effectives de chaque apprenant.

## Description

`push_swap` sorts a list of unique integers using two stacks and only the permitted Push_swap operations. The first argument is the top of stack A. The program prints one operation per line.

The mandatory implementation contains four strategies:
- **Simple:** repeated minimum extraction, O(n²) in the Push_swap operation model.
- **Medium:** range/chunk partitioning followed by maximum extraction from B.
- **Complex:** rank compression followed by LSD binary radix sorting.
- **Adaptive:** chooses Simple when disorder is below 0.2, Medium when disorder is at least 0.2 and below 0.5, and Complex otherwise.

Disorder is calculated on the original input before any stack operation, as the number of inverted pairs divided by all possible pairs.

## Instructions

Compile with:

```sh
make
```

Examples:

```sh
./push_swap 4 67 3 87 23
./push_swap --simple 5 4 3 2 1
./push_swap --medium 4 67 3 87 23
./push_swap --complex 4 67 3 87 23
./push_swap --adaptive 4 67 3 87 23
./push_swap --bench 4 67 3 87 23
```

Operations are written to standard output. Benchmark information is written to standard error. Invalid input prints `Error` to standard error.

## Algorithm details

### Simple

At each iteration, find the minimum in A, rotate it to the top using the shorter direction, then push it to B. Once A contains one element, move B back to A. The strategy is a quadratic baseline in operation count.

### Medium

Normalize values to ranks, then process consecutive ranges of approximately √n ranks from highest to lowest. Each range scans the remaining A once and pushes only its members to B. B contains at most √n values; extracting its maximum repeatedly costs O((√n)²) operations for that range. There are O(√n) ranges, so both scanning and chunk sorting are bounded by O(n√n) generated operations. Processing ranges from high to low places each completed, ascending chunk above the previously completed higher chunks. Auxiliary rank storage is O(n).

### Complex

Copy the input values, heapsort the copy, and map each input to its rank using binary search. This rank compression costs O(n log n) comparisons and O(n) auxiliary space. Then perform LSD radix passes over the ranks. Each pass processes n elements and restores B to A, giving O(n log n) generated Push_swap operations.

### Adaptive

The initial disorder ratio selects the internal strategy:
- disorder < 0.2: Simple;
- 0.2 ≤ disorder < 0.5: Medium;
- disorder ≥ 0.5: Complex.

The thresholds follow the supplied subject. The benchmark reports the actual internal strategy.

## Resources

- The supplied Push_swap subject, version 1.2.
- Knuth, *The Art of Computer Programming*, sorting and searching.
- 42 project documentation and peer review.

AI assistance was used to review the subject requirements, propose a modular implementation, and generate test cases. The learners must review, understand, test, and be able to explain every submitted function and algorithm.

## Group requirements

Complete this section with both learners' logins and describe each learner's actual contribution. The subject requires exactly two learners and meaningful contributions from both.
