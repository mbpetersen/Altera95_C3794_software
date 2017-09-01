/*************************************************************************
*		TSE-MAC & PHY Configuration Functions 							 *
*				   														 *
* Copyright (c) 2013 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"

/****
8.1 Boot from MDIO:
•Reset the PHY
•Write 1.C300h = 0x0000 (put the internal micro into reset)
•Write 4.C05F = 0 NEW!! To initialize Tx LED Status
•Write 4.C05F = 0x3C00 NEW!! To initialize Tx LED Status
•Write 1.C302h = 0x0004
•Write 1.C319h = 0x0038
•Write 1.C31Ah = 0x0098
•Write 3.0026h = 0x0E00
•Write 3.0027h = 0x0893 (DOM_MODE enabled, 1” FR4 loss, hardware LED control)
•Write 3.0028h = 0xA528
•Write 3.0029h = 0x0003
•Write 1.C300h = 0x0002	// reset phy uP
•Write 3.E854 = 0x00C0	// suspend uP
•Download contents of .hex file (firmware load). See Section 13. for detailed address mapping.
//-----------------
The first 16kB of memory is located in the address range 3.8000h - 3.BFFFh.
	...This 16kB memory has an offset of 0x0 - 0x3FFFh.
The next 8kB of memory is located at 4.8000h-4.9FFFh.
	....The 8kB memory space has an offset of 0x4000h-0x5FFFh.
(0x3FFF + 0x1FFF = 0x4000+0x2000=0x6000 = 24576 bytes  (actual length from hex file: 0x5FEE+1 = 24558+1=24559)

The supplied hex file must be written into this address space when using the MDIO programming option.
	...The file must be written to memory beginning at address 3.8000h until address 3.BFFFh is reached.
	...The rest of the file must be written starting at address 4.8000h.
//-----------------
•Write 3.E854h = 0x0040
•[check for firmware heartbeat]
•[optional Checksum check] (see Section 14.)
•while 3.D7FD == 0x10 or 0x0: PAUSE time, T
 *******/
//------------------------------------------------------------------


/**************************************************************************
 * If the module's I2C controller initializes into a funny state,
 *    or there is some unexpected power up sequencing on the 3.3V rail,
 *    the I2C bus could theoretically be corrupted.
 * Issuing the STOP command seq clears all slaves to a known Idle state, ready to receive commands.
 *    The STOPs should be issued before loading the firmware into the QT2025.
 **************************************************************************/
void issue_10G_phy_I2C_stop_seq()
{
	unsigned int i=0;
	//GPIO1 = SCL   GPIO1==Bit^3 0x08 (SCL) //(base = 0x4480)
	//GPIO2 = SDA   GPIO2==Bit^4 0x10 (SDA)

	write_ext_10G_phy(0xC303, 1, 0x4498);     // SDA -> 1, SCL -> 1
	for (i = 0; i < 10; i++) {
		write_ext_10G_phy(0xC303, 1, 0x4490); // SDA -> 1, SCL -> 0
		write_ext_10G_phy(0xC303, 1, 0x4480); // SDA -> 0, SCL -> 0
		write_ext_10G_phy(0xC303, 1, 0x4488); // SDA -> 0, SCL -> 1
		write_ext_10G_phy(0xC303, 1, 0x4498); // SDA -> 1, SCL -> 1
		}
	write_ext_10G_phy(0xC303, 1, 0x4900); 	  // GPIOs off
}

void init_10G_external_phy(_Bool load_firmware)
{
	unsigned int heartbeat=0;
	unsigned char i=0;

//TEMP DEBUG
//	set_TX_REF_CLK_reset(HIGH);	// take Tx/Ref_clk OUT of RESET

	set_10G_PHY_reset(LOW);		//Reset the PHY (ActvL)
	OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS
	set_10G_PHY_reset(HIGH);	//Reset the PHY (ActvL)

	OSTimeDlyHMSM(0, 0, 0, 5);  //wait 5mS

	// write PHY boot sequence - for auto-configure mode
	write_ext_10G_phy(0xC300, 1, 0);		//1.C300h = 0x0000 (put the internal micro into reset)
	write_ext_10G_phy(0xC05F, 4, 0x3C00);	//4.C05Fh = 0x3C00  NEW!! To initialize Tx LED Status
	write_ext_10G_phy(0xC302, 1, 0x0004);	//1.C302h = 0x0004	// ErefClk=156.25, SrefCLK=155.52, uP=156.25MHz
	write_ext_10G_phy(0xC319, 1, 0x0038);	//1.C319h = 0x0038	// 10G,invTout, SFP+ Self configure mode
	write_ext_10G_phy(0xC31A, 1, 0x0098);	//1.C31Ah = 0x0098	// SFP++ module connected
	write_ext_10G_phy(0x0026, 3, 0x0E00);	//3.0026h = 0x0E00	// disable internal freeze points in FW
//	write_ext_10G_phy(0x0027, 3, (0x0893|0x0200)); // ^9=enable default mode
	write_ext_10G_phy(0xD006, 1, 0x000B);	//mbp set LED1 to Rx path
	write_ext_10G_phy(0x0027, 3, 0x0893);	//3.0027h = 0x0893	//(^7=DOM_MODE enabled, RxLoss^654=001: 1” FR4 loss, hardware LED control)
																// 11^Rx_PCS_ctrl=1 in RESET when input Invalid ^321=001  (TxLoss:0.5-1.5dB)
//	write_ext_10G_phy(0x0027, 3, (0x0893&~0x0800)); // do NOT put Rx_PCS in reset
	//	write_ext_10G_phy(0x0028, 3, (0xA528|0x0030)); // 0x70=default is unknown Module, 0x30=10G-LR
	write_ext_10G_phy(0x0028, 3, 0xA528);	//3.0028h = 0xA528
	write_ext_10G_phy(0x0029, 3, 0x0003);	//3.0029h = 0x0003	// ^3:0=0003 3meters Passive DAC short cable length threshold

	write_ext_10G_phy(0xC300, 1, 0x0002);	//1.C300h = 0x0002	// reset phy uP
	write_ext_10G_phy(0xE854, 3, 0x00C0);	//3.E854h = 0x00C0	// suspend uP

	issue_10G_phy_I2C_stop_seq();	// Ensure 10G_PHY I2C bus is not locked up
	if(load_firmware)
		read_flash_qtx_block_and_write_phy(); //===== load QT2025 FW (from Flash into PHY) =================


	write_ext_10G_phy(0xE854, 3, 0x0040);	//3.E854h = 0x0040 // kick'em in the ASS to WAKE-UP (restart)!

	D(1, BUG("\nChecking for heartbeat...."));
	heartbeat = read_ext_10G_phy(0xD7EE, 3);	// 3.7DEE // heartbeat
	D(1, BUG(" %02X/n",heartbeat));
	while(heartbeat == read_ext_10G_phy(0xD7EE, 3) && ++i < 200) // should change every ~768uS - give him 200mS
		OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!

	if(i<200){
		D(1, BUG("Heartbeat DETECTED!!  ...it took %dmS",i));
		D(1, BUG("...confirming heartbeat: %02lX",read_ext_10G_phy(0xD7EE, 3)));
		OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
		D(1, BUG(" %02lX",read_ext_10G_phy(0xD7EE, 3)));
		OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
		D(1, BUG(" %02lX",read_ext_10G_phy(0xD7EE, 3)));
		OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
		D(1, BUG(" %02lX\n",read_ext_10G_phy(0xD7EE, 3)));

		D(1, BUG("Checking & Waiting for QT2025 PHY uP to complete initialization...\n"));
		i=0;
		heartbeat = read_ext_10G_phy(0xD7FD, 3);	// check UC_status - when all good it's not 0x10 or 0!
		//while(++i < 2500 && (heartbeat < 0x50)){  // could take 2seconds! - give him 2.5 seconds!
		while(i++ < 25 && (heartbeat == 0x10 || heartbeat == 0)){  // could take 2seconds! - give him 2.5 seconds!
			OSTimeDlyHMSM(0, 0, 0, 100);  //wait 100mS for updated data!
			heartbeat = read_ext_10G_phy(0xD7FD, 3);
			}
		D(1, BUG("UC_status at %d = %02X...\n",i-1, heartbeat));
		if(i < 25){
			D(1, BUG("Init Complete!!  ...it took %3.2fsecs, (3.D7FD) Status=%04X\n",((float) i/10.0),heartbeat));
			//=== misc PHY configuration =================
			//D(1, BUG("\n\tGPIO STATE PreConfig Reg (1.C303)=0x%04lX \n",read_ext_10G_phy(0xC303, 1)));
			// in case no SFP+ Module is installed ensure FTX block is NOT powered down
			write_ext_10G_phy(0xF052, 1, 0x0022);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX PWDN!
			config_10G_external_phy();
			}
		else{
			D(1, BUG("TIMEOUT!!! ...PHY uP init incomplete (3.D7FD), Status=%04X\n",heartbeat));
			GIGE_STATUS = 0x03; // 10G PHY init incomplete - Timeout
			}
		}
	else{
		D(1, BUG("TIMEOUT!!! ...no heartbeat detected.  :(\n"));
		GIGE_STATUS = 0x02; // No 10G PHY Heartbeat.
		}
}


