
/*************************************************************************
*		Config-Status element definitions								 *
*				   														 *
* Copyright (c) 2015 Greenlee Communications Vista, CA 92081    *
* All rights reserved.                                                   *
*************************************************************************/
#ifndef CONFSTAT_H_
#define CONFSTAT_H_

/****************************************************************/
/********** Packet ID's by Function/PCB/Module type *************/
/****************************************************************/
#define Packet_DS3			0x00
#define Packet_DS1A			0x01
#define Packet_DS1B			0x02
#define Packet_DS0			0x03
#define Packet_PRI			0x04
#define Packet_TIMS			0x05
#define Packet_DDS			0x06
#define Packet_DS3_2		0x07
#define Packet_TIME			0x08
#define Packet_E1A			0x09
#define Packet_E1B			0x0A
#define Packet_E3			0x0B
#define Packet_E3_2			0x0C
#define Packet_DCOM			0x0D

#define Packet_SEEPROMT		0x0E
#define Packet_SEEPROM		0x0F

//#define Packet_AUTOTEST	0x80	// OR'd with Packet_TYPE to define AUTO TEST PACKET!
#define Packet_DIGITS		0x10	// Async packet sent to GUI when new DIGITs received

#define Packet_GIGEA		0x20
#define Packet_GIGEA_MISC	0x21
#define Packet_GIGEB		0x28
#define Packet_GIGEB_MISC	0x29

#define Packet_GIGE10A		0x2A
#define Packet_GIGE10A_MISC	0x2B
#define Packet_GIGE10B		0x2C
#define Packet_GIGE10B_MISC	0x2D

/*******************************************************************************/
/*** SpecialStat Status: gSpecialSpare SysInfo Bytes from GIGE Module to GUI ***/
//**    SpecialStat: 128bytes [2-129] 	SFP Core
//**    SpecialStat: 128bytes [130-257] SFP Diag
//**    SpecialStat: 189bytes [258-448] SPARE in SFP Status array
//**    SpecialStat:   1 byte [449]     Status array checksum
//**    SpecialStat:  40bytes [400-439] HW/FW SysInfo
/*******************************************************************************/
// currently save SYS_INFO to SpecialStat[] ONLY (not SpecialStatB[])
/*******************************************************************************/
// first three are stored/hard-coded in gigle.h
#define HW_VERSION_ptr			400	// rev1 to rev255
#define FW_VERSION1_ptr			401	// ver1.ver0 0 -> 65535,  65535/1000 = v0.0 to v65.535
#define FW_VERSION0_ptr			402	// v1.01 -> 101
// these on down are SW configurable....so are stored in and read from Flash
#define FLASH_OPT_INSTALLED_ptr	403 // ^0=dual-1g, ^1=10G-A, ^2=10G-B, (?) THRU,VLAN,IP,OAM
#define FLASH_OPT_ENABLED_ptr	404 // All ACTIVE LOW! FF=nothing installed, FF=nothing enabled

// sernum not used now - just gonna use 0000-FFFF from lower MAC addresses as SN#!
// ...so these can be re-allocated at some point!
#define SERNUM4_ptr				405 // Serial NUM 00000 -> FFFF0 (1048560)
#define SERNUM3_ptr				406 // 00000 & FFFFF reserved
#define SERNUM2_ptr				407
#define SERNUM1_ptr				408
#define SERNUM0_ptr				409

#define MACA_ADDR_MSB3_ptr		410	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MACA_ADDR_MSB2_ptr		411	// these are 0x001C23174ACB
#define MACA_ADDR_MSB1_ptr		412	// loaded into mac_0 = 0x17231C00  (reverse order)
#define MACA_ADDR_MSB0_ptr		413  //    and into mac_1 = 0x0000CB4A
#define MACA_ADDR_LSB1_ptr		414
#define MACA_ADDR_LSB0_ptr		415

#define MACB_ADDR_MSB3_ptr		416	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MACB_ADDR_MSB2_ptr		417	// these are 0x001C23174ACB
#define MACB_ADDR_MSB1_ptr		418	// loaded into mac_0 = 0x17231C00  (reverse order)
#define MACB_ADDR_MSB0_ptr		419  //    and into mac_1 = 0x0000CB4A
#define MACB_ADDR_LSB1_ptr		420
#define MACB_ADDR_LSB0_ptr		421

#define MAC10A_ADDR_MSB3_ptr	422	// MAC MAC ADDR: eg 11-22-33-44-55-66
#define MAC10A_ADDR_MSB2_ptr	423	// these are 0x112233445566
#define MAC10A_ADDR_MSB1_ptr	424	// loaded into mac_0 = 0x33445566
#define MAC10A_ADDR_MSB0_ptr	425  //    and into mac_1 = 0x00001122
#define MAC10A_ADDR_LSB1_ptr	426
#define MAC10A_ADDR_LSB0_ptr	427

#define MAC10B_ADDR_MSB3_ptr	428	// MAC MAC ADDR: eg 11-22-33-44-55-66
#define MAC10B_ADDR_MSB2_ptr	429	// these are 0x112233445566
#define MAC10B_ADDR_MSB1_ptr	430	// loaded into mac_0 = 0x33445566
#define MAC10B_ADDR_MSB0_ptr	431  //    and into mac_1 = 0x00001122
#define MAC10B_ADDR_LSB1_ptr	432
#define MAC10B_ADDR_LSB0_ptr	433
//*****************************************************************************

/****************************************************************/
/***   NON-Volatile Data stored in Flash as FlashParameters   ***/
/****************************************************************/
#define FlashParameterSize			250 //really the block/sector is 65536 (0-FFFF)
//hard-coded into gigle.h - and change upon flash of elf
//#define PARA_HW_VERSION_ptr	0	// rev1 to rev255
//#define PARA_FW_VERSION1_ptr	1	// ver1.ver0 0 -> 65535,  65535/1000 = v0.0 to v65.535
//#define PARA_FW_VERSION0_ptr	2	// v1.01 -> 101

#define PARA_MACA_MSB3_ptr		0	// MAC-A MAC ADDR: eg 00-1C-23-17-4A-CB
#define PARA_MACA_MSB2_ptr		1	// these are 0x001C23174ACB
#define PARA_MACA_MSB1_ptr		2	// loaded into mac_0 = 0x17231C00  (reverse order)
#define PARA_MACA_MSB0_ptr		3  //    and into mac_1 = 0x0000CB4A
#define junk0					4
#define junk1					5
#define PARA_MACA_LSB1_ptr		6
#define PARA_MACA_LSB0_ptr		7

#define PARA_MACB_MSB3_ptr		8	// MAC-B MAC ADDR: eg 00-1C-23-17-4A-CB
#define PARA_MACB_MSB2_ptr		9	// these are 0x001C23174ACB
#define PARA_MACB_MSB1_ptr		10	// loaded into mac_0 = 0x17231C00  (reverse order)
#define PARA_MACB_MSB0_ptr		11  //    and into mac_1 = 0x0000CB4A
#define junk2					12
#define junk3					13
#define PARA_MACB_LSB1_ptr		14
#define PARA_MACB_LSB0_ptr		15

#define PARA_MAC10A_MSB3_ptr	16	// 10G MAC-A MAC ADDR: eg 11-22-33-44-55-66
#define PARA_MAC10A_MSB2_ptr	17	// these are 0x112233445566
#define PARA_MAC10A_MSB1_ptr	18	// loaded into mac_0 = 0x33445566
#define PARA_MAC10A_MSB0_ptr	19  //    and into mac_1 = 0x00001122
#define junk4					20
#define junk5					21
#define PARA_MAC10A_LSB1_ptr	22
#define PARA_MAC10A_LSB0_ptr	23

#define PARA_MAC10B_MSB3_ptr	24
#define PARA_MAC10B_MSB2_ptr	25
#define PARA_MAC10B_MSB1_ptr	26
#define PARA_MAC10B_MSB0_ptr	27
#define junk6					28
#define junk7					29
#define PARA_MAC10B_LSB1_ptr	30
#define PARA_MAC10B_LSB0_ptr	31

// SW configurable and can be changed from GUI
#define PARA_OPTIONS_INSTALLED_ptr		32	// currently undefined
#define PARA_OPTIONS_ENABLED_ptr		33
#define PARA_LENGTH						34


//****************************************************************
//**** Configuration Bytes from GUI to GIGE Module *****
//****************************************************************
#define MaxConfig 			75		// standard config
#define MaxConfig2 			445		// expanded TSE+10G Ethernet
//...with our new 4800-byte Multi-Stream Only architecture - this should not change from Config2 we use today
// DOES IT ??? - With the inclusion of general headers + emix and stuff it'll be different - >150
#define MaxConfig_ms 		   5750		// further extended for TrafficEngine/MultiStreaming support
#define single_stream_offset   101
//#define ConfigChecksum_ptr	MaxConfig-1 // NOT USED, gigE doesn't look at this!
//#define ConfigChecksum2_ptr	MaxConfig2-1

#define GIGEconfig_ptr 	0
#define ID_ptr			1		   // Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
#define GIGEMODE_ptr	2		// ^0:Copper/Fiber 0=copper,1=fiber, //not used:^1&2=speed:00=10,01=100,10=1G,11=10G
#define SYSCONFIG_ptr  	3

#define OPTIONS_INSTALLED_ptr	4		// tell FW what HW we have available (saved in Flash)
#define OPTIONS_ENABLED_ptr		5			// tell FW if some of the Installed HW Options are Enabled (saved in Flash)
//=================== MAC CONFIG/CONTROL SETTINGS ======
#define MAC_CONFIG3_ptr  	6
#define MAC_CONFIG2_ptr    	7
#define MAC_CONFIG1_ptr   	8
#define MAC_CONFIG0_ptr   	9
//unsigned int MAC_COMMAND_CONFIG = BytesToLong(RxBuffer,MAC_CONFIG3_ptr) // MAC_COMMAND_CONFIG=0x02000013	// 10M
/*

		#define ETH_SPEED_1G	10		(MACA_COMMAND_CONFIG & MAC_COMMAND_SPEED_1G)
		#define ENA_10	11				(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_ENA_10_MSK)
		#define HD_ENA	12			(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_HD_ENA_MSK)
		#define LOOP_ENA	13			(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_LOOPBACK_MSK)
		#define ETH_SPEED_1GB	14
		#define ENA_10B	15			(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_ENA_10_MSK)
		#define HD_ENAB	16			(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_HD_ENA_MSK)
		#define LOOP_ENAB	17		(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_LOOPBACK_MSK)
*/


#define ETH_SPEED_1G	(MACA_COMMAND_CONFIG & MAC_COMMAND_SPEED_1G)
#define ENA_10			(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_ENA_10_MSK)
#define HD_ENA			(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_HD_ENA_MSK)
#define LOOP_ENA		(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_LOOPBACK_MSK)
#define ETH_SPEED_1GB	(MACB_COMMAND_CONFIG & MAC_COMMAND_SPEED_1G)
#define ENA_10B			(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_ENA_10_MSK)
#define HD_ENAB			(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_HD_ENA_MSK)
#define LOOP_ENAB		(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_LOOPBACK_MSK)

		#define MAC_COMMAND_SPEED_MASK	18			// bit.25:ENA_10:1=10M,0=100M  AND  bit.3:ETH_SPEED_1G:1=1G,0=10/100
		#define MAC_COMMAND_SPEED_10M	19
		#define MAC_COMMAND_SPEED_100M	20
		#define MAC_COMMAND_SPEED_1G	21
/////////////////////////////////////////////////////////////////////////////////////////////////
#define MAC_FRAME_LEN1_ptr	10		// ...also used by EGEN (pkt_length) EGEN_PKT_LENTYPE1_ptr
#define MAC_FRAME_LEN0_ptr	11		// The maximum Frame Length the TSE uses to chk rcvd frames

#define MAC_PAUSEQUANT1_ptr	12		// FLOW CONTROL - PAUSE DURATION
#define MAC_PAUSEQUANT0_ptr	13

#define MAC_IPG_LENGTH_ptr	14		// Inter-Packet/Frame Gap size (in bytes)  VALID:8 to 26, 12 is default as typ min IPG
#define MAC_MISC_ptr	15			// Misc control flags RESET_MAC, OMIT_CRC, TX_SHIFT16, RX_SHIFT16, ...

#define MAC_ADDR_LSB11_ptr	16			// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MAC_ADDR_LSB10_ptr	17			// these are 0x001C23174ACB
#define MAC_ADDR_MSB03_ptr	18			// loaded into mac_0 = 0x17231C00  (reverse order)
#define MAC_ADDR_MSB02_ptr	19			//    and into mac_1 = 0x0000CB4A
#define MAC_ADDR_MSB01_ptr	20
#define MAC_ADDR_MSB00_ptr	21

#define MAC_MULTICASTHASH_MSB3_ptr	22	// Multicast Hash table MSB (each bit goes into bit.0 of the respective Hash register)
#define MAC_MULTICASTHASH_MSB2_ptr	23
#define MAC_MULTICASTHASH_MSB1_ptr	24
#define MAC_MULTICASTHASH_MSB0_ptr	25

#define MAC_MULTICASTHASH_LSB3_ptr	26	// Multicast Hash table LSB (each bit goes into bit.0 of the respective Hash register)
#define MAC_MULTICASTHASH_LSB2_ptr	27
#define MAC_MULTICASTHASH_LSB1_ptr	28
#define MAC_MULTICASTHASH_LSB0_ptr	29

