NAME = push_swap

SRC = push_swap.c check_input.c utils.c create_stack.c \
	basic_moves.c rotate_moves.c complex_moves.c \
	complex_moves_prep.c complex_moves_do.c
OBJS = $(SRC:.c=.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

complex: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all complex clean fclean re
