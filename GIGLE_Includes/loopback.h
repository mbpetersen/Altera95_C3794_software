#ifndef LOOPBACK_H_
#define LOOPBACK_H_

// LOOPBACKSTAT_ptr
// Bit 0: Loop up = 1, Loop down = 0
// Bit 1: L3 = 1, L2 = 0
// Bit 2: "Already" flag
// Bit 3: "Busy" flag
// Bit 4: Timeout
// Bit 5: Received a request
// Bit 6: Remote unit responded

#define	REMOTE_LOOPED_UP		0x41
#define	REMOTE_LOOPED_DOWN		0x40
#define TIMEOUT					0x50
#define	L2_LOOPUP_RQST_RCVD		0x21
#define	L3_LOOPUP_RQST_RCVD		0x23
#define L2_LOOPDWN_RQST_RCVD	0x20
#define L3_LOOPDWN_RQST_RCVD	0x22
#define	REMOTE_ALREADY_UP		0x45
#define	REMOTE_ALREADY_DOWN		0x44
#define	REMOTE_BUSY				0x48


// LOOK_FOR RESPONSE
// LOOP-UP = 1, LOOP-DOWN = 2, HELLO = 3

#define LOOPUP		1
#define LOOPDOWN	2
#define HELLO		3


// LOOPBACK EVENT

#define	LOOPUPTSEA		1
#define	LOOPUPTSEB		2
#define	LOOPUP10G		3
#define	LOOPDOWNTSEA	4
#define	LOOPDOWNTSEB	5
#define	LOOPDOWN10G		6


// Swapper addresses

#define SWAPPER_PEER_MAC1_ptr	0x100
#define	SWAPPER_PEER_MAC0_ptr	0x104
#define SWAPPER_PEER_IP3_ptr	0x108
#define	SWAPPER_PEER_IP2_ptr	0x10C
#define	SWAPPER_PEER_IP1_ptr	0x110
#define	SWAPPER_PEER_IP0_ptr	0x114
#define	SWAPPER_PEER_VLAN2_ptr		0x118
#define	SWAPPER_PEER_VLAN1_ptr		0x11C

#define SWAPPER_SELF_MAC1_ptr	0x120
#define SWAPPER_SELF_MAC0_ptr	0x124
#define	SWAPPER_SELF_IP3_ptr	0x128
#define	SWAPPER_SELF_IP2_ptr	0x12C
#define	SWAPPER_SELF_IP1_ptr	0x130
#define	SWAPPER_SELF_IP0_ptr	0x134
#define	SWAPPER_SELF_VLAN2_ptr		0x138
#define	SWAPPER_SELF_VLAN1_ptr		0x13C

// Sniffer

#define SNIFFER_ISR_REGISTER	0x10

#define IDENTITY_TABLE_BASE		0x100
#define SWPR_IDTABLE_VLAN_BASE	0x118
#define BUCKET_ENTRY_SIZE		0x28

#define	ONEHOT_L2_MASK			0x100
#define ONEHOT_VLAN_MASK		0x200
#define	ONEHOT_STACKED_VLAN_MASK	0x600
#define ONEHOT_IPV4_MASK		0x800
#define	ONEHOT_IPV6_MASK		0x1000
#define	ONEHOT_FLAG_MASK		0xC000
#define	ONEHOT_PKTTYPE_MASK		0x3F

#define	HELLO_RESPONSE			0x20
#define	LOOPUP_RESPONSE			0x10
#define	LOOPDOWN_RESPONSE		0x8
#define	HELLO_REQUEST			0x4
#define	LOOPUP_REQUEST			0x2
#define	LOOPDOWN_REQUEST		0x1





#endif