#define MAC_SMAC0_MSB3_ptr	30				 	// Supplementary MAC ADDR  MSB
#define MAC_SMAC0_MSB2_ptr	31				 	//	...used for filtering
#define MAC_SMAC0_MSB1_ptr	32
#define MAC_SMAC0_MSB0_ptr	33
#define MAC_SMAC0_LSB3_ptr	34					//Supplementary MAC ADDR 0 LSB
#define MAC_SMAC0_LSB2_ptr	35
#define MAC_SMAC0_LSB1_ptr	36
#define MAC_SMAC0_LSB0_ptr	37

#define MAC_SMAC1_MSB3_ptr	38					//Supplementary MAC ADDR 1 MSB
#define MAC_SMAC1_MSB2_ptr	39
#define MAC_SMAC1_MSB1_ptr	40
#define MAC_SMAC1_MSB0_ptr	41
#define MAC_SMAC1_LSB3_ptr	42					 //Supplementary MAC ADDR 1 LSB
#define MAC_SMAC1_LSB2_ptr	43
#define MAC_SMAC1_LSB1_ptr	44
#define MAC_SMAC1_LSB0_ptr	45

#define MAC_SMAC2_MSB3_ptr	46		 			 //Supplementary MAC ADDR 2 MSB
#define MAC_SMAC2_MSB2_ptr	47
#define MAC_SMAC2_MSB1_ptr	48
#define MAC_SMAC2_MSB0_ptr	49
#define MAC_SMAC2_LSB3_ptr	50					//Supplementary MAC ADDR 2 LSB
#define MAC_SMAC2_LSB2_ptr	51
#define MAC_SMAC2_LSB1_ptr	52
#define MAC_SMAC2_LSB0_ptr	53

#define MAC_SMAC3_MSB3_ptr	54					//Supplementary MAC ADDR 3 MSB
#define MAC_SMAC3_MSB2_ptr	55
#define MAC_SMAC3_MSB1_ptr	56
#define MAC_SMAC3_MSB0_ptr	57
#define MAC_SMAC3_LSB3_ptr	58					//Supplementary MAC ADDR 3 LSB
#define MAC_SMAC3_LSB2_ptr	59
#define MAC_SMAC3_LSB1_ptr	60
#define MAC_SMAC3_LSB0_ptr	61
//=================== PHY CONFIG/CONTROL SETTINGS ======
#define PHY_CONFIG1_ptr		62			// 16-bits: RESET, MODE, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG,
#define PHY_CONFIG0_ptr 	63		//			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
#define PHY_SFP_MISC_ptr	64		// ^0:1=SFP_A Transmit_OFF, ^1:1=SFP_B Transmit_OFF, ^2:1=SFP+A Transmit_OFF

#define CORE_SIGROUTE_MODES_ptr 	65
//=================== MISC SETTINGS ======
#define CLEAR_ptr     		66		// ^0=restart ET, ^1=reset MAC-A stat's, ^2=reset MAC-B stat's
#define MISC_ptr     		67		// DumpSFPAinfo^0, DumpSFPBinfo^1, SFP+A^2, SFP+B^3 	// ^4:0=Looped/1=End-to-End test, ^5=spare, ^6=TSE_B_SWAP_ADDR, ^7=spare
#define BERT_ptr			68
#define TEST_OPTIONS_PTR 	69		 // Test options to select between (quick test,multistream, y.1564, rfc..) also used for Y.1564  service configuration/service performance/both)
#define TRAFFIC_NUM_OF_STREAMS 	70		// lower nibble is how many streams:
/*********************************************/
#define EGEN_NUM_SECS3_ptr 	71	// num of secs to generate Packets !  ?? hours
#define EGEN_NUM_SECS2_ptr 	72
#define EGEN_NUM_SECS1_ptr 	73	// num of secs to generate Packets !  18+ hours
#define EGEN_NUM_SECS0_ptr 	74		// Rcvd from GUI and just saved for GUI (for GUI recovery)!

#define TRAFFIC_SPARE_ptr_0 	75
#define TRAFFIC_SPARE_ptr_1		76
#define TRAFFIC_SPARE_ptr_2		77
#define TRAFFIC_SPARE_ptr_3		78
#define TRAFFIC_SPARE_ptr_4		79
#define TRAFFIC_SPARE_ptr_5		80
#define TRAFFIC_SPARE_ptr_6		81
#define TRAFFIC_SPARE_ptr_7		82
#define TRAFFIC_SPARE_ptr_8		83
#define TRAFFIC_SPARE_ptr_9		84
#define TRAFFIC_SPARE_ptr_10	85
#define TRAFFIC_SPARE_ptr_11	86
#define TRAFFIC_SPARE_ptr_12	87
#define TRAFFIC_SPARE_ptr_13	88
#define TRAFFIC_SPARE_ptr_14	89
#define TRAFFIC_SPARE_ptr_15	90
#define TRAFFIC_SPARE_ptr_16	91
#define TRAFFIC_SPARE_ptr_17	92
#define TRAFFIC_SPARE_ptr_18	93
#define TRAFFIC_SPARE_ptr_19	94
#define TRAFFIC_SPARE_ptr_20	95
#define TRAFFIC_SPARE_ptr_21	96
#define TRAFFIC_SPARE_ptr_22	97
#define TRAFFIC_SPARE_ptr_23	98
#define TRAFFIC_SPARE_ptr_24	99
#define TRAFFIC_SPARE_ptr_25	100

// Single stream block
//------------------------------------------------------------------
// generic pointer bytes = (0-100) 101 bytes
// single_stream_offset = lengthOf(generic pointer bytes) + 1
//______________________________________________________________________
// gconfig[TRAFFIC_MON_STREAM_CNTL3_ptr + single_stream_offset] [gconfig(0 + 101)]
// gconfig[TRAFFIC_MON_STREAM_CNTL2_ptr + single_stream_offset]	[gconfig(1 + 101)]
//							.
//							.
//							.
// gconfig[TRAFFIC_MSK31_0_ptr + single_stream_offset] [gconfig(340 + offset)]
//______________________________________________________________________
// startstop_ptr (size_offset +1)
// checksum1_ptr (size_offset +1)
// checksum2_ptr (size_offset +1)
//----------------------------------------------------------------------
// *The size_offset being 340 (The number of bytes required for each stream = 0-340)

//#define single_stream_offset   101
//----------------Single Stream ---------------------------------

#define TRAFFIC_MON_STREAM_CNTL3_ptr	single_stream_offset + 0		  // Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
#define TRAFFIC_MON_STREAM_CNTL2_ptr	single_stream_offset + 1		// ^0:Copper/Fiber 0=copper,1=fiber, //not used:^1&2=speed:00=10,01=100,10=1G,11=10G
#define TRAFFIC_MON_STREAM_CNTL1_ptr	single_stream_offset + 2
#define TRAFFIC_MON_STREAM_CNTL0_ptr	single_stream_offset + 3

#define TRAFFIC_RATE_TYPE		single_stream_offset + 4			// MON stream control (for independent capture)
#define TRAFFIC_RATE3_ptr       single_stream_offset + 5
#define TRAFFIC_RATE2_ptr     	single_stream_offset + 6
#define TRAFFIC_RATE1_ptr     	single_stream_offset + 7
#define TRAFFIC_RATE0_ptr     	single_stream_offset + 8				// TBD: e.g. 0=FPS%, 1=Gbps, 2=Mbps, 3=Kbps

#define TRAFFIC_GEN_PKTPERCENT11_ptr	single_stream_offset + 9
#define TRAFFIC_GEN_PKTPERCENT10_ptr	single_stream_offset + 10		  // Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%
#define TRAFFIC_GEN_PKTPERCENT03_ptr	single_stream_offset + 11		  // ie Gbps   0.0001-100.0G,  includes 99.9999 or 9.9999G or 0.9999G
#define TRAFFIC_GEN_PKTPERCENT02_ptr	single_stream_offset + 12		  //  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M
#define TRAFFIC_GEN_PKTPERCENT01_ptr	single_stream_offset + 13		 // 2-byte spares for 100G
#define TRAFFIC_GEN_PKTPERCENT00_ptr	single_stream_offset + 14
//_______________EGEN_PKT_TYPE_ptr____________________
// 0000,0000:  ^10:Ethernet, ^32:IP, ^54:TCP/UDP, ^76:FUTURE
// 10: 00=std Layer2-Ethernet, 01=Ethernet II, 10=802.3 LLC, 11=802.3SNAP
// 32: 00=none, 01=IPv4, 10=IPv6
// 54: 00=none, 01=TCP, 10=UDP
// 76: future
//_________________EGEN_PKT_TYPE_ptr_1____________________
// 0000,0000:  ^7654:MPLS, ^3210:VLAN 	(0=none, N=# of VLAN's or MPLS tags)
//_________________EGEN_PKT_TYPE_ptr_2____________________
//	(8-bits OAM/802.3ag/Y.1731)
#define EGEN_PKT_TYPE_ptr 		single_stream_offset + 15
#define EGEN_PKT_TYPE_ptr_1 	single_stream_offset + 16
#define EGEN_PKT_TYPE_ptr_2 	single_stream_offset + 17
//==================================================================================
/*.....for
TRAFFIC_GEN_CONFIG
TRAFFIC_GEN_CONFIG1
TRAFFIC_GEN_CONFIG2
... in hardware_def.h copy the bit definitions & mask locations so the UI can set these 32-bit words
    as necessary per the FRAME CONFIGURED!
....the UI will be taking over all TRAFFIC_GEN & MON RELATED configurations associated with the following;
all the headers - including dest & src MAC's
and the associated frame type settings like how long the hdr is, where the IPv4 checksum is, where the payload starts, etc. etc.
...all these elements are within these 3 GEN config bytes (note there are some MON config ones as well..*/
#define TRAFFIC_GEN_CONFIG23_ptr	single_stream_offset + 18
#define TRAFFIC_GEN_CONFIG22_ptr	single_stream_offset + 19
#define TRAFFIC_GEN_CONFIG21_ptr	single_stream_offset + 20
#define TRAFFIC_GEN_CONFIG20_ptr	single_stream_offset + 21
#define TRAFFIC_GEN_CONFIG13_ptr	single_stream_offset + 22		// TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed
#define TRAFFIC_GEN_CONFIG12_ptr	single_stream_offset + 23
#define TRAFFIC_GEN_CONFIG11_ptr	single_stream_offset + 24
#define TRAFFIC_GEN_CONFIG10_ptr	single_stream_offset + 25
#define TRAFFIC_GEN_CONFIG03_ptr	single_stream_offset + 26			//TRAFFIC_GEN_CONFIG1
#define TRAFFIC_GEN_CONFIG02_ptr	single_stream_offset + 27
#define TRAFFIC_GEN_CONFIG01_ptr	single_stream_offset + 28		//pkt length
#define TRAFFIC_GEN_CONFIG00_ptr	single_stream_offset + 29		//pkt length

#define SPARE_GEN_CONFIG03_ptr	single_stream_offset + 30
#define SPARE_GEN_CONFIG02_ptr	single_stream_offset + 31
#define SPARE_GEN_CONFIG01_ptr	single_stream_offset + 32
#define SPARE_GEN_CONFIG00_ptr	single_stream_offset + 33
// packet length will be changed by the generator if it is too small to fit required
		//   header plus sequence_number/stream/timestamp.
		//   for ipv4, the header checksum must also be changed.
		// packet length is one value in a Y.1564 EMIX table for random length mode (see RNDUSR and RNDSEQ below)
		//=================================================================================
		//#define TRAFFIC_GEN_CONFIG	0x200	//0x80 Config Fields for GENERATOR
		// 	Structure:     	bits
		// CONFIG_PKLEN		0:15		16-bit packet length 64-9600: 0x40-0x2580 (or mask in RND len modes (e.g. 0xff - clamps size to 255)
		// CONFIG_HDRLEN  	16:23		8-bit header length (header can be 0 to 255 bytes) (note MAC, IPv4, IPv6 overwritten with correct length by GEN)
		// CONFIG_RNDLEN  	24		 	1-bit Random Length packets mode: 1=enable Random mode, 0=fixed length frames
		// CONFIG_RNDPAY		25			1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
		// CONFIG_FRCERR		26			1-bit Force Payload data error (field size bits) DEBUGGING ONLY
		// CONFIG_CONTIN   	27		      1-bit Continuous Generate (ignore PACKET_NUM)
		// CONFIG_BWRANK   	28:31		4-bits bandwidth rank: for streams set priority to higher BW above those with lower BW. 0=top, 15=lowest
		//==================================================================================
					#define LEN_MASK				 //TRAFFIC_GEN_CONFIG3
					#define HDRLEN_MASK
					#define RNDLEN_ON
					#define RNDPAY_ON
					#define FRCERR_ON									//0xFFFF
					#define CONTINUOUS_ON 								//0xFF0000		//62	3E
					#define BWRANK_MASK 					   			//0x01000000
					#define LENOFS_MASK					   			//0x02000000
					#define JUMBO_MASK					   			//0x04000000
					#define HCSOFF_MASK							//0x08000000
					#define ILEN_MASK							//0xF0000000
					#define IHCS_MASK								//0x007F	// Payload 1st byte location (0 is 1st byte of HDR0)

					#define STREAM_CNTL_ENABLE_MASK								//0x0080
					#define STREAM_CNTL_SPARE1_MASK								//0x7F00
					#define STREAM_CNTL_HDRLEN_MASK								//0x0000FFFF	// ILEN = payload length - 64, (payload length=pkt-len-18)
					#define STREAM_CNTL_SPARE2_MASK								//0xFFFF0000
