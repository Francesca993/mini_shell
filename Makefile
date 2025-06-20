# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:20:24 by francesca         #+#    #+#              #
#    Updated: 2025/06/20 18:09:11 by skayed           ###   ########.fr        #
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
CFLAGS = -Wall -Wextra -Werror -g -gdwarf-4 -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline

# === Libraries ===
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

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
		parser/exp_var.c \
		execution/args_processing.c \
		execution/exec_pipeline.c \
		builtins/exit.c \
		builtins/signal.c \
		builtins/builtins.c \
		builtins/pwd.c \
		builtins/cd.c \
		builtins/unset.c \
		builtins/export.c \
		builtins/export_utils.c \
		builtins/echo.c \

# === Object files ===
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# === Rules ===
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

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

valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --suppressions=supp.supp ./$(NAME)

.PHONY: all clean fclean re
