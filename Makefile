NAME		= cub3D
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g

INC_DIR		= include
LIBFT_DIR	= libft
MLX_DIR		= minilibx-linux

INCLUDES	= -I $(INC_DIR) -I $(LIBFT_DIR) -I $(MLX_DIR)

LIBFT		= $(LIBFT_DIR)/libft.a
MLX			= $(MLX_DIR)/libmlx.a
# Depending on the system, minilibx might output libmlx_Linux.a, fallback provided in linker args if needed,
# but standard 42 minilibx-linux generates libmlx.a or libmlx_Linux.a, we'll configure rules.
LIBS		= -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

SRC_EXEC 	= src/main.c \
			  src/cleanup.c \
			  src/collision.c \
			  src/game_start.c \
			  src/init.c \
			  src/input.c \
			  src/move.c \
			  src/pixel.c \
			  src/raycast.c \
			  src/raycast_math.c \
			  src/render.c \
			  src/update_player.c

SRC_PARSE	= src/parsing/parse_colors.c \
			  src/parsing/parse_file.c \
			  src/parsing/parse_free.c \
			  src/parsing/parse_header.c \
			  src/parsing/parse_main.c \
			  src/parsing/parse_map_build.c \
			  src/parsing/parse_map_validate.c \
			  src/parsing/parse_player.c \
			  src/parsing/parse_textures.c \
			  src/parsing/parse_utils.c

SRCS		= $(SRC_EXEC) $(SRC_PARSE)
OBJS		= $(SRCS:.c=.o)

all: $(MLX) $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean; fi
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
