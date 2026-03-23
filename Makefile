##
## EPITECH PROJECT, 2026
## my_teams
## File description:
## Makefile
##

SRC = $(shell find src -type f -name "*.c")
OBJ = $(SRC:.c=.o)

NAME = my_teams

CFLAGS = -Wall -Wextra -Werror -I./include

ifeq ($(ENV), dev)
	CFLAGS += -g3 -DENV_DEV
	CC = gcc
else
	CFLAGS += -O2 -fno-builtin
	CC = epiclang
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
