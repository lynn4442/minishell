# Compiler and flags
CC      = cc
CFLAGS  = -Wall -Wextra -Werror

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I headers -I libft -I ft_printf

# Libraries
LIBFT   = libft/libft.a
FTPRINTF = ft_printf/ft_printf.a

# Source and object files
SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Final executable
NAME    = minishell

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(FTPRINTF)
    $(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(FTPRINTF) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    mkdir -p $(OBJ_DIR)
    $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
    make -C libft

$(FTPRINTF):
    make -C ft_printf

clean:
    rm -rf $(OBJ_DIR)
    make -C libft clean
    make -C ft_printf clean

fclean: clean
    rm -f $(NAME)
    make -C libft fclean
    make -C ft_printf fclean

re: fclean all

.PHONY: all clean fclean re