/*********
UC8051 Status Reg 3.D7FD UC8051_STATUS_REG
	7:0 8 UC_STATUS RW X Micro-controller Status Register
	Indicates the status of the FW.
		10 = Start of FW processing
		20 = Save of FW boot image in progress
		40 = Initialization
		50 = Acquisition in progress
		60 = Acquisition completed
		70 = Tracking in progress
		A0 = In Auto-negotiation (backplane applications)
**********/


void config_10G_external_phy()	// call this upon INSTALLATION of SFP module (into 10G cage)
{
	unsigned int reg_contents=0;
	D(1, BUG("\n-------- Configuring QT2025 PHY --------\n"));

	//Set term off & Disable GPIO1 & GPIO2   note default: 0x609F boot state, after uP init: 0x6D87
	//D(1, BUG("\n\tGPIO STATE Reg (1.C303)=0x%04lX \n",read_ext_10G_phy(0xC303, 1)));
	//write_ext_10G_phy(0xC303, 1, 0x6907);
	//D(1, BUG("\n\tGPIO STATE Reg (1.C303)=0x%04lX \n",read_ext_10G_phy(0xC303, 1)));

	//=== misc PHY configuration (which must be re-written upon Module removal/installation) =================
	reg_contents = read_ext_10G_phy(0xD71A, 3);	// get DOM Periodic Update  register
	//D(1, BUG("Turning on DOM Periodic Update Reg (3.D71A)=%04X\n",reg_contents));
	//	reg_contents &= 0xFFFC;						// mask off periodic update control bit ^1^0
	//	reg_contents |= 0x03;							// 00=once, 01=slow, 10=medium, 11=fast update rate
	//	write_ext_10G_phy(0xA100, 1, reg_contents);	//update periodic update rate
	write_ext_10G_phy(0xD71A, 3, (reg_contents|0x01));	//update periodic update rate
	D(1, BUG("Turning on DOM Periodic Update Reg (3.D71A)=%04X\n",(reg_contents|0x01)));

	//--- additional GUISYS XT2025 PHY initializations ---------------------------
	//write_ext_10G_phy(0xC308, 1, 0x0918); // we want ^9=0 (0x200) and ^14=0 (0x4000)
	D(1, BUG("\nSetting and enabling FPLLOUT..."));
	write_ext_10G_phy(0xC309, 1, (0x1022&(~0x1000)));   	// clr^12 (FPLLOUT_156BUFF_PWDN)
	write_ext_10G_phy(0xC30A, 1, (0x0071|0x0002|0x0008));   //Enable FPLLOUT^1 Tx=clk/66
	D(1, BUG("\n\tFTX CONTROL 1,2,3 Reg (1.C308,9,A)=%04lX %04lX %04lX\n",read_ext_10G_phy(0xC308, 1), read_ext_10G_phy(0xC309, 1), read_ext_10G_phy(0xC30A, 1)));
	//D(1, BUG("\nChecking Core CONTROL Reg (1.F052)=%04lX\n",read_ext_10G_phy(0xF052, 1)));
	//D(1, BUG("\nChecking PMA Vendor Specific Reg (1.C001)=%04lX\n",read_ext_10G_phy(0xC001, 1)));


	// DEBUG poop for Enabling/Disabling 10G SFP+ Tx Laser
	//D(1, BUG("\n\tXFP^11 Reg (1.C301)=0x%04lX \n",read_ext_10G_phy(0xC301, 1)));
	//D(1, BUG("\n\tTXON_FRPAD^0 Reg (1.D002)=0x%04lX \n",read_ext_10G_phy(0xD002, 1)));
	//D(1, BUG("\n\tTXON_FRINV^0 Reg (1.D003)=0x%04lX \n",read_ext_10G_phy(0xD003, 1)));

	//NOTE since our 10G PHY has TXON n/c - it reads LOW and disables ability to Turn OFF Tx Laser
	//	so set INVPAD bit on that input so it reads HIGH, now we can control Tx Laser.
	D(1, BUG("\nSetting INV TXON input signal...(so floater reads HIGH)\n"));
	write_ext_10G_phy(0xD003, 1, (read_ext_10G_phy(0xD003, 1))|0x0001); // set TXONPAD to INV input!

	//D(1, BUG("\n\t...after writing TXON_FRINV^0 Reg (1.D003)=0x%04lX \n",read_ext_10G_phy(0xD003, 1)));
	//D(1, BUG("\n\tTXON_FRPAD^0 Reg (1.D002)=0x%04lX \n",read_ext_10G_phy(0xD002, 1)));

}

