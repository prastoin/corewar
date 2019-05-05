/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 10:07:00 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 00:32:31 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <stdarg.h>

# define BUFF_SIZE 4096
# define CSI "\033["
# define CSI_GREEN CSI "32;01m"
# define CSI_WHITE CSI "37;01m"
# define CSI_BLUE CSI "34;01m"
# define CSI_YELLOW CSI "33;01m"
# define CSI_RED CSI "31m"
# define CSI_RESET CSI "0m"

size_t		ft_strlen(const char *str);
uint8_t		*ft_strcpy(uint8_t *dst, const uint8_t *src);
size_t		search_for(uint8_t *str, uint8_t c);
ssize_t		ft_strncmp(uint8_t *s1, char *str, size_t n);
ssize_t		ft_strcmp(const char *s1, const char *s2);
size_t		atoi_hexa(char nb[], size_t size);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
size_t		nb_len(uintmax_t n);
intmax_t	ft_atoi(char *str);
void		ft_putf_va(int fd, char *fmt, va_list args, size_t padd);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		ft_putnbr_fd(size_t fd, intmax_t nb);
void		handle_hexa(size_t fd, va_list args, size_t padd, char *flag);
void		handle_dlu(size_t fd, va_list args, size_t padd, char *flag);
void		char_fd(size_t fd, uint8_t c, size_t padd);
void		str_hexa_fd(char *str, size_t fd, size_t padd);

#endif
