/*
 * Rocker switch hardware register and descriptor definitions.
 *
 * Copyright (c) 2014 Scott Feldman <sfeldma@cumulusnetworks.com>
 * Copyright (c) 2014 Jiri Pirko <jiri@resnulli.us>
 *
 */

#ifndef _ROCKER_HW_
#define _ROCKER_HW_

#define __le16 uint16_t
#define __le32 uint32_t
#define __le64 uint64_t

/*
 * PCI configuration space
 */

#define ROCKER_PCI_REVISION             0x1
#define ROCKER_PCI_INTERRUPT_PIN        1 /* interrupt pin A */
#define ROCKER_PCI_BAR0_SIZE            0x1000

/*
 * Rocker test registers
 */
#define ROCKER_TEST_REG                 0x0010
#define ROCKER_TEST_REG64               0x0018  /* 8-byte */
#define ROCKER_TEST_IRQ                 0x0020
#define ROCKER_TEST_DMA_ADDR            0x0028  /* 8-byte */
#define ROCKER_TEST_DMA_SIZE            0x0030
#define ROCKER_TEST_DMA_CTRL            0x0034

/*
 * Rocker test register ctrl
 */
#define ROCKER_TEST_DMA_CTRL_CLEAR      (1 << 0)
#define ROCKER_TEST_DMA_CTRL_FILL       (1 << 1)
#define ROCKER_TEST_DMA_CTRL_INVERT     (1 << 2)

/*
 * Rocker IRQ registers
 */
#define ROCKER_IRQ_MASK                 0x0200
#define ROCKER_IRQ_STAT                 0x0204

/*
 * Rocker IRQ status bits
 */
#define ROCKER_IRQ_LINK                 (1 << 0)
#define ROCKER_IRQ_TX_DMA_DONE          (1 << 1)
#define ROCKER_IRQ_RX_DMA_DONE          (1 << 2)
#define ROCKER_IRQ_CMD_DMA_DONE         (1 << 3)
#define ROCKER_IRQ_EVENT_DMA_DONE       (1 << 4)
#define ROCKER_IRQ_TEST_DMA_DONE        (1 << 5)

/*
 * Rocker DMA ring register offsets
 */
#define __ROCKER_DMA_DESC_ADDR(x)       (0x0100 + (x) * 32)     /* 8-byte */
#define __ROCKER_DMA_DESC_SIZE(x)       (0x0108 + (x) * 32)
#define __ROCKER_DMA_DESC_HEAD(x)       (0x010c + (x) * 32)
#define __ROCKER_DMA_DESC_TAIL(x)       (0x0110 + (x) * 32)
#define __ROCKER_DMA_DESC_CTRL(x)       (0x0114 + (x) * 32)
#define __ROCKER_DMA_DESC_RSVD1(x)      (0x0118 + (x) * 32)
#define __ROCKER_DMA_DESC_RSVD2(x)      (0x011c + (x) * 32)

#define ROCKER_DMA_RING_REG_SET(name, index)                                    \
enum {                                                                          \
        ROCKER_ ## name ## _DMA_DESC_ADDR = __ROCKER_DMA_DESC_ADDR(index),      \
        ROCKER_ ## name ## _DMA_DESC_SIZE = __ROCKER_DMA_DESC_SIZE(index),      \
        ROCKER_ ## name ## _DMA_DESC_HEAD = __ROCKER_DMA_DESC_HEAD(index),      \
        ROCKER_ ## name ## _DMA_DESC_TAIL = __ROCKER_DMA_DESC_TAIL(index),      \
        ROCKER_ ## name ## _DMA_DESC_CTRL = __ROCKER_DMA_DESC_CTRL(index),      \
        ROCKER_ ## name ## _DMA_DESC_RSVD1 = __ROCKER_DMA_DESC_RSVD1(index),    \
        ROCKER_ ## name ## _DMA_DESC_RSVD2 = __ROCKER_DMA_DESC_RSVD2(index),    \
};\
enum {\
        ROCKER_ ## name ## _INDEX = index,  \
}

ROCKER_DMA_RING_REG_SET(TX, 0);
ROCKER_DMA_RING_REG_SET(RX, 1);
ROCKER_DMA_RING_REG_SET(CMD, 2);
ROCKER_DMA_RING_REG_SET(EVENT, 3);

