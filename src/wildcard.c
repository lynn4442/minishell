// //wildcard 
// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   wildcard.c                                         :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
// /*   Updated: 2025/03/27 10:00:00 by lyoussef         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"
// #include <dirent.h>

// static int match_pattern(const char *pattern, const char *str)
// {
//     if (*pattern == '\0' && *str == '\0')
//         return (1);
//     if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
//         return (0);
//     if (*pattern == '*')
//         return (match_pattern(pattern + 1, str) || match_pattern(pattern, str + 1));
//     if (*pattern == *str)
//         return (match_pattern(pattern + 1, str + 1));
//     return (0);
// }

// static int count_matches(const char *pattern)
// {
//     DIR *dir;
//     struct dirent *entry;
//     int count;

//     count = 0;
//     dir = opendir(".");
//     if (!dir)
//         return (0);

//     while ((entry = readdir(dir)))
//     {
//         if (entry->d_name[0] != '.' || pattern[0] == '.')
//         {
//             if (match_pattern(pattern, entry->d_name))
//                 count++;
//         }
//     }
//     closedir(dir);
//     return (count);
// }

// char **expand_wildcards(t_exec *exec, const char *pattern)
// {
//     DIR *dir;
//     struct dirent *entry;
//     char **result;
//     int count;
//     int i;

//     count = count_matches(pattern);
//     if (count == 0)
//         return (NULL);

//     result = ft_malloc(&exec->gc, sizeof(char *) * (count + 1));
//     if (!result)
//         return (NULL);

//     dir = opendir(".");
//     if (!dir)
//         return (NULL);

//     i = 0;
//     while ((entry = readdir(dir)))
//     {
//         if (entry->d_name[0] != '.' || pattern[0] == '.')
//         {
//             if (match_pattern(pattern, entry->d_name))
//             {
//                 result[i] = ft_strdup(&exec->gc, entry->d_name);
//                 if (!result[i])
//                 {
//                     closedir(dir);
//                     return (NULL);
//                 }
//                 i++;
//             }
//         }
//     }
//     result[i] = NULL;
//     closedir(dir);
//     return (result);
// }


//to add inside minishell.h
// Add with other function prototypes
// char **expand_wildcards(t_exec *exec, const char *pattern);


//inside the main.c
// ... existing code ...

// static char **expand_args(t_exec *exec, char **args)
// {
//     char **expanded;
//     char **temp;
//     int i;
//     int j;
//     int total;

//     // Count total arguments after expansion
//     total = 0;
//     i = 0;
//     while (args[i])
//     {
//         if (ft_strchr(args[i], '*'))
//         {
//             temp = expand_wildcards(exec, args[i]);
//             if (temp)
//             {
//                 j = 0;
//                 while (temp[j])
//                     j++;
//                 total += j;
//             }
//             else
//                 total++;
//         }
//         else
//             total++;
//         i++;
//     }

//     // Create new argument array
//     expanded = ft_malloc(&exec->gc, sizeof(char *) * (total + 1));
//     if (!expanded)
//         return (NULL);

//     // Fill expanded array
//     total = 0;
//     i = 0;
//     while (args[i])
//     {
//         if (ft_strchr(args[i], '*'))
//         {
//             temp = expand_wildcards(exec, args[i]);
//             if (temp)
//             {
//                 j = 0;
//                 while (temp[j])
//                     expanded[total++] = temp[j++];
//             }
//             else
//                 expanded[total++] = ft_strdup(&exec->gc, args[i]);
//         }
//         else
//             expanded[total++] = ft_strdup(&exec->gc, args[i]);
//         i++;
//     }
//     expanded[total] = NULL;
//     return (expanded);
// }

// // Modify the relevant part in main where you create the command
// if (ft_strlen(input) > 0)
// {
//     add_history(input);
//     args = ft_split(input, ' ', &exec->gc);
//     if (args)
//     {
//         char **expanded_args = expand_args(exec, args);
//         if (expanded_args)
//         {
//             cmd = create_cmd_node(exec, expanded_args);
//             parse_and_execute(exec, cmd, envp);
//         }
//     }
// }
