/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:32:56 by prastoin          #+#    #+#             */
/*   Updated: 2019/04/03 15:14:17 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_core_tab g_ops[17] = {
	[0x01] = {
		0x01, "live", false, {
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x02] = {
		0x02, "ld", true, {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x03] = {
		0x03, "st", true, {
			PARAM_REGISTER, PARAM_INDIRECT | PARAM_REGISTER, 0, 0
		}
	},
	[0x04] = {
		0x04, "add", true, {
			PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x05] = {
		0x05, "sub", true, {
			PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x06] = {
		0x06, "and", true, {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x07] = {
		0x07, "or", true, {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x08] = {
		0x08, "xor", true, {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x09] = {
		0x09, "zjmp", false, {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x0a] = {
		0x0a, "ldi", true, {
			PARAM_ALL | PARAM_INDEX, PARAM_INDEX | PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x0b] = {
		0x0b, "sti", true, {
			PARAM_REGISTER, PARAM_ALL | PARAM_INDEX, PARAM_REGISTER | PARAM_DIRECT | PARAM_INDEX, 0
		}
	},
	[0x0c] = {
		0x0c, "fork", false, {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x0d] = {
		0x0d, "lld", true, {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x0e] = {
		0x0e, "lldi", true, {
			PARAM_ALL, PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x0f] = {
		0x0f, "lfork", false, {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x10] = {
		0x10, "aff", true, {
			PARAM_REGISTER, 0, 0, 0
		}
	}
};
