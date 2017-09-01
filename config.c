/*************************************************************************
*		TSE-MAC & PHY Configuration Functions 							 *
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


//================================================================
void select_fiber_operation(unsigned int base)
{
	PHY_SELECT_FIBER_MODE(base);
	unsigned short temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value &= PHY_NORMAL_OPERATION;		// set to NORMAL OPERATION
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("\n***************\nsetting Normal FIBER operation! (0x%04X)\n", temp_reg_value));

	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("MII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("PHY read Status Register = 0x%04X\n***************\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_STATUS_ADDR)));
}

//================================================================
void select_copper_operation(unsigned int base)
{
	PHY_SELECT_COPPER_MODE(base);
	unsigned short temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value &= PHY_NORMAL_OPERATION;		// set to NORMAL OPERATION
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("\n***************\nsetting Normal COPPER operation! (0x%04X)\n", temp_reg_value));

	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("MII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("PHY read Status Register = 0x%04X\n***************\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_STATUS_ADDR)));
}

//================================================================
void fiber_power_down(unsigned int base)
{
	PHY_SELECT_COPPER_MODE_FIBER_REGS(base);		// we'd only be powering down fiber if we're running copper
	unsigned short temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value |= MII_CONTROL_POWER_DOWN;		// set to POWER DOWN
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
}

//================================================================
void copper_power_down(unsigned int base)
{
	PHY_SELECT_FIBER_MODE_COPPER_REGS(base);		// we'd only be powering down copper if we're running fiber
	unsigned short temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value |= MII_CONTROL_POWER_DOWN;		// set to POWER DOWN
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
}

//================================================================
void restart_copper_auto_negotiation(unsigned int base)
{
	PHY_SELECT_COPPER_MODE(base);
	unsigned short temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value |= MII_CONTROL_RESTART_ANEG;		// restart auto-negotiation (self-clearing)
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
}

//================================================================
void restart_fiber_auto_negotiation(unsigned int base)
{
	//PHY_SELECT_FIBER_MODE_COPPER_REGS(base);
	PHY_SELECT_FIBER_MODE(base);
	unsigned short temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value |= MII_CONTROL_RESTART_ANEG;		// restart auto-negotiation (self-clearing)
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
}


//==============================================================================
//	Configure PHY Rx Delay Clocks
//==============================================================================
void configure_phy_rx_delay_timing(unsigned int base)
{
	// PHY_SET_RXC_DELAYED_TIMING
	SETREAD_18_AUXCNTL_MISC_CNTL_REG(base);
	unsigned short phy_register_value_temp = READ_18_SHADOW(base);

	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUXCNTL_MISC_CNTL_MASK|(phy_register_value_temp|RGMII_RXC_DELAYED_TIMING)|SET_WRITE_ENABLE_MASK);

	SETREAD_18_AUXCNTL_MISC_CNTL_REG(base);
	phy_register_value_temp = READ_18_SHADOW(base);
	D(1, BUG("<><><> set Rx delayed timing AUXCNTL_REG:MISC_CNTL_REG = 0x%04X\n",phy_register_value_temp));
}

//================================================================
void configure_phy_rx_aligned_timing(unsigned int base)
{
	// PHY_SET_RXC_ALIGNED_TIMING
	SETREAD_18_AUXCNTL_MISC_CNTL_REG(base);
	unsigned short phy_register_value_temp = READ_18_SHADOW(base);

	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUXCNTL_MISC_CNTL_MASK|(phy_register_value_temp&RGMII_RXC_ALIGNED_TIMING_MASK)|SET_WRITE_ENABLE_MASK);

	SETREAD_18_AUXCNTL_MISC_CNTL_REG(base);
	phy_register_value_temp = READ_18_SHADOW(base);
	D(1, BUG("<><><> set Rx aligned timing AUXCNTL_REG:MISC_CNTL_REG = 0x%04X\n",phy_register_value_temp));
}


//==============================================================================
//	Configure TSE0 for PHY-A (MDIO_0)
//==============================================================================
void configure_tse0_to_phya()
{
	IOWR_ALTERA_TSEMAC_MDIO_ADDR0(TRIPLE_SPEED_ETHERNET_0_BASE, PHYA_ADDR); 	// set TSE0:MDIO 0 addr space to PHY A

	D(1, BUG("\n...Checking PHY-A...\n"));
	D(1, BUG("\tMII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("\tMII_STATUS reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_STATUS_ADDR)));
	D(1, BUG("\tPHY_IDENTIFIER_MSB reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_IDENTIFIER_MSB)));
	D(1, BUG("\tPHY_IDENTIFIER_LSB reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_IDENTIFIER_LSB)));
}

//==============================================================================
//	Configure TSE1 for PHY-B (MDIO_0)
//==============================================================================
void configure_tse1_to_phyb()
{
	IOWR_ALTERA_TSEMAC_MDIO_ADDR0(TRIPLE_SPEED_ETHERNET_1_BASE, PHYB_ADDR); 	// set TSE1:MDIO 0 addr space to PHY B

	D(1, BUG("...Checking PHY-B...\n"));
	D(1, BUG("\tMII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("\tMII_STATUS reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_STATUS_ADDR)));
	D(1, BUG("\tPHY_IDENTIFIER_MSB reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_IDENTIFIER_MSB)));
	D(1, BUG("\tPHY_IDENTIFIER_LSB reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_IDENTIFIER_LSB)));
}

//==============================================================================
void set_10G_clock_source(unsigned int newvalue)	// set_10G_clock_source(INT156_CLOCK==0) or set_10G_clock_source(FPLLOUT_CLOCK==1 (GEN) / RVCRD_CLOCK==1 (MON))
{
	IOWR_8DIRECT(CLK_MUXER_INGRESS_BASE, 0X0, newvalue);
	IOWR_8DIRECT(CLK_MUXER_EGRESS_BASE, 0X0, newvalue);
}

//==============================================================================
void set_TxClkA_speed(unsigned int newvalue)
{
	PIO2_Content = (PIO2_Content & PORTA_CLK_MASK) | newvalue;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}
//================================================================
void set_TxClkB_speed(unsigned int newvalue)
{
	PIO2_Content = (PIO2_Content & PORTB_CLK_MASK) | newvalue;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, PIO2_Content);
}


//==============================================================================
//	Configure TxCLK SPEED port A (MAC & PHY)
//==============================================================================
void configure_TxClkA_speed()
{
	//if(PHYA_ENABLE){
		if(PHYA_ETH_SPEED == speed_10M){
			set_TxClkA_speed(PORTA_CLK_10M);	D(1, BUG("\n>>>> Setting Port-A CLK MUX to 10M\n"));
			}
		else if(PHYA_ETH_SPEED == speed_100M){
			set_TxClkA_speed(PORTA_CLK_100M);	D(1, BUG("\n>>>> Setting Port-A CLK MUX to 100M\n"));
			}
		else if(PHYA_ETH_SPEED == speed_1G){
			set_TxClkA_speed(PORTA_CLK_1G);		D(1, BUG("\n>>>> Setting Port-A CLK MUX to 1G\n"));
			}
	//	}
	//else{
	//	set_TxClkA_speed(PORTA_CLK_VCC);
	//	}
}

//===========================
void configure_TxClkB_speed()
{
	//if(PHYA_ENABLE){
		if(PHYB_ETH_SPEED == speed_10M){
			set_TxClkB_speed(PORTB_CLK_10M);	D(1, BUG("\n>>>> Setting Port-B CLK MUX to 10M\n"));
			}
		else if(PHYB_ETH_SPEED == speed_100M){
			set_TxClkB_speed(PORTB_CLK_100M);	D(1, BUG("\n>>>> Setting Port-B CLK MUX to 100M\n"));
			}
		else if(PHYB_ETH_SPEED == speed_1G){
			set_TxClkB_speed(PORTB_CLK_1G);		D(1, BUG("\n>>>> Setting Port-B CLK MUX to 1G\n"));
			}
	//	}
	//else{
	//	set_TxClkB_speed(PORTB_CLK_VCC);
	//	}
}


//================================================================
// call AFTER configured for fiber
void configure_100FX_operation(unsigned int base)
{
	unsigned int temp_reg_value = 0;
	D(1, BUG("\n==== Configuring for 100FX MODE ====\n"));

	set_TxClkA_speed(PORTA_CLK_100M);	D(1, BUG("\n>>>> Setting Port-A CLK MUX to 100M\n"));

	// Enable PHYloopback - has never worked!! ???????????
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	D(1, BUG("\t<><><> Enable PHY Loopback <><><>\n"));
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value  | 0x4000); // OR in lpbk bit^14
	D(1, BUG("\tMII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR)));

	//shadow 13 SERDES_100FX_CTRL_MASK		0x4C00	// pg106  10011
	//PHY_SELECT_100FX_MODE(base, temp_reg_value);			// enable 100FX mode - SERDES_100FX_SERDES_ENABLE
	//flush duplex mode to HDX and turn ON!
	temp_reg_value = (SERDES_100FX_CTRL_MASK&SERDES_100FX_SERDES_MASK)|SERDES_100FX_SERDES_ENABLE;
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){	//SERDES_100FX_SERDES_MASK
		if(PHYA_FIBER_FULLDUPLEX == YES)		temp_reg_value |= SERDES_100FX_SERDES_FDX;
		}
	else{
		if(PHYB_FIBER_FULLDUPLEX == YES)		temp_reg_value |= SERDES_100FX_SERDES_FDX;
		}
	PHY_SELECT_100FX_MODE(base, temp_reg_value);
	//***/

	//EXP_REG_SERDES_RX_CONTROL_MASK 		 // Exp 0x50
	SET_17_EXPANSION_TO_SERDES_RX_CONTROL(base);
	WRITE_EXPANSION_REG17_VIA_15(base, 0x0C3B);		// serdes PWR-Down
	WRITE_EXPANSION_REG17_VIA_15(base, 0x0C3A);		// serdes PWR-Up
	DELSELECT_17_EXPANSION_REG(base);

	// Disable PHYloopback - has never worked!! ???????????
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	D(1, BUG("\t<><><>Disable PHY Loopback <><><>\n"));
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value & 0xBFFF); // CLR  lpbk bit^14
	D(1, BUG("\tMII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR)));
}
void exit_100FX_operation(unsigned int base)
{
	unsigned int temp_reg_value = 0;
	PHY_DESELECT_100FX_MODE(base);		// disable 100FX mode
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR);
	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value | 0x8000); // RESET bit^15
	//usleep(2); //...reset takes 2uS to complete...may want to reading RESET bit stays high until reset is complete
	// CANNOT do subsequent writes to to this reg until reset is complete!
}
//==============================================================================
//	PHY-A Fiber Configuration (TSE0)
//==============================================================================
// set_TxClkA_speed(PORTA_CLK_100M);	D(1, BUG("\n>>>> Setting Port-A CLK MUX to 100M\n"));
void configure_phya_fiber()
{
	unsigned int temp_reg_value = 0;

	D(1, BUG("\n===========================================================\n\tConfiguring PHY-A FIBER\n===========================================================\n"));
	//PHY_SELECT_COPPER_MODE_FIBER_REGS(TRIPLE_SPEED_ETHERNET_0_BASE);
	//D(1, BUG("selecting FIBER register set (and copper operating mode)\n");
	PHY_SELECT_FIBER_MODE(TRIPLE_SPEED_ETHERNET_0_BASE);
	D(1, BUG("selecting FIBER mode & register set\n"));
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x00);	// Ensure Bit.15=0 for RGMII mode

	//D(1, BUG("\n==== Configure PHY-A!\n");
	//=== PHY Control Register (REG 0)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value = temp_reg_value & 0x0040;	// mask off RESET,LPBK,SPEED,ANEN & DUPLEX modes
	temp_reg_value = temp_reg_value | 0x0040;	// [6:0] rsrvd must be 0x40
	D(1, BUG("*** Set PHY SPEED to 1000Mbps\n"));

	if(PHYA_FIBER_ENABLE_AN == 1){
		if(PHYA_ETH_SPEED == speed_1G && PHYA_EXT_LPBK_MODE == 1){	// PHYA_EXT_LPBK_MODE is hardcoded OFF (0)
			D(1, BUG("Disable PHY Auto-Negotiation (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x1000;
			D(1, BUG("Enable PHY Auto-Negotiation\n"));
			}
		}
	else{
		D(1, BUG("Disable PHY Auto-Negotiation\n"));
		}

	if (PHYA_FIBER_FULLDUPLEX == 1){
		///TODO
		if(PHYA_ETH_SPEED == speed_1G && PHYA_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY In Full Duplex Mode (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x0100;
			D(1, BUG("Enable PHY In Full Duplex Mode\n"));
			}
		}
	else{
		D(1, BUG("Enable PHY In Half Duplex Mode\n"));
		}

	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("PHY MII Control Register= 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));

	//=== PHY AN Advertisement Register (REG 4)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUTO_NEG_ADV);
	temp_reg_value &= 0xFF9F; 	// mask all to 0 - except 1000X DPX ADV's
	temp_reg_value |= 0x0060;	// adv FDX!  0x0060 adv FDX & HDX
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUTO_NEG_ADV, temp_reg_value);
	D(1, BUG("PHY read AN Advertisement Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUTO_NEG_ADV)));

	//no need show COPPER registers in FIBER mode
	//D(1, BUG("PHY read 1000BASE-T Control Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1000_CONTROL));
	//D(1, BUG("PHY read 1000BASE-T Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1000_STATUS));

	// Don't know if this is for copper and FIBER or copper only
	if(PHYA_ETH_SPEED == speed_1G){
		if(PHYA_EXT_LPBK_MODE == 1){
			PHY_ENABLE_EXTERNAL_LOOPBACK(TRIPLE_SPEED_ETHERNET_0_BASE);
			D(1, BUG("<><><><><> PHY SET TO SUPPORT EXTERNAL LOOPBACK! <><><><><>\n"));
			}
		else
			PHY_NORMAL_REMOVE_EXT_LOOPBACK(TRIPLE_SPEED_ETHERNET_0_BASE);

		//....for loopback w/o external HW loop......if desired)
		//#define SWAP_RX_MDIX	0x0010	// force Tx/Rx on same pair, written to PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK)
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK | SWAP_RX_MDIX);
		//D(1, BUG("\t Swap Rx MDIX: Rx/Tx on same Pair\n");
		}


	// Enable PHYloopback - has never worked!! ???????????
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	if (PHYA_LOOPBACK == 1){
		temp_reg_value = temp_reg_value | 0x4000;
		D(1, BUG("<><><> Enable PHY Loopback <><><>\n"));
		}
	else	// cleared way at the top when MII CONTROL MASKED for 1st write
		D(1, BUG("Disable PHY Loopback\n"));
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);

	//DEBUG: turn OFF Fiber Signal Detect = Signal Detect ANDed with SYNC status, ie Use Signal Detect Directly
	//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER, CPRFBR_AUTODETECT_MASK|0x0042|SET_WRITE_ENABLE_MASK);
	// set FIBER LED mode....when in FIBER mode
//	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER, CPRFBR_AUTODETECT_MASK|0x0040|SET_WRITE_ENABLE_MASK);

	D(1, BUG("MII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("PHY read Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_STATUS_ADDR)));
	D(1, BUG("PHYA_CONFIG (fiber) = 0x%04X\n",PHYA_CONFIG));
}

//==============================================================================
//	PHY-B Fiber Configuration (TSE1)
//==============================================================================
void configure_phyb_fiber()
{
	unsigned int temp_reg_value = 0;

	D(1, BUG("\n===========================================================\n\tConfiguring PHY-B FIBER\n===========================================================\n"));
	//PHY_SELECT_COPPER_MODE_FIBER_REGS(TRIPLE_SPEED_ETHERNET_1_BASE);
	//D(1, BUG("selecting FIBER register set (and copper operating mode)\n");
	PHY_SELECT_FIBER_MODE(TRIPLE_SPEED_ETHERNET_1_BASE);
	D(1, BUG("selecting FIBER mode & register set\n"));
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x00);	// Ensure Bit.15=0 for RGMII mode

	//D(1, BUG("\n==== Configure PHY-B!\n");
	//=== PHY Control Register (REG 0)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value = temp_reg_value & 0x0040;	// mask off RESET,LPBK,SPEED,ANEN & DUPLEX modes
	temp_reg_value = temp_reg_value | 0x0040;	// [6:0] rsrvd must be 0x40
	D(1, BUG("*** Set PHY SPEED to 1000Mbps\n"));

	if(PHYB_FIBER_ENABLE_AN == 1){
		if(PHYB_ETH_SPEED == speed_1G && PHYB_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY Auto-Negotiation (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x1000;
			D(1, BUG("Enable PHY Auto-Negotiation\n"));
			}
		}
	else{
		D(1, BUG("Disable PHY Auto-Negotiation\n"));
		}

	if (PHYB_FIBER_FULLDUPLEX == 1){
		if(PHYB_ETH_SPEED == speed_1G && PHYB_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY In Full Duplex Mode (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x0100;
			D(1, BUG("Enable PHY In Full Duplex Mode\n"));
			}
		}
	else{
		D(1, BUG("Enable PHY In Half Duplex Mode\n"));
		}


	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("PHY MII Control Register= 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));

	//=== PHY AN Advertisement Register (REG 4)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUTO_NEG_ADV);
	temp_reg_value &= 0xFF9F; // mask all to 0 - except 1000X DPX ADV's
	temp_reg_value |= 0x0060;	// adv FDX! 0x0060 adv FDX & HDX
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUTO_NEG_ADV, temp_reg_value);
	D(1, BUG("PHY read AN Advertisement Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUTO_NEG_ADV)));

	//no need show COPPER registers in FIBER mode
	D(1, BUG("PHY read 1000BASE-T Control Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1000_CONTROL)));
	D(1, BUG("PHY read 1000BASE-T Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1000_STATUS)));

	// Don't know if this is for copper and FIBER or copper only
	if(PHYB_ETH_SPEED == speed_1G){
		if(PHYB_EXT_LPBK_MODE == 1){
			PHY_ENABLE_EXTERNAL_LOOPBACK(TRIPLE_SPEED_ETHERNET_1_BASE);
			D(1, BUG("<><><><><> PHY SET TO SUPPORT EXTERNAL LOOPBACK! <><><><><>\n"));
			}
		else
			PHY_NORMAL_REMOVE_EXT_LOOPBACK(TRIPLE_SPEED_ETHERNET_1_BASE);

		//....for loopback w/o external HW loop......if desired)
		//#define SWAP_RX_MDIX	0x0010	// force Tx/Rx on same pair, written to PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK)
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK | SWAP_RX_MDIX);
		//D(1, BUG("\t Swap Rx MDIX: Rx/Tx on same Pair\n");
		}


	// Enable PHYloopback - has never worked!! ???????????
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	if (PHYB_LOOPBACK == 1){
		temp_reg_value = temp_reg_value | 0x4000;
		D(1, BUG("<><><> Enable PHY Loopback <><><>\n"));
		}
	else{	// cleared way at the top when MII CONTROL MASKED for 1st write
		D(1, BUG("Disable PHY Loopback\n"));
		}
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);

	//DEBUG turn OFF Fiber Signal Detect = Signal Detect ANDed with SYNC status, ie Use Signal Detect Directly
	//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER, CPRFBR_AUTODETECT_MASK|0x0042|SET_WRITE_ENABLE_MASK);
	// set FIBER LED mode....when in FIBER mode
//	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER, CPRFBR_AUTODETECT_MASK|0x0040|SET_WRITE_ENABLE_MASK);

	D(1, BUG("MII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("PHY read Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_STATUS_ADDR)));
	D(1, BUG("PHYB_CONFIG (fiber) = 0x%04X\n",PHYB_CONFIG));
}

//==============================================================================
//	PHY-A Configuration (TSE1)
//==============================================================================
void configure_phya_copper()	// configure_phya_copper(unsigned int base)
{
	unsigned int temp_reg_value = 0;

	D(1, BUG("\n===========================================================\n\tConfiguring PHY-A COPPER\n===========================================================\n"));
	PHY_SELECT_COPPER_MODE(TRIPLE_SPEED_ETHERNET_0_BASE); // ...and reg's
	D(1, BUG("selecting COPPER mode & register set\n"));
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x00);	// Ensure Bit.15=0 for RGMII mode

	//D(1, BUG("\n==== Configure PHY-A!\n");
	//=== PHY Control Register (REG 0)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value = temp_reg_value & 0x0EBF;	// mask off RESET,LPBK,SPEED,ANEN & DUPLEX modes

	switch(PHYA_ETH_SPEED){
		case speed_10M:
			// SPEED bits (6,13) stay at 00 - done by MASK above
			D(1, BUG("*** Set PHY SPEED to 10Mbps\n"));
			break;
		case speed_100M:
			temp_reg_value = temp_reg_value | 0x2000;
			D(1, BUG("*** Set PHY SPEED to 100Mbps\n"));
			break;
		case speed_1G:
			temp_reg_value = temp_reg_value | 0x0040;
			D(1, BUG("*** Set PHY SPEED to 1000Mbps\n"));
			break;
		default:
			D(1, BUG("INVALID - NO CHANGE!  ...PHY SPEED stays at last value.\n"));
			break;
		}

	if(PHYA_COPPER_ENABLE_AN == 1){
		if(PHYA_ETH_SPEED == speed_1G && PHYA_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY Auto-Negotiation (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x1000;
			D(1, BUG("Enable PHY Auto-Negotiation\n"));
			}
		}
	else{
		D(1, BUG("Disable PHY Auto-Negotiation\n"));
		}

	if (PHYA_COPPER_FULLDUPLEX == 1){
		if(PHYA_ETH_SPEED == speed_1G && PHYA_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY In Full Duplex Mode (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x0100;
			D(1, BUG("Enable PHY In Full Duplex Mode\n"));
			}
		}
	else{
		D(1, BUG("Enable PHY In Half Duplex Mode\n"));
		}

	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("PHY read Control Register= 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));

	//=== PHY AN Advertisement Register (REG 4)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUTO_NEG_ADV);
	temp_reg_value = temp_reg_value & 0xFE1F;

	// As test set MAY want to adv only 1 if NOT in auto-neg!  ??
	switch (PHYA_ETH_SPEED) {
		case speed_10M:
			if(PHYA_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0040;	// adv 10FDX
				D(1, BUG("Advertise PHY 10BASE-TX Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				D(1, BUG("Advertise PHY 10BASE-TX Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				D(1, BUG("Advertise PHY 10BASE-TX Half Duplex\n"));
				}
			break;
		case speed_100M:	//Then  adv 10 & 100
			if (PHYA_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0040; 	// adv 10FDX
				temp_reg_value = temp_reg_value | 0x0100;	// adv 100FDX
				//D(1, BUG("Advertise PHY 100BASE-TX Full Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			break;
		case speed_1G:	//at 1G adv 10 & 100 as well!
			if (PHYA_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0040; 	// adv 10FDX
				temp_reg_value = temp_reg_value | 0x0100;	// adv 100FDX
				//D(1, BUG("Advertise PHY 100BASE-TX Full Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			break;
		default:
			break;
		}

	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUTO_NEG_ADV, temp_reg_value);
	D(1, BUG("PHY read AN Advertisement Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUTO_NEG_ADV)));

	//=== PHY 1000BASE-T Control Register (REG 9)
//	if(PHYA_COPPER_ENABLE_AN == 0){	// if AUTO_NEG is NOT ON; speed, DPX mode and master/slave must be set
		temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1000_CONTROL);
		temp_reg_value = temp_reg_value & 0xFCFF;
		temp_reg_value = temp_reg_value & 0xE7FF;	// mask off Auto/Manual Master/slave selection and Master/slave flag

		//#define PHY_1G_FORCE_MASTER	0x1800	// OR into (reg9) PHY_1000_CONTROL for operation w/external HW loopback
		// they also want 0x0040 written to PHY_MII_CONTROL_ADDR (forced 1000speed and no FDX!??)
		// 0x8400 to reg 18
		//#define AUX_CONTROL_ENABLE_EXT_LOOPBACK	0x8400
		//#define AUX_CONTROL_NORMAL_OPERATION	    0x0400
		//#define PHY_NORMAL_REMOVE_EXT_LOOPBACK	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|AUX_CONTROL_NORMAL_OPERATION);

		if(PHYA_ETH_SPEED == speed_1G){
			if (PHYA_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0200;	// bit.9
				D(1, BUG("Advertise PHY 1000BASE-T Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0100;	// bit.8
				D(1, BUG("Advertise PHY 1000BASE-T Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0100;	// bit.8
				D(1, BUG("Advertise PHY 1000BASE-T Half Duplex\n"));
				}
			if (PHYA_EXT_LPBK_MODE == 1){
				temp_reg_value = temp_reg_value | PHY_1G_FORCE_MASTER;	// Force manual selection of Master mode!
				D(1, BUG("MANUAL MASTER MODE Selected!\n"));
				}
			else {
				//temp_reg_value = temp_reg_value | 0x0100;	// bit.8
				D(1, BUG("Std Auto-Master/Slave selection\n"));
				}
			}

		IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1000_CONTROL, temp_reg_value);
		D(1, BUG("PHY read 1000BASE-T Control Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1000_CONTROL)));
		D(1, BUG("PHY read 1000BASE-T Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1000_STATUS)));
//		}//End if ENABLE_AN is ON


	//=======MODIFY bits for EXT LPBK and Jumbo mode then write to AUX_CONTROL_REG_MASK of PHY_AUXCNTL_REGISTER_SELECT======
	temp_reg_value = AUX_CONTROL_NORMAL_OPERATION;
	if(PHYA_ETH_SPEED == speed_1G){
		if(PHYA_EXT_LPBK_MODE == 1){
			//PHY_ENABLE_EXTERNAL_LOOPBACK(TRIPLE_SPEED_ETHERNET_0_BASE);
			temp_reg_value = AUX_CONTROL_ENABLE_EXT_LOOPBACK;
			D(1, BUG("<><><><><> PHY-A SET TO SUPPORT EXTERNAL LOOPBACK! <><><><><>\n"));
			}
		//else
			//PHY_NORMAL_REMOVE_EXT_LOOPBACK(TRIPLE_SPEED_ETHERNET_0_BASE);
		//....for loopback w/o external HW loop......(nominal diff)
		//#define SWAP_RX_MDIX	0x0010	// force Tx/Rx on same pair, written to PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK)
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK | SWAP_RX_MDIX);
		//D(1, BUG("\t Swap Rx MDIX: Rx/Tx on same Pair\n");
		}
	//=== Set 10M jumbo mode (up to 10k frame size)
	if(PHYA_ETH_SPEED == speed_100M){
		//PHY_ENABLE_COPPER_JUMBO(TRIPLE_SPEED_ETHERNET_0_BASE);	// set bit^14=1
		temp_reg_value |= AUX_CONTROL_SET_COPPER_JUMBO;
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x0001); // set bit^0=1 for hi-latency (for 1G Jumbo mode, fyi:adds 16nS)
		D(1, BUG("<><><> PHY-A set for 100M JUMBO MODE <><><>\n"));
		}
	else{
	  	//PHY_DISABLE_COPPER_JUMBO(TRIPLE_SPEED_ETHERNET_0_BASE); // this is default (bit^14=0)
		temp_reg_value |= AUX_CONTROL_NO_COPPER_JUMBO;
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x0000); // set bit^0=1 for low-latency
		D(1, BUG("<><><> PHY-A: DISABLE 100M JUMBO MODE<><><>\n"));
		}

	//Write EXT LPBK and Jumbo mode to AUX_CONTROL_REG_MASK of PHY_AUXCNTL_REGISTER_SELECT
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|temp_reg_value);

	//default for PHY_PHY_EXT_CONTROL reg is all zero's (really only need for 1G JUMBO mode)
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x0001); // set bit^0=1 for hi-latency (for 1G Jumbo mode, fyi:adds 16nS)
	//======================================================================================


	//==================== RGMII timing ========================
	// ***** set RX and TXC/D timing delay on/off based on tcl_defs *********
	if(PHYA_RGMII_TX_TIMING_CTRL_ON){
		SET_PHY_GTX_CLOCK_DELAY_ON(TRIPLE_SPEED_ETHERNET_0_BASE);
		D(1, BUG("Set Tx Delay ON ---> GTX_CLOCK_DELAY_ON\n"));
		}
	else{
		SET_PHY_GTX_CLOCK_DELAY_BYPASSED(TRIPLE_SPEED_ETHERNET_0_BASE);
		D(1, BUG("Set Tx Delay OFF ---> GTX_CLOCK_DELAY_BYPASSED\n"));
		}
	if(PHYA_RGMII_RX_TIMING_CTRL_ON)
		configure_phy_rx_delay_timing(TRIPLE_SPEED_ETHERNET_0_BASE);
	else
		configure_phy_rx_aligned_timing(TRIPLE_SPEED_ETHERNET_0_BASE);
	//==================



	// Enable PHYloopback - has NEVER worked! ??????
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	if (PHYA_LOOPBACK == 1){
		temp_reg_value = temp_reg_value | 0x4000;
		D(1, BUG("<><><> Enable PHY Loopback <><><>\n"));
		}
	else{	// cleared way at the top when MII CONTROL MASKED for 1st write
		D(1, BUG("Disable PHY Loopback\n"));
		}
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("MII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("PHY read Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_MII_STATUS_ADDR)));
	D(1, BUG("PHYA_CONFIG (copper) = 0x%04X\n",PHYA_CONFIG));

    // DEBUG READ to see PHY CLOCK settings
	SETREAD_18_AUXCNTL_MISC_CNTL_REG(TRIPLE_SPEED_ETHERNET_0_BASE);
	temp_reg_value = READ_18_SHADOW(TRIPLE_SPEED_ETHERNET_0_BASE);
	D(1, BUG("RGMII TIMING SETTING: Rx timing Setting 18:MISC_CNTL_REG = 0x%04X\n (0x0100=delayed,0=aligned)\n",temp_reg_value));

	SETREAD_1C_SHADOW_REG(TRIPLE_SPEED_ETHERNET_0_BASE,CLOCK_ALIGN_MASK);
	temp_reg_value = READ_1C_SHADOW(TRIPLE_SPEED_ETHERNET_0_BASE);
	D(1, BUG("RGMII TIMING SETTING: GTX timing Setting 1C:CLOCK_ALIGN_REG = 0x%04X\n (0x0200=delayed,0=bypassed)\n",temp_reg_value));


}

//==============================================================================
//	PHY-B Configuration (TSE1)
//==============================================================================
void configure_phyb_copper()	// configure_phyb_copper(unsigned int base)
{
	unsigned int temp_reg_value = 0;

	D(1, BUG("\n===========================================================\n\tConfiguring PHY-B COPPER\n===========================================================\n"));
	PHY_SELECT_COPPER_MODE(TRIPLE_SPEED_ETHERNET_1_BASE); // ...and reg's
	D(1, BUG("selecting COPPER mode & register set\n"));
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x00);	// Ensure Bit.15=0 for RGMII mode

	//D(1, BUG("\n==== Configure PHY-B!\n");
	//=== PHY Control Register (REG 0)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	temp_reg_value = temp_reg_value & 0x0EBF;	// mask off RESET,LPBK,SPEED,ANEN & DUPLEX modes

	switch(PHYB_ETH_SPEED){
		case speed_10M:
			// SPEED bits (6,13) stay at 00 - done by MASK above
			D(1, BUG("*** Set PHY SPEED to 10Mbps\n"));
			break;
		case speed_100M:
			temp_reg_value = temp_reg_value | 0x2000;
			D(1, BUG("*** Set PHY SPEED to 100Mbps\n"));
			break;
		case speed_1G:
			temp_reg_value = temp_reg_value | 0x0040;
			D(1, BUG("*** Set PHY SPEED to 1000Mbps\n"));
			break;
		default:
			D(1, BUG("INVALID - NO CHANGE!  ...PHY SPEED stays at last value.\n"));
			break;
		}

	if(PHYB_COPPER_ENABLE_AN == 1){
		if(PHYB_ETH_SPEED == speed_1G && PHYB_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY Auto-Negotiation (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x1000;
			D(1, BUG("Enable PHY Auto-Negotiation\n"));
			}
		}
	else{
		D(1, BUG("Disable PHY Auto-Negotiation\n"));
		}

	if (PHYB_COPPER_FULLDUPLEX == 1){
		if(PHYB_ETH_SPEED == speed_1G && PHYB_EXT_LPBK_MODE == 1){
			D(1, BUG("Disable PHY In Full Duplex Mode (due to 1000+EXT_LPBK_MODE)\n"));
			}
		else{
			temp_reg_value = temp_reg_value | 0x0100;
			D(1, BUG("Enable PHY In Full Duplex Mode\n"));
			}
		}
	else{
		D(1, BUG("Enable PHY In Half Duplex Mode\n"));
		}

	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("PHY read Control Register= 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));

	//=== PHY AN Advertisement Register (REG 4)
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUTO_NEG_ADV);
	temp_reg_value = temp_reg_value & 0xFE1F;

	// As test set MAY want to adv only 1 if NOT in auto-neg!  ??
	switch (PHYB_ETH_SPEED) {
		case speed_10M:
			if(PHYB_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0040;
				D(1, BUG("Advertise PHY 10BASE-TX Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0020;
				D(1, BUG("Advertise PHY 10BASE-TX Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0020;
				D(1, BUG("Advertise PHY 10BASE-TX Half Duplex\n"));
				}
			break;
		case speed_100M:
			if (PHYB_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0040; 	// adv 10FDX
				temp_reg_value = temp_reg_value | 0x0100;	// adv 100FDX
				//D(1, BUG("Advertise PHY 100BASE-TX Full Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			break;
		case speed_1G:	//at 1G adv 10 & 100 as well!
			if (PHYB_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0040; 	// adv 10FDX
				temp_reg_value = temp_reg_value | 0x0100;	// adv 100FDX
				//D(1, BUG("Advertise PHY 100BASE-TX Full Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0020;	// adv 10HDX
				temp_reg_value = temp_reg_value | 0x0080;	// adv 100HDX
				//D(1, BUG("Advertise PHY 100BASE-TX Half Duplex\n");
				D(1, BUG("Advertise PHY 10BASE-TX & 100BASE-TX Half Duplex\n"));
				}
			break;
		default:
			break;
		}

	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUTO_NEG_ADV, temp_reg_value);
	D(1, BUG("PHY read AN Advertisement Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUTO_NEG_ADV)));

	//=== PHY 1000BASE-T Control Register (REG 9)
//	if(PHYB_ENABLE_AN == 0){	// if AUTO_NEG is NOT ON; speed, DPX mode and master/slave must be set
		temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1000_CONTROL);
		temp_reg_value = temp_reg_value & 0xFCFF;
		temp_reg_value = temp_reg_value & 0xE7FF;	// mask off Auto/Manual Master/slave selection and Master/slave flag

		//#define PHY_1G_FORCE_MASTER	0x1800	// OR into (reg9) PHY_1000_CONTROL for operation w/external HW loopback
		// they also want 0x0040 written to PHY_MII_CONTROL_ADDR (forced 1000speed and no FDX!??)
		// 0x8400 to reg 18
		//#define AUX_CONTROL_ENABLE_EXT_LOOPBACK	0x8400
		//#define AUX_CONTROL_NORMAL_OPERATION	    0x0400
		//#define PHY_NORMAL_REMOVE_EXT_LOOPBACK	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|AUX_CONTROL_NORMAL_OPERATION);

		if(PHYB_ETH_SPEED==speed_1G){
			if (PHYB_COPPER_FULLDUPLEX == 1){
				temp_reg_value = temp_reg_value | 0x0200;	// bit.9
				D(1, BUG("Advertise PHY 1000BASE-T Full Duplex\n"));

				temp_reg_value = temp_reg_value | 0x0100;	// bit.8
				D(1, BUG("Advertise PHY 1000BASE-T Half Duplex\n"));
				}
			else {
				temp_reg_value = temp_reg_value | 0x0100;	// bit.8
				D(1, BUG("Advertise PHY 1000BASE-T Half Duplex\n"));
				}
			if (PHYB_EXT_LPBK_MODE == 1){
				temp_reg_value = temp_reg_value | PHY_1G_FORCE_MASTER;	// Force manual selection of Master mode!
				D(1, BUG("MANUAL MASTER MODE Selected!\n"));
				}
			else {
				//temp_reg_value = temp_reg_value | 0x0100;	// bit.8
				D(1, BUG("Std Auto-Master/Slave selection\n"));
				}
			}

		IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1000_CONTROL, temp_reg_value);
		D(1, BUG("PHY read 1000BASE-T Control Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1000_CONTROL)));
		D(1, BUG("PHY read 1000BASE-T Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1000_STATUS)));
//		}//End if ENABLE_AN is ON



	//=======MODIFY bits for EXT LPBK and Jumbo mode then write to AUX_CONTROL_REG_MASK of PHY_AUXCNTL_REGISTER_SELECT======
	temp_reg_value = AUX_CONTROL_NORMAL_OPERATION;
	if(PHYB_ETH_SPEED == speed_1G){
		if(PHYB_EXT_LPBK_MODE == 1){
			//PHY_ENABLE_EXTERNAL_LOOPBACK(TRIPLE_SPEED_ETHERNET_0_BASE);
			temp_reg_value = AUX_CONTROL_ENABLE_EXT_LOOPBACK;
			D(1, BUG("<><><><><> PHY-B SET TO SUPPORT EXTERNAL LOOPBACK! <><><><><>\n"));
			}
		//else
			//PHY_NORMAL_REMOVE_EXT_LOOPBACK(TRIPLE_SPEED_ETHERNET_0_BASE);
		//....for loopback w/o external HW loop......(nominal diff)
		//#define SWAP_RX_MDIX	0x0010	// force Tx/Rx on same pair, written to PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK)
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT, AUXCNTL_MISC_TEST_MASK | SWAP_RX_MDIX);
		//D(1, BUG("\t Swap Rx MDIX: Rx/Tx on same Pair\n");
		}
	//=== Set 10M jumbo mode (up to 10k frame size)
	if(PHYB_ETH_SPEED == speed_100M){
		//PHY_ENABLE_COPPER_JUMBO(TRIPLE_SPEED_ETHERNET_1_BASE);	// set bit^14=1
		temp_reg_value |= AUX_CONTROL_SET_COPPER_JUMBO;
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x0001); // set bit^0=1 for hi-latency (for 1G Jumbo mode, fyi:adds 16nS)
		D(1, BUG("<><><> PHY-B set for 100M JUMBO MODE <><><>\n"));
		}
	else{
	  	//PHY_DISABLE_COPPER_JUMBO(TRIPLE_SPEED_ETHERNET_1_BASE); // this is default (bit^14=0)
		temp_reg_value |= AUX_CONTROL_NO_COPPER_JUMBO;
		//IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x0000); // set bit^0=1 for low-latency
		D(1, BUG("<><><> PHY-B: DISABLE 100M JUMBO MODE<><><>\n"));
		}
	//Write EXT LPBK and Jumbo mode to AUX_CONTROL_REG_MASK of PHY_AUXCNTL_REGISTER_SELECT
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|temp_reg_value);

	//default for PHY_PHY_EXT_CONTROL reg is all zero's (really only need for 1G JUMBO mode)
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_PHY_EXT_CONTROL, 0x0001); // set bit^0=1 for hi-latency (for 1G jumbo fyi:adds 16nS)
	//==========================================================================================


	//====================
	// ***** set RX and TXC/D timing delay on/off based on tcl_defs *********
	if(PHYB_RGMII_TX_TIMING_CTRL_ON){
		SET_PHY_GTX_CLOCK_DELAY_ON(TRIPLE_SPEED_ETHERNET_1_BASE);
		D(1, BUG("Set Tx Delay ON ---> GTX_CLOCK_DELAY_ON\n"));
		}
	else{
		SET_PHY_GTX_CLOCK_DELAY_BYPASSED(TRIPLE_SPEED_ETHERNET_1_BASE);
		D(1, BUG("Set Tx Delay OFF ---> GTX_CLOCK_DELAY_BYPASSED\n"));
		}
	if(PHYB_RGMII_RX_TIMING_CTRL_ON)
		configure_phy_rx_delay_timing(TRIPLE_SPEED_ETHERNET_1_BASE);
	else
		configure_phy_rx_aligned_timing(TRIPLE_SPEED_ETHERNET_1_BASE);
	//==================


	// Enable PHYloopback
	temp_reg_value = IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR);
	if (PHYB_LOOPBACK == 1){
		temp_reg_value = temp_reg_value | 0x4000;
		D(1, BUG("<><><> Enable PHY Loopback <><><>\n"));
		}
	else{	// cleared way at the top when MII CONTROL MASKED for 1st write
		D(1, BUG("Disable PHY Loopback\n"));
		}
	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR, temp_reg_value);
	D(1, BUG("MII_CONTROL_ADDR reg = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_CONTROL_ADDR)));
	D(1, BUG("PHY read Status Register = 0x%04X\n",IORD_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_MII_STATUS_ADDR)));
	D(1, BUG("PHYB_CONFIG (copper) = 0x%04X\n",PHYB_CONFIG));

    // DEBUG READ to see PHY CLOCK settings
	SETREAD_18_AUXCNTL_MISC_CNTL_REG(TRIPLE_SPEED_ETHERNET_1_BASE);
	temp_reg_value = READ_18_SHADOW(TRIPLE_SPEED_ETHERNET_1_BASE);
	D(1, BUG("RGMII TIMING SETTING: Rx timing Setting 18:MISC_CNTL_REG = 0x%04X\n (0x0100=delayed,0=aligned)\n",temp_reg_value));

	SETREAD_1C_SHADOW_REG(TRIPLE_SPEED_ETHERNET_1_BASE,CLOCK_ALIGN_MASK);
	temp_reg_value = READ_1C_SHADOW(TRIPLE_SPEED_ETHERNET_1_BASE);
	D(1, BUG("RGMII TIMING SETTING: GTX timing Setting 1C:CLOCK_ALIGN_REG = 0x%04X\n (0x0200=delayed,0=bypassed)\n",temp_reg_value));

}



//==============================================================================
//	TSE 0 and 1 MAC Configuration
//==============================================================================
void configure_tse_mac(unsigned int base)
{
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){
		D(1, BUG("\n===========================================================\n\tTSE-0 MAC Configuration Settings\n===========================================================\n"));
		}
	else{
		D(1, BUG("\n===========================================================\n\tTSE-1 MAC Configuration Settings\n===========================================================\n"));
		}

	unsigned long TSE_MAC_Rev  = IORD_ALTERA_TSEMAC_REV(base);
	D(1, BUG("TSE MAC Rev: \t 0x%04lX\n",TSE_MAC_Rev));

	if(base == TRIPLE_SPEED_ETHERNET_0_BASE){
		D(1, BUG("MACA_COMMAND_CONFIG = 0x%08X\n",MACA_COMMAND_CONFIG));
		if(ETH_SPEED_1G){
			D(1, BUG("\n*** MACA set to 1G!\n"));
			}
		else{
			if(ENA_10){
				D(1, BUG("\n*** MACA set to 10M\n"));
				}
			else{
				D(1, BUG("\n*** MACA set to 100M\n"));
				}
			}

		if(LOOP_ENA){
			D(1, BUG("\n<><><> MACA LOOPBACK ENABLED <><><>\n"));
			}
		}
	else{
		D(1, BUG("MACB_COMMAND_CONFIG = 0x%08X\n",MACB_COMMAND_CONFIG));
		if(ETH_SPEED_1GB){
			D(1, BUG("\n*** MACB set to 1G!\n"));
			}
		else{
			if(ENA_10B){
				D(1, BUG("\n*** MACB set to 10M\n"));\
				}
			else{
				D(1, BUG("\n*** MACB set to 100M\n"));
				}
			}

		if(LOOP_ENAB){
			D(1, BUG("\n<><><> MACB LOOPBACK ENABLED <><><>\n"));
			}
		}

	IOWR_ALTERA_TSEMAC_SCRATCH(base,MAC_SCRATCH);	// TSE MAC write Scratch  = MAC_SCRATCH");
	unsigned long TSE_MAC_Scratch = IORD_ALTERA_TSEMAC_SCRATCH(base);	// TSE MAC read Scratch
	D(1, BUG("TSE_MAC_Scratch: \t 0x%08lX\n",TSE_MAC_Scratch));

	/*******
	//(TRIPLE_SPEED_ETHERNET_0_BASE,[expr (COMMAND_CONFIG_VALUE | (1 << 13))]);
	//mbp IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_0_BASE,(COMMAND_CONFIG_VALUE | ALTERA_TSEMAC_CMD_SW_RESET_MSK));
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,(MACA_COMMAND_CONFIG | ALTERA_TSEMAC_CMD_SW_RESET_MSK));
	else
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,(MACB_COMMAND_CONFIG | ALTERA_TSEMAC_CMD_SW_RESET_MSK));
	unsigned long Command_Config = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
	D(1, BUG("Command_Config+reset:  0x%08lX\n",Command_Config));

	//OSTimeDlyHMSM(0, 0, 0, 2);//======DEBUG===========================================================
	wait_for_tseMacReset(base);

	//mbp IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_0_BASE,COMMAND_CONFIG_VALUE);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,MACA_COMMAND_CONFIG);
	else
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,MACB_COMMAND_CONFIG);
	Command_Config = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
	D(1, BUG("Command_Config: \t 0x%08lX\n",Command_Config));
	*****/
	tseMacWriteCommandConfigAndReset(base);

	unsigned long Command_Config = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
	if(Command_Config&ALTERA_TSEMAC_CMD_PROMIS_EN_MSK){
		D(1, BUG("\tPromiscuous Mode is ON\n"));
		}
	else{
		D(1, BUG("\tPromiscuous Mode is OFF\n"));
		}
	if(Command_Config&ALTERA_TSEMAC_CMD_PAUSE_IGNORE_MSK){
		D(1, BUG("\tPause Ignore is ON (No Flow Control)\n"));
		}
	else{
		D(1, BUG("\tPause Ignore is OFF (Flow Control is ON)\n"));
		}

	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_MAC_0(base,MAC_0);	// not swapped
	else
		IOWR_ALTERA_TSEMAC_MAC_0(base,MAC_0B);	// not swapped
	unsigned long MAC_Address_0  = IORD_ALTERA_TSEMAC_MAC_0(base);
	D(1, BUG("MAC_Address_0: \t 0x%08lX\n",MAC_Address_0));

	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_MAC_1(base,MAC_1);	// not swapped
	else
		IOWR_ALTERA_TSEMAC_MAC_1(base,MAC_1B);	// not swapped
	unsigned long MAC_Address_1  = IORD_ALTERA_TSEMAC_MAC_1(base);
	D(1, BUG("MAC_Address_1: \t 0x%08lX\n",MAC_Address_1));

	IOWR_ALTERA_TSEMAC_FRM_LENGTH(base, MAC_FRM_LENGTH);
	unsigned long Frame_Length  = IORD_ALTERA_TSEMAC_FRM_LENGTH(base);
	D(1, BUG("Frame_Length: \t %0lu\n",Frame_Length));

	IOWR_ALTERA_TSEMAC_PAUSE_QUANT(base,PAUSE_QUANT);
	unsigned long Pause_Quanta  = IORD_ALTERA_TSEMAC_PAUSE_QUANT(base);
	D(1, BUG("Pause_Quanta: \t %0lu\n",Pause_Quanta));

	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_RX_SECTION_EMPTY(base, RX_SECTION_EMPTY);
	else
		IOWR_ALTERA_TSEMAC_RX_SECTION_EMPTY(base, RX_SECTION_EMPTYB);
	unsigned long RX_Section_Empty  = IORD_ALTERA_TSEMAC_RX_SECTION_EMPTY(base);
	D(1, BUG("RX_Section_Empty: \t %0lu\n",RX_Section_Empty));

	unsigned long RX_Section_Full;
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
//		RX_Section_Full = RX_SECTION_FULL_CUT; // cut-through to maintain generator/monitor functionality
//NEW DEBUG
		RX_Section_Full = RX_SECTION_FULL_SAF; // needed for LAN>WAN speeds in LAN/WAN mode (nah) - but only supports <9% due to TSE FIFO - we think.
	else
		//RX_Section_Full = RX_SECTION_FULLB_CUT;//larger FIFO or cut-through REQUIRED for Jumbo frames
		RX_Section_Full = RX_SECTION_FULLB_SAF;// store & forward to support LAN-WAN mode as well as loopback function!

	IOWR_ALTERA_TSEMAC_RX_SECTION_FULL(base, RX_Section_Full);
	RX_Section_Full  = IORD_ALTERA_TSEMAC_RX_SECTION_FULL(base);
	D(1, BUG("RX_Section_Full: \t %0lu\n",RX_Section_Full));

	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_TX_SECTION_EMPTY(base, TX_SECTION_EMPTY);
	else
		IOWR_ALTERA_TSEMAC_TX_SECTION_EMPTY(base, TX_SECTION_EMPTYB);
	unsigned long TX_Section_Empty  = IORD_ALTERA_TSEMAC_TX_SECTION_EMPTY(base);
	D(1, BUG("TX_Section_Empty: \t %0lu\n",TX_Section_Empty));

	IOWR_ALTERA_TSEMAC_TX_SECTION_FULL(base, TX_SECTION_FULL);
	unsigned long TX_Section_Full  = IORD_ALTERA_TSEMAC_TX_SECTION_FULL(base);
	D(1, BUG("TX_Section_Full: \t %0lu\n",TX_Section_Full));

	IOWR_ALTERA_TSEMAC_RX_ALMOST_EMPTY(base, RX_ALMOST_EMPTY);
	unsigned long RX_Almost_Empty  = IORD_ALTERA_TSEMAC_RX_ALMOST_EMPTY(base);
	D(1, BUG("RX_Almost_Empty: \t %0lu\n",RX_Almost_Empty));

	IOWR_ALTERA_TSEMAC_RX_ALMOST_FULL(base, RX_ALMOST_FULL);
	unsigned long RX_Almost_Full  = IORD_ALTERA_TSEMAC_RX_ALMOST_FULL(base);
	D(1, BUG("RX_Almost_Full: \t %0lu\n",RX_Almost_Full));

	IOWR_ALTERA_TSEMAC_TX_ALMOST_EMPTY(base, TX_ALMOST_EMPTY);
	unsigned long TX_Almost_Empty  = IORD_ALTERA_TSEMAC_TX_ALMOST_EMPTY(base);
	D(1, BUG("TX_Almost_Empty: \t %0lu\n",TX_Almost_Empty));

	IOWR_ALTERA_TSEMAC_TX_ALMOST_FULL(base, TX_ALMOST_FULL);
	unsigned long TX_Almost_Full = IORD_ALTERA_TSEMAC_TX_ALMOST_FULL(base);
	D(1, BUG("TX_Almost_Full: \t %0lu\n",TX_Almost_Full));

	IOWR_ALTERA_TSEMAC_MDIO_ADDR0(base, MDIO_ADDR0);
	unsigned long MDIO_Address_0 = IORD_ALTERA_TSEMAC_MDIO_ADDR0(base);
	D(1, BUG("MDIO_Address_0: \t 0x%08lX\n",MDIO_Address_0));

	IOWR_ALTERA_TSEMAC_MDIO_ADDR1(base, MDIO_ADDR1);
	unsigned long MDIO_Address_1 = IORD_ALTERA_TSEMAC_MDIO_ADDR1(base);
	D(1, BUG("MDIO_Address_1: \t 0x%08lX\n",MDIO_Address_1));

	unsigned long Register_Status = IORD_ALTERA_TSEMAC_REG_STAT(base);
	D(1, BUG("MAC Register_Status:  0x%02lX\n",Register_Status));

	IOWR_ALTERA_TSEMAC_TX_IPG_LENGTH(base, TX_IPG_LENGTH);
	unsigned long TX_IPG_Length = IORD_ALTERA_TSEMAC_TX_IPG_LENGTH(base);
	D(1, BUG("TX_IPG_Length: \t %0lu\n",TX_IPG_Length));

	IOWR_ALTERA_TSEMAC_TX_CMD_STAT(base, TX_CMD_STAT_VALUE);
	unsigned long TX_Command_Status = IORD_ALTERA_TSEMAC_TX_CMD_STAT(base);
	D(1, BUG("TX_Command_Status: \t 0x%02lX\n",TX_Command_Status));

	IOWR_ALTERA_TSEMAC_RX_CMD_STAT(base, RX_CMD_STAT_VALUE);
	unsigned long RX_Command_Status  = IORD_ALTERA_TSEMAC_RX_CMD_STAT(base);
	D(1, BUG("RX_Command_Status: \t %0lX\n",RX_Command_Status));
}


//===================================================================================
//void restart_auto_negotiation(unsigned int base_address, unsigned int tech_ena)
//{
/////////// WHEN & WHY ?
//}


//================================================================
void tseMacWriteCommandConfigAndReset(unsigned int base)
{
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,(MACA_COMMAND_CONFIG | ALTERA_TSEMAC_CMD_SW_RESET_MSK));
	else
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,(MACB_COMMAND_CONFIG | ALTERA_TSEMAC_CMD_SW_RESET_MSK));
	unsigned long Command_Config = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
	D(1, BUG("Command_Config+reset:  0x%08lX\n",Command_Config));

	//OSTimeDlyHMSM(0, 0, 0, 2);//======DEBUG===========================================================
	wait_for_tseMacReset(base);

	//mbp IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_0_BASE,COMMAND_CONFIG_VALUE);
	if(base == TRIPLE_SPEED_ETHERNET_0_BASE)
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,MACA_COMMAND_CONFIG);
	else
		IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,MACB_COMMAND_CONFIG);
}


//================================================================
void wait_for_tseMacReset(unsigned int base)
{
    // ===================================================
    // Wait for software reset of TSE core to complete
    // ===================================================
    int status = 0x2000;
    while (status & 0x2000)
        status = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
}
//================================================================
int tseMacReset(unsigned int base)
{
    int status = 0;
    // ===================================================
    // Perform software reset the TSE core
    // ===================================================
    IOWR_ALTERA_TSEMAC_CMD_CONFIG(base,0x00002000);
    status = 0x2000;
    while (status & 0x2000)
        status = IORD_ALTERA_TSEMAC_CMD_CONFIG(base);
    return 0;
}

/**
void AvalonLoopbackConfig (unsigned int base_address, unsigned int loopback_mode, unsigned int ext_loopback_mode, unsigned int port_1_loopback_mode)
{
	int mac_rx_to_tx_loopback_ena = 0;

	if ((loopback_mode == 0) || ((loopback_mode == 3) && (ext_loopback_mode == 2) && (port_1_loopback_mode == 0))) {
		mac_rx_to_tx_loopback_ena = 3;					//enable avalon loopback and src & dst address swap
		//mac_rx_to_tx_loopback_ena = 1;				//enable avalon loopback and src & disable address swap
	}

	IOWR_16DIRECT(base_address, AVALON_ST_LOOPBACK_ENA, mac_rx_to_tx_loopback_ena);

	return;
}
***/

/*** for future use.........
int tseGenMonConfigure(struct gTestStruct * test)
{

 unsigned int data=0;
 // Setup Monitor -
   // Most of the status bits are reset to 0 when you toggle
   //  the start bit.
   //
 // comment: Hock Siang
  IOWR_32DIRECT(EMON0_BASE, EMON_NUM_PACKET, test->number_packets);

  DD(1, BUG("EMON0- Receiving Packets = %u\n", test->number_packets);
  // Setup Generator
  IOWR_32DIRECT(EGEN0_BASE, EGEN_NUM_PACKET, test->number_packets);
  DD(1, BUG("EGEN0- Sending Packets = %u\n", test->number_packets);


  // If random packet lengths are selected
  // then don't even set a packet length... it's bad form
  //
  if(test->random_length == TRUE)
    data = 0x0001;
  else
    data = (test->packet_length << 1) & 0x7FFE;

  // Random data pattern
  if(test->packet_data == RANDOM)
    data |= 0x8000;

  IOWR_32DIRECT(EGEN0_BASE, EGEN_CONF_SETTING, data);
  DD(1, BUG("EGEN0- Configuration Setting = 0x%x\n", data);

  // Write the seed registers later...
  IOWR_32DIRECT(EGEN0_BASE, EGEN_RAND_SEED1, test->seed_value_high);
  DD(1, BUG("EGEN0- Seed Value High = 0x%x\n", test->seed_value_high);
  IOWR_32DIRECT(EGEN0_BASE, EGEN_RAND_SEED0, test->seed_value_low);
  DD(1, BUG("EGEN0- Seed Value Low = 0x%x\n", test->seed_value_low);

  // Need to deal withthe src and dest registers later...
  IOWR_32DIRECT(EGEN0_BASE, EGEN_SRC_ADDR1, test->macaddra_high);
  DD(1, BUG("EGEN0- MAC SRC ADDR High = 0x%x\n", test->macaddra_high);
  IOWR_32DIRECT(EGEN0_BASE, EGEN_SRC_ADDR0, test->macaddra_low);
  DD(1, BUG("EGEN0- MAC SRC ADDR Low = 0x%x\n", test->macaddra_low);
  IOWR_32DIRECT(EGEN0_BASE, EGEN_DEST_ADDR1, test->macaddrb_high);
  DD(1, BUG("EGEN0- MAC SRC ADDR High = 0x%x\n", 0x0000FFFF);
  IOWR_32DIRECT(EGEN0_BASE, EGEN_DEST_ADDR0, test->macaddrb_low);
  DD(1, BUG("EGEN0- MAC DST ADDR Low = 0x%x\n", test->macaddrb_low);

  // Start Operation
  IOWR_32DIRECT(EMON0_BASE, EMON_RX_CTRL_STAT, 0x00000001);
  DD(1, BUG("EMON0- RX CTL STAT = 0x%x\n", 0x1);
  usleep(100000);
  IOWR_32DIRECT(EGEN0_BASE, EGEN_OPERATION, 0x00000001);
  DD(1, BUG("EGEN0- OPERATION = 0x%x\n", 0x1);

  test->test_control = ACTIVE;
  test->error_code = SUCCESS;

  return TRUE;
}
********/


/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
