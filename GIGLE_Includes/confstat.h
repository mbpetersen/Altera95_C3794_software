

/************************************************************************
*		Config-Status element definitions								 *
*				   														 *
* Copyright (c) 2015 Greenlee Communications Vista, CA 92081    *
* All rights reserved.                                                   *
************************************************************************/
#ifndef CONFSTAT_H_
#define CONFSTAT_H_

/**************************************************************
********* Packet ID's by Function/PCB/Module type ************
**************************************************************/

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
#define Packet_MultiStream  0x75
#define Packet_LOOPBACK		0x35

#define Packet_C3794		0x40
/*****************************************************************************
 SpecialStat Status: gSpecialSpare SysInfo Bytes from GIGE Module to GUI **
 SpecialStat: 128bytes [2-129] 	SFP Core
 SpecialStat: 128bytes [130-257] SFP Diag
 SpecialStat: 189bytes [258-448] SPARE in SFP Status array
 SpecialStat:   1 byte [449]     Status array checksum
 SpecialStat:  40bytes [400-439] HW/FW SysInfo
****************************************************************************
// currently save SYS_INFO to SpecialStat[] ONLY (not SpecialStatB[])/
*****************************************************************************/
// first three are stored/hard-coded in gigle.h
#define HW_VERSION_ptr			400	// rev1 to rev255
#define FW_VERSION1_ptr			401	// ver1.ver0 0 -> 65535,  65535/1000 = v0.0 to v65.535
#define FW_VERSION0_ptr			402	// v1.01 -> 101
// these on down are SW configurable....so are stored in and read from Flash
#define FLASH_OPT_INSTALLED_ptr	403 // ^0=dual-1g, ^1=10G-A, ^2=10G-B, (?) THRU,VLAN,IP,OAM
#define FLASH_OPT_ENABLED_ptr	404 // All ACTIVE LOW! FF=nothing installed, FF=nothing enabled

// sernum not used now - just gonna use 0000-FFFF from lower MAC addresses as SN#!
// ...so these can be re-allocated at some point!
#define FLASH_OPT1_INSTALLED_ptr	405 // Serial NUM 00000 -> FFFF0 (1048560)
#define FLASH_OPT1_ENABLED_ptr		406 // 00000 & FFFFF reserved
#define FLASH_OPT2_INSTALLED_ptr	407
#define FLASH_OPT2_ENABLED_ptr		408
#define SPARE_FLASH_ptr				409


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

/**************************************************************
**   NON-Volatile Data stored in Flash as FlashParameters   **
**************************************************************/
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
#define MaxConfig2 			445//785//5545		// expanded TSE+10G Ethernet
//...with our new 4800-byte Multi-Stream Only architecture - this should not change from Config2 we use today
// DOES IT ??? - With the inclusion of general headers + emix and stuff it'll be different - >150
#define MaxConfig_ms 	    5502		// further extended for TrafficEngine/MultiStreaming support

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
					#define ETH_SPEED_1G	(MACA_COMMAND_CONFIG & MAC_COMMAND_SPEED_1G)
					#define ENA_10			(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_ENA_10_MSK)
					#define HD_ENA			(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_HD_ENA_MSK)
					#define LOOP_ENA		(MACA_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_LOOPBACK_MSK)
					#define ETH_SPEED_1GB	(MACB_COMMAND_CONFIG & MAC_COMMAND_SPEED_1G)
					#define ENA_10B			(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_ENA_10_MSK)
					#define HD_ENAB			(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_HD_ENA_MSK)
					#define LOOP_ENAB		(MACB_COMMAND_CONFIG & ALTERA_TSEMAC_CMD_LOOPBACK_MSK)
					#define MAC_COMMAND_SPEED_MASK	0xFDFFFFF7		// bit.25:ENA_10:1=10M,0=100M  AND  bit.3:ETH_SPEED_1G:1=1G,0=10/100
					#define MAC_COMMAND_SPEED_10M	0x02000000
					#define MAC_COMMAND_SPEED_100M	0x00000000
					#define MAC_COMMAND_SPEED_1G	0x00000008
/////////////////////////////////////////////////////////////////////////////////////////////////
#define MAC_FRAME_LEN1_ptr	10		// ...also used by EGEN (pkt_length) EGEN_PKT_LENTYPE1_ptr
#define MAC_FRAME_LEN0_ptr	11		// The maximum Frame Length the TSE uses to chk rcvd frames

#define MAC_PAUSEQUANT1_ptr	12		// FLOW CONTROL - PAUSE DURATION
#define MAC_PAUSEQUANT0_ptr	13

#define MAC_IPG_LENGTH_ptr	14		// Inter-Packet/Frame Gap size (in bytes)  VALID:8 to 26, 12 is default as typ min IPG
#define MAC_MISC_ptr		15			// Misc control flags RESET_MAC, OMIT_CRC, TX_SHIFT16, RX_SHIFT16, ...

#define MAC_ADDR_MSB03_ptr	16			// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MAC_ADDR_MSB02_ptr	17			// these are 0x001C23174ACB
#define MAC_ADDR_MSB01_ptr	18			// loaded into mac_0 = 0x17231C00  (reverse order)
#define MAC_ADDR_MSB00_ptr	19			//    and into mac_1 = 0x0000CB4A
#define MAC_ADDR_LSB11_ptr	20
#define MAC_ADDR_LSB10_ptr	21

#define MAC_MULTICASTHASH_MSB3_ptr	22	// Multicast Hash table MSB (each bit goes into bit.0 of the respective Hash register)
#define MAC_MULTICASTHASH_MSB2_ptr	23
#define MAC_MULTICASTHASH_MSB1_ptr	24
#define MAC_MULTICASTHASH_MSB0_ptr	25

#define MAC_MULTICASTHASH_LSB3_ptr	26	// Multicast Hash table LSB (each bit goes into bit.0 of the respective Hash register)
#define MAC_MULTICASTHASH_LSB2_ptr	27
#define MAC_MULTICASTHASH_LSB1_ptr	28
#define MAC_MULTICASTHASH_LSB0_ptr	29

//Currently being used to pass IP src and Dest addresses for Loopback. Can be alternatively used as SMAC by toggling a state where one state defines SMAC and the other defines lpbk for IP
//The cost we pay for wanting more IP addresses - Ipv6.

/*
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
*/
#define LOOPBACK_IP0_SRC3_ptr	30					//Supplementary MAC ADDR 3 MSB
#define LOOPBACK_IP0_SRC2_ptr	31					//SRC IP ADDR (for IPv4 and IPv6)
#define LOOPBACK_IP0_SRC1_ptr	32
#define LOOPBACK_IP0_SRC0_ptr	33

#define LOOPBACK_IP1_SRC3_ptr	34					//Supplementary MAC ADDR 3 LSB
#define LOOPBACK_IP1_SRC2_ptr	35
#define LOOPBACK_IP1_SRC1_ptr	36
#define LOOPBACK_IP1_SRC0_ptr	37

#define LOOPBACK_IP2_SRC3_ptr	38					//Supplementary MAC ADDR 3 MSB
#define LOOPBACK_IP2_SRC2_ptr	39
#define LOOPBACK_IP2_SRC1_ptr	40
#define LOOPBACK_IP2_SRC0_ptr	41

#define LOOPBACK_IP3_SRC3_ptr	42					//Supplementary MAC ADDR 3 LSB
#define LOOPBACK_IP3_SRC2_ptr	43
#define LOOPBACK_IP3_SRC1_ptr	44
#define LOOPBACK_IP3_SRC0_ptr	45

#define LOOPBACK_IP0_DEST3_ptr	46					//Supplementary MAC ADDR 3 MSB
#define LOOPBACK_IP0_DEST2_ptr	47
#define LOOPBACK_IP0_DEST1_ptr	48
#define LOOPBACK_IP0_DEST0_ptr	49

#define LOOPBACK_IP1_DEST3_ptr	50					//Supplementary MAC ADDR 3 LSB
#define LOOPBACK_IP1_DEST2_ptr	51					//DEST IP ADDR (for IPv4 and IPv6)
#define LOOPBACK_IP1_DEST1_ptr	52
#define LOOPBACK_IP1_DEST0_ptr	53

#define LOOPBACK_IP2_DEST3_ptr	54					//Supplementary MAC ADDR 3 MSB
#define LOOPBACK_IP2_DEST2_ptr	55
#define LOOPBACK_IP2_DEST1_ptr	56
#define LOOPBACK_IP2_DEST0_ptr	57

#define LOOPBACK_IP3_DEST3_ptr	58					//Supplementary MAC ADDR 3 LSB
#define LOOPBACK_IP3_DEST2_ptr	59
#define LOOPBACK_IP3_DEST1_ptr	60
#define LOOPBACK_IP3_DEST0_ptr	61
//=================== PHY CONFIG/CONTROL SETTINGS ======
#define PHY_CONFIG1_ptr		62			// 16-bits: RESET, MODE, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG,
#define PHY_CONFIG0_ptr 	63		//			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
#define PHY_SFP_MISC_ptr	64		// ^0:1=SFP_A Transmit_OFF, ^1:1=SFP_B Transmit_OFF, ^2:1=SFP+A Transmit_OFF

