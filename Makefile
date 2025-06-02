# Compiler and flags
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

# Directories
SRC_DIR = src
PARSER_DIR = src/parser
ENV_DIR = src/env
ECHO_DIR = src/echo
SIGNALS_DIR = src/signals
UNSET_DIR = src/unset
CD_DIR = src/cd
EXPORT_DIR = src/export
QUOTES_DIR = src/quotes
REDIRECTIONS_DIR = src/redirections
PIPES_DIR = src/pipes
EXECUTION_DIR = src/execution
PWD_DIR = src/pwd
EXIT_DIR = src/exit
CORE_DIR = src/core
UTILS_DIR = src/utils
OBJ_DIR = obj
PARSER_OBJ_DIR = obj/parser
ENV_OBJ_DIR = obj/env
ECHO_OBJ_DIR = obj/echo
SIGNALS_OBJ_DIR = obj/signals
UNSET_OBJ_DIR = obj/unset
CD_OBJ_DIR = obj/cd
EXPORT_OBJ_DIR = obj/export
QUOTES_OBJ_DIR = obj/quotes
REDIRECTIONS_OBJ_DIR = obj/redirections
PIPES_OBJ_DIR = obj/pipes
EXECUTION_OBJ_DIR = obj/execution
PWD_OBJ_DIR = obj/pwd
EXIT_OBJ_DIR = obj/exit
CORE_OBJ_DIR = obj/core
UTILS_OBJ_DIR = obj/utils
INCLUDES = -I headers -I libft -I src/parser -I src/env -I src/echo -I src/signals -I src/unset -I src/cd -I src/export -I src/quotes -I src/redirections -I src/pipes -I src/execution -I src/pwd -I src/exit -I src/core -I src/utils -g

## Libraries
LIBFT   = libft/libft.a
LDFLAGS = -Llibft -lft -lreadline -lncurses

