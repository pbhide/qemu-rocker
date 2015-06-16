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
#ifndef _ROCKER_P4_LF_H_
#define _ROCKER_P4_LF_H_
struct rocker_p4_mac_learn_digest_digest_entry {
	unsigned char ethernet_sa[6];
	unsigned short ingress_metadata_fid;
	unsigned short standard_metadata_ingress_port;
};

struct  rocker_p4_mac_learn_digest_digest_msg {
	unsigned int                  dev_tgt;
	unsigned short                num_entries;
	struct rocker_p4_mac_learn_digest_digest_entry  *entries;
};


typedef unsigned int (*digest_cb_fn_type)(unsigned int sess_hdl,
			struct rocker_p4_mac_learn_digest_digest_msg *msg,
	                void *callback_fn_cookie);
unsigned int
lf_mac_learn_digest_register(unsigned int sess_hdl,
			digest_cb_fn_type cb_fn,
			void *client_data);

unsigned int
lf_mac_learn_digest_deregister(unsigned int sess_hdl);

unsigned int
lf_mac_learn_digest_notify_ack(unsigned int sess_hdl,
		struct rocker_p4_mac_learn_digest_digest_msg *digest_msg);
#endif
