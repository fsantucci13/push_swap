
NAME = push_swap

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I.

SRC = src/main.c \
      src/errors.c \
      src/parsing/parse.c \
      src/parsing/parse_read.c \
      src/parsing/parse_utils.c \
      src/operations/operations_a.c \
      src/operations/operations_b.c \
      src/operations/operations_both.c \
      src/operations/operations_helpers.c \
      src/sorting/ranks.c \
      src/sorting/sort_simple.c \
      src/sorting/sort_medium.c \
      src/sorting/sort_medium_back.c \
      src/sorting/sort_complex.c \
      src/sorting/sort_adaptive.c \
      src/bench/bench.c \
      src/bench/bench_helpers.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