//_______________________________________________________________________________________
//		a	  b		 c		 d		  e		  f		  g		        h		          u         invalid and
//	     64	128		256		512		1024	1280	usr_g	MTU(pktlen)	user_u     terminates_seq
//	      0      1          2          3           4           5          6               7                 8             9-F
//_______________________________________________________________________________________
#define EGEN_PKT_EMIXUSERSIZE3_ptr	single_stream_offset + 34
#define EGEN_PKT_EMIXUSERSIZE2_ptr	single_stream_offset + 35					//		0x00000001		// 1-bit:  Stream Enable bit
#define EGEN_PKT_EMIXUSERSIZE1_ptr	single_stream_offset + 36					//		0x000000FE		// 7-bit: spare - not used
#define EGEN_PKT_EMIXUSERSIZE0_ptr	single_stream_offset + 37					//		0x0003FF00		//10-bit: Stream header length

#define EGEN_EMIXSEQ33_ptr  	single_stream_offset + 38
#define EGEN_EMIXSEQ32_ptr  	single_stream_offset + 39
#define EGEN_EMIXSEQ31_ptr  	single_stream_offset + 40
#define EGEN_EMIXSEQ30_ptr  	single_stream_offset + 41
#define EGEN_EMIXSEQ23_ptr		single_stream_offset + 42			//LSNibble sent last
#define EGEN_EMIXSEQ22_ptr  	single_stream_offset + 43
#define EGEN_EMIXSEQ21_ptr  	single_stream_offset + 44
#define EGEN_EMIXSEQ20_ptr 		single_stream_offset + 45
#define EGEN_EMIXSEQ13_ptr		single_stream_offset + 46
#define EGEN_EMIXSEQ12_ptr  	single_stream_offset + 47
#define EGEN_EMIXSEQ11_ptr		single_stream_offset + 48
#define EGEN_EMIXSEQ10_ptr		single_stream_offset + 49
#define EGEN_EMIXSEQ03_ptr 		single_stream_offset + 50					//(good to set 1,2&3 def as 0xF0000000)
#define EGEN_EMIXSEQ02_ptr 		single_stream_offset + 51
#define EGEN_EMIXSEQ01_ptr		single_stream_offset + 52
#define EGEN_EMIXSEQ00_ptr  	single_stream_offset + 53
/***************************************************************************************************************************
// Config bytes related to Y.1564 & RFC2544 User selectable Test thresholds
/***************************************************************************************************************************
#define TEST_OPTIONS_PTR 			XX // Test options to select between (quick test,multistream, y.1564, rfc..) - In the main Config is used to set the different type of test- service configuration/service performance/both)
...//...CIR & EIR are 4-bytes (they are rates - like TRAFFICRATE_TYPE above)
//RFC2544 will share/overlap these thresholds to allow threshold support for RFC2544 as well*/
#define Y1564_CIR_3_ptr	single_stream_offset + 54			// MSNibble sent 1st   def for Random=0x0123456F
#define Y1564_CIR_2_ptr	single_stream_offset + 55			//Committed information rate
#define Y1564_CIR_1_ptr	single_stream_offset + 56
#define Y1564_CIR_0_ptr	single_stream_offset + 57

#define Y1564_EIR_3_ptr	single_stream_offset + 58				 //Expected information rate
#define Y1564_EIR_2_ptr	single_stream_offset + 59
#define Y1564_EIR_1_ptr	single_stream_offset + 60
#define Y1564_EIR_0_ptr	single_stream_offset + 61

#define Y1564_CIR_STEPS_ptr	single_stream_offset + 62				//Steps for CIR – first 4 bits

#define Y1564_MAX_FTD_3_ptr 	single_stream_offset + 63					//Expected information rate
#define Y1564_MAX_FTD_2_ptr 	single_stream_offset + 64					//Expected information rate
#define Y1564_MAX_FTD_1_ptr 	single_stream_offset + 65					//Expected information rate
#define Y1564_MAX_FTD_0_ptr 	single_stream_offset + 66					//Expected information rate

#define Y1564_MAX_FDV_3_ptr 	single_stream_offset + 67
#define Y1564_MAX_FDV_2_ptr 	single_stream_offset + 68			//4 byte Float
#define Y1564_MAX_FDV_1_ptr 	single_stream_offset + 69
#define Y1564_MAX_FDV_0_ptr 	single_stream_offset + 70

#define Y1564_MAX_FRAME_LOSS_3_ptr 	single_stream_offset + 71
#define Y1564_MAX_FRAME_LOSS_2_ptr 	single_stream_offset + 72			//4 byte Float
#define Y1564_MAX_FRAME_LOSS_1_ptr 	single_stream_offset + 73
#define Y1564_MAX_FRAME_LOSS_0_ptr 	single_stream_offset + 74

#define Y1564_MAX_AVAIL_3_ptr 	single_stream_offset + 75
#define Y1564_MAX_AVAIL_2_ptr 	single_stream_offset + 76		//4 byte Float
#define Y1564_MAX_AVAIL_1_ptr 	single_stream_offset + 77
#define Y1564_MAX_AVAIL_0_ptr 	single_stream_offset + 78

#define SPARE_THRESHOLD_03	single_stream_offset + 79
#define SPARE_THRESHOLD_02	single_stream_offset + 80			//4 byte Float
#define SPARE_THRESHOLD_01	single_stream_offset + 81
#define SPARE_THRESHOLD_00	single_stream_offset + 82

#define TRAFFIC_GEN_SPARE_ptr	single_stream_offset + 83
#define TRAFFIC_HDR_DEF_ptr		single_stream_offset + 84	 // additional instructional info for what is contained in HDR ptr's

#define TRAFFIC_HDR0_3_ptr	single_stream_offset + 85
#define TRAFFIC_HDR0_2_ptr	single_stream_offset + 86
#define TRAFFIC_HDR0_1_ptr	single_stream_offset + 87
#define TRAFFIC_HDR0_0_ptr	single_stream_offset + 88
#define TRAFFIC_HDR1_3_ptr	single_stream_offset + 89				// additional instructional info for what is contained in HDR ptr's
#define TRAFFIC_HDR1_2_ptr	single_stream_offset + 90
#define TRAFFIC_HDR1_1_ptr	single_stream_offset + 91
#define TRAFFIC_HDR1_0_ptr	single_stream_offset + 92
#define TRAFFIC_HDR2_3_ptr	single_stream_offset + 93
#define TRAFFIC_HDR2_2_ptr	single_stream_offset + 94
#define TRAFFIC_HDR2_1_ptr	single_stream_offset + 95
#define TRAFFIC_HDR2_0_ptr	single_stream_offset + 96
#define TRAFFIC_HDR3_3_ptr	single_stream_offset + 97
#define TRAFFIC_HDR3_2_ptr	single_stream_offset + 98
#define TRAFFIC_HDR3_1_ptr	single_stream_offset + 99
#define TRAFFIC_HDR3_0_ptr	single_stream_offset + 100
#define TRAFFIC_HDR4_3_ptr	single_stream_offset + 101
#define TRAFFIC_HDR4_2_ptr	single_stream_offset + 102
#define TRAFFIC_HDR4_1_ptr	single_stream_offset + 103
#define TRAFFIC_HDR4_0_ptr	single_stream_offset + 104
#define TRAFFIC_HDR5_3_ptr	single_stream_offset + 105
#define TRAFFIC_HDR5_2_ptr	single_stream_offset + 106
#define TRAFFIC_HDR5_1_ptr	single_stream_offset + 107
#define TRAFFIC_HDR5_0_ptr	single_stream_offset + 108
#define TRAFFIC_HDR6_3_ptr	single_stream_offset + 109
#define TRAFFIC_HDR6_2_ptr	single_stream_offset + 110
#define TRAFFIC_HDR6_1_ptr	single_stream_offset + 111
#define TRAFFIC_HDR6_0_ptr	single_stream_offset + 112
#define TRAFFIC_HDR7_3_ptr	single_stream_offset + 113
#define TRAFFIC_HDR7_2_ptr	single_stream_offset + 114
#define TRAFFIC_HDR7_1_ptr	single_stream_offset + 115
#define TRAFFIC_HDR7_0_ptr	single_stream_offset + 116
#define TRAFFIC_HDR8_3_ptr	single_stream_offset + 117
#define TRAFFIC_HDR8_2_ptr	single_stream_offset + 118
#define TRAFFIC_HDR8_1_ptr	single_stream_offset + 119
#define TRAFFIC_HDR8_0_ptr	single_stream_offset + 120
#define TRAFFIC_HDR9_3_ptr	single_stream_offset + 121
#define TRAFFIC_HDR9_2_ptr	single_stream_offset + 122
#define TRAFFIC_HDR9_1_ptr	single_stream_offset + 123
#define TRAFFIC_HDR9_0_ptr	single_stream_offset + 124
#define TRAFFIC_HDR10_3_ptr	single_stream_offset + 125
#define TRAFFIC_HDR10_2_ptr	single_stream_offset + 126
#define TRAFFIC_HDR10_1_ptr	single_stream_offset + 127
#define TRAFFIC_HDR10_0_ptr	single_stream_offset + 128
#define TRAFFIC_HDR11_3_ptr	single_stream_offset + 129
#define TRAFFIC_HDR11_2_ptr	single_stream_offset + 130
#define TRAFFIC_HDR11_1_ptr	single_stream_offset + 131
#define TRAFFIC_HDR11_0_ptr	single_stream_offset + 132
#define TRAFFIC_HDR12_3_ptr	single_stream_offset + 133
#define TRAFFIC_HDR12_2_ptr	single_stream_offset + 134
#define TRAFFIC_HDR12_1_ptr	single_stream_offset + 135
#define TRAFFIC_HDR12_0_ptr	single_stream_offset + 136
#define TRAFFIC_HDR13_3_ptr	single_stream_offset + 137
#define TRAFFIC_HDR13_2_ptr	single_stream_offset + 138
#define TRAFFIC_HDR13_1_ptr	single_stream_offset + 139
#define TRAFFIC_HDR13_0_ptr	single_stream_offset + 140
#define TRAFFIC_HDR14_3_ptr	single_stream_offset + 141
#define TRAFFIC_HDR14_2_ptr	single_stream_offset + 142
#define TRAFFIC_HDR14_1_ptr	single_stream_offset + 143
#define TRAFFIC_HDR14_0_ptr	single_stream_offset + 144
#define TRAFFIC_HDR15_3_ptr	single_stream_offset + 145
#define TRAFFIC_HDR15_2_ptr	single_stream_offset + 146
#define TRAFFIC_HDR15_1_ptr	single_stream_offset + 147
#define TRAFFIC_HDR15_0_ptr	single_stream_offset + 148
#define TRAFFIC_HDR16_3_ptr	single_stream_offset + 149
#define TRAFFIC_HDR16_2_ptr	single_stream_offset + 150
#define TRAFFIC_HDR16_1_ptr	single_stream_offset + 151
#define TRAFFIC_HDR16_0_ptr	single_stream_offset + 152
#define TRAFFIC_HDR17_3_ptr	single_stream_offset + 153
#define TRAFFIC_HDR17_2_ptr	single_stream_offset + 154
#define TRAFFIC_HDR17_1_ptr	single_stream_offset + 155
#define TRAFFIC_HDR17_0_ptr	single_stream_offset + 156
#define TRAFFIC_HDR18_3_ptr	single_stream_offset + 157
#define TRAFFIC_HDR18_2_ptr	single_stream_offset + 158
#define TRAFFIC_HDR18_1_ptr	single_stream_offset + 159
#define TRAFFIC_HDR18_0_ptr	single_stream_offset + 160
#define TRAFFIC_HDR19_3_ptr	single_stream_offset + 161
#define TRAFFIC_HDR19_2_ptr	single_stream_offset + 162
#define TRAFFIC_HDR19_1_ptr	single_stream_offset + 163
#define TRAFFIC_HDR19_0_ptr	single_stream_offset + 164
#define TRAFFIC_HDR20_3_ptr	single_stream_offset + 165
#define TRAFFIC_HDR20_2_ptr	single_stream_offset + 166
#define TRAFFIC_HDR20_1_ptr	single_stream_offset + 167
#define TRAFFIC_HDR20_0_ptr	single_stream_offset + 168
#define TRAFFIC_HDR21_3_ptr	single_stream_offset + 169
#define TRAFFIC_HDR21_2_ptr	single_stream_offset + 170
#define TRAFFIC_HDR21_1_ptr	single_stream_offset + 171
#define TRAFFIC_HDR21_0_ptr	single_stream_offset + 172
#define TRAFFIC_HDR22_3_ptr	single_stream_offset + 173
#define TRAFFIC_HDR22_2_ptr	single_stream_offset + 174
#define TRAFFIC_HDR22_1_ptr	single_stream_offset + 175
#define TRAFFIC_HDR22_0_ptr	single_stream_offset + 176
#define TRAFFIC_HDR23_3_ptr	single_stream_offset + 177
#define TRAFFIC_HDR23_2_ptr	single_stream_offset + 178
#define TRAFFIC_HDR23_1_ptr	single_stream_offset + 179
#define TRAFFIC_HDR23_0_ptr	single_stream_offset + 180
#define TRAFFIC_HDR24_3_ptr	single_stream_offset + 181
#define TRAFFIC_HDR24_2_ptr	single_stream_offset + 182
#define TRAFFIC_HDR24_1_ptr	single_stream_offset + 183
#define TRAFFIC_HDR24_0_ptr	single_stream_offset + 184
#define TRAFFIC_HDR25_3_ptr	single_stream_offset + 185
#define TRAFFIC_HDR25_2_ptr	single_stream_offset + 186
#define TRAFFIC_HDR25_1_ptr	single_stream_offset + 187
#define TRAFFIC_HDR25_0_ptr	single_stream_offset + 188
#define TRAFFIC_HDR26_3_ptr	single_stream_offset + 189
#define TRAFFIC_HDR26_2_ptr	single_stream_offset + 190
#define TRAFFIC_HDR26_1_ptr	single_stream_offset + 191
#define TRAFFIC_HDR26_0_ptr	single_stream_offset + 192
#define TRAFFIC_HDR27_3_ptr	single_stream_offset + 193
#define TRAFFIC_HDR27_2_ptr	single_stream_offset + 194
#define TRAFFIC_HDR27_1_ptr	single_stream_offset + 195
#define TRAFFIC_HDR27_0_ptr	single_stream_offset + 196
#define TRAFFIC_HDR28_3_ptr	single_stream_offset + 197
#define TRAFFIC_HDR28_2_ptr	single_stream_offset + 198
#define TRAFFIC_HDR28_1_ptr	single_stream_offset + 199
#define TRAFFIC_HDR28_0_ptr	single_stream_offset + 200
#define TRAFFIC_HDR29_3_ptr	single_stream_offset + 201
#define TRAFFIC_HDR29_2_ptr	single_stream_offset + 202
#define TRAFFIC_HDR29_1_ptr	single_stream_offset + 203
#define TRAFFIC_HDR29_0_ptr	single_stream_offset + 204
#define TRAFFIC_HDR30_3_ptr	single_stream_offset + 205
#define TRAFFIC_HDR30_2_ptr	single_stream_offset + 206
#define TRAFFIC_HDR30_1_ptr	single_stream_offset + 207
#define TRAFFIC_HDR30_0_ptr	single_stream_offset + 208
#define TRAFFIC_HDR31_3_ptr	single_stream_offset + 209
#define TRAFFIC_HDR31_2_ptr	single_stream_offset + 210
#define TRAFFIC_HDR31_1_ptr	single_stream_offset + 211
#define TRAFFIC_HDR31_0_ptr	single_stream_offset + 212

