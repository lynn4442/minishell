# Compiler and flags
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I headers -I libft -g

## Libraries
LIBFT   = libft/libft.a
LDFLAGS = -Llibft -lft -lreadline -lncurses

# Source and object files
SRCS	= $(wildcard $(SRC_DIR)/*.c)
OBJS	= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Final executable
NAME	= minishell

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C libft

clean:
	rm -rf $(OBJ_DIR)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean $(NAME)

.PHONY: all clean fclean re
