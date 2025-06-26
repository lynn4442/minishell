/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include "../include/types.h"

//ft_char_utils.c
int		ft_isspace(char c);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isdigit(int c);

//ft_validation.c
int		is_numeric(const char *str);
int		is_valid_var_name(const char *name);

//ft_string_conv.c
int		ft_atoi(const char *nptr);
char	*ft_itoa(int n, t_gc *gc);

//ft_string_cmp.c
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

//ft_string_copy.c
char	*ft_strcpy(char *dest, char *src);
char	*ft_strdup(t_gc *gc, const char *src);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);
char	*ft_strndup(t_gc *free, const char *s, size_t len);
void	*ft_memset(void *s, int c, size_t n);

//ft_string_join.c
char	*ft_strjoin(const char *s1, const char *s2, t_gc *x);
size_t	ft_strlcat(char *dst, const char *src, size_t size);

//ft_string_basic.c
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(const char *s, int fd);

//ft_string_extract.c
char	*ft_strtrim(char const *s1, char const *set, t_gc *gc);
char	*ft_substr(char const *s, unsigned int start, size_t len, t_gc *gc);

//ft_array_utils.c
char	**ft_split(char const *s, char c, t_gc *free);

void	*ft_malloc(t_gc *gc, size_t size);
void	*ft_memcpy(void *dst, const void *src, size_t n);

#endif
