# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#              #
#    Updated: 2026/04/14 22:51:09 by rmakende         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

# Directories
LIBFT_DIR	= libft
MLX_DIR		= minilibx-linux
INC_DIR		= include
PARSE_DIR	= src/parsing
EXEC_DIR	= src/execution
SRC_DIR		= src

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
LIBMLX		= $(MLX_DIR)/libmlx.a

# Include flags
INCLUDES	= -I$(INC_DIR) -I$(MLX_DIR) -I$(LIBFT_DIR) -I$(PARSE_DIR)

# Source files
SRC_FILES	= $(SRC_DIR)/main.c \
			  $(EXEC_DIR)/game_start.c \
			  $(EXEC_DIR)/init.c \
			  $(EXEC_DIR)/input.c \
			  $(EXEC_DIR)/menu.c \
			  $(EXEC_DIR)/menu_render.c \
			  $(EXEC_DIR)/menu_utils.c \
			  $(EXEC_DIR)/move.c \
			  $(EXEC_DIR)/update_player.c \
			  $(EXEC_DIR)/collision.c \
			  $(EXEC_DIR)/render.c \
			  $(EXEC_DIR)/raycast.c \
			  $(EXEC_DIR)/raycast_math.c \
			  $(EXEC_DIR)/pixel.c \
			  $(EXEC_DIR)/cleanup.c

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
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
