NAME = push_swap
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = main.c errors.c parse.c parse_utils.c parse_read.c ranks.c operations_a.c operations_b.c \
	operations_both.c operations_helpers.c sort_simple.c sort_medium.c sort_medium_back.c \
	sort_complex.c sort_adaptive.c bench.c bench_helpers.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
