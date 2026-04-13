# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#              #
#    Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g
RM			= rm -f

# Directories
LIBFT_DIR	= libft
MLX_DIR		= src/minilibx-linux
INC_DIR		= include
PARSE_DIR	= parsing
SRC_DIR		= src

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
LIBMLX		= $(MLX_DIR)/libmlx.a

# Include flags
INCLUDES	= -I$(INC_DIR) -I$(MLX_DIR) -I$(LIBFT_DIR) -I$(PARSE_DIR)

# Source files
SRC_FILES	= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/game_start.c \
			  $(SRC_DIR)/init.c \
			  $(SRC_DIR)/input.c \
			  $(SRC_DIR)/move.c \
			  $(SRC_DIR)/update_player.c \
			  $(SRC_DIR)/collision.c \
			  $(SRC_DIR)/render.c \
			  $(SRC_DIR)/raycast.c \
			  $(SRC_DIR)/raycast_math.c \
			  $(SRC_DIR)/pixel.c \
			  $(SRC_DIR)/cleanup.c

PARSE_FILES	= $(PARSE_DIR)/parse_main.c \
			  $(PARSE_DIR)/parse_file.c \
			  $(PARSE_DIR)/parse_header.c \
			  $(PARSE_DIR)/parse_textures.c \
			  $(PARSE_DIR)/parse_colors.c \
			  $(PARSE_DIR)/parse_map_build.c \
			  $(PARSE_DIR)/parse_map_validate.c \
			  $(PARSE_DIR)/parse_player.c \
			  $(PARSE_DIR)/parse_free.c \
			  $(PARSE_DIR)/parse_utils.c

OBJS		= $(SRC_FILES:.c=.o) $(PARSE_FILES:.c=.o)

# Linker flags
LDFLAGS		= -L$(MLX_DIR) -lmlx -L$(LIBFT_DIR) -lft -lX11 -lXext -lm

all: $(LIBFT) $(LIBMLX) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(LIBMLX):
	cd $(MLX_DIR) && chmod +x configure && ./configure

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
