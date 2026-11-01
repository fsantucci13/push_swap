NAME = push_swap
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = main.c utils.c flags.c parse.c disorder.c normalize.c operations.c \
	sort_simple.c sort_helpers.c sort_medium.c sort_complex.c \
	sort_adaptive.c bench.c
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
