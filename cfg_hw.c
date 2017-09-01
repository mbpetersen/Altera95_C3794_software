/*************************************************************************
*		CONFIG MODULE; Process Updated CONFIG array data from GUI		 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"

//********************************************************************************
// System Configuration
//		Execute Configuration & Test changes as requested by the GUI
//********************************************************************************
void config_hardware()
{
unsigned short i=0;
unsigned char temp=0;
	for(i=2;i<ConfigChecksum2;i++){   // *** Start with GIGEMODE
		//if(RxBuffer[i] != ConfigStat[i]){
		if((GIGE_A && RxBuffer[i] != ConfigStat[i]) || (!GIGE_A && RxBuffer[i] != ConfigStatB[i])){
//			D(1, BUG("cfg_hw changed state: %d \told: %x \tnew: %x\n", i, ConfigStat[i], RxBuffer[i]));
			switch(i){
				// ***************************************************************
				// **** GIGE Mode  ***
				//	 ModeCopper10M 	0	// new Mode
				//	 ModeCopper100M 	1
				//	 ModeCopper1G 	2
				//	 ModeFiber1G 		3
				//	 ModeFiber10G 		4
				//	 ModeFiber100M 	5
				//   ModeFilber100G	6
				// TSE speed changes handle by TSE MAC & PHY config bytes
				// 10G/100G speeds handled by specific LINK packets
				// ***************************************************************
				case GIGEMODE_ptr:
					temp = RxBuffer[i];			// if mode is different
					D(1, BUG("\n******************* TSE MODE Changed: %0d\n",temp));
					//if((temp&0x01) == mode_copper){ 	// if COPPER MODE
					if(temp <= ModeCopper1G){ 	// if COPPER MODE
						if(GIGE_A){
							D(1, BUG("PHYA copper: "));
							select_copper_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
							//?? fiber_power_down(TRIPLE_SPEED_ETHERNET_0_BASE);
							if(ConfigStat[i] > 2)		// if we were in Fiber...
								RxBuffer[PHY_CONFIG0_ptr]	 |= 	(unsigned char) PHY_CONFIG_RESTART_ANEG_MASK;	// kick PHY_CONFIG to configure PHY
							}
						else{
							D(1, BUG("PHYB copper: "));
							select_copper_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
							//?? fiber_power_down(TRIPLE_SPEED_ETHERNET_1_BASE);
							if(ConfigStatB[i] > 2)		// if we were in Fiber...
								RxBuffer[PHY_CONFIG0_ptr]	 |= 	(unsigned char) PHY_CONFIG_RESTART_ANEG_MASK;	// kick PHY_CONFIG to configure PHY
							}
						}
					//else{	// mode_fiber
					///TODO
					else if(temp == ModeFiber1G || temp == ModeFiber100M) {
						if(GIGE_A) {
							D(1, BUG("PHYA fiber: "));
							select_fiber_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
							//?? copper_power_down(TRIPLE_SPEED_ETHERNET_0_BASE);
							if(ConfigStat[i] < 3)		// if we're switching from Copper into Fiber
								RxBuffer[PHY_CONFIG0_ptr] |= (unsigned char) PHY_CONFIG_RESTART_ANEG_MASK;	// kick PHY_CONFIG to configure PHY
						}
						else {
							D(1, BUG("PHYB fiber: "));
							select_fiber_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
							//?? copper_power_down(TRIPLE_SPEED_ETHERNET_1_BASE);
							if(ConfigStatB[i] < 3)		// if we're switching from Copper into Fiber
								RxBuffer[PHY_CONFIG0_ptr] |= (unsigned char) PHY_CONFIG_RESTART_ANEG_MASK;	// kick PHY_CONFIG to configure PHY
						}
					}
					//Display_Port_Status = YES; // kick display output on change
					break;
					//*!!! Factory issued HW/configuration settings !!!*/

				case OPTIONS_INSTALLED_ptr:
					// The Flash has stored the last setting, which is recovered to the variables OPTIONS_INSTALLED
					// when the GUI modifies these - tweak them here and then the WRITE flash will load the updated
					// settings into the flash!
					OPTIONS_INSTALLED = RxBuffer[i];
					D(1, BUG("cfg_hw set: OPTIONS Installed:%04X Enabled:%04X (ActvL)\n",OPTIONS_INSTALLED, OPTIONS_ENABLED));
					FlashParameters[PARA_OPTIONS_INSTALLED_ptr] = OPTIONS_INSTALLED; //cc here for subsequent Flash write
					break;

				case OPTIONS_ENABLED_ptr:
					OPTIONS_ENABLED = RxBuffer[i];
					D(1, BUG("cfg_hw set: OPTIONS Installed:%04X Enabled:%04X (ActvL)\n",OPTIONS_INSTALLED, OPTIONS_ENABLED));
					FlashParameters[PARA_OPTIONS_ENABLED_ptr] = OPTIONS_ENABLED; //cc here for subsequent Flash write
					break;

				// ^0: Read Flash command & refresh SysInfo dump to GUI
				// ^1: Write FlashParameters into Flash (pre-erases block)
				// ^32: Erase Flash: 01=parameters[0x04], 10=FW (sof & elf)[0x08], 11=all (including PHY) [0x0C]
				case SYSCONFIG_ptr: // from A on boot or by hidden Factory/Engr access
					D(1, BUG("_______________________SYSCONFIG_ptr = 0x%0X_____________________________ \n",RxBuffer[i]));

					D(1, BUG("_______________________SYSCONFIG_ptr = 0x%0X_____________________________ \n",RxBuffer[i]));



				//	D(1, BUG("_______________________CONFIGSTAT = 0x%0X_____________________________ \n",ConfigStat[i])&0x0C);

					if(((RxBuffer[i])&0x01) == 0x01 && ((ConfigStat[i])&0x01)==0x00) {
						D(1, BUG("SYSCONFIG: READING FLASH \n"));
						read_flash_parameters();					// read System Parameters from Flash
						D(1, BUG("SYSCONFIG: LOADING FLASH TO SYSVARS \n"));
						load_flashparameters_into_systemvars();	// Load them into the System var's
						RxBuffer[i] |= 0x80;
					}

					// GUI should NOT issue write command until after FlashParameters have been set
					if(((RxBuffer[i])&0x02) == 0x02 && ((ConfigStat[i])&0x02)==0x00) {
						D(1, BUG("SYSCONFIG: WRITING FLASH \n"));
						D(1, BUG("buffer value = 0x%0x \n",RxBuffer[i]));
						D(1, BUG("buffer value = 0x%0x \n",RxBuffer[i]));
						D(1, BUG("buffer value = 0x%0x \n",RxBuffer[i]));
						write_flash_parameter_block();
						RxBuffer[i] |= 0x80;
					}

					//08.2015   parameters: top block 127, phy at 126, spare at 124 and 125, sof starts at 0 with the ELF on top of that
					//	case FLASH_PARAMETERS_BLOCK	0
					//	case FLASH_PHY_BLOCK				1
					//	case FLASH_USER1_BLOCK			2
					//	case FLASH_USER2_BLOCK			3
					//	case FLASH_SOF_ELF_BLOCKS		4 	//erase from 0 through user2 block
					//if(((RxBuffer[i])&0x0C) != 0 && ((ConfigStat[i])&0x0C)==0x00){
					if(((RxBuffer[i])&0x0C) != ((ConfigStat[i])&0x0C)) {
						D(1, BUG("Inside Erasing Block: SYSCONFIG_ptr = 0x%0X \n",RxBuffer[i]));

						if(((RxBuffer[i])&0x0C) == 0x04){		// erase parameters: block 127 of 128 total
							D(1, BUG("SYSCONFIG: ERASING FLASH PARAMETERS\n"));
							erase_flash_block(FLASH_PARAMETERS_BLOCK);
							}
						else if(((RxBuffer[i])&0x0C) == 0x08){		// erase only FW (sof + elf) - not PHY or spare:  0->123 (leave 124,125,126 and 127)
							D(1, BUG("SYSCONFIG: ERASING FLASH SOF+ELF (params) \n"));
							//TODO commented out for now - protecting accidental erasure of FPGA!
							//TODO until we get flash reprogram functioning - there's no point.
							erase_flash_block(FLASH_PARAMETERS_BLOCK);
							//erase_flash_block(FLASH_SOF_ELF_BLOCKS);
							}
						else if(((RxBuffer[i])&0x0C) == 0x0C){		// erase all but parameters: 0-126 (leave 127)
							D(1, BUG("SYSCONFIG: ERASING FLASH  ALL but PARAMETERS (params)\n"));
							//erase_flash_block(FLASH_SOF_ELF_BLOCKS);
							//erase_flash_block(FLASH_PHY_BLOCK);
							}
						RxBuffer[i] |= 0x80; // flag GUI to clear SYSCONFIG for subsequent command
					}

					break;
				case TEST_OPTIONS_PTR:
					//if(GIGE_A){
					D(1, BUG("TEST_OPTIONS_PTR: %04X\n", RxBuffer[i]));
					if (RxBuffer[i] != ConfigStat[i]) {
						test_options = RxBuffer[i];
						if (test_options == 0x00) {
							D(1, BUG("QUICK TEST_OPTIONS_PTR:%04X\n", RxBuffer[i]));
						}
						else if (test_options == 0x01) {
							D(1, BUG("MS TEST_OPTIONS_PTR:%04X\n", RxBuffer[i]));
						}
					}
					ConfigStat[TEST_OPTIONS_PTR] = RxBuffer[TEST_OPTIONS_PTR];
					ConfigStatB[TEST_OPTIONS_PTR] = RxBuffer[TEST_OPTIONS_PTR];
					break;
				case MAC_CONFIG3_ptr:	//4
				case MAC_CONFIG2_ptr:	//5
				case MAC_CONFIG1_ptr:	//6
				case MAC_CONFIG0_ptr:	//7
					// Only process once for any word change!......
					if(GIGE_A) {
						if(BytesToLong(RxBuffer, MAC_CONFIG3_ptr) != BytesToLong(ConfigStat, MAC_CONFIG3_ptr)){
							MACA_COMMAND_CONFIG = BytesToLong(RxBuffer, MAC_CONFIG3_ptr);
							//D(1, BUG("\n...Changing TSE Port-A MAC settings\n"));
							configure_tse_mac(TRIPLE_SPEED_ETHERNET_0_BASE);
							configure_tse0_to_phya();	// run this after configuring the MAC
						}
					}
					else {
						if(BytesToLong(RxBuffer, MAC_CONFIG3_ptr) != BytesToLong(ConfigStatB, MAC_CONFIG3_ptr)) {
							MACB_COMMAND_CONFIG = BytesToLong(RxBuffer, MAC_CONFIG3_ptr);
							//D(1, BUG("\n...Changing TSE Port-B MAC settings\n"));
							configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
							configure_tse1_to_phyb();	// run this after configuring the MAC
						}
					}
					break;
				case MAC_FRAME_LEN1_ptr:	//8 .....MTU size of our MAC's
				case MAC_FRAME_LEN0_ptr:	//9
					// NOT CURRENTLY USED: since GUI does not change this parameter - (effectively the MTU size of test-set) ****
					//todo the change in the value need not change the MTU size of our unit. It remains at 9600
					/*if(GIGE_A){
						if(BytesToInt(RxBuffer, MAC_FRAME_LEN1_ptr) != BytesToInt(ConfigStat, MAC_FRAME_LEN1_ptr)){
							D(1, BUG("...Setting MAC-A Frame Size\n"));
							MAC_FRM_LENGTH = BytesToInt(RxBuffer, MAC_FRAME_LEN1_ptr);  //was FRM_LENGTH MAC max is 9600
							if(MAC_FRM_LENGTH  == 0)	// protect from no GUI input
								MAC_FRM_LENGTH = 9600;  // was 1518
							SaveBytesInt(ConfigStat,MAC_FRAME_LEN1_ptr,MAC_FRM_LENGTH);


							configure_tse_mac(TRIPLE_SPEED_ETHERNET_0_BASE);
							configure_tse0_to_phya();	// run this after configuring the MAC

							//EGEN: A fixed length ranges between 24 to 9,600 bytes; a random length ranges between 24 and 1518
							// 		we're default on fixed length packets
							}
						}
					else{
						if(BytesToInt(RxBuffer, MAC_FRAME_LEN1_ptr) != BytesToInt(ConfigStatB, MAC_FRAME_LEN1_ptr)){
							D(1, BUG("...Setting MAC-B Frame Size\n"));
							MAC_FRM_LENGTH = BytesToInt(RxBuffer, MAC_FRAME_LEN1_ptr);  //was FRM_LENGTH MAC max is 9600
							if(MAC_FRM_LENGTH  == 0)	// protect from no GUI input
								MAC_FRM_LENGTH = 9600;
							SaveBytesInt(ConfigStatB,MAC_FRAME_LEN1_ptr,MAC_FRM_LENGTH);


							configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
							configure_tse1_to_phyb();	// run this after configuring the MAC

							//EGEN: A fixed length ranges between 24 to 9,600 bytes; a random length ranges between 24 and 1518
							// 		we're default on fixed length packets
							}
						}*/
					break;
				//===================================================================
				//**** Accessed by SYS_CONFIG Factory menu *******************
				// * usually will be our source_addr for the EGEN as well!
				case MAC_ADDR_MSB03_ptr:	//14	// MAC MAC ADDR: eg 00-1C-23-17-4A-CB
				case MAC_ADDR_MSB02_ptr:	//15	// these are 0x001C23174ACB
				case MAC_ADDR_MSB01_ptr:	//16	// MSB3_ptr loaded into mac_0 = 0x17231C00  (reverse order)
				case MAC_ADDR_MSB00_ptr:	//17  	// LSB1_ptr loaded into mac_1 = 0x0000CB4A
				case MAC_ADDR_LSB11_ptr:	//18
				case MAC_ADDR_LSB10_ptr:	//19
					// OLD - GUISYS unused MAC block....
					//old MSB3:0 => MAC_0x = 0xAF250000;	// TSE MAC: 00-00-25-AF-xx-yy   PortB: BF
					//old LSB1:0 => MAC_1x = 0x0000yyxx;	// 10G MAC: 00-00-25-AA-xx-yy	PortB: BB
					//NEW:  Greenlee MAC block:  50-64-41
					//MSB3:0 => MAC_0x = 0x0A416450;	// TSE MAC: 50-64-41-0A-xx-yy   PortB: 0B
					//LSB1:0 => MAC_1x = 0x0000yyxx;	// 10G MAC: 50-64-41-1A-xx-yy	PortB: 1B

					if(GIGE_A) {
						// NOTE: we get into this twice - for some reason, although we're saving to ConfigStat!  ?? causes dbl mac(s) inits
						if(BytesToLong(RxBuffer, MAC_ADDR_MSB03_ptr) != BytesToLong(ConfigStat, MAC_ADDR_MSB03_ptr) || BytesToInt(RxBuffer, MAC_ADDR_LSB11_ptr) != BytesToInt(ConfigStat, MAC_ADDR_LSB11_ptr)) {
							//old TSE MAC A: 00-00-25-AF-xx-yy  MSB[3:0]=AF,25,00,00  LSB[1:0]=yy,xx
							//old TSE MAC B: 00-00-25-BF-xx-yy
							// TSE MAC A: 50-64-41-0A-xx-yy  MSB[3:0]=0A,41,64,50  LSB[1:0]=yy,xx
							// TSE MAC B: 50-64-41-0B-xx-yy

							MAC_0 = BytesToLong(RxBuffer,MAC_ADDR_MSB03_ptr);
							MAC_1 = (unsigned int) BytesToInt(RxBuffer,MAC_ADDR_LSB11_ptr);

							SaveBytesLong(ConfigStat,MAC_ADDR_MSB03_ptr,MAC_0);
							SaveBytesInt(ConfigStat,MAC_ADDR_LSB11_ptr,MAC_1);

							//old RxBuffer[MAC_ADDR_MSB3_ptr] += 0x10;  // set Port-B MAC to 00-00-25-BF-xx-xx (from A's 00-00-25-AF-xx-xx)
							//NEW: Greenlee MAC's
							RxBuffer[MAC_ADDR_MSB03_ptr] += 0x01;  // set Port-B MAC to 50-61-41-0B-xx-xx (from A's 50-64-41-0A-xx-xx)
							MAC_0B = BytesToLong(RxBuffer,MAC_ADDR_MSB03_ptr);
							MAC_1B = (unsigned int) BytesToInt(RxBuffer,MAC_ADDR_LSB11_ptr);

							//--------------------------------------------------------------------
							// Since TSE GUI scrambles MAC Addresses we must
							//    descramble the array elements to unscrambled 10G MAC ADDR to normal!
							//--------------------------------------------------------------------
							// TSE MAC A: 00-00-25-AF-xx-yy  in array like this:   MSB[3:0]=AF,25,00,00  LSB[1:0]=yy,xx
							//old 10G MAC A: 00-00-25-AA-xx-yy  must convert to this: LSB[1:0]=00,00  MSB[3:0]=25,AA,xx,yy
							//old 10G MAC B: 00-00-25-BB-xx-yy
							//NEW Greenlee MAC's
							// TSE MAC A: 50-64-41-0A-xx-yy  in array like this:   MSB[3:0]=0A,41,64,50  LSB[1:0]=yy,xx
							// 10G MAC A: 50-64-41-1A-xx-yy  must convert to this: LSB[1:0]=50,64  MSB[3:0]=41,0A,xx,yy
							// 10G MAC B: 50-64-41-1B-xx-yy (future 2nd 10G port, not flashing now...)

							//old RxBuffer[MAC_ADDR_MSB3_ptr] -= 0x15; // return to 'AA' for 10G portA (TSE-B:0xBF-0x15=0xAA)
							//NEW Greenlee MAC's
							RxBuffer[MAC_ADDR_MSB03_ptr] += 0x0F; // return to '1A' for 10G portA (TSE-B:0x0B+0x0F=0x1A)

							unsigned tmpLong = 0;
							tmpLong = (unsigned long)  RxBuffer[MAC_ADDR_MSB02_ptr]<<24;
							tmpLong += (unsigned long) RxBuffer[MAC_ADDR_MSB03_ptr]<<16;
							tmpLong += (unsigned long) RxBuffer[MAC_ADDR_LSB10_ptr]<<8;
							tmpLong += (unsigned long) RxBuffer[MAC_ADDR_LSB11_ptr];

							unsigned int tmpInt = 0;
							//RxBuffer[MAC_ADDR_LSB0_ptr] = RxBuffer[MAC_ADDR_MSB1_ptr];
							//RxBuffer[MAC_ADDR_LSB1_ptr] = RxBuffer[MAC_ADDR_MSB0_ptr];
							tmpInt = RxBuffer[MAC_ADDR_MSB01_ptr];
							tmpInt += (RxBuffer[MAC_ADDR_MSB00_ptr])*0x100;

							//MAC10G_0 = BytesToLong(RxBuffer,MAC_ADDR_MSB3_ptr);
							//MAC10G_1 = (unsigned int) BytesToInt(RxBuffer,MAC_ADDR_LSB1_ptr);
							MAC10G_0 = tmpLong;
							MAC10G_1 = tmpInt;

//11-14 comment out until add 2nd 10G port!
//old						MAC10G_0B = tmpLong + 0x00110000;	// set Port-B MAC to 00-00-25-BB-xx-xx (from A's 00-00-25-AA-xx-xx)
//Greenlee MAC's			MAC10G_0B = tmpLong + 0x00010000;	// set Port-B MAC to 50-64-41-1B-xx-xx (from A's 50-64-41-1A-xx-xx)
//							MAC10G_1B = tmpInt;
							D(1, BUG("...Setting 10G Port-A MAC Addr: %x %x\n",MAC10G_0,MAC10G_1));
//							D(1, BUG("...Setting 10G Port-B MAC Addr: %x %x\n",MAC10G_0B,MAC10G_1B));
							// ============================
							//-----------------------------------------------------

							save_systemvars_to_flashparameters();

							D(1, BUG("...Setting Port-A MAC Addr: %x %x\n",MAC_0,MAC_1));
							configure_tse_mac(TRIPLE_SPEED_ETHERNET_0_BASE);
							configure_tse0_to_phya();	// run this after configuring the MAC

							D(1, BUG("...Setting Port-B MAC Addr: %x %x\n",MAC_0B,MAC_1B));
							configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
							configure_tse1_to_phyb();	// run this after configuring the MAC

							if(gOPT_INSTALLED_a10g) { // if installed write MAC
								D(1, BUG("...STILL NEED TO Confirm setting 10G Port-A MAC Addr to the 10G MAC!"));
								init_10G_MAC();
							}

						}

					}//End GIGE: Port-A
					/*******
					else{	// if GIGE: Port-B
						if(BytesToLong(RxBuffer, MAC_ADDR_MSB3_ptr) != BytesToLong(ConfigStatB, MAC_ADDR_MSB3_ptr) || BytesToInt(RxBuffer, MAC_ADDR_LSB1_ptr) != BytesToInt(ConfigStatB, MAC_ADDR_LSB1_ptr)){
							MAC_0B = BytesToLong(RxBuffer,MAC_ADDR_MSB3_ptr);
							MAC_1B = (unsigned int) BytesToInt(RxBuffer,MAC_ADDR_LSB1_ptr);
							SaveBytesLong(ConfigStatB,MAC_ADDR_MSB3_ptr,MAC_0B);
							SaveBytesInt(ConfigStatB,MAC_ADDR_LSB1_ptr,MAC_1B);

							save_systemvars_to_flashparameters();

							D(1, BUG("...Setting Port-B MAC Addr: %x %x\n",MAC_0B,MAC_1B));
							configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
							configure_tse1_to_phyb();	// run this after configuring the MAC
							}
						}//End GIGE: Port-B
						*********/
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
						D(1, BUG("Inside PHY_CONFIG1_ptr \t"));
						if(BytesToInt(RxBuffer, PHY_CONFIG1_ptr) != BytesToInt(ConfigStat, PHY_CONFIG1_ptr)) {
							D(1, BUG("PHY_CONFIG1/0_ptr has changed!\n"));
							RxBuffer[PHY_CONFIG0_ptr]	 &= (unsigned char) ~PHY_CONFIG_RESTART_ANEG_MASK;	// clear PHY_CONFIG0 kicker bit

							PHYA_CONFIG = BytesToInt(RxBuffer, PHY_CONFIG1_ptr);
							SaveBytesInt(ConfigStat,PHY_CONFIG1_ptr,PHYA_CONFIG);
							PHYA_ETH_SPEED = PHYA_CONFIG & PHY_CONFIG_ETH_SPEED_MASK;

							configure_TxClkA_speed();	// set Clock for new Ethernet Speed selection

							//On speed change: reset num_packets & load% calculations...........
				//			SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		//Call on: Frame Size, TestSecs, FPS change or SPEED change
				//			SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);							// called on Frame Size, FPS change or SPEED

							//==== Mode: Copper ==================================================
							if(RxBuffer[GIGEMODE_ptr] <= ModeCopper1G){	// Mode: Copper
								//if(ConfigStat[GIGEMODE_ptr] == ModeFiber100M)	// if we were in 100FX then exit...
									exit_100FX_operation(TRIPLE_SPEED_ETHERNET_0_BASE);

								PHYA_COPPER_FULLDUPLEX = NO;
								if(PHYA_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
									PHYA_COPPER_FULLDUPLEX = YES;

								PHYA_COPPER_ENABLE_AN = NO;
								if(PHYA_CONFIG & PHY_CONFIG_ENABLE_AN_MASK)
									PHYA_COPPER_ENABLE_AN = YES;

								configure_phya_copper();
								select_copper_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
								restart_copper_auto_negotiation(TRIPLE_SPEED_ETHERNET_0_BASE);
							}

							//==== Mode: Fiber ============================================
							else if(RxBuffer[GIGEMODE_ptr] == ModeFiber1G) {
								//if(ConfigStat[GIGEMODE_ptr] == ModeFiber100M)	// if we were in 100FX then exit...
									exit_100FX_operation(TRIPLE_SPEED_ETHERNET_0_BASE);

								D(1, BUG("CONFIGURING Port-A for Fiber-1G\n"));
								if(PHYA_ETH_SPEED == speed_1G){	// if 1G configure copper to the 1G as well!
									PHYA_COPPER_FULLDUPLEX = NO;
									if(PHYA_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
										PHYA_COPPER_FULLDUPLEX = YES;

									PHYA_COPPER_ENABLE_AN = NO;
									if(PHYA_CONFIG & PHY_CONFIG_ENABLE_AN_MASK)
										PHYA_COPPER_ENABLE_AN = YES;

									configure_phya_copper();
									restart_copper_auto_negotiation(TRIPLE_SPEED_ETHERNET_0_BASE);
									}

								PHYA_FIBER_FULLDUPLEX = NO;
								if(PHYA_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
									PHYA_FIBER_FULLDUPLEX = YES;

								PHYA_FIBER_ENABLE_AN = NO;
								if(PHYA_CONFIG & PHY_CONFIG_ENABLE_AN_MASK)
									PHYA_FIBER_ENABLE_AN = YES;

								configure_phya_fiber();
								select_fiber_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
								restart_fiber_auto_negotiation(TRIPLE_SPEED_ETHERNET_0_BASE);
							}
							else if(RxBuffer[GIGEMODE_ptr] == ModeFiber100M) {
								D(1, BUG("Port-A: Selected for 100FX\n"));
								PHYA_FIBER_FULLDUPLEX = NO;
								if(PHYA_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
									PHYA_FIBER_FULLDUPLEX = YES;

								PHYA_FIBER_ENABLE_AN = NO;	//Auto-Neg at 100FX is NOT Supported (by Broadcom BCM5482)

								configure_phya_fiber();
								select_fiber_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
								configure_100FX_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
								//PHYA_ETH_SPEED = speed_100M; ...set now by GUI
								//SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		//Call on: Frame Size, TestSecs, FPS change or SPEED change
								//SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);							// called on Frame Size, FPS change or SPEED
								//restart_fiber_auto_negotiation(TRIPLE_SPEED_ETHERNET_0_BASE);	// this spanks 100FX
							}
						}//END changed config
					}//END GIGE_A
					else {// Port-B
						if(BytesToInt(RxBuffer, PHY_CONFIG1_ptr) != BytesToInt(ConfigStatB, PHY_CONFIG1_ptr)) {
							RxBuffer[PHY_CONFIG0_ptr]	 &= (unsigned char) ~PHY_CONFIG_RESTART_ANEG_MASK;	// clear PHY_CONFIG0 kicker bit

							PHYB_CONFIG = BytesToInt(RxBuffer, PHY_CONFIG1_ptr);
							//SaveBytesInt(ConfigStat,PHY_CONFIG1_ptr,PHYB_CONFIG);
							PHYB_ETH_SPEED = PHYB_CONFIG & PHY_CONFIG_ETH_SPEED_MASK;

							configure_TxClkB_speed();	// set Clock for new Ethernet Speed selection
							//old if((RxBuffer[GIGEMODE_ptr]&0x01) == mode_copper){	// Mode: Copper
							if(RxBuffer[GIGEMODE_ptr] <= ModeCopper1G){	// Mode: Copper
								//if(ConfigStat[GIGEMODE_ptr] == ModeFiber100M)	// if we were in 100FX then exit...
									exit_100FX_operation(TRIPLE_SPEED_ETHERNET_1_BASE);

								PHYB_COPPER_FULLDUPLEX = NO;
								if(PHYB_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
									PHYB_COPPER_FULLDUPLEX = YES;

								PHYB_COPPER_ENABLE_AN = NO;
								if(PHYB_CONFIG & PHY_CONFIG_ENABLE_AN_MASK)
									PHYB_COPPER_ENABLE_AN = YES;

								configure_phyb_copper();
								select_copper_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
								restart_copper_auto_negotiation(TRIPLE_SPEED_ETHERNET_1_BASE);
								}

							//==== Mode: Fiber ============================================
							else if(RxBuffer[GIGEMODE_ptr] == ModeFiber1G) {
								//if(ConfigStat[GIGEMODE_ptr] == ModeFiber100M)	// if we were in 100FX then exit...
									exit_100FX_operation(TRIPLE_SPEED_ETHERNET_1_BASE);

								if(PHYA_ETH_SPEED == speed_1G) {	// if 1G configure copper to the 1G as well!
									PHYB_COPPER_FULLDUPLEX = NO;
									if(PHYB_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
										PHYB_COPPER_FULLDUPLEX = YES;

									PHYB_COPPER_ENABLE_AN = NO;
									if(PHYB_CONFIG & PHY_CONFIG_ENABLE_AN_MASK)
										PHYB_COPPER_ENABLE_AN = YES;

									configure_phyb_copper();
									restart_copper_auto_negotiation(TRIPLE_SPEED_ETHERNET_1_BASE);
								}
								PHYB_FIBER_FULLDUPLEX = NO;
								if(PHYB_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
									PHYB_FIBER_FULLDUPLEX = YES;

								PHYB_FIBER_ENABLE_AN = NO;
								if(PHYB_CONFIG & PHY_CONFIG_ENABLE_AN_MASK)
									PHYA_FIBER_ENABLE_AN = YES;

								configure_phyb_fiber();
								select_fiber_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
								restart_fiber_auto_negotiation(TRIPLE_SPEED_ETHERNET_1_BASE);
							}
							else if(RxBuffer[GIGEMODE_ptr] == ModeFiber100M) {
								D(1, BUG("CONFIGURING Port-B for 100FX\n"));
								PHYB_FIBER_FULLDUPLEX = NO;
								if(PHYB_CONFIG & PHY_CONFIG_FULLDUPLEX_MASK)
									PHYB_FIBER_FULLDUPLEX = YES;

								PHYB_FIBER_ENABLE_AN = NO;	//Auto-Neg at 100FX is NOT Supported (by Broadcom BCM5482)

								configure_phyb_fiber();
								select_fiber_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
								configure_100FX_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
							}
						}//END changed config
					}//END GIGE_B

						// set all other tcl #def's as SPEED.....
						// if GUI imposes a PHY change execute it here.....
						//.........................
					break;

				case PHY_SFP_MISC_ptr:	//62	// ^0:1=SFP_A Transmit_OFF, ^1:1=SFP_B Transmit_OFF, ^2:1=SFP+_A Transmit_OFF
					// can access SFP A, B and 10G from any Link-state; A, B or 10G...so maintain states across Links (Machines)
					if(((RxBuffer[i])&0x01) != ((ConfigStat[i])&0x01) || ((RxBuffer[i])&0x01) != ((ConfigStatB[i])&0x01) || ((RxBuffer[i])&0x01) != ((ConfigStat10G[i])&0x01)){ // if SFP_A Transmit_OFF has changed
						if(((RxBuffer[i])&0x01) == 0x01)	sfp_transmit_laser(PortA, OFF);
						else								sfp_transmit_laser(PortA, ON);
						ConfigStat[i] = RxBuffer[i];   // cc to A sfp handler
						ConfigStatB[i] = RxBuffer[i];   // cc to B sfp handler
						ConfigStat10G[i] = RxBuffer[i]; // cc to 10G sfp handler
						}

					if(((RxBuffer[i])&0x02) != ((ConfigStat[i])&0x02) || ((RxBuffer[i])&0x01) != ((ConfigStatB[i])&0x01) || ((RxBuffer[i])&0x01) != ((ConfigStat10G[i])&0x01)){ // if SFP_B Transmit_OFF has changed
						if(((RxBuffer[i])&0x02) == 0x02)	sfp_transmit_laser(PortB, OFF);
						else								sfp_transmit_laser(PortB, ON);
						ConfigStat[i] = RxBuffer[i];   // cc to A sfp handler
						ConfigStatB[i] = RxBuffer[i];   // cc to B sfp handler
						ConfigStat10G[i] = RxBuffer[i]; // cc to 10G sfp handler
						}

					if(((RxBuffer[i])&0x04) != ((ConfigStat[i])&0x04) || ((RxBuffer[i])&0x01) != ((ConfigStatB[i])&0x01) || ((RxBuffer[i])&0x01) != ((ConfigStat10G[i])&0x01)){ // if SFP+A Transmit_OFF has changed
						if(((RxBuffer[i])&0x04) == 0x04)	sfp10G_transmit_laser(PortA, OFF);
						else								sfp10G_transmit_laser(PortA, ON);
						ConfigStat[i] = RxBuffer[i];   // cc to A sfp handler
						ConfigStatB[i] = RxBuffer[i];   // cc to B sfp handler
						ConfigStat10G[i] = RxBuffer[i]; // cc to 10G sfp handler
						}
					break;

				//----------------------------------------------
				case CORE_SIGROUTE_MODES_ptr:	//69 ^0:0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK), 1=TSE_LANWAN_MODE, ^1:0=10G_NORMAL_MODE,1=10G_LANWAN_MODE (10G+B=LANWAN,A=GEN/MON)
					// RCVD in 10G & TSE MODE ---> accessed in both
					/*** // RCVD in 10G MODE only! ---> accessed in cfg10_hw.c
					else if(RxBuffer[i] == 0x02){	//0=NORMAL_MODE, 2=10G_LANWAN_MODE
						SET_10G_LANWAN_MODE;	// 10G & B in LAN-WAN mode, sets TSE-A to normal

						ConfigStat[CORE_SIGROUTE_MODES_ptr]    = RxBuffer[i];	// Save New operating MODE to all
						ConfigStatB[CORE_SIGROUTE_MODES_ptr]   = RxBuffer[i];
						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
						}
					***/
					if(!GIGE_A && !GIGE10){	// Currently TSE B-port ONLY
						//if 0=Looped/0x10=End-to-End test has changed.....
						D(1, BUG("[TSE-B] CORE_SIGROUTE_MODES_ptrresult RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
						D(1, BUG("[TSE-B] CORE_SIGROUTE_MODES_ptr result ConfigStat[MISC_ptr]:  %x\n",ConfigStatB[CORE_SIGROUTE_MODES_ptr]));

						if(((RxBuffer[i])&0x10) != ((ConfigStatB[i])&0x10)){ // ^4: 0=looped test, 1=end-to-end
							if( (RxBuffer[i] &0x10) == 0x10){
								EMON_TIMEOUT = 100;		// 10seconds (100 *100mS) for end-to-end tests
								END_TO_END_TEST = YES;
								D(1, BUG("\nEnd-to-End test\n"));
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x10;
								}
							else{
								EMON_TIMEOUT = 50;		// 5seconds (50 *100mS) for far-end-Looped tests
								END_TO_END_TEST = NO;
								D(1, BUG("\nLoopback\n"));
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x10;
								}
							}

						if(((RxBuffer[i])&0x20) != ((ConfigStatB[i])&0x20)) { // ^5:Loopback mode: 0=normal, 1=loopback enabled
							//Enable/Disable Loopback
							if((RxBuffer[i] & 0x20) == 0x20) {
								D(1, BUG("\nEnable LPBK\n"));
								//SET_TSE_B_TX_TO_NW_LOOPBACK;	// Enable LPBK
								SET_TSE_B_LOOPBACK;
								D(1, BUG("B:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));
								D(1, BUG("B:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x20;

								SaveBytesLong(ConfigStatB,SECONDS3_ptr,0);	// Clear ET when going into lpbk
							}
							else {
								D(1, BUG("\nDisable LPBK\n"));
								SET_TSE_B_TX_3MUX_TO_NO_OUTPUT; // 'disable' LPBK
								//splitter stays at 1
								D(1, BUG("B:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));
								D(1, BUG("B:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x20;
							}
						}

						if(((RxBuffer[i])&0x40) != ((ConfigStatB[i])&0x40)) { // ^6: Swap MAC: 0= no Swap, 1=Swap MAC
							if((RxBuffer[i] & 0x40) == 0x40){
								D(1, BUG("\n enable Swap mac\n"));
								SET_SWAP_B_TOGGLE |= 0x01;
								SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
								 D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
								 ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x40;
							}
							else {
								D(1, BUG("\n disable swap mac \n"));
								SET_SWAP_B_TOGGLE &= ~0x01;
								SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
								D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x40;
							}
						}


						//swap IP
						if((RxBuffer[i]& 0x80) != (ConfigStatB[i]&0x80)) {	// ^7: Swap IP: 0= no Swap, 1=Swap IP addresses
							if((RxBuffer[i] & 0x80) == 0x80) {
								D(1, BUG("\n swap IP\n"));
								SET_SWAP_B_TOGGLE |= 0x02;
								 D(1, BUG("B:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_B_TOGGLE));
								 SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
								 D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
								 ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x80;
							}
							else {
								SET_SWAP_B_TOGGLE &= ~0x02;
								D(1, BUG("B:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_B_TOGGLE));
								SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
								D(1, BUG("\n disable swap IP\n"));
								D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
							}
						}

					}//End Port B..........................................

					//***************************************************************
					else { //PORT A LOOPBACK/SWAP Test-mode OPERATIONS
						//if 0=Looped/0x10=End-to-End test has changed.....
						D(1, BUG("[TSE-A] CORE_SIGROUTE_MODES_ptrresult RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
						D(1, BUG("[TSE-A] CORE_SIGROUTE_MODES_ptr result ConfigStat[MISC_ptr]:  %x\n",ConfigStat[CORE_SIGROUTE_MODES_ptr]));

						if(((RxBuffer[i])&0x01) != ((ConfigStat[i])&0x01)) {	//^0:TSE LAN/WAN 0=NORMAL_MODE, 1=LAN/WAN
							if((RxBuffer[i] & 0x01) == 0x00) {	//MAN/WAN mode: 0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK)
								D(1, BUG("TSE_NORMAL_MODE (A=GEN/MON,B=LPBK)"));
								D(1, BUG("//////////////////////////// (TSE NORMAL)"));

								SET_TSE_A_NORMAL_TEST_MODE;
								D(1, BUG("SET_TSE_A_NORMAL_TEST_MODE\n"));
								D(1, BUG("MUX TSE_A_TX_3MUX TSE_NORMAL_MODE: 0x%04X\n", IORD_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG)));
								D(1, BUG("SET_TSE_A_RX_2SPLITTER_TO_EMON TSE_NORMAL_MODE: 0x%04X\n", IORD_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG)));


								SET_TSE_B_LOOPBACK;
								D(1, BUG("SET_TSE_B_LOOPBACK\n"));
								D(1, BUG("MUX TSE_B_TX_3MUX SET_TSE_B_LOOPBACK: 0x%04X\n", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));
								D(1, BUG("TSE_B_RX_3SPLITTER SET_TSE_B_LOOPBACK: 0x%04X\n",IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));

								SET_SWAP_B_TOGGLE|=0x01;
								SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
								D(1, BUG("SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE)\n"));
								D(1, BUG("B:ADDR_SWAPPER STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));

								SET_10G_NORMAL_TEST_MODE;
								D(1, BUG("SET_10G_NORMAL_TEST_MODE\n"));
								D(1, BUG("MUX A10G_TX_3MUX SET_10G_NORMAL_TEST_MODE: 0x%04X\n", IORD_16DIRECT(A10G_TX_3MUX, ST_MUX_REG)));
								D(1, BUG("SET_10G_RX_3SPLITTER_TO_TMON SET_10G_NORMAL_TEST_MODE: 0x%04X\n", IORD_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG)));
								//we know GUI is taking B out of Promiscuous mode from A-link, we won't see B since we're in A so...
								MACB_COMMAND_CONFIG &= ~0x10; // force promiscuous mode off for B!
								SaveBytesLong(ConfigStatB,MAC_CONFIG3_ptr,MACB_COMMAND_CONFIG);
								configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
								configure_tse1_to_phyb();	// run this after configuring the MAC

								//we know GUI is taking 10G out of Promiscuous mode from A-link, we won't see 10G since we're in A so...
								MAC10A_COMMAND_CONFIG &= ~0x00000001;	// force promiscuous mode off for 10G!
								SaveBytesLong(ConfigStat10G,MAC_CONFIG3_ptr,MAC10A_COMMAND_CONFIG);
								init_10G_MAC();

								//todo ask why the mode should be saved in all - that is confusing the UI
								ConfigStat[CORE_SIGROUTE_MODES_ptr]    = RxBuffer[i];	// Save New operating MODE to all
							}
							else {//if((RxBuffer[i] & 0x01) == 0x01){	//LAN/WAN: 1=TSE_LANWAN_MODE (10G NORMAL)
								D(1, BUG("1=TSE_LANWAN_MODE"));
								D(1, BUG("//////////////////////////// (TSE LAN WAN)"));
								SET_TSE_LANWAN_MODE;	// A&B in LAN-WAN mode, sets 10G to normal and SWAP OFF!
								D(1, BUG("\nSET_10G_NORMAL_TEST_MODE\n"));
								D(1, BUG("MUX A10G_TX_3MUX SET_10G_NORMAL_TEST_MODE: 0x%04X\n", IORD_16DIRECT(A10G_TX_3MUX, ST_MUX_REG)));
								D(1, BUG("SET_10G_RX_3SPLITTER_TO_TMON SET_10G_NORMAL_TEST_MODE: 0x%04X\n", IORD_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG)));


								D(1, BUG("\nSET_TSE_A_TX_3MUX_TO_TSE_LANWAN\n"));
								D(1, BUG("TSE_A_TX_3MUX: 0x%04X\n",IORD_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG)));

								D(1, BUG("\nSET_TSE_B_RX_TO_10G_LANWAN\n"));
								D(1, BUG("TSE_A_RX_2SPLITTER: 0x%04X\n",IORD_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG)));

								D(1, BUG("\nSET_TSE_B_TX_TO_TSE_LANWAN\n"));
								D(1, BUG("TSE_B_TX_3MUX: 0x%04X\n",IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));

								D(1, BUG("\nSET_TSE_B_RX_TO_TSE_LANWAN\n"));
								D(1, BUG("TSE_B_RX_3SPLITTER: 0x%04X\n",IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));

								SET_SWAP_B_TOGGLE = 0;
								SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
								ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0xE0;

								D(1, BUG("\nSET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE)\n"));
								D(1, BUG("B:ADDR_SWAPPER STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));

								//we know GUI is putting B in Promiscuous mode from A-link, we won't see B since we're in A so...
								MACB_COMMAND_CONFIG |= 0x10; // force promiscuous mode on for B!
								SaveBytesLong(ConfigStatB,MAC_CONFIG3_ptr,MACB_COMMAND_CONFIG);
								configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);
								configure_tse1_to_phyb();	// run this after configuring the MAC

								//we know GUI is putting 10G in Promiscuous mode from A-link, we won't see 10G since we're in A so...
								MAC10A_COMMAND_CONFIG |= 0x00000001;	// force promiscuous mode on for 10G!
								SaveBytesLong(ConfigStat10G,MAC_CONFIG3_ptr,MAC10A_COMMAND_CONFIG);
								init_10G_MAC();

								ConfigStat[CORE_SIGROUTE_MODES_ptr]    = RxBuffer[i];	// Save New operating MODE to all
							}
						}

						//if 0=Looped/0x10=End-to-End test has changed.....
						if(((RxBuffer[i])&0x10) != ((ConfigStat[i])&0x10)) { // ^4: 0=looped test, 1=end-to-end
							if( (RxBuffer[i] &0x10) == 0x10){
								EMON_TIMEOUT = 100;		// 10seconds (100 *100mS) for end-to-end tests
								END_TO_END_TEST = YES;
								D(1, BUG("\nEnd-to-End test\n"));
								ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x10;	// maintain bit states across A, B (10G has it's own flag)
							}
							else {
								EMON_TIMEOUT = 50;		// 5seconds (50 *100mS) for far-end-Looped tests
								END_TO_END_TEST = NO;
								D(1, BUG("\nLoopback\n"));
								ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x10;	// maintain bit states across A, B (10G has it's own flag)
							}
						}

						if((RxBuffer[i]& 0x20) != (ConfigStat[i] & 0x20)) {	//^5=TSE_A_LOOPBACK, 0=Loopback Off, 1=Loopback ON
							if((RxBuffer[i] & 0x20) == 0x20) {
								D(1, BUG("\nEnable LPBK\n"));
								SET_TSE_A_LOOPBACK;
								D(1, BUG("A:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_1GE_A_BASE, ST_MUX_REG)));
								D(1, BUG("A:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_SPLITTER_REG)));
								ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x20;

								SaveBytesLong(ConfigStat,SECONDS3_ptr,0);	// Clear ET when going into lpbk
							}
							else {
								D(1, BUG("\nDisable LPBK\n"));
								SET_TSE_A_NORMAL_TEST_MODE;//disable LPBK - MUX TO EGEN
								/*SET_TSE_A_TX_3MUX_TO_NO_OUTPUT; // 'disable' LPBK
								SET_TSE_A_RX_2SPLITTER_TO_EMON;*/
								D(1, BUG("A:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_1GE_A_BASE, ST_MUX_REG)));
								D(1, BUG("A:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_SPLITTER_REG)));
								ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x20;
							}
						}

						// Port-A swap MAC
						if((RxBuffer[i]& 0x40) != (ConfigStat[i]&0x40)) {	//^6:0=No MAC SWAP, 1=MAC SWAP ON
							if((RxBuffer[i] & 0x40) == 0x40){
								SET_SWAP_A_TOGGLE |= 0x01;
								D(1, BUG("\nEnable swap mac\n"));
								D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
								SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
								D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
								ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x40;
								}
							else{
								D(1, BUG("\nDisable swap mac\n"));
								SET_SWAP_A_TOGGLE &= ~0x01;
								 D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
								SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
								 D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
								 ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x40;
								}
							}

						//swap IP
						if((RxBuffer[i]& 0x80) != (ConfigStat[i]&0x80)) {	// Port-A TSE_A_SWAP_ADDR
							D(1, BUG("CORE_SIGROUTE_MODES_ptrresult RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
							D(1, BUG("CORE_SIGROUTE_MODES_ptr result ConfigStat[MISC_ptr]:  %x\n",ConfigStat[CORE_SIGROUTE_MODES_ptr]));
							if((RxBuffer[i] & 0x80) == 0x80) {
								D(1, BUG("\n swap IP\n"));
								SET_SWAP_A_TOGGLE |= 0x02;
								D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
								SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
								D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
								ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x80;
							}
							else {
								SET_SWAP_A_TOGGLE &= ~0x02;
								 D(1, BUG("\n disable swap IP\n"));
								 D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
								 SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
								 D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
								 ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
							}
						}

						//remote loop up
						if((RxBuffer[i]& 0x04) != (ConfigStat[i]&0x04)) {
							D(1, BUG("CORE_SIGROUTE_MODES_ptr result RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
							D(1, BUG("CORE_SIGROUTE_MODES_ptr result ConfigStat[MISC_ptr]:  %x\n",ConfigStat[CORE_SIGROUTE_MODES_ptr]));
							//ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
//							if((RxBuffer[i] & 0x04) == 0x04){
								D(1, BUG("remote loopup"));

								// Update MAC Address
								destination_tse_addr0 = BytesToLong(RxBuffer,TRAFFIC_HDR1_1_ptr);	// use GUI selected dest MAC addr
								destination_tse_addr1 = (unsigned int) BytesToInt(RxBuffer,TRAFFIC_HDR2_1_ptr);
								SETGEN_TSEMAC_DEST_ADDR(destination_tse_addr1, destination_tse_addr0);

								SEND_LOOPBACK_PACKET();
								ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
//							}
//							else{
//								D(1, BUG("disable remote loopup"));
//							}
							D(1, BUG("RxBuffer[i]: %x\n", RxBuffer[i]));
							D(1, BUG("ConfigStat[sigroutes_ptr]:  %x\n",ConfigStat[CORE_SIGROUTE_MODES_ptr]));
							//ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
						}

						//remote loop down
						if((RxBuffer[i]& 0x08) != (ConfigStat[i]&0x08)){
							D(1, BUG("CORE_SIGROUTE_MODES_ptrresult RxBuffer[MISC_ptr]:  %x\n",RxBuffer[CORE_SIGROUTE_MODES_ptr]));
							D(1, BUG("CORE_SIGROUTE_MODES_ptr result ConfigStat[MISC_ptr]:  %x\n",ConfigStat[CORE_SIGROUTE_MODES_ptr]));
							//ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
//							if((RxBuffer[i] & 0x08) == 0x08){
							destination_tse_addr0 = BytesToLong(RxBuffer,TRAFFIC_HDR1_1_ptr);	// use GUI selected dest MAC addr
							destination_tse_addr1 = (unsigned int) BytesToInt(RxBuffer,TRAFFIC_HDR2_1_ptr);
							SETGEN_TSEMAC_DEST_ADDR(destination_tse_addr1, destination_tse_addr0);
							D(1, BUG("DEST_TSE_ADDR0: %X\n", destination_tse_addr0));
							D(1, BUG("DEST_TSE_ADDR1: %X\n", destination_tse_addr1));

								D(1, BUG("remote loopdown"));
								SEND_LOOPBACK_PACKET();
//							}
//							else{
//								D(1, BUG("disable remote loopdown"));
//							}
							ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
							D(1, BUG("RxBuffer[i]: %x\n", RxBuffer[i]));
							D(1, BUG("ConfigStat[sigroutes_ptr]:  %x\n",ConfigStat[CORE_SIGROUTE_MODES_ptr]));
							//ConfigStat[CORE_SIGROUTE_MODES_ptr] = RxBuffer[i];
						}
					}//End Port A..........................................
					break;

				case CLEAR_ptr:	//70
					if (GIGE_A) {
						if(((RxBuffer[i])&0x01) != ((ConfigStat[i])&0x01)) // if Clear Bit has been TOGGLED
							SaveBytesLong(ConfigStat,SECONDS3_ptr,0);

						if (((RxBuffer[i])&0x04) != ((ConfigStat[i])&0x04)) {
							D(1, BUG("clear the stats \n"));
							tseMacWriteCommandConfigAndReset(TRIPLE_SPEED_ETHERNET_0_BASE);
							SaveBytesLong(ConfigStat,SECONDS3_ptr,0); // restart port-A timer
						}
					}
					else {
						if(((RxBuffer[i])&0x04) != ((ConfigStatB[i])&0x04)) {  // if Reset MAC-B stat's has been TOGGLED
							tseMacWriteCommandConfigAndReset(TRIPLE_SPEED_ETHERNET_1_BASE);
							SaveBytesLong(ConfigStatB,SECONDS3_ptr,0); // restart port-B timer
						}
					}

					break;

				case MISC_ptr:	//71	DumpSFPAinfo^0 , DumpSFPBinfo^1
					// might need to check/monitor ConfigStatB to ensure Port setting is don't care.....

					if(((RxBuffer[i])&0x01) != ((ConfigStat[i])&0x01)) {		// if DumpSFPAinfo
						READ_SFPA_DATA = YES;
						if(((RxBuffer[i])&0x01) == 0) {	// maintain bit states across A, B and 10G
							ConfigStat[MISC_ptr] &= ~0x01;
							ConfigStatB[MISC_ptr] &= ~0x01;
							ConfigStat10G[MISC_ptr] &= ~0x01;
						}
						else {
							ConfigStat[MISC_ptr] |= 0x01;
							ConfigStatB[MISC_ptr] |= 0x01;
							ConfigStat10G[MISC_ptr] |= 0x01;
						}
					}
					if(((RxBuffer[i])&0x02) != ((ConfigStat[i])&0x02)) {		// if DumpSFPBinfo
						READ_SFPB_DATA = YES;
						if(((RxBuffer[i])&0x02) == 0) {	// maintain bit states across A, B and 10G
							ConfigStat[MISC_ptr] &= ~0x02;
							ConfigStatB[MISC_ptr] &= ~0x02;
							ConfigStat10G[MISC_ptr] &= ~0x02;
						}
						else {
							ConfigStat[MISC_ptr] |= 0x02;
							ConfigStatB[MISC_ptr] |= 0x02;
							ConfigStat10G[MISC_ptr] |= 0x02;
						}
					}
					if(((RxBuffer[i])&0x04) != ((ConfigStat[i])&0x04)) {		// if DumpSFP+_A info
						READ_SFP10A_DATA = YES;
						if(((RxBuffer[i])&0x04) == 0) {	// maintain bit states across A, B and 10G
							ConfigStat[MISC_ptr] &= ~0x04;
							ConfigStatB[MISC_ptr] &= ~0x04;
							ConfigStat10G[MISC_ptr] &= ~0x04;
						}
						else {
							ConfigStat[MISC_ptr] |= 0x04;
							ConfigStatB[MISC_ptr] |= 0x04;
							ConfigStat10G[MISC_ptr] |= 0x04;
						}
					}
	//todo uncomment the code below - commented out for testing purpose
			/*		if(((RxBuffer[i])&0x40) != ((ConfigStatB[i])&0x40)){	// ^6 to send back the big packet
						D(1, BUG("consolidated result RxBuffer[MISC_ptr %x\n",RxBuffer[MISC_ptr]));
						D(1, BUG("consolidated result ConfigStat[MISC_ptr %x\n",ConfigStat[MISC_ptr]));
						D(1, BUG("Big packet SEND RxBuffer[MISC_ptr %x\n",RxBuffer[MISC_ptr]));
						if(MS_PKT_RCVD){SEND_MS_PACKET=YES;}
						if(((RxBuffer[i])&0x40) == 0){	// maintain bit states across A, B and 10G
						ConfigStat[MISC_ptr] &= ~0x40;
						ConfigStatB[MISC_ptr] &= ~0x40;
						ConfigStat10G[MISC_ptr] &= ~0x40;
						}
						else{
						ConfigStat[MISC_ptr] |= 0x40;
						ConfigStatB[MISC_ptr] |= 0x40;
						ConfigStat10G[MISC_ptr] |= 0x40;
					}
					}*/

					if(((RxBuffer[i])&0x80) != ((ConfigStat[i])&0x80)) {//MISC_ptr ^7

						if((RxBuffer[i]&0x80)== 0x80) { // sending back consolidated result for MS.
							D(1, BUG("consolidated result RxBuffer[MISC_ptr %x\n",RxBuffer[MISC_ptr]));
							D(1, BUG("consolidated result ConfigStat[MISC_ptr %x\n",ConfigStat[MISC_ptr]));
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
//							DBG_MON_STATS();
						}
						ConfigStat[MISC_ptr] = RxBuffer[MISC_ptr];
					}
					break;
					case LOOPBACK_ptr: //74
						D(1, BUG("\nRXBUFFER: %X\n", RxBuffer[LOOPBACK_ptr]));
						D(1, BUG("\nLOOPBACK_PTR: %X\n", ConfigStat[LOOPBACK_ptr]));
						D(1, BUG("LOOPBACK_PTR b: %X\n", ConfigStatB[LOOPBACK_ptr]));

						if(!GIGE_A && !GIGE10) { // Currently TSE B-port ONLY
							// Clear Loopback
							if (((RxBuffer[i]) & 0x01) != ((ConfigStatB[i]) & 0x01)) {
								  D(1, BUG("\nTSE-B: CLEAR LOOPBACK_PTR: %X\n", ConfigStatB[LOOPBACK_ptr]));
								  D(1, BUG("TSE-B: CLEAR LOOPBACK_PTR: %X\n", RxBuffer[LOOPBACK_ptr]));
								  ConfigStatB[LOOPBACKSTAT_ptr] = 0;
							}

							// When detect remote loopback is enabled, mask sniffer
							if ((RxBuffer[i] & 0x02) != (ConfigStatB[i] & 0x02)){
								  if ((RxBuffer[LOOPBACK_ptr] & 0x02)==0x02) {
										DETECT_REMOTE_LOOPBACK_TSEB = 1;
										D(1, BUG("\n detect LOOPBACK_PTR: %X\n", ConfigStatB[LOOPBACK_ptr]));
										D(1, BUG("TSE-B: DETECT LOOPBACK: %d\n", DETECT_REMOTE_LOOPBACK_TSEB));
								  }
								  else {
										DETECT_REMOTE_LOOPBACK_TSEB = 0;
										D(1, BUG("\n detect LOOPBACK_PTR: %X\n", ConfigStatB[LOOPBACK_ptr]));
										D(1, BUG("TSE-B: DETECT LOOPBACK: %d\n", DETECT_REMOTE_LOOPBACK_TSEB));
								  }
							}

							if ((RxBuffer[i] & 0x04) != (ConfigStatB[i] & 0x04)) {
								  SEND_HELLO_PACKET();
							}
						}
						else{
							// Clear Loopback
							if (((RxBuffer[i]) & 0x01) != ((ConfigStat[i]) & 0x01)) {
								  D(1, BUG("\nCLEAR LOOPBACK_PTR: %X\n", ConfigStat[LOOPBACK_ptr]));
								  D(1, BUG("TSE-A: CLEAR LOOPBACK_PTR: %X\n", RxBuffer[LOOPBACK_ptr]));
								  ConfigStat[LOOPBACKSTAT_ptr] = 0;
							}

							// When detect remote loopback is enabled, mask sniffer
							if ((RxBuffer[i] & 0x02) != (ConfigStat[i] & 0x02)){
								  if ((RxBuffer[LOOPBACK_ptr] & 0x02)==0x02) {
										DETECT_REMOTE_LOOPBACK_TSEA = 1;
										D(1, BUG("\n detect LOOPBACK_PTR: %X\n", ConfigStat[LOOPBACK_ptr]));
										D(1, BUG("TSE-A: DETECT LOOPBACK: %d\n", DETECT_REMOTE_LOOPBACK_TSEA));
								  }
								  else {
										DETECT_REMOTE_LOOPBACK_TSEA = 0;
										D(1, BUG("\n detect LOOPBACK_PTR: %X\n", ConfigStat[LOOPBACK_ptr]));
										D(1, BUG("TSE-A: DETECT LOOPBACK: %d\n", DETECT_REMOTE_LOOPBACK_TSEA));
								  }
							}

							if ((RxBuffer[i] & 0x04) != (ConfigStat[i] & 0x04)) {//Query
								  SEND_HELLO_PACKET();
							}
						}

						/*if ((RxBuffer[LOOPBACK_ptr] & 0xF0)!= (ConfigStat[i]& 0xF0)) {
							D(1, BUG("\nSEND_LOOPBACK_PACKET LOOPBACK_PTR: %X\n", ConfigStat[LOOPBACK_ptr]));
							SEND_LOOPBACK_PACKET();
						}*/

						break;


				//--------------------------------------------------------------------------------------
				//	TRAFFIC GENERATION POOP
				//--------------------------------------------------------------------------------------
				//	if(((RxBuffer[i])&0x02) == 0x02) // Set Fixed or Random Packet Length as User selected!
				//		length_sel = def_pkt_length_random;
				//	else
				//		length_sel = def_pkt_length_fixed;


				case EGEN_NUM_SECS3_ptr:
				case EGEN_NUM_SECS2_ptr:
				case EGEN_NUM_SECS1_ptr:
				case EGEN_NUM_SECS0_ptr: // duration limited to F FFFF
					if(GIGE_A){
					if(BytesToLong(RxBuffer,EGEN_NUM_SECS3_ptr) != BytesToLong(ConfigStat,EGEN_NUM_SECS3_ptr)){
				//	if(BytesToInt(RxBuffer,EGEN_NUM_SECS1_ptr) != BytesToInt(ConfigStat,EGEN_NUM_SECS1_ptr)){

						D(1, BUG("...RxBuffer[EGEN_NUM_SECS3_ptr] %x\n",RxBuffer[EGEN_NUM_SECS3_ptr]));
						D(1, BUG("...RxBuffer[EGEN_NUM_SECS2_ptr] %x\n",RxBuffer[EGEN_NUM_SECS2_ptr]));
						D(1, BUG("...RxBuffer[EGEN_NUM_SECS1_ptr] %x\n",RxBuffer[EGEN_NUM_SECS1_ptr]));
						D(1, BUG("...RxBuffer[EGEN_NUM_SECS0_ptr] %x\n",RxBuffer[EGEN_NUM_SECS0_ptr]));

					gen_number_seconds = BytesToLong(RxBuffer,EGEN_NUM_SECS3_ptr); // save selected secs as well
					D(1, BUG("!!!!!!*****...Setting seconds to *********!!!!!!!%u\n",gen_number_seconds));
					//reset num_packets calculations...........
					SaveBytesLong(ConfigStat,EGEN_NUM_SECS3_ptr,gen_number_seconds);
					SaveBytesLong(ConfigStatB,EGEN_NUM_SECS3_ptr,gen_number_seconds);
					SaveBytesLong(ConfigStat10G,EGEN_NUM_SECS3_ptr,gen_number_seconds);
					}
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
				//commented out
				D(1, BUG("...RxBuffer[TRAFFIC_NUM_OF_STREAMS] %x\n",RxBuffer[TRAFFIC_NUM_OF_STREAMS]));
				D(1, BUG("...ConfigStat[TRAFFIC_NUM_OF_STREAMS] %x\n",ConfigStat[TRAFFIC_NUM_OF_STREAMS]));

				if((RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0x0F) != (ConfigStat[TRAFFIC_NUM_OF_STREAMS]&0x0F)) {
					D(1, BUG("...RxBuffer[TRAFFIC_NUM_OF_STREAMS] %x\n",RxBuffer[TRAFFIC_NUM_OF_STREAMS]));
					num_of_streams=((unsigned char) RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0x0F) +1 ; // num_of_stream ==1 for stream 0
					D(1, BUG("...total number of streams %d\n",num_of_streams));
				}

				if((RxBuffer[TRAFFIC_NUM_OF_STREAMS]&0xF0) != (ConfigStat[TRAFFIC_NUM_OF_STREAMS]&0xF0) && ((RxBuffer[MISC_ptr]&0x80) == 0x00))
				{
					stream_no=(unsigned short) (RxBuffer[TRAFFIC_NUM_OF_STREAMS]>>4)&0xF; // get which stream exactly and send the results corresponding to that stream
					//D(1, BUG("...selecting the exact stream %x\n",stream_no));
					clearTotalResults();
					read_mon_status(stream_no);
					D(1, BUG("\n===== Result STATS for stream %d =======\n",stream_no));
					DBG_MON_STATS();
				}
				ConfigStat[TRAFFIC_NUM_OF_STREAMS] = RxBuffer[TRAFFIC_NUM_OF_STREAMS];
				break;

				case STARTSTOP_ptr:		//148 Test Start/Stop & status
					//*** THIS IS REALLY START/STOP a TEST *****
					//....so START a TEST whether on Port A or B
					if(GIGE_A){
						SET_TRAFFIC_TO_TSE;	// point GEN & MON to TSE port-A
						D(1, BUG(">> TRAFFIC ENGINE switched to TSE-A port!\n"));
						if(RxTestStartGuiBit == 0x10){				// if Test = START
							gCUSTOM_TEST_START = YES;
							ConfigStat[STARTSTOP_ptr] = RxTestStartToggle;
							ConfigStatB[STARTSTOP_ptr] = RxTestStartToggle;
							ConfigStat[STARTSTOP_ptr] |= 0x20|0x10;	// reflect MOD start and return GUI start
							ConfigStatB[STARTSTOP_ptr] |= 0x20|0x10;

							ConfigStat10G[STARTSTOP_ptr] = RxTestStartToggle;
							ConfigStat10GB[STARTSTOP_ptr] = RxTestStartToggle;
							ConfigStat10G[STARTSTOP_ptr] |= 0x20|0x10;	// reflect MOD start and return GUI start
							ConfigStat10GB[STARTSTOP_ptr] |= 0x20|0x10;
							D(1, BUG(">> Start TEST (TSE) cmd rcvd (A) Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));
						}
						else if(RxTestStartGuiBit == 0x00){ 	// if Test = STOP
//						else if (RxBuffer[STARTSTOP_ptr] == 0x00) {
							if(TestStartModBit){			// if test still running
								gCUSTOM_TEST_STOP = YES;
								D(1, BUG(">> Manual Stop TEST (TSE) cmd rcvd (A) Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));
								}
							else{										// else if test has stopped...
								ConfigStat[STARTSTOP_ptr] = 0;		// cleanUp test flag
								ConfigStatB[STARTSTOP_ptr] = 0;		// cleanUp test flag

								ConfigStat10G[STARTSTOP_ptr] = 0;	// cleanUp test flag
								ConfigStat10GB[STARTSTOP_ptr] = 0;	// cleanUp test flag
								D(1, BUG(">> TEST (TSE) done (0x00) GUI rcvd (A) Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));
								}
							}
						}
					break;

		default:
			break;

			}//End Switch
	}//End if NEW Config byte (command)
}//End for
//=== if Config Updated Copy Rxbuffer into Config portion of ConfigStat array

for(i=2;i<MaxConfig2-1;i++){    // load new config into ConfigStat
	if(i != STARTSTOP_ptr){		// STARTSTOP handled like LINK config byte
	//	D(1, BUG( "\n<>SENDING BACK THE STATUS\n"));
		if(GIGE_A)
			ConfigStat[i] = RxBuffer[i];
		else
			ConfigStatB[i] = RxBuffer[i];
	}
}



}


/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
