/*************************************************************************
*		PHY Link & LED status handling									 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
//	    		   GUISYS		MEMORY MAP
//	TSE0		0x1000-0x13FF
//	ETH0 GEN	0x1A00-0x1A3F
//	ETH0 MON	0x1AA0-0x1ABF
//	TSE1		0x0400-0x07FF
//	CPU DBG		0x0800-0x0FFF
//  ST_MUX		0x0040-0x005F

#include "gigle.h"
#include "extvars.h"

//==============================================================================
//	Read 10G QT2025 PHY Operating Mode: PHY-A
//==============================================================================
//===== for passing up to GUI =================
void read_10G_phy_operating_mode()
{
	unsigned short reg_value = (read_ext_10G_phy(0xD002, 1));

	// ^0:SFP+ RXLOSB_I; 0=LOS, 1=signal
	if(reg_value&0x0004)	// 1.D002^2=RxLOSB_I  0=LOS,1=Signal
		reg_value = 0x0001;	// pass along Signal
	else
		reg_value = 0;

	// ^1:3.0021^15; BLOCK_LOCK 1=Locked, 0=no Lock   (Locked==LINK ACTIVE)
	if((read_ext_10G_phy(0x0021, 3))&0x8000) // ^15=BLOCK_LOCK = Rx Link Active
		reg_value |= 0x0002;

	//SaveBytesInt(ConfigStat10G, PHY_OPERATING_MODE1_ptr, reg_value);
	SaveBytesInt(ConfigStat10G, PHY10GA_OPERATING_MODE1_ptr, reg_value);
	// cc to A & B for GUI access
	SaveBytesInt(ConfigStat, PHY10GA_OPERATING_MODE1_ptr, reg_value);
	SaveBytesInt(ConfigStatB, PHY10GA_OPERATING_MODE1_ptr, reg_value);

	// Not currently used for 10G
	SaveBytesInt(ConfigStat10G, PHY_MII_STAT1_ptr, 0x00);

}


//==============================================================================
//	Update PHY Link ACTIVE LED's..........
//==============================================================================
//===== for passing up to GUI =================
void update_link_actv_leds(unsigned int base, unsigned short reg_value)
{
	if((reg_value & OPERATING_MODE_STATUS_MASK) == OPERATING_MODE_RGMII_COPPER){
		if(reg_value&OPERATING_MODE_COPPER_LINK_UP_MASK){	// LINK UP!

			if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	// NOTE: Fiber Link up will over-ride RJ45 LED ON (to the SFP port)
				set_PortA_LED(ON);		 // PHYB_TURN_LED1PHYA_A_ON;
			else
				set_PortB_LED(ON);		 // PHYB_TURN_LED2PHYB_B_ON;
			}
		else{	// Link Down
			if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
				set_PortA_LED(OFF);		 // PHYB_TURN_LED1PHYA_A_OFF;
			else
				set_PortB_LED(OFF);		 // PHYB_TURN_LED2PHYB_B_OFF;
			}
		}
	// if 100FX (100M Fiber) or SERDES (1G Fiber)
	else if((reg_value & OPERATING_MODE_STATUS_MASK) == OPERATING_MODE_RGMII_100FX || (reg_value & OPERATING_MODE_STATUS_MASK) == OPERATING_MODE_RGMII_SERDES){
		if(reg_value&OPERATING_MODE_FIBER_LINK_UP_MASK){	// LINK UP!
			if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	// NOTE: Fiber Link up will over-ride RJ45 LED ON (to the SFP port)
				set_PortA_LED(ON);		 // PHYB_TURN_LED1PHYA_A_ON;
			else
				set_PortB_LED(ON);		 // PHYB_TURN_LED2PHYB_B_ON;
			}
		else{	// Link Down
			if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
				set_PortA_LED(OFF);		 // PHYB_TURN_LED1PHYA_A_OFF;
			else
				set_PortB_LED(OFF);		 // PHYB_TURN_LED2PHYB_B_OFF;
			}
		}
}


//==============================================================================
//	Read PHY Operating Mode: PHY-A and B via TSE0, TSE1
//==============================================================================
//===== for passing up to GUI =================
void read_phy_operating_mode(unsigned int base)
{
unsigned short	reg_value=0,tmp_int=0;

	SET_17_EXPANSION_TO_OPERATING_MODE_STATUS(base);
	reg_value = READ_EXPANSION_REG17_VIA_15(base);
	DELSELECT_17_EXPANSION_REG(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){	// Save PHY status to Both Port-A and Port-B settings! (each look at both)
		SaveBytesInt(ConfigStat, PHY_OPERATING_MODE1_ptr, reg_value);
		SaveBytesInt(ConfigStatB, PHY_OPERATING_MODE1_ptr, reg_value);
		SaveBytesInt(ConfigStat10G, PHY_OPERATING_MODE1_ptr, reg_value);
		}
	else{
		SaveBytesInt(ConfigStat, PHYB_OPERATING_MODE1_ptr, reg_value);
		SaveBytesInt(ConfigStatB, PHYB_OPERATING_MODE1_ptr, reg_value);
		SaveBytesInt(ConfigStat10G, PHYB_OPERATING_MODE1_ptr, reg_value);
		}

	update_link_actv_leds(base, reg_value);

	//SETREAD_1C_SHADOW_REG(base,MODE_CONTROL_MASK);
	//tmp_int = READ_1C_SHADOW(base);

	tmp_int = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_STATUS_ADDR);
	//if(base == TRIPLE_SPEED_ETHERNET_0_BASE){
	//	D(1, BUG("PHY-A read Status Register = 0x%04X\n",tmp_int));
	//	}
	//else{
	//	D(1, BUG("PHY-B read Status Register = 0x%04X\n",tmp_int));
	//	}
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){	// Save PHY status to Both Port-A and Port-B settings! (each look at both)
		SaveBytesInt(ConfigStat, PHY_MII_STAT1_ptr, tmp_int);
		SaveBytesInt(ConfigStatB, PHY_MII_STAT1_ptr, tmp_int);
		SaveBytesInt(ConfigStat10G, PHY_MII_STAT1_ptr, tmp_int);
		}
	else{
		SaveBytesInt(ConfigStat, PHYB_MII_STAT1_ptr, tmp_int);
		SaveBytesInt(ConfigStatB, PHYB_MII_STAT1_ptr, tmp_int);
		SaveBytesInt(ConfigStat10G, PHYB_MII_STAT1_ptr, tmp_int);
		}

	//new misc PHY status - for now only using single byte
	tmp_int = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_PHY_EXT_STATUS);
	if(tmp_int & MDI_CROSSOVER_STATE_MASK) // Read MDI bit^13: 1=crossed,0=normal
		tmp_int = 0x0001;	//set PHY10G_MISC^0:0=normal,1=crossed
	else
		tmp_int = 0;
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){	// Save PHY status to Both Port-A and Port-B settings! (each look at both)
		/*** debug
		if(ConfigStat[PHY_MISC0_ptr] != tmp_int){
			if(tmp_int & 0x01){
				D(1, BUG("\nA:MDIX Crossed!\n"));
				}
			else{
				D(1, BUG("\nA:MDIX Normal!\n"));
				}
			}
		***/
		ConfigStat[PHY_MISC0_ptr]= tmp_int;
		ConfigStatB[PHY_MISC0_ptr]= tmp_int;
		ConfigStat10G[PHY_MISC0_ptr]= tmp_int;
		}
	else{
		/*** debug
		if(ConfigStat[PHYB_MISC0_ptr] != tmp_int){
			if(tmp_int & 0x01){
				D(1, BUG("\nB:MDIX Crossed!\n"));
				}
			else{
				D(1, BUG("\nB:MDIX Normal!\n"));
				}
			}
		***/
		ConfigStat[PHYB_MISC0_ptr]= tmp_int;
		ConfigStatB[PHYB_MISC0_ptr]= tmp_int;
		ConfigStat10G[PHYB_MISC0_ptr]= tmp_int;
		}
}

