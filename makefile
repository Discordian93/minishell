# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 23:01:13 by yuliano           #+#    #+#              #
#    Updated: 2025/07/06 12:21:48 by ypacileo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# Nombre del ejecutable
NAME = minishell

# Archivos objeto
OBJS = ft_token.o main.o

# Flags de compilación: incluye depuración (-g)
CFLAGS = -Wall -Wextra -Werror -g -I./

# Flags de enlace para readline y ncurses
LDFLAGS = -lreadline -lncurses

# Librerías adicionales (libft)
LIBS = libft/libft.a

# Regla principal: compila librerías y el ejecutable
all: libs $(NAME)
	@echo "✅ DONE"

# Compila las librerías (libft)
libs:
	@make -C libft

# Compila el ejecutable principal
$(NAME): $(OBJS)
	@gcc $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(LDFLAGS)
	@echo "✅ COMPILED"

# Regla para compilar archivos .c a .o
%.o : %.c
	@gcc $(CFLAGS) -c $< -o $@

# Limpia archivos .o y limpia libft
clean:
	@make clean -C libft
	@rm -f $(OBJS)

# Limpieza total (incluye ejecutable)
fclean: clean
	@rm -f $(NAME)

# Regla para recompilar desde cero
re: fclean all

# Especifica que estas no son archivos, sino comandos
.PHONY: all clean fclean re libs
