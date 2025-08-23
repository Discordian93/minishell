# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/26 14:52:38 by ypacileo          #+#    #+#              #
#    Updated: 2024/07/13 14:03:21 by ypacileo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
LIB = ar rcs
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf
OBJ = ft_atoi.o ft_bzero.o ft_calloc.o ft_isalnum.o ft_isalpha.o\
ft_isascii.o ft_isdigit.o ft_isprint.o ft_itoa.o ft_memchr.o\
ft_memcmp.o ft_memcpy.o ft_memmove.o ft_memset.o ft_putchar_fd.o\
ft_putendl_fd.o ft_putnbr_fd.o ft_putstr_fd.o ft_split.o ft_strampi.o\
ft_strchr.o ft_strdup.o ft_striteri.o ft_strjoin.o ft_strlcat.o\
ft_strlcpy.o ft_strlen.o ft_strncmp.o ft_strnstr.o ft_strrchr.o\
ft_strtrim.o ft_substr.o ft_tolower.o ft_toupper.o
INCLUDE = libft.h
all: $(NAME)

$(NAME) : $(OBJ) $(INCLUDE)
	$(LIB) $(NAME) $(OBJ)

%.o : %.c
	gcc  -c  $(CFLAGS) $< -o $@

clean :
	${RM} ${OBJ}

fclean : clean
	${RM} ${NAME}

re : fclean all

.PHONY: all clean fclean re