#define CORE_SIGROUTE_MODES_ptr 65
//=================== MISC SETTINGS ======
#define CLEAR_ptr     	66		// ^0=restart ET, ^1=reset MAC-A stat's, ^2=reset MAC-B stat's
#define MISC_ptr     	67		// DumpSFPAinfo^0, DumpSFPBinfo^1, SFP+A^2, SFP+B^3
#define BERT_ptr		68
//TEST_OPTIONS_ptr – new def as shared with Justin
//^0:Quick Test ^1:Multistream Test ^2:Y.1564 service configuration ^3: Y.1564 service performance ^4: Y.1564 both(configuration & performance) ^5:future(probably rfc test) ^6:future ^7:future
#define TEST_OPTIONS_PTR 		69		//^0:Quick Test ^1:Multistream Test options to select between (quick test,multistream, y.1564, rfc..) also used for Y.1564  service configuration/service performance/both)
#define TRAFFIC_NUM_OF_STREAMS 	70		// lower nibble is how many streams:
/*******************************************
//FW will use Duration-SECS to calculate TOTAL NUM of PKTS to GEN whether 1 stream or 16-streams.
//FW will pass TOTAL_NUM_PKTS in status to UI (for Frame loss measurements)
//NOTE: NUM_PKTS will be by stream
// ...set on UI Duration change*/
#define EGEN_NUM_SECS3_ptr 	71	// previously all four bytes were used to send frame size and seconds information. Now as the fw is the only one that calculates the number of packets no need to send that number down
#define EGEN_NUM_SECS2_ptr 	72  // which makes the upper bytes EXTRA
#define EGEN_NUM_SECS1_ptr 	73	// num of secs to generate Packets !  18+ hours
#define EGEN_NUM_SECS0_ptr 	74		// Rcvd from GUI and just saved for GUI (for GUI recovery)!

#define  TRAFFIC_GEN_NUM_BURST_PACKETS10_ptr 	75//extra
#define  TRAFFIC_GEN_NUM_BURST_PACKETS03_ptr	76
#define  TRAFFIC_GEN_NUM_BURST_PACKETS02_ptr	77
#define  TRAFFIC_GEN_NUM_BURST_PACKETS01_ptr	78
#define  TRAFFIC_GEN_NUM_BURST_PACKETS00_ptr	79

//First 8 bytes to be used for UDP in loopback
#define  LOOPBACK_ptr			80 //loopback_ptr

#define UDP_SRC_ADD_ptr03 		81
#define UDP_SRC_ADD_ptr02 	    82
#define UDP_DEST_ADD_ptr03 	    83
#define UDP_DEST_ADD_ptr02 	    84
#define UDP_LENGTH_ptr01    	85
#define UDP_LENGTH_ptr00    	86
#define UDP_CHKSUM_ptr01    	87
#define UDP_CHKSUM_ptr00    	88



#define  TRAFFIC_SPARE_ptr_10	89
#define  TRAFFIC_SPARE_ptr_11	90

#define  TRAFFIC_SPARE_ptr_12	91
#define  TRAFFIC_SPARE_ptr_13	92
#define  TRAFFIC_SPARE_ptr_14	93
#define  TRAFFIC_SPARE_ptr_15	94
#define  TRAFFIC_SPARE_ptr_26	95
#define  TRAFFIC_SPARE_ptr_27	96
#define  TRAFFIC_SPARE_ptr_28	97
#define  TRAFFIC_SPARE_ptr_29	98
//#define  TRAFFIC_SPARE_ptr_30	99 //TRAFFIC_GEN_PKT_EMIXUSERSIZE4_ptr - used to handle the case w in EMIX
//#define  TRAFFIC_SPARE_ptr_31	100


/*
#define TRAFFIC_HDR0_3_ptr	186
#define TRAFFIC_HDR0_2_ptr	187
#define TRAFFIC_HDR0_1_ptr	188
#define TRAFFIC_HDR0_0_ptr	189
#define TRAFFIC_HDR1_3_ptr	190
#define TRAFFIC_HDR1_2_ptr	191
#define TRAFFIC_HDR1_1_ptr	192
#define TRAFFIC_HDR1_0_ptr	193
#define TRAFFIC_HDR2_3_ptr	194
#define TRAFFIC_HDR2_2_ptr	195
#define TRAFFIC_HDR2_1_ptr	196
#define TRAFFIC_HDR2_0_ptr	197
*/





/*

#define TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr	135
		#define TRAFFIC_GEN_PKT_EMIXUSERSIZE2_ptr	136
		#define TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr	137
		#define TRAFFIC_GEN_PKT_EMIXUSERSIZE0_ptr	138

		#define TRAFFIC_GEN_EMIXSEQ33_ptr  	139			// MSNibble sent 1st   def for Random=0x0123456F
		#define TRAFFIC_GEN_EMIXSEQ32_ptr  	140
		#define TRAFFIC_GEN_EMIXSEQ31_ptr  	141
		#define TRAFFIC_GEN_EMIXSEQ30_ptr  	142
		#define TRAFFIC_GEN_EMIXSEQ23_ptr		143				//(good to set 1,2&3 def as 0xF0000000)
		#define TRAFFIC_GEN_EMIXSEQ22_ptr  	144
		#define TRAFFIC_GEN_EMIXSEQ21_ptr  	145
		#define TRAFFIC_GEN_EMIXSEQ20_ptr 		146
		#define TRAFFIC_GEN_EMIXSEQ13_ptr		147
		#define TRAFFIC_GEN_EMIXSEQ12_ptr  	148
		#define TRAFFIC_GEN_EMIXSEQ11_ptr		149
		#define TRAFFIC_GEN_EMIXSEQ10_ptr		150
		#define TRAFFIC_GEN_EMIXSEQ03_ptr 		151			//LSNibble sent last
		#define TRAFFIC_GEN_EMIXSEQ02_ptr 		152
		#define TRAFFIC_GEN_EMIXSEQ01_ptr		153
		#define TRAFFIC_GEN_EMIXSEQ00_ptr  	154
*/







// Single stream block
//------------------------------------------------------------------
// generic pointer bytes = (0-98) 99 bytes
// single_stream_offset = lengthOf(generic pointer bytes) + 1
//______________________________________________________________________
// gconfig[TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr + single_stream_offset] [gconfig(0 + 99)]
// gconfig[TRAFFIC_GEN_PKT_EMIXUSERSIZE4_ptr + single_stream_offset]	[gconfig(1 + 99)]
// gconfig[TRAFFIC_MON_STREAM_CNTL3_ptr + single_stream_offset] [gconfig(2 + 99)]
// gconfig[TRAFFIC_MON_STREAM_CNTL2_ptr + single_stream_offset]	[gconfig(3 + 99)]
//							.
//							.
//							.
// gconfig[TRAFFIC_MSK31_0_ptr + single_stream_offset] [gconfig(342 + offset)]
//______________________________________________________________________
// startstop_ptr (size_offset +1)
// checksum1_ptr (size_offset +1)
// checksum2_ptr (size_offset +1)
//----------------------------------------------------------------------
// *The size_offset being 343 (The number of bytes required for each stream = 0 to 342)

//unsigned int single_stream_offset   99


