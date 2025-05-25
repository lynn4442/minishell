#include "../minishell.h"

static char **split_input_by_pipe(char *input, t_exec *exec)
{
    char 		**parts;
    int			i;
    char		*trimmed;

    i = 0;
    parts = ft_split(input, '|', &exec->gc);
    if (!parts)
        return (NULL);
    while (parts[i])
    {
        trimmed = ft_strtrim(parts[i], " \t", &exec->gc);
        if (!trimmed)
            return (NULL);
        if (ft_strlen(trimmed) == 0)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
            exec->exit_status = 2;
            return (NULL);
        }
        parts[i] = trimmed; // Replace original with trimmed string (if your GC setup allows this)
        i++;
    }
    return (parts); // Array of trimmed commands split by pipe
}
static char ***split_commands_into_args(char **commands, t_exec *exec)
{
    int i;
    int count;
    char ***commands_args;

    i = 0;
    count = 0;
    while (commands[count])
        count++;

    commands_args = ft_malloc(&exec->gc, (count + 1) * sizeof(char **));
    if (!commands_args)
        return (NULL);

    while (commands[i])
    {
        commands_args[i] = split_preserve_quotes(commands[i], &exec->gc);
        if (!commands_args[i])
            return (NULL);
        i++;
    }
    commands_args[i] = NULL;
    return (commands_args);
}
char ***split_input_into_commands_and_args(char *input, t_exec *exec)
{
    char **commands;
    char ***commands_args;

    commands = split_input_by_pipe(input, exec);// Step 1: Split input by pipe, trim and validate commands
    if (!commands)
        return (NULL);
    commands_args = split_commands_into_args(commands, exec);    // Step 2: For each trimmed command, split into arguments preserving quotes
    if (!commands_args)
        return (NULL);
    return (commands_args);  // 3D array: commands_args[command_index][arg_index]
}