/*
 * QEMU rocker switch emulation - PCI device - rocker_p4 program
 *
 * Copyright (c) 2015 Parag Bhide <parag.bhide@barefootnetworks.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <stdarg.h>
#include "net/eth.h"
#include "qemu/iov.h"
#include "qemu/timer.h"
#include "qmp-commands.h"

#include "rocker.h"
#include "rocker_hw.h"
#include "rocker_fp.h"
#include "rocker_tlv.h"
#include "rocker_world.h"
#include "rocker_desc.h"
#include "rocker_p4_rmt.h"
#include "rocker_p4_features.h"
#include "rocker_p4_enums.h"
#include "rocker_p4_tables_if.h"
#include "rocker_p4.h"
#include "rocker_p4_common.h"
#include "rocker_p4_lf.h"

#define ROCKER_P4_SESSION     0x01

bool rocker_p4_is_cpu_port(int port);
bool rocker_p4_is_table_valid(unsigned int table_id);
static void rocker_p4_uninit(World *world);
static int rocker_p4_init(World *world);

bool rocker_p4_is_cpu_port(int port)
{
    return port == CPU_PORT;
}

bool rocker_p4_is_table_valid(unsigned int table_id)
{
    return (table_id > 0 && table_id < RMT_TABLE_COUNT);
}
static void rocker_p4_uninit(World *world)
{
    /* unregister from mac_learn_notification */
    lf_mac_learn_digest_deregister(ROCKER_P4_SESSION);
    /* common uninit for all P4 worlds */
    rocker_p4_rmt_uninit(world);
}

static unsigned int
rocker_p4_mac_learn_notification(unsigned int sess_hdl,
                      struct rocker_p4_mac_learn_digest_digest_msg *msg,
                      void *cookie)
{
    struct p4_rmt_world *p4_rmt = (struct p4_rmt_world *)cookie;
    int i;
    for (i = 0; i < msg->num_entries; i++) {
        unsigned char addr[6];
        unsigned int pport = msg->entries[i].standard_metadata_ingress_port;
        unsigned short fid = htons(msg->entries[i].ingress_metadata_fid);

        memcpy(addr, msg->entries[i].ethernet_sa, 6);
        rocker_event_mac_vlan_seen(p4_rmt->rocker,
                            pport,
                            addr,
                            fid);
        DPRINTF("Rocker_P4 mac learn notification on port %d vlan %d\n",
                                        pport, fid);
    }

    lf_mac_learn_digest_notify_ack(ROCKER_P4_SESSION, msg);
    return 0;
}

static int rocker_p4_init(World *world)
{
    struct p4_rmt_world *p4_rmt = world_private(world);

    rocker_p4_rmt_init(world);

    p4_rmt->table_ops = &rocker_p4_table_ops[0];
    p4_rmt->is_cpu_port = rocker_p4_is_cpu_port;
    p4_rmt->is_table_valid = rocker_p4_is_table_valid;
    /* Current P4 model does not support multiple pipelines (programs / worlds)
     * within the same instance (process), once the support is added
     * process_pkt for a pipeline associated with the ingress port
     * will be called
     */
    p4_rmt->process_pkt = rmt_process_pkt;

    /*  register for mac learn notification with learn filter */
    lf_mac_learn_digest_register(ROCKER_P4_SESSION,
                                rocker_p4_mac_learn_notification,
                                (void *)p4_rmt);

    return 0;
}

static WorldOps p4_ops = {
    .init   = rocker_p4_init,
    .uninit = rocker_p4_uninit,
    .ig     = rocker_p4_rmt_ig,
    .cmd    = rocker_p4_rmt_cmd,
};

World *rocker_p4_world_alloc(Rocker *r)
{
    return world_alloc(r, sizeof(struct p4_rmt_world),
            ROCKER_WORLD_TYPE_ROCKER_P4, "rocker_p4", &p4_ops);
}