#define TRAFFIC_MSK0_3_ptr	single_stream_offset + 213
#define TRAFFIC_MSK0_2_ptr	single_stream_offset + 214
#define TRAFFIC_MSK0_1_ptr	single_stream_offset + 215
#define TRAFFIC_MSK0_0_ptr	single_stream_offset + 216
#define TRAFFIC_MSK1_3_ptr	single_stream_offset + 217
#define TRAFFIC_MSK1_2_ptr	single_stream_offset + 218
#define TRAFFIC_MSK1_1_ptr	single_stream_offset + 219
#define TRAFFIC_MSK1_0_ptr	single_stream_offset + 220
#define TRAFFIC_MSK2_3_ptr	single_stream_offset + 221
#define TRAFFIC_MSK2_2_ptr	single_stream_offset + 222
#define TRAFFIC_MSK2_1_ptr	single_stream_offset + 223
#define TRAFFIC_MSK2_0_ptr	single_stream_offset + 224
#define TRAFFIC_MSK3_3_ptr	single_stream_offset + 225
#define TRAFFIC_MSK3_2_ptr	single_stream_offset + 226
#define TRAFFIC_MSK3_1_ptr	single_stream_offset + 227
#define TRAFFIC_MSK3_0_ptr	single_stream_offset + 228
#define TRAFFIC_MSK4_3_ptr	single_stream_offset + 229
#define TRAFFIC_MSK4_2_ptr	single_stream_offset + 230
#define TRAFFIC_MSK4_1_ptr	single_stream_offset + 231
#define TRAFFIC_MSK4_0_ptr	single_stream_offset + 232
#define TRAFFIC_MSK5_3_ptr	single_stream_offset + 233
#define TRAFFIC_MSK5_2_ptr	single_stream_offset + 234
#define TRAFFIC_MSK5_1_ptr	single_stream_offset + 235
#define TRAFFIC_MSK5_0_ptr	single_stream_offset + 236
#define TRAFFIC_MSK6_3_ptr	single_stream_offset + 237
#define TRAFFIC_MSK6_2_ptr	single_stream_offset + 238
#define TRAFFIC_MSK6_1_ptr	single_stream_offset + 239
#define TRAFFIC_MSK6_0_ptr	single_stream_offset + 240
#define TRAFFIC_MSK7_3_ptr	single_stream_offset + 241
#define TRAFFIC_MSK7_2_ptr	single_stream_offset + 242
#define TRAFFIC_MSK7_1_ptr	single_stream_offset + 243
#define TRAFFIC_MSK7_0_ptr	single_stream_offset + 244
#define TRAFFIC_MSK8_3_ptr	single_stream_offset + 245
#define TRAFFIC_MSK8_2_ptr	single_stream_offset + 246
#define TRAFFIC_MSK8_1_ptr	single_stream_offset + 247
#define TRAFFIC_MSK8_0_ptr	single_stream_offset + 248
#define TRAFFIC_MSK9_3_ptr	single_stream_offset + 249
#define TRAFFIC_MSK9_2_ptr	single_stream_offset + 250
#define TRAFFIC_MSK9_1_ptr	single_stream_offset + 251
#define TRAFFIC_MSK9_0_ptr	single_stream_offset + 252
#define TRAFFIC_MSK10_3_ptr	single_stream_offset + 253
#define TRAFFIC_MSK10_2_ptr	single_stream_offset + 254
#define TRAFFIC_MSK10_1_ptr	single_stream_offset + 255
#define TRAFFIC_MSK10_0_ptr	single_stream_offset + 256
#define TRAFFIC_MSK11_3_ptr	single_stream_offset + 257
#define TRAFFIC_MSK11_2_ptr	single_stream_offset + 258
#define TRAFFIC_MSK11_1_ptr	single_stream_offset + 259
#define TRAFFIC_MSK11_0_ptr	single_stream_offset + 260
#define TRAFFIC_MSK12_3_ptr	single_stream_offset + 261
#define TRAFFIC_MSK12_2_ptr	single_stream_offset + 262
#define TRAFFIC_MSK12_1_ptr	single_stream_offset + 263
#define TRAFFIC_MSK12_0_ptr	single_stream_offset + 264
#define TRAFFIC_MSK13_3_ptr	single_stream_offset + 265
#define TRAFFIC_MSK13_2_ptr	single_stream_offset + 266
#define TRAFFIC_MSK13_1_ptr	single_stream_offset + 267
#define TRAFFIC_MSK13_0_ptr	single_stream_offset + 268
#define TRAFFIC_MSK14_3_ptr	single_stream_offset + 269
#define TRAFFIC_MSK14_2_ptr	single_stream_offset + 270
#define TRAFFIC_MSK14_1_ptr	single_stream_offset + 271
#define TRAFFIC_MSK14_0_ptr	single_stream_offset + 272
#define TRAFFIC_MSK15_3_ptr	single_stream_offset + 273
#define TRAFFIC_MSK15_2_ptr	single_stream_offset + 274
#define TRAFFIC_MSK15_1_ptr	single_stream_offset + 275
#define TRAFFIC_MSK15_0_ptr	single_stream_offset + 276
#define TRAFFIC_MSK16_3_ptr	single_stream_offset + 277
#define TRAFFIC_MSK16_2_ptr	single_stream_offset + 278
#define TRAFFIC_MSK16_1_ptr	single_stream_offset + 279
#define TRAFFIC_MSK16_0_ptr	single_stream_offset + 280
#define TRAFFIC_MSK17_3_ptr	single_stream_offset + 281
#define TRAFFIC_MSK17_2_ptr	single_stream_offset + 282
#define TRAFFIC_MSK17_1_ptr	single_stream_offset + 283
#define TRAFFIC_MSK17_0_ptr	single_stream_offset + 284
#define TRAFFIC_MSK18_3_ptr	single_stream_offset + 285
#define TRAFFIC_MSK18_2_ptr	single_stream_offset + 286
#define TRAFFIC_MSK18_1_ptr	single_stream_offset + 287
#define TRAFFIC_MSK18_0_ptr	single_stream_offset + 288
#define TRAFFIC_MSK19_3_ptr	single_stream_offset + 289
#define TRAFFIC_MSK19_2_ptr	single_stream_offset + 290
#define TRAFFIC_MSK19_1_ptr	single_stream_offset + 291
#define TRAFFIC_MSK19_0_ptr	single_stream_offset + 292
#define TRAFFIC_MSK20_3_ptr	single_stream_offset + 293
#define TRAFFIC_MSK20_2_ptr	single_stream_offset + 294
#define TRAFFIC_MSK20_1_ptr	single_stream_offset + 295
#define TRAFFIC_MSK20_0_ptr	single_stream_offset + 296
#define TRAFFIC_MSK21_3_ptr	single_stream_offset + 297
#define TRAFFIC_MSK21_2_ptr	single_stream_offset + 298
#define TRAFFIC_MSK21_1_ptr	single_stream_offset + 299
#define TRAFFIC_MSK21_0_ptr	single_stream_offset + 300
#define TRAFFIC_MSK22_3_ptr	single_stream_offset + 301
#define TRAFFIC_MSK22_2_ptr	single_stream_offset + 302
#define TRAFFIC_MSK22_1_ptr	single_stream_offset + 303
#define TRAFFIC_MSK22_0_ptr	single_stream_offset + 304
#define TRAFFIC_MSK23_3_ptr	single_stream_offset + 305
#define TRAFFIC_MSK23_2_ptr	single_stream_offset + 306
#define TRAFFIC_MSK23_1_ptr	single_stream_offset + 307
#define TRAFFIC_MSK23_0_ptr	single_stream_offset + 308
#define TRAFFIC_MSK24_3_ptr	single_stream_offset + 309
#define TRAFFIC_MSK24_2_ptr	single_stream_offset + 310
#define TRAFFIC_MSK24_1_ptr	single_stream_offset + 311
#define TRAFFIC_MSK24_0_ptr	single_stream_offset + 312
#define TRAFFIC_MSK25_3_ptr	single_stream_offset + 313
#define TRAFFIC_MSK25_2_ptr	single_stream_offset + 314
#define TRAFFIC_MSK25_1_ptr	single_stream_offset + 315
#define TRAFFIC_MSK25_0_ptr	single_stream_offset + 316
#define TRAFFIC_MSK26_3_ptr	single_stream_offset + 317
#define TRAFFIC_MSK26_2_ptr	single_stream_offset + 318
#define TRAFFIC_MSK26_1_ptr	single_stream_offset + 319
#define TRAFFIC_MSK26_0_ptr	single_stream_offset + 320
#define TRAFFIC_MSK27_3_ptr	single_stream_offset + 321
#define TRAFFIC_MSK27_2_ptr	single_stream_offset + 322
#define TRAFFIC_MSK27_1_ptr	single_stream_offset + 323
#define TRAFFIC_MSK27_0_ptr	single_stream_offset + 324
#define TRAFFIC_MSK28_3_ptr	single_stream_offset + 325
#define TRAFFIC_MSK28_2_ptr	single_stream_offset + 326
#define TRAFFIC_MSK28_1_ptr	single_stream_offset + 327
#define TRAFFIC_MSK28_0_ptr	single_stream_offset + 328
#define TRAFFIC_MSK29_3_ptr	single_stream_offset + 329
#define TRAFFIC_MSK29_2_ptr	single_stream_offset + 330
#define TRAFFIC_MSK29_1_ptr	single_stream_offset + 331
#define TRAFFIC_MSK29_0_ptr	single_stream_offset + 332
#define TRAFFIC_MSK30_3_ptr	single_stream_offset + 333
#define TRAFFIC_MSK30_2_ptr	single_stream_offset + 334
#define TRAFFIC_MSK30_1_ptr	single_stream_offset + 335
#define TRAFFIC_MSK30_0_ptr	single_stream_offset + 336
#define TRAFFIC_MSK31_3_ptr	single_stream_offset + 337
#define TRAFFIC_MSK31_2_ptr	single_stream_offset + 338
#define TRAFFIC_MSK31_1_ptr	single_stream_offset + 339
#define TRAFFIC_MSK31_0_ptr	single_stream_offset + 340


#define STARTSTOP_ptr		442				// have this at end so all setup crap is executed 1st!
#define ConfigChecksum_spare			(MaxConfig2-1)	// currently 150-1=149
#define ConfigChecksum2				(MaxConfig2-1)


//*** END of GIGE Config data *******
/****************************************************************/
/********** STATUS Bytes from GIGE Module to GUI ****************/
/****************************************************************/
#define MaxStatus 			MaxConfig+125
#define StatusChecksum_ptr	MaxStatus-1

#define MaxStatus2 			MaxConfig2+300
#define StatusChecksum2_ptr	MaxStatus2-1

#define GIGEstatus_ptr  	MaxConfig2+0 	// LOW_BAT^8
#define GIGETYPE_ptr		MaxConfig2+1	// type 0: Basic DS1 test set
#define GIGEVER1_ptr		MaxConfig2+2	// ver1.ver0  e.g.  0.0 to 255.255
#define GIGEVER0_ptr		MaxConfig2+3	// current FW revision
#define GIGERESULT_ptr		MaxConfig2+4	// GIGE HW-FW status/debug FAULT CODES in var: GIGE_STATUS

