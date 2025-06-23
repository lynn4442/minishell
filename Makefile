# Compiler and flags
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
RM		= rm -f

# Final executable
NAME	= minishell

# Directories
OBJ_DIR = obj
SRC_DIR = src

# Source files
SRCS = 	src/cd/cd_core.c \
		src/cd/cd_utils.c \
		src/core/check_builtins.c \
		src/core/init_data.c \
		src/core/main.c \
		src/echo/echo_core.c \
		src/echo/echo_redirection.c \
		src/echo/echo_utils.c \
		src/env/env_convert.c \
		src/env/env_core.c \
		src/env/env_utils.c \
		src/env/env_var_utils.c \
		src/env/shlvl.c \
		src/execution/child_process.c \
		src/execution/command_dispatch.c \
		src/execution/command_preparation.c \
		src/execution/execution_core.c \
		src/execution/execution_error_handling.c \
		src/execution/path_handling.c \
		src/execution/path_handling2.c \
		src/exit/exit.c \
		src/export/export.c \
		src/export/export_cmd.c \
		src/export/export_display.c \
		src/export/export_validation.c \
		src/parser/lexer_tokens.c \
		src/parser/lexer_words.c \
		src/parser/parser_error_handling.c \
		src/parser/parser_file_handling.c \
		src/parser/parser_file_input.c \
		src/parser/parser_lexer.c \
		src/parser/parser_lexer_utils.c \
		src/parser/parser_pipeline.c \
		src/parser/parser_pipeline_redirections.c \
		src/parser/parser_utils.c \
		src/parser/parser_cmd_core.c \
		src/parser/parser_cmd_utils.c \
		src/parser/parser_cmd_redirections.c \
		src/pipes/debug_pipe.c \
		src/pipes/pipe_execution.c \
		src/pipes/pipe_input.c \
		src/pipes/pipe_output.c \
		src/pipes/pipe_setup.c \
		src/pipes/pipe_utils.c \
		src/pipes/pipe.c \
		src/pipes/pipe_handling.c \
		src/pipes/pipe_util1.c \
		src/pipes/pipe_util2.c \
		src/pipes/pipe_util3.c \
		src/pwd/pwd.c \
		src/quotes/Heredoc_quotes_handling.c \
		src/quotes/quote_handling.c \
		src/quotes/quote_handling_2.c \
		src/quotes/quote_handling_helper.c \
		src/redirections/heredoc.c \
		src/redirections/redirection_args.c \
		src/redirections/redirection_handling.c \
		src/redirections/redirection_input.c \
		src/redirections/redirection_output.c \
		src/redirections/redirections.c \
		src/redirections/redirections_util_1.c \
		src/redirections/redirections_util_2.c \
		src/redirections/redirections_util_3.c \
		src/redirections/redirections_util_4.c \
		src/redirections/redirections_util_5.c \
		src/signals/signal_exit.c \
		src/signals/signal_handling.c \
		src/unset/unset_core.c \
		src/unset/unset_errors.c \
		src/unset/unset_operations.c \
		src/utils/utils.c

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Include paths
INCLUDES = -I include -I libft -I src/parser -I src/env -I src/echo -I src/signals -I src/unset -I src/cd -I src/export -I src/quotes -I src/redirections -I src/pipes -I src/execution -I src/pwd -I src/exit -I src/core -I src/utils

# Libraries
LIBFT   = libft/libft.a
LDFLAGS = -Llibft -lft -lreadline -lncurses

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C libft

clean:
	rm -rf $(OBJ_DIR)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
