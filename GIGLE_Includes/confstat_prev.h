
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
#define Packet_DS3		0x00
#define Packet_DS1A		0x01
#define Packet_DS1B		0x02
#define Packet_DS0		0x03
#define Packet_PRI		0x04
#define Packet_TIMS		0x05
#define Packet_DDS		0x06
#define Packet_DS3_2	0x07
#define Packet_TIME		0x08
#define Packet_E1A		0x09
#define Packet_E1B		0x0A
#define Packet_E3		0x0B
#define Packet_E3_2		0x0C
#define Packet_DCOM		0x0D

#define Packet_SEEPROMT	0x0E
#define Packet_SEEPROM	0x0F

//#define Packet_AUTOTEST	0x80	// OR'd with Packet_TYPE to define AUTO TEST PACKET!
#define Packet_DIGITS	0x10	// Async packet sent to GUI when new DIGITs received

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
#define HW_VERSION_ptr		400	// rev1 to rev255
#define FW_VERSION1_ptr		401	// ver1.ver0 0 -> 65535,  65535/1000 = v0.0 to v65.535
#define FW_VERSION0_ptr		402	// v1.01 -> 101
// these on down are SW configurable....so are stored in and read from Flash
#define FLASH_OPT_INSTALLED_ptr	403 // ^0=dual-1g, ^1=10G-A, ^2=10G-B, (?) THRU,VLAN,IP,OAM
#define FLASH_OPT_ENABLED_ptr	404 // All ACTIVE LOW! FF=nothing installed, FF=nothing enabled

// sernum not used now - just gonna use 0000-FFFF from lower MAC addresses as SN#!
// ...so these can be re-allocated at some point!
#define SERNUM4_ptr			405 // Serial NUM 00000 -> FFFF0 (1048560)
#define SERNUM3_ptr			406 // 00000 & FFFFF reserved
#define SERNUM2_ptr			407
#define SERNUM1_ptr			408
#define SERNUM0_ptr			409

#define MACA_ADDR_MSB3_ptr	410	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MACA_ADDR_MSB2_ptr	411	// these are 0x001C23174ACB
#define MACA_ADDR_MSB1_ptr	412	// loaded into mac_0 = 0x17231C00  (reverse order)
#define MACA_ADDR_MSB0_ptr	413  //    and into mac_1 = 0x0000CB4A
#define MACA_ADDR_LSB1_ptr	414
#define MACA_ADDR_LSB0_ptr	415

#define MACB_ADDR_MSB3_ptr	416	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MACB_ADDR_MSB2_ptr	417	// these are 0x001C23174ACB
#define MACB_ADDR_MSB1_ptr	418	// loaded into mac_0 = 0x17231C00  (reverse order)
#define MACB_ADDR_MSB0_ptr	419  //    and into mac_1 = 0x0000CB4A
#define MACB_ADDR_LSB1_ptr	420
#define MACB_ADDR_LSB0_ptr	421

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
#define FlashParameterSize		250 //really the block/sector is 65536 (0-FFFF)
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

#define PARA_LENGTH				34

/****************************************************************/
/********** Configuration Bytes from GUI to GIGE Module *********/
/****************************************************************/
#define MaxConfig 			75
#define MaxConfig2 			150

//#define ConfigChecksum_ptr	MaxConfig-1 // NOT USED, gigE doesn't look at this!
#define ConfigChecksum2_ptr	MaxConfig2-1

#define GIGEconfig_ptr 		0
#define ID_ptr		    	1   // Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
#define GIGEMODE_ptr		2	// ^0:Copper/Fiber 0=copper,1=fiber, //not used:^1&2=speed:00=10,01=100,10=1G,11=10G
//#define SYSCONFIG_ptr  		3

		//=================== MAC CONFIG/CONTROL SETTINGS ======
#define MAC_CONFIG3_ptr  	4	// MAC COMMAND-CONFIG Register
#define MAC_CONFIG2_ptr    	5
#define MAC_CONFIG1_ptr   	6
#define MAC_CONFIG0_ptr   	7
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

#define MAC_FRAME_LEN1_ptr	8	// ...also used by EGEN (pkt_length) EGEN_PKT_LENTYPE1_ptr
#define MAC_FRAME_LEN0_ptr  9	// The maximum Frame Length the TSE uses to chk rcvd frames

#define MAC_PAUSEQUANT1_ptr 10	// FLOW CONTROL - PAUSE DURATION
#define MAC_PAUSEQUANT0_ptr 11

#define MAC_IPG_LENGTH_ptr	12	// Inter-Packet/Frame Gap size (in bytes)  VALID:8 to 26, 12 is default as typ min IPG
#define MAC_MISC_ptr		13	// Misc control flags RESET_MAC, OMIT_CRC, TX_SHIFT16, RX_SHIFT16, ...

#define MAC_ADDR_MSB3_ptr	14	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
#define MAC_ADDR_MSB2_ptr	15	// these are 0x001C23174ACB
#define MAC_ADDR_MSB1_ptr	16	// loaded into mac_0 = 0x17231C00  (reverse order)
#define MAC_ADDR_MSB0_ptr	17  //    and into mac_1 = 0x0000CB4A
#define MAC_ADDR_LSB1_ptr	18
#define MAC_ADDR_LSB0_ptr	19

#define MAC_MULTICASTHASH_MSB3_ptr	20	// Multicast Hash table MSB (each bit goes into bit.0 of the respective Hash register)
#define MAC_MULTICASTHASH_MSB2_ptr	21
#define MAC_MULTICASTHASH_MSB1_ptr	22
#define MAC_MULTICASTHASH_MSB0_ptr	23
#define MAC_MULTICASTHASH_LSB3_ptr	24	// Multicast Hash table LSB (each bit goes into bit.0 of the respective Hash register)
#define MAC_MULTICASTHASH_LSB2_ptr	25
#define MAC_MULTICASTHASH_LSB1_ptr	26
#define MAC_MULTICASTHASH_LSB0_ptr	27