void sfp10G_transmit_laser(_Bool sfpa, _Bool enable)
{
	if(sfpa){	// we've only got SFP+ A ....for now
		if(enable){
			write_ext_10G_phy(0x0009, 1, 0x0001);	// 1=enable Tx (QT2x25D: pdf doc spanked)
			D(1, BUG( "\n<> 10G PORT-A SFP+ TRANSMIT LASER: ON!)\n"));
			}
		else{
			write_ext_10G_phy(0x0009, 1, 0);		// 0=disable Tx
			D(1, BUG( "\n<> 10G PORT-A SFP+ TRANSMIT LASER: OFF!)\n"));
			}
		//PHY Tx is disabled (TXENABLE_TOPADB: 1=enabled)  (QT2x25D: pdf doc spanked)
		D(1, BUG("\n\tPMD TxDisable^0 Reg (1.0009)=0x%04lX \n",read_ext_10G_phy(0x0009, 1)));
		}
	else
		D(1, BUG( "\n<> NO PORT-B SFP+ ...so NO CHANGE!\n"));
}

void loopback_xgxs_sys_10G_external_phy(_Bool enable)
{
	unsigned int reg_contents = read_ext_10G_phy(0xC000, 4);

	if(enable){
		write_ext_10G_phy(0xC000, 4, (reg_contents | 0x4000)); 		// set XGXS_SYS Loopback enable bit
		D(1, BUG("\nSetting XGXS_SYS Loopback (%04lX)",read_ext_10G_phy(0xC000, 4)));
		}
	else{
		write_ext_10G_phy(0xC000, 4, (reg_contents & (~0x4000)));	// clr XGXS_SYS Loopback enable bit
		D(1, BUG("\nClearing XGXS_SYS Loopback (%04lX)",read_ext_10G_phy(0xC000, 4)));
		}
}
void loopback_pma_sys_10G_external_phy(_Bool enable)
{
	unsigned int reg_contents = read_ext_10G_phy(0x0000, 1);

	if(enable){
		write_ext_10G_phy(0x0000, 1, (reg_contents | 0x0001)); 		// set PMA_SYS Loopback enable bit
		D(1, BUG("\nSetting PMA_SYS Loopback (%04lX)",read_ext_10G_phy(0x0000, 1)));
		}
	else{
		write_ext_10G_phy(0x0000, 1, (reg_contents & (~0x0001)));	// clr PMA_SYS Loopback enable bit
		D(1, BUG("\nClearing PMA_SYS Loopback (%04lX)",read_ext_10G_phy(0x0000, 1)));
		}
}

//=== (internal) XAUI-PHY initialization =================
void init_10G_xaui()
{
	unsigned char i=0,reg_val=0;

	D(1, BUG("\nInitializing XAUI-PHY (internal)\n"));
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, CAL_BLK_POWERDOWN,0x000F); // set on 4 channels
	//OSTimeDlyHMSM(0, 0, 0, 5);  //wait 1mS and check again!
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, CAL_BLK_POWERDOWN,0x0000); // clr on 4 channels
	//OSTimeDlyHMSM(0, 0, 0, 5);  //wait 1mS and check again!

