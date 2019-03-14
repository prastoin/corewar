/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:32:56 by prastoin          #+#    #+#             */
/*   Updated: 2019/03/14 15:48:19 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_core_tab g_ops[] = {
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
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x10] = {
		0x10, "ldi", {
			PARAM_ALL, PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x11] = {
		0x11, "sti", {
			PARAM_REGISTER, PARAM_ALL, PARAM_REGISTER | PARAM_DIRECT, 0
		}
	},
	[0x12] = {
		0x12, "fork", {
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x13] = {
		0x13, "lld", {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x14] = {
		0x14, "lldi", {
			PARAM_ALL, PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x15] = {
		0x15, "lfork", {
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x16] = {
		0x16, "aff", {
			PARAM_REGISTER, 0, 0, 0
		}
	},
};