//----------------Single Stream ---------------------------------
/*
#define TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr	99
#define TRAFFIC_GEN_PKT_EMIXUSERSIZE4_ptr	100
#define TRAFFIC_MON_STREAM_CNTL3_ptr	101			// MON stream control (for independent capture)
#define TRAFFIC_MON_STREAM_CNTL2_ptr	102
#define TRAFFIC_MON_STREAM_CNTL1_ptr	103
#define TRAFFIC_MON_STREAM_CNTL0_ptr	104

#define TRAFFICRATE_TYPE			105				// TBD: e.g. 0=FPS%, 1=Gbps, 2=Mbps, 3=Kbps
#define TRAFFIC_RATE3_ptr          	106
#define TRAFFIC_RATE2_ptr     	107		 // Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%
#define TRAFFIC_RATE1_ptr     	108			 // ie Gbps   0.0001-100.0G,  includes 99.9999 or 9.9999G or 0.9999G
#define TRAFFIC_RATE0_ptr     	109		  //  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M

#define TRAFFIC_GEN_PKTPERCENT11_ptr	110	 // 2-byte spares for 100G
#define TRAFFIC_GEN_PKTPERCENT10_ptr	111
#define TRAFFIC_GEN_PKTPERCENT03_ptr	112
#define TRAFFIC_GEN_PKTPERCENT02_ptr	113
#define TRAFFIC_GEN_PKTPERCENT01_ptr	114
#define TRAFFIC_GEN_PKTPERCENT00_ptr	115
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
#define EGEN_PKT_TYPE_ptr 		116
#define EGEN_PKT_TYPE_ptr_1 	117
#define EGEN_PKT_TYPE_ptr_2 	118

......for
TRAFFIC_GEN_CONFIG
TRAFFIC_GEN_CONFIG1
TRAFFIC_GEN_CONFIG2
... in hardware_def.h copy the bit definitions & mask locations so the UI can set these 32-bit words
    as necessary per the FRAME CONFIGURED!
....the UI will be taking over all TRAFFIC_GEN & MON RELATED configurations associated with the following;

and the associated frame type settings like how long the hdr is, where the IPv4 checksum is, where the payload starts, etc. etc.
...all these elements are within these 3 GEN config bytes (note there are some MON config ones as well..
	// packet length will be changed by the generator if it is too small to fit required
//   header plus sequence_number/stream/timestamp.
//   for ipv4, the header checksum must also be changed.
// packet length is one value in a Y.1564 EMIX table for random length mode (see RNDUSR and RNDSEQ below));
//=================================================================================
//public static  short  TRAFFIC_GEN_CONFIG	0x200	//0x80 Config Fields for GENERATOR
// 	Structure:     	bits
// CONFIG_PKLEN		0:15		16-bit packet length 64-9600: 0x40-0x2580 (or mask in RND len modes (e.g. 0xff - clamps size to 255));
// CONFIG_HDRLEN  	16:23		8-bit header length (header can be 0 to 255 bytes)); (note MAC, IPv4, IPv6 overwritten with correct length by GEN));
// CONFIG_RNDLEN  	24		 	1-bit Random Length packets mode: 1=enable Random mode, 0=fixed length frames
// CONFIG_RNDPAY		25			1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
// CONFIG_FRCERR		26			1-bit Force Payload data error (field size bits)); DEBUGGING ONLY
// CONFIG_CONTIN   	27		      1-bit Continuous Generate (ignore PACKET_NUM));
// CONFIG_BWRANK   	28:31		4-bits bandwidth rank: for streams set priority to higher BW above those with lower BW. 0=top, 15=lowest
//==================================================================================
//	unsigned int  traffic_gen_config2 = 12;	// non-IPv4, 802.3frame Length field at byte12
		// CONFG2_LENOFS_SIZ		0:6			// 7 bits: Length field byte offset in HDR
		// in EMIX mode set offset to 1st byte of HDR array (ie 802.3 frame frame_len starts at byte 12 so set this to 12).
		// CONFG2_JUMBO_SIZ		   7			// 1 bit:  jumbo length to 0x8870 handling field size bits
		//#define JUMBO_MASK		0x80
		// CONFG2_HCSOFS_SIZ	8:14		// 7-bits: ipv4 hcs byte offset field size bits
		// CONFG2_SPARE2_SIZ	     15:31		// 17-bits: spare field size bits
	//	unsigned int  traffic_gen_config3 = 0x059C;			// 802.3, 1518=payload len of 1500, 1500-64=1436, 0x5ee-0x40=0x059C
		// CONFIG3_ILEN		0:15			16-bit:  intermediate length for random length recalc field size bits
		// CONFIG3_IHCS	     16:31			16-bit: intermediate HCS for random length HCS recalc field size bits



#define TRAFFIC_GEN_CONFIG23_ptr	119		  //TRAFFIC_GEN_CONFIG3
#define TRAFFIC_GEN_CONFIG22_ptr	120
#define TRAFFIC_GEN_CONFIG21_ptr	121
#define TRAFFIC_GEN_CONFIG20_ptr	122

#define TRAFFIC_GEN_CONFIG13_ptr	123			//TRAFFIC_GEN_CONFIG2
#define TRAFFIC_GEN_CONFIG12_ptr	124		 //15:31 // 17-bits: spare field size bits
#define TRAFFIC_GEN_CONFIG11_ptr	125		//CONFG2_HCSOFS_SIZ	8:14 // 7-bits: ipv4 hcs byte offset field size bits
#define TRAFFIC_GEN_CONFIG10_ptr	126		// 7 bits: Length field byte offset in HDR + 1 bit:  jumbo length to 0x8870 handling field size bits

#define TRAFFIC_GEN_CONFIG03_ptr	127		// TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed
#define TRAFFIC_GEN_CONFIG02_ptr	128
#define TRAFFIC_GEN_CONFIG01_ptr	129
#define TRAFFIC_GEN_CONFIG00_ptr	130


#define TRAFFIC_GEN_CONFIG33_ptr	131
#define TRAFFIC_GEN_CONFIG32_ptr	132
#define TRAFFIC_GEN_CONFIG31_ptr	133
#define TRAFFIC_GEN_CONFIG30_ptr	134
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
					#define LEN_MASK				0xFFFF
					#define HDRLEN_MASK				0xFF0000
					#define RNDLEN_ON				0x01000000
					#define RNDPAY_ON				0x02000000
					#define FRCERR_ON				0x04000000								//0xFFFF
					#define CONTINUOUS_ON 			0x08000000					//0xFF0000		//62	3E
					#define BWRANK_MASK 			0xF0000000		   			//0x01000000

					#define LENOFS_MASK				0x007F	// Payload 1st byte location (0 is 1st byte of HDR0)
					#define JUMBO_MASK				0x0080
					#define HCSOFF_MASK				0x7F00

					#define ILEN_MASK				0x0000FFFF	// ILEN = payload length - 64, (payload length=pkt-len-18)
					#define IHCS_MASK				0xFFFF0000

					#define STREAM_CNTL_ENABLE_MASK			0x00000001		// 1-bit:  Stream Enable bit
					#define STREAM_CNTL_SPARE1_MASK			0x000000FE		// 7-bit: spare - not used
					#define STREAM_CNTL_HDRLEN_MASK			0x0003FF00		//10-bit: Stream header length
					#define STREAM_CNTL_SPARE2_MASK			0xFFFC0000		//14-bit: spare - not used

//_______________________________________________________________________________________
//		a	  b		 c		 d		  e		  f		  g		        h	   u         invalid and
//	    64	  128	256		512		1024	1280	usr_g	MTU(pktlen)  user_u     terminates_seq
//	    0      1     2       3        4       5       6             7      8             9-F
//_______________________________________________________________________________________
#define EGEN_PKT_EMIXUSERSIZE3_ptr	135  //user_u MSB
#define EGEN_PKT_EMIXUSERSIZE2_ptr	136  //user_u LSB
#define EGEN_PKT_EMIXUSERSIZE1_ptr	137  //user_g MSB
#define EGEN_PKT_EMIXUSERSIZE0_ptr	138  //user_g LSB

#define EGEN_EMIXSEQ33_ptr  	139			// MSNibble sent 1st default for Random=0x0123456F
#define EGEN_EMIXSEQ32_ptr  	140
#define EGEN_EMIXSEQ31_ptr  	141
#define EGEN_EMIXSEQ30_ptr  	142
#define EGEN_EMIXSEQ23_ptr		143			//(good to set 1,2&3 def as 0xF0000000)
#define EGEN_EMIXSEQ22_ptr  	144
#define EGEN_EMIXSEQ21_ptr  	145
#define EGEN_EMIXSEQ20_ptr 		146
#define EGEN_EMIXSEQ13_ptr		147
#define EGEN_EMIXSEQ12_ptr  	148
#define EGEN_EMIXSEQ11_ptr		149
#define EGEN_EMIXSEQ10_ptr		150
#define EGEN_EMIXSEQ03_ptr 		151
#define EGEN_EMIXSEQ02_ptr 		152
#define EGEN_EMIXSEQ01_ptr		153
#define EGEN_EMIXSEQ00_ptr  	154
*************************************************************************************************************************
// Config bytes related to Y.1564 & RFC2544 User selectable Test thresholds
***************************************************************************************************************************
#define TEST_OPTIONS_PTR 			XX // Test options to select between (quick test,multistream, y.1564, rfc..) - In the main Config is used to set the different type of test- service configuration/service performance/both)
...//...CIR & EIR are 4-bytes (they are rates - like TRAFFICRATE_TYPE above)
//RFC2544 will share/overlap these thresholds to allow threshold support for RFC2544 as well
#define  Y1564_CIR_3_ptr	155				 //Committed information rate
#define  Y1564_CIR_2_ptr	156
#define  Y1564_CIR_1_ptr	157
#define  Y1564_CIR_0_ptr	158

#define  Y1564_EIR_3_ptr	159					//Expected information rate
#define  Y1564_EIR_2_ptr	160					//Expected information rate
#define  Y1564_EIR_1_ptr	161					//Expected information rate
#define  Y1564_EIR_0_ptr	162					//Expected information rate

#define  Y1564_CIR_STEPS_ptr	163				//Steps for CIR – first 4 bits

#define  Y1564_MAX_FTD_3_ptr 	164			//4 byte Float
#define  Y1564_MAX_FTD_2_ptr 	165
#define  Y1564_MAX_FTD_1_ptr 	166
#define  Y1564_MAX_FTD_0_ptr 	167

#define  Y1564_MAX_FDV_3_ptr 	168			//4 byte Float
#define  Y1564_MAX_FDV_2_ptr 	169
#define  Y1564_MAX_FDV_1_ptr 	170
#define  Y1564_MAX_FDV_0_ptr 	171

#define  Y1564_MAX_FRAME_LOSS_3_ptr 	172		//4 byte Float
#define  Y1564_MAX_FRAME_LOSS_2_ptr 	173
#define  Y1564_MAX_FRAME_LOSS_1_ptr 	174
#define  Y1564_MAX_FRAME_LOSS_0_ptr 	175

#define  Y1564_MAX_AVAIL_3_ptr 	176			//4 byte Float
#define  Y1564_MAX_AVAIL_2_ptr 	177
#define  Y1564_MAX_AVAIL_1_ptr 	178
#define  Y1564_MAX_AVAIL_0_ptr 	179

#define SPARE_THRESHOLD_03	180
#define SPARE_THRESHOLD_02	181
#define SPARE_THRESHOLD_01	182
#define SPARE_THRESHOLD_00	183

#define TRAFFIC_GEN_SPARE_ptr	184
#define TRAFFIC_HDR_DEF_ptr		185				// additional instructional info for what is contained in HDR ptr's


#define TRAFFIC_HDR0_3_ptr	186
#define TRAFFIC_HDR0_2_ptr	187
#define TRAFFIC_HDR0_1_ptr	188
#define TRAFFIC_HDR0_0_ptr	189
#define TRAFFIC_HDR1_3_ptr	190
#define TRAFFIC_HDR1_2_ptr	191
#define TRAFFIC_HDR1_1_ptr	192
#define TRAFFIC_HDR1_0_ptr	193
#define TRAFFIC_HDR2_3_ptr	194
#define TRAFFIC_HDR2_2_ptr	195
#define TRAFFIC_HDR2_1_ptr	196
#define TRAFFIC_HDR2_0_ptr	197


#define TRAFFIC_HDR3_3_ptr	198
#define TRAFFIC_HDR3_2_ptr	199
#define TRAFFIC_HDR3_1_ptr	200
#define TRAFFIC_HDR3_0_ptr	201
#define TRAFFIC_HDR4_3_ptr	202
#define TRAFFIC_HDR4_2_ptr	203
#define TRAFFIC_HDR4_1_ptr	204
#define TRAFFIC_HDR4_0_ptr	205
#define TRAFFIC_HDR5_3_ptr	206
#define TRAFFIC_HDR5_2_ptr	207
#define TRAFFIC_HDR5_1_ptr	208
#define TRAFFIC_HDR5_0_ptr	209
#define TRAFFIC_HDR6_3_ptr	210
#define TRAFFIC_HDR6_2_ptr	211
#define TRAFFIC_HDR6_1_ptr	212
#define TRAFFIC_HDR6_0_ptr	213
#define TRAFFIC_HDR7_3_ptr	214
#define TRAFFIC_HDR7_2_ptr	215
#define TRAFFIC_HDR7_1_ptr	216
#define TRAFFIC_HDR7_0_ptr	217
#define TRAFFIC_HDR8_3_ptr	218
#define TRAFFIC_HDR8_2_ptr	219
#define TRAFFIC_HDR8_1_ptr	220
#define TRAFFIC_HDR8_0_ptr	221
#define TRAFFIC_HDR9_3_ptr	222
#define TRAFFIC_HDR9_2_ptr	223
#define TRAFFIC_HDR9_1_ptr	224
#define TRAFFIC_HDR9_0_ptr	225
#define TRAFFIC_HDR10_3_ptr	226
#define TRAFFIC_HDR10_2_ptr	227
#define TRAFFIC_HDR10_1_ptr	228
#define TRAFFIC_HDR10_0_ptr	229
#define TRAFFIC_HDR11_3_ptr	230
#define TRAFFIC_HDR11_2_ptr	231
#define TRAFFIC_HDR11_1_ptr	232
#define TRAFFIC_HDR11_0_ptr	233
#define TRAFFIC_HDR12_3_ptr	234
#define TRAFFIC_HDR12_2_ptr	235
#define TRAFFIC_HDR12_1_ptr	236
#define TRAFFIC_HDR12_0_ptr	237
#define TRAFFIC_HDR13_3_ptr	238
#define TRAFFIC_HDR13_2_ptr	239
#define TRAFFIC_HDR13_1_ptr	240
#define TRAFFIC_HDR13_0_ptr	241
#define TRAFFIC_HDR14_3_ptr	242
#define TRAFFIC_HDR14_2_ptr	243
#define TRAFFIC_HDR14_1_ptr	244
#define TRAFFIC_HDR14_0_ptr	245
#define TRAFFIC_HDR15_3_ptr	246
#define TRAFFIC_HDR15_2_ptr	247
#define TRAFFIC_HDR15_1_ptr	248
#define TRAFFIC_HDR15_0_ptr	249
#define TRAFFIC_HDR16_3_ptr	250
#define TRAFFIC_HDR16_2_ptr	251
#define TRAFFIC_HDR16_1_ptr	252
#define TRAFFIC_HDR16_0_ptr	253
#define TRAFFIC_HDR17_3_ptr	254
#define TRAFFIC_HDR17_2_ptr	255
#define TRAFFIC_HDR17_1_ptr	256
#define TRAFFIC_HDR17_0_ptr	257
#define TRAFFIC_HDR18_3_ptr	258
#define TRAFFIC_HDR18_2_ptr	259
#define TRAFFIC_HDR18_1_ptr	260
#define TRAFFIC_HDR18_0_ptr	261
#define TRAFFIC_HDR19_3_ptr	262
#define TRAFFIC_HDR19_2_ptr	263
#define TRAFFIC_HDR19_1_ptr	264
#define TRAFFIC_HDR19_0_ptr	265
#define TRAFFIC_HDR20_3_ptr	266
#define TRAFFIC_HDR20_2_ptr	267
#define TRAFFIC_HDR20_1_ptr	268
#define TRAFFIC_HDR20_0_ptr	269
#define TRAFFIC_HDR21_3_ptr	270
#define TRAFFIC_HDR21_2_ptr	271
#define TRAFFIC_HDR21_1_ptr	272
#define TRAFFIC_HDR21_0_ptr	273
#define TRAFFIC_HDR22_3_ptr	274
#define TRAFFIC_HDR22_2_ptr	275
#define TRAFFIC_HDR22_1_ptr	276
#define TRAFFIC_HDR22_0_ptr	277
#define TRAFFIC_HDR23_3_ptr	278
#define TRAFFIC_HDR23_2_ptr	279
#define TRAFFIC_HDR23_1_ptr	280
#define TRAFFIC_HDR23_0_ptr	281
#define TRAFFIC_HDR24_3_ptr	282
#define TRAFFIC_HDR24_2_ptr	283
#define TRAFFIC_HDR24_1_ptr	284
#define TRAFFIC_HDR24_0_ptr	285
#define TRAFFIC_HDR25_3_ptr	286
#define TRAFFIC_HDR25_2_ptr	287
#define TRAFFIC_HDR25_1_ptr	288
#define TRAFFIC_HDR25_0_ptr	289
#define TRAFFIC_HDR26_3_ptr	290
#define TRAFFIC_HDR26_2_ptr	291
#define TRAFFIC_HDR26_1_ptr	292
#define TRAFFIC_HDR26_0_ptr	293
#define TRAFFIC_HDR27_3_ptr	294
#define TRAFFIC_HDR27_2_ptr	295
#define TRAFFIC_HDR27_1_ptr	296
#define TRAFFIC_HDR27_0_ptr	297
#define TRAFFIC_HDR28_3_ptr	298
#define TRAFFIC_HDR28_2_ptr	299
#define TRAFFIC_HDR28_1_ptr	300
#define TRAFFIC_HDR28_0_ptr	301
#define TRAFFIC_HDR29_3_ptr	302
#define TRAFFIC_HDR29_2_ptr	303
#define TRAFFIC_HDR29_1_ptr	304
#define TRAFFIC_HDR29_0_ptr	305
#define TRAFFIC_HDR30_3_ptr	306
#define TRAFFIC_HDR30_2_ptr	307
#define TRAFFIC_HDR30_1_ptr	308
#define TRAFFIC_HDR30_0_ptr	309
#define TRAFFIC_HDR31_3_ptr	310
#define TRAFFIC_HDR31_2_ptr	311
#define TRAFFIC_HDR31_1_ptr	312
#define TRAFFIC_HDR31_0_ptr	313

#define TRAFFIC_MSK0_3_ptr	314
#define TRAFFIC_MSK0_2_ptr	315
#define TRAFFIC_MSK0_1_ptr	316
#define TRAFFIC_MSK0_0_ptr	317
#define TRAFFIC_MSK1_3_ptr	318
#define TRAFFIC_MSK1_2_ptr	319
#define TRAFFIC_MSK1_1_ptr	320
#define TRAFFIC_MSK1_0_ptr	321
#define TRAFFIC_MSK2_3_ptr	322
#define TRAFFIC_MSK2_2_ptr	323
#define TRAFFIC_MSK2_1_ptr	324
#define TRAFFIC_MSK2_0_ptr	325
#define TRAFFIC_MSK3_3_ptr	326
#define TRAFFIC_MSK3_2_ptr	327
#define TRAFFIC_MSK3_1_ptr	328
#define TRAFFIC_MSK3_0_ptr	329
#define TRAFFIC_MSK4_3_ptr	330
#define TRAFFIC_MSK4_2_ptr	331
#define TRAFFIC_MSK4_1_ptr	332
#define TRAFFIC_MSK4_0_ptr	333
#define TRAFFIC_MSK5_3_ptr	334
#define TRAFFIC_MSK5_2_ptr	335
#define TRAFFIC_MSK5_1_ptr	336
#define TRAFFIC_MSK5_0_ptr	337
#define TRAFFIC_MSK6_3_ptr	338
#define TRAFFIC_MSK6_2_ptr	339
#define TRAFFIC_MSK6_1_ptr	340
#define TRAFFIC_MSK6_0_ptr	341
#define TRAFFIC_MSK7_3_ptr	342
#define TRAFFIC_MSK7_2_ptr	343
#define TRAFFIC_MSK7_1_ptr	344
#define TRAFFIC_MSK7_0_ptr	345
#define TRAFFIC_MSK8_3_ptr	346
#define TRAFFIC_MSK8_2_ptr	347
#define TRAFFIC_MSK8_1_ptr	348
#define TRAFFIC_MSK8_0_ptr	349
#define TRAFFIC_MSK9_3_ptr	350
#define TRAFFIC_MSK9_2_ptr	351
#define TRAFFIC_MSK9_1_ptr	352
#define TRAFFIC_MSK9_0_ptr	353
#define TRAFFIC_MSK10_3_ptr	354
#define TRAFFIC_MSK10_2_ptr	355
#define TRAFFIC_MSK10_1_ptr	356
#define TRAFFIC_MSK10_0_ptr	357
#define TRAFFIC_MSK11_3_ptr	358
#define TRAFFIC_MSK11_2_ptr	359
#define TRAFFIC_MSK11_1_ptr	360
#define TRAFFIC_MSK11_0_ptr	361
#define TRAFFIC_MSK12_3_ptr	362
#define TRAFFIC_MSK12_2_ptr	363
#define TRAFFIC_MSK12_1_ptr	364
#define TRAFFIC_MSK12_0_ptr	365
#define TRAFFIC_MSK13_3_ptr	366
#define TRAFFIC_MSK13_2_ptr	367
#define TRAFFIC_MSK13_1_ptr	368
#define TRAFFIC_MSK13_0_ptr	369
#define TRAFFIC_MSK14_3_ptr	370
#define TRAFFIC_MSK14_2_ptr	371
#define TRAFFIC_MSK14_1_ptr	372
#define TRAFFIC_MSK14_0_ptr	373
#define TRAFFIC_MSK15_3_ptr	374
#define TRAFFIC_MSK15_2_ptr	375
#define TRAFFIC_MSK15_1_ptr	376
#define TRAFFIC_MSK15_0_ptr	377
#define TRAFFIC_MSK16_3_ptr	378
#define TRAFFIC_MSK16_2_ptr	379
#define TRAFFIC_MSK16_1_ptr	380
#define TRAFFIC_MSK16_0_ptr	381
#define TRAFFIC_MSK17_3_ptr	382
#define TRAFFIC_MSK17_2_ptr	383
#define TRAFFIC_MSK17_1_ptr	384
#define TRAFFIC_MSK17_0_ptr	385
#define TRAFFIC_MSK18_3_ptr	386
#define TRAFFIC_MSK18_2_ptr	387
#define TRAFFIC_MSK18_1_ptr	388
#define TRAFFIC_MSK18_0_ptr	389
#define TRAFFIC_MSK19_3_ptr	390
#define TRAFFIC_MSK19_2_ptr	391
#define TRAFFIC_MSK19_1_ptr	392
#define TRAFFIC_MSK19_0_ptr	393
#define TRAFFIC_MSK20_3_ptr	394
#define TRAFFIC_MSK20_2_ptr	395
#define TRAFFIC_MSK20_1_ptr	396
#define TRAFFIC_MSK20_0_ptr	397
#define TRAFFIC_MSK21_3_ptr	398
#define TRAFFIC_MSK21_2_ptr	399
#define TRAFFIC_MSK21_1_ptr	400
#define TRAFFIC_MSK21_0_ptr	401
#define TRAFFIC_MSK22_3_ptr	402
#define TRAFFIC_MSK22_2_ptr	403
#define TRAFFIC_MSK22_1_ptr	404
#define TRAFFIC_MSK22_0_ptr	405
#define TRAFFIC_MSK23_3_ptr	406
#define TRAFFIC_MSK23_2_ptr	407
#define TRAFFIC_MSK23_1_ptr	408
#define TRAFFIC_MSK23_0_ptr	409
#define TRAFFIC_MSK24_3_ptr	410
#define TRAFFIC_MSK24_2_ptr	411
#define TRAFFIC_MSK24_1_ptr	412
#define TRAFFIC_MSK24_0_ptr	413
#define TRAFFIC_MSK25_3_ptr 414
#define TRAFFIC_MSK25_2_ptr	415
#define TRAFFIC_MSK25_1_ptr 416
#define TRAFFIC_MSK25_0_ptr	417
#define TRAFFIC_MSK26_3_ptr	418
#define TRAFFIC_MSK26_2_ptr	419
#define TRAFFIC_MSK26_1_ptr	420
#define TRAFFIC_MSK26_0_ptr	421
#define TRAFFIC_MSK27_3_ptr	422
#define TRAFFIC_MSK27_2_ptr	423
#define TRAFFIC_MSK27_1_ptr	424
#define TRAFFIC_MSK27_0_ptr	425
#define TRAFFIC_MSK28_3_ptr	426
#define TRAFFIC_MSK28_2_ptr	427
#define TRAFFIC_MSK28_1_ptr	428
#define TRAFFIC_MSK28_0_ptr	429
#define TRAFFIC_MSK29_3_ptr	430
#define TRAFFIC_MSK29_2_ptr	431
#define TRAFFIC_MSK29_1_ptr	432
#define TRAFFIC_MSK29_0_ptr	433
#define TRAFFIC_MSK30_3_ptr	434
#define TRAFFIC_MSK30_2_ptr	435
#define TRAFFIC_MSK30_1_ptr	436
#define TRAFFIC_MSK30_0_ptr	437
#define TRAFFIC_MSK31_3_ptr	438
#define TRAFFIC_MSK31_2_ptr	439
#define TRAFFIC_MSK31_1_ptr	440
#define TRAFFIC_MSK31_0_ptr	441

//next block = 442 - 784
*/
#define STARTSTOP_ptr		 442				// have this at end so all setup crap is executed 1st!
#define ConfigChecksum_spare 443

