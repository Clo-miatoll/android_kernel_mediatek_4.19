// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#include <linux/err.h>
#include <linux/rcupdate.h>
#include "tuning.h"

void set_capacity_margin(unsigned int margin)
{
	if (margin >= SCHED_CAPACITY_SCALE)
		capacity_margin = margin;
}
EXPORT_SYMBOL(set_capacity_margin);

unsigned int get_capacity_margin(void)
{
	return capacity_margin;
}
EXPORT_SYMBOL(get_capacity_margin);
