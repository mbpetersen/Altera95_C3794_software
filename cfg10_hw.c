/*************************************************************************
*		10G CONFIG MODULE; Process Updated CONFIG array data from GUI	 *
*				   														 *
* Copyright (c) 2013 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
//********************************************************************************
// System Configuration
// Execute Configuration & Test changes as requested by the GUI
//********************************************************************************
void config_hardware10G()
{
unsigned short i=0;
//unsigned char temp=0;//,j=0;
//unsigned long LongInt=0;


	for(i=2;i<ConfigChecksum2;i++){   // *** Start with GIGEMODE
		if(RxBuffer[i] != ConfigStat10G[i]){
		//if((GIGE_A && RxBuffer[i] != ConfigStat10G[i]) || (!GIGE_A && RxBuffer[i] != ConfigStat10GB[i])){
			switch(i){
				// ***************************************************************
				// **** GIGE Mode, ^0:Copper/Fiber 0=copper,1=fiber     ***
				// ***************************************************************
				case GIGEMODE_ptr:
					// *** 10G ALWAYS FIBER!
					D(1, BUG("\n******************* 10G MODE Changed: %0d\n",RxBuffer[i]));
					//this never get's called since it's a re-LINK of the ConfgStat!
					//On speed change: reset num_packets & load% calculations...........
					//SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		//Call on: Frame Size, TestSecs, FPS change or SPEED change
					//SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);							// called on Frame Size, FPS change or SPEED
					// switch to TSE or 100G handled by LINK packet change
					break;
				//**********************************************************
				// ^0: Read Flash command & refresh SysInfo dump to GUI
				// ^1: Write FlashParameters into Flash (pre-erases block)
				case SYSCONFIG_ptr:
					// TSE port-A only - for now.
					break;

				case TEST_OPTIONS_PTR:
					//if(GIGE_A){
					if(RxBuffer[i]!=ConfigStat10G[i]){
					test_options =RxBuffer[i];
						if(test_options==0x00){
						D(1, BUG("QUICK TEST_OPTIONS_PTR:%04X\n",RxBuffer[i]));}
						else if(test_options==0x01){
							D(1, BUG("MS TEST_OPTIONS_PTR:%04X\n",RxBuffer[i]));
					}
					}
					ConfigStat10G[TEST_OPTIONS_PTR] = RxBuffer[TEST_OPTIONS_PTR];
					break;

				case MAC_CONFIG3_ptr:	//4
				case MAC_CONFIG2_ptr:	//5
				case MAC_CONFIG1_ptr:	//6
				case MAC_CONFIG0_ptr:	//7
					// Only process once for any word change!......
					//if(GIGE_A){
					if(BytesToLong(RxBuffer, MAC_CONFIG3_ptr) != BytesToLong(ConfigStat10G, MAC_CONFIG3_ptr)){
						D(1, BUG("\n Seeing new 10G MAC Config Change....\n"));
						MAC10A_COMMAND_CONFIG = BytesToLong(RxBuffer, MAC_CONFIG3_ptr);
						SaveBytesLong(ConfigStat10G,MAC_CONFIG3_ptr,MAC10A_COMMAND_CONFIG);
						init_10G_MAC();
						}
					break;
				case MAC_FRAME_LEN1_ptr:	//8
				case MAC_FRAME_LEN0_ptr:	//9
					// Only process once for any word change!......
				/*	if(GIGE_A){
						if(BytesToInt(RxBuffer, MAC_FRAME_LEN1_ptr) != BytesToInt(ConfigStat10G, MAC_FRAME_LEN1_ptr)){
							D(1, BUG("...Setting 10G MAC-A Max Frame Size (system MTU)\n"));
							MAC_FRM_LENGTH = BytesToInt(RxBuffer, MAC_FRAME_LEN1_ptr);  //was FRM_LENGTH MAC max is 9600
							if(MAC_FRM_LENGTH  == 0)	// protect from no GUI input
								MAC_FRM_LENGTH = 9600;  // was 1518
							SaveBytesInt(ConfigStat10G,MAC_FRAME_LEN1_ptr,MAC_FRM_LENGTH);


						}
					}*/
					break;


				//===================================================================
				//**** hardcoded now in tcl_def's #define's
				// * usually will be our source_addr for the EGEN as well!
				case MAC_ADDR_MSB03_ptr:	//14	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
				case MAC_ADDR_MSB02_ptr:	//15	// these are 0x001C23174ACB
				case MAC_ADDR_MSB01_ptr:	//16	// MSB3_ptr loaded into mac_0 = 0x17231C00  (reverse order)
				case MAC_ADDR_MSB00_ptr:	//17  	// LSB1_ptr loaded into mac_1 = 0x0000CB4A
				case MAC_ADDR_LSB11_ptr:	//18
				case MAC_ADDR_LSB10_ptr:	//19

					//not sure the 10G is in rev order like the TSE - but written correctly in cfg_hw.c
					//old MSB3:0 => MAC10G_0x = 0xAA250000;	// 10G MAC: 00-00-25-AA-xx-yy   TSE MAC: 00-00-25-AF-xx-yy
					//old LSB1:0 => MAC10G_1x = 0x0000yyxx;
					//NEW: Greenlee MAC's........
					//MSB3:0 => MAC10G_0x = 0x1A416450;	// 10G MAC: 50-64-41-1A-xx-yy   TSE MAC: 50-64-41-0A-xx-yy
					//LSB1:0 => MAC10G_1x = 0x0000yyxx;
					if(GIGE_A){
						// this is done in TSE portA mode only - auto sets all 4 MAC's
						}//End GIGE: Port-A
					break;
				//****/

				//===================================================================
				// PHY_CONFIG_ETH_SPEED_MASK		0x0003	// 2-bits
				// PHY_CONFIG_FULLDUPLEX_MASK		0x0004	// 1-bit	1= FDX, 0=HDX
				// PHY_CONFIG_ENABLE_AN_MASK		0x0008	// 1-bit	1=enabled
				// PHY_CONFIG_RESTART_ANEG_MASK		0x0010	// 1-bit
				// PHY_CONFIG_ENABLE_MDIX_MASK		0x0020	// 1-bit
				// PHY_CONFIG_MISC_1G_CNTL_MASK		0x0060	// 2-bits
				// PHY_CONFIG_RESET_MASK			0x0080	// 1-bit
				// PHY_CONFIG_ANEG_ADV_MASK			0x0F00	// 4-bits	adv eth support (10,100,1G,FDX, etc)
				// PHY_CONFIG_LOOPBACK_MASK			0x3000	// 2-bits
				// PHY_CONFIG_PWR_DOWN_MASK			0x4000	// 1-bit
				//===================================================================
				// should only see COP or FBR PHY_CONFIG's change - not both at once.
				case PHY_CONFIG1_ptr:	//60 16-bits: RESET, MODE, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG,
				case PHY_CONFIG0_ptr:	//61			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
					if(GIGE_A){
						if(BytesToInt(RxBuffer, PHY_CONFIG1_ptr) != BytesToInt(ConfigStat10G, PHY_CONFIG1_ptr)){
							// TODO
							}//END changed config
						}//END GIGE_A
					else{
						if(BytesToInt(RxBuffer, PHY_CONFIG1_ptr) != BytesToInt(ConfigStat10GB, PHY_CONFIG1_ptr)){
							// TODO
							}//END changed config
						}//END GIGE_B

						// set all other tcl #def's as SPEED.....
						// if GUI imposes a PHY change execute it here.....
						//.........................
					break;

				case PHY_SFP_MISC_ptr:	//62	// ^0:1=SFP_A Transmit_OFF, ^1:1=SFP_B Transmit_OFF, ^2:1=SFP+_A Transmit_OFF
					// can access SFP A, B and 10G from any Link-state; A, B or 10G...so maintain states across Links (Machines)
					if(((RxBuffer[i])&0x01) != ((ConfigStat10G[i])&0x01) || ((RxBuffer[i])&0x02) != ((ConfigStat[i])&0x02) || ((RxBuffer[i])&0x01) != ((ConfigStatB[i])&0x01)){ // if SFP_A Transmit_OFF has changed
						if(((RxBuffer[i])&0x01) == 0x01)		sfp_transmit_laser(PortA, OFF);
						else								sfp_transmit_laser(PortA, ON);
						ConfigStat[i] = RxBuffer[i];   	// cc to A sfp handler
						ConfigStatB[i] = RxBuffer[i];   	// cc to B sfp handler
						ConfigStat10G[i] = RxBuffer[i]; 	// cc to 10G sfp handler
						}

					if(((RxBuffer[i])&0x02) != ((ConfigStat10G[i])&0x02) || ((RxBuffer[i])&0x02) != ((ConfigStat[i])&0x02) || ((RxBuffer[i])&0x01) != ((ConfigStatB[i])&0x01)){ // if SFP_B Transmit_OFF has changed
						if(((RxBuffer[i])&0x02) == 0x02)		sfp_transmit_laser(PortB, OFF);
						else								sfp_transmit_laser(PortB, ON);
						ConfigStat[i] = RxBuffer[i];   	// cc to A sfp handler
						ConfigStatB[i] = RxBuffer[i];   	// cc to B sfp handler
						ConfigStat10G[i] = RxBuffer[i]; 	// cc to 10G sfp handler
						}

					if(((RxBuffer[i])&0x04) != ((ConfigStat10G[i])&0x04) || ((RxBuffer[i])&0x02) != ((ConfigStat[i])&0x02) || ((RxBuffer[i])&0x01) != ((ConfigStatB[i])&0x01)){ // if 10G SFP-A Transmit_OFF has changed
						if(((RxBuffer[i])&0x04) == 0x04)	sfp10G_transmit_laser(PortA, OFF);
						else								sfp10G_transmit_laser(PortA, ON);
						ConfigStat[i] = RxBuffer[i];   	// cc to A sfp handler
						ConfigStatB[i] = RxBuffer[i];   	// cc to B sfp handler
						ConfigStat10G[i] = RxBuffer[i]; 	// cc to 10G sfp handler
						}
					break;

				case CORE_SIGROUTE_MODES_ptr:	//69 ^0:0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK), 1=TSE_LANWAN_MODE, ^1:0=10G_NORMAL_MODE,1=10G_LANWAN_MODE (10G+B=LANWAN,A=GEN/MON)
				// RCVD in 10G & TSE MODE ---> accessed in both
				if(((RxBuffer[i])&0x02) != ((ConfigStat10G[i])&0x02)){	//^1:0=10G_NORMAL_MODE, 1=LAN/WAN
					if((RxBuffer[i] & 0x02) == 0x00){
						D(1, BUG("//////////////////////////// (10G NORMAL)"));
						D(1, BUG("\nSET_TSE_A_NORMAL_TEST_MODE\n"));
						SET_TSE_A_NORMAL_TEST_MODE;//0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK)
						D(1, BUG("MUX TSE_A_TX_3MUX TSE_NORMAL_MODE: 0x%04X\n", IORD_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG)));
						D(1, BUG("SET_TSE_A_RX_2SPLITTER_TO_EMON TSE_NORMAL_MODE: 0x%04X\n", IORD_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG)));

						D(1, BUG("\nSET_TSE_B_LOOPBACK\n"));
						SET_TSE_B_LOOPBACK;
						D(1, BUG("MUX TSE_B_TX_3MUX SET_TSE_B_LOOPBACK: 0x%04X\n", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));
						D(1, BUG("TSE_B_RX_3SPLITTER SET_TSE_B_LOOPBACK: 0x%04X\n",IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));

						SET_SWAP_B_TOGGLE |=0x01;//set swap ON
						D(1, BUG("\nSET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE)\n"));
						SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
						D(1, BUG("B:ADDR_SWAPPER STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
						D(1, BUG("\nSET_10G_NORMAL_TEST_MODE\n"));
						SET_10G_NORMAL_TEST_MODE;
						D(1, BUG("MUX A10G_TX_3MUX SET_10G_NORMAL_TEST_MODE: 0x%04X\n", IORD_16DIRECT(A10G_TX_3MUX, ST_MUX_REG)));
						D(1, BUG("SET_10G_RX_3SPLITTER_TO_TMON SET_10G_NORMAL_TEST_MODE: 0x%04X\n", IORD_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG)));

						//we know GUI is taking A out of Promiscuous mode from 10G-link, we won't see A since we're in 10G so...
						MACA_COMMAND_CONFIG &= ~0x10; // force promiscuous mode off for A!
						SaveBytesLong(ConfigStat,MAC_CONFIG3_ptr,MACA_COMMAND_CONFIG);
						configure_tse_mac(TRIPLE_SPEED_ETHERNET_0_BASE);
						configure_tse0_to_phya();	// run this after configuring the MAC

						//we know GUI is taking B out of Promiscuous mode from 10G-link, we won't see B since we're in 10G so...
						MACB_COMMAND_CONFIG &= ~0x10; // force promiscuous mode off for B!
						SaveBytesLong(ConfigStatB,MAC_CONFIG3_ptr,MACB_COMMAND_CONFIG);
						configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
						configure_tse1_to_phyb();	// run this after configuring the MAC

						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
						}
					else{//if((RxBuffer[i] & 0x02) == 0x02){	//0=NORMAL_MODE, 2=10G_LANWAN_MODE
						D(1, BUG("//////////////////////////////////SET_10G_LANWAN_MODE"));
						SET_10G_LANWAN_MODE;	// 10G & B in LAN-WAN mode, sets TSE-A to normal

						D(1, BUG("\n10G:SET_TSE_A_NORMAL_TEST_MODE\n"));
						D(1, BUG("10G:MUX TSE_A_TX_3MUX TSE_NORMAL_MODE: 0x%04X\n", IORD_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG)));
						D(1, BUG("10G:SET_TSE_A_RX_2SPLITTER_TO_EMON TSE_NORMAL_MODE: 0x%04X\n", IORD_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG)));

						D(1, BUG("\n10G:SET_TSE_B_TX_TO_10G_LANWAN\n"));
						D(1, BUG("10G:MUX TSE_B_TX_3MUX SET_TSE_B_TX_TO_10G_LANWAN: 0x%04X\n", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));

						D(1, BUG("\n10G:SET_TSE_B_RX_TO_10G_LANWAN\n"));
						D(1, BUG("10G:TSE_B_RX_3SPLITTER SET_TSE_B_RX_TO_10G_LANWAN: 0x%04X\n", IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));

						D(1, BUG("\n10G:SET_10G_TX_TO_10G_LANWAN\n"));
						D(1, BUG("10G:A10G_TX_3MUX SET_10G_TX_TO_10G_LANWAN: 0x%04X\n",  IORD_16DIRECT(A10G_TX_3MUX, ST_MUX_REG)));

						D(1, BUG("\n10G:SET_10G_RX_TO_10G_LANWAN\n"));
						D(1, BUG("10G:SPLITTER SET_10G_RX_TO_10G_LANWAN: 0x%04X\n",  IORD_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG)));

						SET_SWAP_B_TOGGLE  = 0;
						SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
						D(1, BUG("\n10G:SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE)\n"));
						D(1, BUG("10G:ADDR_SWAPPER STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
						//we know GUI is putting A in Promiscuous mode from 10G-link, we won't see A since we're in 10G so...
						MACA_COMMAND_CONFIG |= 0x10; // force promiscuous mode on for B!
						SaveBytesLong(ConfigStat,MAC_CONFIG3_ptr,MACA_COMMAND_CONFIG);
						configure_tse_mac(TRIPLE_SPEED_ETHERNET_0_BASE);
						configure_tse0_to_phya();	// run this after configuring the MAC

						//we know GUI is putting B in Promiscuous mode from A-link, we won't see B since we're in A so...
						MACB_COMMAND_CONFIG |= 0x10; // force promiscuous mode on for B!
						SaveBytesLong(ConfigStatB,MAC_CONFIG3_ptr,MACB_COMMAND_CONFIG);
						configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
						configure_tse1_to_phyb();	// run this after configuring the MAC
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
						}
					}//End LAN/WAN Mode changed

				if(((RxBuffer[i])&0x10) != ((ConfigStat10G[i])&0x10)){	//^4:0=Looped-test, 1=End-to-End test
					if((RxBuffer[i] & 0x10) == 0x10){
						EMON_TIMEOUT = 100;		// 10seconds (100 *100mS) for end-to-end tests
						END_TO_END_TEST = YES;
						D(1, BUG("\nEnd-to-End test\n"));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |= 0x10;	// maintain bit states across A, B (10G has it's own flag)
						}
					else{
						EMON_TIMEOUT = 50;		// 5seconds (50 *100mS) for far-end-Looped tests
						END_TO_END_TEST = NO;
						D(1, BUG("\nLooped test\n"));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &= ~0x10;	// maintain bit states across A, B (10G has it's own flag)
						}
					}

				//***** 10G LOOPBACK Enable/disable **************************************
				if(((RxBuffer[i])&0x20) != ((ConfigStat10G[i])&0x20)){	//^5:0=10G_Loopback OFF, 1=Loopback ON
					if((RxBuffer[i] & 0x20) == 0x20){	// Loopback ON
						D(1, BUG("\nEnable LPBK\n"));
						SET_10G_TX_TO_NW_LOOPBACK;		// Enable LPBK
						SET_10G_RX_3SPLITTER_TO_10G_TX;
						D(1, BUG("10GA:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_10GE_BASE, ST_MUX_REG)));
						D(1, BUG("10GA:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_0_BASE, ST_SPLITTER_REG)));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |=0x20;

						SET_SWAP_10GA_TOGGLE |= 0x08; 	// also tell swapper we're in lpbk
						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
						D(1, BUG("10A:SET_SWAP_10GA_TOGGLE STATE - LPBK ON: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
						D(1, BUG("10A:ADDR_SWAPPER STATE - LPBK ON: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));

						//SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD; //test inside init_10G_MAC()
						//init_10G_MAC();

						D(1, BUG("\nSetting PHY TxClk to FTX_LTIME_EN...(use Recovered Clock)\n"));
						write_ext_10G_phy(0xC001, 1, (read_ext_10G_phy(0xC001, 1))|0x0200); // set FTX_LTIME_EN ON - to use Recovered Clock as Tx Clock


						//init_10G_SC_FIFO(ETH_10G_SCFIFO_STORE_FORWARD);	//init & set RxFIFO to Store & Fwd Mode for Lpbk
						if (SFP_10G_PRESENT) {
							set_reset_10GMAC_FIFOS();
							SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD;
							D(1, BUG("SWITCHING TO RCVRD\n"));
							usleep(1);
							clear_reset_10GMAC_FIFOS();
							init_10G_MAC();
							init_10G_SC_FIFO(ETH_10G_SCFIFO_STORE_FORWARD);
							int reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
							reg_val |= 0x01;  // Activate Tx Digital Reset
							IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT, reg_val);
							init_10G_xaui(); // no need when in auto-CDR lock it's back everytime!
						}

						//reset_10G_MAC_FIFOS();

						SaveBytesLong(ConfigStat10G,SECONDS3_ptr,0);	// Clear ET when going into lpbk
					}
					else {	// Loopback OFF

						D(1, BUG("\nDisable LPBK\n"));
						SET_10G_NORMAL_TEST_MODE;
						D(1, BUG("10GA:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_10GE_BASE, ST_MUX_REG)));
						D(1, BUG("10GA:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_0_BASE, ST_SPLITTER_REG)));
						//    SET_10G_TX_3MUX_TO_NO_OUTPUT; // 'disable' LPBK
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &=~0x20;

						SET_SWAP_10GA_TOGGLE &= ~0x08;      // also tell swapper we're not in lpbk
						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
						D(1, BUG("10A:SET_SWAP_10GA_TOGGLE STATE - LPBK OFF: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
						D(1, BUG("10A:ADDR_SWAPPER STATE - LPBK OFF: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));

						//SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;//test inside init_10G_MAC()
						//init_10G_MAC();

						D(1, BUG("\nSetting PHY TxClk to FPLL_OUT...(use Local OSC, FPLL_OUT)\n"));
						write_ext_10G_phy(0xC001, 1, (read_ext_10G_phy(0xC001, 1))&0xFDFF); // set FTX_LTIME_EN OFF - to use local OSC as Tx Clock


						//init_10G_SC_FIFO(ETH_10G_SCFIFO_CUT_THRU);		//init & set RxFIFO to Cut-thru mode for NORMAL mode

						if (SFP_10G_PRESENT) {
							set_reset_10GMAC_FIFOS();

							SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;
							D(1, BUG("SWITCHING TO TXFPLL\n"));

	//							else {
	//								SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD;
	//								D(1, BUG("SWITCHING TO RCVRD\n"));
	//							}
							usleep(1);
							clear_reset_10GMAC_FIFOS();
							init_10G_MAC();
							init_10G_SC_FIFO(ETH_10G_SCFIFO_CUT_THRU);		//init & set RxFIFO to Cut-thru mode for NORMAL mode

							int reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
							reg_val |= 0x01;  // Activate Tx Digital Reset
							IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT, reg_val);
							init_10G_xaui(); // no need when in auto-CDR lock it's back everytime!
						}

//							D(1, BUG("new_size: %x\n", IORD_32DIRECT(SWAPPER64_10G_BASE, 0x28)));

					}

				}//End lpbk mode changed

				//*** SWAP MAC ADDRESS ****************************
				if(((RxBuffer[i])&0x40) != ((ConfigStat10G[i])&0x40)){	//^6:0=10G_No MAC SWAP, 1=MAC SWAP ON
					if((RxBuffer[i] & 0x40) == 0x40){
						D(1, BUG("\nSwap MAC addr 10G\n"));
						SET_SWAP_10GA_TOGGLE |= 0x01;
						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
						D(1, BUG("10A:SET_SWAP_10GA_TOGGLE  STATE: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
						D(1, BUG("10A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |=0x40;
						}
					else{
						D(1, BUG("\nDisable Swap MAC addr 10G\n"));
						SET_SWAP_10GA_TOGGLE &= ~0x01;
						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
						D(1, BUG("10A:SET_SWAP_10GA_TOGGLE  STATE: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
						D(1, BUG("10A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &=~0x40;
						}
					}

				//*** SWAP IP ADDRESS ****************************
				if(((RxBuffer[i])&0x80) != ((ConfigStat10G[i])&0x80)){	//^7:0=No IP SWAP, 1=IP SWAP ON
					if((RxBuffer[i] & 0x80) == 0x80){
						D(1, BUG("\n swap IP\n"));
						SET_SWAP_10GA_TOGGLE |= 0x02;
						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
						D(1, BUG("10A:SET_SWAP_10GA_TOGGLE  STATE: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
						D(1, BUG("10A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |=0x80;
						}
					else{
						SET_SWAP_10GA_TOGGLE &= ~0x02;
						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
						D(1, BUG("\n disable swap IP\n"));
						D(1, BUG("10A:SET_SWAP_10GA_TOGGLE  STATE: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
						D(1, BUG("10A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &=~0x80;
						}
					}

				//remote loop up (debug/test/eval code////only)
				if((RxBuffer[i]& 0x04) != (ConfigStat10G[i]&0x04)){
					D(1, BUG("CORE_SIGROUTE_MODES_ptr result RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
					D(1, BUG("CORE_SIGROUTE_MODES_ptr result ConfigStat10G[MISC_ptr]:  %x\n",ConfigStat10G[CORE_SIGROUTE_MODES_ptr]));
					//ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
//							if((RxBuffer[i] & 0x04) == 0x04){
						D(1, BUG("remote loopup"));

						// Update MAC Address
						destination_10g_addr0 = BytesToLong(RxBuffer,TRAFFIC_HDR1_1_ptr);	// use GUI selected dest MAC addr
						destination_10g_addr1 = (unsigned int) BytesToInt(RxBuffer,TRAFFIC_HDR2_1_ptr);
						SETGENMONHDR_10GMAC_DEST_ADDR(destination_10g_addr1, destination_10g_addr0);

						SEND_LOOPBACK_PACKET();
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
//							}
//							else{
//								D(1, BUG("disable remote loopup"));
//							}
					D(1, BUG("RxBuffer[i]: %x\n", RxBuffer[i]));
					D(1, BUG("ConfigStat[sigroutes_ptr]:  %x\n",ConfigStat10G[CORE_SIGROUTE_MODES_ptr]));
					//ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
				}

				//remote loop down
				if((RxBuffer[i]& 0x08) != (ConfigStat10G[i]&0x08)){
					D(1, BUG("CORE_SIGROUTE_MODES_ptrresult RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
					D(1, BUG("CORE_SIGROUTE_MODES_ptr result ConfigStat10G[MISC_ptr]:  %x\n",ConfigStat10G[CORE_SIGROUTE_MODES_ptr]));
					//if((RxBuffer[i] & 0x80) == 0x80){
					destination_10g_addr0 = BytesToLong(RxBuffer,TRAFFIC_HDR1_1_ptr);	// use GUI selected dest MAC addr
					destination_10g_addr1 = (unsigned int) BytesToInt(RxBuffer,TRAFFIC_HDR2_1_ptr);
					SETGENMONHDR_10GMAC_DEST_ADDR(destination_10g_addr1, destination_10g_addr0);
					D(1, BUG("DEST_10G_ADDR0: %X\n", destination_10g_addr0));
					D(1, BUG("DEST_10G_ADDR1: %X\n", destination_10g_addr1));

//							source_10g_addr0 = BytesToLong(RxBuffer,EGEN_SRCMAC_MSB3_ptr);
//							source_10g_addr1 = (unsigned int) BytesToInt(RxBuffer,EGEN_SRCMAC_LSB1_ptr);
//							SETGENMONHDR_10GMAC_SOURCE_ADDR(source_10g_addr1, source_10g_addr0);
//							D(1, BUG("SRC_10G_ADDR0: %X\n", source_10g_addr0));
//							D(1, BUG("SRC_10G_ADDR1: %X\n", source_10g_addr1));

						D(1, BUG("remote loopdown"));
						SEND_LOOPBACK_PACKET();
					//}
//							else{
//								D(1, BUG("disable remote loopdown"));
//							}
					ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
				}

				break;

				case CLEAR_ptr:	//70
					if(((RxBuffer[i])&0x01) != ((ConfigStat10G[i])&0x01)) // if Clear Bit has been TOGGLED
						SaveBytesLong(ConfigStat10G,SECONDS3_ptr,0); //restart timer for new test
					if(((RxBuffer[i])&0x04) != ((ConfigStat10G[i])&0x04)){
						D(1, BUG("clear the stats \n"));
						CLR_10G_MAC_STATS();
						SaveBytesLong(ConfigStat10G,SECONDS3_ptr,0);//restart timer for clearing MAC stats
					}
					break;
				case MISC_ptr:	//71	DumpSFPAinfo^0 , DumpSFPBinfo^1
					// might need to check/monitor ConfigStat10GB to ensure Port setting is don't care.....
					/*D(1, BUG("consolidated result RxBuffer[MISC_ptr]:  %x\n",RxBuffer[MISC_ptr]));
										D(1, BUG("consolidated result ConfigStat[MISC_ptr]:  %x\n",ConfigStat[MISC_ptr]));*/
					if(((RxBuffer[i])&0x01) != ((ConfigStat10G[i])&0x01)){		// if DumpSFPAinfo
						READ_SFPA_DATA = YES;
						if(((RxBuffer[i])&0x01) == 0){	// maintain bit states across A, B and 10G
							ConfigStat[MISC_ptr] &= ~0x01;
							ConfigStatB[MISC_ptr] &= ~0x01;
							ConfigStat10G[MISC_ptr] &= ~0x01;
							}
						else{
							ConfigStat[MISC_ptr] |= 0x01;
							ConfigStatB[MISC_ptr] |= 0x01;
							ConfigStat10G[MISC_ptr] |= 0x01;
							}
						}
					if(((RxBuffer[i])&0x02) != ((ConfigStat10G[i])&0x02)){		// if DumpSFPBinfo
						READ_SFPB_DATA = YES;
						if(((RxBuffer[i])&0x02) == 0){	// maintain bit states across A, B and 10G
							ConfigStat[MISC_ptr] &= ~0x02;
							ConfigStatB[MISC_ptr] &= ~0x02;
							ConfigStat10G[MISC_ptr] &= ~0x02;
							}
						else{
							ConfigStat[MISC_ptr] |= 0x02;
							ConfigStatB[MISC_ptr] |= 0x02;
							ConfigStat10G[MISC_ptr] |= 0x02;
							}
						}
					if(((RxBuffer[i])&0x04) != ((ConfigStat10G[i])&0x04)){		// if DumpSFP+_A info
						READ_SFP10A_DATA = YES;
						if(((RxBuffer[i])&0x04) == 0){	// maintain bit states across A, B and 10G
							ConfigStat[MISC_ptr] &= ~0x04;
							ConfigStatB[MISC_ptr] &= ~0x04;
							ConfigStat10G[MISC_ptr] &= ~0x04;
							}
						else{
							ConfigStat[MISC_ptr] |= 0x04;
							ConfigStatB[MISC_ptr] |= 0x04;
							ConfigStat10G[MISC_ptr] |= 0x04;
							}
						}
/*
						if(((RxBuffer[i])&0x40) != ((ConfigStat10G[i])&0x40)){	// ^6 to send back the big packet
							D(1, BUG("consolidated result RxBuffer[MISC_ptr %x\n",RxBuffer[MISC_ptr]));
							D(1, BUG("consolidated result ConfigStat[MISC_ptr %x\n",ConfigStat10G[MISC_ptr]));
							D(1, BUG("Big packet SEND RxBuffer[MISC_ptr %x\n",RxBuffer[MISC_ptr]));
							if(MS_PKT_RCVD){SEND_MS_PACKET=YES;}
							if(((RxBuffer[i])&0x40) == 0){	// maintain bit states across A, B and 10G

							ConfigStat10G[MISC_ptr] &= ~0x40;
							}
							else{

							ConfigStat10G[MISC_ptr] |= 0x40;
						}
						}*/
					if(((RxBuffer[i])&0x80) != ((ConfigStat10G[i])&0x80)){//MISC_ptr ^7
						if((RxBuffer[i]&0x80)== 0x80){ // sending back consolidated result for MS.
							D(1, BUG("consolidated result RxBuffer[MISC_ptr %x\n",RxBuffer[MISC_ptr]));
							D(1, BUG("consolidated result ConfigStat[MISC_ptr %x\n",ConfigStat10G[MISC_ptr]));
							unsigned short s = 0;
							clearTotalResults();
							for(s=0;s<num_of_streams;s++) {read_mon_status(s);}//send back consolidated results
							D(1, BUG("\n ===== CONSOLIDATED RESULTS for number of streams:%x =======\n",num_of_streams));
							DBG_MON_STATS();
//							ConfigStat[MISC_ptr] = RxBuffer[i]&~0x80;
//							ConfigStatB[MISC_ptr] = RxBuffer[i]&~0x80;
//							ConfigStat10G[MISC_ptr] = RxBuffer[i]&~0x80;
						}
						else {
							stream_no=(unsigned short) (RxBuffer[TRAFFIC_NUM_OF_STREAMS]>>4)&0xF; // get which stream exactly and send the results corresponding to that stream
							//D(1, BUG("...selecting the exact stream %x\n",stream_no));
							clearTotalResults();
							read_mon_status(stream_no);
							D(1, BUG("\n===== Result STATS from MISC_ptr for stream %x =======\n",stream_no));
							DBG_MON_STATS();
						}

						ConfigStat10G[MISC_ptr] = RxBuffer[MISC_ptr];
					}
					break;

				case BERT_ptr:	//72
					break;

				case EGEN_NUM_SECS3_ptr:
				case EGEN_NUM_SECS2_ptr:
				case EGEN_NUM_SECS1_ptr:
				case EGEN_NUM_SECS0_ptr:
				//if(GIGE_A){
					if(BytesToLong(RxBuffer,EGEN_NUM_SECS3_ptr) != BytesToLong(ConfigStat10G,EGEN_NUM_SECS3_ptr)){
				//	if(BytesToInt(RxBuffer,EGEN_NUM_SECS1_ptr) != BytesToInt(ConfigStat,EGEN_NUM_SECS1_ptr)){
				gen_number_seconds = BytesToLong(RxBuffer,EGEN_NUM_SECS3_ptr); // save selected secs as well
				D(1, BUG("!!!!!!*****...Setting seconds to *********!!!!!!!%u\n",gen_number_seconds));
				SaveBytesLong(ConfigStat,EGEN_NUM_SECS3_ptr,gen_number_seconds);
				SaveBytesLong(ConfigStatB,EGEN_NUM_SECS3_ptr,gen_number_seconds);
				SaveBytesLong(ConfigStat10G,EGEN_NUM_SECS3_ptr,gen_number_seconds);

				  }
				break;

			case TRAFFIC_NUM_OF_STREAMS : //lower nibble is the number of streams - 0 to 15 = 16 streams //upper nibble is which stream exactly - used for sending results back
				//existing code for ms to run
				/*//for now let stream_no == num_of streams
				stream_no =((unsigned short) RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0x0F);
				D(1, BUG("...selecting the stream %d\n",stream_no));
				num_of_streams = stream_no+1;
				D(1, BUG("...total number of streams %d\n",num_of_streams));
			*/
				if((RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0x0F) != (ConfigStat10G[TRAFFIC_NUM_OF_STREAMS]&0x0F)){
					D(1, BUG("...RxBuffer[TRAFFIC_NUM_OF_STREAMS] %x\n",RxBuffer[TRAFFIC_NUM_OF_STREAMS]));
					num_of_streams=((unsigned char) RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0x0F) +1 ; // num_of_stream ==1 for stream 0
					D(1, BUG("...total number of streams %d\n",num_of_streams));
				}

				if((RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0xF0) != (ConfigStat10G[TRAFFIC_NUM_OF_STREAMS]&0xF0))
				{
					stream_no=(unsigned char) (RxBuffer[TRAFFIC_NUM_OF_STREAMS]>>4)&0xF; // get which stream exactly and send the results corresponding to that stream
					//D(1, BUG("...selecting the exact stream %x\n",stream_no));
					clearTotalResults();
					read_mon_status(stream_no);
					D(1, BUG("\n===== Result STATS for stream %x =======\n",stream_no));
					DBG_MON_STATS();
				}
			
				ConfigStat10G[TRAFFIC_NUM_OF_STREAMS] = RxBuffer[TRAFFIC_NUM_OF_STREAMS];
				break;

					//*** Unit to loop up with has been selected ***
				case LOOPBACK_ptr: //74
					D(1, BUG("\n\nLOOPBACK_PTR: %X\n", ConfigStat10G[LOOPBACK_ptr]));

					if(GIGE10){ // Currently TSE B-port ONLY
						// Clear Loopback
						if (((RxBuffer[i]) & 0x01) != ((ConfigStat10G[i]) & 0x01)) {
							  D(1, BUG("\nCLEAR LOOPBACK_PTR: %X\n", ConfigStat10G[LOOPBACK_ptr]));
							  D(1, BUG("\nCLEAR LOOPBACK_PTR: %X\n", RxBuffer[LOOPBACK_ptr]));
							  ConfigStat10G[LOOPBACKSTAT_ptr] = 0;
						}

						// When detect remote loopback is enabled, mask sniffer
						if ((RxBuffer[i] & 0x02) != (ConfigStat10G[i] & 0x02)){
							  if ((RxBuffer[LOOPBACK_ptr] & 0x02)==0x02) {
									DETECT_REMOTE_LOOPBACK_10GA = 1;
									D(1, BUG("\n detect LOOPBACK_PTR: %X\n", ConfigStat10G[LOOPBACK_ptr]));
									D(1, BUG("\nDETECT LOOPBACK: %d\n", DETECT_REMOTE_LOOPBACK_10GA));
							  }
							  else {
									DETECT_REMOTE_LOOPBACK_10GA = 0;
									D(1, BUG("\n detect LOOPBACK_PTR: %X\n", ConfigStat10G[LOOPBACK_ptr]));
									D(1, BUG("\nDETECT LOOPBACK: %d\n", DETECT_REMOTE_LOOPBACK_10GA));
							  }
						}

						if ((RxBuffer[i] & 0x04) != (ConfigStat10G[i] & 0x04)) {
							  SEND_HELLO_PACKET();
						}
					}
					break;

			/*	case SPARE_GEN_CONFIG01_ptr:
				case SPARE_GEN_CONFIG00_ptr:
													D(1, BUG("!!!!!!*****...Setting frame size from *********!!!!!!!%u\n",traffic_gen_pkt_length));

																							if(GIGE_A){
																								if(BytesToInt(RxBuffer, SPARE_GEN_CONFIG01_ptr) != BytesToInt(ConfigStat,SPARE_GEN_CONFIG01_ptr)){

																									//EGEN: A fixed length ranges between 24 to 9,600 bytes; a random length ranges between 24 and 1518
																									// 		we're default on fixed length packets
																									D(1, BUG("...Setting EGEN (TSE) Frame Size\n"));
																									traffic_gen_pkt_length = BytesToInt(RxBuffer, SPARE_GEN_CONFIG01_ptr);
																									if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
																										traffic_gen_pkt_length = 64;
																									SaveBytesInt(ConfigStat,SPARE_ptr_3,traffic_gen_pkt_length);
																									SaveBytesInt(ConfigStat10G,SPARE_ptr_3, traffic_gen_pkt_length);

																									if(traffic_gen_pkt_length > 1518)
																										SET_TRAFFIC_GEN_JUMBO_BY_STREAM(YES);
																									else
																									SET_TRAFFIC_GEN_JUMBO_BY_STREAM(NO);
																									SETGEN_PKTLENGTH_BY_STREAM(traffic_gen_pkt_length);
																									//config_frame_structure();	// rewrite Frame headers if necessary based on frame_type

																									//reset num_packets & load% calculations...........
																									SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
																									SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);	// called on Frame Size or FPS change
																									//ConfigStat[FPS_ptr] = egen_line_load_rate;
																									//ConfigStatB[FPS_ptr] = egen_line_load_rate;
																									//ConfigStat10GB[FPS_ptr] = egen_line_load_rate;
																									}
																								}
																							break;*/
			/*	case FPS_ptr:	//73		Update for NEW GEN
					egen_line_load_rate = RxBuffer[FPS_ptr];
					D(1, BUG("...Setting 10G EGEN FPS to %u\n",egen_line_load_rate));
					SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
					SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);	// called on Frame Size or FPS change
					ConfigStat[FPS_ptr] = egen_line_load_rate;
					ConfigStatB[FPS_ptr] = egen_line_load_rate;
					ConfigStat10GB[FPS_ptr] = egen_line_load_rate;
					break;*/

				//--------------------------------------------------------------------------------------
				//	TRAFFIC GENERATION POOP
				//--------------------------------------------------------------------------------------
	/*			case EGEN_CONTROL_ptr:	//75... // ^0=ON/OFF, ^1=RANDOM_LEN,  ^2=T1-payload, ^3=payload (0:inc, 1:PRBS),  ^5=ERR_INJ
					D(1, BUG("...Setting Fixed/Random Frame Length OR INC/PRBS payload\n"));
					if(((RxBuffer[i])&0x02) == 0x02) // Set Fixed or Random Packet Length as User selected!
						SET_TRAFFIC_GEN_RNDLEN_BY_STREAM(ON);
					else
						SET_TRAFFIC_GEN_RNDLEN_BY_STREAM(OFF);

					//reset num_packets & load% calculations...........
					//_______________________________________________________________
					if((traffic_gen_config&RNDLEN_ON) == RNDLEN_ON || traffic_gen_pkt_length > 1518){
						SETGEN_PKTLENGTH_BY_STREAM(9600);				// when EMIX is on don't clip frame sizes
						SET_TRAFFIC_GEN_JUMBO_BY_STREAM(YES);
						}
					else{
						SET_TRAFFIC_GEN_JUMBO_BY_STREAM(NO);
						SETGEN_PKTLENGTH_BY_STREAM(traffic_gen_pkt_length);
						}
					SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
					SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);	// called on Frame Size or FPS change
					ConfigStat[FPS_ptr] = egen_line_load_rate;
					ConfigStatB[FPS_ptr] = egen_line_load_rate;
					ConfigStat10GB[FPS_ptr] = egen_line_load_rate;
					//_______________________________________________________________

					if(((RxBuffer[i])&0x08) == 0x08) // Set INC or PRBS Payload as User selected!
						SET_TRAFFIC_GEN_RNDPAY_BY_STREAM(ON);
					else
						SET_TRAFFIC_GEN_RNDPAY_BY_STREAM(OFF);

					ConfigStat10G[EGEN_CONTROL_ptr] = RxBuffer[i];	// save state for proper GUI display whether in A or B port
					ConfigStat10GB[EGEN_CONTROL_ptr] = RxBuffer[i];
					ConfigStat[EGEN_CONTROL_ptr] = RxBuffer[i];	// save state for proper GUI display whether in A or B port
					ConfigStatB[EGEN_CONTROL_ptr] = RxBuffer[i];
					break;

				case EGEN_NUM_PACKETS3_ptr:	//76
				case EGEN_NUM_PACKETS2_ptr:	//77
				case EGEN_NUM_PACKETS1_ptr:	//78
				case EGEN_NUM_PACKETS0_ptr:	//79
				// Add in new case for Upper 8-bits UI_ptr - load into unsigned short "number_packet_msb"
					// Only process once for any word change!......
					// No EGEN on B ....so SETUP TEST Poop whether on Port A or B
					if(BytesToLong(RxBuffer,EGEN_NUM_PACKETS3_ptr) != BytesToLong(ConfigStat10G,EGEN_NUM_PACKETS3_ptr)){
						number_packet_lsb = BytesToLong(RxBuffer,EGEN_NUM_PACKETS3_ptr);
						SETGEN_STREAM_NUMPKTS(number_packet_lsb, 0); 		// DBUGmsg in function
						SETMON_TOTALNUMPKTS(number_packet_lsb, 0);		// set MON num pkts to expect
						SaveBytesLong(ConfigStat10G, EGEN_NUM_PACKETS3_ptr, number_packet_lsb);
						SaveBytesLong(ConfigStat10GB, EGEN_NUM_PACKETS3_ptr, number_packet_lsb);
						SaveBytesLong(ConfigStat, EGEN_NUM_PACKETS3_ptr, number_packet_lsb);
						SaveBytesLong(ConfigStatB, EGEN_NUM_PACKETS3_ptr, number_packet_lsb);

						gen_number_seconds = BytesToInt(RxBuffer,EGEN_NUM_SECS1_ptr); // save selected secs as well
						//reset num_packets calculations...........
						SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
						SaveBytesInt(ConfigStat10G,EGEN_NUM_SECS1_ptr,gen_number_seconds);
						SaveBytesInt(ConfigStat10GB,EGEN_NUM_SECS1_ptr,gen_number_seconds);
						SaveBytesInt(ConfigStat,EGEN_NUM_SECS1_ptr,gen_number_seconds);
						SaveBytesInt(ConfigStatB,EGEN_NUM_SECS1_ptr,gen_number_seconds);
						}
					break;

				//===================================================================
				case EGEN_SRCMAC_MSB3_ptr:	//86	// MAC MAC ADDR: eg 11-22-33-44-55-66
				case EGEN_SRCMAC_MSB2_ptr:	//87	// 10G different than TSE!!!
				case EGEN_SRCMAC_MSB1_ptr:	//88	// MSB3_ptr loaded into destination_tse_addr0 = lower32
				case EGEN_SRCMAC_MSB0_ptr:	//89  	// LSB1_ptr loaded into destination_tse_addr1 = upper16
				case EGEN_SRCMAC_LSB1_ptr:	//90
				case EGEN_SRCMAC_LSB0_ptr:	//91
					// ...this is the MAC address our EGEN is feeding! (TSE0)
					//MSB3:0 => source_tse_addr0 = 0x33445566;	// MAC: 11-22-33-44-55-66
					//LSB1:0 => source_tse_addr1 = 0x00001122;
					// Destination MAC only for our EGEN - A/B independent!
					******* So do nothing (in GUI) until we feel a reason to change it!
					*********
					if(BytesToLong(RxBuffer, EGEN_SRCMAC_MSB3_ptr) != BytesToLong(ConfigStat10G, EGEN_SRCMAC_MSB3_ptr) || BytesToInt(RxBuffer, EGEN_SRCMAC_LSB1_ptr) != BytesToInt(ConfigStat10G, EGEN_SRCMAC_LSB1_ptr)){
						source_10g_addr0 = BytesToLong(RxBuffer,EGEN_SRCMAC_MSB3_ptr);
						source_10g_addr1 = (unsigned int) BytesToInt(RxBuffer,EGEN_SRCMAC_LSB1_ptr);

						SaveBytesLong(ConfigStat10G, EGEN_SRCMAC_MSB3_ptr, source_10g_addr1);
						SaveBytesInt(ConfigStat10G, EGEN_SRCMAC_LSB1_ptr, source_10g_addr1);

						// written to Engine prior to Traffic test
						}

					break;
				//===================================================================
				case EGEN_DESTMAC_MSB3_ptr:	//86	// MAC MAC ADDR: eg 11-22-33-44-55-66
				case EGEN_DESTMAC_MSB2_ptr:	//87	// 10G different than TSE!!!
				case EGEN_DESTMAC_MSB1_ptr:	//88	// MSB3_ptr loaded into destination_tse_addr0 = lower32
				case EGEN_DESTMAC_MSB0_ptr:	//89  	// LSB1_ptr loaded into destination_tse_addr1 = upper16
				case EGEN_DESTMAC_LSB1_ptr:	//90
				case EGEN_DESTMAC_LSB0_ptr:	//91
					// ...this is the MAC address our EGEN is feeding! (TSE0)
					//MSB3:0 => dest_addr0 = 0x33445566;	// MAC: 11-22-33-44-55-66
					//LSB1:0 => dest_addr1 = 0x00001122;
					// Destination MAC only for our EGEN - A/B independent!
					if(BytesToLong(RxBuffer, EGEN_DESTMAC_MSB3_ptr) != BytesToLong(ConfigStat10G, EGEN_DESTMAC_MSB3_ptr) || BytesToInt(RxBuffer, EGEN_DESTMAC_LSB1_ptr) != BytesToInt(ConfigStat10G, EGEN_DESTMAC_LSB1_ptr)){
						destination_10g_addr0 = BytesToLong(RxBuffer,EGEN_DESTMAC_MSB3_ptr);	// use GUI selected dest MAC addr
						destination_10g_addr1 = (unsigned int) BytesToInt(RxBuffer,EGEN_DESTMAC_LSB1_ptr);

						SaveBytesLong(ConfigStat10G, EGEN_DESTMAC_MSB3_ptr, destination_10g_addr0);
						SaveBytesInt(ConfigStat10G, EGEN_DESTMAC_LSB1_ptr, destination_10g_addr1);

						// written to Engine prior to Traffic test
						}
					break;


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
				case EGEN_VLAN1_TCI1_ptr:	//  92	// VLAN-1 TCI: PCP,DEI,ID  (3-bits,1-bit,12-bits)
				case EGEN_VLAN1_TCI0_ptr:	//  93	// if 1-VLAN then 8100+TCI, if stacked-VLAN then 88A8+TCI
				case EGEN_VLAN2_TCI1_ptr:	// 	94	// VLAN-2 TCI: PCP,DEI,ID  (3-bits,1-bit,12-bits)
				case EGEN_VLAN2_TCI0_ptr:	// 	95	// if 1-VLAN then 8100+TCI
				case EGEN_PKT_TYPE_ptr: 		// 100
					config_frame_structure();
					break;

				case EGEN_IP1_HEADER3_ptr:
				case EGEN_IP1_HEADER2_ptr:
				case EGEN_IP1_HEADER1_ptr:
				case EGEN_IP1_HEADER0_ptr:
				//
				case EGEN_IP2_HEADER3_ptr:
				case EGEN_IP2_HEADER2_ptr:
				case EGEN_IP2_HEADER1_ptr:
				case EGEN_IP2_HEADER0_ptr:
				//
				case EGEN_IPV4_HDR3_ptr:
				case EGEN_IPV4_HDR2_ptr:
				case EGEN_IPV4_HDR1_ptr:
				case EGEN_IPV4_HDR0_ptr:
				//
				case EGEN_IPV4_SRC3_ptr:
				case EGEN_IPV4_SRC2_ptr:
				case EGEN_IPV4_SRC1_ptr:
				case EGEN_IPV4_SRC0_ptr:

				//IPv6

				case EGEN_SRC3_IP3_ptr:
				case EGEN_SRC3_IP2_ptr:
				case EGEN_SRC3_IP1_ptr:
				case EGEN_SRC3_IP0_ptr:

				case EGEN_SRC4_IP3_ptr:
				case EGEN_SRC4_IP2_ptr:
				case EGEN_SRC4_IP1_ptr:
				case EGEN_SRC4_IP0_ptr:

				case EGEN_DEST1_IP3_ptr:
				case EGEN_DEST1_IP2_ptr:
				case EGEN_DEST1_IP1_ptr:
				case EGEN_DEST1_IP0_ptr:

				case EGEN_DEST2_IP3_ptr:
				case EGEN_DEST2_IP2_ptr:
				case EGEN_DEST2_IP1_ptr:
				case EGEN_DEST2_IP0_ptr:

				case EGEN_DEST3_IP3_ptr:
				case EGEN_DEST3_IP2_ptr:
				case EGEN_DEST3_IP1_ptr:
				case EGEN_DEST3_IP0_ptr:

				case EGEN_DEST4_IP3_ptr:
				case EGEN_DEST4_IP2_ptr:
				case EGEN_DEST4_IP1_ptr:
				case EGEN_DEST4_IP0_ptr:

						// check Words for any byte change and save new word - so we don't run through multiple times for same change
						if(BytesToLong(RxBuffer, EGEN_IP1_HEADER3_ptr) != BytesToLong(ConfigStat, EGEN_IP1_HEADER3_ptr))
							SaveBytesLong(ConfigStat,EGEN_IP1_HEADER3_ptr, BytesToLong(RxBuffer,EGEN_IP1_HEADER3_ptr));
						if(BytesToLong(RxBuffer, EGEN_IP2_HEADER3_ptr) != BytesToLong(ConfigStat, EGEN_IP2_HEADER3_ptr))
							SaveBytesLong(ConfigStat,EGEN_IP2_HEADER3_ptr, BytesToLong(RxBuffer,EGEN_IP2_HEADER3_ptr));
						if(BytesToLong(RxBuffer, EGEN_IPV4_HDR3_ptr) != BytesToLong(ConfigStat, EGEN_IPV4_HDR3_ptr))
							SaveBytesLong(ConfigStat,EGEN_IPV4_HDR3_ptr, BytesToLong(RxBuffer,EGEN_IPV4_HDR3_ptr));
						if(BytesToLong(RxBuffer, EGEN_IPV4_SRC3_ptr) != BytesToLong(ConfigStat, EGEN_IPV4_SRC3_ptr))
							SaveBytesLong(ConfigStat,EGEN_IPV4_SRC3_ptr, BytesToLong(RxBuffer,EGEN_IPV4_SRC3_ptr));

						if(BytesToLong(RxBuffer, EGEN_SRC1_IP3_ptr) != BytesToLong(ConfigStat, EGEN_SRC1_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_SRC1_IP3_ptr, BytesToLong(RxBuffer,EGEN_SRC1_IP3_ptr));
						if(BytesToLong(RxBuffer, EGEN_SRC2_IP3_ptr) != BytesToLong(ConfigStat, EGEN_SRC2_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_SRC2_IP3_ptr, BytesToLong(RxBuffer,EGEN_SRC2_IP3_ptr));
						if(BytesToLong(RxBuffer, EGEN_SRC3_IP3_ptr) != BytesToLong(ConfigStat, EGEN_SRC3_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_SRC3_IP3_ptr, BytesToLong(RxBuffer,EGEN_SRC3_IP3_ptr));
						if(BytesToLong(RxBuffer, EGEN_SRC4_IP3_ptr) != BytesToLong(ConfigStat, EGEN_SRC4_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_SRC4_IP3_ptr, BytesToLong(RxBuffer,EGEN_SRC4_IP3_ptr));

						if(BytesToLong(RxBuffer, EGEN_DEST1_IP3_ptr) != BytesToLong(ConfigStat, EGEN_DEST1_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_DEST1_IP3_ptr, BytesToLong(RxBuffer,EGEN_DEST1_IP3_ptr));
						if(BytesToLong(RxBuffer, EGEN_DEST2_IP3_ptr) != BytesToLong(ConfigStat, EGEN_DEST2_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_DEST2_IP3_ptr, BytesToLong(RxBuffer,EGEN_DEST2_IP3_ptr));
						if(BytesToLong(RxBuffer, EGEN_DEST3_IP3_ptr) != BytesToLong(ConfigStat, EGEN_DEST3_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_DEST3_IP3_ptr, BytesToLong(RxBuffer,EGEN_DEST3_IP3_ptr));
						if(BytesToLong(RxBuffer, EGEN_DEST4_IP3_ptr) != BytesToLong(ConfigStat, EGEN_DEST4_IP3_ptr))
							SaveBytesLong(ConfigStat,EGEN_DEST4_IP3_ptr, BytesToLong(RxBuffer,EGEN_DEST4_IP3_ptr));
					//.....If any IP parameter changes - update Traffic Engine accordingly!
					//_________________EGEN_PKT_TYPE_ptr____________________
					// 0000,0000:  ^10:Ethernet, ^32:VLAN, ^54:IP, ^76:future    VLAN=0x04, SVLAN=0x08, IPv4=0x10,14,18  IPv6=0x20,24,28
					// 10: 00=std Layer2-Ethernet, 01=EthernetII, 10=802.3 LLC, 11=802.3SNAP
					// 32: 00=none, 01=VLAN, 10=StackedVLAN, 11:future 3-VLAN's
					// 54: 00=none, 01=IPv4, 10=IPv6
					// 76: future (compatibility modes)
					if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x30) == 0x10){	// IPv4
						if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x10){		// Straight IP - no VLAN
							config_frame_ipv4();
							}
						else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x14){	// IPv4+VLAN
							config_frame_ipv4_vlan();	 // VLAN + IPv4
							}
						else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x18){	// IPv4+Stacked-VLAN
							config_frame_ipv4_stacked_vlan();
							}
						//DUMP_GEN_HDRS_BY_STREAM();
						//DUMP_MON_HDRS_BY_STREAM();
						//DUMP_MON_MASK_BY_STREAM();
						}
					else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x30) == 0x20){		// IPv6
													if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x20){			// Straight IP - no VLAN
														D(1, BUG("config10_hardware:___Going to Generate IPv6 Packets___\n"));
														//SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_IP6);
														// TODO	setup HDR's as necessary
														config_frame_ipv6();
														}
													else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x24){	// IPv6+VLAN
													//	D(1, BUG("___Generate VLAN-tagged IPv6 Packets___\n"));
													//	SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_VLANIP6);
														// TODO	setup HDR's as necessary
														config_frame_ipv6_vlan();
														}
													else if((RxBuffer[EGEN_PKT_TYPE_ptr]&0x3E) == 0x28){	// IPv6+Stacked-VLAN
														//D(1, BUG("___Generate Stacked-VLAN-tagged IPv6 Packets___\n"));
														//SET_TRAFFIC_GEN_HDRLEN_BY_STREAM(HDRLEN_SVLANIP6);
														// TODO	setup HDR's as necessary
														config_frame_ipv6_stacked_vlan();
														}
													D(1, BUG("___config_hardware 10 10 10 :Going to Generate IPv6 Packets___\n"));
																DUMP_GEN_HDRS_BY_STREAM();
																DUMP_MON_HDRS_BY_STREAM();
																DUMP_MON_MASK_BY_STREAM();
					}
					break;

				case EGEN_PKT_LENGTH1_ptr:	//144
				case EGEN_PKT_LENGTH0_ptr:	//145
					// Only process once for any word change!......
					//D(1, BUG("...Checking if we should set 10G EGEN Frame Size\n"));
					if(BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr) != BytesToInt(ConfigStat10G, EGEN_PKT_LENGTH1_ptr)){
						//EGEN: A fixed length ranges between 24 to 9,600 bytes; a random length ranges between 24 and 1518
						// 		we're default on fixed length packets
						traffic_gen_pkt_length = BytesToInt(RxBuffer, EGEN_PKT_LENGTH1_ptr);
						if(traffic_gen_pkt_length  == 0)	// protect from no GUI input
							traffic_gen_pkt_length = 64;
						D(1, BUG("...Setting EGEN (10G) Frame Size to %u\n",traffic_gen_pkt_length));
						SaveBytesInt(ConfigStat10G, EGEN_PKT_LENGTH1_ptr, traffic_gen_pkt_length);
						SaveBytesInt(ConfigStat,EGEN_PKT_LENGTH1_ptr, traffic_gen_pkt_length);

						if(traffic_gen_pkt_length > 1518)
							SET_TRAFFIC_GEN_JUMBO_BY_STREAM(YES);
						else
							SET_TRAFFIC_GEN_JUMBO_BY_STREAM(NO);
						SETGEN_PKTLENGTH_BY_STREAM(traffic_gen_pkt_length);

						config_frame_structure();	// rewrite Frame headers if necessary based on frame_type

						//reset num_packets & load% calculations...........
						SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
						SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);							// called on Frame Size, FPS change or SPEED change
						ConfigStat[FPS_ptr] = egen_line_load_rate;
						ConfigStatB[FPS_ptr] = egen_line_load_rate;
						ConfigStat10GB[FPS_ptr] = egen_line_load_rate;
						}
					break;*/

				case STARTSTOP_ptr:		//148 Test Start/Stop & status
					//*** THIS IS REALLY START/STOP a TEST *****
					//....only Port A
					if(RxTestStartGuiBit == 0x10){				// if Test = START

						SET_TRAFFIC_TO_10G;	// point GEN & MON to 10G port-A
						D(1, BUG(">> TRAFFIC ENGINE switched to 10G-A port!\n"));

						gCUSTOM_TEST_START = YES;
						ConfigStat10G[STARTSTOP_ptr] = RxTestStartToggle;
						ConfigStat10GB[STARTSTOP_ptr] = RxTestStartToggle;
						ConfigStat10G[STARTSTOP_ptr] |= 0x20|0x10;	// reflect MOD start and return GUI start
						ConfigStat10GB[STARTSTOP_ptr] |= 0x20|0x10;

						ConfigStat[STARTSTOP_ptr] = RxTestStartToggle;
						ConfigStatB[STARTSTOP_ptr] = RxTestStartToggle;
						ConfigStat[STARTSTOP_ptr] |= 0x20|0x10;	// reflect MOD start and return GUI start
						ConfigStatB[STARTSTOP_ptr] |= 0x20|0x10;
						D(1, BUG(">> Start TEST (10G) cmd rcvd (A) Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));

						}
					else if(RxTestStartGuiBit == 0x00){ 	// if Test = STOP
						if(TestStartModBit10){			// if test still running
							gCUSTOM_TEST_STOP = YES;
							D(1, BUG(">> Stop TEST (10G) cmd rcvd (A) Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));
							}
						else{										// else if test has stopped...
							ConfigStat10G[STARTSTOP_ptr] = 0;	// cleanUp test flag
							ConfigStat10GB[STARTSTOP_ptr] = 0;	// cleanUp test flag

							ConfigStat[STARTSTOP_ptr] = 0;		// cleanUp test flag
							ConfigStatB[STARTSTOP_ptr] = 0;		// cleanUp test flag
							D(1, BUG(">> TEST (10G) done (0x00) GUI rcvd (A) Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));

						}
						}
					break;

				default:
					break;
 				}//End Switch
			}//End if NEW Config byte (command)
		}//End for

	//=== if Config Updated Copy Rxbuffer into Config portion of ConfigStat10G array
	for(i=2;i<MaxConfig2-1;i++){    // load new config into ConfigStat10G
		if(i != STARTSTOP_ptr){		// STARTSTOP handled like LINK config byte
			if(GIGE10)
				ConfigStat10G[i] = RxBuffer[i];
			//else
			//	ConfigStat10GB[i] = RxBuffer[i];
			}
 		}

	// ************************************************************************
}/* End Config hardware function */


/*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, USA.    *
* All rights reserved.                                                   		*
*************************************************************************/