#define SECONDS3_ptr		MaxConfig2+5	// TEST SECONDS (ELAP)
#define SECONDS2_ptr  		MaxConfig2+6	//*
#define SECONDS1_ptr  		MaxConfig2+7
#define SECONDS0_ptr  		MaxConfig2+8

#define MISC_STAT_ptr		MaxConfig2+9	// Bleep^0,Error^1,History^2,test_running^3,LOCAL_CHG^4,
#define SFP_STAT_ptr		MaxConfig2+10	// SFP changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
				#define SFP_STATE_MASK		0xF9		// mask off or clear SFP state flags
				//#define SFP10G_STATE_MASK	0xE7		// mask off or clear SFP+ state flags
				#define SFP10G_STATE_MASK	0xF7		// mask off A-only or clear SFP+ state flags

//===== PHY STATUS ============================================
#define PHY_OPERATING_MODE1_ptr			MaxConfig2+11
#define PHY_OPERATING_MODE0_ptr			MaxConfig2+12	//!!UNUSED!!
#define PHYB_OPERATING_MODE1_ptr		MaxConfig2+13
#define PHYB_OPERATING_MODE0_ptr		MaxConfig2+14	//!!UNUSED!!
#define PHY10GA_OPERATING_MODE1_ptr		MaxConfig2+15	//10G port-A state
#define PHY10GA_OPERATING_MODE0_ptr		MaxConfig2+16	//!!UNUSED!!

#define PHY_OPT_POWER1_ptr			MaxConfig2+17
#define PHY_OPT_POWER0_ptr			MaxConfig2+18
#define PHYB_OPT_POWER1_ptr			MaxConfig2+19
#define PHYB_OPT_POWER0_ptr			MaxConfig2+20
#define PHY10G_OPT_POWER1_ptr		MaxConfig2+21	// 10G-A Power
#define PHY10G_OPT_POWER0_ptr		MaxConfig2+22
#define PHY10GB_OPT_POWER1_ptr		MaxConfig2+23	// 10G-B Power //!!UNUSED!!
#define PHY10GB_OPT_POWER0_ptr		MaxConfig2+24	//!!UNUSED!!

#define PHY_CABLE_DIAG1_ptr			MaxConfig2+25	//!!UNUSED!!
#define PHY_CABLE_DIAG0_ptr			MaxConfig2+26	//!!UNUSED!!

#define PHY_MII_STAT1_ptr			MaxConfig2+27
#define PHY_MII_STAT0_ptr			MaxConfig2+28	//!!UNUSED!!
#define PHYB_MII_STAT1_ptr			MaxConfig2+29	//* TSE only
#define PHYB_MII_STAT0_ptr			MaxConfig2+30	//!!UNUSED!!

//MISC PHY STAT bit encoded here..........
// PHY_EXT_STATUS (11h) ^13:MDI Crossover state ==> PHY10G_MISC^0:0=normal,1=crossed
#define PHY_MISC1_ptr				MaxConfig2+31
#define PHY_MISC0_ptr				MaxConfig2+32	//start with just a byte
#define PHYB_MISC1_ptr				MaxConfig2+33
#define PHYB_MISC0_ptr				MaxConfig2+34	//start with just a byte
//MISC 10G PHY A/B status/info
#define MISC_ptr					MaxConfig2+35	// need for 10G A/B MISC?
#define MISC_ptr					MaxConfig2+36
#define MISC_ptr					MaxConfig2+37
#define MISC_ptr					MaxConfig2+38

//===== MAC STATUS GENERAL ======================================================

#define MAC_SNAP_TX_FRAME_CNT03_ptr		MaxConfig2+39	//* 64-bits: 1=Hi32,0=Lo32
#define MAC_SNAP_TX_FRAME_CNT02_ptr		MaxConfig2+40
#define MAC_SNAP_TX_FRAME_CNT01_ptr		MaxConfig2+41
#define MAC_SNAP_TX_FRAME_CNT00_ptr		MaxConfig2+42

#define MAC_SNAP_RX_FRAME_CNT03_ptr		MaxConfig2+43
#define MAC_SNAP_RX_FRAME_CNT02_ptr		MaxConfig2+44
#define MAC_SNAP_RX_FRAME_CNT01_ptr		MaxConfig2+45
#define MAC_SNAP_RX_FRAME_CNT00_ptr		MaxConfig2+46

#define MAC_COLLISION_CNT3_ptr			MaxConfig2+47
#define MAC_COLLISION_CNT2_ptr			MaxConfig2+48
#define MAC_COLLISION_CNT1_ptr			MaxConfig2+49
#define MAC_COLLISION_CNT0_ptr			MaxConfig2+50

#define MAC_VLAN_STAT3_ptr				MaxConfig2+51
#define MAC_VLAN_STAT2_ptr				MaxConfig2+52
#define MAC_VLAN_STAT1_ptr				MaxConfig2+53	//Has the ability to detect a VLAN packet
#define MAC_VLAN_STAT0_ptr				MaxConfig2+54

//===== MAC STATUS COUNTERS =====================================================
#define MAC_FRAME_TX_OK13_ptr			MaxConfig2+55	//*
#define MAC_FRAME_TX_OK12_ptr			MaxConfig2+56
#define MAC_FRAME_TX_OK11_ptr			MaxConfig2+57
#define MAC_FRAME_TX_OK10_ptr			MaxConfig2+58
#define MAC_FRAME_TX_OK03_ptr			MaxConfig2+59	//*
#define MAC_FRAME_TX_OK02_ptr			MaxConfig2+60
#define MAC_FRAME_TX_OK01_ptr			MaxConfig2+61
#define MAC_FRAME_TX_OK00_ptr			MaxConfig2+62

#define MAC_FRAME_RX_OK13_ptr			MaxConfig2+63	//*
#define MAC_FRAME_RX_OK12_ptr			MaxConfig2+64
#define MAC_FRAME_RX_OK11_ptr			MaxConfig2+65
#define MAC_FRAME_RX_OK10_ptr			MaxConfig2+66
#define MAC_FRAME_RX_OK03_ptr			MaxConfig2+67	//*
#define MAC_FRAME_RX_OK02_ptr			MaxConfig2+68
#define MAC_FRAME_RX_OK01_ptr			MaxConfig2+69
#define MAC_FRAME_RX_OK00_ptr			MaxConfig2+70

#define MAC_FRAME_FCS_ERRS11_ptr			MaxConfig2+71 // extra
#define MAC_FRAME_FCS_ERRS10_ptr			MaxConfig2+72 // extra
#define MAC_FRAME_FCS_ERRS03_ptr			MaxConfig2+73	//*
#define MAC_FRAME_FCS_ERRS02_ptr			MaxConfig2+74
#define MAC_FRAME_FCS_ERRS01_ptr			MaxConfig2+75
#define MAC_FRAME_FCS_ERRS00_ptr			MaxConfig2+76

#define MAC_ALIGNMENT_ERRS11_ptr			MaxConfig2+77// extra
#define MAC_ALIGNMENT_ERRS10_ptr			MaxConfig2+78
#define MAC_ALIGNMENT_ERRS03_ptr			MaxConfig2+79	//*
#define MAC_ALIGNMENT_ERRS02_ptr			MaxConfig2+80
#define MAC_ALIGNMENT_ERRS01_ptr			MaxConfig2+81
#define MAC_ALIGNMENT_ERRS00_ptr			MaxConfig2+82

#define MAC_OCTETS_TX_OK21_ptr			MaxConfig2+83	// extra
#define MAC_OCTETS_TX_OK20_ptr			MaxConfig2+84	// extra
#define MAC_OCTETS_TX_OK13_ptr			MaxConfig2+85	// 64 bits
#define MAC_OCTETS_TX_OK12_ptr			MaxConfig2+86	//*
#define MAC_OCTETS_TX_OK11_ptr			MaxConfig2+87
#define MAC_OCTETS_TX_OK10_ptr			MaxConfig2+88
#define MAC_OCTETS_TX_OK03_ptr			MaxConfig2+89
#define MAC_OCTETS_TX_OK02_ptr			MaxConfig2+90
#define MAC_OCTETS_TX_OK01_ptr			MaxConfig2+91
#define MAC_OCTETS_TX_OK00_ptr			MaxConfig2+92

#define MAC_PAUSE_FRAME_TX11_ptr			MaxConfig2+93 //extraa
#define MAC_PAUSE_FRAME_TX10_ptr			MaxConfig2+94 //extraa
#define MAC_PAUSE_FRAME_TX03_ptr			MaxConfig2+95	//*
#define MAC_PAUSE_FRAME_TX02_ptr			MaxConfig2+96
#define MAC_PAUSE_FRAME_TX01_ptr			MaxConfig2+97
#define MAC_PAUSE_FRAME_TX00_ptr			MaxConfig2+98

#define MAC_PAUSE_FRAME_RX11_ptr			MaxConfig2+99//extra
#define MAC_PAUSE_FRAME_RX10_ptr			MaxConfig2+100
#define MAC_PAUSE_FRAME_RX03_ptr			MaxConfig2+101	//*
#define MAC_PAUSE_FRAME_RX02_ptr			MaxConfig2+102
#define MAC_PAUSE_FRAME_RX01_ptr			MaxConfig2+103
#define MAC_PAUSE_FRAME_RX00_ptr			MaxConfig2+104

#define MAC_ERR_FRAMES_RX11_ptr			MaxConfig2+105 //extra
#define MAC_ERR_FRAMES_RX10_ptr			MaxConfig2+106
#define MAC_ERR_FRAMES_RX03_ptr			MaxConfig2+107	// ifInErrors
#define MAC_ERR_FRAMES_RX02_ptr			MaxConfig2+108	//*
#define MAC_ERR_FRAMES_RX01_ptr			MaxConfig2+109
#define MAC_ERR_FRAMES_RX00_ptr			MaxConfig2+110

#define MAC_ERR_FRAMES_TX11_ptr			MaxConfig2+111 // extra
#define MAC_ERR_FRAMES_TX10_ptr			MaxConfig2+112
#define MAC_ERR_FRAMES_TX03_ptr			MaxConfig2+113	// ifOutErrors
#define MAC_ERR_FRAMES_TX02_ptr			MaxConfig2+114	//*
#define MAC_ERR_FRAMES_TX01_ptr			MaxConfig2+115
#define MAC_ERR_FRAMES_TX00_ptr			MaxConfig2+116

#define MAC_UNICAST_FRAMES_RX13_ptr			MaxConfig2+117	//*
#define MAC_UNICAST_FRAMES_RX12_ptr			MaxConfig2+118
#define MAC_UNICAST_FRAMES_RX11_ptr			MaxConfig2+119
#define MAC_UNICAST_FRAMES_RX10_ptr			MaxConfig2+120
#define MAC_UNICAST_FRAMES_RX03_ptr			MaxConfig2+121	//*
#define MAC_UNICAST_FRAMES_RX02_ptr			MaxConfig2+122
#define MAC_UNICAST_FRAMES_RX01_ptr			MaxConfig2+123 //
#define MAC_UNICAST_FRAMES_RX00_ptr			MaxConfig2+124

#define MAC_MULTICAST_FRAMES_RX13_ptr		MaxConfig2+125	//*
#define MAC_MULTICAST_FRAMES_RX12_ptr		MaxConfig2+126
#define MAC_MULTICAST_FRAMES_RX11_ptr		MaxConfig2+127
#define MAC_MULTICAST_FRAMES_RX10_ptr		MaxConfig2+128
#define MAC_MULTICAST_FRAMES_RX03_ptr		MaxConfig2+129	//*
#define MAC_MULTICAST_FRAMES_RX02_ptr		MaxConfig2+130
#define MAC_MULTICAST_FRAMES_RX01_ptr		MaxConfig2+131 //extra
#define MAC_MULTICAST_FRAMES_RX00_ptr		MaxConfig2+132

#define MAC_BROADCAST_FRAMES_RX13_ptr		MaxConfig2+133	//*
#define MAC_BROADCAST_FRAMES_RX12_ptr		MaxConfig2+134
#define MAC_BROADCAST_FRAMES_RX11_ptr		MaxConfig2+135
#define MAC_BROADCAST_FRAMES_RX10_ptr		MaxConfig2+136
#define MAC_BROADCAST_FRAMES_RX03_ptr		MaxConfig2+137	//*
#define MAC_BROADCAST_FRAMES_RX02_ptr		MaxConfig2+138
#define MAC_BROADCAST_FRAMES_RX01_ptr		MaxConfig2+139 //extra
#define MAC_BROADCAST_FRAMES_RX00_ptr		MaxConfig2+140

#define MAC_UNICAST_FRAMES_TX13_ptr			MaxConfig2+141	//*
#define MAC_UNICAST_FRAMES_TX12_ptr			MaxConfig2+142
#define MAC_UNICAST_FRAMES_TX11_ptr			MaxConfig2+143
#define MAC_UNICAST_FRAMES_TX10_ptr			MaxConfig2+144
#define MAC_UNICAST_FRAMES_TX03_ptr			MaxConfig2+145	//*
#define MAC_UNICAST_FRAMES_TX02_ptr			MaxConfig2+146
#define MAC_UNICAST_FRAMES_TX01_ptr			MaxConfig2+147 //extra
#define MAC_UNICAST_FRAMES_TX00_ptr			MaxConfig2+148

