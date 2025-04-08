///home/lyoussef/bin/cat
///usr/local/sbin/cat
///usr/local/bin/cat
///usr/sbin/cat
///usr/bin/cat
///sbin/cat
///bin/cat
///usr/games/cat
///usr/local/games/cat
///snap/bin/cat

//things that are currently not working
// 1- shlvl
// 2- multiple > or multiple <
// 3- echo $$


//execve(cmd ,path,envp ) bas you need to do this in a fork
//INCREMENT THE SHLVL=1 in case manna hone mnaamoul
//export bede aatiya parameter tabaa shlevel
//in case env -i --> hard code 1

// pwd --> in case manna bel env baamoul export for the variable
// old pwd --> in case manna bel env baamoul export for the variable

//cat
//grep
//wc -l

//0	Success (No error)
//1	General error (Miscellaneous error)
//2	Incorrect usage (Misuse of shell builtins)
//126	Command found but not executable
//127	Command not found
//128	Invalid argument to exit
//130	Command terminated by Ctrl+C (SIGINT)
//131	Command terminated by Ctrl+\ (SIGQUIT)
//137	Command killed with kill -9 (SIGKILL)
//139	Segmentation fault (SIGSEGV)
//255	Exit status out of range

//PIPEXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//int	check_cmd(t_cmd *c)
//{
//	int		i;
//	char	*cmd;

//	i = -1;
//	while (c->path[++i])
//	{
//		cmd = ft_join(c->path[i], c->cmd_name);
//		if (!cmd)
//			return (0);
//		if (access(cmd, X_OK) != -1)
//		{
//			free(cmd);
//			return (1);
//		}
//		free(cmd);
//	}
//	error_msg(c->cmd_name);
//	return (0);
//}

//char	**get_path(char **ep)
//{
//	char	**ret;
//	char	*env;
//	int		i;

//	i = -1;
//	while (ep[++i])
//	{
//		if (!ft_strncmp(ep[i], "PATH=", PATH))
//		{
//			env = ft_substr(ep[i], START, ft_strlen(ep[i]));
//			if (!env)
//				return (NULL);
//			ret = ft_split(env, ':');
//			if (!ret)
//			{
//				free(env);
//				return (NULL);
//			}
//			free (env);
//			return (ret);
//		}
//	}
//	return (NULL);
//}

//int	get_cmd(char **ep, t_cmd *c, char *cmd)
//{
//	int		i;
//	char	**tmp;

//	i = -1;
//	c->path = get_path(ep);
//	if (!c->path)
//		return (0);
//	tmp = ft_split(cmd, ' ');
//	if (!tmp)
//		return (0);
//	c->cmd_name = ft_substr(tmp[i + 1], 0, ft_strlen(tmp[i + 1]) - 1);
//	if (!c->cmd_name)
//		return (free_array(tmp));
//	while (tmp[++i])
//	{
//		c->args[i] = ft_substr(tmp[i], 0, ft_strlen(tmp[i]) - 1);
//		if (!c->args[i])
//		{
//			free_array(c->args);
//			return (free_array(tmp));
//		}
//	}
//	c->args[i] = 0;
//	free_array(tmp);
//	return (1);
//}




//MAIN AFTER DIVIDING -------------------------------------
//after dividing this function

//void parse_and_execute(t_exec *exec, t_cmd_node *cmd) {
//    if (!cmd || !cmd->arr) {
//        printf("Error: Invalid command node\n");
//        return;
//    }

//    if (is_builtin_command(cmd->arr[0])) {
//        handle_builtin_commands(exec, cmd);
//    } else {
//        handle_non_builtin_commands(exec, cmd);
//    }
//}

//void handle_builtin_commands(t_exec *exec, t_cmd_node *cmd) {
//    if (ft_strcmp(cmd->arr[0], "echo") == 0)
//        ft_echo(cmd, exec->env_list, exec);
//    else if (ft_strcmp(cmd->arr[0], "cd") == 0)
//        ft_cd(exec, cmd->arr[1]);
//    else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
//        ft_pwd(exec);
//    else if (ft_strcmp(cmd->arr[0], "env") == 0)
//        ft_env(exec, cmd->arr);
//    else if (ft_strcmp(cmd->arr[0], "export") == 0) {
//        if (cmd->arr[1] == NULL)
//            ft_export(exec->env_list);
//        else
//            handle_export(cmd->arr[1], &exec->env_list, &exec->gc);
//    }
//    else if (ft_strcmp(cmd->arr[0], "unset") == 0)
//        unset_env_var(exec, cmd->arr[1]);
//    else if (ft_strcmp(cmd->arr[0], "exit") == 0)
//        ft_exit(cmd->arr, exec->exit_status);
//}

//void handle_non_builtin_commands(t_exec *exec, t_cmd_node *cmd) {
//    char **path_dirs = get_path_from_env(exec->env_list);
//    if (!path_dirs) {
//        printf("minihell: %s: command not found\n", cmd->arr[0]);
//        exec->exit_status = 127;
//        return;
//    }

//    int found = 0;
//    int i = 0;
//    while (path_dirs[i]) {
//        char *full_path = ft_join(path_dirs[i], "/", cmd->arr[0], &exec->gc);
//        if (!full_path) {
//            printf("minihell: memory allocation failed\n");
//            exec->exit_status = 1;
//            return;
//        }

//        if (access(full_path, X_OK) == 0) {
//            execute_command(cmd, &exec->gc, full_path);
//            found = 1;
//            break;
//        }
//        i++;
//    }

//    if (!found) {
//        printf("minihell: %s: command not found\n", cmd->arr[0]);
//        exec->exit_status = 127;
//    }
//}

//int is_builtin_command(const char *cmd)
//{
//	const char *builtins[] = {
//		"echo", "cd", "pwd", "env", "export", "unset", "exit", NULL };
//	int i;

//	i = 0;
//	while (builtins[i])
//	{
//		if (ft_strcmp(cmd, builtins[i]) == 0)
//			return 1;
//		i++;
//	}
//	return 0;
//}

