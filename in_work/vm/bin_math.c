/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 11:23:08 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/08 16:42:59 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

bool	bin_add(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	uint16_t			save;
	ssize_t				i;
	bool				carry;

	carry = false;
	i = REG_SIZE - 1;
	printf("\n");
	printf("OP1 = %d %d %d %d, OP2 = %d %d %d %d\n", op1[0], op1[1], op1[2], op1[3], op2[0], op2[1], op2[2], op2[3]);
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
		printf("save = %d\n", save);
		i--;
	}
	return (!carry);
}

bool	bin_sub(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
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

void	bin_and(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	size_t				i;

	i = 0;
	while (i < REG_SIZE)
	{
		res[i] = op1[i] & op2[i];
		i++;
	}
}

void	bin_or(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	size_t				i;

	i = 0;
	while (i < REG_SIZE)
	{
		res[i] = op1[i] | op2[i];
		i++;
	}
}

void	bin_xor(uint8_t op1[REG_SIZE], uint8_t op2[REG_SIZE], uint8_t res[REG_SIZE])
{
	size_t				i;

	i = 0;
	while (i < REG_SIZE)
	{
		res[i] = op1[i] ^ op2[i];
		i++;
	}
}