#define MAC_MULTICAST_FRAMES_TX13_ptr		MaxConfig2+149	//*
#define MAC_MULTICAST_FRAMES_TX12_ptr		MaxConfig2+150
#define MAC_MULTICAST_FRAMES_TX11_ptr		MaxConfig2+151
#define MAC_MULTICAST_FRAMES_TX10_ptr		MaxConfig2+152
#define MAC_MULTICAST_FRAMES_TX03_ptr		MaxConfig2+153	//*
#define MAC_MULTICAST_FRAMES_TX02_ptr		MaxConfig2+154
#define MAC_MULTICAST_FRAMES_TX01_ptr		MaxConfig2+155 //extra
#define MAC_MULTICAST_FRAMES_TX00_ptr		MaxConfig2+156

#define MAC_BROADCAST_FRAMES_TX13_ptr		MaxConfig2+157	//*
#define MAC_BROADCAST_FRAMES_TX12_ptr		MaxConfig2+158
#define MAC_BROADCAST_FRAMES_TX11_ptr		MaxConfig2+159
#define MAC_BROADCAST_FRAMES_TX10_ptr		MaxConfig2+160
#define MAC_BROADCAST_FRAMES_TX03_ptr		MaxConfig2+161	//*
#define MAC_BROADCAST_FRAMES_TX02_ptr		MaxConfig2+162
#define MAC_BROADCAST_FRAMES_TX01_ptr		MaxConfig2+163
#define MAC_BROADCAST_FRAMES_TX00_ptr		MaxConfig2+164

#define MAC_ETHER_STATS_DROP11_ptr			MaxConfig2+165	//*extra
#define MAC_ETHER_STATS_DROP10_ptr			MaxConfig2+166
#define MAC_ETHER_STATS_DROP03_ptr			MaxConfig2+167	// The number of frames that are dropped due to MAC internal errors
#define MAC_ETHER_STATS_DROP02_ptr			MaxConfig2+168	// ...when FIFO buffer overflow persists
#define MAC_ETHER_STATS_DROP01_ptr			MaxConfig2+169	//*
#define MAC_ETHER_STATS_DROP00_ptr			MaxConfig2+170

#define MAC_ETHER_STATS_OCTETS21_ptr		MaxConfig2+171 //extra
#define MAC_ETHER_STATS_OCTETS20_ptr		MaxConfig2+172
#define MAC_ETHER_STATS_OCTETS13_ptr		MaxConfig2+173	// 64-bits
#define MAC_ETHER_STATS_OCTETS12_ptr		MaxConfig2+174	// total octets rcvd: good & bad
#define MAC_ETHER_STATS_OCTETS11_ptr		MaxConfig2+175	//*
#define MAC_ETHER_STATS_OCTETS10_ptr		MaxConfig2+176
#define MAC_ETHER_STATS_OCTETS03_ptr		MaxConfig2+177
#define MAC_ETHER_STATS_OCTETS02_ptr		MaxConfig2+178
#define MAC_ETHER_STATS_OCTETS01_ptr		MaxConfig2+179
#define MAC_ETHER_STATS_OCTETS00_ptr		MaxConfig2+180

#define MAC_ETHER_STATS_PACKETS11_ptr		MaxConfig2+181 //extra
#define MAC_ETHER_STATS_PACKETS10_ptr		MaxConfig2+182
#define MAC_ETHER_STATS_PACKETS03_ptr		MaxConfig2+183	// total packets (frames) rcvd: good & bad
#define MAC_ETHER_STATS_PACKETS02_ptr		MaxConfig2+184	//*
#define MAC_ETHER_STATS_PACKETS01_ptr		MaxConfig2+185
#define MAC_ETHER_STATS_PACKETS00_ptr		MaxConfig2+186

//===== all packet(frame) counters include both good & bad (errored) frames ===========
#define MAC_UNDERSIZED_PACKETS_RX11_ptr		MaxConfig2+187 // extra
#define MAC_UNDERSIZED_PACKETS_RX10_ptr		MaxConfig2+188
#define MAC_UNDERSIZED_PACKETS_RX03_ptr		MaxConfig2+189	// Undersized Frames are <64-bytes
#define MAC_UNDERSIZED_PACKETS_RX02_ptr		MaxConfig2+190	//*
#define MAC_UNDERSIZED_PACKETS_RX01_ptr		MaxConfig2+191
#define MAC_UNDERSIZED_PACKETS_RX00_ptr		MaxConfig2+192

#define MAC_OVERSIZED_PACKETS_RX11_ptr		MaxConfig2+193	//*
#define MAC_OVERSIZED_PACKETS_RX10_ptr		MaxConfig2+194
#define MAC_OVERSIZED_PACKETS_RX03_ptr		MaxConfig2+195	// Oversized Frames are > value in "frm_length" register
#define MAC_OVERSIZED_PACKETS_RX02_ptr		MaxConfig2+196	// (vars.h) MAC_FRM_LENGTH
#define MAC_OVERSIZED_PACKETS_RX01_ptr		MaxConfig2+197	//*
#define MAC_OVERSIZED_PACKETS_RX00_ptr		MaxConfig2+198

#define MAC_64_PACKETS_RX11_ptr				MaxConfig2+199 //extra
#define MAC_64_PACKETS_RX10_ptr				MaxConfig2+200
#define MAC_64_PACKETS_RX03_ptr				MaxConfig2+201	//*
#define MAC_64_PACKETS_RX02_ptr				MaxConfig2+202
#define MAC_64_PACKETS_RX01_ptr				MaxConfig2+203
#define MAC_64_PACKETS_RX00_ptr				MaxConfig2+204

#define MAC_65_127_PACKETS_RX11_ptr			MaxConfig2+205 // extra
#define MAC_65_127_PACKETS_RX10_ptr			MaxConfig2+206
#define MAC_65_127_PACKETS_RX03_ptr			MaxConfig2+207	//*
#define MAC_65_127_PACKETS_RX02_ptr			MaxConfig2+208
#define MAC_65_127_PACKETS_RX01_ptr			MaxConfig2+209
#define MAC_65_127_PACKETS_RX00_ptr			MaxConfig2+210

#define MAC_128_255_PACKETS_RX11_ptr			MaxConfig2+211 //extra
#define MAC_128_255_PACKETS_RX10_ptr			MaxConfig2+212
#define MAC_128_255_PACKETS_RX03_ptr			MaxConfig2+213	//*
#define MAC_128_255_PACKETS_RX02_ptr			MaxConfig2+214
#define MAC_128_255_PACKETS_RX01_ptr			MaxConfig2+215
#define MAC_128_255_PACKETS_RX00_ptr			MaxConfig2+216

#define MAC_256_511_PACKETS_RX11_ptr			MaxConfig2+217 //extra
#define MAC_256_511_PACKETS_RX10_ptr			MaxConfig2+218
#define MAC_256_511_PACKETS_RX03_ptr			MaxConfig2+219	//*
#define MAC_256_511_PACKETS_RX02_ptr			MaxConfig2+220
#define MAC_256_511_PACKETS_RX01_ptr			MaxConfig2+221
#define MAC_256_511_PACKETS_RX00_ptr			MaxConfig2+222

#define MAC_512_1023_PACKETS_RX11_ptr		MaxConfig2+223 //extra
#define MAC_512_1023_PACKETS_RX10_ptr		MaxConfig2+224
#define MAC_512_1023_PACKETS_RX03_ptr		MaxConfig2+225	//*
#define MAC_512_1023_PACKETS_RX02_ptr		MaxConfig2+226
#define MAC_512_1023_PACKETS_RX01_ptr		MaxConfig2+227
#define MAC_512_1023_PACKETS_RX00_ptr		MaxConfig2+228

#define MAC_1024_1518_PACKETS_RX11_ptr		MaxConfig2+229 //extra
#define MAC_1024_1518_PACKETS_RX10_ptr		MaxConfig2+230
#define MAC_1024_1518_PACKETS_RX03_ptr		MaxConfig2+231	//*
#define MAC_1024_1518_PACKETS_RX02_ptr		MaxConfig2+232
#define MAC_1024_1518_PACKETS_RX01_ptr		MaxConfig2+233
#define MAC_1024_1518_PACKETS_RX00_ptr		MaxConfig2+234

#define MAC_1519_FLEN_PACKETS_RX11_ptr		MaxConfig2+235	//*extra
#define MAC_1519_FLEN_PACKETS_RX10_ptr		MaxConfig2+236
#define MAC_1519_FLEN_PACKETS_RX03_ptr		MaxConfig2+237	// frames 1519 to max frame length defined in FRM_LENGTH
#define MAC_1519_FLEN_PACKETS_RX02_ptr		MaxConfig2+238	// (MAC_FRM_LENGTH)
#define MAC_1519_FLEN_PACKETS_RX01_ptr		MaxConfig2+239	//*
#define MAC_1519_FLEN_PACKETS_RX00_ptr		MaxConfig2+240

#define MAC_JABBERS_RX11_ptr					MaxConfig2+241 //extra
#define MAC_JABBERS_RX10_ptr					MaxConfig2+242
#define MAC_JABBERS_RX03_ptr					MaxConfig2+243	// Too Long frames with CRC error
#define MAC_JABBERS_RX02_ptr					MaxConfig2+244	//*
#define MAC_JABBERS_RX01_ptr					MaxConfig2+245
#define MAC_JABBERS_RX00_ptr					MaxConfig2+246

#define MAC_FRAGMENTS_RX11_ptr				MaxConfig2+247 //extra
#define MAC_FRAGMENTS_RX10_ptr				MaxConfig2+248
#define MAC_FRAGMENTS_RX03_ptr				MaxConfig2+249	// Too Short frames with CRC error
#define MAC_FRAGMENTS_RX02_ptr				MaxConfig2+250	//*
#define MAC_FRAGMENTS_RX01_ptr				MaxConfig2+251
#define MAC_FRAGMENTS_RX00_ptr				MaxConfig2+252

#define MAC_OCTETS_RX_OK21_ptr			MaxConfig2+253 //extra
#define MAC_OCTETS_RX_OK20_ptr			MaxConfig2+254
#define MAC_OCTETS_RX_OK13_ptr			MaxConfig2+255	// 64 bits
#define MAC_OCTETS_RX_OK12_ptr			MaxConfig2+256	// this includes OVH+IPG (6+(default)IPG of 12 = 18)
#define MAC_OCTETS_RX_OK11_ptr			MaxConfig2+257	// so for 1000 packets would be 1000*(Frm_len) + 1000*(OVH_IPG)
#define MAC_OCTETS_RX_OK10_ptr			MaxConfig2+258	// this is a means to determine IPG of incoming unknown packets!
#define MAC_OCTETS_RX_OK03_ptr			MaxConfig2+259	//*
#define MAC_OCTETS_RX_OK02_ptr			MaxConfig2+260
#define MAC_OCTETS_RX_OK01_ptr			MaxConfig2+261
#define MAC_OCTETS_RX_OK00_ptr			MaxConfig2+262


//===== TRAFFIC STATUS ====================================================================
#define TRAFFIC_GEN_RUNNING_ptr			MaxConfig2+263 //
#define TRAFFIC_GEN_STREAM_KEY_ptr		MaxConfig2+264 // //0x02 (0=1st stream, 1=2nd stream, etc.)
#define TRAFFIC_GEN_START_ptr 			MaxConfig2+265// //0x03 1-bit, 1=START generator!
#define TRAFFIC_GEN_STOP_ptr			MaxConfig2+266// //0x04 1-bit, 1=STOP generator!

#define TRAFFIC_GEN_ERROR3_ptr			MaxConfig2+267
#define TRAFFIC_GEN_ERROR2_ptr			MaxConfig2+268
#define TRAFFIC_GEN_ERROR1_ptr			MaxConfig2+269
#define TRAFFIC_GEN_ERROR0_ptr			MaxConfig2+270

#define TRAFFIC_GEN_VERSION3_ptr		MaxConfig2+271
#define TRAFFIC_GEN_VERSION2_ptr		MaxConfig2+272
#define TRAFFIC_GEN_VERSION1_ptr		MaxConfig2+273
#define TRAFFIC_GEN_VERSION0_ptr		MaxConfig2+274

//number of frames config'd by Duration for GEN & MON total (stream_idx=0) or by stream (FW calc'd frames)
#define TRAFFIC_NUM_STREAM_PACKET11_ptr		MaxConfig2+275 //extra
#define TRAFFIC_NUM_STREAM_PACKET10_ptr		MaxConfig2+276 //TRAFFIC_MON_NUM_PACKET1_ptr  - upper 8 bits
#define TRAFFIC_NUM_STREAM_PACKET03_ptr	 	MaxConfig2+277 //lower 32 bits
#define TRAFFIC_NUM_STREAM_PACKET02_ptr		MaxConfig2+278
#define TRAFFIC_NUM_STREAM_PACKET01_ptr		MaxConfig2+279
#define TRAFFIC_NUM_STREAM_PACKET00_ptr		MaxConfig2+280

// total number of frames actually sent (by reading GEN)
#define EGEN_TX_PKT_SENT_CNT11_ptr		MaxConfig2+281	//*  TAKES THE VALUE OF TRAFFIC_GEN_PKT_TX_CNT
#define EGEN_TX_PKT_SENT_CNT10_ptr		MaxConfig2+282
#define EGEN_TX_PKT_SENT_CNT03_ptr		MaxConfig2+283
#define EGEN_TX_PKT_SENT_CNT02_ptr		MaxConfig2+284
#define EGEN_TX_PKT_SENT_CNT01_ptr		MaxConfig2+285
#define EGEN_TX_PKT_SENT_CNT00_ptr		MaxConfig2+286

