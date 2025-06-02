/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>

// Memory management structures
typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

typedef struct s_garbage_collector
{
	t_mem_node	*head;
}	t_gc;

// Command type enumeration
typedef enum e_type {
	PIPE,
	SMP_CMD
}	t_type;

// Environment variable structure
typedef struct s_env_var
{
	char				*key;
	bool				equal;
	char				*value;
	char				*all;
	struct s_env_var	*next;
	struct s_env_var	*prev;
}	t_env_var;

// Forward declaration for circular reference
typedef struct s_exec	t_exec;

// Command node structure
typedef struct s_cmd_node
{
	char				**arr;
	char				*in;
	char				*out;
	t_type				type;
	char				*heredoc;
	int					append;
	int					err;
	int					ex_heredoc;
	t_exec				*exec;
	struct s_cmd_node	*next;
}	t_cmd_node;

// Execution context structure
typedef struct s_exec
{
	t_cmd_node	*cmd_list;
	t_env_var	*env_list;
	int			exit_status;
	t_gc		gc;
	int			heredoc_counter;
}	t_exec;

// Quote checking structure
typedef struct s_quote_check
{
	int		i;
	int		len;
	char	*res;
	char	quote_type;
	int		escaped;
}	t_quote_check;

// Token element structure for parsing
typedef struct s_elem {
	char			*token;
	int				space_after;
	struct s_elem	*next;
}	t_elem;
#endif
