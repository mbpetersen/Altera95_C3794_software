/*************************************************************************
*		Initialization Routine										 	 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"


//********************************************************************************
// System Initialization
//	Communication Link between: GIGE and GUI
//********************************************************************************
void init_gige()
{

	GIGE_A = YES;						// init to GIGE_A!
	GIGE10 = NO;						// 10G 'mode' OFF
	MS_PKT = NO;

	destination_tse_addr0	= MAC_0B;
	D(1, BUG("Init: destination_tse_addr0: %x\n", destination_tse_addr0));
	destination_tse_addr1	= MAC_1B;
	D(1, BUG("Init: destination_tse_addr1: %x\n", destination_tse_addr1));
	source_tse_addr0		= MAC_0;		// usually this IS the same as the MAC (sourcing the EGEN)
	D(1, BUG("Init: source_tse_addr0: %x\n", source_tse_addr0));
	source_tse_addr1		= MAC_1;		// init to Port-A MAC addr (our EGEN)
	D(1, BUG("Init: source_tse_addr1: %x\n", source_tse_addr1));

	destination_10g_addr0	= MAC10G_0; // use 10G-A for now - maybe only 1 10G port!   (fut) MAC10G_0B
	D(1, BUG("Init: destination_10g_addr0: %x\n", destination_10g_addr0));
	destination_10g_addr1	= MAC10G_1; // use 10G-A for now - maybe only 1 10G port!   (fut) MAC10G_1B
	D(1, BUG("Init: destination_10g_addr1: %x\n", destination_10g_addr1));
	source_10g_addr0		= MAC10G_0;	// usually this IS the same as the MAC (sourcing the EGEN)
	D(1, BUG("Init: source_10g_addr0: %x\n", source_10g_addr0));
	source_10g_addr1		= MAC10G_1;	// init to Port-A MAC addr (our EGEN)
	D(1, BUG("Init: source_10g_addr1: %x\n", source_10g_addr1));


	//===========================================================================
	// **** SETUP INIT CONFIG-A TO GUI ***********
	//===========================================================================
	ConfigStat[ID_ptr] = Packet_GIGEA;
	ConfigStat[SYSCONFIG_ptr] = 0x00;
	SaveBytesLong(ConfigStat, MAC_CONFIG3_ptr, MACA_COMMAND_CONFIG);
	SaveBytesInt(ConfigStat, MAC_FRAME_LEN1_ptr, MAC_FRM_LENGTH);		// egen_pkt_length == MAC_FRM_LENGTH
	SaveBytesLong(ConfigStat,MAC_ADDR_MSB03_ptr,MAC_0);
	SaveBytesInt(ConfigStat,MAC_ADDR_LSB11_ptr,MAC_1);
	ConfigStat[MAC_IPG_LENGTH_ptr] = TX_IPG_LENGTH;


	SaveBytesLong(ConfigStat,EGEN_NUM_SECS3_ptr,gen_number_seconds);
	SaveBytesInt(ConfigStat,TRAFFIC_GEN_CONFIG01_ptr,traffic_gen_pkt_length);//1518
	SaveBytesLong(ConfigStat,TRAFFIC_RATE3_ptr,0x42c80000);//100%

	SaveBytesLong(ConfigStat,TRAFFIC_HDR0_3_ptr,source_tse_addr0);//src_mac_addr //msb
	SaveBytesInt(ConfigStat,TRAFFIC_HDR1_3_ptr,source_tse_addr1);//src_mac_addr //lsb
	SaveBytesLong(ConfigStat,TRAFFIC_HDR1_1_ptr,destination_tse_addr0);//dest_mac_addr //msb
	SaveBytesInt(ConfigStat,TRAFFIC_HDR2_1_ptr,destination_tse_addr1);//dest_mac_addr //lsb

	//ConfigStat[TRAFFIC_GEN_CONFIG00_ptr] = 0; // initialize to fixed length frame sizes

	ConfigStat[GIGEMODE_ptr] = ModeCopper100M;//old mode_copper;	// init to copper
	SaveBytesInt(ConfigStat,PHY_CONFIG1_ptr,PHYA_CONFIG);

	ConfigStat[CORE_SIGROUTE_MODES_ptr] = 0;	// TSE A&B and 10G all in NORMAL operating MODE
	ConfigStat[LOOPBACK_ptr] = 0x02;	// Detect loopback on for all ports
	ConfigStat[MISC_ptr] = 0x00;


	//===========================================================================
	// **** SETUP INIT CONFIG-B TO GUI ***********
	//===========================================================================
	ConfigStatB[ID_ptr] = Packet_GIGEB;
	ConfigStatB[SYSCONFIG_ptr] = 0x00;
	SaveBytesLong(ConfigStatB, MAC_CONFIG3_ptr, MACB_COMMAND_CONFIG);
	SaveBytesInt(ConfigStatB, MAC_FRAME_LEN1_ptr, MAC_FRM_LENGTH);		// egen_pkt_length == MAC_FRM_LENGTH
	/*SaveBytesLong(ConfigStatB,MACB_ADDR_MSB3_ptr,MAC_0B);
	SaveBytesInt(ConfigStatB,MACB_ADDR_LSB1_ptr,MAC_1B);*/
	SaveBytesLong(ConfigStatB,MAC_ADDR_MSB03_ptr,MAC_0B);
	SaveBytesInt(ConfigStatB,MAC_ADDR_LSB11_ptr,MAC_1B);
	ConfigStatB[MAC_IPG_LENGTH_ptr] = TX_IPG_LENGTH;

	SaveBytesLong(ConfigStatB,TRAFFIC_RATE3_ptr,0x42c80000);//100%
	SaveBytesLong(ConfigStatB,EGEN_NUM_SECS3_ptr,gen_number_seconds);
	SaveBytesInt(ConfigStatB,TRAFFIC_GEN_CONFIG01_ptr,traffic_gen_pkt_length);//1518
	SaveBytesLong(ConfigStatB,TRAFFIC_HDR0_3_ptr,source_tse_addr0);//src_mac_addr //msb
	SaveBytesInt(ConfigStatB,TRAFFIC_HDR1_3_ptr,source_tse_addr1);//src_mac_addr //lsb
	SaveBytesLong(ConfigStatB,TRAFFIC_HDR1_1_ptr,destination_tse_addr0);//dest_mac_addr //msb
	SaveBytesInt(ConfigStatB,TRAFFIC_HDR2_1_ptr,destination_tse_addr1);//dest_mac_addr //lsb


	ConfigStatB[GIGEMODE_ptr] = ModeCopper100M;//old mode_copper;	// init to copper
	SaveBytesInt(ConfigStatB,PHY_CONFIG1_ptr,PHYB_CONFIG);

	ConfigStatB[CORE_SIGROUTE_MODES_ptr] = 0x60; // Port-B is initialized with 0x20:B-LPBK Enabled,  0x40:SWAP MAC addresses
	ConfigStatB[LOOPBACK_ptr] = 0x02;//Auto loop ON

	//===========================================================================
	// **** 10G SETUP INIT CONFIG-A TO GUI ***********
	//===========================================================================
	unsigned int i=0;
	for(i=0;i<MaxStatus2;i++)   // *** flush 10G array
		ConfigStat10G[i] = 0;

	ConfigStat10G[ID_ptr] = Packet_GIGE10A;
	ConfigStat10G[SYSCONFIG_ptr] = 0x00;
	SaveBytesLong(ConfigStat10G, MAC_CONFIG3_ptr, MAC10A_COMMAND_CONFIG);
	SaveBytesInt(ConfigStat10G, MAC_FRAME_LEN1_ptr, MAC_FRM_LENGTH);		// egen_pkt_length == MAC_FRM_LENGTH
	/*SaveBytesLong(ConfigStat10G,MAC10A_ADDR_MSB3_ptr,MAC10G_0); // MAC ADDRS in 10G array are 'clean' -> non-scrambled
	SaveBytesInt(ConfigStat10G,MAC10A_ADDR_LSB1_ptr,MAC10G_1);*/
	SaveBytesLong(ConfigStat10G,MAC_ADDR_MSB03_ptr,MAC10G_0);
	SaveBytesInt(ConfigStat10G,MAC_ADDR_LSB11_ptr,MAC10G_1);
	ConfigStat10G[MAC_IPG_LENGTH_ptr] = TX_IPG_LENGTH;

	SaveBytesLong(ConfigStat10G,TRAFFIC_RATE3_ptr,0x42c80000);//100%
	SaveBytesLong(ConfigStat10G,EGEN_NUM_SECS3_ptr,gen_number_seconds);
	SaveBytesInt(ConfigStat10G,TRAFFIC_GEN_CONFIG01_ptr,traffic_gen_pkt_length);//1518
	SaveBytesLong(ConfigStat10G,TRAFFIC_HDR0_3_ptr,source_10g_addr0);//src_mac_addr //msb
	SaveBytesInt(ConfigStat10G,TRAFFIC_HDR1_3_ptr,source_10g_addr1);//src_mac_addr //lsb
	SaveBytesLong(ConfigStat10G,TRAFFIC_HDR1_1_ptr,destination_10g_addr0);//dest_mac_addr //msb
	SaveBytesInt(ConfigStat10G,TRAFFIC_HDR2_1_ptr,destination_10g_addr1);//dest_mac_addr //lsb

	ConfigStat10G[LOOPBACK_ptr] = 0x02;
	ConfigStat10G[GIGEMODE_ptr] = ModeFiber10G;//old mode_fiber;	// init to fiber

	//===========================================================================
	//	SYS INFO carried to GUI via SpecialStat[]  ...info @ gigle.h
	//===========================================================================
	SpecialStat[HW_VERSION_ptr] = GIGE_HW_VER;		//400	// rev1 to rev255
	//SpecialStat[FW_VERSION1_ptr]= GIGE_FW_VER1;	//401	// ver1.ver0 0 -> 65535,  65535/1000 = v0.0 to v65.535
	//SpecialStat[FW_VERSION0_ptr]= GIGE_FW_VER0;	//402	// v1.01 -> 101
	SpecialStat[FW_VERSION1_ptr] = GIGE_FW_VER1;
	SpecialStat[FW_VERSION0_ptr] = GIGE_FW_VER0;
	SpecialStat[FLASH_OPT_INSTALLED_ptr] = OPTIONS_INSTALLED;	//403	^0=dual-1g, ^1=10G-A, ^2=10G-B, (?) THRU,VLAN,IP,OAM
	SpecialStat[FLASH_OPT_ENABLED_ptr] = OPTIONS_ENABLED;		//404	All ACTIVE LOW! FF=nothing installed, FF=nothing enabled


	SaveBytesLong(SpecialStat,MACA_ADDR_MSB3_ptr,MAC_0);
	SaveBytesInt(SpecialStat,MACA_ADDR_LSB1_ptr,MAC_1);
	SaveBytesLong(SpecialStat,MACB_ADDR_MSB3_ptr,MAC_0B);
	SaveBytesInt(SpecialStat,MACB_ADDR_LSB1_ptr,MAC_1B);

	SaveBytesLong(SpecialStat,MAC10A_ADDR_MSB3_ptr,MAC10G_0);
	SaveBytesInt(SpecialStat,MAC10A_ADDR_LSB1_ptr,MAC10G_1);
	SaveBytesLong(SpecialStat, MAC10B_ADDR_MSB3_ptr, IORD_32DIRECT(0x180800, 12)); //FPGA ver.
	//SaveBytesInt(SpecialStat, MAC10B_ADDR_LSB1_ptr, MAC10G_1B);//extra bytes used prev to send 10GB mac address

	// cc Flashed stored option settings to GUI on link-up
	ConfigStat[OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	ConfigStat[OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;
	ConfigStatB[OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	ConfigStatB[OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;
	ConfigStat10G[OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED;
	ConfigStat10G[OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED;
}