#define MAC_SMAC0_MSB3_ptr	28	// Supplementary MAC ADDR 0 MSB
#define MAC_SMAC0_MSB2_ptr	29	// ...used for filtering
#define MAC_SMAC0_MSB1_ptr	30
#define MAC_SMAC0_MSB0_ptr	31
#define MAC_SMAC0_LSB3_ptr	32	// Supplementary MAC ADDR 0 LSB
#define MAC_SMAC0_LSB2_ptr	33
#define MAC_SMAC0_LSB1_ptr	34
#define MAC_SMAC0_LSB0_ptr	35

#define MAC_SMAC1_MSB3_ptr	36	// Supplementary MAC ADDR 1 MSB
#define MAC_SMAC1_MSB2_ptr	37
#define MAC_SMAC1_MSB1_ptr	38
#define MAC_SMAC1_MSB0_ptr	39
#define MAC_SMAC1_LSB3_ptr	40	// Supplementary MAC ADDR 1 LSB
#define MAC_SMAC1_LSB2_ptr	41
#define MAC_SMAC1_LSB1_ptr	42
#define MAC_SMAC1_LSB0_ptr	43

#define MAC_SMAC2_MSB3_ptr	44	// Supplementary MAC ADDR 2 MSB
#define MAC_SMAC2_MSB2_ptr	45
#define MAC_SMAC2_MSB1_ptr	46
#define MAC_SMAC2_MSB0_ptr	47
#define MAC_SMAC2_LSB3_ptr	48	// Supplementary MAC ADDR 2 LSB
#define MAC_SMAC2_LSB2_ptr	49
#define MAC_SMAC2_LSB1_ptr	50
#define MAC_SMAC2_LSB0_ptr	51

#define MAC_SMAC3_MSB3_ptr	52	// Supplementary MAC ADDR 3 MSB
#define MAC_SMAC3_MSB2_ptr	53
#define MAC_SMAC3_MSB1_ptr	54
#define MAC_SMAC3_MSB0_ptr	55
#define MAC_SMAC3_LSB3_ptr	56	// Supplementary MAC ADDR 3 LSB
#define MAC_SMAC3_LSB2_ptr	57
#define MAC_SMAC3_LSB1_ptr	58
#define MAC_SMAC3_LSB0_ptr	59

		//=================== PHY CONFIG/CONTROL SETTINGS ======
#define PHY_CONFIG1_ptr		60	// 16-bits: RESET, MODE, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG,
#define PHY_CONFIG0_ptr 	61	//			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
#define PHY_SFP_MISC_ptr	62	// ^0:1=SFP_A Transmit_OFF, ^1:1=SFP_B Transmit_OFF, ^2:1=SFP+A Transmit_OFF

//#define PHY_FBR_CONFIG1_ptr	63	// 16-bits: RESET, MODE, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG
//#define PHY_FBR_CONFIG0_ptr 64	// 			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
//#define PHY_FBR_MISC_ptr	65

//#define PHY_LEDSELECT1_ptr	66	// stolen to support SFP A/B power
//#define PHY_LEDSELECT2_ptr	67
//#define PHY_POWER1_ptr		68
//#define PHY_POWER0_ptr		69

//======= SIGNAL ROUTING SETTINGS for EACH PORT ====== (various (TX) MUX and (RX) SPLITTER SETTINGS)
// future fancy-pants SIG routing controls
//#define TSE_A_SIGROUTE_ptr 	66	// bit.01=MUX, bit.45=SPLITTER (port selection 0-3)
//#define TSE_B_SIGROUTE_ptr 	67
//#define A_10G_SIGROUTE_ptr 	68
//#define B_10G_SIGROUTE_ptr 	69
#define CORE_SIGROUTE_MODES_ptr 69	// ^0:0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK), 1=TSE_LANWAN_MODE, ^1:0=10G_NORMAL_MODE,1=10G_LANWAN_MODE (10G+B=LANWAN,A=GEN/MON)


		//=================== MISC SETTINGS ======
#define CLEAR_ptr     		70	// ^0=restart ET, ^1=reset MAC-A stat's, ^2=reset MAC-B stat's
#define MISC_ptr     		71	// DumpSFPAinfo^0, DumpSFPBinfo^1, SFP+A^2, SFP+B^3 	// ^4:0=Looped/1=End-to-End test, ^5=spare, ^6=TSE_B_SWAP_ADDR, ^7=spare
#define BERT_ptr			72

#define FPS_ptr     			73  // save selected FPS - only save integer portion....

#define ConfigChecksum		74	//************************************************************

		//=================== EGEN SETTINGS ======  // note EGEN "pkt_length"   uses MAC "FRM_LENGTH"
#define EGEN_CONTROL_ptr		75	// ^0=ON/OFF, ^1=RANDOM_LEN,  ^2=T1-payload, ^3=payload (0:inc, 1:PRBS),  ^5=ERR_INJ

#define EGEN_NUM_PACKETS3_ptr	76	//
#define EGEN_NUM_PACKETS2_ptr	77
#define EGEN_NUM_PACKETS1_ptr	78	//
#define EGEN_NUM_PACKETS0_ptr	79

#define EGEN_SRCMAC_MSB3_ptr	80	// SRC MAC ADDR
#define EGEN_SRCMAC_MSB2_ptr	81
#define EGEN_SRCMAC_MSB1_ptr	82
#define EGEN_SRCMAC_MSB0_ptr	83

#define EGEN_SRCMAC_LSB1_ptr	84
#define EGEN_SRCMAC_LSB0_ptr	85

#define EGEN_DESTMAC_MSB3_ptr	86	// DEST MAC ADDR
#define EGEN_DESTMAC_MSB2_ptr	87
#define EGEN_DESTMAC_MSB1_ptr	88
#define EGEN_DESTMAC_MSB0_ptr	89

