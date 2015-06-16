/*
 * Rocker P4 switch device driver 
 *
 * Copyright (c) 2015 Parag Bhide <parag.bhide@barefootnetworks.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef _ROCKER_P4_ENUMS_H_
#define _ROCKER_P4_ENUMS_H_

enum rocker_p4_table_e {
	RMT_TABLE_NONE,
	RMT_TABLE_PORT_VLAN_MAPPING,
	RMT_TABLE_RMAC,
	RMT_TABLE_IPV4_HRT,
	RMT_TABLE_DMAC,
	RMT_TABLE_SMAC,
	RMT_TABLE_COUNT
};

#endif