// number of frames actually rcvd by reading MON
#define TRAFFIC_MON_NUM_RCVD_PACKET11_ptr		MaxConfig2+287 //extra
#define TRAFFIC_MON_NUM_RCVD_PACKET10_ptr		MaxConfig2+288 //TRAFFIC_MON_NUM_PACKET1_ptr  - upper 8 bits
#define TRAFFIC_MON_NUM_RCVD_PACKET03_ptr	 	MaxConfig2+289 //lower 32 bits
#define TRAFFIC_MON_NUM_RCVD_PACKET02_ptr		MaxConfig2+290
#define TRAFFIC_MON_NUM_RCVD_PACKET01_ptr		MaxConfig2+291
#define TRAFFIC_MON_NUM_RCVD_PACKET00_ptr		MaxConfig2+292

#define TRAFFIC_MON_RX_CTRL_STAT03_ptr			MaxConfig2+293	// receive_ctrl_status
#define TRAFFIC_MON_RX_CTRL_STAT02_ptr			MaxConfig2+294
#define TRAFFIC_MON_RX_CTRL_STAT01_ptr			MaxConfig2+295
#define TRAFFIC_MON_RX_CTRL_STAT00_ptr			MaxConfig2+296

	#define TRAFFIC_MON_START_BIT
	#define TRAFFIC_MON_RECEIVE_START_AND_CLEAR_CNTRS_BIT
	#define TRAFFIC_MON_RECEIVE_STOP_BIT
	#define TRAFFIC_MON_RECEIVE_CONTROL_DONE_STATUS_BIT
	#define TRAFFIC_MON_RECEIVE_ERROR_STATUS_BIT
	#define TRAFFIC_MON_ACTIVE_BIT
	#define TRAFFIC_MON_RECEIVE_CONTINUOUS_BIT
	#define TRAFFIC_MON_RESET_BIT


#define TRAFFIC_MON_CYCLE_RX_CNT21_ptr			MaxConfig2+297//extra
#define TRAFFIC_MON_CYCLE_RX_CNT20_ptr			MaxConfig2+298//extra
#define TRAFFIC_MON_CYCLE_RX_CNT13_ptr			MaxConfig2+299		//	read_traffic_reg(MON, TRAFFIC_MON_CYCLE_RX_CNT0);
#define TRAFFIC_MON_CYCLE_RX_CNT12_ptr			MaxConfig2+300		// This value is put in locycles and then total duration is calculated and put above- do we need this?
#define TRAFFIC_MON_CYCLE_RX_CNT11_ptr			MaxConfig2+301
#define TRAFFIC_MON_CYCLE_RX_CNT10_ptr			MaxConfig2+302
#define TRAFFIC_MON_CYCLE_RX_CNT03_ptr			MaxConfig2+303		//	read_traffic_reg(MON, TRAFFIC_MON_CYCLE_RX_CNT1); //value put in EMON_RX_BYTE_CNT13_ptr
#define TRAFFIC_MON_CYCLE_RX_CNT02_ptr			MaxConfig2+304
#define TRAFFIC_MON_CYCLE_RX_CNT01_ptr			MaxConfig2+305
#define TRAFFIC_MON_CYCLE_RX_CNT00_ptr			MaxConfig2+306

#define TRAFFIC_MON_PKT_RX_MAC_ERR_CNT3_ptr		MaxConfig2+307				// count of packets with errors detected by the mac and - dbg
#define TRAFFIC_MON_PKT_RX_MAC_ERR_CNT2_ptr		MaxConfig2+308
#define TRAFFIC_MON_PKT_RX_MAC_ERR_CNT1_ptr		MaxConfig2+309
#define TRAFFIC_MON_PKT_RX_MAC_ERR_CNT0_ptr		MaxConfig2+310

#define TRAFFIC_MON_PKT_RX_EOP_ERR13_ptr		MaxConfig2+311				// count of packets without a valid stream id from pkt
#define TRAFFIC_MON_PKT_RX_EOP_ERR12_ptr		MaxConfig2+312
#define TRAFFIC_MON_PKT_RX_EOP_ERR11_ptr		MaxConfig2+313
#define TRAFFIC_MON_PKT_RX_EOP_ERR10_ptr		MaxConfig2+314
#define TRAFFIC_MON_PKT_RX_EOP_ERR03_ptr     	MaxConfig2+315
#define TRAFFIC_MON_PKT_RX_EOP_ERR02_ptr        MaxConfig2+316
#define TRAFFIC_MON_PKT_RX_EOP_ERR01_ptr        MaxConfig2+317
#define TRAFFIC_MON_PKT_RX_EOP_ERR00_ptr        MaxConfig2+318

#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR13_ptr	MaxConfig2+319					// count of packets with a timeout error
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR12_ptr	MaxConfig2+320
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR11_ptr	MaxConfig2+321
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR10_ptr	MaxConfig2+322
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR03_ptr    MaxConfig2+323
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR02_ptr    MaxConfig2+324
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR01_ptr    MaxConfig2+325
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR00_ptr    MaxConfig2+326

#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR13_ptr	MaxConfig2+327				// 0X17 X4: count of packets with no stream header match and not a
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR12_ptr	MaxConfig2+328
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR11_ptr	MaxConfig2+329
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR10_ptr	MaxConfig2+330
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR03_ptr    MaxConfig2+331
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR02_ptr    MaxConfig2+332
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR01_ptr    MaxConfig2+333
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR00_ptr	MaxConfig2+334

#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR13_ptr  MaxConfig2+335
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR12_ptr  MaxConfig2+336
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR11_ptr  MaxConfig2+337
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR10_ptr  MaxConfig2+338
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR03_ptr	 MaxConfig2+339			// count of packets with multiple stream header matches
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR02_ptr	 MaxConfig2+340			// count of packets with multiple stream header matches
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR01_ptr	 MaxConfig2+341		// count of packets with multiple stream header matches
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR00_ptr	 MaxConfig2+342			// count of packets with multiple stream header matches

#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT13_ptr    MaxConfig2+343
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT12_ptr    MaxConfig2+344
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT11_ptr    MaxConfig2+345
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT10_ptr    MaxConfig2+346
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT03_ptr    MaxConfig2+347
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT02_ptr    MaxConfig2+348
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT01_ptr    MaxConfig2+349
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT00_ptr    MaxConfig2+350

#define TRAFFIC_MON_STREAM_KEY3_ptr				MaxConfig2+351	 	// stream key, which stream is the nios accessing
#define TRAFFIC_MON_STREAM_KEY2_ptr				MaxConfig2+352
#define TRAFFIC_MON_STREAM_KEY1_ptr				MaxConfig2+353
#define TRAFFIC_MON_STREAM_KEY0_ptr				MaxConfig2+354

			#define STREAM_KEY_INDEX_MASK					 // STREAM_KEY_INDEX_MASK); 8 bit
			#define STREAM_KEY_NUM_STREAMS_MASK				  //STREAM_KEY_NUM_STREAMS_MASK); - 8 bit
			#define STREAM_KEY_SPARE1					 	//Spare //read_traffic_reg(MON, STREAM_KEY_SPARE) - 16 bit
			#define SPARE_MASK

#define TRAFFIC_MON_BROADCAST_RX21_ptr			MaxConfig2+355 //extra
#define TRAFFIC_MON_BROADCAST_RX20_ptr			MaxConfig2+356
#define TRAFFIC_MON_BROADCAST_RX13_ptr			MaxConfig2+357	// TRAFFIC_MON_BROADCAST_RX_CNT1
#define TRAFFIC_MON_BROADCAST_RX12_ptr			MaxConfig2+358
#define TRAFFIC_MON_BROADCAST_RX11_ptr			MaxConfig2+359
#define TRAFFIC_MON_BROADCAST_RX10_ptr			MaxConfig2+360
#define TRAFFIC_MON_BROADCAST_RX03_ptr			MaxConfig2+361	//TRAFFIC_MON_BROADCAST_RX_CNT
#define TRAFFIC_MON_BROADCAST_RX02_ptr			MaxConfig2+362
#define TRAFFIC_MON_BROADCAST_RX01_ptr			MaxConfig2+363
#define TRAFFIC_MON_BROADCAST_RX00_ptr			MaxConfig2+364

#define TRAFFIC_MON_PKT_RX_MAC_ERR21_ptr		MaxConfig2+365
#define TRAFFIC_MON_PKT_RX_MAC_ERR20_ptr		MaxConfig2+366
#define TRAFFIC_MON_PKT_RX_MAC_ERR13_ptr		MaxConfig2+367	//		 TRAFFIC_MON_PKT_RX_MAC_ERR1
#define TRAFFIC_MON_PKT_RX_MAC_ERR12_ptr		MaxConfig2+368
#define TRAFFIC_MON_PKT_RX_MAC_ERR11_ptr		MaxConfig2+369
#define TRAFFIC_MON_PKT_RX_MAC_ERR10_ptr		MaxConfig2+370
#define TRAFFIC_MON_PKT_RX_MAC_ERR03_ptr		MaxConfig2+371//		TRAFFIC_MON_PKT_RX_MAC_ERR
#define TRAFFIC_MON_PKT_RX_MAC_ERR02_ptr		MaxConfig2+372
#define TRAFFIC_MON_PKT_RX_MAC_ERR01_ptr		MaxConfig2+373
#define TRAFFIC_MON_PKT_RX_MAC_ERR00_ptr		MaxConfig2+374

#define TRAFFIC_MON_DELAY_SUM23_ptr       		MaxConfig2+375		 //     read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM2);//packet delay sum MSB
#define TRAFFIC_MON_DELAY_SUM22_ptr				MaxConfig2+376
#define TRAFFIC_MON_DELAY_SUM21_ptr				MaxConfig2+377
#define TRAFFIC_MON_DELAY_SUM20_ptr				MaxConfig2+378
#define TRAFFIC_MON_DELAY_SUM13_ptr       		MaxConfig2+379		 //     read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM1);//  packet delay sum MSB
#define TRAFFIC_MON_DELAY_SUM12_ptr				MaxConfig2+380
#define TRAFFIC_MON_DELAY_SUM11_ptr				MaxConfig2+381
#define TRAFFIC_MON_DELAY_SUM10_ptr				MaxConfig2+382
#define TRAFFIC_MON_DELAY_SUM03_ptr        		MaxConfig2+383	//    read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM0); ///packet delay sum LSB
#define TRAFFIC_MON_DELAY_SUM02_ptr				MaxConfig2+384
#define TRAFFIC_MON_DELAY_SUM01_ptr				MaxConfig2+385
#define TRAFFIC_MON_DELAY_SUM00_ptr				MaxConfig2+386

#define TEST_STATUS_DELAY_MIN	MaxConfig2+387				 //spare
#define TEST_STATUS_DELAY_MIN3 	MaxConfig2+388	 //Results for threshold comparison
#define TEST_STATUS_DELAY_MIN2 	MaxConfig2+389
#define TEST_STATUS_DELAY_MIN1 	MaxConfig2+390
#define TEST_STATUS_DELAY_MIN0 	MaxConfig2+391

#define TEST_STATUS_DELAY_MAX	MaxConfig2+392	 //spare
#define TEST_STATUS_DELAY_MAX3 	MaxConfig2+393	 //Results for threshold comparison
#define TEST_STATUS_DELAY_MAX2 	MaxConfig2+394
#define TEST_STATUS_DELAY_MAX1 	MaxConfig2+395
#define TEST_STATUS_DELAY_MAX0 	MaxConfig2+396

#define TRAFFIC_MON_RX_BYTE_CNT21_ptr	MaxConfig2+397		//spare
#define TRAFFIC_MON_RX_BYTE_CNT20_ptr	MaxConfig2+398
#define TRAFFIC_MON_RX_BYTE_CNT13_ptr	MaxConfig2+399				//* 64-bits: 1=Hi32,0=Lo32 //TRAFFIC_MON_BYTE_RX_CNT1
#define TRAFFIC_MON_RX_BYTE_CNT12_ptr	MaxConfig2+400
#define TRAFFIC_MON_RX_BYTE_CNT11_ptr	MaxConfig2+401			//previously EMON_RX_BYTE_CNT
#define TRAFFIC_MON_RX_BYTE_CNT10_ptr	MaxConfig2+402
#define TRAFFIC_MON_RX_BYTE_CNT03_ptr	MaxConfig2+403				//TRAFFIC_MON_BYTE_RX_CNT0
#define TRAFFIC_MON_RX_BYTE_CNT02_ptr	MaxConfig2+404
#define TRAFFIC_MON_RX_BYTE_CNT01_ptr	MaxConfig2+405
#define TRAFFIC_MON_RX_BYTE_CNT00_ptr	MaxConfig2+406

#define TRAFFIC_MON_RX_PKT_OK_CNT13_ptr	MaxConfig2+407		//* TRAFFIC_MON_PKT_RX_OK
#define TRAFFIC_MON_RX_PKT_OK_CNT12_ptr	MaxConfig2+408
#define TRAFFIC_MON_RX_PKT_OK_CNT11_ptr	MaxConfig2+409
#define TRAFFIC_MON_RX_PKT_OK_CNT10_ptr	MaxConfig2+410
#define TRAFFIC_MON_RX_PKT_OK_CNT03_ptr	MaxConfig2+411		//* TRAFFIC_MON_PKT_RX_OK
#define TRAFFIC_MON_RX_PKT_OK_CNT02_ptr	MaxConfig2+412
#define TRAFFIC_MON_RX_PKT_OK_CNT01_ptr	MaxConfig2+413
#define TRAFFIC_MON_RX_PKT_OK_CNT00_ptr	MaxConfig2+414