//#define ConfigChecksum2		(MaxConfig2-1)


//*** END of GIGE Config data *******
/**************************************************************
********* STATUS Bytes from GIGE Module to GUI ***************
**************************************************************/
#define MaxStatus 			MaxConfig+125
#define StatusChecksum_ptr	MaxStatus-1

#define MaxStatus2 			MaxConfig2+460
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
#define MISC3_ptr					MaxConfig2+35	// need for 10G A/B MISC?
#define MISC2_ptr					MaxConfig2+36
#define MISC1_ptr					MaxConfig2+37
#define MISC0_ptr					MaxConfig2+38

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
#define TRAFFIC_NUM_STREAM_PACKET10_ptr		MaxConfig2+276
#define TRAFFIC_NUM_STREAM_PACKET03_ptr	 	MaxConfig2+277 //lower 32 bits
#define TRAFFIC_NUM_STREAM_PACKET02_ptr		MaxConfig2+278
#define TRAFFIC_NUM_STREAM_PACKET01_ptr		MaxConfig2+279
#define TRAFFIC_NUM_STREAM_PACKET00_ptr		MaxConfig2+280

// total number of frames actually sent (by reading GEN)
#define TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr		MaxConfig2+281 //MSB
#define TRAFFIC_GEN_TX_PKT_SENT_CNT10_ptr		MaxConfig2+282
#define TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr		MaxConfig2+283 //LSB
#define TRAFFIC_GEN_TX_PKT_SENT_CNT02_ptr		MaxConfig2+284
#define TRAFFIC_GEN_TX_PKT_SENT_CNT01_ptr		MaxConfig2+285
#define TRAFFIC_GEN_TX_PKT_SENT_CNT00_ptr		MaxConfig2+286

