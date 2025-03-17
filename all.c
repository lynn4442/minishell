/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <stdlib.h>
# include <sys/wait.h>
# include "libft/libft.h"

//t_elem: la kel token
//t_type: type tabaa l command
//t_env_var: lal env ta asema
//t_cmd_node: la kel command

typedef struct s_elem {
	char			*token;
	int				space_after;
	struct s_elem	*next;
}	t_elem;

typedef enum e_type {
	PIPE,
	SMP_CMD,
}	t_type;

typedef struct s_env_var {
	char	*name;
	char	*value;
	struct s_env_var	*next;
	struct s_env_var	*prev;
}	t_env_var;

typedef struct s_cmd_node {
	char				**arr;
	char				*in;
	char				*out;
	t_type				type;
	char				*heredoc;
	int					append;
	int					err;
	int					ex_heredoc;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_exec{
	t_cmd_node	*cmd_list;
	t_env_var	*env_list;
	int			exit_status;
}	t_exec;

//init_data
t_env_var	*create_env_var(char *name, char *value);
void		free_env_list(t_env_var *env);
t_cmd_node	*create_cmd_node(char **args);
void		free_cmd_node(t_cmd_node *cmd);
void		free_elem_list(t_elem *elem);
t_exec		*init_exec();
void		free_exec(t_exec *exec);
void		add_env_var(t_exec *exec, char *name, char *value);

//echo
char		*get_env_value(t_env_var *env_list, char *var_name);
void		print_arg(char *arg, t_env_var *env);
void		ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec);

//cd
t_env_var	*get_env_var(t_exec *exec, const char *name);
void		update_env_var(t_exec *exec, const char *name, const char *value);
int			change_dir(const char *path, t_exec *exec);
void		update_pwd_vars(t_exec *exec, const char *old_pwd);
int			ft_cd(t_exec *exec, const char *arg);

//export
void		swap_env_vars(t_env_var *a, t_env_var *b);
void		sort_env_vars(t_env_var *head);
void		ft_export(t_env_var *env_list);

//unset
t_env_var	*remove_env_var(t_exec *exec, const char *name);
void		free_env_var(t_exec *exec, t_env_var *node);
void		unset_env_var(t_exec *exec, const char *name);

//pwd
char		*get_current_directory(void);
void		ft_pwd(t_exec *exec);

//env
void		ft_env(t_exec *exec);

//exit
int			is_numeric(const char *str);
void		ft_exit(char **args, int last_exit_status);

//main
int			main();

#endif


//cd
t_env_var *get_env_var(t_exec *exec, const char *name)
{
	t_env_var *current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}

void update_env_var(t_exec *exec, const char *name, const char *value)
{
	t_env_var *var = get_env_var(exec, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
	}
	else
	{
		t_env_var *new_var = malloc(sizeof(t_env_var));
		new_var->name = ft_strdup(name);
		new_var->value = ft_strdup(value);
		new_var->next = exec->env_list;
		new_var->prev = NULL;
		if (exec->env_list)
			exec->env_list->prev = new_var;
		exec->env_list = new_var;
	}
}

int change_dir(const char *path, t_exec *exec)
{
	if (!path || *path == '\0')
	{
		write(2, "cd: OLDPWD not set\n", 19);
		exec->exit_status = 1;
		return 1;
	}
	if (chdir(path) != 0)
	{
		perror("minishell cd:");
		exec->exit_status = 1;
		return 1;
	}
	return 0;
}

void update_pwd_vars(t_exec *exec, const char *old_pwd)
{
	char cwd[1024];
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd");
		exec->exit_status = 1;
		return;
	}
	update_env_var(exec, "OLDPWD", old_pwd);
	update_env_var(exec, "PWD", cwd);
	exec->exit_status = 0;
}