//	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_CH_BIT_MASK,0x000F); // set to perform reset on 4 channels
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT,0x0003); // initiate Auto ^0:Tx & ^1:Rx reset
//	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_MANUAL_CONTROL,0x000E); // initiate Manual Tx & Rx reset
//	OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
//	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_MANUAL_CONTROL,0x0000); // clear Manual Tx & Rx reset
//	OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
	/***
	D(1, BUG("\nXAUI-PHY wd.0x22 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x22*4))));
	D(1, BUG("XAUI-PHY wd.0x65 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x65*4))));
	D(1, BUG("XAUI-PHY wd.0x66 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x66*4))));
	D(1, BUG("XAUI-PHY wd.0x67 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x67*4))));
	D(1, BUG("XAUI-PHY wd.0x84 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x84*4))));
	D(1, BUG("XAUI-PHY wd.0x85 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x85*4))));
	D(1, BUG("XAUI-PHY wd.0x86 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x86*4))));
	D(1, BUG("XAUI-PHY wd.0x87 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x87*4))));
	D(1, BUG("XAUI-PHY wd.0x88 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x88*4))));
	D(1, BUG("XAUI-PHY wd.0x89 = %04X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, (0x89*4))));
	***/

	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktodata,0x000F);
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktoref,0x000F);
	// Both zero places XAUI CDR in Auto-Lock mode (as opposed to Manual Lock mode)
	// ...this resolved module removal & insertion issue
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktodata,0x0000);
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktoref,0x0000);

	D(1, BUG("\nCHECKING XAUI for XAUI-PHY Tx & Rx_ready....\n"));
	reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
	D(1, BUG("XAUI-PHY Rx/Tx_ready = %04X, ..waiting (if needed)\n",reg_val));
	i=0;
	while((reg_val&0x03) != 0x03 && ++i < 250){ // should change every ~768uS - give him 200mS
		//OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
		usleep(1000);
		reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
		}
	if(i<250){
		D(1, BUG("XAUI-PHY READY!!!  Rx & Tx_ready (%08X).  It took %dmS\n",reg_val ,i));
		D(1, BUG("\tXAUI_RESET_MANUAL_CONTROL = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_MANUAL_CONTROL)));
		D(1, BUG("\tXAUI_PHY_SERIALLPBK = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PHY_SERIALLPBK)));
		D(1, BUG("\tXAUI_PMA_rx_set_locktodata = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktodata)));
		D(1, BUG("\tXAUI_PMA_rx_set_locktoref = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktoref)));
		D(1, BUG("\tXAUI_PMA_rx_is_lockedtodata = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_is_lockedtodata)));
		D(1, BUG("\tXAUI_PMA_rx_is_lockedtoref = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_is_lockedtoref)));
		D(1, BUG("\tPMA_TX_PLL_IS_LOCKED = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, PMA_TX_PLL_IS_LOCKED)));
		}
	else{
		if((reg_val&0x01) != 0x01){ // bit^0 is Tx_ready
			D(1, BUG("TIMEOUT!!!  XAUI-PHY Tx NOT ready!\n"));
			GIGE_STATUS = 0x04; // 10G XAUI Tx not ready
			}
		if((reg_val&0x02) != 0x02){ // bit^1 is Rx_ready
			D(1, BUG("TIMEOUT!!!  XAUI-PHY Rx NOT ready!\n"));
			GIGE_STATUS = 0x05; // 10G XAUI Rx not ready
			}
		if((reg_val&0x03) == 0){ // bit^1&2=0 is Both
			GIGE_STATUS = 0x06; // 10G XAUI Tx & Rx not ready
			}
		D(1, BUG("\tXAUI_RESET_MANUAL_CONTROL = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_MANUAL_CONTROL)));
		D(1, BUG("\tXAUI_PHY_SERIALLPBK = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PHY_SERIALLPBK)));
		D(1, BUG("\tXAUI_PMA_rx_set_locktodata = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktodata)));
		D(1, BUG("\tXAUI_PMA_rx_set_locktoref = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_set_locktoref)));
		D(1, BUG("\tXAUI_PMA_rx_is_lockedtodata = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_is_lockedtodata)));
		D(1, BUG("\tXAUI_PMA_rx_is_lockedtoref = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PMA_rx_is_lockedtoref)));
		D(1, BUG("\tPMA_TX_PLL_IS_LOCKED = %08X\n",IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, PMA_TX_PLL_IS_LOCKED)));
		}
}
void XAUI_SERIAL_SELFLPBK(_Bool enable)
{
	//unsigned int reg_val = 0;//IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PHY_SERIALLPBK);

	if(enable){
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PHY_SERIALLPBK,0x000F);
		D(1, BUG("\nSETTING XAUI serial PMA Loopback (self-loopback)"));
		}
	else{
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PHY_SERIALLPBK,0x0000);
		D(1, BUG("\nCLEARING XAUI serial PMA Loopback (self-loopback)"));
		}

	D(1, BUG(" (%04X)\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_PHY_SERIALLPBK)));
}





//=== SC-FIFO (Tx/Rx) initialization =================
// I.E. ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR; (0x10400 | 0x0010 = 0x10410, word:0x4104)
// I.E. ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR; (0x10600 | 0x0010 = 0x10610, word:0x4184)
void init_10G_SC_FIFO(unsigned char fifo_mode)
{
	unsigned int reg_val=0;

	D(1, BUG("\nInitializing SC-FIFO's (Rx & Tx) Mode=%0d (1=cut_thru,0=S&F)\n",fifo_mode));
	//Set Rx and Tx to store & forward mode: 0=set to store & forward mode, >0=cut-through mode
	//*** 7/28/2014 *** change default write of 0 (store&forward) to 1 (cut-through)
	//FIFO not deep enough to hold full 9600packet - craps out at 4096!  (512x64 = 512x8=4096bytes)
	//10/22 update function to use variable call to set FIFO_MODE in main.c!  ETH_10G_SCFIFO_CUT_THRU in hw_def.h
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR, fifo_mode);
	//Tx FIFO always in Cut-through in FGPA - cannot be modifie in FW.
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_TX_SC_FIFO_BASE_ADDR, SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR, fifo_mode);

	//Enable Rx & Tx drop-on-error: 0=disable, 1=enable
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_DROP_ON_ERROR_ADDR, 0x01);
	//NOTE this is irrelevant in Cut-Thru modes. i.e. it doesn't Drop.
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_TX_SC_FIFO_BASE_ADDR, SC_FIFO_DROP_ON_ERROR_ADDR, 0x01);

	//BT loopback precipitate:  Required for FIFO drop on Full/error
    //reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_ALMOST_FULL_THRESHOLD_ADDR);
    reg_val = 0x7F5;
    //D(1, BUG("\n\n\nreg_val: %x\n\n\n", reg_val));
    IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_ALMOST_FULL_THRESHOLD_ADDR, reg_val-10); //128-10 ingress

    //reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_TX_SC_FIFO_BASE_ADDR, SC_FIFO_ALMOST_FULL_THRESHOLD_ADDR);
    //reg_val = 0x7F5;
    reg_val = 0x3F;
    //D(1, BUG("\n\n\nreg_val: %x\n\n\n", reg_val));
    IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_TX_SC_FIFO_BASE_ADDR, SC_FIFO_ALMOST_FULL_THRESHOLD_ADDR, reg_val-16); //needs max-(10+3) min to warn across swapper latency

}


//=== 10G MAC initialization =================
//default is promiscuous mode
//
void init_10G_MAC()
{
	unsigned int reg_val=0;
	unsigned char i = 0;

	D(1, BUG("\nInitializing 10G MAC\n"));

	//=== Disable MAC transmit and receive datapath ====
	//	Disable the 10GbE MAC transmit and receive datapath before changing any configuration register!
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_TRANSFER_CONTROL_ADDR, 0x01);//Disable the MAC Receive Path
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_TRANSFER_CONTROL_ADDR, 0x01);//Disable the MAC Transmit Path

	//Check the MAC Transmit and Receive Paths are disabled
	reg_val=0;
	i=0;
	while(((reg_val&0x01) == 0) && ++i < 50){ 	  // wait for Rx to be disabled (==1) //wait for 200mS
		OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
		reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_TRANSFER_STATUS_ADDR);
		}
	if(i>50)
		{
		D(1, BUG("10G MAC: Rx Path disable TIMEOUT!!\n"));
		}
	else{
		D(1, BUG("10G MAC: Rx Path disabled\n"));
		}

	reg_val=0;
	i=0;
	while((reg_val&0x01) == 0 && ++i < 50) {	  // wait for Tx to be disabled (==1) //wait for 200mS
		OSTimeDlyHMSM(0, 0, 0, 1);  //wait 1mS and check again!
		reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_TRANSFER_STATUS_ADDR);
		}
	if(i>50)
		{
		D(1, BUG("10G MAC: Tx Path disable TIMEOUT!!\n"));
		}
	else{
		D(1, BUG("10G MAC: Tx Path disabled\n"));
		}
	//=== MAC address configuration ===================
	//Assume MAC address is 00-1C-23-17-4A-CB
	//Configure the MAC Receive MAC Address
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_ADDR0_ADDR, 0x17231C00);
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_ADDR1_ADDR, 0x0000CB4A);
	SET_10G_MAC_RX_ADDR(MAC10G_1, MAC10G_0);

	//Configure the MAC Transmit MAC Address
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_MACADDR0_ADDR, 0xC3C2C1C0); // C5C4, 0xC3C2C1C0
	//IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_MACADDR1_ADDR, 0x0000C5C4);
	SET_10G_MAC_TX_ADDR(MAC10G_1, MAC10G_0);

	//=== MAC function configuration ==================
	//Maximum Frame Length is 9600 bytes
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_MAXLENGTH_ADDR, MAC_FRM_LENGTH);
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_FRAME_MAXLENGTH_ADDR, MAC_FRM_LENGTH);

	//Maximum Pause Quanta Value for Flow Control
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_PAUSEFRAME_QUANTA_ADDR, 1000);

	//CRC and Padding Removal for MAC Receive
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_PADCRC_CONTROL_ADDR, 0x03);

	//Padding Removal for MAC Transmit
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_PADINS_CONTROL_ADDR, 0x01);

	//CRC Removal for MAC Transmit
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_CRCINS_CONTROL_ADDR, 0x03);

	//TX MAC Address Insertion on Transmit Frame
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_CONTROL_ADDR, 0x01);

	//Configure the RX Frame Control Register
	//__________________________________________________________________________________________
	reg_val = EN_ALLMCAST_BIT;
	//Enable/Disable the promiscuous (transparent) mode by setting EN_ALLUCAST bit to 1=Promiscuous, 0=NOT promsicuous!
	if(MAC10A_COMMAND_CONFIG&0x00000001){	// if Promsicuous Mode is Enabled then Turn Promiscuous ON
		reg_val |= EN_ALLUCAST_BIT;
		D(1, BUG("\t10G Promiscuous Mode is ON\n"));
		}
	else{									// else turn Promiscuous OFF
		D(1, BUG("\t10G Promiscuous Mode is OFF\n"));
		}
	if(MAC10A_COMMAND_CONFIG&0x00000002){	// if EnableRxPauseIgnore
		reg_val |= IGNORE_PAUSE_BIT;		// Start Ignoring Rx'd Pause Frames!
		D(1, BUG("\t10G Pause Ignore is ON (No Flow Control)\n"));
		}
	else{
		D(1, BUG("\t10G Pause Ignore is OFF (Flow Control is ON)\n"));
		}
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_CONTROL_ADDR, reg_val);
	//__________________________________________________________________________________________

	//=== Enable MAC transmit and receive datapath ====
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_PACKET_TRANSFER_BASE_ADDR, 0); //Enable the MAC Receive Path
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_TRANSFER_CONTROL_ADDR, 0); //Enable the MAC Transmit Path
	//Check the Transmit and Receive Path is enabled
	reg_val=0x0;
	while(((reg_val&0x01) != 0) && ++i < 50) {	  // wait for Rx to be enabled (==0)
		reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_TRANSFER_STATUS_ADDR);}
	if(i>50)
		{
		D(1, BUG("10G MAC: Rx Path enable TIMEOUT!!\n"));
		}
	else{
		D(1, BUG("10G MAC: Rx Path enabled\n"));
		}

	reg_val=0x0;
	while(((reg_val&0x01) != 0) && ++i < 50) {	  // wait for Tx to be enabled (==0)
		reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_TRANSFER_STATUS_ADDR);}
	if(i>50)
		{
		D(1, BUG("10G MAC: Tx Path enable TIMEOUT!!\n"));
		}
	else{
		D(1, BUG("10G MAC: Tx Path enabled\n"));
		}
	D(1, BUG("Initialization Complete!  10G MAC\n"));
}

