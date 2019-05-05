/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prastoin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:32:56 by prastoin          #+#    #+#             */
/*   Updated: 2019/05/06 00:25:50 by prastoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_core_tab g_ops[17] = {
	[0x01] = {
		0x01, "live", false, 10, {
			PARAM_DIRECT, 0, 0, 0
		}
	},
	[0x02] = {
		0x02, "ld", true, 5, {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x03] = {
		0x03, "st", true, 5, {
			PARAM_REGISTER, PARAM_INDIRECT | PARAM_REGISTER, 0, 0
		}
	},
	[0x04] = {
		0x04, "add", true, 10, {
			PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x05] = {
		0x05, "sub", true, 10, {
			PARAM_REGISTER, PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x06] = {
		0x06, "and", true, 6, {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x07] = {
		0x07, "or", true, 6, {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x08] = {
		0x08, "xor", true, 6, {
			PARAM_ALL, PARAM_ALL, PARAM_REGISTER, 0
		}
	},
	[0x09] = {
		0x09, "zjmp", false, 20, {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x0a] = {
		0x0a, "ldi", true, 25, {
			PARAM_ALL | PARAM_INDEX,
			PARAM_INDEX | PARAM_DIRECT | PARAM_REGISTER, PARAM_REGISTER, 0
		}
	},
	[0x0b] = {
		0x0b, "sti", true, 25, {
			PARAM_REGISTER | PARAM_INDEX,
			PARAM_ALL | PARAM_INDEX,
			PARAM_REGISTER | PARAM_DIRECT | PARAM_INDEX, 0
		}
	},
	[0x0c] = {
		0x0c, "fork", false, 800, {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x0d] = {
		0x0d, "lld", true, 10, {
			PARAM_DIRECT | PARAM_INDIRECT, PARAM_REGISTER, 0, 0
		}
	},
	[0x0e] = {
		0x0e, "lldi", true, 50, {
			PARAM_ALL | PARAM_INDEX,
			PARAM_DIRECT | PARAM_REGISTER | PARAM_INDEX, PARAM_REGISTER, 0
		}
	},
	[0x0f] = {
		0x0f, "lfork", false, 1000, {
			PARAM_DIRECT | PARAM_INDEX, 0, 0, 0
		}
	},
	[0x10] = {
		0x10, "aff", true, 2, {
			PARAM_REGISTER, 0, 0, 0
		}
	}
};
