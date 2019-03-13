/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 10:07:00 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/13 14:13:25 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
#define FT_STRING_H

#include <stdlib.h>
#include <unistd.h>

size_t	ft_strlen(const char *str);
uint8_t		*ft_strcpy(uint8_t *dst, const uint8_t *src);
size_t		search_for(uint8_t *str, uint8_t c);
ssize_t		ft_strncmp(uint8_t *s1, char *str, size_t n);

#endif