//=== DEBUG: recover & decode print PHY A/B operating modes =======================
void read_phy_operating_mode_printf(unsigned int base)
{
unsigned short	reg_value=0,tmp_int=0;

	SET_17_EXPANSION_TO_OPERATING_MODE_STATUS(base);
	reg_value = READ_EXPANSION_REG17_VIA_15(base);
	DELSELECT_17_EXPANSION_REG(base);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		SaveBytesInt(ConfigStat, PHY_OPERATING_MODE1_ptr, reg_value);
	else
		SaveBytesInt(ConfigStat, PHYB_OPERATING_MODE1_ptr, reg_value);

//	SETREAD_1C_SHADOW_REG(base,MODE_CONTROL_MASK);
//	tmp_int = READ_1C_SHADOW(base);
//	D(1, BUG("MODE CONTROL Register = 0x%04X\n",tmp_int));
//	D(1, BUG("PHY STATUS Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_STATUS_ADDR)));

	//=== CHECK for ENERGY on PORT ===================
	if(reg_value&OPERATING_MODE_COPPER_ENERGY_DET){
		D(1, BUG("COPPER Energy!\t"));
		}
	else{
		D(1, BUG("No Copper...\t"));
		}

	if(reg_value&OPERATING_MODE_FIBER_ENERGY_DET){
		D(1, BUG("FIBER Energy!\t"));
		}
	else{
		D(1, BUG("No Fiber...\t"));
		}

	//=== CHECK for LINK ===================
	tmp_int = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_STATUS_ADDR);
	D(1, BUG("\nPHY_MII_STATUS_ADDR = %04X\n",tmp_int));
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		SaveBytesInt(ConfigStat, PHY_MII_STAT1_ptr, tmp_int);
	else
		SaveBytesInt(ConfigStat, PHYB_MII_STAT1_ptr, tmp_int);