/*
 * Helper macro to do convert a dma ring register 
 * to its index.  Based on the fact that the register
 * group stride is 32 bytes.
 */
#define ROCKER_RING_INDEX(reg) ((reg >> 8) & 0xf)

/*
 * Rocker DMA Descriptor and completion structs
 */
struct rocker_dma_tlv {
    uint32_t type;
    uint16_t len;
} __attribute__((packed, aligned (8)));

struct rocker_desc {
    __le64 buf_addr;
    uint64_t cookie;
    __le16 buf_size;
    __le16 tlv_size;
    __le16 resv[5];   /* pad to 32 bytes */
    __le16 comp_status;
} __attribute__((packed, aligned (8)));

/*
 * Rocker TLV type fields
 */

enum {
    /* Nest type */
    TLV_NEST = 1,

    /* Logical port number */
    TLV_LPORT,

    /* TX TLV's */
    TLV_TX_OFFLOAD,
    TLV_TX_L3_CSUM_OFF,
    TLV_TX_TSO_MSS,
    TLV_TX_TSO_HDR_LEN,
    TLV_TX_FRAG_CNT,
    TLV_TX_FRAG,

    /* RX TLV's */
    TLV_RX_FLAGS,
    TLV_RX_CSUM,
    TLV_RX_PACKET_LEN,
    TLV_RX_PACKET,

    /* Flow Table TLV's */
    TLV_FLOW_CMD,
    TLV_FLOW_TBL,
    TLV_FLOW_PRIORITY,
    TLV_FLOW_HARDTIME,
    TLV_FLOW_IDLETIME,
    TLV_FLOW_COOKIE,
    TLV_FLOW_IG_PORT,               /* nest */
    TLV_FLOW_IN_PORT,               /* lport */
    TLV_FLOW_IN_PORT_MASK,
    TLV_FLOW_VLAN,                  /* nest */
    TLV_FLOW_VLAN_ID,
    TLV_FLOW_VLAN_ID_MASK,
    TLV_FLOW_VLAN_PCP,
    TLV_FLOW_VLAN_PCP_MASK,
    TLV_FLOW_NEW_VLAN_ID,
    TLV_FLOW_TERM_MAC,              /* nest */
    TLV_FLOW_ETHERTYPE,
    TLV_FLOW_SRC_MAC,
    TLV_FLOW_SRC_MAC_MASK,
    TLV_FLOW_DST_MAC,
    TLV_FLOW_DST_MAC_MASK,
    TLV_FLOW_BRIDGING,              /* nest */
    TLV_FLOW_TUNNEL_ID,
    TLV_FLOW_GROUP_ID,
    TLV_FLOW_TUN_LOG_PORT,          /* lport */
    TLV_FLOW_OUT_PORT,              /* lport */
    TLV_FLOW_UNICAST_ROUTING,       /* nest */
    TLV_FLOW_DST_IP,
    TLV_FLOW_DST_IP_MASK,
    TLV_FLOW_DST_IPV6,
    TLV_FLOW_DST_IPV6_MASK,
    TLV_FLOW_MULTICAST_ROUTING,     /* nest */
    TLV_FLOW_SRC_IP,
    TLV_FLOW_SRC_IP_MASK,
    TLV_FLOW_SRC_IPV6,
    TLV_FLOW_SRC_IPV6_MASK,
    TLV_FLOW_L2_PORT,               /* lport */
    TLV_FLOW_INDEX,
    TLV_OVERLAY_TYPE,
    TLV_FLOW_ACL,                   /* nest */
    TLV_FLOW_SRC_ARP_IP,
    TLV_FLOW_SRC_ARP_IP_MASK,
    TLV_FLOW_IP_PROTO,
    TLV_FLOW_IP_PROTO_MASK,
    TLV_FLOW_DSCP,
    TLV_FLOW_DSCP_MASK,
    TLV_FLOW_ECN,
    TLV_FLOW_ECN_MASK,
    TLV_FLOW_L4_SRC_PORT,
    TLV_FLOW_L4_SRC_PORT_MASK,
    TLV_FLOW_L4_DST_PORT,
    TLV_FLOW_L4_DST_PORT_MASK,
    TLV_FLOW_ICMP_TYPE,
    TLV_FLOW_ICMP_TYPE_MASK,
    TLV_FLOW_ICMP_CODE,
    TLV_FLOW_ICMP_CODE_MASK,
    TLV_FLOW_IPV6_LABEL,
    TLV_FLOW_IPV6_LABEL_MASK,
    TLV_FLOW_QUEUE_ID_ACTION,
    TLV_FLOW_QUEUE_ID,
    TLV_FLOW_VLAN_PCP_ACTION,
    TLV_FLOW_NEW_VLAN_PCP,
    TLV_FLOW_DSCP_ACTION,
    TLV_FLOW_NEW_DSCP,
    TLV_FLOW_CLEAR_ACTIONS,
    TLV_FLOW_STAT_DURATION,
    TLV_FLOW_STAT_REF_COUNT,
    TLV_FLOW_STAT_BUCKET_COUNT,
    TLV_FLOW_STAT_RX_PKT,
    TLV_FLOW_STAT_TX_PKT,
    TLV_FLOW_GROUP_CMD,
    TLV_FLOW_GROUP_TYPE,
    TLV_FLOW_GROUP_ACTION,          /* nest */

