NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror
THREAD_FLAGS = -pthread

SRC = coders/main.c \
	  coders/parser.c \
	  coders/init.c \
	  coders/coder.c \
	  coders/simulation.c \
	  coders/utils.c \
	  coders/utils2.c \
	  coders/heap.c \
	  coders/heap2.c \
	  coders/heap3.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) $(OBJ) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re