# Source files explicitly listed
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
PARSER_FILES = $(wildcard $(PARSER_DIR)/*.c)
ENV_FILES = $(wildcard $(ENV_DIR)/*.c)
ECHO_FILES = $(wildcard $(ECHO_DIR)/*.c)
SIGNALS_FILES = $(wildcard $(SIGNALS_DIR)/*.c)
UNSET_FILES = $(wildcard $(UNSET_DIR)/*.c)
CD_FILES = $(wildcard $(CD_DIR)/*.c)
EXPORT_FILES = $(wildcard $(EXPORT_DIR)/*.c)
QUOTES_FILES = $(wildcard $(QUOTES_DIR)/*.c)
REDIRECTIONS_FILES = $(wildcard $(REDIRECTIONS_DIR)/*.c)
PIPES_FILES = $(wildcard $(PIPES_DIR)/*.c)
EXECUTION_FILES = $(wildcard $(EXECUTION_DIR)/*.c)
PWD_FILES = $(wildcard $(PWD_DIR)/*.c)
EXIT_FILES = $(wildcard $(EXIT_DIR)/*.c)
CORE_FILES = $(wildcard $(CORE_DIR)/*.c)
UTILS_FILES = $(wildcard $(UTILS_DIR)/*.c)

# Object files
MAIN_OBJS = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
PARSER_OBJS = $(PARSER_FILES:$(PARSER_DIR)/%.c=$(PARSER_OBJ_DIR)/%.o)
ENV_OBJS = $(ENV_FILES:$(ENV_DIR)/%.c=$(ENV_OBJ_DIR)/%.o)
ECHO_OBJS = $(ECHO_FILES:$(ECHO_DIR)/%.c=$(ECHO_OBJ_DIR)/%.o)
SIGNALS_OBJS = $(SIGNALS_FILES:$(SIGNALS_DIR)/%.c=$(SIGNALS_OBJ_DIR)/%.o)
UNSET_OBJS = $(UNSET_FILES:$(UNSET_DIR)/%.c=$(UNSET_OBJ_DIR)/%.o)
CD_OBJS = $(CD_FILES:$(CD_DIR)/%.c=$(CD_OBJ_DIR)/%.o)
EXPORT_OBJS = $(EXPORT_FILES:$(EXPORT_DIR)/%.c=$(EXPORT_OBJ_DIR)/%.o)
QUOTES_OBJS = $(QUOTES_FILES:$(QUOTES_DIR)/%.c=$(QUOTES_OBJ_DIR)/%.o)
REDIRECTIONS_OBJS = $(REDIRECTIONS_FILES:$(REDIRECTIONS_DIR)/%.c=$(REDIRECTIONS_OBJ_DIR)/%.o)
PIPES_OBJS = $(PIPES_FILES:$(PIPES_DIR)/%.c=$(PIPES_OBJ_DIR)/%.o)
EXECUTION_OBJS = $(EXECUTION_FILES:$(EXECUTION_DIR)/%.c=$(EXECUTION_OBJ_DIR)/%.o)
PWD_OBJS = $(PWD_FILES:$(PWD_DIR)/%.c=$(PWD_OBJ_DIR)/%.o)
EXIT_OBJS = $(EXIT_FILES:$(EXIT_DIR)/%.c=$(EXIT_OBJ_DIR)/%.o)
CORE_OBJS = $(CORE_FILES:$(CORE_DIR)/%.c=$(CORE_OBJ_DIR)/%.o)
UTILS_OBJS = $(UTILS_FILES:$(UTILS_DIR)/%.c=$(UTILS_OBJ_DIR)/%.o)
OBJS = $(MAIN_OBJS) $(PARSER_OBJS) $(ENV_OBJS) $(ECHO_OBJS) $(SIGNALS_OBJS) $(UNSET_OBJS) $(CD_OBJS) $(EXPORT_OBJS) $(QUOTES_OBJS) $(REDIRECTIONS_OBJS) $(PIPES_OBJS) $(EXECUTION_OBJS) $(PWD_OBJS) $(EXIT_OBJS) $(CORE_OBJS) $(UTILS_OBJS)

# Final executable
NAME	= minishell

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PARSER_OBJ_DIR)/%.o: $(PARSER_DIR)/%.c
	mkdir -p $(PARSER_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(ENV_OBJ_DIR)/%.o: $(ENV_DIR)/%.c
	mkdir -p $(ENV_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(ECHO_OBJ_DIR)/%.o: $(ECHO_DIR)/%.c
	mkdir -p $(ECHO_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(SIGNALS_OBJ_DIR)/%.o: $(SIGNALS_DIR)/%.c
	mkdir -p $(SIGNALS_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(UNSET_OBJ_DIR)/%.o: $(UNSET_DIR)/%.c
	mkdir -p $(UNSET_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(CD_OBJ_DIR)/%.o: $(CD_DIR)/%.c
	mkdir -p $(CD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXPORT_OBJ_DIR)/%.o: $(EXPORT_DIR)/%.c
	mkdir -p $(EXPORT_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(QUOTES_OBJ_DIR)/%.o: $(QUOTES_DIR)/%.c
	mkdir -p $(QUOTES_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(REDIRECTIONS_OBJ_DIR)/%.o: $(REDIRECTIONS_DIR)/%.c
	mkdir -p $(REDIRECTIONS_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PIPES_OBJ_DIR)/%.o: $(PIPES_DIR)/%.c
	mkdir -p $(PIPES_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTION_OBJ_DIR)/%.o: $(EXECUTION_DIR)/%.c
	mkdir -p $(EXECUTION_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PWD_OBJ_DIR)/%.o: $(PWD_DIR)/%.c
	mkdir -p $(PWD_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXIT_OBJ_DIR)/%.o: $(EXIT_DIR)/%.c
	mkdir -p $(EXIT_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(CORE_OBJ_DIR)/%.o: $(CORE_DIR)/%.c
	mkdir -p $(CORE_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(UTILS_OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(UTILS_OBJ_DIR)
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
