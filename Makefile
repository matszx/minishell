# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcygan <mcygan@student.s19.be>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/02 15:24:36 by mcygan            #+#    #+#              #
#    Updated: 2024/09/02 15:37:22 by mcygan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	minishell

CC 			=	clang
CFLAGS 		=	-Wall -Wextra -Werror -fsanitize=address -g -I ~/.brew/opt/readline/include
READLINE	=	-lreadline -L ~/.brew/opt/readline/lib

LIB_DIR 	=	libft/
LIB 		=	$(LIB_DIR)libft.a

SRC_DIR		= 	src/
SRC_FILES 	=	main.c \
        		prompt.c \
				sig_handler.c
SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR		=	obj/
OBJ			=	$(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@make -C $(LIB_DIR)
	# compiling minishell
	@$(CC) $(CFLAGS) $(LIB) $(READLINE) $(OBJ) -o $(NAME)
	# minishell ready

clean :
	# cleaning up
	@rm -rf $(OBJ_DIR)
	@make fclean -C $(LIB_DIR)

fclean : clean
	# deleting $(NAME)
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean flcean re
