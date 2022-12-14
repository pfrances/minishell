# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pfrances <pfrances@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/17 13:52:28 by pfrances          #+#    #+#              #
#    Updated: 2023/01/08 16:16:16 by pfrances         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
LIBRARY = -L/usr/lib/x86_64-linux-gnu -lreadline
INCLUDE = -I includes -I/usr/include/readline/

FT_PRINTF_DIR = ./ft_printf
FT_PRINTF = $(FT_PRINTF_DIR)/ft_printf.a
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
MAIN_SRCS = $(SRCS_DIR)/main.c
MAIN_OBJS = $(subst $(SRCS_DIR), $(OBJS_DIR), $(MAIN_SRCS:.c=.o))
SRCS = $(MAIN_SRCS) $(LEXER_SRCS) $(PARSER_SRCS)
OBJS = $(MAIN_OBJS) $(LEXER_OBJS) $(PARSER_OBJS)

##################################LEXER########################################
LEXER_SRCS_DIR = $(SRCS_DIR)/lexer
LEXER_OBJS_DIR = $(OBJS_DIR)/lexer
LEXER_SRCS = $(addprefix $(LEXER_SRCS_DIR)/,	lexer_frees.c			\
												ft_split_charset.c		\
												lexer_list.c			\
												lexer_tools.c			\
												lexer.c					\
												manage_tokens.c)
LEXER_OBJS = $(subst $(LEXER_SRCS_DIR), $(LEXER_OBJS_DIR), $(LEXER_SRCS:.c=.o))
###############################################################################
##################################PARSER########################################
PARSER_SRCS_DIR = $(SRCS_DIR)/parser
PARSER_OBJS_DIR = $(OBJS_DIR)/parser
PARSER_SRCS = $(addprefix $(PARSER_SRCS_DIR)/,	parser.c				\
												parse_nodes1.c			\
												parse_nodes2.c)
PARSER_OBJS = $(subst $(PARSER_SRCS_DIR), $(PARSER_OBJS_DIR), $(PARSER_SRCS:.c=.o))
###############################################################################

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(FT_PRINTF)
	$(CC) $(CFLAGS) $(OBJS) $(LIBRARY) $(INCLUDE) $(LIBFT) $(FT_PRINTF) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBRARY) -c $< -o $@

$(LEXER_OBJS_DIR)/%.o: $(LEXER_SRCS_DIR)/%.c
	@mkdir -p $(LEXER_OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBRARY) -c $< -o $@

$(PARSER_OBJS_DIR)/%.o: $(PARSER_SRCS_DIR)/%.c
	@mkdir -p $(PARSER_OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBRARY) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) bonus

$(FT_PRINTF):
	make -C $(FT_PRINTF_DIR)

clean:
	rm -rf $(OBJS_DIR)
	make -C $(LIBFT_DIR) clean
	make -C $(FT_PRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)
	rm -f $(FT_PRINTF)

re: fclean all

.PHONY: all clean fclean re