# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: francesca <francesca@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:20:24 by francesca         #+#    #+#              #
#    Updated: 2025/05/09 11:12:48 by francesca        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Se usi readline e termcap, ricordati di aggiungere nel tuo Makefile:
#LIBS = -lreadline -ltermcap
#Assicurati che tutti i .c includano #include "minishell.h" (e header specifici se servono)
#Tieni i .h coerenti: ogni modulo (parser.c) può avere il suo (parser.h)
#Se usi sottocartelle per gli header, ricorda di aggiungere -Iparser -Iexecutor ... nel INCLUDES

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iheader

SRC 	= 	main.c \
			env/init_env.c \
			
OBJ = $(SRC:.c=.o)

# Libft
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline

# Esegui il Makefile di libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all