#define EGEN_DESTMAC_LSB1_ptr	90
#define EGEN_DESTMAC_LSB0_ptr	91

#define EGEN_VLAN1_TCI1_ptr 	92	// VLAN-1 TCI: PCP,DEI,ID  (3-bits,1-bit,12-bits)
#define EGEN_VLAN1_TCI0_ptr	93	// if 1-VLAN then 8100+TCI, if stacked-VLAN then 88A8+TCI
#define EGEN_VLAN2_TCI1_ptr	94	// VLAN-2 TCI: PCP,DEI,ID  (3-bits,1-bit,12-bits)
#define EGEN_VLAN2_TCI0_ptr	95	// if 1-VLAN then 8100+TCI

//#define EGEN_FPSPAUSECOUNT3_ptr	96	// %FPS delay time in clks + Frame_Tx_clks
//#define EGEN_FPSPAUSECOUNT2_ptr	97	// ...at current frame & speed settings, subtracting 2 clks from count/S_WAIT process
//#define EGEN_FPSPAUSECOUNT1_ptr	98	//    AND subtracting 2 more clks lost somewhere in the state/counting process. (?)
//#define EGEN_FPSPAUSECOUNT0_ptr	99	// (1 from S_WAIT state + 1 for clock_count ">" pause_delay_count and 2 from "??")

//_______________________________________________________________________________________
//		a	  b		 c		 d		  e		  f		  g		        h		          u         invalid and
//	     64	128		256		512		1024	1280	usr_g	MTU(pktlen)	user_u     terminates_seq
//	      0      1          2          3           4           5          6               7                 8             9-F
//_______________________________________________________________________________________
// psuedo-RANDOM & EMIX:  (up to 32 sizes)  (RNDSEQ0 = 0x01234F00 is a 5-size sequence: 64,128,256,512,1024, F terminates seq)
#define EGEN_EMIXSEQ0_ptr			96	// MSNibble sent 1st   def for Random=0x0123456F
#define EGEN_EMIXSEQ1_ptr			97  //   (good to set 1,2&3 def as 0xF0000000)
#define EGEN_EMIXSEQ2_ptr			98
#define EGEN_EMIXSEQ3_ptr			99 // LSNibble sent last

#define EGEN_PKT_EMIXUSERSIZE_ptr	101 //User Frame Sizes: G=lower-16, U=upper-16 bits
//#define DEF_GEN_EMIXUSER	0x258005EE		// U=2580, G=05EE

//#define EGEN_PKT_ETHERTYPE0_ptr	101 // not used

//_________________EGEN_PKT_TYPE_ptr____________________ 
// 0000,0000:  ^10:Ethernet, ^32:VLAN, ^54:IP, ^76:future
// 10: 00=std Layer2-Ethernet, 01=EthernetII, 10=802.3 LLC, 11=802.3SNAP 
// 32: 00=none, 01=VLAN, 10=StackedVLAN, 11:future
// 54: 00=none, 01=IPv4, 10=IPv6
// 76: future (compatibility modes)
#define EGEN_PKT_TYPE_ptr 		100 
//#define EGEN_PKT_STREAMNUM_ptr 101 ---> stolen for EMIXUSERSIZE (above) need a new spot for this

#define EGEN_IP1_HEADER3_ptr	102	//1 IP HEADER (1st 4octets, 32-bits) (IPv4:4total) - 0x0800 is hardcoded!
#define EGEN_IP1_HEADER2_ptr	103
#define EGEN_IP1_HEADER1_ptr	104
#define EGEN_IP1_HEADER0_ptr	105

#define EGEN_IP2_HEADER3_ptr	106	//2 IP HEADER (2nd 4octets, 32-bits) (IPv4:8total)
#define EGEN_IP2_HEADER2_ptr	107
#define EGEN_IP2_HEADER1_ptr	108
#define EGEN_IP2_HEADER0_ptr	109

#define EGEN_IPV4_HDR3_ptr		110	//3 IPv4 3rd HEADER	(IPv4:12total)
#define EGEN_IPV4_HDR2_ptr		111
#define EGEN_IPV4_HDR1_ptr		112
#define EGEN_IPV4_HDR0_ptr		113
#define EGEN_SRC1_IP3_ptr		110	//3 IPv6 SRC_ADDR 1:4
#define EGEN_SRC1_IP2_ptr		111
#define EGEN_SRC1_IP1_ptr		112
#define EGEN_SRC1_IP0_ptr		113

#define EGEN_IPV4_SRC3_ptr		118	//4 IPv4 SRC_ADDR	(IPv4:16total)
#define EGEN_IPV4_SRC2_ptr		119
#define EGEN_IPV4_SRC1_ptr		120
#define EGEN_IPV4_SRC0_ptr		121
#define EGEN_SRC2_IP3_ptr		118	//4 IPv6 SRC_ADDR 2:4
#define EGEN_SRC2_IP2_ptr		119
#define EGEN_SRC2_IP1_ptr		120
#define EGEN_SRC2_IP0_ptr		121
	
#define EGEN_SRC3_IP3_ptr		122	//5 IPv6 SRC_ADDR 3:4
#define EGEN_SRC3_IP2_ptr		123
#define EGEN_SRC3_IP1_ptr		124
#define EGEN_SRC3_IP0_ptr		125
	
#define EGEN_SRC4_IP3_ptr		126	//6 IPv6 SRC_ADDR 4:4
#define EGEN_SRC4_IP2_ptr		127
#define EGEN_SRC4_IP1_ptr		128
#define EGEN_SRC4_IP0_ptr		129

#define EGEN_DEST1_IP3_ptr		114	//7 IPv4 & IPv6 DEST_ADDR 1:4	(IPv4:20total)
#define EGEN_DEST1_IP2_ptr		115
#define EGEN_DEST1_IP1_ptr		116
#define EGEN_DEST1_IP0_ptr		117