// number of frames actually rcvd by reading MON
#define TRAFFIC_MON_NUM_RCVD_PACKET11_ptr		MaxConfig2+287 //extra
#define TRAFFIC_MON_NUM_RCVD_PACKET10_ptr		MaxConfig2+288
#define TRAFFIC_MON_NUM_RCVD_PACKET03_ptr	 	MaxConfig2+289 //lower 32 bits
#define TRAFFIC_MON_NUM_RCVD_PACKET02_ptr		MaxConfig2+290
#define TRAFFIC_MON_NUM_RCVD_PACKET01_ptr		MaxConfig2+291	//unused in UI
#define TRAFFIC_MON_NUM_RCVD_PACKET00_ptr		MaxConfig2+292

#define TRAFFIC_MON_RX_CTRL_STAT03_ptr			MaxConfig2+293	// receive_ctrl_status
#define TRAFFIC_MON_RX_CTRL_STAT02_ptr			MaxConfig2+294
#define TRAFFIC_MON_RX_CTRL_STAT01_ptr			MaxConfig2+295
#define TRAFFIC_MON_RX_CTRL_STAT00_ptr			MaxConfig2+296

		#define TRAFFIC_MON_START_BIT	                     	0x01 // start&clr bit
		#define TRAFFIC_MON_RECEIVE_START_AND_CLEAR_CNTRS_BIT	0x01 // start&clr bit
		#define TRAFFIC_MON_RECEIVE_STOP_BIT					0x02 // STOP bit
		#define TRAFFIC_MON_RECEIVE_CONTROL_DONE_STATUS_BIT		0x04 // DONE bit
		#define TRAFFIC_MON_RECEIVE_ERROR_STATUS_BIT			0x08 // rcv error flag
		// TRAFFIC_RX_CTRL_STAT_CODE		4:9			6-bit:  rcv error code
		//#define ERROR_CODE_MASK									0x03F0
		// TRAFFIC_RX_CTRL_SPARE			10:14		6-bit: spare - not used
		#define TRAFFIC_MON_ACTIVE_BIT                          0x0008000
		#define TRAFFIC_MON_RECEIVE_CONTINUOUS_BIT				0x0010000 // RCV Continuous (^16)
		#define TRAFFIC_MON_RESET_BIT	                        0x80000000 //RESET bit


#define TRAFFIC_MON_CYCLE_RX_CNT21_ptr			MaxConfig2+297//extra	This value represents total time to rcv all packets (for rcvd rate calculations)
#define TRAFFIC_MON_CYCLE_RX_CNT20_ptr			MaxConfig2+298//extra
#define TRAFFIC_MON_CYCLE_RX_CNT13_ptr			MaxConfig2+299
#define TRAFFIC_MON_CYCLE_RX_CNT12_ptr			MaxConfig2+300
#define TRAFFIC_MON_CYCLE_RX_CNT11_ptr			MaxConfig2+301
#define TRAFFIC_MON_CYCLE_RX_CNT10_ptr			MaxConfig2+302
#define TRAFFIC_MON_CYCLE_RX_CNT03_ptr			MaxConfig2+303
#define TRAFFIC_MON_CYCLE_RX_CNT02_ptr			MaxConfig2+304
#define TRAFFIC_MON_CYCLE_RX_CNT01_ptr			MaxConfig2+305
#define TRAFFIC_MON_CYCLE_RX_CNT00_ptr			MaxConfig2+306


