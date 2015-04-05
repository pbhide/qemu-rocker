/*
Copyright 2013-present Barefoot Networks, Inc. 

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
/*
 * DO NOT EDIT:  This file is automatically generated.
 *
 */
#ifndef _ROCKER_p4_l2l3_LF_H_
#define _ROCKER_p4_l2l3_LF_H_
typedef struct rocker_p4_l2l3_mac_learn_digest_digest_entry {
  unsigned char ethernet_srcAddr[6];
  unsigned short ingress_metadata_fid;
  unsigned short standard_metadata_ingress_port;
} rocker_p4_l2l3_mac_learn_digest_digest_entry_t;

// Should be able to cast this to pipe_flow_lrn_msg_t.
typedef struct  rocker_p4_l2l3_mac_learn_digest_digest_msg {
  unsigned int                  dev_tgt;
  unsigned short                num_entries;
  rocker_p4_l2l3_mac_learn_digest_digest_entry_t  *entries;
} rocker_p4_l2l3_mac_learn_digest_digest_msg_t;

// Should be able to cast this to pipe_flow_lrn_notify_cb.

typedef unsigned int (*digest_cb_fn_type)(unsigned int sess_hdl,
                                              rocker_p4_l2l3_mac_learn_digest_digest_msg_t *msg,
                                              void *callback_fn_cookie);
unsigned int
lf_mac_learn_digest_register(unsigned int sess_hdl, digest_cb_fn_type cb_fn, void *client_data);

unsigned int
lf_mac_learn_digest_deregister(unsigned int sess_hdl);

unsigned int
lf_mac_learn_digest_notify_ack(unsigned int sess_hdl, rocker_p4_l2l3_mac_learn_digest_digest_msg_t *digest_msg);
#endif //_ROCKER_p4_l2l3_LF_H_