#define EGEN_DEST2_IP3_ptr		130	//8 IPv6 DEST_ADDR 2:4
#define EGEN_DEST2_IP2_ptr		131
#define EGEN_DEST2_IP1_ptr		132
#define EGEN_DEST2_IP0_ptr		133
	
#define EGEN_DEST3_IP3_ptr		134	//9 IPv6 DEST_ADDR 3:4
#define EGEN_DEST3_IP2_ptr		135
#define EGEN_DEST3_IP1_ptr		136
#define EGEN_DEST3_IP0_ptr		137
	
#define EGEN_DEST4_IP3_ptr		138	//10 IPv6 DEST_ADDR 4:4
#define EGEN_DEST4_IP2_ptr		139
#define EGEN_DEST4_IP1_ptr		140
#define EGEN_DEST4_IP0_ptr		141

#define EGEN_NUM_SECS1_ptr 		142	// num of secs to generate Packets!
#define EGEN_NUM_SECS0_ptr 		143	// Rcvd from GUI and just saved for GUI (for GUI recovery)!

#define EGEN_PKT_LENGTH1_ptr	144	// egen_pkt_length (Generator Frame_Length)
#define EGEN_PKT_LENGTH0_ptr	145

//#define OPTIONS_INSTALLED_ptr		146	// tell FW what HW we have available (saved in Flash)
//#define OPTIONS_ENABLED_ptr			147	// tell FW if some of the Installed HW Options are Enabled (saved in Flash)

#define STARTSTOP_ptr			148				// have this at end so all setup crap is executed 1st!
#define ConfigChecksum2			(MaxConfig2-1)	// currently 150-1=149

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

#define SECONDS3_ptr	MaxConfig2+5	// TEST SECONDS (ELAP)
#define SECONDS2_ptr  	MaxConfig2+6	//*
#define SECONDS1_ptr  	MaxConfig2+7
#define SECONDS0_ptr  	MaxConfig2+8

#define MISC_STAT_ptr	MaxConfig2+9	// Bleep^0,Error^1,History^2,test_running^3,LOCAL_CHG^4,
#define SFP_STAT_ptr	MaxConfig2+10	// SFP changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
	#define SFP_STATE_MASK		0xF9		// mask off or clear SFP state flags
	//#define SFP10G_STATE_MASK	0xE7		// mask off or clear SFP+ state flags
	#define SFP10G_STATE_MASK	0xF7		// mask off A-only or clear SFP+ state flags


//===== PHY STATUS ============================================
#define PHY_OPERATING_MODE1_ptr		MaxConfig2+11	//*
#define PHY_OPERATING_MODE0_ptr		MaxConfig2+12
#define PHYB_OPERATING_MODE1_ptr	MaxConfig2+13	//*
#define PHYB_OPERATING_MODE0_ptr	MaxConfig2+14
#define PHY_OPT_POWER1_ptr		MaxConfig2+15	//
#define PHY_OPT_POWER0_ptr		MaxConfig2+16
#define PHY_CABLE_DIAG1_ptr		MaxConfig2+17	//
#define PHY_CABLE_DIAG0__ptr	MaxConfig2+18
#define PHY_MII_STAT1_ptr		MaxConfig2+19	//*
#define PHY_MII_STAT0_ptr		MaxConfig2+20
//.....BORROWING SOME ROOM!...........
#define EMON_RX_CYCLE_END_CNT13_ptr		MaxConfig2+21	//* 64-bits: 1=Hi32,0=Lo32
#define EMON_RX_CYCLE_END_CNT12_ptr		MaxConfig2+22
#define EMON_RX_CYCLE_END_CNT11_ptr		MaxConfig2+23
#define EMON_RX_CYCLE_END_CNT10_ptr		MaxConfig2+24
#define EMON_RX_CYCLE_END_CNT03_ptr		MaxConfig2+25
#define EMON_RX_CYCLE_END_CNT02_ptr		MaxConfig2+26
#define EMON_RX_CYCLE_END_CNT01_ptr		MaxConfig2+27
#define EMON_RX_CYCLE_END_CNT00_ptr		MaxConfig2+28
//#define PHY_1G_STAT1_ptr		MaxConfig2+21
//#define PHY_1G_STAT0_ptr		MaxConfig2+22
//#define PHY_AUX1G_STAT1_ptr	MaxConfig2+23
//#define PHY_AUX1G_STAT0_ptr	MaxConfig2+24
//#define PHY_HCD_STATS1_ptr	MaxConfig2+25
//#define PHY_HCD_STATS0_ptr	MaxConfig2+26
//#define PHY_PRI_SERDES1_ptr	MaxConfig2+27
//#define PHY_PRI_SERDES0_ptr	MaxConfig2+28
//.....BORROWING SOME ROOM!...........
#define MAC_SNAP_TX_FRAME_CNT03_ptr		MaxConfig2+29	//* 64-bits: 1=Hi32,0=Lo32
#define MAC_SNAP_TX_FRAME_CNT02_ptr		MaxConfig2+30
#define MAC_SNAP_TX_FRAME_CNT01_ptr		MaxConfig2+31
#define MAC_SNAP_TX_FRAME_CNT00_ptr		MaxConfig2+32
#define MAC_SNAP_RX_FRAME_CNT03_ptr		MaxConfig2+33
#define MAC_SNAP_RX_FRAME_CNT02_ptr		MaxConfig2+34
#define MAC_SNAP_RX_FRAME_CNT01_ptr		MaxConfig2+35
#define MAC_SNAP_RX_FRAME_CNT00_ptr		MaxConfig2+36
//#define PHY_AUX_STATUS1_ptr	MaxConfig2+29
//#define PHY_AUX_STATUS0_ptr	MaxConfig2+30
//#define PHY_IEEE_EXT_STATUS_ptr			MaxConfig2+31	// 4-bits
//#define PHY_FALSECARRIERSENSE_CNTR_ptr	MaxConfig2+32
//#define PHY_LINK_PARTNER1_ptr			MaxConfig2+33
//#define PHY_LINK_PARTNER0_ptr			MaxConfig2+34
//#define PHY_RCV_ERR_CNTR1_ptr			MaxConfig2+35
//#define PHY_RCV_ERR_CNTR0_ptr			MaxConfig2+36
#define PHY10GA_OPERATING_MODE1_ptr		MaxConfig2+37	//10G port-A state
#define PHY10GA_OPERATING_MODE0_ptr		MaxConfig2+38