#define TRAFFIC_MON_PKT_RX_MAC_ERR_CNT3_ptr		MaxConfig2+307				// SPARE
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

#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR13_ptr	MaxConfig2+319				// count of packets with a timeout error
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

#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR13_ptr  MaxConfig2+335				// count of packets with multiple stream header matches
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR12_ptr  MaxConfig2+336
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR11_ptr  MaxConfig2+337
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR10_ptr  MaxConfig2+338
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR03_ptr	 MaxConfig2+339
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR02_ptr	 MaxConfig2+340
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR01_ptr	 MaxConfig2+341
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR00_ptr	 MaxConfig2+342

#define TRAFFIC_MON_RX_NUM_PKT_LEFT13_ptr    MaxConfig2+343 				// NUM of PKTS the MON still waiting to RCV
#define TRAFFIC_MON_RX_NUM_PKT_LEFT12_ptr    MaxConfig2+344
#define TRAFFIC_MON_RX_NUM_PKT_LEFT11_ptr    MaxConfig2+345
#define TRAFFIC_MON_RX_NUM_PKT_LEFT10_ptr    MaxConfig2+346
#define TRAFFIC_MON_RX_NUM_PKT_LEFT03_ptr    MaxConfig2+347
#define TRAFFIC_MON_RX_NUM_PKT_LEFT02_ptr    MaxConfig2+348
#define TRAFFIC_MON_RX_NUM_PKT_LEFT01_ptr    MaxConfig2+349
#define TRAFFIC_MON_RX_NUM_PKT_LEFT00_ptr    MaxConfig2+350

#define TRAFFIC_MON_STREAM_KEY3_ptr				MaxConfig2+351	 	// stream key, which stream is the nios accessing
#define TRAFFIC_MON_STREAM_KEY2_ptr				MaxConfig2+352
#define TRAFFIC_MON_STREAM_KEY1_ptr				MaxConfig2+353
#define TRAFFIC_MON_STREAM_KEY0_ptr				MaxConfig2+354


					#define STREAM_KEY_INDEX_MASK		0x000000FF		//  8-bit: Stream index for HDR field
					#define STREAM_KEY_SPARE			0x00FFFF00		//16-bit: spare - not used
					#define STREAM_KEY_NUM_STREAMS_MASK	0xFF000000		//  8-bit: number of Matchers synthesized (read only)

#define TRAFFIC_MON_BROADCAST_RX21_ptr			MaxConfig2+355 //extra
#define TRAFFIC_MON_BROADCAST_RX20_ptr			MaxConfig2+356
#define TRAFFIC_MON_BROADCAST_RX13_ptr			MaxConfig2+357		//unused in UI
#define TRAFFIC_MON_BROADCAST_RX12_ptr			MaxConfig2+358
#define TRAFFIC_MON_BROADCAST_RX11_ptr			MaxConfig2+359
#define TRAFFIC_MON_BROADCAST_RX10_ptr			MaxConfig2+360
#define TRAFFIC_MON_BROADCAST_RX03_ptr			MaxConfig2+361
#define TRAFFIC_MON_BROADCAST_RX02_ptr			MaxConfig2+362
#define TRAFFIC_MON_BROADCAST_RX01_ptr			MaxConfig2+363
#define TRAFFIC_MON_BROADCAST_RX00_ptr			MaxConfig2+364

#define TRAFFIC_MON_PKT_RX_MAC_ERR21_ptr		MaxConfig2+365
#define TRAFFIC_MON_PKT_RX_MAC_ERR20_ptr		MaxConfig2+366
#define TRAFFIC_MON_PKT_RX_MAC_ERR13_ptr		MaxConfig2+367	//		 TRAFFIC_MON_PKT_RX_MAC_ERR1
#define TRAFFIC_MON_PKT_RX_MAC_ERR12_ptr		MaxConfig2+368
#define TRAFFIC_MON_PKT_RX_MAC_ERR11_ptr		MaxConfig2+369
#define TRAFFIC_MON_PKT_RX_MAC_ERR10_ptr		MaxConfig2+370
#define TRAFFIC_MON_PKT_RX_MAC_ERR03_ptr		MaxConfig2+371  //		TRAFFIC_MON_PKT_RX_MAC_ERR
#define TRAFFIC_MON_PKT_RX_MAC_ERR02_ptr		MaxConfig2+372
#define TRAFFIC_MON_PKT_RX_MAC_ERR01_ptr		MaxConfig2+373
#define TRAFFIC_MON_PKT_RX_MAC_ERR00_ptr		MaxConfig2+374

// UI: in quick/RFC test this value/num_frames = avg latency
//OR....UI update: use min, max and avg values for latency
// always use this for avg latency
//and min & max for min & max.  Sinc ethis is how this data will be reported in Y.1564
#define TRAFFIC_MON_DELAY_SUM23_ptr       		MaxConfig2+375		 //     read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM2);//packet delay sum MSB
#define TRAFFIC_MON_DELAY_SUM22_ptr				MaxConfig2+376
#define TRAFFIC_MON_DELAY_SUM21_ptr				MaxConfig2+377
#define TRAFFIC_MON_DELAY_SUM20_ptr				MaxConfig2+378
#define TRAFFIC_MON_DELAY_SUM13_ptr       		MaxConfig2+379		 //     read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM1);//  packet delay sum MSB
#define TRAFFIC_MON_DELAY_SUM12_ptr				MaxConfig2+380
#define TRAFFIC_MON_DELAY_SUM11_ptr				MaxConfig2+381
#define TRAFFIC_MON_DELAY_SUM10_ptr				MaxConfig2+382
#define TRAFFIC_MON_DELAY_SUM03_ptr        		MaxConfig2+383		  //    read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM0); ///packet delay sum LSB
#define TRAFFIC_MON_DELAY_SUM02_ptr				MaxConfig2+384
#define TRAFFIC_MON_DELAY_SUM01_ptr				MaxConfig2+385
#define TRAFFIC_MON_DELAY_SUM00_ptr				MaxConfig2+386

#define TRAFFIC_MON_DELAY_MIN_ptr	MaxConfig2+387	 //spare
#define TRAFFIC_MON_DELAY_MIN03_ptr 	MaxConfig2+388	 //Results for threshold comparison
#define TRAFFIC_MON_DELAY_MIN02_ptr 	MaxConfig2+389
#define TRAFFIC_MON_DELAY_MIN01_ptr 	MaxConfig2+390
#define TRAFFIC_MON_DELAY_MIN00_ptr 	MaxConfig2+391

#define TRAFFIC_MON_DELAY_MAX_ptr 		MaxConfig2+392	 //spare
#define TRAFFIC_MON_DELAY_MAX03_ptr 	MaxConfig2+393	 //Results for threshold comparison
#define TRAFFIC_MON_DELAY_MAX02_ptr 	MaxConfig2+394
#define TRAFFIC_MON_DELAY_MAX01_ptr 	MaxConfig2+395
#define TRAFFIC_MON_DELAY_MAX00_ptr		MaxConfig2+396

#define TRAFFIC_MON_RX_BYTE_CNT21_ptr	MaxConfig2+397		//spare
#define TRAFFIC_MON_RX_BYTE_CNT20_ptr	MaxConfig2+398		//UI uses this to calculate throughput rate ((tot_bytes*8+OVH)/time_base)
#define TRAFFIC_MON_RX_BYTE_CNT13_ptr	MaxConfig2+399		//* 64-bits: 1=Hi32,0=Lo32 //TRAFFIC_MON_BYTE_RX_CNT1
#define TRAFFIC_MON_RX_BYTE_CNT12_ptr	MaxConfig2+400
#define TRAFFIC_MON_RX_BYTE_CNT11_ptr	MaxConfig2+401
#define TRAFFIC_MON_RX_BYTE_CNT10_ptr	MaxConfig2+402
#define TRAFFIC_MON_RX_BYTE_CNT03_ptr	MaxConfig2+403
#define TRAFFIC_MON_RX_BYTE_CNT02_ptr	MaxConfig2+404
#define TRAFFIC_MON_RX_BYTE_CNT01_ptr	MaxConfig2+405
#define TRAFFIC_MON_RX_BYTE_CNT00_ptr	MaxConfig2+406

#define TRAFFIC_MON_RX_PKT_OK_CNT13_ptr	MaxConfig2+407		//* TRAFFIC_MON_PKT_RX_OK
#define TRAFFIC_MON_RX_PKT_OK_CNT12_ptr	MaxConfig2+408
#define TRAFFIC_MON_RX_PKT_OK_CNT11_ptr	MaxConfig2+409		////previously EMON_PKT_RX_OK_CNT
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

#define SDT_SHORTEST_TIME1_ptr	MaxConfig2+425		//Service Distruption Time (?)  in Seconds?
#define SDT_SHORTEST_TIME0_ptr	MaxConfig2+426		// Y.1564 used for Availability:
//Service Disruption Time, prelim_DEF: time in sec's where InfoRate is below that specified
//Availability = total_time_of_test-SDT

#define SDT_LONGEST_TIME1_ptr	MaxConfig2+427		//
#define SDT_LONGEST_TIME0_ptr	MaxConfig2+428

#define SDT_AVG_TIME1_ptr	MaxConfig2+429			// don't know if this is something the GUI can calc
#define SDT_AVG_TIME0_ptr	MaxConfig2+430			// ...or it must be done real-time

