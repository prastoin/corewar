/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:32:56 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/19 16:31:08 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_core_tab g_ops[17] = {
	[0x01] = {
		0x01, "live", {
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x02] = {
		0x02, "ld", {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x03] = {
		0x03, "st", {
			PARAM_REGISTER, PARAM_INDIRECT | PARAM_REGISTER, 0, 0
		}
	},
	[0x04] = {
		0x04, "add", {
			PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x05] = {
		0x05, "sub", {
			PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x06] = {
		0x06, "and", {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x07] = {
		0x07, "or", {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x08] = {
		0x08, "xor", {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x09] = {
		0x09, "zjmp", {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x0a] = {
		0x0a, "ldi", {
			PARAM_ALL | PARAM_INDEX, PARAM_INDEX | PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x0b] = {
		0x0b, "sti", {
			PARAM_REGISTER, PARAM_ALL | PARAM_INDEX, PARAM_REGISTER | PARAM_DIRECT | PARAM_INDEX, 0
		}
	},
	[0x0c] = {
		0x0c, "fork", {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x0d] = {
		0x0d, "lld", {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x0e] = {
		0x0e, "lldi", {
			PARAM_ALL, PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x0f] = {
		0x0f, "lfork", {
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x10] = {
		0x10, "aff", {
			PARAM_REGISTER, 0, 0, 0
		}
	}
};