#define PHYB_MII_STAT1_ptr		MaxConfig2+39	//* TSE only
#define PHYB_MII_STAT0_ptr		MaxConfig2+40
//#define PHY_100FX_SERDES_STAT1_ptr	MaxConfig2+39
//#define PHY_100FX_SERDES_STAT0_ptr	MaxConfig2+40
#define PHYB_OPT_POWER1_ptr		MaxConfig2+41
#define PHYB_OPT_POWER0_ptr		MaxConfig2+42

//===== EMON STATUS ====================================================================
#define EMON_RX_CTRL_STAT1_ptr		MaxConfig2+43	//*? 9 bits total // not used anywhere
#define EMON_RX_CTRL_STAT0_ptr		MaxConfig2+44	//unused

#define EMON_RX_BYTE_CNT13_ptr		MaxConfig2+45	//* 64-bits: 1=Hi32,0=Lo32
#define EMON_RX_BYTE_CNT12_ptr		MaxConfig2+46
#define EMON_RX_BYTE_CNT11_ptr		MaxConfig2+47
#define EMON_RX_BYTE_CNT10_ptr		MaxConfig2+48
#define EMON_RX_BYTE_CNT03_ptr		MaxConfig2+49
#define EMON_RX_BYTE_CNT02_ptr		MaxConfig2+50
#define EMON_RX_BYTE_CNT01_ptr		MaxConfig2+51
#define EMON_RX_BYTE_CNT00_ptr		MaxConfig2+52

#define EMON_RX_CYCLE_CNT13_ptr		MaxConfig2+53	//* 64-bits: 1=Hi32,0=Lo32
#define EMON_RX_CYCLE_CNT12_ptr		MaxConfig2+54
#define EMON_RX_CYCLE_CNT11_ptr		MaxConfig2+55
#define EMON_RX_CYCLE_CNT10_ptr		MaxConfig2+56
#define EMON_RX_CYCLE_CNT03_ptr		MaxConfig2+57
#define EMON_RX_CYCLE_CNT02_ptr		MaxConfig2+58
#define EMON_RX_CYCLE_CNT01_ptr		MaxConfig2+59
#define EMON_RX_CYCLE_CNT00_ptr		MaxConfig2+60

#define EMON_RX_PKT_OK_CNT3_ptr		MaxConfig2+61	//*
#define EMON_RX_PKT_OK_CNT2_ptr		MaxConfig2+62
#define EMON_RX_PKT_OK_CNT1_ptr		MaxConfig2+63
#define EMON_RX_PKT_OK_CNT0_ptr		MaxConfig2+64

#define EMON_RX_PKT_ERR_CNT3_ptr	MaxConfig2+65	//*
#define EMON_RX_PKT_ERR_CNT2_ptr	MaxConfig2+66
#define EMON_RX_PKT_ERR_CNT1_ptr	MaxConfig2+67
#define EMON_RX_PKT_ERR_CNT0_ptr	MaxConfig2+68

#define EMON_PKT_BUILD_CNT1_ptr		MaxConfig2+69	//*D peek into EGEN actual pkt build time in clks
#define EMON_PKT_BUILD_CNT0_ptr		MaxConfig2+70

//#define EMON_X1_ptr		MaxConfig2+71
//#define EMON_X0_ptr		MaxConfig2+72

//===== SDT: SERVICE DISRUPTION TIME ============================================
#define SDT_SHORTEST_TIME1_ptr		MaxConfig2+73	//
#define SDT_SHORTEST_TIME0_ptr		MaxConfig2+74

#define SDT_LONGEST_TIME1_ptr		MaxConfig2+75	//
#define SDT_LONGEST_TIME0_ptr		MaxConfig2+76

#define SDT_AVG_TIME1_ptr			MaxConfig2+77	// don't know if this is something the GUI can calc
#define SDT_AVG_TIME0_ptr			MaxConfig2+78	// ...or it must be done real-time

#define SDT_TOTAL_TIME3_ptr			MaxConfig2+79	//
#define SDT_TOTAL_TIME2_ptr			MaxConfig2+80
#define SDT_TOTAL_TIME1_ptr			MaxConfig2+81
#define SDT_TOTAL_TIME0_ptr			MaxConfig2+82

//===== MAC STATUS GENERAL ======================================================
#define MAC_RCV_FRAME_TYPE_ptr		MaxConfig2+83	//
//#define MAC_rcv_spare_ptr			MaxConfig2+84

//#define MAC_RUNT_CNT3_ptr			MaxConfig2+85
//#define MAC_RUNT_CNT2_ptr			MaxConfig2+86
#define MAC_RUNT_CNT1_ptr			MaxConfig2+87	//
#define MAC_RUNT_CNT0_ptr			MaxConfig2+88

//#define MAC_COLLISION_CNT2_ptr	MaxConfig2+89
//#define MAC_COLLISION_CNT2_ptr	MaxConfig2+90
#define MAC_COLLISION_CNT1_ptr		MaxConfig2+91	//
#define MAC_COLLISION_CNT0_ptr		MaxConfig2+92