    /* Trunking TLV's */
    TLV_TRUNK_CMD,
    TLV_TRUNK_LPORT,                /* lport */
    TLV_TRUNK_HASH,
    TLV_TRUNK_MEMBER,               /* lport */
    TLV_TRUNK_ACTIVE,

    /* Bridging TLV's */
    TLV_BRIDGE_CMD,
    TLV_BRIDGE_LPORT,               /* lport */
    TLV_BRIDGE_MEMBER,              /* lport */
};

/*
 * Command TLV value definitions
 */
#define CMD_ADD                         0
#define CMD_DEL                         1
#define CMD_MOD                         2
#define CMD_GET_STATS                   3

/*
 * Tx offload modes
 */

#define TX_OFFLOAD_NONE                 0
#define TX_OFFLOAD_IP_CSUM              1
#define TX_OFFLOAD_TCP_UDP_CSUM         2
#define TX_OFFLOAD_L3_CSUM              3
#define TX_OFFLOAD_TSO                  4

/*
 * Rx parsing flags
 */

#define RX_FLAGS_IPV4                   (1 << 0)
#define RX_FLAGS_IPV6                   (1 << 1)
#define RX_FLAGS_CSUM_CALC              (1 << 2)
#define RX_FLAGS_IPV4_CSUM_GOOD         (1 << 3)
#define RX_FLAGS_IP_FRAG                (1 << 4)
#define RX_FLAGS_TCP                    (1 << 5)
#define RX_FLAGS_UDP                    (1 << 6)
#define RX_FLAGS_TCP_UDP_CSUM_GOOD      (1 << 7)

/*
 * Flow group types
 */

enum flow_group_type {
    GROUP_TYPE_L2_INTERFACE = 0,
    GROUP_TYPE_L2_REWRITE,
    GROUP_TYPE_L3_UCAST,
    GROUP_TYPE_L2_MCAST,
    GROUP_TYPE_L2_FLOOD,
    GROUP_TYPE_L3_INTERFACE,
    GROUP_TYPE_L3_MCAST,
    GROUP_TYPE_L3_ECMP,
    GROUP_TYPE_L2_OVERLAY,
};

/*
 * Flow overlay types
 */

enum flow_overlay_type {
    OVERLAY_TYPE_FLOOD_UCAST = 0,
    OVERLAY_TYPE_FLOOD_MCAST,
    OVERLAY_TYPE_MCAST_UCAST,
    OVERLAY_TYPE_MCAST_MCAST,
};

/*
 * Rocker general purpose registers
 */
#define ROCKER_ENDIANESS_SEL            0x0300
#define ROCKER_PORT_PHYS_COUNT          0x0304
#define ROCKER_PORT_PHYS_MODE           0x0308 /* 8-byte */
#define ROCKER_PORT_PHYS_LINK_STATUS    0x0310 /* 8-byte */
#define ROCKER_PORT_PHYS_ENABLE         0x0318 /* 8-byte */

#endif /* _ROCKER_HW_ */
