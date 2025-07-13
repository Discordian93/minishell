# Makefile for minishell parser

NAME = test_parser
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Source files
SRCS = lexer.c \
       parser.c \
       utils.c \
       test_parser.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = minishell.h

# Default target
all: $(NAME)

# Build the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Compile source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Clean everything
fclean: clean
	rm -f $(NAME)

# Rebuild
re: fclean all

.PHONY: all clean fclean re