//#define MAC_VLAN_STAT3_ptr		MaxConfig2+93
//#define MAC_VLAN_STAT2_ptr		MaxConfig2+94
#define MAC_VLAN_STAT1_ptr			MaxConfig2+95	//
#define MAC_VLAN_STAT0_ptr			MaxConfig2+96


//===== MAC STATUS COUNTERS =====================================================
#define MAC_FRAME_TX_OK3_ptr			MaxConfig2+97	//*
#define MAC_FRAME_TX_OK2_ptr			MaxConfig2+98
#define MAC_FRAME_TX_OK1_ptr			MaxConfig2+99
#define MAC_FRAME_TX_OK0_ptr			MaxConfig2+100

#define MAC_FRAME_RX_OK3_ptr			MaxConfig2+101	//*
#define MAC_FRAME_RX_OK2_ptr			MaxConfig2+102
#define MAC_FRAME_RX_OK1_ptr			MaxConfig2+103
#define MAC_FRAME_RX_OK0_ptr			MaxConfig2+104

#define MAC_FRAME_FCS_ERRS3_ptr			MaxConfig2+105	//*
#define MAC_FRAME_FCS_ERRS2_ptr			MaxConfig2+106
#define MAC_FRAME_FCS_ERRS1_ptr			MaxConfig2+107
#define MAC_FRAME_FCS_ERRS0_ptr			MaxConfig2+108

#define MAC_ALIGNMENT_ERRS3_ptr			MaxConfig2+109	//*
#define MAC_ALIGNMENT_ERRS2_ptr			MaxConfig2+110
#define MAC_ALIGNMENT_ERRS1_ptr			MaxConfig2+111
#define MAC_ALIGNMENT_ERRS0_ptr			MaxConfig2+112

#define MAC_OCTETS_TX_OK13_ptr			MaxConfig2+113	// 64 bits
#define MAC_OCTETS_TX_OK12_ptr			MaxConfig2+114	//*
#define MAC_OCTETS_TX_OK11_ptr			MaxConfig2+115
#define MAC_OCTETS_TX_OK10_ptr			MaxConfig2+116
#define MAC_OCTETS_TX_OK03_ptr			MaxConfig2+117
#define MAC_OCTETS_TX_OK02_ptr			MaxConfig2+118
#define MAC_OCTETS_TX_OK01_ptr			MaxConfig2+119
#define MAC_OCTETS_TX_OK00_ptr			MaxConfig2+120

#define MAC_PAUSE_FRAME_TX3_ptr			MaxConfig2+121	//*
#define MAC_PAUSE_FRAME_TX2_ptr			MaxConfig2+122
#define MAC_PAUSE_FRAME_TX1_ptr			MaxConfig2+123
#define MAC_PAUSE_FRAME_TX0_ptr			MaxConfig2+124

#define MAC_PAUSE_FRAME_RX3_ptr			MaxConfig2+125	//*
#define MAC_PAUSE_FRAME_RX2_ptr			MaxConfig2+126
#define MAC_PAUSE_FRAME_RX1_ptr			MaxConfig2+127
#define MAC_PAUSE_FRAME_RX0_ptr			MaxConfig2+128

#define MAC_ERR_FRAMES_RX3_ptr			MaxConfig2+129	// ifInErrors
#define MAC_ERR_FRAMES_RX2_ptr			MaxConfig2+130	//*
#define MAC_ERR_FRAMES_RX1_ptr			MaxConfig2+131
#define MAC_ERR_FRAMES_RX0_ptr			MaxConfig2+132

#define MAC_ERR_FRAMES_TX3_ptr			MaxConfig2+133	// ifOutErrors
#define MAC_ERR_FRAMES_TX2_ptr			MaxConfig2+134	//*
#define MAC_ERR_FRAMES_TX1_ptr			MaxConfig2+135
#define MAC_ERR_FRAMES_TX0_ptr			MaxConfig2+136

#define MAC_UNICAST_FRAMES_RX3_ptr			MaxConfig2+137	//*
#define MAC_UNICAST_FRAMES_RX2_ptr			MaxConfig2+138
#define MAC_UNICAST_FRAMES_RX1_ptr			MaxConfig2+139
#define MAC_UNICAST_FRAMES_RX0_ptr			MaxConfig2+140

#define MAC_MULTICAST_FRAMES_RX3_ptr		MaxConfig2+141	//*
#define MAC_MULTICAST_FRAMES_RX2_ptr		MaxConfig2+142
#define MAC_MULTICAST_FRAMES_RX1_ptr		MaxConfig2+143
#define MAC_MULTICAST_FRAMES_RX0_ptr		MaxConfig2+144

#define MAC_BROADCAST_FRAMES_RX3_ptr		MaxConfig2+145	//*
#define MAC_BROADCAST_FRAMES_RX2_ptr		MaxConfig2+146
#define MAC_BROADCAST_FRAMES_RX1_ptr		MaxConfig2+147
#define MAC_BROADCAST_FRAMES_RX0_ptr		MaxConfig2+148

#define MAC_UNICAST_FRAMES_TX3_ptr			MaxConfig2+149	//*
#define MAC_UNICAST_FRAMES_TX2_ptr			MaxConfig2+150
#define MAC_UNICAST_FRAMES_TX1_ptr			MaxConfig2+151
#define MAC_UNICAST_FRAMES_TX0_ptr			MaxConfig2+152

#define MAC_MULTICAST_FRAMES_TX3_ptr		MaxConfig2+153	//*
#define MAC_MULTICAST_FRAMES_TX2_ptr		MaxConfig2+154
#define MAC_MULTICAST_FRAMES_TX1_ptr		MaxConfig2+155
#define MAC_MULTICAST_FRAMES_TX0_ptr		MaxConfig2+156