int ft_cd(t_exec *exec, const char *arg)
{
	char		*old_pwd;
	t_env_var	*pwd_var;
	t_env_var *home_var;

	pwd_var = get_env_var(exec, "PWD");
	if (pwd_var)
	  old_pwd = pwd_var->value;
	else
	  old_pwd = "";
	if (!arg || ft_strcmp(arg, "~") == 0)
	{
		home_var = get_env_var(exec, "HOME");
		if (!home_var || !home_var->value)
		{
			write(2, "cd: HOME not set\n", 17);
			exec->exit_status = 1;
			return 1;
		}
		arg = home_var->value;
	}
	else if (ft_strcmp(arg, "-") == 0)
	{
		t_env_var *oldpwd_var = get_env_var(exec, "OLDPWD");
		if (!oldpwd_var || !oldpwd_var->value)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			exec->exit_status = 1;
			return 1;
		}
		printf("%s\n", oldpwd_var->value);
		arg = oldpwd_var->value;
	}
	printf("Changing directory to: %s\n", arg);
	if (change_dir(arg, exec) == 0)
	{
		update_pwd_vars(exec, old_pwd);
		return (0);
	}
	return (1);
}

//echo

char *get_env_value(t_env_var *env_list, char *var_name) {
	t_env_var *current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
			return current->value;
		current = current->next;
	}
	return (NULL, printf("Environment variable not found.\n"));
}


void print_arg(char *arg, t_env_var *env)
{
	char *env_value;

	if (!arg)
		return ;
	if (arg[0] == '$' && arg[1] != '\0')
	{
		env_value = get_env_value(env, arg + 1);
		if (env_value)
			write(1, env_value, ft_strlen(env_value));
	}
	else
		write(1, arg, ft_strlen(arg));
}

void ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int i;
	int no_newline;

	i = 1;
	no_newline = 0;
	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;
	while (cmd->arr[i] && ft_strcmp(cmd->arr[i], "-n") == 0)
	{
		no_newline = 1;
		i++;
	}
	while (cmd->arr[i])
	{
		print_arg(cmd->arr[i], env);
		if (cmd->arr[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	exec->exit_status = 0;
}

//env
void ft_env(t_exec *exec)
{
	t_env_var *current;

	if (!exec || !exec->env_list)
		return ;
	current = exec->env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		else
			printf("%s=\n", current->name);
		current = current->next;
	}
}

//exit
int is_numeric(const char *str)
{
	int i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void ft_exit(char **args, int last_exit_status)
{
	printf("exit\n");
	if (!args[1])
		exit(last_exit_status);
	if (is_numeric(args[1]))
	{
		if (args[2])
		{
			printf("exit: too many arguments\n");
			return ;
		}
		exit(atoi(args[1]) % 256);
	}
	else
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
}

//export
void swap_env_vars(t_env_var *a, t_env_var *b)
{
	char *temp_name;
	char *temp_value;

	temp_name = a->name;
	a->name = b->name;
	b->name = temp_name;

	temp_value = a->value;
	a->value = b->value;
	b->value = temp_value;
}

void sort_env_vars(t_env_var *head)
{
	int swapped;
	t_env_var *ptr1;
	t_env_var *lptr;

	lptr = NULL;
	if (head == NULL)
		return;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
			{
				swap_env_vars(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}

void ft_export(t_env_var *env_list)
{
	t_env_var *temp;

	temp = env_list;
	sort_env_vars(env_list);
	while (temp)
	{
		ft_putstr_fd(temp->name, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(temp->value, 1);
		ft_putstr_fd("\n", 1);
		temp = temp->next;
	}
}

//init data
t_env_var *create_env_var(char *name, char *value)
{
	t_env_var *env = malloc(sizeof(t_env_var));
	if (!env)
		return NULL;
	env->name = ft_strdup(name);
	env->value = ft_strdup(value);
	if (!env->name || !env->value)
	{
		free(env->name);
		free(env->value);
		free(env);
		return NULL;
	}
	env->next = NULL;
	env->prev = NULL;
	return env;
}

void free_env_list(t_env_var *env)
{
	t_env_var *tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

t_cmd_node *create_cmd_node(char **args)
{
	t_cmd_node *cmd = malloc(sizeof(t_cmd_node));
	int count;
	int i;

	i = 0;
	count = 0;
	if (!cmd)
		return NULL;
	while (args && args[count])
		count++;
	cmd->arr = malloc((count + 1) * sizeof(char *));
	if (!cmd->arr)
		return (free(cmd), NULL);
	while (i < count)
	{
		cmd->arr[i] = ft_strdup(args[i]);
		i++;
	}
	cmd->arr[count] = NULL;
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->type = SMP_CMD;
	cmd->heredoc = NULL;
	cmd->append = 0;
	cmd->err = 0;
	cmd->ex_heredoc = 0;
	cmd->next = NULL;
	return cmd;
}

void free_cmd_node(t_cmd_node *cmd)
{
	t_cmd_node *tmp;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;

		if (tmp->arr)
		{
			for (int i = 0; tmp->arr[i]; i++)
				free(tmp->arr[i]);
			free(tmp->arr);
		}
		free(tmp->in);
		free(tmp->out);
		free(tmp->heredoc);
		free(tmp);
	}
}

void free_elem_list(t_elem *elem)
{
	t_elem *tmp;
	while (elem)
	{
		tmp = elem;
		elem = elem->next;
		free(tmp->token);
		free(tmp);
	}
}

t_exec *init_exec()
{
	t_exec *exec = malloc(sizeof(t_exec));
	if (!exec)
		return NULL;
	exec->cmd_list = NULL;
	exec->env_list = NULL;
	exec->exit_status = 0;
	return exec;
}

void free_exec(t_exec *exec)
{
	if (!exec)
		return;
	free_env_list(exec->env_list);
	free_cmd_node(exec->cmd_list);
	free(exec);
}

void add_env_var(t_exec *exec, char *name, char *value)
{
	t_env_var *new_var;

	new_var = create_env_var(name, value);
	if (!new_var)
		return;
	if (!exec->env_list)
		exec->env_list = new_var;
	else
	{
		t_env_var *temp = exec->env_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_var;
		new_var->prev = temp;
	}
}

//input_redirection
void ft_input_red(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void execute_ft_input_red(t_cmd_node *cmd)
{
	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (cmd->in)
			ft_input_red(cmd->in);
		execvp(cmd->arr[0], cmd->arr);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	wait(NULL);
}

//execvp(command, args) --> bs baatiya l command hiye bet nabbich lahala
//execve(path, args, envp) --> eza aande full control aal execution yaane baarif kelchi aal env variable

//output_redirection

char *get_current_directory(void)
{
	char *buf = malloc(1024);
	if (!buf)
		return (NULL);
	if (!getcwd(buf, 1024))
	{
		free(buf);
		return (NULL);
	}
	return buf;
}

void ft_pwd(t_exec *exec)
{
	char *cwd = get_current_directory();
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
	}
	else
	{
		t_env_var *pwd_var = get_env_var(exec, "PWD");
		if (pwd_var && pwd_var->value)
		{
			ft_putstr_fd(pwd_var->value, 1);
			ft_putstr_fd("\n", 1);
		}
		else
		{
			ft_putstr_fd("pwd: error retrieving current directory\n", 2);
			exec->exit_status = 1;
			return;
		}
	}
	exec->exit_status = 0;
}

//pwd

char *get_current_directory(void)
{
	char *buf = malloc(1024);
	if (!buf)
		return (NULL);
	if (!getcwd(buf, 1024))
	{
		free(buf);
		return (NULL);
	}
	return buf;
}

void ft_pwd(t_exec *exec)
{
	char *cwd = get_current_directory();
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
	}
	else
	{
		t_env_var *pwd_var = get_env_var(exec, "PWD");
		if (pwd_var && pwd_var->value)
		{
			ft_putstr_fd(pwd_var->value, 1);
			ft_putstr_fd("\n", 1);
		}
		else
		{
			ft_putstr_fd("pwd: error retrieving current directory\n", 2);
			exec->exit_status = 1;
			return;
		}
	}
	exec->exit_status = 0;
}

//unset
t_env_var *remove_env_var(t_exec *exec, const char *name)
{
	t_env_var *current;
	t_env_var *prev;

	current = exec->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev == NULL)
			{
				exec->env_list = current->next;
				if (current->next)
					current->next->prev = NULL;
			}
			else if (current->next)
			{
				prev->next = current->next;
				current->next->prev = prev;
			}
			else
				prev->next = NULL;
			return current;
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

void free_env_var(t_exec *exec, t_env_var *node)
{
	if (node)
	{
		free(node->name);
		free(node->value);
		free(node);
		exec->exit_status = 0;
	}
	else
		exec->exit_status = 0;
}

void unset_env_var(t_exec *exec, const char *name)
{
	t_env_var *node_to_remove;

	node_to_remove = remove_env_var(exec, name);
	free_env_var(exec, node_to_remove);
}