void SET_10G_MAC_RX_ADDR(unsigned int mac_address1, unsigned long mac_address0)
{
   IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_ADDR0_ADDR, mac_address0);
   IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_ADDR1_ADDR, mac_address1);
   D(1, BUG("10G MAC Rx address set to %04X%04lX\n",mac_address1, mac_address0));
}

//Configure the MAC Transmit MAC Address
void SET_10G_MAC_TX_ADDR(unsigned int mac_address1, unsigned long mac_address0)
{
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_MACADDR0_ADDR, mac_address0); // C5C4, 0xC3C2C1C0
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_MACADDR1_ADDR, mac_address1);
	D(1, BUG("10G MAC Tx address set to %04X%04lX\n",mac_address1, mac_address0));
}

void CLR_10G_MAC_STATS()
{
   	//reg_write $TXMAC_STATS $ClearStats 0x01
   	//reg_write $RXMAC_STATS $ClearStats 0x01
	D(1, BUG("\n\tCleared 10G MAC Stats\n"));
    IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_STATISTICS_BASE_ADDR|STATISTICS_CLEAR_OFFSET, 0x01);
    IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_STATISTICS_BASE_ADDR|STATISTICS_CLEAR_OFFSET, 0x01);
    maxRxBustinessVal = 0;
}