#define MAC_BROADCAST_FRAMES_TX3_ptr		MaxConfig2+157	//*
#define MAC_BROADCAST_FRAMES_TX2_ptr		MaxConfig2+158
#define MAC_BROADCAST_FRAMES_TX1_ptr		MaxConfig2+159
#define MAC_BROADCAST_FRAMES_TX0_ptr		MaxConfig2+160

#define MAC_ETHER_STATS_DROP3_ptr			MaxConfig2+161	// The number of frames that are dropped due to MAC internal errors
#define MAC_ETHER_STATS_DROP2_ptr			MaxConfig2+162	// ...when FIFO buffer overflow persists
#define MAC_ETHER_STATS_DROP1_ptr			MaxConfig2+163	//*
#define MAC_ETHER_STATS_DROP0_ptr			MaxConfig2+164

#define MAC_ETHER_STATS_OCTETS13_ptr		MaxConfig2+165	// 64-bits
#define MAC_ETHER_STATS_OCTETS12_ptr		MaxConfig2+166	// total octets rcvd: good & bad
#define MAC_ETHER_STATS_OCTETS11_ptr		MaxConfig2+167	//*
#define MAC_ETHER_STATS_OCTETS10_ptr		MaxConfig2+168
#define MAC_ETHER_STATS_OCTETS03_ptr		MaxConfig2+169
#define MAC_ETHER_STATS_OCTETS02_ptr		MaxConfig2+170
#define MAC_ETHER_STATS_OCTETS01_ptr		MaxConfig2+171
#define MAC_ETHER_STATS_OCTETS00_ptr		MaxConfig2+172

#define MAC_ETHER_STATS_PACKETS3_ptr		MaxConfig2+173	// total packets (frames) rcvd: good & bad
#define MAC_ETHER_STATS_PACKETS2_ptr		MaxConfig2+174	//*
#define MAC_ETHER_STATS_PACKETS1_ptr		MaxConfig2+175
#define MAC_ETHER_STATS_PACKETS0_ptr		MaxConfig2+176

//===== all packet(frame) counters include both good & bad (errored) frames ===========
#define MAC_UNDERSIZED_PACKETS_RX3_ptr		MaxConfig2+177	// Undersized Frames are <64-bytes
#define MAC_UNDERSIZED_PACKETS_RX2_ptr		MaxConfig2+178	//*
#define MAC_UNDERSIZED_PACKETS_RX1_ptr		MaxConfig2+179
#define MAC_UNDERSIZED_PACKETS_RX0_ptr		MaxConfig2+180

#define MAC_OVERSIZED_PACKETS_RX3_ptr		MaxConfig2+181	// Oversized Frames are > value in "frm_length" register
#define MAC_OVERSIZED_PACKETS_RX2_ptr		MaxConfig2+182	// (vars.h) MAC_FRM_LENGTH
#define MAC_OVERSIZED_PACKETS_RX1_ptr		MaxConfig2+183	//*
#define MAC_OVERSIZED_PACKETS_RX0_ptr		MaxConfig2+184

#define MAC_64_PACKETS_RX3_ptr				MaxConfig2+185	//*
#define MAC_64_PACKETS_RX2_ptr				MaxConfig2+186
#define MAC_64_PACKETS_RX1_ptr				MaxConfig2+187
#define MAC_64_PACKETS_RX0_ptr				MaxConfig2+188

#define MAC_65_127_PACKETS_RX3_ptr			MaxConfig2+189	//*
#define MAC_65_127_PACKETS_RX2_ptr			MaxConfig2+190
#define MAC_65_127_PACKETS_RX1_ptr			MaxConfig2+191
#define MAC_65_127_PACKETS_RX0_ptr			MaxConfig2+192

#define MAC_128_255_PACKETS_RX3_ptr			MaxConfig2+193	//*
#define MAC_128_255_PACKETS_RX2_ptr			MaxConfig2+194
#define MAC_128_255_PACKETS_RX1_ptr			MaxConfig2+195
#define MAC_128_255_PACKETS_RX0_ptr			MaxConfig2+196

#define MAC_256_511_PACKETS_RX3_ptr			MaxConfig2+197	//*
#define MAC_256_511_PACKETS_RX2_ptr			MaxConfig2+198
#define MAC_256_511_PACKETS_RX1_ptr			MaxConfig2+199
#define MAC_256_511_PACKETS_RX0_ptr			MaxConfig2+200

#define MAC_512_1023_PACKETS_RX3_ptr		MaxConfig2+201	//*
#define MAC_512_1023_PACKETS_RX2_ptr		MaxConfig2+202
#define MAC_512_1023_PACKETS_RX1_ptr		MaxConfig2+203
#define MAC_512_1023_PACKETS_RX0_ptr		MaxConfig2+204

#define MAC_1024_1518_PACKETS_RX3_ptr		MaxConfig2+205	//*
#define MAC_1024_1518_PACKETS_RX2_ptr		MaxConfig2+206
#define MAC_1024_1518_PACKETS_RX1_ptr		MaxConfig2+207
#define MAC_1024_1518_PACKETS_RX0_ptr		MaxConfig2+208

#define MAC_1519_FLEN_PACKETS_RX3_ptr		MaxConfig2+209	// frames 1519 to max frame length defined in FRM_LENGTH
#define MAC_1519_FLEN_PACKETS_RX2_ptr		MaxConfig2+210	// (MAC_FRM_LENGTH)
#define MAC_1519_FLEN_PACKETS_RX1_ptr		MaxConfig2+211	//*
#define MAC_1519_FLEN_PACKETS_RX0_ptr		MaxConfig2+212

#define MAC_JABBERS_RX3_ptr					MaxConfig2+213	// Too Long frames with CRC error
#define MAC_JABBERS_RX2_ptr					MaxConfig2+214	//*
#define MAC_JABBERS_RX1_ptr					MaxConfig2+215
#define MAC_JABBERS_RX0_ptr					MaxConfig2+216

