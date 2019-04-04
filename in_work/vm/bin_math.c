/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:23:08 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/04 14:56:28 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool	add_bin(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	uint16_t			save;
	ssize_t				i;
	bool				carry;

	carry = false;
	i = REG_SIZE - 1;
	while (i >= 0)
	{
		save = op1[i] + op2[i];
		if (carry)
		{
			save += 1;
			carry = false;
		}
		if (save >= 256)
			carry = true;
		res[i] = (uint8_t)save;
		i--;
	}
	return (!carry);
}

bool	sub_bin(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	uint16_t			save;
	ssize_t				i;
	bool				carry;

	carry = false;
	i = REG_SIZE - 1;
	while (i >= 0)
	{
		save = op1[i] - op2[i];
		if (carry)
		{
			save -= 1;
			carry = false;
		}
		if (save >= 256)
			carry = true;
		res[i] = (uint8_t)save;
		i--;
	}
	return (!carry);
}

void	and_bin(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	size_t				i;

	i = 0;
	while (i < REG_SIZE)
	{
		res[i] = op1[i] & op2[i];
		i++;
	}
}

void	or_bin(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	size_t				i;

	i = 0;
	while (i < REG_SIZE)
	{
		res[i] = op1[i] | op2[i];
		i++;
	}
}

void	xor_bin(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	size_t				i;

	i = 0;
	while (i < REG_SIZE)
	{
		res[i] = op1[i] ^ op2[i];
		i++;
	}
}
