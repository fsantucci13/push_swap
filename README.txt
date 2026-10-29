push_swap project

Compilation:
    make re

Normal test:
    ./push_swap 4 67 3 87 23 | ./checker_linux 4 67 3 87 23

With --complex:
    ./push_swap --complex 4 67 3 87 23 | ./checker_linux 4 67 3 87 23

Important note for zsh users:
In zsh, unquoted scalar expansion like $ARG is not split into separate words by default.
To run the exact subject-style command using ARG="4 67 3 87 23", first enable splitting in the shell:
    setopt SH_WORD_SPLIT
    ARG="4 67 3 87 23"; ./push_swap --complex $ARG | ./checker_linux $ARG

Alternatively, use Bash, or in zsh use $=ARG.
The --complex option is accepted, but this version currently uses the same sorting algorithm as the normal mode.