#define TRAFFIC_MON_RX_PKT_ERR_CNT21_ptr	MaxConfig2+415	//extra
#define TRAFFIC_MON_RX_PKT_ERR_CNT20_ptr	MaxConfig2+416
#define TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr	MaxConfig2+417	//*TRAFFIC_MON_PKT_RX_ERR1
#define TRAFFIC_MON_RX_PKT_ERR_CNT12_ptr	MaxConfig2+418
#define TRAFFIC_MON_RX_PKT_ERR_CNT11_ptr	MaxConfig2+419
#define TRAFFIC_MON_RX_PKT_ERR_CNT10_ptr	MaxConfig2+420
#define TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr	MaxConfig2+421	//*TRAFFIC_MON_PKT_RX_ERR
#define TRAFFIC_MON_RX_PKT_ERR_CNT02_ptr	MaxConfig2+422
#define TRAFFIC_MON_RX_PKT_ERR_CNT01_ptr	MaxConfig2+423
#define TRAFFIC_MON_RX_PKT_ERR_CNT00_ptr	MaxConfig2+424

#define SDT_SHORTEST_TIME1_ptr	MaxConfig2+425		//
#define SDT_SHORTEST_TIME0_ptr	MaxConfig2+426

#define SDT_LONGEST_TIME1_ptr	MaxConfig2+427		//
#define SDT_LONGEST_TIME0_ptr	MaxConfig2+428

#define SDT_AVG_TIME1_ptr	MaxConfig2+429			// don't know if this is something the GUI can calc
#define SDT_AVG_TIME0_ptr	MaxConfig2+430			// ...or it must be done real-time

#define SDT_TOTAL_TIME3_ptr	MaxConfig2+431			//
#define SDT_TOTAL_TIME2_ptr	MaxConfig2+432
#define SDT_TOTAL_TIME1_ptr	MaxConfig2+433
#define SDT_TOTAL_TIME0_ptr	MaxConfig2+434

#define SPARE_ptr_0 	MaxConfig2+435
#define SPARE_ptr_1		MaxConfig2+436
#define SPARE_ptr_2		MaxConfig2+437
#define SPARE_ptr_3		MaxConfig2+438
#define SPARE_ptr_4		MaxConfig2+439
#define SPARE_ptr_5		MaxConfig2+440
#define SPARE_ptr_6		MaxConfig2+441
#define SPARE_ptr_7		MaxConfig2+442
#define SPARE_ptr_8		MaxConfig2+443
#define SPARE_ptr_9		MaxConfig2+444
#define SPARE_ptr_10	MaxConfig2+445
#define SPARE_ptr_11	MaxConfig2+446
#define SPARE_ptr_12	MaxConfig2+447
#define SPARE_ptr_13	MaxConfig2+448
#define SPARE_ptr_14	MaxConfig2+449
#define SPARE_ptr_15	MaxConfig2+450
#define SPARE_ptr_16	MaxConfig2+451
#define SPARE_ptr_17	MaxConfig2+452
#define SPARE_ptr_18	MaxConfig2+453
#define SPARE_ptr_19	MaxConfig2+454

//#define MaxStatus2 			MaxConfig2+300
//#define StatusChecksum2_ptr	MaxStatus2-1
#define StatusChecksum2						MaxConfig2+(MaxStatus2-1)	//*
/*******************************************************************/
/*	SPECIFIC BIT definitions   */
/*******************************************************************/
#define LED_ON	0
#define LED_OFF	1


#define gBleep   (gStatus[MISC_STAT_ptr]&0x01)	// ^0 Bleeeeeep....

#define gLowBat  (gStatus[GIGEstatus_ptr]&0x80)	// Low Battery Warning....

#define	gLosCnt	 (BytesToInt(gStatus,LOS_COUNT1_ptr))

#define	gBpvCnt	 (BytesToLong(gStatus,BPV_COUNT3_ptr))

#define ParALARM	0	// Common flag for common Display of ALARM types
#define ParEVENT	6	// Common flag for common Display of EVENT types


//..GIGE-MODE,SPEED.................
//#define mode_copper	0x00	// Mode bit.0
//#define mode_fiber	0x01

#define ModeCopper10M 	0	// new Mode
#define ModeCopper100M  1
#define ModeCopper1G 	2
#define ModeFiber1G 	3
#define ModeFiber10G 	4
#define ModeFiber100G 	5
#define ModeFiber100M 	6

//=== written to PHY_COP_CONFIG1_ptr & PHY_FBR_CONFIG1_ptr =========
#define PHY_CONFIG_ETH_SPEED_MASK		0x0003	// 2-bits
	#define speed_10M	0x00	// PHYx_ETH_SPEED: PHY_COP_CONFIG1_ptr & PHY_FBR_CONFIG1_ptr  bit.0-1
	#define speed_100M	0x01
	#define speed_1G	0x02
	#define speed_10G   0x03
#define PHY_CONFIG_FULLDUPLEX_MASK		0x0004	// 1-bit	1= FDX, 0=HDX
#define PHY_CONFIG_ENABLE_AN_MASK		0x0008	// 1-bit	1=enabled
#define PHY_CONFIG_RESTART_ANEG_MASK	0x0010	// 1-bit	// this can be auto-handled by NIOS
#define PHY_CONFIG_ENABLE_MDIX_MASK		0x0020	// 1-bit
#define PHY_CONFIG_MISC_1G_CNTL_MASK	0x0060	// 2-bits
#define PHY_CONFIG_RESET_MASK			0x0080	// 1-bit
#define PHY_CONFIG_ANEG_ADV_MASK		0x0F00	// 4-bits	adv eth support (10,100,1G,FDX, etc)
#define PHY_CONFIG_LOOPBACK_MASK		0x3000	// 2-bits
#define PHY_CONFIG_PWR_DOWN_MASK		0x4000	// 1-bit

//#define PHYA_ETH_SPEED (PHYA_COPPER_CONFIG & PHY_CONFIG_ETH_SPEED_MASK)
//#define PHYB_ETH_SPEED (PHYB_COPPER_CONFIG & PHY_CONFIG_ETH_SPEED_MASK)

// Packet Length...
#define 	def_pkt_length_fixed	0
#define 	def_pkt_length_random	1

#define ParLos		0
#define	ParOof		1

#define DispCnt		0	// Event & Bert error display options
#define DispBer		1
#define	DispEs		2
#define	DispEfs		3
#define DispPefs	4
#define DispSes		5
#define	DispPses	6
#define DispSec		7
#define	DispET		8



/************ BERT Patterns supported by the GB103 module ****************/
#define bert_off	0
#define bert_qrss	1
#define bert_2047	2
#define bert_223	3
#define bert_220	4
#define bert_215	5
#define bert_3in24	6
#define bert_all1	7
#define bert_all0	8
#define bert_1		9
#define bert_v54	10		/* 2^7-1 QRW */
#define bert_2in8	11		/* 2:8  2-1's in 8-bits */
#define bert_1and7	12		/* 1:7  1-1 and 7-0's   8-bits */
#define bert_rsv	13
#define bert_alt	14
#define bert_63		15
#define bert_511	16
#define bert_TxIdle	17		/* reserved for DS3:Tx IDLE flag from PDA to GB103 */
#define bert_TxAIS	18		/* reserved for DS3:Tx AIS  flag from PDA to GB103 */
#define bert_TxRAI	19		/* reserved for DS3:Tx RAI  flag from PDA to GB103 */
/**** Start of Bridged-Tap patterns ****/
#define bert_btap	20
#define bert_all1_	21		/* all-1's */
#define bert_alt_	22		/* alternating 1,0 */
#define bert_1and3	23		/* 1:3  4-bits */
#define bert_1and5	24
#define bert_1and6	25
#define bert_1and7_	26
#define bert_2and8	27
#define bert_2and9	28
#define bert_2and10	29
#define bert_2and11	30
#define bert_2and12	31
#define bert_2and13	32
#define bert_2and14	33
#define bert_3in18	34
#define bert_3in19	35
#define bert_3in20	36
#define bert_3in21	37
#define bert_3in22	38
#define bert_3in23	39
#define bert_3in24_	40
#define bert_qrss_	41
#define bert_dbug	42	/* 1111,0110  8-bit pattern */
/*** End Bridged-Tap ***/
/*** DS3 Patterns ***/
#define bert_1100	43	/* DS3 1100   4-bit pattern */
#define bert_1000	44	/* DS3 1000   4-bit pattern */
/*********************/
#define bert_auto	45
/*** DS1 Patterns ***/
#define bert_1in16	46	/* DS1 1in16  16-bit pattern */
#define bert_daly	47	/* DS1 55-byte pattern */
#define bert_D5D5	49	/* DS1 Intelligent Repeater Query pattern */
/*** Loopback in progress detection  ***/
#define bert_511V	50	/* 2^9-1 (511) QRW: used for loopback in progress detection */
/*** Reserved for User pattern ***/
#define bert_user	0x7f
/*** Loopback patterns ***/
#define bert_2in5	0x81	/* Loop-Up DS1 NIU */
#define bert_3in5	0x82	/* Loop-Dn DS1 NIU */
#define bert_4in7	0x83	/* Loop HDSL HTU-C toward NW   */
#define bert_2in6	0x84	/* Loop HDSL Rptr1 toward NW   */
#define bert_3in6	0x85	/* Loop HDSL Rptr2 toward NW   */
#define bert_3in7	0x86	/* Loop HDSL HTU-R toward NW   */
#define bert_6in7	0x87	/* Loop HDSL HTU-C toward Cust */
#define bert_4in6	0x88	/* Loop HDSL Rptr1 toward Cust */
#define bert_5in6	0x89	/* Loop HDSL Rptr2 toward Cust */
#define bert_5in7	0x8A	/* Loop HDSL HTU-R toward Cust */
/***********************************************************/
/*****************************************************************/
/*** GIGE-A&B: MODULE DEFINITIONS For DIRTY/Update bit handling **/
/*****************************************************************/
#define RxDirtyPDABit	RxBuffer[GIGEconfig_ptr]&0x80		// Set/Clr and sent by PDA
#define DirtyPDABit		ConfigStat[GIGEconfig_ptr]&0x80		// Returned to PDA by MOD
//#define DirtyPDABit10G	ConfigStat[GIGEconfig_ptr]&0x80		// Returned to PDA by MOD
#define RxDirtyModBit	RxBuffer[GIGEconfig_ptr]&0x40		// Returned to MOD by PDA
#define DirtyModBit		ConfigStat[GIGEconfig_ptr]&0x40		// Set/Clr and sent by MOD
#define RxUpdateFlag	RxBuffer[GIGEconfig_ptr]&0x20		// Set/Clr and sent by PDA
#define UpdateFlag		ConfigStat[GIGEconfig_ptr]&0x20		// Returned to PDA from MOD
#define RxReSyncFlag	(RxBuffer[GIGEconfig_ptr]&0x10)		// Send Re-Sync LINK flag to PDA on PWR cycle/link lost
#define ReSyncFlag		(ConfigStat[GIGEconfig_ptr]&0x10)	// Send Re-Sync LINK flag to PDA on PWR cycle/link lost
#define ReSyncFlagB		(ConfigStatB[GIGEconfig_ptr]&0x10)	// Send Re-Sync LINK flag to PDA on PWR cycle/link lost
#define ReSyncFlag10G	(ConfigStat10G[GIGEconfig_ptr]&0x10)// Send Re-Sync LINK flag to PDA on PWR cycle/link lost
#define ValidConfig		(RxBuffer[GIGEconfig_ptr]&0xC0)

//======= TEST FLAGS/STATES ===============
#define RxTestStartGuiBit	(RxBuffer[STARTSTOP_ptr]&0x10)		// Set/Clr and sent by GUI	(GUI cmd 'START TEST')
																// START: |=0x10  STOP: &=0x10
#define RxTestStartToggle	(RxBuffer[STARTSTOP_ptr]&0x01)		// GUI toggles to MOD to kick consecutive tests

#define TestStartGuiBit		(ConfigStat[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)
#define TestStartGuiBitB	(ConfigStatB[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)
#define TestStartGuiBit10	(ConfigStat10G[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)

#define TestStartModBit		(ConfigStat[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)
#define TestStartModBitB	(ConfigStatB[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)
#define TestStartModBit10	(ConfigStat10G[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)

#define TestStoppedModBit	(ConfigStat[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)
#define TestStoppedModBitB	(ConfigStatB[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)
#define TestStoppedModBit10	(ConfigStat10G[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)

//#define TestStoppedModBit10B (ConfigStat10GB[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)
#define gOPT_INSTALLED_dual1g	((OPTIONS_INSTALLED&0x01)==0)
#define gOPT_INSTALLED_a10g		((OPTIONS_INSTALLED&0x02)==0)
#define gOPT_INSTALLED_b10g		((OPTIONS_INSTALLED&0x04)==0)

#define gOPT_ENABLED_dual1g	((OPTIONS_ENABLED&0x01)==0)
#define gOPT_ENABLED_a10g	((OPTIONS_ENABLED&0x02)==0)
#define gOPT_ENABLED_b10g	((OPTIONS_ENABLED&0x04)==0)

#endif /* CONFSTAT_H_ */

//extra bytes for 100G sized counts  :\

/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
