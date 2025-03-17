/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/17 17:07:27 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void swap_env_vars(t_env_var *a, t_env_var *b)
{
	char *temp_name;
	char *temp_value;

	temp_name = a->key;
	a->key = b->key;
	b->key = temp_name;

	temp_value = a->value;
	a->value = b->value;
	b->value = temp_value;
}

void sort_env_vars(t_env_var *head)
{
	int swapped;
	t_env_var *current;
	t_env_var *last_unsorted;

	last_unsorted = NULL;
	if (head == NULL)
		return;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = head;

		while (current->next != last_unsorted)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_env_vars(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		last_unsorted = current;
	}
}

void add_or_update_env_var(t_env_var **env_list, char *name, char *value)
{
    t_env_var *temp = *env_list;

    while (temp)
    {
        if (ft_strcmp(temp->key, name) == 0)
        {
            free(temp->value);
            if (value == NULL || ft_strcmp(value, "") == 0)
            {
                temp->value = ft_strdup("");
            }
            else
            {
                temp->value = ft_strdup(value);
            }
            return;
        }
        temp = temp->next;
    }
    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->key = ft_strdup(name);
    if (value == NULL || ft_strcmp(value, "") == 0)
		new_var->value = ft_strdup("");
    else
		new_var->value = ft_strdup(value);
    new_var->next = *env_list;
    *env_list = new_var;
}

//treke l sort abel ma taamle iteration aal list
void ft_export(t_env_var *env_list)
{
    t_env_var *temp;

    if (env_list == NULL)
        return;

    sort_env_vars(env_list);
    temp = env_list;

    while (temp)
    {
        ft_putstr_fd("declare -x ", 1);
        ft_putstr_fd(temp->key, 1);

        // If the value is an empty string, print =""; otherwise, just print the value.
        if (temp->value && ft_strcmp(temp->value, "") == 0)
        {
            ft_putstr_fd("=\"\"", 1);
        }
        else if (temp->value && ft_strcmp(temp->value, "") != 0)
        {
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(temp->value, 1);
            ft_putstr_fd("\"", 1);
        }

        ft_putstr_fd("\n", 1); // Always print a newline after each entry
        temp = temp->next;
    }
}

int is_valid_var_name(const char *name)
{
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	for (int i = 1; name[i]; i++)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
	}
	return (1);
}

int ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}


void handle_export(char *cmd, t_env_var **env_list)
{
	char *equal_sign;
	char *start;
	char *end;

	start = cmd;
	while (*start && ft_isspace(*start))
		start++;
	end = start + ft_strlen(start) - 1;
	while (end >= start && ft_isspace(*end))
		end--;
	*(end + 1) = '\0';
	equal_sign = ft_strchr(start, '=');
	if (!equal_sign)
	{
		if (!is_valid_var_name(start))
			return;
		add_or_update_env_var(env_list, start, NULL);
		return;
	}
	*equal_sign = '\0';
	char *name = start;
	char *value = equal_sign + 1;
	if (!is_valid_var_name(name))
		return;
	add_or_update_env_var(env_list, name, value);
}
