/home/lyoussef/bin/cat
/usr/local/sbin/cat
/usr/local/bin/cat
/usr/sbin/cat
/usr/bin/cat
/sbin/cat
/bin/cat
/usr/games/cat
/usr/local/games/cat
/snap/bin/cat

execve(cmd ,path,envp ) bas you need to do this in a fork why ?? because


cat
grep
wc -l

//0	Success (No error)
//1	General error (Miscellaneous error)
//2	Incorrect usage (Misuse of shell builtins)
//126	Command found but not executable
//127	Command not found
//128	Invalid argument to exit
//130	Command terminated by Ctrl+C (SIGINT)
//137	Command killed with kill -9 (SIGKILL)
//139	Segmentation fault (SIGSEGV)
//255	Exit status out of range



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