void dump_MAC_settings()
{
	D(1, BUG("\n\t 	               =================================================================\n"));
	D(1, BUG("\t 			| 10G MAC CONFIGURATION DUMP\n"));
	D(1, BUG("\t 	               =================================================================\n"));
	D(1, BUG("\t 			| RX PAD/CRC STRIPPING	= %04X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_PADCRC_CONTROL_ADDR)));
 	D(1, BUG("\t 			| RX FIFO DROP ON ERROR 	= %04X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_DROP_ON_ERROR_ADDR)));
 	D(1, BUG("\t 			| LOCAL LOOPBACK @ XGMII	= %04X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_LPBK_BASE_ADDR, XGMII_LLPBK)));
 	D(1, BUG("\t 			| LOCAL LOOPBACK @ PMA	= %04X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, SERIAL_LLPBK)));
 	D(1, BUG("\t 			| MAC TX SADDR INSERTION CTRL 	= %04X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_CONTROL_ADDR)));
 	D(1, BUG("\t 			| PRIMARY MAC ADDRESS-1/0	= %08X ", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_ADDR1_ADDR)));
 	D(1, BUG("%08X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_ADDR0_ADDR)));
 	D(1, BUG("\t 			| MAX FRAME LENGTH 		= %0u\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_MAXLENGTH_ADDR)));
 	D(1, BUG("\t 			| SUPPLEMENTARY ADDRESS-0 	= %08X ", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP0_ADDR1_ADDR)));
 	D(1, BUG("%08X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP0_ADDR0_ADDR)));
 	D(1, BUG("\t 			| SUPPLEMENTARY ADDRESS-1	= %08X ", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP1_ADDR1_ADDR)));
 	D(1, BUG("%08X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP1_ADDR0_ADDR)));
 	D(1, BUG("\t 			| SUPPLEMENTARY ADDRESS-2	= %08X ", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP2_ADDR1_ADDR)));
 	D(1, BUG("%08X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP2_ADDR0_ADDR)));
 	D(1, BUG("\t 			| SUPPLEMENTARY ADDRESS-3	= %08X ", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP3_ADDR1_ADDR)));
 	D(1, BUG("%08X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, RX_FRAME_SUP3_ADDR0_ADDR)));
 	D(1, BUG("\t 			| TX MAC SOURCE ADDRESS  	= %08X ", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_MACADDR1_ADDR)));
 	D(1, BUG("%08X\n", IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR, TX_ADDRINS_MACADDR0_ADDR)));
}

void read_10G_MAC_Stats()
{
	unsigned long rx_stat = ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR|RX_STATISTICS_BASE_ADDR;
	unsigned long tx_stat = ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MAC_BASE_ADDR|TX_STATISTICS_BASE_ADDR;
	unsigned long newRxBustinessVal = 0;

	D(1, BUG("\n\t 	               =================================================================\n"));
	D(1, BUG("\t 			| 10G MAC Rx STATISTICS DUMP (and some Tx)\n"));
	D(1, BUG("\t 	               =================================================================\n"));
	D(1, BUG("\t 			| clr [0x00]  			0x%0x\n", IORD_32DIRECT(rx_stat, STATISTICS_CLEAR_OFFSET)));

	unsigned long tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_framesOK_OFFSET); // note we use 64bit reader - butmost reg's are 36-bit
	D(1, BUG("\t 			| Tx framesOK [0x08]			%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_FRAME_TX_OK03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_framesOK_OFFSET);
	D(1, BUG("\t 			| Rx framesOK [0x08]			%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_FRAME_RX_OK03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_framesErr_OFFSET);
	D(1, BUG("\t 			| framesErr [0x10]			%lu  --not in TSE MAC--\n", tmpInt));
	// this is a new 10G stat!!!!!
	// these are Errored Frames Rx'd or Tx'd
	// STATISTICS_ifErrors_OFFSET == Errored or Invalid Frames Rx'd or Tx'd!
	//- possibly load this into the TSE Alignment Error array element(s)

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_framesCRCErr_OFFSET);
	D(1, BUG("\t 			| framesCRCErr [0x18]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_FRAME_FCS_ERRS03_ptr, tmpInt);

	D(1, BUG("\t 			| AlignmentErr [0x??]		--- not in 10G MAC!? ---\n"));
	//SaveBytesLong(ConfigStat10G, MAC_ALIGNMENT_ERRS3_ptr, 0); // just send 0.
	tmpInt = IORD_16DIRECT(SWAPPER64_10G_BASE, SWAPPER64_10G_TXFIFO_FULL_CNT);
	if(tmpInt > 0xFFF0)
		newRxBustinessVal = 0xFFF0;
	else
		newRxBustinessVal = tmpInt;
	if(newRxBustinessVal > maxRxBustinessVal)
		maxRxBustinessVal = newRxBustinessVal;
//	D(1, BUG("\t 			| Rx Burstiness		%lu  max: %u\n", tmpInt, maxRxBustinessVal));
	IOWR_16DIRECT(SWAPPER64_10G_BASE, SWAPPER64_10G_TXFIFO_FULL_CNT, 0x01);		// clear/reset Counter in FPGA
	SaveBytesInt(ConfigStat10G, MAC_ALIGNMENT_ERRS11_ptr, maxRxBustinessVal);	// max value in upper bytes
	SaveBytesInt(ConfigStat10G, MAC_ALIGNMENT_ERRS03_ptr, newRxBustinessVal);	// current value in lower bytes

	tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_octetsOK_OFFSET);
	D(1, BUG("\t 			| Tx octetsOK [0x20]			%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G,  MAC_OCTETS_TX_OK03_ptr, IORD_32DIRECT(tx_stat, STATISTICS_octetsOK_OFFSET));
		SaveBytesLong(ConfigStat10G, MAC_OCTETS_TX_OK13_ptr, IORD_32DIRECT(tx_stat, STATISTICS_octetsOK_OFFSET+STATISTICS_OFFSET_MSB));

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_octetsOK_OFFSET);
	D(1, BUG("\t 			| Rx octetsOK [0x20]			%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G,  MAC_OCTETS_RX_OK03_ptr, IORD_32DIRECT(rx_stat, STATISTICS_octetsOK_OFFSET));
		SaveBytesLong(ConfigStat10G, MAC_OCTETS_RX_OK13_ptr, IORD_32DIRECT(rx_stat, STATISTICS_octetsOK_OFFSET+STATISTICS_OFFSET_MSB));

	tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_pauseMACCtrlFrames_OFFSET);
	D(1, BUG("\t 			| Tx pauseMACCtrlFrames [0x28]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_PAUSE_FRAME_TX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_pauseMACCtrlFrames_OFFSET);
	D(1, BUG("\t 			| Rx pauseMACCtrlFrames [0x28]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_PAUSE_FRAME_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_ifErrors_OFFSET);
	D(1, BUG("\t 			| Tx ifErrors [0x30]			%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_ERR_FRAMES_TX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_ifErrors_OFFSET);
	D(1, BUG("\t 			| Rx ifErrors [0x30]			%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_ERR_FRAMES_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_unicastFramesOK_OFFSET);
	D(1, BUG("\t 			| Tx unicastFramesOK [0x38]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_UNICAST_FRAMES_TX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_unicastFramesOK_OFFSET);
	D(1, BUG("\t 			| Rx unicastFramesOK [0x38]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_UNICAST_FRAMES_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_unicastFramesErr_OFFSET);
	D(1, BUG("\t 			| unicastFramesErr [0x40]		%lu  --not in TSE MAC--\n", tmpInt));

	tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_multicastFramesOK_OFFSET);
	D(1, BUG("\t 			| Tx multicastFramesOK [0x48]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_MULTICAST_FRAMES_TX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_multicastFramesOK_OFFSET);
	D(1, BUG("\t 			| Rx multicastFramesOK [0x48]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_MULTICAST_FRAMES_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_multicastFramesErr_OFFSET);
	D(1, BUG("\t 			| multicastFramesErr [0x50]		%lu  --not in TSE MAC--\n", tmpInt));

	//DEBUG DEBUG DEBUG
	tmpInt = read_10GMAC_64bit_stats(tx_stat, STATISTICS_broadcastFramesOK_OFFSET);
	//tmpInt = read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX);  "MAX_DELAY"
	D(1, BUG("\t 			| Tx broadcastFramesOK [0x58]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_BROADCAST_FRAMES_TX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_broadcastFramesOK_OFFSET);
	D(1, BUG("\t 			| Rx broadcastFramesOK [0x58]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_BROADCAST_FRAMES_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_broadcastFramesErr_OFFSET);
	D(1, BUG("\t 			| broadcastFramesErr [0x60]		%lu  --not in TSE MAC--\n", tmpInt));

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsOctets_OFFSET);
	D(1, BUG("\t 			| etherStatsOctets [0x68]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_ETHER_STATS_OCTETS03_ptr, IORD_32DIRECT(rx_stat, STATISTICS_etherStatsOctets_OFFSET));
		SaveBytesLong(ConfigStat10G, MAC_ETHER_STATS_OCTETS13_ptr, IORD_32DIRECT(rx_stat, STATISTICS_etherStatsOctets_OFFSET+STATISTICS_OFFSET_MSB));

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts [0x70]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_ETHER_STATS_PACKETS03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsUndersizePkts_OFFSET);
	D(1, BUG("\t 			| etherStatsUnderSizePkts [0x78]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_UNDERSIZED_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsOversizePkts_OFFSET);
	D(1, BUG("\t 			| etherStatsOversizePkts [0x80]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_OVERSIZED_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts64Octets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts64Octets [0x88]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_64_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts65to127Octets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts65to127Octets [0x90]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_65_127_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts128to255Octets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts128to255Octets [0x98]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_128_255_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts256to511Octets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts256to511Octet [0xA0]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_256_511_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts512to1023Octets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts512to1023Octets [0xA8]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_512_1023_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatPkts1024to1518Octets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts1024to1518Octets [0xB0]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_1024_1518_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsPkts1519toXOctets_OFFSET);
	D(1, BUG("\t 			| etherStatsPkts1518toXOctets [0xB8]	%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_1519_FLEN_PACKETS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsJabbers_OFFSET);
	D(1, BUG("\t 			| etherStatsJabbers [0xC8]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_JABBERS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsFragments_OFFSET);
	D(1, BUG("\t 			| etherStatsFragments [0xC0]		%lu\n", tmpInt));
		SaveBytesLong(ConfigStat10G, MAC_FRAGMENTS_RX03_ptr, tmpInt);

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_etherStatsCRCErr_OFFSET);
	D(1, BUG("\t 			| etherStatsCRCErr [0xD0]		%lu  --not in TSE MAC--\n", tmpInt));

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_unicastMACCtrlFrames_OFFSET);
	D(1, BUG("\t 			| unicastMACCtrlFrames [0xD8]		%lu  --not in TSE MAC--\n", tmpInt));

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_multicastMACCtrlFrames_OFFSET);
	D(1, BUG("\t 			| multicastMACCtrlFrames [0xE0]	%lu  --not in TSE MAC--\n", tmpInt));

	tmpInt = read_10GMAC_64bit_stats(rx_stat, STATISTICS_broadcastMACCtrlFrames_OFFSET);
	D(1, BUG("\t 			| broadcastMACCtrlFrames [0xE8]	%lu  --not in TSE MAC--\n", tmpInt));

}

unsigned long read_10GMAC_64bit_stats(unsigned long base, unsigned int offset)
{
	unsigned long LOW_32DATA = IORD_32DIRECT(base, offset);
	unsigned long HI_32DATA  = IORD_32DIRECT(base, offset+STATISTICS_OFFSET_MSB);
	unsigned long STAT_64 = (HI_32DATA * 4294967296.0) + LOW_32DATA;
	return(STAT_64);
}


//===============================================================================================================
void write_ext_10G_phy(unsigned int reg, unsigned int devad, unsigned long value)
{
	unsigned long phy_word = reg*0x10000;
	phy_word |= PHY_PRTAD;
	phy_word |= (devad&0x07); //for QT2025: 1, 2, 3 or 4
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MDIO_BASE_ADDR, MDIO_PHY_CONTROL, phy_word);
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MDIO_BASE_ADDR, MDIO_PHY_RW, value);
}
unsigned long read_ext_10G_phy(unsigned int reg, unsigned int devad)
{
	unsigned long phy_word = reg*0x10000, dataout=0;
	phy_word |= PHY_PRTAD;
	phy_word |= (devad&0x07); //for QT2025: 1, 2, 3 or 4
	IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MDIO_BASE_ADDR, MDIO_PHY_CONTROL, phy_word);
	dataout = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_MDIO_BASE_ADDR, MDIO_PHY_RW);
	return dataout;
}


//=============================================================================
//====================================
void set_10G_PHY_reset(_Bool state)	// Actv LOW for 500uS after PowerGood
{
	if(state == HIGH)
		PIO2_Content |= ETH10G_PHY_RESET_MASK;
	else
		PIO2_Content &= ~ETH10G_PHY_RESET_MASK;

	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}

/*** PIO_2 Bit.13 no longer in use in FPGA
void set_TX_REF_CLK_reset(_Bool state)
{
	if(state == HIGH)
		PIO2_Content |= ETH_10G_TX_REF_CLK_RESET_MASK;
	else
		PIO2_Content &= ~ETH_10G_TX_REF_CLK_RESET_MASK;

	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}*/

//PIO2_2 enables/disables the 156.25MHz oscillator to PHY
//must be on many milliseconds before PHY reset is negated
void enable_PHY_clock(_Bool state)
{
	if(state == TRUE)	//Actv High (enable 10G PHY CLK)
		PIO2_Content |= ETH10G_PHY_CLK_MASK;
	else
		PIO2_Content &= ~ETH10G_PHY_CLK_MASK;

	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}


void read_10G_sfp_info(_Bool sfp_a)
{
    if(sfp_a){
    	D(1, BUG("Reading Port-A SFP+ info...\n"));
		sfp_A_read_10G_block_into_Array(Special10GStat, SFP_ADDR_CORE_A0, 0, 128, 2);	// start offset into array at 2 (Link+ID bytes)
		sfp_A_read_10G_block_into_Array(Special10GStat, SFP_ADDR_DIAG_A2, 0, 128, 130);	// start offset into array at 2+127
    	}
    else{
    	D(1, BUG("Reading Port-B SFP+ info...\n"));
		sfp_B_read_10G_block_into_Array(Special10GStatB, SFP_ADDR_CORE_A0, 0, 128, 2);	// start offset into array at 2 (Link+ID bytes)
		sfp_B_read_10G_block_into_Array(Special10GStatB, SFP_ADDR_DIAG_A2, 0, 128, 130);// start offset into array at 2+127
    	}

}
void sfp_A_read_10G_block_into_Array(unsigned char* array, int base_address, int start_address, unsigned int num_to_read, unsigned int offset)
{
	unsigned char i=0;

	if(base_address == SFP_ADDR_CORE_A0){
		for(i=0;i<num_to_read;i++){								// read num# consecutive bytes
			array[i+offset] = read_ext_10G_phy(0xD000+start_address+i, 3); // load A0 mirror into array
			}
		}
	else if(base_address == SFP_ADDR_DIAG_A2){
		for(i=0;i<num_to_read;i++){								// read num# consecutive bytes
			//array[i+offset] = read_ext_10G_phy(0x8007+i, 1); // load A2 mirror into array
			array[i+offset] = read_ext_10G_phy(0xA000+start_address+i, 1); // load A2 mirror into array
			}
		}
}
void sfp_B_read_10G_block_into_Array(unsigned char* array, int base_address, int start_address, unsigned int num_to_read, unsigned int offset)
{
	//will modify/create read_ext_10G_PHY() functions for portA/B access
}

//==================================================================================
void read_10G_sfp_power(_Bool selected_port) // replaces 1G's power when 10G selected!
{
	unsigned char dmi_support = 0;

    if(selected_port){
		dmi_support = Special10GStat[Diagnostic_Monitoring_Type+2];//SFP_ADDR_CORE_A0, Diagnostic_Monitoring_Type +2 in!
		//if(byte92&0x40) != 0){		// if bit.6==1 DMI is implemented
		if(dmi_support == 0xFF || dmi_support == 0x7F || (dmi_support&0x40) != 0x40){	// if reading crap or bit.6!=1 (no DMI)
			ConfigStat10G[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStat10G[PHY10G_OPT_POWER0_ptr] = 0xFF;	// flag GUI to "---" SFP-A RxPower
			//ConfigStat10GB[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStat10GB[PHY10G_OPT_POWER0_ptr] = 0xFF;
			ConfigStat[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStat[PHY10G_OPT_POWER0_ptr] = 0xFF;
			ConfigStatB[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStatB[PHY10G_OPT_POWER0_ptr] = 0xFF;
			return;	// ....then skip trying to read/load power
			}

		//=== need 10G power spot! OR replace 1G's power display when 10G selected!
		sfp_A_read_10G_block_into_Array(ConfigStat10G, SFP_ADDR_DIAG_A2, 104, 2, PHY10G_OPT_POWER1_ptr); // load power at PWR1 (PWR1,PWR0)
		//ConfigStat10GB[PHY10G_OPT_POWER1_ptr] = ConfigStat10G[PHY10G_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		//ConfigStat10GB[PHY10G_OPT_POWER0_ptr] = ConfigStat10G[PHY10G_OPT_POWER0_ptr];
		ConfigStat[PHY10G_OPT_POWER1_ptr] = ConfigStat10G[PHY10G_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStat[PHY10G_OPT_POWER0_ptr] = ConfigStat10G[PHY10G_OPT_POWER0_ptr];
		ConfigStatB[PHY10G_OPT_POWER1_ptr] = ConfigStat10G[PHY10G_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStatB[PHY10G_OPT_POWER0_ptr] = ConfigStat10G[PHY10G_OPT_POWER0_ptr];
    	}
}
    /**** no 10G B port ....yet
    else{
		dmi_support = Special10GStatB[Diagnostic_Monitoring_Type+2];//SFP_ADDR_CORE_A0, Diagnostic_Monitoring_Type +2 in!
		//if(byte92&0x40) != 0){		// if bit.6==1 DMI is implemented
		if(dmi_support == 0xFF || dmi_support == 0x7F || (dmi_support&0x40) != 0x40){	// if reading crap or bit.6!=1 (no DMI)
			ConfigStat10G[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStat10G[PHY10G_OPT_POWER0_ptr] = 0xFF;	// flag GUI to "---" SFP-A RxPower
			ConfigStat10GB[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStat10GB[PHY10G_OPT_POWER0_ptr] = 0xFF;
			ConfigStat[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStat[PHY10G_OPT_POWER0_ptr] = 0xFF;
			ConfigStatB[PHY10G_OPT_POWER1_ptr] = 0xFF; ConfigStatB[PHY10G_OPT_POWER0_ptr] = 0xFF;
			return;	// ....then skip trying to read/load power
			}

		//=== need 10G power spot! OR replace 1G's power display when 10G selected!
		sfp_B_read_10G_block_into_Array(ConfigStat10G, SFP_ADDR_DIAG_A2, 104, 2, PHY10G_OPT_POWER1_ptr); // load power at PWR1 (PWR1,PWR0)
		ConfigStat10G[PHY10G_OPT_POWER1_ptr] = ConfigStat10GB[PHY10G_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStat10G[PHY10G_OPT_POWER1_ptr] = ConfigStat10GB[PHY10G_OPT_POWER1_ptr];
		ConfigStat[PHY10G_OPT_POWER1_ptr] = ConfigStat10GB[PHY10G_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStat[PHY10G_OPT_POWER1_ptr] = ConfigStat10GB[PHY10G_OPT_POWER1_ptr];
		ConfigStatB[PHY10G_OPT_POWER1_ptr] = ConfigStat10GB[PHY10G_OPT_POWER1_ptr];	// cross pollinate power results so Port selection is don't care!
		ConfigStatB[PHY10G_OPT_POWER1_ptr] = ConfigStat10GB[PHY10G_OPT_POWER1_ptr];
    	}
    *****/



//============================================================
void set_10G_MAC_line_loopback(_Bool state)	// Loop Rx XGMII back to Tx (in from and back to PHY)
{
	if(state == YES){
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_LPBK_BASE_ADDR, ETH_10G_LINE_LOOPBACK, 1);
		}
	else{
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_LPBK_BASE_ADDR, ETH_10G_LINE_LOOPBACK, 0);
		}
}

void set_10G_MAC_self_loopback(_Bool state) // loop MAC Tx XGMII to MAC Rx XGMII - Tx still goes to PHY
{
	if(state == YES){
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_LPBK_BASE_ADDR, ETH_10G_LOCAL_LOOPBACK, 1);
		D(1, BUG("\nTurning Self-Loopback: ON (still Tx to PHY)\n"));
		}
	else{
		IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_LPBK_BASE_ADDR, ETH_10G_LOCAL_LOOPBACK, 0);
		D(1, BUG("\nTurning Self-Loopback: OFF (still Tx to PHY)\n"));
		}
}

//====================================
void reset_10G_MAC_FIFOS()
{
	PIO3_Content = IORD_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content | RESET_10GMAC_FIFOS_PIO3_mask | RESET_DCFIFO1_SCFIFO0_PIO3_mask);
	usleep(1);	// 1uSec delay
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content & ~RESET_10GMAC_FIFOS_PIO3_mask & ~RESET_DCFIFO1_SCFIFO0_PIO3_mask);
}

//==================================
void set_reset_10GMAC_FIFOS() {
	PIO3_Content = IORD_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content | RESET_10GMAC_FIFOS_PIO3_mask | RESET_DCFIFO1_SCFIFO0_PIO3_mask);
	//IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content | RESET_DCFIFO1_SCFIFO0_PIO3_mask);
}

void clear_reset_10GMAC_FIFOS() {
	PIO3_Content = IORD_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content & ~RESET_10GMAC_FIFOS_PIO3_mask & ~RESET_DCFIFO1_SCFIFO0_PIO3_mask);
	//IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content & ~RESET_DCFIFO1_SCFIFO0_PIO3_mask);
}



/*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, USA.    *
* All rights reserved.                                                   		*
*************************************************************************/