#define MAC_FRAGMENTS_RX3_ptr				MaxConfig2+217	// Too Short frames with CRC error
#define MAC_FRAGMENTS_RX2_ptr				MaxConfig2+218	//*
#define MAC_FRAGMENTS_RX1_ptr				MaxConfig2+219
#define MAC_FRAGMENTS_RX0_ptr				MaxConfig2+220

//#define MAC_3_ptr			MaxConfig2+221
//#define MAC_2_ptr			MaxConfig2+222
//#define MAC_1_ptr			MaxConfig2+223
//#define MAC_0_ptr			MaxConfig2+224
#define MAC_OCTETS_RX_OK13_ptr			MaxConfig2+221	// 64 bits
#define MAC_OCTETS_RX_OK12_ptr			MaxConfig2+222	// this includes OVH+IPG (6+(default)IPG of 12 = 18)
#define MAC_OCTETS_RX_OK11_ptr			MaxConfig2+223	// so for 1000 packets would be 1000*(Frm_len) + 1000*(OVH_IPG)
#define MAC_OCTETS_RX_OK10_ptr			MaxConfig2+224	// this is a means to determine IPG of incoming unknown packets!
#define MAC_OCTETS_RX_OK03_ptr			MaxConfig2+225	//*
#define MAC_OCTETS_RX_OK02_ptr			MaxConfig2+226
#define MAC_OCTETS_RX_OK01_ptr			MaxConfig2+227
#define MAC_OCTETS_RX_OK00_ptr			MaxConfig2+228

//were temp...no longer used....
//#define EMON_LATENCY_CYCLE_CNT13_ptr	MaxConfig2+229	// 64-bits: 1=Hi32,0=Lo32
//#define EMON_LATENCY_CYCLE_CNT12_ptr	MaxConfig2+230	//*
//#define EMON_LATENCY_CYCLE_CNT11_ptr	MaxConfig2+231
//#define EMON_LATENCY_CYCLE_CNT10_ptr	MaxConfig2+232
//#define EMON_LATENCY_CYCLE_CNT03_ptr	MaxConfig2+233
//#define EMON_LATENCY_CYCLE_CNT02_ptr	MaxConfig2+234
//#define EMON_LATENCY_CYCLE_CNT01_ptr	MaxConfig2+235
//#define EMON_LATENCY_CYCLE_CNT00_ptr	MaxConfig2+236
//
//#define EMON_JITTER_CYCLE_CNT03_ptr		MaxConfig2+237
//#define EMON_JITTER_CYCLE_CNT02_ptr		MaxConfig2+238
//#define EMON_JITTER_CYCLE_CNT01_ptr		MaxConfig2+239
//#define EMON_JITTER_CYCLE_CNT00_ptr		MaxConfig2+240
#define PHY10G_OPT_POWER1_ptr	MaxConfig2+229	// 10G-A Power
#define PHY10G_OPT_POWER0_ptr	MaxConfig2+230
#define PHY10GB_OPT_POWER1_ptr	MaxConfig2+231	// 10G-B Power
#define PHY10GB_OPT_POWER0_ptr	MaxConfig2+232

//MISC PHY STAT bit encoded here..........
// PHY_EXT_STATUS (11h) ^13:MDI Crossover state ==> PHY10G_MISC^0:0=normal,1=crossed
#define PHY_MISC1_ptr	MaxConfig2+233
#define PHY_MISC0_ptr	MaxConfig2+234	//start with just a byte
#define PHYB_MISC1_ptr	MaxConfig2+235
#define PHYB_MISC0_ptr	MaxConfig2+236	//start with just a byte
//#define MISC_ptr		MaxConfig2+237	// need for 10G A/B MISC?
//#define MISC_ptr		MaxConfig2+238
//#define MISC_ptr		MaxConfig2+239
//#define MISC_ptr		MaxConfig2+240

#define EGEN_TX_PKT_SENT_CNT3_ptr		MaxConfig2+241	//* = same as TRAFFIC_GEN_PKT_TX_CNT
#define EGEN_TX_PKT_SENT_CNT2_ptr		MaxConfig2+242
#define EGEN_TX_PKT_SENT_CNT1_ptr		MaxConfig2+243
#define EGEN_TX_PKT_SENT_CNT0_ptr		MaxConfig2+244
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
#define ModeCopper100M  	1
#define ModeCopper1G 		2
#define ModeFiber1G 		3
#define ModeFiber10G 		4
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

#define TestStartGuiBit		(ConfigStat[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)//for TSE
#define TestStartGuiBitB	(ConfigStatB[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)//no references -p
#define TestStartGuiBit10	(ConfigStat10G[STARTSTOP_ptr]&0x10)	// Returned to GUI by MOD	(MOD sees Start Test cmd)// for 10G only

#define TestStartModBit		(ConfigStat[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)
#define TestStartModBitB	(ConfigStatB[STARTSTOP_ptr]&0x20)	// Set/Clr and sent by MOD 	(test has started)//no references -p
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

/****************************************************************************************************************************
STATUS for Y.1564
Latency cycle value
****************************************************************************************************************************/
#define MIN_0_THRESHOLD XX  //Minimum value for any threshold (float)
#define MIN_1_THRESHOLD XX
#define MIN_2_THRESHOLD XX
#define MIN_3_THRESHOLD XX

#define MAX_0_THRESHOLD XX  //Maximum value for any threshold (float)
#define MAX_1_THRESHOLD XX
#define MAX_2_THRESHOLD XX
#define MAX_3_THRESHOLD XX

#define AVG_0_THRESHOLD XX  //Average value for any threshold (float)
#define AVG_1_THRESHOLD XX
#define AVG_2_THRESHOLD XX
#define AVG_3_THRESHOLD XX

#define TEST_PROGRESS XX // To report about the current frame/stream being executed for the test progress dialog


#endif /* CONFSTAT_H_ */
/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
