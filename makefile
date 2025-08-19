# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 23:01:13 by yuliano           #+#    #+#              #
#    Updated: 2025/08/19 22:40:32 by yuliano          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC_DIR = src
OBJ_DIR = obj

SRCS = token.c main.c parse.c util.c bultins.c clean.c \
    runcmd.c environ.c expand.c heredoc.c check_token.c \
	count_token.c get_path.c signal.c check.c run_exec.c \
	run_pipe.c run_redir.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CFLAGS = -Wall -Wextra -Werror -g -I./
LDFLAGS = -lreadline -lncurses
LIBS = libft/libft.a

all: libs $(NAME)
	@echo "✅ DONE"

libs:
	@make -C libft

$(NAME): $(OBJS)
	@gcc $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(LDFLAGS)
	@echo "✅ COMPILED"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@gcc $(CFLAGS) -c $< -o $@

clean:
	@make clean -C libft
	@rm -f $(OBJ_DIR)/*.o

fclean: clean
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all clean fclean re libs