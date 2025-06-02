# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Target library name
NAME = libft.a

# Source files
SRCS = ft_char_utils.c \
       ft_validation.c \
       ft_string_conv.c \
       ft_string_cmp.c \
       ft_string_copy.c \
       ft_string_join.c \
       ft_string_basic.c \
       ft_string_extract.c \
       ft_array_utils.c \
       ft_memory_utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header file
HEADER = libft.h

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
