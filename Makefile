# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:20:24 by francesca         #+#    #+#              #
#    Updated: 2025/06/07 20:55:21 by skayed           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === Colors ===
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
RESET	= \033[0m
BLUE	= \033[0;34m

# === Project name ===
NAME	= minishell

# === Compiler settings ===
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iheader -g

# === Libraries ===
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -lreadline

# === Directories ===
SRC_DIR = .
OBJ_DIR = obj

# === Sources ===
SRC =	main.c \
		env/init_env.c \
		parser/lexer_utils.c \
		parser/lexer.c \
		parser/parser.c \
		parser/build_pipeline.c \
		parser/utils.c \
		parser/quote_handling.c \
		parser/backslash.c \

# === Object files ===
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# === Rules ===
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
