# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 17:14:51 by mcygan            #+#    #+#              #
#    Updated: 2024/10/22 16:17:58 by dzapata          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	libft.a

CC 			=	cc
CFLAGS		=	-Wall -Wextra -Werror -fsanitize=address -g

SRC_DIR		=	src/
SRC_FILES 	= 	ft_isalpha.c \
				ft_isdigit.c \
				ft_isalnum.c \
				ft_isspace.c \
				ft_isprint.c \
				ft_isascii.c \
				ft_putchar_fd.c \
				ft_putnbr_fd.c \
				ft_putstr_fd.c \
				ft_putendl_fd.c \
				ft_strlen.c \
				ft_strlcpy.c \
				ft_strlcat.c \
				ft_toupper.c \
				ft_tolower.c \
				ft_strchr.c \
				ft_strrchr.c \
				ft_strnstr.c \
				ft_strcmp.c \
				ft_strncmp.c \
				ft_atoi.c \
				ft_itoa.c \
				ft_bzero.c \
				ft_memset.c \
				ft_memcpy.c \
				ft_memmove.c \
				ft_memchr.c \
				ft_memcmp.c \
				ft_calloc.c \
				ft_strdup.c \
				ft_substr.c \
				ft_strjoin.c \
				ft_strtrim.c \
				ft_split.c \
				ft_strmapi.c \
				ft_striteri.c \
				ft_lstsize.c \
				ft_lstlast.c \
				ft_lstnew.c \
				ft_lstadd_front.c \
				ft_lstadd_back.c \
				ft_lstdelone.c \
				ft_lstclear.c \
				ft_lstiter.c \
				ft_lstmap.c
SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR		=	obj/
OBJ 		=	$(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))


$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@echo "# compiling libft"
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re
