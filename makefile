# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 23:01:13 by yuliano           #+#    #+#              #
#    Updated: 2025/08/22 15:10:25 by yuliano          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC_DIR = src
OBJ_DIR = obj

SRCS = token.c main.c parse.c util.c bultins.c clean.c \
    runcmd.c handle_environ.c env_aux.c my_setenv.c add_to_environ.c \
	environ.c environ2.c environ3.c expand.c heredoc.c check_token.c \
	token_count.c get_path.c signal.c check.c run_exec.c \
	run_pipe.c run_redir.c builtin_parents.c builtin_child.c \
	parse_pipe.c parse_redi.c token_util.c clean_tree.c heredoc_utils.c \
	heredoc1.c expand2.c

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