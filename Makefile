# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/02 15:24:36 by mcygan            #+#    #+#              #
#    Updated: 2024/10/20 20:57:05 by dzapata          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Variables =====================================================================

NAME 			=	minishell

CC 				=	cc

CFLAGS 			=	-Wall -Wextra -fsanitize=address -g \
					-I./libft/inc -I~/.brew/opt/readline/include
					
INC				=	-L./libft -lft \
					-L~/.brew/opt/readline/lib -lreadline \

LIB_DIR 		=	libft/

SRC_DIR			=	src/

SRC_FILES 		=	main.c \
					prompt.c \
					expander.c \
					parser.c \
					builtins.c \
					env.c \
					env_utils.c \
					signals.c \
					redirect.c \
					execute.c \
					errors.c \
					builtins_utils.c
					
SRC				=	$(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR			=	obj/

OBJ				=	$(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

#Functions =======================================================================

$(OBJ_DIR)%.o	:	$(SRC_DIR)%.c
					@$(CC) $(CFLAGS) -c $< -o $@

all				:	$(OBJ_DIR) $(NAME)

$(OBJ_DIR)		:
					@mkdir -p $(OBJ_DIR)

$(NAME)			:	$(OBJ)
					@make -sC $(LIB_DIR)
					# compiling $(NAME)
					@$(CC) $(CFLAGS) $(OBJ) $(INC) -o $(NAME)
					# ready

clean			:
					# cleaning up
					@rm -rf $(OBJ_DIR)
					@make fclean -sC $(LIB_DIR)

fclean			:	clean
					# $(NAME) deleted
					@rm -f $(NAME)

re				:	fclean all

.PHONY			:	all clean flcean re