#define SDT_TOTAL_TIME3_ptr	MaxConfig2+431			//
#define SDT_TOTAL_TIME2_ptr	MaxConfig2+432
#define SDT_TOTAL_TIME1_ptr	MaxConfig2+433
#define SDT_TOTAL_TIME0_ptr	MaxConfig2+434

//spare for loopback
#define LOOPBACKSTAT_ptr23            MaxConfig2+435
#define LOOPBACKSTAT_ptr22            MaxConfig2+436
#define LOOPBACKSTAT_ptr21            MaxConfig2+437
#define LOOPBACKSTAT_ptr20            MaxConfig2+438

#define LOOPBACKSTAT_ptr13            MaxConfig2+439
#define LOOPBACKSTAT_ptr12            MaxConfig2+440
#define LOOPBACKSTAT_ptr11            MaxConfig2+441
#define LOOPBACKSTAT_ptr10            MaxConfig2+442

#define LOOPBACKSTAT_ptr03            MaxConfig2+443
#define LOOPBACKSTAT_ptr02            MaxConfig2+444
#define LOOPBACKSTAT_ptr01            MaxConfig2+445
#define LOOPBACKSTAT_ptr00            MaxConfig2+446

#define LOOPBACKSTAT_ptr          	  MaxConfig2+447

#define SPARE_ptr_13	MaxConfig2+448 //global byte cnt?
#define SPARE_ptr_14	MaxConfig2+449
#define SPARE_ptr_15	MaxConfig2+450
#define SPARE_ptr_16	MaxConfig2+451
#define SPARE_ptr_17	MaxConfig2+452
#define SPARE_ptr_18	MaxConfig2+453
#define SPARE_ptr_19	MaxConfig2+454
#define SPARE_ptr_20	MaxConfig2+455
#define SPARE_ptr_21	MaxConfig2+456
#define SPARE_ptr_22	MaxConfig2+457
#define SPARE_ptr_23	MaxConfig2+458

#define StatusChecksum2						MaxConfig2+(MaxStatus2-1) //459	//*
/*****************************************************************
	SPECIFIC BIT definitions
*****************************************************************/
#define LED_ON	0
#define LED_OFF	1




//..GIGE-MODE,SPEED.................
//#define mode_copper	0x00	// Mode bit.0
//#define mode_fiber	0x01

#define ModeCopper10M 	0	// new Mode
#define ModeCopper100M  1
#define ModeCopper1G 	2
#define ModeFiber1G 	3
#define ModeFiber10G 	4
#define ModeFiber100M 	5
#define ModeFiber100G 	6

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

/*********************************************************
***************************************************************
** GIGE-A&B: MODULE DEFINITIONS For DIRTY/Update bit handling *
***************************************************************/
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
#define ReSyncFlagC37	(ConfigStatC37[GIGEconfig_ptr]&0x10)	// Send Re-Sync LINK flag to PDA on PWR cycle/link lost
#define ValidConfig		(RxBuffer[GIGEconfig_ptr]&0xC0)

//======= TEST FLAGS/STATES ===============
#define RxTestStartGuiBit	(RxBuffer[STARTSTOP_ptr]&0x10)		// Set/Clr and sent by GUI	(GUI cmd 'START TEST')
																// START: |=0x10  STOP: &=0x10
#define RxTestStartToggle	(RxBuffer[STARTSTOP_ptr]&0x01)		// GUI toggles to MOD to kick consecutive tests

#define RxTestDoneBit		(RxBuffer[STARTSTOP_ptr]&0x40)

#define TestStartGuiBit		(ConfigStat[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)
#define TestStartGuiBitB	(ConfigStatB[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)
#define TestStartGuiBit10	(ConfigStat10G[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)

#define TestStartModBit		(ConfigStat[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)
#define TestStartModBitB	(ConfigStatB[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)
#define TestStartModBit10	(ConfigStat10G[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)

#define TestStoppedModBit	(ConfigStat[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)
#define TestStoppedModBitB	(ConfigStatB[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)
#define TestStoppedModBit10	(ConfigStat10G[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)

#define RxDONEdetected       (RxBuffer[STARTSTOP_ptr]&0x40)            // GUI sets DONE as ACK to FW that DONE was detected by the GUI

//#define TestStoppedModBit10B (ConfigStat10GB[STARTSTOP_ptr]&0x40)	// Set/Clr and sent by MOD 	(test has stopped/DONE)
#define gOPT_INSTALLED_dual1g	((OPTIONS_INSTALLED&0x01)==0)
#define gOPT_INSTALLED_a10g		((OPTIONS_INSTALLED&0x02)==0)
#define gOPT_INSTALLED_b10g		((OPTIONS_INSTALLED&0x04)==0)

#define gOPT_ENABLED_dual1g	((OPTIONS_ENABLED&0x01)==0)
#define gOPT_ENABLED_a10g	((OPTIONS_ENABLED&0x02)==0)
#define gOPT_ENABLED_b10g	((OPTIONS_ENABLED&0x04)==0)





//************************************************************************
//	C37.94	DEFINITIONS
//************************************************************************
#define	C3794_READY		(C3794_Status&0x80)

/*********************************************************************/
/********** Configuration Bytes from UI to C37.94 "Module" ***********/
/*********************************************************************/
#define C37config_ptr		0
#define ID_ptr 			    1	// Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
#define TEST_ptr 			2	//
#define AUTO_ptr    		3	// Auto/Stress Test
#define MODE_ptr 			4	//
#define CLOCK_ptr         	5	// ^4:CLK_MODE: 0=NW/recovered, 1=Internal OSC,  ^3:OFFSET_POL: 0=Pos, 1=Neg
#define CLK_PPM1_ptr		6	// signed short 2's complement (truncated to 9-bits in FPGA)
#define CLK_PPM0_ptr		7	//
#define CLK_SPR_ptr			8	// Spare
#define TX_CHANS_ptr    	9
#define RX_CHANS_ptr    	10
#define OVH_errors_ptr		11	// insert OVH errors: F-bits/BPV's, etc.
#define Lpbk_ptr      		12	// Loopback Up/Down


#define BERTC37_ptr 		13	// pattern
#define CLEARC37_ptr    	14	// RESTART^0, clear gTimesUp flag^1, PDA_ACK^7 to LOCAL_CHG (misc_stat^6)
#define MISCC37_ptr     	15	// Insert BERT err^8, ^3;0x08=INVERT_BERT
#define BER_ptr				16	// set BER
#define UserSize_ptr		17	// User BERT pattern size (1-32 bits)
#define USER3_ptr  	   		18	// SAVE & Execute USER BERT Pattern
#define USER2_ptr    		19
#define USER1_ptr   		20
#define USER0_ptr   		21

#define ConfigChecksum		74

/****************************************************************/
/********* STATUS Bytes from C37.94 "Module" to UI **************/
/****************************************************************/
//#define MaxStatus 			MaxConfig+125
//#define StatusChecksum_ptr	MaxStatus-1

#define C37status_ptr 	MaxConfig+0 // LOW_BAT^8
#define C37TYPE_ptr		MaxConfig+1	// type 0: Basic DS1 test set
#define C37VER1_ptr		MaxConfig+2	// ver1.ver0  e.g.  0.0 to 255.255
#define C37VER0_ptr		MaxConfig+3	// current FW revision
#define MOD_STATUS1_ptr	MaxConfig+4	//
#define MOD_STATUS0_ptr	MaxConfig+5	//
#define OVH_STATUS_ptr  MaxConfig+6	// ALARMS & Events
#define LPBK_STATE_ptr	MaxConfig+7

#define FREQ1_ptr 		MaxConfig+12	// Rcvd FREQ & LEVEL
#define FREQ0_ptr 		MaxConfig+13
#define LEVEL1_ptr		MaxConfig+14
#define LEVEL0_ptr 		MaxConfig+15
#define DELAY3_ptr		MaxConfig+16  	// 32-bit DS0 byte transmission delay
#define DELAY2_ptr		MaxConfig+17  	// 24-bit DS0 byte transmission delay
#define DELAY1_ptr		MaxConfig+18  	// 16-bit DS0 byte transmission delay
#define DELAY0_ptr		MaxConfig+19	//

#define LOS_COUNT1_ptr  MaxConfig+28	// Alarms
#define LOS_COUNT0_ptr  MaxConfig+29
#define LOS_SEC1_ptr  	MaxConfig+30
#define LOS_SEC0_ptr  	MaxConfig+31
#define OOF_COUNT1_ptr  MaxConfig+32
#define OOF_COUNT0_ptr  MaxConfig+33
#define OOF_SEC1_ptr  	MaxConfig+34
#define OOF_SEC0_ptr  	MaxConfig+35
#define YEL_COUNT1_ptr  MaxConfig+36
#define YEL_COUNT0_ptr  MaxConfig+37
#define YEL_SEC1_ptr 	MaxConfig+38
#define YEL_SEC0_ptr 	MaxConfig+39
#define FBIT_COUNT3_ptr MaxConfig+40	// EVENTS
#define FBIT_COUNT2_ptr MaxConfig+41
#define FBIT_COUNT1_ptr MaxConfig+42
#define FBIT_COUNT0_ptr	MaxConfig+43
#define CODE_COUNT3_ptr MaxConfig+44
#define CODE_COUNT2_ptr MaxConfig+45
#define CODE_COUNT1_ptr MaxConfig+46
#define CODE_COUNT0_ptr	MaxConfig+47