/*********	(LINK ACTIVE LED - old way via MII STATUS REG)
	if((tmp_int&0x0004)==0x0004){	// 0x0004 COPPER LINK STATUS:0x0400, FIBER LINK STATUS (in AUX_1G_STATUS REG)
		//PHYB_TURN_LED1PHYA_A_ON;	//	D(1, BUG("PHY Link Up!!! (0x%02X)",tmp_int);
		}
	else{
		//PHYB_TURN_LED1PHYA_A_OFF;	//	D(1, BUG("PHY Link Down! (0x%02X)",tmp_int);
		}
*********/
	if((tmp_int&0x0020)==0x0020){	// 0x0020 = Auto-Neg Complete
		D(1, BUG("Auto-Neg COMPLETE!\n"));
		}
	else{
		D(1, BUG("A-Neg in process.....\n"));
		}

	D(1, BUG("OPERATING MODE:"));
	switch(reg_value&OPERATING_MODE_STATUS_MASK){
		//==============================================
		case OPERATING_MODE_RGMII_COPPER:	//0x0006	// 0x0006 (00110) RGMII to Copper
			D(1, BUG(" RGMII_COPPER"));

			if(reg_value&OPERATING_MODE_COPPER_LINK_UP_MASK){
				D(1, BUG(" - LINK UP!"));
				if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	// NOTE: Fiber Link up will over-ride RJ45 LED ON (to the SFP port)
					set_PortA_LED(ON);		 // PHYB_TURN_LED1PHYA_A_ON;

				else
					set_PortB_LED(ON);		 // PHYB_TURN_LED2PHYB_B_ON;
				}
			else{
				D(1, BUG(" - Link Down"));
				if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
					set_PortA_LED(OFF);		 // PHYB_TURN_LED1PHYA_A_OFF;
				else
					set_PortB_LED(OFF);		 // PHYB_TURN_LED2PHYB_B_OFF;
				}

			//D(1, BUG("COPPER SPEED:"));
			if((reg_value&OPERATING_MODE_COPPER_SPEED_MASK)==OPERATING_MODE_COPPER_SPEED_10M_ANEG){
				if((tmp_int&0x0020)==0x0020){
					D(1, BUG(" (10M"));
					}
				else{
					D(1, BUG(" (ANEG"));
					}
				}
			else if((reg_value&OPERATING_MODE_COPPER_SPEED_MASK)==OPERATING_MODE_COPPER_SPEED_100M){
				D(1, BUG(" (100M"));
				}
			else if((reg_value&OPERATING_MODE_COPPER_SPEED_MASK)==OPERATING_MODE_COPPER_SPEED_1G){
				D(1, BUG(" (1G"));
				}
			else{
				D(1, BUG(" (ERROR!"));
				}

			if(reg_value&OPERATING_MODE_COPPER_DUPLEX_MASK){
				D(1, BUG(" - Full-Duplex)\n"));
				}
			else{
				if((tmp_int&0x0020)==0x0020){
					D(1, BUG(" - Half-Duplex)\n"));
					}
				else{ // ANEN incomplete
					D(1, BUG(" - ANEG)\n"));
					}
				}

			break;
		//==============================================
		case OPERATING_MODE_RGMII_100FX:	//0x000D	// 0x000D (01101) RGMII to 100-Fiber
			D(1, BUG(" RGMII_100FX"));

			if(reg_value&OPERATING_MODE_FIBER_LINK_UP_MASK){
				D(1, BUG(" - LINK UP!"));
				if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	// NOTE: Fiber Link up will over-ride RJ45 LED ON (to the SFP port)
					set_PortA_LED(ON);		 // PHYB_TURN_LED1PHYA_A_ON;
				else
					set_PortB_LED(ON);		 // PHYB_TURN_LED2PHYB_B_ON;
				}
			else{
				D(1, BUG(" - Link Down"));
				if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
					set_PortA_LED(OFF);		 // PHYB_TURN_LED1PHYA_A_OFF;
				else
					set_PortB_LED(OFF);		 // PHYB_TURN_LED2PHYB_B_OFF;
				}

			//D(1, BUG("FIBER SPEED:"));
			if((reg_value&OPERATING_MODE_FIBER_SPEED_MASK)==OPERATING_MODE_FIBER_SPEED_10M_ANEG){
				if((tmp_int&0x0020)==0x0020){
					D(1, BUG(" (10M"));
					}
				else{
					D(1, BUG(" (ANEG"));
					}
				}
			else if((reg_value&OPERATING_MODE_FIBER_SPEED_MASK)==OPERATING_MODE_FIBER_SPEED_100M){
				D(1, BUG(" (100M"));
				}
			else if((reg_value&OPERATING_MODE_FIBER_SPEED_MASK)==OPERATING_MODE_FIBER_SPEED_1G){
				D(1, BUG(" (1G"));
				}
			else{
				D(1, BUG(" (ERROR!"));
				}

			if(reg_value&OPERATING_MODE_FIBER_DUPLEX_MASK){
				D(1, BUG(" - Full-Duplex)\n"));
				}
			else{
				//if((tmp_int&0x0020)==0x0020){	// no Auto-NEG in 100FX mode...
					D(1, BUG(" - Half-Duplex)\n"));
				//	}
				//else{ // ANEN incomplete
				//	D(1, BUG(" - ANEG)\n"));
				//	}
				}

//			if(reg_value&OPERATING_MODE_SYNC_STATUS_VALID)
//				D(1, BUG("SYNC STATUS: SerDes PCS Rcv Sync'd\n");
//			else
//				D(1, BUG("SYNC STATUS: SerDes PCS Rcv NOT Sync'd\n");
			break;
		//==============================================
		case OPERATING_MODE_RGMII_SGMII:	//0x000E	// 0x000E (01110) RGMII to SGMII
			D(1, BUG(" RGMII_SGMII\n"));
			break;
		//==============================================
		case OPERATING_MODE_RGMII_SERDES:	//0x000F	// 0x000F (01101) RGMII to SerDes
			D(1, BUG(" RGMII_SERDES"));

			if(reg_value&OPERATING_MODE_FIBER_LINK_UP_MASK){
				D(1, BUG(" - LINK UP!"));
				if(base == TRIPLE_SPEED_ETHERNET_0_BASE)	// NOTE: Fiber Link up will over-ride RJ45 LED ON (to the SFP port)
					set_PortA_LED(ON);		 // PHYB_TURN_LED1PHYA_A_ON;
				else
					set_PortB_LED(ON);		 // PHYB_TURN_LED2PHYB_B_ON;
				}
			else{
				D(1, BUG(" - Link Down"));
				if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
					set_PortA_LED(OFF);		 // PHYB_TURN_LED1PHYA_A_OFF;
				else
					set_PortB_LED(OFF);		 // PHYB_TURN_LED2PHYB_B_OFF;
				}


			//D(1, BUG("FIBER SPEED:");
			if((reg_value&OPERATING_MODE_FIBER_SPEED_MASK)==OPERATING_MODE_FIBER_SPEED_10M_ANEG){
				if((tmp_int&0x0020)==0x0020){
					D(1, BUG(" (10M"));
					}
				else{
					D(1, BUG(" (ANEG"));
					}
				}
			else if((reg_value&OPERATING_MODE_FIBER_SPEED_MASK)==OPERATING_MODE_FIBER_SPEED_100M){
				D(1, BUG(" (100M"));
				}
			else if((reg_value&OPERATING_MODE_FIBER_SPEED_MASK)==OPERATING_MODE_FIBER_SPEED_1G){
				D(1, BUG(" (1G"));
				}
			else{
				D(1, BUG(" (ERROR!"));
				}

			if(reg_value&OPERATING_MODE_FIBER_DUPLEX_MASK){
				D(1, BUG(" - Full-Duplex)\n"));
				}
			else{
				if((tmp_int&0x0020)==0x0020){
					D(1, BUG(" - Half-Duplex)\n"));
					}
				else{ // ANEN incomplete
					D(1, BUG(" - ANEG)\n"));
					}
				}

//			if(reg_value&OPERATING_MODE_SYNC_STATUS_VALID)
//				D(1, BUG("SYNC STATUS: SerDes PCS Rcv Sync'd\n");
//			else
//				D(1, BUG("SYNC STATUS: SerDes PCS Rcv NOT Sync'd\n");
			break;
		//==============================================
		case OPERATING_MODE_SGMII_SERDES:	//0x0012	// 0x0012 (10010) SGMII to SerDes
			D(1, BUG(" SGMII_SERDES\n"));
			break;
		case OPERATING_MODE_SGMII_100FX:	//0x0013	// 0x0013 (10011) SGMII to 100-Fiber
			D(1, BUG(" SGMII_100FX\n"));
			break;
		case OPERATING_MODE_SGMII_COPPER:	//0x0014	// 0x0014 (10100) SGMII to Copper
			D(1, BUG(" SGMII_COPPER\n"));
			break;
		case OPERATING_MODE_SERDES_SERDES:	//0x0016	// 0x0016 (10110) SerDes to SerDes (media converter)
			D(1, BUG(" SERDES_SERDES\n"));
			break;
		case OPERATING_MODE_SERDES_COPPER:	//0x0017	// 0x0017 (10111) SerDes to Copper (media converter)
			D(1, BUG(" SERDES_COPPER\n"));
			break;
		default:
			break;
	}




	//return(reg_value);

}



//==============================================================================
// CHANGE "PIO2_Content" to "PIO2_Content"
void set_PortA_LED(_Bool state)
{
	PIO2_Content &= ~PORTA_1G_LED_MASK;
	if(state == OFF)	// writing 1 = OFF
		PIO2_Content |= PORTA_1G_LED_MASK;

	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}
void set_PortB_LED(_Bool state)
{
	PIO2_Content &= ~PORTB_1G_LED_MASK;
	if(state == OFF)	// writing 1 = OFF
		PIO2_Content |= PORTB_1G_LED_MASK;

	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}


/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
