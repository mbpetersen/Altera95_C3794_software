/************************************************************************
*	config frame by type and parameters		  			 *
*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, CA USA.*
* All rights reserved.                                                   		 *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"

//_________________EGEN_PKT_TYPE_ptr____________________
// 0000,0000:  ^10:Ethernet, ^32:VLAN, ^54:IP, ^76:future    VLAN=0x04, SVLAN=0x08, IPv4=0x10,14,18  IPv6=0x20,24,28
// 10: 00=std Layer2-Ethernet, 01=EthernetII, 10=802.3 LLC, 11=802.3SNAP
// 32: 00=none, 01=VLAN, 10=StackedVLAN, 11:future 3-VLAN's
// 54: 00=none, 01=IPv4, 10=IPv6
// 76: future (compatibility modes)
//	SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(unsigned int value)
//		HDRLEN_ETH		0x0E0000		//14	0E
//		HDRLEN_VLAN		0x120000		//18	12
//		HDRLEN_SVLAN		0x160000		//22	16
//		HDRLEN_IP4		0x220000		//34	22
//		HDRLEN_VLANIP4	0x260000		//38	26
//		HDRLEN_SVLANIP4	0x2A0000		//42	2A
//		HDRLEN_IP6		0x360000		//54	36
//		HDRLEN_VLANIP6	0x3A0000		//58	3A
//		HDRLEN_SVLANIP6	0x3E0000		//62	3E
// If any of these change - update the HDR's as the User changes them!
//case EGEN_VLAN1_TCI1_ptr:	//  92	// VLAN-1 TCI: PCP,DEI,ID  (3-bits,1-bit,12-bits)
//case EGEN_VLAN1_TCI0_ptr:	//  93	// if 1-VLAN then 8100+TCI, if stacked-VLAN then 88A8+TCI
//case EGEN_VLAN2_TCI1_ptr:	// 	94	// VLAN-2 TCI: PCP,DEI,ID  (3-bits,1-bit,12-bits)
//case EGEN_VLAN2_TCI0_ptr:	// 	95	// if 1-VLAN then 8100+TCI
//case EGEN_PKT_TYPE_ptr: 		//100

//set all frame parameters as required by User selected frame type
void config_frame_structure()
{
	/*if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x01) == 0x00){				// Our Std Layer-2 (802.3)
		D(1, BUG("___Going to Generate 802.3 Packets from config_frame___\n"));
		config_frame_raw802_3();	// Our Std Layer-2 (802.3)
		}
		else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x01) == 0x01){	// EthernetII - same as raw 802.3 replace length with type=0x0800!
		D(1, BUG("___Going to Generate ETH || Packets from config_frame___\n"));
		config_frame_ethII();	// EthernetII - same as raw 802.3 replace length with type=0x0800!							DUMP_GEN_HDRS_BY_STREAM();
		}
	**
	else if(RxBuffer[EGEN_PKT_TYPE_ptr]&0x03 == 0x02)	// 802.3 LLC
		SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_ETH);
	else if(RxBuffer[EGEN_PKT_TYPE_ptr]&0x03 == 0x03)	// 802.3SNAP
		SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_ETH);
	**
	   // VLAN
	   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | MAC destination address                                         | MAC source address  | 1	HDR0,1
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                     | 0x8100              | VLAN TCI            | 2	HDR2,HDR3=0x8100+TCI
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | Frame length        | Sequence number                                                 | 3	HDR4=len/type
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | Stream              | Time stamp                                | Payload ...         | 4
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
	else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x04){	// VLAN
		config_frame_vlan()	;	// VLAN
		}

	   // STACKED VLAN
	   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | MAC destination address                                         | MAC source address  | 1   HDR0,1
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                     | 0x88a8              | VLAN TCI            | 2	HDR2, HDR3=0x88a8+TCI1
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | 0x8100              | VLAN TCI            | Frame length        | Sequence number     | 3	HDR4=0x8100+TCI2, HDR4=len/type+0x0000
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                     | Stream              | Time stamp          | 4
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.               | Payload ...                                                     | 5
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
	else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x08){	// Stacked-VLAN
		config_frame_stacked_vlan();	// STACKED VLAN
		}

	   // IPv4
	   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | MAC destination address                                 		    | MAC source address | 1	HDR0, HDR1
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                     	     | 0x0800               | Ver/IHL  | DSCP/ECN | 2	HDR2, HDR3=0x0800+IP1.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | Total Length        | Identification      | Flags/Fragment offst| TTL      | Protocol | 3	HDR4=IP1.10+IP2.32, HDR5=IP2.10+IPV4_HDR.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | Header checksum     | Source IP address                         | Destination IP addr | 4	HDR6=IPV4_HDR.10+IPV4_SRC.32, HDR7=IPV4_SRC.10+DEST1_IP.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.               | Sequence number                                                 | 5	HDR8= DEST1_IP.10 + 0x00
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | Stream              | Time stamp                                | Payload ...         | 6
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
	else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x30) == 0x10){	// IPv4
		if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x10){		// Straight IP - no VLAN
			config_frame_ipv4();	 // IPv4
			}

		   // VLAN + IPv4
		   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | MAC destination address                                         | MAC source address  | 1	HDR0, HDR1
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | cont.                                     | 0x8100              | VLAN TCI            | 2	HDR2,HDR3=0x8100+TCI
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | 0x0800              | Ver/IHL  | DSCP/ECN | Total Length        | Identification      | 3
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | Flags/Fragment offst| TTL      | Protocol | Header checksum     | Source IP address   | 4
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | cont.               | Destination IP addr                       | Sequence number     | 5
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | cont.                                     | Stream              | Time stamp          | 6
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | cont.               | Payload ...                                                     | 7
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
		else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x14){	// IPv4+VLAN
			config_frame_ipv4_vlan();	 // VLAN + IPv4
			}

		   // has_stacked_vlan_mac_ipv4
		   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | MAC destination address                                         | MAC source address  | 1
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | cont.                                     | 0x88a8              | VLAN TCI            | 2
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | 0x8100              | VLAN TCI            | 0x0800              | Ver/IHL  | DSCP/ECN | 3
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | Total Length        | Identification      | Flags/Fragment offst| TTL      | Protocol | 4
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | Header checksum     | Source IP address                         | Destination IP addr | 5
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | cont.               | Sequence number                                                 | 6
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   // | Stream              | Time stamp                                | Payload ...         | 7
		   // +----------+----------+----------+----------+----------+----------+----------+----------+
		   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
		else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x18){	// IPv4+Stacked-VLAN
			config_frame_ipv4_stacked_vlan();
			}
		}

	else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x30) == 0x20){		// IPv6
		if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x20){			// Straight IP - no VLAN
			//D(1, BUG("___Going to Generate IPv6 Packets from config_frame___\n"));
			//SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_IP6);
			// TODO	setup HDR's as necessary
			config_frame_ipv6();

			}
		else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x24){	// IPv6+VLAN
			D(1, BUG("___Generate VLAN-tagged IPv6 Packets___\n"));
		//	SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_VLANIP6);
			// TODO	setup HDR's as necessary
			config_frame_ipv6_vlan();
			}
		else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x28){	// IPv6+Stacked-VLAN
			D(1, BUG("___Generate Stacked-VLAN-tagged IPv6 Packets___\n"));
			//SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_SVLANIP6);
			// TODO	setup HDR's as necessary
			config_frame_ipv6_stacked_vlan();
			}
		}


}



void config_frame_raw802_3()	// Our Std Layer-2 (802.3)
{
	//if(RxBuffer[EGEN_PKT_TYPE_ptr] == 0)
	D(1, BUG("___Setting Raw 802.3 (layer-2) Packets___\n"));
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_ETH);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_LAY2);		//GEN_CONFIG2: for 802.3 length field starts at byte 12
	SET_GEN_IPV4_HCSOFS_BY_STREAM(OFF, 0);

	//_______set GEN_HDR's______________________________________________
	unsigned int header_len = (HDRLEN_ETH/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	unsigned long hdr3  = traffic_gen_pkt_length;
	if(traffic_gen_pkt_length > 1518){		// if Jumbo frame then,
		SET_TRAFFIC_GEN_JUMBO_BY_STREAM(YES);
		hdr3 = 0x8870;						// tag JUMBO into len/type field
		}
	else{									// instead of payload length
		SET_TRAFFIC_GEN_JUMBO_BY_STREAM(NO);
		hdr3 = traffic_gen_pkt_length-header_len-4;		// payload length = packet-length-header_len-4 for FCS
		}
	//HDR3 is location of length field for raw 802.3 frames...
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3, hdr3*0x10000);
	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR4, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_ETH);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	//write_traffic_reg(MON, TRAFFIC_MON_HDR3, hdr3);
	//write_traffic_reg(MON, TRAFFIC_MON_MSK3, 0xFFFF0000);	//(for now) Trap on LEN/TYPE field tag!
}

void config_frame_ethII()	// EthernetII - same as raw 802.3 replace length with type=0x0800!
{
	//if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x03) == 0x01){
	D(1, BUG("___Setting Eth-II (layer-2) Packets___\n"));
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_ETH);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_LAY2);		//GEN_CONFIG2: for 802.3 length field starts at byte 12
	SET_GEN_IPV4_HCSOFS_BY_STREAM(OFF, 0);

	//_______set GEN_HDR's______________________________________________
	//lenField not used on EthII: unsigned int header_len = (HDRLEN_ETH/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	unsigned long hdr3  = traffic_gen_pkt_length;
	//this is set in EGEN_PKT_LENGTH1_ptr change!
	//if(traffic_gen_pkt_length > 1518)		// if Jumbo frame then,
	//	SET_TRAFFIC_GEN_JUMBO_BY_STREAM(YES);
	//else
	//	SET_TRAFFIC_GEN_JUMBO_BY_STREAM(NO);
	hdr3 = 0x0800;	// EthII Always Type (0x0800)
	//HDR3 is location of type field for Eth-II frames (0x0800)...
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3, hdr3*0x10000);
	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR4, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_ETH);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR3, hdr3);
	write_traffic_reg(MON, TRAFFIC_MON_MSK3, 0xFFFF0000);	//(for now) Trap on TYPE field tag!
}

**
else if(RxBuffer[EGEN_PKT_TYPE_ptr]&0x03 == 0x02)	// 802.3 LLC
	SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_ETH);
else if(RxBuffer[EGEN_PKT_TYPE_ptr]&0x03 == 0x03)	// 802.3SNAP
	SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_ETH);
**

void config_frame_vlan()	// VLAN
{
   // VLAN
   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | MAC destination address                                         | MAC source address  | 1	HDR0,1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | 0x8100              | VLAN TCI            | 2	HDR2,HDR3=0x8100+TCI
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Frame length        | Sequence number                                                 | 3	HDR4=len/type
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Stream              | Time stamp                                | Payload ...         | 4
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
//if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x04){	// VLAN
	D(1, BUG("___Generate VLAN Packets___\n"));
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_VLAN);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_VLAN);		//GEN_CONFIG2:
	SET_GEN_IPV4_HCSOFS_BY_STREAM(OFF, 0);

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_VLAN1_TCI1_ptr);
	hdr3 += 0x81000000;			// 8100tci1:  Make it 0x8100xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3,hdr3);
	//HDR4 is location of length field for VLAN frames...
	unsigned int header_len = (HDRLEN_VLAN/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, (traffic_gen_pkt_length-header_len-4)*0x10000); // payload length = packet-length-header_len-4 for FCS
	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR5, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_VLAN);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR3, hdr3);
	write_traffic_reg(MON, TRAFFIC_MON_MSK3, 0xFFFFFFFF);	// Trap on VLAG tag!
}


void config_frame_stacked_vlan()	// STACKED VLAN
{
   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | MAC destination address                                         | MAC source address  | 1   HDR0,1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | 0x88a8              | VLAN TCI            | 2	HDR2, HDR3=0x88a8+TCI1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | 0x8100              | VLAN TCI            | Frame length        | Sequence number     | 3	HDR4=0x8100+TCI2, HDR4=len/type+0x0000
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | Stream              | Time stamp          | 4
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.               | Payload ...                                                     | 5
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
//if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x08){	// Stacked-VLAN
	D(1, BUG("___Generate Stacked-VLAN Packets___\n"));
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_SVLAN);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_SVLAN);		//GEN_CONFIG2:
	SET_GEN_IPV4_HCSOFS_BY_STREAM(OFF, 0);

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_VLAN1_TCI1_ptr);
	hdr3 += 0x88a80000;			// 88a8tci1:  Make it 0x88a8xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3, hdr3);
	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_VLAN2_TCI1_ptr);
	hdr4 += 0x81000000;			// 8100tci1:  Make it 0x8100xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);
	//HDR5 is location of length field for Stacked VLAN frames...
	unsigned int header_len = (HDRLEN_SVLAN/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, (traffic_gen_pkt_length-header_len-4)*0x10000); // payload length = packet-length-header_len-4 for FCS
	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR6, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_SVLAN);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR3, hdr3);
	write_traffic_reg(MON, TRAFFIC_MON_HDR4, hdr4);
	write_traffic_reg(MON, TRAFFIC_MON_MSK3, 0xFFFFFFFF);	// Trap on 1st and 2nd VLAG tag!
	write_traffic_reg(MON, TRAFFIC_MON_MSK4, 0xFFFFFFFF);
}

void config_frame_ipv4()	 // IPv4
{
//  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
// +----------+----------+----------+----------+----------+----------+----------+----------+
// | MAC destination address                                 		    | MAC source address| 1	HDR0, HDR1
// +----------+----------+----------+----------+----------+----------+----------+----------+
// | cont.                                     	     	  | 0x0800      |Ver/IHL | DSCP/ECN| 2	HDR2, HDR3=0x0800+IP1.32
// +----------+----------+----------+----------+----------+----------+----------+----------+
// | Total Length       | Identification      | Flags/FragBits     | TTL      | Prot.    | 3	HDR4=IP1.10+IP2.32, HDR5=IP2.10+IPV4_HDR.32
// +----------+----------+----------+----------+----------+----------+----------+----------+
// | Header checksum     | Source IP address                         | Destination IP addr | 4	HDR6=IPV4_HDR.10+IPV4_SRC.32, HDR7=IPV4_SRC.10+DEST1_IP.32
// +----------+----------+----------+----------+----------+----------+----------+----------+
// | cont.               | Sequence number                                                 | 5	HDR8= DEST1_IP.10 + 0x00
// +----------+----------+----------+----------+----------+----------+----------+----------+
// | Stream              | Time stamp                                | Payload ...         | 6
// +----------+----------+----------+----------+----------+----------+----------+----------+
//  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
//if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x30) == 0x10){	// IPv4
//if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x10){		// Straight IP - no VLAN
	D(1, BUG("___Generate IPv4 Packets___\n"));
	//HDR3=0x0800+IP1.32					EGEN_IP1_HEADER3_ptr
	//HDR4=IP1.10+IP2.32					EGEN_IP2_HEADER3_ptr
	//HDR5=IP2.10+IPV4_HDR.32			EGEN_IPV4_HDR3_ptr
	//HDR6=IPV4_HDR.10+IPV4_SRC.32		EGEN_IPV4_SRC3_ptr
	//HDR7=IPV4_SRC.10+DEST1_IP.32		EGEN_DEST1_IP3_ptr
	//HDR8= DEST1_IP.10 + 0x00
	//
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_IP4);	//(HCSOFS=24)
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_IPV4);		//GEN_CONFIG2:
	//This is the beginning of checksum byte field counting from 0.  Therefore, the value for this would be 0x18 or 24 for IPV4.
	//Each VLAN tag adds +4 to base IP offset of 24
	//Each MPLS tag adds +6 to base IP offset of 24
	//NEXT TWO only done on IPv4 Frame Types..................
	SET_GEN_IPV4_HCSOFS_BY_STREAM(ON, HDRLEN_IP4);
	// this is calc'd and written by UI into the appropriate IP HDR word (IPV4_HDR.10)!
	//unsigned int value = BytesToInt(RxBuffer,EGEN_IPV4_HDR1_ptr);
	//D(1, BUG("\EGEN_IPV4_HDR1_ptr&0 value =%08X\n", value));
	SET_GEN_IPV4_IHCS_BY_STREAM(BytesToInt(RxBuffer,EGEN_IPV4_HDR1_ptr));// IPV4_1 & 0 has IHCS value
	//note: really no need to write into the HDR - since it's calc'd by the GEN

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_IP1_HEADER3_ptr);	//HDR3=0x0800+IP1.32
	hdr3 += 0x08000000;			// 0x0800
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3, hdr3);

	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)*0x10000;
	hdr4 += BytesToInt(RxBuffer,EGEN_IP2_HEADER3_ptr);	//HDR4=IP1.10+IP2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);

	unsigned long hdr5  = BytesToInt(RxBuffer,EGEN_IP2_HEADER1_ptr)*0x10000;
	hdr5 += BytesToInt(RxBuffer,EGEN_IPV4_HDR3_ptr);		//HDR5=IP2.10+IPV4_HDR.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, hdr5);

	unsigned long hdr6  = BytesToInt(RxBuffer,EGEN_IPV4_HDR1_ptr)*0x10000;
	hdr6 += BytesToInt(RxBuffer,EGEN_IPV4_SRC3_ptr);		//HDR6=IPV4_HDR.10+IPV4_SRC.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR6, hdr6);

	unsigned long hdr7  = BytesToInt(RxBuffer,EGEN_IPV4_SRC1_ptr)*0x10000;
	hdr7 += BytesToInt(RxBuffer,EGEN_DEST1_IP3_ptr);		//HDR7=IPV4_SRC.10+DEST1_IP.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR7, hdr7);

	unsigned long hdr8  = BytesToInt(RxBuffer,EGEN_DEST1_IP1_ptr)*0x10000;
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR8, hdr8);		//HDR8= DEST1_IP.10 + xxxx

	//HDRx is location of length field for IPv4 frames...
	//unsigned int header_len = (HDRLEN_IP4/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	//write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, (traffic_gen_pkt_length-header_len-4)*0x10000); // payload length = packet-length-header_len-4 for FCS

	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR9, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_IP4);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR6, hdr6);
	write_traffic_reg(MON, TRAFFIC_MON_HDR7, hdr7);
	write_traffic_reg(MON, TRAFFIC_MON_HDR8, hdr8);
	write_traffic_reg(MON, TRAFFIC_MON_MSK6, 0x0000FFFF);	// Trap on Src-IP Addr!  (assuming lpbk-device swapped IP addresses or it's our src on hard loop)
	write_traffic_reg(MON, TRAFFIC_MON_MSK7, 0xFFFF0000);
	//write_traffic_reg(MON, TRAFFIC_MON_MSK7, 0x0000FFFF);	// Trap on Dest-IP Addr!
	//write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0xFFFF0000);
}

void config_frame_ipv4_vlan()	 // VLAN + IPv4
{
   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | MAC destination address                                         | MAC source address  | 1	HDR0, HDR1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | 0x8100              | VLAN TCI            | 2	HDR2,HDR3=0x8100+TCI
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | 0x0800              | Ver/IHL  | DSCP/ECN | Total Length        | Identification      | 3
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Flags/Fragment offst| TTL      | Protocol | Header checksum     | Source IP address   | 4
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.               | Destination IP addr                       | Sequence number     | 5
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | Stream              | Time stamp          | 6
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.               | Payload ...                                                     | 7
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
//else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x14){	// IPv4+VLAN
	D(1, BUG("___Generate VLAN-tagged IPv4 Packets___\n"));
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_VLANIP4);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_VLANIPV4);		//GEN_CONFIG2:
	SET_GEN_IPV4_HCSOFS_BY_STREAM(ON, HDRLEN_VLANIP4);
	SET_GEN_IPV4_IHCS_BY_STREAM(BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)); // IP1 & 0 has IHCS value

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_VLAN1_TCI1_ptr);
	hdr3 += 0x81000000;			// 8100tci1:  Make it 0x8100xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3,hdr3);

	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_IP1_HEADER3_ptr); //HDR4=0x0800+IP1.32
	hdr4 += 0x08000000;			// 0x0800
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);

	unsigned long hdr5  = BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)*0x10000;
	hdr5 += BytesToInt(RxBuffer,EGEN_IP2_HEADER3_ptr);	//HDR5=IP1.10+IP2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, hdr5);

	unsigned long hdr6  = BytesToInt(RxBuffer,EGEN_IP2_HEADER1_ptr)*0x10000;
	hdr6 += BytesToInt(RxBuffer,EGEN_IPV4_HDR3_ptr);	//HDR6=IP2.10+IPV4_HDR.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR6, hdr6);

	unsigned long hdr7  = BytesToInt(RxBuffer,EGEN_IPV4_HDR1_ptr)*0x10000;
	hdr7 += BytesToInt(RxBuffer,EGEN_IPV4_SRC3_ptr);		//HDR7=IPV4_HDR.10+IPV4_SRC.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR7, hdr7);

	unsigned long hdr8  = BytesToInt(RxBuffer,EGEN_IPV4_SRC1_ptr)*0x10000;
	hdr8 += BytesToInt(RxBuffer,EGEN_DEST1_IP3_ptr);	//HDR8=IPV4_SRC.10+DEST1_IP.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR8, hdr8);

	unsigned long hdr9  = BytesToInt(RxBuffer,EGEN_DEST1_IP1_ptr)*0x10000;
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR9, hdr9);		//HDR9= DEST1_IP.10 + xxxx

	//HDRx is location of length field for IPv4 frames...
	//unsigned int header_len = (HDRLEN_VLANIP4/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	//write_traffic_reg(GEN, TRAFFIC_GEN_HDRx, (traffic_gen_pkt_length-header_len-4)*0x10000); // payload length = packet-length-header_len-4 for FCS

	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR10, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_VLANIP4);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR3, hdr3);
	write_traffic_reg(MON, TRAFFIC_MON_HDR7, hdr7);
	write_traffic_reg(MON, TRAFFIC_MON_HDR8, hdr8);
	write_traffic_reg(MON, TRAFFIC_MON_HDR9, hdr9);
	write_traffic_reg(MON, TRAFFIC_MON_MSK3, 0xFFFFFFFF);	// Trap on VLAG tag!
	//write_traffic_reg(MON, TRAFFIC_MON_MSK7, 0x0000FFFF);	// Trap on Src-IP Addr!  (assuming lpbk-device swapped IP addresses or it's our src on hard loop)
	//write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0xFFFF0000);
	//write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0x0000FFFF);	// Trap on Dest-IP Addr!
	//write_traffic_reg(MON, TRAFFIC_MON_MSK9, 0xFFFF0000);
}

void config_frame_ipv4_stacked_vlan()	 // has_stacked_vlan_mac_ipv4
{
   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | MAC destination address                                         | MAC source address  | 1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | 0x88a8              | VLAN TCI            | 2
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | 0x8100              | VLAN TCI            | 0x0800              | Ver/IHL  | DSCP/ECN | 3
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Total Length        | Identification      | Flags/Fragment offst| TTL      | Protocol | 4
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Header checksum     | Source IP address                         | Destination IP addr | 5
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.               | Sequence number                                                 | 6
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Stream              | Time stamp                                | Payload ...         | 7
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
// if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x18){	// IPv4+Stacked-VLAN
	D(1, BUG("___Generate Stacked-VLAN-tagged IPv4 Packets___\n"));
	//
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_SVLANIP4);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_SVLANIPV4);		//GEN_CONFIG2:
	SET_GEN_IPV4_HCSOFS_BY_STREAM(ON, HDRLEN_SVLANIP4);
	SET_GEN_IPV4_IHCS_BY_STREAM(BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)); // IP1 & 0 has IHCS value

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_VLAN1_TCI1_ptr);
	hdr3 += 0x88a80000;			// 88a8tci1:  Make it 0x88a8xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3,hdr3);

	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_VLAN2_TCI1_ptr);
	hdr4 += 0x81000000;			// 8100tci1:  Make it 0x8100xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);

	unsigned long hdr5  = BytesToInt(RxBuffer,EGEN_IP1_HEADER3_ptr); //HDR5=0x0800+IP1.32
	hdr5 += 0x08000000;			// 0x0800
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, hdr5);

	unsigned long hdr6  = BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)*0x10000;
	hdr6 += BytesToInt(RxBuffer,EGEN_IP2_HEADER3_ptr);	//HDR6=IP1.10+IP2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR6, hdr6);

	unsigned long hdr7  = BytesToInt(RxBuffer,EGEN_IP2_HEADER1_ptr)*0x10000;
	hdr7 += BytesToInt(RxBuffer,EGEN_IPV4_HDR3_ptr);	//HDR7=IP2.10+IPV4_HDR.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR7, hdr7);

	unsigned long hdr8  = BytesToInt(RxBuffer,EGEN_IPV4_HDR1_ptr)*0x10000;
	hdr8 += BytesToInt(RxBuffer,EGEN_IPV4_SRC3_ptr);		//HDR8=IPV4_HDR.10+IPV4_SRC.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR8, hdr8);

	unsigned long hdr9  = BytesToInt(RxBuffer,EGEN_IPV4_SRC1_ptr)*0x10000;
	hdr9 += BytesToInt(RxBuffer,EGEN_DEST1_IP3_ptr);	//HDR9=IPV4_SRC.10+DEST1_IP.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR9, hdr9);

	unsigned long hdr10  = BytesToInt(RxBuffer,EGEN_DEST1_IP1_ptr)*0x10000;
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR10, hdr10);		//HDR10= DEST1_IP.10 + xxxx

	//HDRx is location of length field for IPv4 frames...
	//unsigned int header_len = (HDRLEN_VLANIP4/0x10000);	// actual decimal value of header length (not bit shifted Register value)
	//write_traffic_reg(GEN, TRAFFIC_GEN_HDRx, (traffic_gen_pkt_length-header_len-4)*0x10000); // payload length = packet-length-header_len-4 for FCS

	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR11, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_SVLANIP4);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR3, hdr3);
	write_traffic_reg(MON, TRAFFIC_MON_HDR8, hdr8);
	write_traffic_reg(MON, TRAFFIC_MON_HDR9, hdr9);
	write_traffic_reg(MON, TRAFFIC_MON_HDR10, hdr10);
	write_traffic_reg(MON, TRAFFIC_MON_MSK3, 0xFFFFFFFF);	// Trap on VLAG tag!
	//write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0x0000FFFF);	// Trap on Src-IP Addr!  (assuming lpbk-device swapped IP addresses or it's our src on hard loop)
	//write_traffic_reg(MON, TRAFFIC_MON_MSK9, 0xFFFF0000);
	//write_traffic_reg(MON, TRAFFIC_MON_MSK9, 0x0000FFFF);	// Trap on Dest-IP Addr!
	//write_traffic_reg(MON, TRAFFIC_MON_MSK10, 0xFFFF0000);
}

***** COMING SOON!
						else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x30) == 0x20){		// IPv6
							if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x20){			// Straight IP - no VLAN
								D(1, BUG("___Generate IPv6 Packets___\n"));
								SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_IP6);
								// TODO	setup HDR's as necessary
								}
							else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x24){	// IPv6+VLAN
								D(1, BUG("___Generate VLAN-tagged IPv6 Packets___\n"));
								SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_VLANIP6);
								// TODO	setup HDR's as necessary
								}
							else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x28){	// IPv6+Stacked-VLAN
								D(1, BUG("___Generate Stacked-VLAN-tagged IPv6 Packets___\n"));
								SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_SVLANIP6);
								// TODO	setup HDR's as necessary
								}
							}
**********
void config_frame_ipv6()	 // IPv4
{

	 //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | MAC destination address                                         | MAC source address  | 1	HDR0, HDR1
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                                | 0x86DD   |Ver  |Trfic cls|flw  | 2	HDR2,HDR3=IP1.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // |Flow label           | Payload length      | Nxt Hdr  | Hop limit|  Source IP address  | 3   HDR4=IP1.10+IP2.32,HDR5=IP2.10+SRC1.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                                                                 | 4   HDR6=SRC1.10+SRC2.32,HDR7=SRC2.10+SRC3.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.               |                                                      |Dest Addr | 5   HDR8=SRC3.10+SRC4.32,HDR9=SRC4.10+DEST1.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                                                                 | 6   HDR10=DEST1.10+DEST2.32,HDR11=DEST2.10+DEST3.32
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   // | cont.                                                          |                      | 7   HDR12=DEST3.10+DEST4.32,HDR13=DEST4.10
	   // +----------+----------+----------+----------+----------+----------+----------+----------+
	   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0

//	D(1, BUG("___Generate IPv6 Packets___\n"));
	//HDR3=0x86DD+IP1.32					EGEN_IP1_HEADER3_ptr
	//HDR4=IP1.10+IP2.32					EGEN_IP1_HEADER3_ptr
	//HDR5=IP2.10+SRC1.32
	//HDR6=SRC1.10+SRC2.32
	//HDR7=SRC2.10+SRC3.32
	//HDR8=SRC3.10+SRC4.32
	//HDR9=SRC4.10+DEST1.32
	//HDR10=DEST1.10+DEST2.32
	//HDR11=DEST2.10+DEST3.32
	//HDR12=DEST3.10+DEST4.32
	//HDR13=DEST4.10
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;


	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_IP6);	//(HCSOFS=24)
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_IPV6);		//GEN_CONFIG2:
	SET_GEN_IPV4_HCSOFS_BY_STREAM(OFF, 0);

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_IP1_HEADER3_ptr);	//HDR3=0x86DD+IP1.32
	hdr3 += 0x86DD0000;			// 0x86DD
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3, hdr3);

	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)*0x10000;
	hdr4 += BytesToInt(RxBuffer,EGEN_IP2_HEADER3_ptr);	//HDR4=IP1.10+IP2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);

	unsigned long hdr5  = BytesToInt(RxBuffer,EGEN_IP2_HEADER1_ptr)*0x10000;
	hdr5 += BytesToInt(RxBuffer,EGEN_SRC1_IP3_ptr);		//HDR5=IP2.10+SRC1.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, hdr5);

	unsigned long hdr6  = BytesToInt(RxBuffer,EGEN_SRC1_IP1_ptr)*0x10000;
	hdr6 += BytesToInt(RxBuffer,EGEN_SRC2_IP3_ptr);		//HDR6=SRC1.10+SRC2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR6, hdr6);

	unsigned long hdr7  = BytesToInt(RxBuffer,EGEN_SRC2_IP1_ptr)*0x10000;//HDR7=SRC2.10+SRC3.32
	hdr7 += BytesToInt(RxBuffer,EGEN_SRC3_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR7, hdr7);

	unsigned long hdr8  = BytesToInt(RxBuffer,EGEN_SRC3_IP1_ptr)*0x10000;//HDR8=SRC3.10+SRC4.32
	hdr8 += BytesToInt(RxBuffer,EGEN_SRC4_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR8, hdr8);

	unsigned long hdr9  = BytesToInt(RxBuffer,EGEN_SRC4_IP1_ptr)*0x10000;//HDR9=SRC4.10+DEST1.32
	hdr9 += BytesToInt(RxBuffer,EGEN_DEST1_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR9, hdr9);

	unsigned long hdr10  = BytesToInt(RxBuffer,EGEN_DEST1_IP1_ptr)*0x10000;//HDR10=DEST1.10+DEST2.32
	hdr10 += BytesToInt(RxBuffer,EGEN_DEST2_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR10, hdr10);

	unsigned long hdr11  = BytesToInt(RxBuffer,EGEN_DEST2_IP1_ptr)*0x10000;//HDR11=DEST2.10+DEST3.32
	hdr11 += BytesToInt(RxBuffer,EGEN_DEST3_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR11, hdr11);

	unsigned long hdr12  = BytesToInt(RxBuffer,EGEN_DEST3_IP1_ptr)*0x10000;//HDR12=DEST3.10+DEST4.32
	hdr12 += BytesToInt(RxBuffer,EGEN_DEST4_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR12, hdr12);

	unsigned long hdr13  = BytesToInt(RxBuffer,EGEN_DEST4_IP1_ptr)*0x10000;		//HDR13=DEST4.10
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR13, hdr13);

	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR14, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_IP6);	// Config & Enable Stream and Clear MASK REG's

	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR5, hdr5);
	write_traffic_reg(MON, TRAFFIC_MON_HDR6, hdr6);
	write_traffic_reg(MON, TRAFFIC_MON_HDR7, hdr7);
	write_traffic_reg(MON, TRAFFIC_MON_HDR8, hdr8);
	write_traffic_reg(MON, TRAFFIC_MON_HDR9, hdr9);


	write_traffic_reg(MON, TRAFFIC_MON_MSK5, 0x0000FFFF);	// Trap on Src-IP Addr!  (assuming lpbk-device swapped IP addresses or it's our src on hard loop)
	write_traffic_reg(MON, TRAFFIC_MON_MSK6, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK7, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK9, 0xFFFF0000);

}

void config_frame_ipv6_vlan()	 // VLAN + IPv6
{
   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | MAC destination address                                         | MAC source address  | 1	HDR0, HDR1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | 0x8100              | VLAN TCI            | 2	HDR2,HDR3=0x8100+TCI
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // |  0x86DD  |Ver |Trfic cls  |flw                       |Payload length       | Nxt Hdr  | 3
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | Hop limit|  Source IP address                                                         | 4
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.    | Destination IP addr                       								  | 5
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.    | Stream              | Time stamp                                           | 6
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.               | Payload ...                                                     | 7
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   //  63      56 55      48 47      40 39      32 31      24 23      16 15       8 7        0
	//D(1, BUG("___Generate VLAN-tagged IPv6 Packets___\n"));
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_VLANIP6);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_VLANIPV6);		//GEN_CONFIG2:

	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_VLAN1_TCI1_ptr);
	hdr3 += 0x81000000;			// 8100tci1:  Make it 0x8100xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3,hdr3);

	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_IP1_HEADER3_ptr);	//HDR3=0x86DD+IP1.32
	hdr4 += 0x86DD0000;			// 0x86DD
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);

	unsigned long hdr5  = BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)*0x10000;
	hdr5 += BytesToInt(RxBuffer,EGEN_IP2_HEADER3_ptr);	//HDR4=IP1.10+IP2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, hdr5);

	unsigned long hdr6  = BytesToInt(RxBuffer,EGEN_IP2_HEADER1_ptr)*0x10000;
	hdr6 += BytesToInt(RxBuffer,EGEN_SRC1_IP3_ptr);		//HDR5=IP2.10+SRC1.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR6, hdr6);

	unsigned long hdr7  = BytesToInt(RxBuffer,EGEN_SRC1_IP1_ptr)*0x10000;
	hdr7 += BytesToInt(RxBuffer,EGEN_SRC2_IP3_ptr);		//HDR6=SRC1.10+SRC2.32
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR7, hdr7);

	unsigned long hdr8  = BytesToInt(RxBuffer,EGEN_SRC2_IP1_ptr)*0x10000;//HDR7=SRC2.10+SRC3.32
	hdr8 += BytesToInt(RxBuffer,EGEN_SRC3_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR8, hdr8);

	unsigned long hdr9  = BytesToInt(RxBuffer,EGEN_SRC3_IP1_ptr)*0x10000;//HDR8=SRC3.10+SRC4.32
	hdr9 += BytesToInt(RxBuffer,EGEN_SRC4_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR9, hdr9);

	unsigned long hdr10  = BytesToInt(RxBuffer,EGEN_SRC4_IP1_ptr)*0x10000;//HDR9=SRC4.10+DEST1.32
	hdr10 += BytesToInt(RxBuffer,EGEN_DEST1_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR10, hdr10);

	unsigned long hdr11  = BytesToInt(RxBuffer,EGEN_DEST1_IP1_ptr)*0x10000;//HDR10=DEST1.10+DEST2.32
	hdr11 += BytesToInt(RxBuffer,EGEN_DEST2_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR11, hdr11);

	unsigned long hdr12  = BytesToInt(RxBuffer,EGEN_DEST2_IP1_ptr)*0x10000;//HDR11=DEST2.10+DEST3.32
	hdr12 += BytesToInt(RxBuffer,EGEN_DEST3_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR12, hdr12);

	unsigned long hdr13  = BytesToInt(RxBuffer,EGEN_DEST3_IP1_ptr)*0x10000;//HDR12=DEST3.10+DEST4.32
	hdr13 += BytesToInt(RxBuffer,EGEN_DEST4_IP3_ptr);
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR13, hdr13);

	unsigned long hdr14  = BytesToInt(RxBuffer,EGEN_DEST4_IP1_ptr)*0x10000;		//HDR13=DEST4.10
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR14, hdr14);


	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR15, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_VLANIP6);	// Config & Enable Stream and Clear MASK REG's


	// set MON_HDR's____________________________________________________
	write_traffic_reg(MON, TRAFFIC_MON_HDR6, hdr6);
	write_traffic_reg(MON, TRAFFIC_MON_HDR7, hdr7);
	write_traffic_reg(MON, TRAFFIC_MON_HDR8, hdr8);
	write_traffic_reg(MON, TRAFFIC_MON_HDR9, hdr9);
	write_traffic_reg(MON, TRAFFIC_MON_HDR10, hdr10);


		// Trap on Src-IP Addr!  (assuming lpbk-device swapped IP addresses or it's our src on hard loop)
	write_traffic_reg(MON, TRAFFIC_MON_MSK6, 0xFFFF0000);
	write_traffic_reg(MON, TRAFFIC_MON_MSK7, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK9, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK10, 0x0000FFFF);
}

void config_frame_ipv6_stacked_vlan()	 // has_stacked_vlan_mac_ipv6
{
   //  127    120 119    112 111    104 103     96 95      88 87      80 79      72 71      64
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | MAC destination address                                         | MAC source address  | 1
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                     | 0x88a8              | VLAN TCI            | 2
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | 0x8100              | VLAN TCI            | 0x86DD              | Ver/IHL  | DSCP/ECN | 3
   // +----------+----------+----------+----------+----------+----------+----------+----------+
//    |Trfic cls |flw                  |Payload length       | Nxt Hdr  | Hop limit|  Source IP address
   // +----------+----------+----------+----------+----------+----------+----------+----------+
   // | cont.                                                                      |Destination IP addr              | 4
   // +----------+----------+----------+----------+----------+----------+----------+----------+
// // | cont.                      								                   |Stream    | 5
// // +----------+----------+----------+----------+----------+----------+----------+----------+
// // |Time stamp                                                                             | 6
// // +----------+----------+----------+----------+----------+----------+----------+----------+
// // | cont.               | Payload ...                                                     | 7
// // +----------+----------+----------+----------+----------+----------+----------+----------+
	//D(1, BUG("___Generate Stacked-VLAN-tagged IPv6 Packets___\n"));
	//
	//in case packet_len is changed further down in Config_array
	traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
	if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
		traffic_gen_pkt_length = 64;

	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	//GEN CONFIGS + HDR0-HDR2 (MAC ADDR's)______________________________
	SET_GEN_BY_STREAM(traffic_gen_pkt_length, HDRLEN_SVLANIP6);
	SET_TRAFFIC_GEN_LENOFS_BY_STREAM(LENOFS_SVLANIPV6);		//GEN_CONFIG2:


	//_______set GEN_HDR's______________________________________________
	unsigned long hdr3  = BytesToInt(RxBuffer,EGEN_VLAN1_TCI1_ptr);
	hdr3 += 0x88a80000;			// 88a8tci1:  Make it 0x88a8xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR3,hdr3);

	unsigned long hdr4  = BytesToInt(RxBuffer,EGEN_VLAN2_TCI1_ptr);
	hdr4 += 0x81000000;			// 8100tci1:  Make it 0x8100xxxx, xxxx=PCP:DEI:ID word [3:1:12])
	write_traffic_reg(GEN, TRAFFIC_GEN_HDR4, hdr4);

	unsigned long hdr5  = BytesToInt(RxBuffer,EGEN_IP1_HEADER3_ptr);	//HDR3=0x86DD+IP1.32
		hdr5 += 0x86DD0000;			// 0x86DD
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR5, hdr5);

		unsigned long hdr6  = BytesToInt(RxBuffer,EGEN_IP1_HEADER1_ptr)*0x10000;
		hdr6 += BytesToInt(RxBuffer,EGEN_IP2_HEADER3_ptr);	//HDR4=IP1.10+IP2.32
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR6, hdr6);

		unsigned long hdr7  = BytesToInt(RxBuffer,EGEN_IP2_HEADER1_ptr)*0x10000;
		hdr7 += BytesToInt(RxBuffer,EGEN_SRC1_IP3_ptr);		//HDR5=IP2.10+SRC1.32
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR7, hdr7);

		unsigned long hdr8  = BytesToInt(RxBuffer,EGEN_SRC1_IP1_ptr)*0x10000;
		hdr8 += BytesToInt(RxBuffer,EGEN_SRC2_IP3_ptr);		//HDR6=SRC1.10+SRC2.32
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR8, hdr8);

		unsigned long hdr9  = BytesToInt(RxBuffer,EGEN_SRC2_IP1_ptr)*0x10000;//HDR7=SRC2.10+SRC3.32
		hdr9 += BytesToInt(RxBuffer,EGEN_SRC3_IP3_ptr);
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR9, hdr9);

		unsigned long hdr10  = BytesToInt(RxBuffer,EGEN_SRC3_IP1_ptr)*0x10000;//HDR8=SRC3.10+SRC4.32
		hdr10 += BytesToInt(RxBuffer,EGEN_SRC4_IP3_ptr);
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR10, hdr10);

		unsigned long hdr11  = BytesToInt(RxBuffer,EGEN_SRC4_IP1_ptr)*0x10000;//HDR9=SRC4.10+DEST1.32
		hdr11 += BytesToInt(RxBuffer,EGEN_DEST1_IP3_ptr);
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR11, hdr11);

		unsigned long hdr12  = BytesToInt(RxBuffer,EGEN_DEST1_IP1_ptr)*0x10000;//HDR10=DEST1.10+DEST2.32
		hdr12 += BytesToInt(RxBuffer,EGEN_DEST2_IP3_ptr);
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR12, hdr12);

		unsigned long hdr13  = BytesToInt(RxBuffer,EGEN_DEST2_IP1_ptr)*0x10000;//HDR11=DEST2.10+DEST3.32
		hdr13 += BytesToInt(RxBuffer,EGEN_DEST3_IP3_ptr);
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR13, hdr13);

		unsigned long hdr14  = BytesToInt(RxBuffer,EGEN_DEST3_IP1_ptr)*0x10000;//HDR12=DEST3.10+DEST4.32
		hdr14 += BytesToInt(RxBuffer,EGEN_DEST4_IP3_ptr);
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR14, hdr14);

		unsigned long hdr15  = BytesToInt(RxBuffer,EGEN_DEST4_IP1_ptr)*0x10000;		//HDR13=DEST4.10
		write_traffic_reg(GEN, TRAFFIC_GEN_HDR15, hdr15);

	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR16, TRAFFIC_GEN_HDR31); // clear Unused headers

	//_______set MON ___________________________________________________
	SET_MON_BY_STREAM(HDRLEN_SVLANIP6);	// Config & Enable Stream and Clear MASK REG's


	// set MON_HDR's____________________________________________________

	write_traffic_reg(MON, TRAFFIC_MON_HDR7, hdr7);
	write_traffic_reg(MON, TRAFFIC_MON_HDR8, hdr8);
	write_traffic_reg(MON, TRAFFIC_MON_HDR9, hdr9);
	write_traffic_reg(MON, TRAFFIC_MON_HDR10, hdr10);
	write_traffic_reg(MON, TRAFFIC_MON_HDR11, hdr11);

		// Trap on Src-IP Addr!  (assuming lpbk-device swapped IP addresses or it's our src on hard loop)
	write_traffic_reg(MON, TRAFFIC_MON_MSK7, 0xFFFF0000);
	write_traffic_reg(MON, TRAFFIC_MON_MSK8, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK9, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK10, 0xFFFFFFFF);
	write_traffic_reg(MON, TRAFFIC_MON_MSK11, 0x0000FFFF);
}
*/

/*void Write_Gen_Traffic()
//	void config_frame_vlan()
{

//	SET_GEN_MON_TO_STREAM(selected_stream);  //Ensure we're pointing to the current Selected Stream!!

	int i;
	for(i=0;i<122;i=i+4) //frame length should be in one of the gen_configs
	{
		unsigned long hdr  = BytesToInt(RxBuffer,Hdr[i])*0x10000;
		hdr += BytesToInt(RxBuffer,Hdr[i+1]);		//HDR5=IP2.10+SRC1.32

		write_traffic_reg(GEN, TRAFFIC_GEN_HDR[i], hdr);
	}
}*/
/*void Write_Mon_Traffic()
{
int i;
	for(i=0;i<128;i=i+4) //frame length should be in one of the gen_configs
	{
		unsigned long hdr  = BytesToInt(RxBuffer,Hdr[i])*0x10000;
		hdr += BytesToInt(RxBuffer,Hdr[i+1]);		//HDR5=IP2.10+SRC1.32
		write_traffic_reg(MON, TRAFFIC_GEN_HDR[i], hdr);
	}
}*/
}

/*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, CA USA.*
* All rights reserved.                                                   		 *
*************************************************************************/