#define BSR_ptr			MaxConfig+50	// BERT Status (SYNC, etc)
#define SBERT_ptr       MaxConfig+51	// Sequence/Auto BERT set by FW to notify UI
#define BECR3_ptr		MaxConfig+52
#define BECR2_ptr		MaxConfig+53
#define BECR1_ptr  		MaxConfig+54
#define BECR0_ptr  		MaxConfig+55
#define CODE_ES1_ptr  	MaxConfig+56	//ES & SES
#define CODE_ES0_ptr  	MaxConfig+57
#define FBIT_ES1_ptr  	MaxConfig+58
#define FBIT_ES0_ptr  	MaxConfig+59
#define CODE_SES1_ptr  	MaxConfig+60
#define CODE_SES0_ptr  	MaxConfig+61
#define FBIT_SES1_ptr  	MaxConfig+62
#define FBIT_SES0_ptr  	MaxConfig+63
#define BERT_ES1_ptr  	MaxConfig+64
#define BERT_ES0_ptr  	MaxConfig+65
#define BERT_SES1_ptr  	MaxConfig+66
#define BERT_SES0_ptr  	MaxConfig+67

#define UAS1_ptr		MaxConfig+72	// UAS
#define UAS0_ptr		MaxConfig+73
#define C37SECONDS3_ptr	MaxConfig+74	// ELAP or TEST SECONDS
#define C37SECONDS2_ptr MaxConfig+75
#define C37SECONDS1_ptr MaxConfig+76
#define C37SECONDS0_ptr MaxConfig+77

typedef enum {
	Poly511,
	Poly2047,
	Poly2e15,

	Poly2e20,
	Poly2e23,

	PolyQRSS,

	Poly63,
	PolyV54,

	AIS,       // all 1's
	A0S,       // all 0's
	Alt,       // Alternating 1010...
	ThreeIn24, // 0010_0000_0000_0000_0010_0010
	OneIn8,    // 0000_0001  same as 1:7
	TwoIn8,    // 0100_0010

	OneIn5,    // 0100_0010 0x00000010
	OneIn6     // 0100_0010 0x00000020
} BERT_Patterns;

/********************************/
/* BERT definitions			  */
/********************************/
#define SYNC	(BERT_STATUS&0x01)   // 2172 BERT status register
#define RLOS	(BERT_STATUS&0x10)
#define RA1A0	(BERT_STATUS&0x60)
#define RALL1	(BERT_STATUS&0x40)
#define RALL0	(BERT_STATUS&0x20)
#define SYNCINCNT	3
#define SYNCOUTCNT	3

// C37.94 Frame: 256 bits  256bits at 8kHz rate = 2.048Mbps  256/8=32bytes total
// Header	16bits		abcdefgh,00001111
//		abcdefg alternate between 1001,1011 and 11y1,1111 where y=Yellow Alarm: 0=normal, 1= badRcvSig (declare LOS)
// yellow==RDI: when rcvd means the remote end is seeing BAD or no signal.
// when we see Bad Signal or LOS we send yellow to remote and sends AIS toward CPE (Chan Data filled with All-1's)

// Overhead	48bits		ppqqrrss,10101010,10101010,10101010,10101010,10101010
//		24 actual bits, (each is followed by it's complement)
//		pqrs			# chans (N) used: N=1:0001, N=3:0011, N=12:1100

// Channel 192bits		D1D1 D2D2,D3D3 D4D4, ....D96D96
//		96 actual bits, (each is followed by it's complement)  N=12chans*8bits = 96 bits (12bytes) + 96 complements (12bytes)=192 total (24bytes)
//		so in Raw form each 8 bits of channel data occupy 16 bits: if 1stChan data is 0x1110,1100 on the line it's: 10101001,10100101
//	IDLE chans are filled with All-1's  so that's 10101010,10101010  (All1's followed by their complements)
//
#define HDR1_ptr 		MaxConfig+80	// 256 bits (32bytes) of Raw data as its rcvd
#define HDR2_ptr 		MaxConfig+81	//
#define OVH1_ptr		MaxConfig+82
#define OVH2_ptr		MaxConfig+83
#define OVH3_ptr    	MaxConfig+84
#define OVH4_ptr    	MaxConfig+85
#define OVH5_ptr    	MaxConfig+86
#define OVH6_ptr     	MaxConfig+87
#define CH1A_ptr    	MaxConfig+88
#define CH1B_ptr     	MaxConfig+89
#define CH2A_ptr     	MaxConfig+90
#define CH2B_ptr     	MaxConfig+91	//
#define CH3A_ptr 		MaxConfig+92	//
#define CH3B_ptr 		MaxConfig+93	//
#define CH4A_ptr		MaxConfig+94
#define CH4B_ptr		MaxConfig+95
#define CH5A_ptr    	MaxConfig+96
#define CH5B_ptr    	MaxConfig+97
#define CH6A_ptr    	MaxConfig+98
#define CH6B_ptr     	MaxConfig+99
#define CH7A_ptr     	MaxConfig+100
#define CH7B_ptr     	MaxConfig+101
#define CH8A_ptr     	MaxConfig+102
#define CH8B_ptr     	MaxConfig+103
#define CH9A_ptr     	MaxConfig+104
#define CH9B_ptr     	MaxConfig+105
#define CH10A_ptr     	MaxConfig+106
#define CH10B_ptr     	MaxConfig+107
#define CH11A_ptr     	MaxConfig+108
#define CH11B_ptr     	MaxConfig+109
#define CH12A_ptr     	MaxConfig+110
#define CH12B_ptr     	MaxConfig+111


#define DEBUG_ptr			MaxConfig+120	// DEBUG spot
#define AUTO_RESULT1_ptr	MaxConfig+121
#define AUTO_RESULT0_ptr	MaxConfig+122	// MACRO/AUTO/STRESS TEST result codes
#define MISC_STATC37_ptr	MaxConfig+123	// Bleep^0, Error^1, History^2, tweedle^3, TimesUP^4, FAIL^5^, LOCAL_CHG^6
#define MaxStatus 			MaxConfig+125

/** UI items....
#define gBleep   (gStatus[MISC_STAT_ptr]&0x01)	// ^0 Bleeeeeep....
#define gErrors  (gStatus[MISC_STAT_ptr]&0x02)	// ^1 ERRORS LED
#define gHistory (gStatus[MISC_STAT_ptr]&0x04)	// ^2 HISTORY LED
#define gTweedle (gStatus[MISC_STAT_ptr]&0x08)	// ^3 Lpbk/Sync sound....
#define gTimesUp (gStatus[MISC_STAT_ptr]&0x10)	// ^4 Timed test IVL complete!
#define gBtapFail (gStatus[MISC_STAT_ptr]&0x20)	// ^5 BTAP failed!
// LocalChange (gStatus[MISC_STAT_ptr]&0x40)    // ^6 Local Change (in BRICK)
#define gUas     gStatus[MISC_STAT_ptr]&0x80)	// ^7 in a UAS condition

#define gClear 	 	(gStatus[DS1CLEAR_ptr])			// RESTART ERRORs = 0x01

#define gLowBat  (gStatus[GB101status_ptr]&0x80)	// Low Battery Warning....

#define	gLosCnt	 (BytesToInt(gStatus,LOS_COUNT1_ptr))
#define	gOofCnt	 (BytesToInt(gStatus,OOF_COUNT1_ptr))
#define	gAisCnt	 (BytesToInt(gStatus,AIS_COUNT1_ptr))
#define	gYelCnt  (BytesToInt(gStatus,YEL_COUNT1_ptr))

#define	gBpvCnt	 (BytesToLong(gStatus,BPV_COUNT3_ptr))
#define	gCrcCnt  (BytesToLong(gStatus,CRC_COUNT3_ptr))
#define	gFbitCnt (BytesToLong(gStatus,FBIT_COUNT3_ptr))
#define	gDen1Cnt (BytesToLong(gStatus,Den1_COUNT3_ptr))
#define	gExzCnt	 (BytesToLong(gStatus,EXZ_COUNT3_ptr))

#define	gLosSec	 (BytesToInt(gStatus,LOS_SEC1_ptr))
#define	gOofSec  (BytesToInt(gStatus,OOF_SEC1_ptr))
#define	gAisSec	 (BytesToInt(gStatus,AIS_SEC1_ptr))
#define	gYelSec  (BytesToInt(gStatus,YEL_SEC1_ptr))
#define	gUasSec	 (BytesToInt(gStatus,UAS1_ptr))

#define	gBpvES	 (BytesToInt(gStatus,BPV_ES1_ptr))
#define	gCrcES   (BytesToInt(gStatus,CRC_ES1_ptr))
#define	gFbitES  (BytesToInt(gStatus,FBIT_ES1_ptr))
//#define	gExzES	 (BytesToInt(gStatus,EXZ_ES1_ptr))

#define	gBpvSES	 (BytesToInt(gStatus,BPV_SES1_ptr))
#define	gCrcSES  (BytesToInt(gStatus,CRC_SES1_ptr))
#define	gFbitSES (BytesToInt(gStatus,FBIT_SES1_ptr))

#define	gPatCnt  (BytesToLong(gStatus,BECR3_ptr))		// BERT ERROR CNT
#define	gPatES	 (BytesToInt(gStatus,BERT_ES1_ptr))
#define	gPatSES	 (BytesToInt(gStatus,BERT_SES1_ptr))

#define	gSECONDS (BytesToLong(gStatus,SECONDS3_ptr))
**/

#endif /* CONFSTAT_H_*/
/************************************************************************
* Copyright (c) 2017 Greenlee Communications Vista, CA USA.    			*
* All rights reserved.                                                  *
************************************************************************/

