#ifndef HARDWARE_DEF_H_
#define HARDWARE_DEF_H_

//#include "gigle.h"
//#include "alt_types.h"

//====================================================================
//	EPCS - FLASH device Definitions & Functions
//====================================================================
/***
 	 	 	int last_sector = regions->offset+(regions->number_of_blocks)-1;
			int phy_sector = regions->offset+(regions->number_of_blocks)-2;
			int user1_sector = regions->offset+(regions->number_of_blocks)-3;
			int user2_sector = regions->offset+(regions->number_of_blocks)-4;
 ***/
#define FLASH_PARAMETERS_BLOCK	0
#define FLASH_PHY_BLOCK				1
#define FLASH_USER1_BLOCK			2
#define FLASH_USER2_BLOCK			3
#define FLASH_SOF_ELF_BLOCKS		4	//erase from 0 through user2 block


//====================================================================
//	PIO Definitions & Functions
//====================================================================
// PIO_2 utilization................................
//	rev1 PIO_2_BASE  Bit.43=PortB, Bit.65=PortA, Bit.0 SFP_A_SCL, Bit.1=SFP_B_SCL output
// 				00=10, 01=100, 10=1G, 11=Vcc
#define PORTA_SFP_A_SCL_MASK	0x0001	// bit.0
#define PORTB_SFP_B_SCL_MASK	0x0002	// bit.1

#define	ETH10G_PHY_CLK_MASK		0x0004	// bit.2  Actv Low (enable 10G PHY CLK)

#define	PORTB_CLK_10M	0x00	// bit.34
#define	PORTB_CLK_100M	0x08
#define	PORTB_CLK_1G		0x10
#define	PORTB_CLK_VCC	0x18
#define	PORTA_CLK_10M	0x00	// bit.56
#define	PORTA_CLK_100M	0x20
#define	PORTA_CLK_1G		0x40
#define	PORTA_CLK_VCC	0x60
#define PORTA_CLK_MASK	0xFF9F	// mask PortA CLK bits to 0
#define PORTB_CLK_MASK	0xFFE7	// mask PortB CLK bits to 0
/*** currently unused....retained to facilitate future usage
#define unused_PIO2_bit7_mask 			0x0080	// bit.7
#define unused_PIO2_bit10_mask 			0x0400	// bit.10
#define unused_PIO2_bit11_mask 			0x0800	// bit.11
#define ETH_10G_TX_REF_CLK_RESET_MASK 	0x2000  // bit.13 - ActvHIGH (asserted!)
#define unused_PIO2_bit14_mask 			0x4000	// bit.14
#define unused_PIO2_bit15_mask 			0x8000	// bit.15
****/
#define PORTA_1G_LED_MASK		0x0100	// bit.8
#define PORTB_1G_LED_MASK		0x0200  // bit.9
#define ETH10G_PHY_RESET_MASK	0x1000  // bit.12 - ActvLow for 500uS after PowerGood

#define ROUTE_TRAFFIC_TO_GUI_mask 		0x4000	// bit.14
	#define FPLLOUT_CLOCK 				0x0001	// bit.14  0=FPLLOUT 156.25M clock (10G test modes only)
	#define INT156_CLOCK 					0x0000	// bit.14: 1=internal 156.25M clock (TSE test modes)


#define PORTA_SFP_A_SCL_MASKLOW	0xFFFE	// mask PortA SFP_A_SCL bit to 0
#define PORTB_SFP_B_SCL_MASKLOW	0xFFFD	// mask PortB SFP_B_SCL bit to 0
#define PORTA_SFP_A_SCL_HIGH		0x0001	// set A CLK HIGH
#define PORTB_SFP_B_SCL_HIGH		0x0002	// set B CLK HIGH
#define PORTA_SFP_A_SDA_MASKLOW	0xFFFE	// mask PortA SFP_A_SDA bit to 0
#define PORTB_SFP_B_SDA_MASKLOW	0xFFFD	// mask PortB SFP_B_SDA bit to 0
#define PORTA_SFP_A_SDA_HIGH		0x0001	// SET A DATA HIGH
#define PORTB_SFP_B_SDA_HIGH		0x0002	// SET B DATA HIGH

//not used anymore - perhaps in the future
//#define ALTERA_AVALON_PIO_DIRECTION_A_INPUT_MASK  0xFE
//#define ALTERA_AVALON_PIO_DIRECTION_A_OUTPUT 	  	0x01
//#define ALTERA_AVALON_PIO_DIRECTION_B_INPUT_MASK  0xFD
//#define ALTERA_AVALON_PIO_DIRECTION_B_OUTPUT 	  	0x02
//====================================================================
// PIO_3 utilization................................
// 0,1,2,3=FPGA debug pins, ^4:err4, DC_FIFO6, ^5:DC_FIFO1, SCFIFO0, ERR5, ^6:DC_FIFO5, ^7:TxMAC
//firmware should add reset sequence to various elements whenever the 10GE clock_mux changes state.
//Use PIO3_Content,   PIO_3IO_BASE
//
//	0) read content of register
//	1) set the appropriate bit (OR with mask)
//	2) wait a very short time (ie 100nS is fine) ...just use usleep(1)  1 uS.
//	3) clear bit by writing it back (AND with mask)
//Upon reset, section MUST be re-initialized. Elements listed below use name per block diagram.
//
#define	RESET_C3794_mask				0x0002	// bit.1  set high on Boot.
//Bit2 resets : GEN
//Bit3 resets : MON
//Bit4 resets : err4, dc_fifo6
//Bit5 resets : dc_fifo1, sc_fifo0, err5
//Bit6 resets : dc_fifo5
//Bit7 resets : 10G MAC
#define	RESET_GEN_mask					0x0004	// bit.2
#define RESET_MON_mask					0x0008	// bit.3
#define RESET_ERR4_DCFIFO6_PIO3_mask 	0x0010	// bit.4
#define RESET_DCFIFO1_SCFIFO0_PIO3_mask 0x0020	// bit.5
#define RESET_DCFIFO5_PIO3_mask 		0x0040	// bit.6
#define RESET_10GMAC_FIFOS_PIO3_mask 	0x0080	// bit.7
//Note: Contains the additional 10GE MAC reset (bit7). It resets the 10GE fifos at the same time.
//Watch for XAUI training and potential problem with MAC<->PHY handshake.


//For example: IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, value |= 0x10);
//	Would assert reset to ERR4 and DC_FIFO6 which should be the primary focus.
//	Other elements may not be necessary for now but we run the risk of breaking anything that touches this new clock down the road.  Note that bit7, egress MAC, does not exist at the moment as I am told MAC has the ability to register section of its datapath by software (to be confirmed).

//	    64-bit msb stat registers
//		IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK 		0x3C*4=0xF0 msb_aOctetsTransmittedOK
//		IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK		0x3D*4=0xF4 msb_aOctetsReceivedOK
//		IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS	0x3E*4=0xF8 msb_etherStatsOctets
//====================================================================
#define IORD_ALTERA_TSEMAC_A_OCTETS_TX_OK_MSB(base)      IORD_32DIRECT(base, 0xF0)
#define IORD_ALTERA_TSEMAC_A_OCTETS_RX_OK_MSB(base)      IORD_32DIRECT(base, 0xF4)
#define IORD_ALTERA_TSEMAC_ETHER_STATS_OCTETS_MSB(base)  IORD_32DIRECT(base, 0xF8)
//===============================================================================
// MUX & Splitter devices 3-input:1-output and MUX disable (only on 3:1 mux)
// LANWAN mode: PING & Browse.
//===============================================================================
#define	TSE_A_TX_3MUX		MUX31_ST_1GE_A_BASE
#define	TSE_B_TX_3MUX		MUX31_ST_1GE_B_BASE
#define	A10G_TX_3MUX		MUX31_ST_10GE_BASE
#define	TSE_A_RX_2SPLITTER	GUISYS_AVALON_ST_SPLITTER_1_BASE
#define	TSE_B_RX_3SPLITTER	GUISYS_AVALON_ST_SPLITTER_2_BASE
#define	A10G_RX_3SPLITTER	GUISYS_AVALON_ST_SPLITTER_0_BASE
#define TSE_B_TX_SWAPPER	TSE_ADDR_SWAPPER_B_BASE
#define TSE_A_TX_SWAPPER	TSE_ADDR_SWAPPER_A_BASE
//===============================================================================
#define ST_MUX_REG				0x0000
#define ST_SPLITTER_REG			0x0000
#define TSE_SWAPPER_REG			0x0000
//===============================================================================
#define MISMATCH_DETECTOR_10G 		0x10
#define TSE_A_MISMATCH_DETECTOR 	0x10
#define TSE_B_MISMATCH_DETECTOR 	0x10
// bit definitions
#define SET_PORT_0				0
#define SET_PORT_1				1
#define SET_PORT_2				2
#define SET_PORT_3				3	// disables output on 3 input muxes since there's no 4th input
#define SET_SWAP_OFF			0
#define SET_SWAP_ON				1
#define SET_TRAFFIC_TO_10G		IOWR_16DIRECT(ST_MUX10G_2_TO_1_ERROR6_0_BASE, ST_MUX_REG, SET_PORT_0);IOWR_16DIRECT(SPLIT12_ST_GEN_BASE, ST_SPLITTER_REG, SET_PORT_0)
#define SET_TRAFFIC_TO_TSE		IOWR_16DIRECT(ST_MUX10G_2_TO_1_ERROR6_0_BASE, ST_MUX_REG, SET_PORT_1);IOWR_16DIRECT(SPLIT12_ST_GEN_BASE, ST_SPLITTER_REG, SET_PORT_1)
//===============================================================================
//  IOWR_16DIRECT(ST_MUX_2_TO_1_0_BASE, ST_MUX_REG, 0x00);				// TSE-A-TX from: 0:EGEN,  1:B-RX
//  IOWR_16DIRECT(ST_MUX_3_TO_1_1_BASE, ST_MUX_REG, 0x00);				// TSE-B-TX from: 0:B-RX(B-lpbk), 1:A-Rx, 2:10G-Rx
//  IOWR_16DIRECT(TSE_ADDR_SWAPPER_0_BASE, TSE_SWAPPER_REG, 0x00);		// TSE-B-TX:      0:NO-SWAP, 1:SWAP  (swap's MAC for mux'd outgoing signal)
//  IOWR_16DIRECT(ST_MUX10G_3_TO_1_0_BASE, ST_MUX_REG, 0x00);			//   10G-TX from: 0:TGEN,  1:B-Rx,  2:10G-Rx (10G-lpbk)
//  IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_0_BASE, ST_SPLITTER_REG, 0x00);	//  10G-Rx to: 0:TMON, 1:B-Tx  -- Select which RDY to pass to 10G-Rx (rather than AND of all the output RDY-ins's)
//  IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_SPLITTER_REG, 0x00);	//TSE-A-RX to: 0:EMON, 1:B-Tx  -- Select which RDY to pass (rather than AND of all the output RDY-ins's)
//  IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_2_BASE, ST_SPLITTER_REG, 0x01);	//TSE-B-RX to: 0:A-Tx,1:B-Tx, 2:10G-Tx  -- Select which RDY to pass (rather than AND of all the output RDY-ins's)
//===============================================================================
// NOTE: 3:1 MUX can disable output by selecting invalid input! (ie 0x03 or 2'b11)
//===============================================================================
#define SET_TSE_A_TX_3MUX_TO_EGEN 			IOWR_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG, SET_PORT_2)
#define SET_TSE_A_TX_3MUX_TO_B_RX 			IOWR_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG, SET_PORT_1)
#define SET_TSE_A_TX_3MUX_TO_A_RX			IOWR_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG, SET_PORT_0)
#define	SET_TSE_A_TX_TO_NW_LOOPBACK			SET_TSE_A_TX_3MUX_TO_A_RX
#define SET_TSE_A_TX_3MUX_TO_NO_OUTPUT		IOWR_16DIRECT(TSE_A_TX_3MUX, ST_MUX_REG, SET_PORT_3)
#define SET_TSE_A_TX_3MUX_TO_TSE_LANWAN		SET_TSE_A_TX_3MUX_TO_B_RX

#define SET_TSE_B_TX_3MUX_TO_B_RX			IOWR_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG, SET_PORT_0)
#define SET_TSE_B_TX_TO_NW_LOOPBACK			SET_TSE_B_TX_3MUX_TO_B_RX
#define SET_TSE_B_TX_3MUX_TO_A_RX			IOWR_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG, SET_PORT_1)
#define SET_TSE_B_TX_TO_TSE_LANWAN			SET_TSE_B_TX_3MUX_TO_A_RX
#define SET_TSE_B_TX_3MUX_TO_10G_RX			IOWR_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG, SET_PORT_2)
#define SET_TSE_B_TX_TO_10G_LANWAN			SET_TSE_B_TX_3MUX_TO_10G_RX
#define SET_TSE_B_TX_3MUX_TO_NO_OUTPUT		IOWR_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG, SET_PORT_3)

#define	SET_10G_TX_3MUX_TO_TGEN				IOWR_16DIRECT(A10G_TX_3MUX, ST_MUX_REG, SET_PORT_0)
#define	SET_10G_TX_3MUX_TO_B_RX				IOWR_16DIRECT(A10G_TX_3MUX, ST_MUX_REG, SET_PORT_1)
#define	SET_10G_TX_TO_10G_LANWAN			SET_10G_TX_3MUX_TO_B_RX
#define	SET_10G_TX_3MUX_TO_10G_RX			IOWR_16DIRECT(A10G_TX_3MUX, ST_MUX_REG, SET_PORT_2)
#define	SET_10G_TX_TO_NW_LOOPBACK			SET_10G_TX_3MUX_TO_10G_RX	// doesn't work?  ...need's analysis
#define	SET_10G_TX_3MUX_TO_NO_OUTPUT		IOWR_16DIRECT(A10G_TX_3MUX, ST_MUX_REG, SET_PORT_3)

#define	SET_TSE_A_RX_2SPLITTER_TO_EMON		IOWR_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG, SET_PORT_0)
#define	SET_TSE_A_RX_2SPLITTER_TO_B_TX		IOWR_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG, SET_PORT_1)
#define	SET_TSE_A_RX_2SPLITTER_TO_A_TX		IOWR_16DIRECT(TSE_A_RX_2SPLITTER, ST_SPLITTER_REG, SET_PORT_2)
#define	SET_TSE_A_RX_TO_NW_LOOPBACK			SET_TSE_A_RX_2SPLITTER_TO_A_TX
#define	SET_TSE_A_RX_2SPLITTER_TO_TSE_LANWAN	SET_TSE_A_RX_2SPLITTER_TO_B_TX

#define	SET_TSE_B_RX_3SPLITTER_TO_A_TX		IOWR_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG, SET_PORT_0)
#define	SET_TSE_B_RX_TO_TSE_LANWAN			SET_TSE_B_RX_3SPLITTER_TO_A_TX
#define	SET_TSE_B_RX_3SPLITTER_TO_B_TX		IOWR_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG, SET_PORT_1)
#define	SET_TSE_B_RX_TO_NW_LOOPBACK			SET_TSE_B_RX_3SPLITTER_TO_B_TX
#define	SET_TSE_B_RX_3SPLITTER_TO_10G_TX	IOWR_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG, SET_PORT_2)
#define	SET_TSE_B_RX_TO_10G_LANWAN			SET_TSE_B_RX_3SPLITTER_TO_10G_TX

#define	SET_10G_RX_3SPLITTER_TO_TMON		IOWR_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG, SET_PORT_0)
#define	SET_10G_RX_3SPLITTER_TO_B_TX		IOWR_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG, SET_PORT_1)
#define	SET_10G_RX_TO_10G_LANWAN			SET_10G_RX_3SPLITTER_TO_B_TX
#define	SET_10G_RX_3SPLITTER_TO_10G_TX		IOWR_16DIRECT(A10G_RX_3SPLITTER, ST_SPLITTER_REG, SET_PORT_2)
#define	SET_10G_RX_TO_NW_LOOPBACK			SET_10G_RX_3SPLITTER_TO_10G_TX	// doesn't work?  ...need's analysis
//===============================================================================
#define SET_TSE_A_NORMAL_TEST_MODE			SET_TSE_A_TX_3MUX_TO_EGEN;SET_TSE_A_RX_2SPLITTER_TO_EMON
#define SET_TSE_A_LOOPBACK					SET_TSE_A_TX_TO_NW_LOOPBACK;SET_TSE_A_RX_TO_NW_LOOPBACK
#define SET_TSE_B_LOOPBACK					SET_TSE_B_TX_TO_NW_LOOPBACK;SET_TSE_B_RX_TO_NW_LOOPBACK
#define SET_TSE_B_MAC_ADDR_SWAP(data)		IOWR_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG, data) //SET_SWAP_ON/SET_SWAP_OFF
#define SET_TSE_A_MAC_ADDR_SWAP(data)		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG, data) //SET_SWAP_ON/SET_SWAP_OFF
#define SET_10GA_MAC_ADDR_SWAP(data)		IOWR_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG, data) //SET_SWAP_ON/SET_SWAP_OFF

#define SWAPPER64_10G_TXFIFO_FULL_CNT		0x24	// count of back pressure int osystem from TxFIFO (toward the PHY and out into the NW)
#define SET_10G_NORMAL_TEST_MODE			SET_10G_TX_3MUX_TO_TGEN;SET_10G_RX_3SPLITTER_TO_TMON
#define	SET_10G_LOOPBACK					SET_10G_TX_TO_NW_LOOPBACK;SET_10G_RX_TO_NW_LOOPBACK	// doesn't work?  ...need's analysis

#define SET_TSE_LANWAN_MODE					SET_10G_NORMAL_TEST_MODE;SET_TSE_A_TX_3MUX_TO_TSE_LANWAN;SET_TSE_A_RX_2SPLITTER_TO_TSE_LANWAN;SET_TSE_B_TX_TO_TSE_LANWAN;SET_TSE_B_RX_TO_TSE_LANWAN
#define SET_10G_LANWAN_MODE					SET_TSE_A_NORMAL_TEST_MODE;SET_TSE_B_TX_TO_10G_LANWAN;SET_TSE_B_RX_TO_10G_LANWAN;SET_10G_TX_TO_10G_LANWAN;SET_10G_RX_TO_10G_LANWAN
//=======================================================================================================
#define SET_10GLOOPPATH_CLOCKMUX_TO_INT156		IOWR_8DIRECT(CLK_MUXER_10GE_LOOP_BASE, 0X0, 0);
#define SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL		IOWR_8DIRECT(CLK_MUXER_10GE_LOOP_BASE, 0X0, 1);
#define SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD		IOWR_8DIRECT(CLK_MUXER_10GE_LOOP_BASE, 0X0, 2);
//====================================================================
//====================================================================
// Broadcomm (Dual) PHY Register Address Offset
// Port 0
#define MDIO_ADDR0_SPACE					0x80	// dw:0x80-0x9F  byte:0x200-0x27C
#define MDIO_ADDR1_SPACE					0xA0	// dw:0xA0-0xBF	 byte:0x280-0x2FC

#define MDIO_0	0	// TSE0 talks to PHYA through MDIO_0, and TSE1 to PHYB through it's MDIO_0
#define MDIO_1	1	// ...so this is unused.

// Broadcom BCM5482 PHY-A & PHY-B Register definitions - A/B selected by TSE0 or TSE1 (MDIO_0)
// TODO
#define PHY_MII_CONTROL_ADDR			0x00	//C/F PHY_CONTROL_ADDRESS
		#define	MII_CONTROL_RESET				0x8000	//15 self-clearing
		#define	MII_CONTROL_INT_LPBK			0x4000
		#define	MII_CONTROL_SPEEDBIT0			0x2000	//13 bit 6,13: 00=10,01=100,10=1G,11=rsrvd
		#define	MII_CONTROL_ENABLE_ANEG		0x1000
		#define	MII_CONTROL_POWER_DOWN		0x0800
		#define	MII_CONTROL_RESTART_ANEG		0x0200	//9 self-clearing
		#define	MII_CONTROL_FULL_DUPLEX		0x0100
		#define	MII_CONTROL_SPEEDBIT1			0x0040	//6 bit 6,13: 00=10,01=100,10=1G,11=rsrvd
#define PHY_MII_STATUS_ADDR				0x01	//C/F (Copper & Fiber versions of these registers)
#define PHY_IDENTIFIER_MSB				0x02
#define PHY_IDENTIFIER_LSB				0x03
#define PHY_AUTO_NEG_ADV				0x04	//C/F PHY_AUTO_NEG_ADDRESS
#define PHY_AUTO_NEG_LPA				0x05	//C/F
#define PHY_AUTO_NEG_EXP				0x06	//C/F
#define PHY_NEXT_PAGE_TX				0x07
#define PHY_LINK_PARTNER_RX_NEXT_PAGE	0x08
#define PHY_1000_CONTROL				0x09	// PHY_1000BASE_T_CONTROL_ADDRESS
#define PHY_1000_STATUS					0x0A	// PHY_1000BASE_T_STATUS_ADDRESS
#define PHY_IEEE_EXT_STATUS				0x0F	//C/F

#define PHY_PHY_EXT_CONTROL				0x10
#define PHY_PHY_EXT_STATUS				0x11
#define MDI_CROSSOVER_STATE_MASK		0x2000	//bit^13=MDI crossover state
#define PHY_RCV_ERR_COUNTER				0x12
#define PHY_FALSE_CARRIER_SENSE_COUNTER	0x13
#define PHY_RCVR_NOT_OK_COUNTER			0x14	// count is clipped to 0xFF!

#define	PHY_EXPANSION_REG_ACCESS		0x15	// R/W for all register below!  0x17 is for selection & 0x15 is for R/W
#define PHY_EXPANSION_REG_SELECT		0x17	//pg73 write EXPANSION_REG SELECTOR and then R/W via ACCESS REG (0x15)
		#define	EXP_DESELECT_MASK					0x0000	// write to 0x17 to deSelect Expansion register
		#define	EXP_REGISTER_MASK					0x0F00
			#define EXP_REG_RXTX_CNTR_MASK					0x0000	// Rx/Tx Packet Cntr - COPPER ONLY - Rx/Tx set by bit-11 of reg 0x18
			#define EXP_REG_INTR_STAT_MASK						0x0001
			#define EXP_REG_LED_SEL_MASK						0x0004
			#define EXP_REG_LED_FLASH_MASK					0x0005
			#define EXP_REG_LED_BLINK_MASK						0x0006
			#define EXP_REG_OPERATING_MODE_STATUS_MASK		0x0042
				#define OPERATING_MODE_FIBER_LINK_UP_MASK		0x8000	// 1=link up (in Fiber, SGMII or GBIC modes)
					#define OPERATING_MODE_FIBER_SPEED_MASK		0x6000	// 00=10M, 01=100M, 10=1G  (SerDes speeds)
						#define OPERATING_MODE_FIBER_SPEED_10M_ANEG	0x0000	// 00
						#define OPERATING_MODE_FIBER_SPEED_100M			0x2000	// 01
						#define OPERATING_MODE_FIBER_SPEED_1G			0x4000	// 10
					#define OPERATING_MODE_FIBER_DUPLEX_MASK	0x1000	// 1=Full, 0=half (0= still in auto-neg)
					#define OPERATING_MODE_COPPER_LINK_UP_MASK	0x0800	// 1=link up (on copper port)
					#define OPERATING_MODE_COPPER_SPEED_MASK	0x0600	// 00=10M, 01=100M, 10=1G (00 when still in auto-neg)
						#define OPERATING_MODE_COPPER_SPEED_10M_ANEG	0x0000	// 00
						#define OPERATING_MODE_COPPER_SPEED_100M		0x0200	// 01
						#define OPERATING_MODE_COPPER_SPEED_1G			0x0400	// 10
					#define OPERATING_MODE_COPPER_DUPLEX_MASK	0x0100	// 1=Full, 0=half	(0= still in auto-neg)
					#define OPERATING_MODE_COPPER_ENERGY_DET	0x0080	// 1=Energy detected on Copper port
					#define OPERATING_MODE_FIBER_ENERGY_DET		0x0040	// 1=Energy detected on Fiber signal Detect Pin
					#define OPERATING_MODE_SYNC_STATUS_VALID	0x0020	// 1=Valid SerDes PCS Rcv SYNC
					#define OPERATING_MODE_STATUS_MASK			0x001F	//
						#define OPERATING_MODE_RGMII_COPPER			0x0006	// 0x0006 (00110) RGMII to Copper
						#define OPERATING_MODE_RGMII_100FX			0x000D	// 0x000D (01101) RGMII to 100-Fiber
						#define OPERATING_MODE_RGMII_SGMII			0x000E	// 0x000E (01110) RGMII to SGMII
						#define OPERATING_MODE_RGMII_SERDES			0x000F	// 0x000F (01101) RGMII to SerDes
						#define OPERATING_MODE_SGMII_SERDES			0x0012	// 0x0012 (10010) SGMII to SerDes
						#define OPERATING_MODE_SGMII_100FX			0x0013	// 0x0013 (10011) SGMII to 100-Fiber
						#define OPERATING_MODE_SGMII_COPPER			0x0014	// 0x0014 (10100) SGMII to Copper
						#define OPERATING_MODE_SERDES_SERDES			0x0016	// 0x0016 (10110) SerDes to SerDes (media converter)
						#define OPERATING_MODE_SERDES_COPPER		0x0017	// 0x0017 (10111) SerDes to Copper (media converter)
				#define EXP_REG_SELECT_RECOVERED_CLK_MASK	0x0044
				#define EXP_REG_SERDES_RX_CONTROL_MASK		0x0050
				#define EXP_REG_SERDES_CONTROL_MASK			0x0052

	#define	SECSER_REGISTER_MASK				0x0E00
		#define	SECSER_1GX_CNTL_MASK				0x0000
		#define	SECSER_ANEG_ADV_MASK				0x0004
		#define	SECSER_1GX_ANLPA_MASK				0x0005
		#define	SECSER_1GX_ANSECSER_MASK			0x0006
		#define	SECSER_1GX_ANNPTX_MASK				0x0007
		#define	SECSER_1GX_ANLPNP_MASK				0x0008
		#define	SECSER_1GX_EXTSTATUS_MASK			0x000F
		#define	SECSER_MISC1_MASK					0x0010
		#define	SECSER_100FX_STATUS_MASK			0x0011
		#define	SECSER_100FX_TEST_MASK				0x0012
		#define	SECSER_PACKET_CNT_MASK				0x0013
		#define	SECSER_ERROR_CNT_MASK				0x0014
		#define	SECSER_SGMII_SLAVE_MASK				0x0015
		#define	SECSER_CONTROL_MASK					0x0016
		#define	SECSER_MISC_CNTL2_MASK				0x0017
		#define	SECSER_MISC_CNTL3_MASK				0x0018
		#define	SECSER_ANEG_DEBUG_MASK				0x001A
		#define	SECSER_1GX_AUXCNTL_MASK				0x001B
		#define	SECSER_1GX_AUXSTAT_MASK				0x001C
		#define	SECSER_1GX_MISCSTAT_MASK			0x001D
		#define	SECSER_SPARE_REG_MASK				0x0021
		#define	SECSER_INTR_STATUS_MASK				0x0030
		#define	SECSER_INTR_MASK_MASK				0x0031
		#define	SECSER_LINE_LPBKCNTL_MASK			0x0044


#define PHY_AUXCNTL_REGISTER_SELECT		0x18
	#define	AUX_CONTROL_REG_MASK				0x0000
	#define	AUXCNTL_10T_REG_MASK				0x0001
	#define	AUXCNTL_PWR_MII_MASK				0x0002
	#define	AUXCNTL_MISC_TEST_MASK				0x0004
		#define LINESIDE_LOOPBACK_ON				0x8000
		#define LINESIDE_LOOPBACK_OFF				0x0000
	#define	AUXCNTL_MISC_CNTL_MASK				0x0007	// NOTE: for 7 (111) bit.15 must be written to control which bits are written!
		#define READ_AUX_CONTROL_REG				0x0000	// Write to AUXCNTL_REG (bit.15=0) to READ AUX_CONTROL SHADOW REG (000)
		#define READ_AUXCNTL_MISC_CNTL_REG			0x7007	// Write to AUXCNTL_REG (bit.15=0) to READ MISC_CONTROL SHADOW REG (111)
		// AFTER READ MUST SET READ REG SELECTER=000 & bit.15 as appropriate
			#define RGMII_RXC_DELAYED_TIMING		0x0100	// OR to AUXCNTL_MISC_CNTL_MASK (18h) with AUXCNTL_MISC_CNTL_MASK (and WRITE_MASK) to select
			#define RGMII_RXC_ALIGNED_TIMING_MASK	0xFEFF	// AND to AUXCNTL_MISC_CNTL_MASK (18h) with AUXCNTL_MISC_CNTL_MASK (and WRITE_MASK) to select


#define PHY_AUX_STATUS_SUMMARY			0x19
#define PHY_INTR_STATUS					0x1A
#define PHY_INTR_MASK					0x1B		// pg89
// TO READ 1C SHADOW Reg's, Write SHADOW Reg address to 1C with (bit.15=0)
// Subsequent read will be that register!
#define PHY_1C_SHADOW_REGISTER			0x1C
	#define	SPARE_CNTL1_MASK					0x0800	//00010 pg91
	#define	CLOCK_ALIGN_MASK					0x0C00	//00011 pg92
		#define	GTX_CLOCK_DELAYED					0x0200	// OR to PHY_1C_SHADOW_REGISTER (1Ch) with CLOCK_ALIGN_MASK (and WRITE_MASK) to Select
		#define GTX_CLOCK_DELAY_BYPASSED_MASK	0xFDFF	// AND to PHY_1C_SHADOW_REGISTER (1Ch) with CLOCK_ALIGN_MASK (and WRITE_MASK) to select

	#define	SPARE_CNTL2_MASK					0x1000	//00100 pg93
	#define	SPARE_CNTL3_MASK					0x1400	//00101 pg94
	#define	LED_STATUS_MASK						0x2000	//01000 pg95
	#define	LED_CONTROL_MASK					0x2400	// pg97
	#define	AUTO_PWRDN_MASK						0x2800	// pg98
	#define	LED_SELECT1_MASK					0x3400	// pg100
		#define SET_LED14_OFF_MASK					0x000E	// set LED Off (high)	LED1:select1, LED4:select2
		#define SET_LED14_ON_MASK					0x000F	// set LED ON  (low)	LED1:select1, LED4:select2
		#define SET_LED23_OFF_MASK					0x00E0	// set LED Off (high)	LED3:select1, LED2:select2
		#define SET_LED23_ON_MASK					0x00F0	// set LED ON  (low)	LED3:select1, LED2:select2

	#define	LED_SELECT2_MASK					0x3800	// pg102
	#define	EXTL_CONTROL2_MASK					0x3000	// pg99
	#define	LED_GPIO_MASK						0x3C00	// pg103
	#define	SHADOW16_REG16_MBP_MASK				0x4000	// not defined! ??
		#define PHY_SELECT_SHADOW16_REG(base, data)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, data&(SHADOW16_REG16_MBP_MASK|SET_WRITE_ENABLE_MASK))
			#define PHY_SET_SHADOW16_REG_BIT1_0_MASK 	0xFFFD	// MASK RD to set bit.1=0
	#define	SERDES_100FX_STAT_MASK				0x4400	// pg104
	#define	SERDES_100FX_EXT_MASK				0x4800	// pg106

	#define	SERDES_100FX_CTRL_MASK				0x4C00	// pg106 ----> 10011
			#define SERDES_100FX_SERDES_MASK		0xFFFC //mask on/off  & FDX/HDX bits
			#define SERDES_100FX_SERDES_ENABLE		0x0001	// OR in   bit^0=1 for 100FX,  bit^0=0 for 1000FX
			#define SERDES_100FX_SERDES_DISABLE	0xFFFE	// AND in  bit^0=1 for 100FX,  bit^0=0 for 1000FX
			#define SERDES_100FX_SERDES_FDX		0x0002	// OR in   bit^1=1 for 100FX Full Duplex  or 0 for Half Duplex
			#define SERDES_100FX_SERDES_HDX		0xFFFD	// AND in  bit^1 to 0 to clear (or half-duplex).

	#define	SEC_SERDES_CNTL_MASK				0x5000	// pg107
	#define	SGMII_SLAVE_MASK						0x5400	// pg109
	#define	PRI_SERDES_CNTL_MASK				0x5800	// pg111
	#define	MISC_1GX_CNTL2_MASK					0x5C00	// pg112
	#define	AUTODET_SGMII_MC_MASK				0x6000	// pg113
	#define	ANEG_DEBUG_1GX_MASK				0x6800	// pg114
	#define	AUX_1GX_CNTL_MASK					0x6C00	// pg116
	#define	AUX_1GX_STATUS_MASK				0x7000	// pg117
	#define	MISC_1GX_STATUS_MASK				0x7400	// pg119
	#define	CPRFBR_AUTODETECT_MASK			0x7800	// 11110
	#define	MODE_CONTROL_MASK					0x7C00	// 11111

#define PHY_MASTER_SLAVE_SEED_REGISTER	0x1D	//with BIT.15=0 pg124
#define PHY_HCD_STATUS_REGISTER			0x1D	//with BIT.15=1 pg125
#define PHY_TEST_REGISTER				0x1E	// pg127
//#define PHY_RSRVD_REGISTER					0x1F

//== General purpose MASK's and OR's
#define SET_WRITE_ENABLE_MASK			0x8000	// set bit.15=1	- (write with ~MASK to disable write).

//====================================================================
//	Special PHY Macro Functions
//====================================================================
#define PHY_1G_FORCE_MASTER	0x1800	// OR into (reg9) PHY_1000_CONTROL for operation w/external HW loopback
// they also want 0x0040 written to PHY_MII_CONTROL_ADDR (forced 1000speed and no FDX!??)
// 0x8400 to reg 18
#define AUX_CONTROL_ENABLE_EXT_LOOPBACK		0x8400
#define AUX_CONTROL_NORMAL_OPERATION	    0x0400
#define PHY_ENABLE_EXTERNAL_LOOPBACK(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|AUX_CONTROL_ENABLE_EXT_LOOPBACK);
#define PHY_NORMAL_REMOVE_EXT_LOOPBACK(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|AUX_CONTROL_NORMAL_OPERATION);

#define AUX_CONTROL_SET_COPPER_JUMBO		0x4000	// Or in these from read and write to change just this mode!
#define AUX_CONTROL_NO_COPPER_JUMBO_MASK	0xDFFF	// AND in to clr Jumbo mode
#define AUX_CONTROL_NO_COPPER_JUMBO			0x0000	// OR into register
#define PHY_ENABLE_COPPER_JUMBO(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|AUX_CONTROL_SET_COPPER_JUMBO);
#define PHY_DISABLE_COPPER_JUMBO(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUX_CONTROL_REG_MASK|AUX_CONTROL_NO_COPPER_JUMBO);

#define PHY_FIBER_REGS_SELECT		0x0001
#define PHY_FIBER_MODE_SELECT			0x0002	// selects fiber reg's too! don't think so, or it'd be 3!
#define PHY_COPPER_MODE_SELECT		0x0000	// selects copper reg's too!
#define PHY_SELECT_COPPER_MODE(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, MODE_CONTROL_MASK|SET_WRITE_ENABLE_MASK)
#define PHY_SELECT_FIBER_MODE(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, MODE_CONTROL_MASK|PHY_FIBER_MODE_SELECT|PHY_FIBER_REGS_SELECT|SET_WRITE_ENABLE_MASK)

#define PHY_NORMAL_OPERATION		0xF7FF	// not power down
#define PHY_SELECT_COPPER_MODE_FIBER_REGS(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, MODE_CONTROL_MASK|PHY_FIBER_REGS_SELECT|SET_WRITE_ENABLE_MASK)
#define PHY_SELECT_FIBER_MODE_COPPER_REGS(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, MODE_CONTROL_MASK|PHY_FIBER_MODE_SELECT|SET_WRITE_ENABLE_MASK)

#define PHY_LINESIDE_LOOPBACK_ON(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUXCNTL_MISC_TEST_MASK|LINESIDE_LOOPBACK_ON)
#define PHY_LINESIDE_LOOPBACK_OFF(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT,AUXCNTL_MISC_TEST_MASK|LINESIDE_LOOPBACK_OFF)

#define PHY_SELECT_100FX_MODE(base, value)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, value|SET_WRITE_ENABLE_MASK)
#define PHY_DESELECT_100FX_MODE(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, (SERDES_100FX_CTRL_MASK&SERDES_100FX_SERDES_DISABLE)|SET_WRITE_ENABLE_MASK)

#define PHY_SELECT_100FX_FDX(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, SERDES_100FX_CTRL_MASK|SERDES_100FX_SERDES_FDX|SET_WRITE_ENABLE_MASK)
#define PHY_SELECT_100FX_HDX(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, (SERDES_100FX_CTRL_MASK&SERDES_100FX_SERDES_HDX)|SET_WRITE_ENABLE_MASK)

// All our LED's (2) are connected to PHYB
//#define PHYA_TURN_LED1_ON		IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT1_MASK|SET_WRITE_ENABLE_MASK|SET_LED14_ON_MASK);
//#define PHYA_TURN_LED1_OFF	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT1_MASK|SET_WRITE_ENABLE_MASK|SET_LED14_OFF_MASK);
//#define PHYA_TURN_LED2_ON		IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT2_MASK|SET_WRITE_ENABLE_MASK|SET_LED23_ON_MASK);
//#define PHYA_TURN_LED2_OFF	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_0_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT2_MASK|SET_WRITE_ENABLE_MASK|SET_LED23_OFF_MASK);
//********* don't use LED2 - as it hoses Fiber_SD input to PHY!!! ******
//#define PHYB_TURN_LED1PHYA_A_ON		IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT1_MASK|SET_WRITE_ENABLE_MASK|SET_LED14_ON_MASK)
//#define PHYB_TURN_LED1PHYA_A_OFF	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT1_MASK|SET_WRITE_ENABLE_MASK|SET_LED14_OFF_MASK)
//#define PHYB_TURN_LED2PHYB_B_ON		IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT2_MASK|SET_WRITE_ENABLE_MASK|SET_LED23_ON_MASK)
//#define PHYB_TURN_LED2PHYB_B_OFF	IOWR_ALTERA_TSEMAC_MDIO(TRIPLE_SPEED_ETHERNET_1_BASE, MDIO_0, PHY_1C_SHADOW_REGISTER,LED_SELECT2_MASK|SET_WRITE_ENABLE_MASK|SET_LED23_OFF_MASK)

#define SET_PHY_GTX_CLOCK_DELAY_ON(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, CLOCK_ALIGN_MASK|GTX_CLOCK_DELAYED|SET_WRITE_ENABLE_MASK)
#define SET_PHY_GTX_CLOCK_DELAY_BYPASSED(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, CLOCK_ALIGN_MASK|SET_WRITE_ENABLE_MASK)

#define SETREAD_18_AUXCNTL_MISC_CNTL_REG(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT, READ_AUXCNTL_MISC_CNTL_REG);
#define SETREAD_18_AUXCNTL_REG(base)			IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT, READ_AUX_CONTROL_REG);
#define READ_18_SHADOW(base)					IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_AUXCNTL_REGISTER_SELECT);

#define SETREAD_1C_SHADOW_REG(base,reg)			IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER, reg);
#define READ_1C_SHADOW(base)					IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_1C_SHADOW_REGISTER);


//===== EXPANSION Register Macro's ==========
#define SET_17_EXPANSION_REG(base,reg)					IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_EXPANSION_REG_SELECT, EXP_REGISTER_MASK|reg);
#define DELSELECT_17_EXPANSION_REG(base)				IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_EXPANSION_REG_SELECT, EXP_DESELECT_MASK);
#define READ_EXPANSION_REG17_VIA_15(base)				IORD_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_EXPANSION_REG_ACCESS);
#define SET_17_EXPANSION_TO_OPERATING_MODE_STATUS(base)	IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_EXPANSION_REG_SELECT, EXP_REGISTER_MASK|EXP_REG_OPERATING_MODE_STATUS_MASK);
#define SET_17_EXPANSION_TO_SERDES_RX_CONTROL(base)		IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_EXPANSION_REG_SELECT, EXP_REGISTER_MASK|EXP_REG_SERDES_RX_CONTROL_MASK);
#define WRITE_EXPANSION_REG17_VIA_15(base, value)				IOWR_ALTERA_TSEMAC_MDIO(base, MDIO_0, PHY_EXPANSION_REG_ACCESS, value);

//============================================================================
//==== SFP Module PHY Registers Definition ===================================
//============================================================================
#define SFP_READ					0x01
#define SFP_ADDR_CORE_A0       		0xA0
#define SFP_ADDR_DIAG_A2           	0xA2

//======== SFP REG ADDRESSES (std-SFP and DMI-SFP:A0) ==============
#define Identifier					0		//1  Type of transceiver (see Table 3.2)
#define Ext_Identifier				1		//1  Extended identifier of type of transceiver (see Table 3.3)
#define Connector					2		//1  Code for connector type (see Table 3.4)
#define Transceiver3				3		//8  Code for electronic or optical compatibility (see Table 3.5)
#define Transceiver4				4
#define Transceiver5				5
#define Transceiver6				6
#define Transceiver7				7
#define Transceiver8				8
#define Transceiver9				9
#define Transceiver10				10
#define Encoding					11		//1  Code for high speed serial encoding algorithm (see Table 3.6)
#define BR_Nominal					12		//1  Nominal signalling rate, units of 100MBd.
//Rate_Identifier DMI-SFP only, reserved for core SFP
#define Rate_Identifier				13		//1  Type of rate select functionality (see Table 3.6a)
#define Length_SMF_km				14		//1  Link length supported for single mode fiber, units of km
#define Length_SMF					15		//1  Link length supported for single mode fiber, units of 100 m
#define Length_50um					16		//1  Link length supported for 50 um OM2 fiber, units of 10 m
#define Length_62_5um				17		//1  Link length supported for 62.5 um OM1 fiber, units of 10 m
#define Length_cable				18		//1  Link length supported for copper or direct attach cable, units of m
//Length_OM3 DMI-SFP only, reserved for core SFP
#define Length_OM3					19		//1  Link length supported for 50 um OM3 fiber, units of 10 m
#define Vendor_name1				20		//16  SFP vendor name (ASCII)
#define Vendor_name2				21
#define Vendor_name3				22
#define Vendor_name4				23
#define Vendor_name5				24
#define Vendor_name6				25
#define Vendor_name7				26
#define Vendor_name8				27
#define Vendor_name9				28
#define Vendor_name10				29
#define Vendor_name11				30
#define Vendor_name12				31
#define Vendor_name13				32
#define Vendor_name14				33
#define Vendor_name15				34
#define Vendor_name16				35
//Transceiver36 DMI-SFP only, reserved for core SFP
#define Transceiver36				36		//1  Code for electronic or optical compatibility (see Table 3.5)
#define Vendor_OUI					37-39	//3  SFP vendor IEEE company ID
#define Vendor_PN					40-55	//16  Part number provided by SFP vendor (ASCII)
#define Vendor_rev					56-59	//4  Revision level for part number provided by vendor (ASCII)
//Wavelength DMI-SFP only, 60-61 reserved for core SFP
#define Wavelength					60-61	//2  Laser wavelength (Passive/Active Cable Specification Compliance)
//#define Unallocated				62		//1
#define CC_BASE						63		//1  Check code for Base ID Fields (addresses 0 to 62)
//======== EXTENDED ID FIELDS ===================
#define Options						64-65	//2  Indicates which optional transceiver signals are implemented (see Table 3.7)
#define BR_max						66		//1  Upper bit rate margin, units of %
#define BR_min						67		//1  Lower bit rate margin, units of %
#define Vendor_SN					68-83	//16  Serial number provided by vendor (ASCII)
#define Date_code					84-91	//8  Vendor’s manufacturing date code (see Table 3.8)
//92-94 are DMI-SFP only, reserved for core SFP
#define Diagnostic_Monitoring_Type	92		//1  - Indicates which type of diagnostic monitoring is implemented (if any) in the transceiver (see Table 3.9)
#define Enhanced_Options			93		//1  Indicates which optional enhanced features are implemented (if any) in the transceiver (see Table 3.10)
#define SFF_8472_Compliance			94		//1  Indicates which revision of SFF-8472 the transceiver complies with. (see Table 3.12)
#define CC_EXT 						95		//1 Check code for the Extended ID Fields (addresses 64 to 94)
//======== VENDOR SPECIFIC ID FIELDS ============
#define Vendor_Specific_CORE		96-127	//32  Vendor Specific EEPROM
//#define Reserved_SFF-8079 			128-255	//128 Reserved for SFF-8079
//====================================================================

//======== SFP REG ADDRESSES (DMI-SFP:A2) ==============
//      DIAGNOSTIC AND CONTROL/STATUS FIELDS
#define AW_Thresholds		0-39	  		//40 Diagnostic Flag Alarm and Warning Thresholds (see Table 3.15)
//#define Unallocated		40-55 			//16
#define Ext_Cal_Constants	56-91	  		//36 Diagnostic calibration constants for optional External Calibration (see Table 3.16)
//#define Unallocated		92-94 			//3
#define CC_DMI				95		  		//1 Check code for Base Diagnostic Fields (addresses 0 to 94)
#define Diagnostics			96-105  		//10 Diagnostic Monitor Data (internally or externally calibrated) (see Table 3.17)
#define SFP_RxPower			104				// RxPower at 104+105 (104=MSB)
//#define Unallocated		106-109 		//4
#define Status_Control		110  			//1 Optional Status and Control Bits (see Table 3.17)
//#define Reserved			111  			//1 Reserved for SFF-8079
#define Alarm_Flags			112-113  		//2 Diagnostic Alarm Flag Status Bits (see Table 3.18)
//#define Unallocated		114-115 		//2
#define Warning_Flags		116-117  		//2 Diagnostic Warning Flag Status Bits (see Table 3.18)
#define Ext_Status_Control	118-119  		//2 Extended module control and status bytes (see Table 3.18a)
//======== GENERAL USE FIELDS =====================
#define Vendor_Specific_DMI	120-127  		//8 Vendor specific memory addresses (see Table 3.19)
#define User_EEPROM			128-247  		//120 User writable non-volatile memory (see Table 3.20)
#define Vendor_Control		248-255  		//8 Vendor specific control addresses (see Table 3.21)


//=======================================================================================================
//=======================================================================================================
//	10G
//=======================================================================================================
//10G_SC_FIFO mode: 0=set to store & forward mode, >0=cut-through mode
#define ETH_10G_SCFIFO_STORE_FORWARD	0
#define ETH_10G_SCFIFO_CUT_THRU			1

//#define ETH_10G_DESIGN_EXAMPLE_0_BASE 0x00100000 //system.h 0x0010:0000 - 0x017:FFFF
#define ETH_10G_MAC_BASE_ADDR 			0x00000000
#define ETH_10G_MDIO_BASE_ADDR 			0x00010000
#define ETH_10G_LPBK_BASE_ADDR 			0x00010200
#define ETH_10G_RX_SC_FIFO_BASE_ADDR 	0x00010400
#define ETH_10G_TX_SC_FIFO_BASE_ADDR 	0x00010600
#define ETH_10G_XAUI_BASE_ADDR 			0x00040000

// TEMP TEMP
//#define TRAFFIC_CONTROLLER_10G_0_BASE  0x04000000 //system.h
// BOGUS BOGUS BOGUS BOGUS BOGUS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define TRAFFIC_CONTROLLER_GUISYS_0_BASE 0x04000000
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define TRAFFIC_CONTROLLER_GEN_BASE_ADDR 0x00000000
#define TRAFFIC_CONTROLLER_MON_BASE_ADDR 0x00040000 // 0x00 01 00 00  23:16=8d1


//===============================================================================
//	10G: MDIO
//===============================================================================
//#define ETH_10G_MDIO_BASE_ADDR	0x00010000	// base addr of MDIO within ETH10G block
#define MDIO_PHY_CONTROL			0x84	// write who you want to access
#define MDIO_PHY_RW					0x80	// R/W content to external PHY
#define PHY_PRTAD				  0x0200	// 0x02<<8
#define PHY_QTX_DEVAD_1_PMAD		0x01
#define PHY_QTX_DEVAD_2_WIS			0x02
#define PHY_QTX_DEVAD_3_PCS			0x03
#define PHY_QTX_DEVAD_4_XGXS		0x04

//==============================================================================
//	10G: LOOPBACK Map
//==============================================================================
//#define ETH_10G_LPBK_BASE_ADDR 			0x00010200
#define ETH_10G_LINE_LOOPBACK	0x00 //Set this register to 1 to enable line loopback; 0 to disable it.
#define ETH_10G_RSRVD_LOOPBACK	0x04 // Reserved —
#define ETH_10G_LOCAL_LOOPBACK	0x08 //Set this register to 1 to enable local loopback; 0 to disable it.
#define XGMII_LLPBK             0x08 //??
//==============================================================================
//	10G: SC-FIFO Map
//==============================================================================
//#define ETH_10G_RX_SC_FIFO_BASE_ADDR 	0x00010400
//#define ETH_10G_TX_SC_FIFO_BASE_ADDR 	0x00010600
//........... SC-FIFO Register Offsets ...............
// I.E. ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR; (0x10400 | 0x0010 = 0x10410, word:0x4104)
// I.E. ETH_10G_RX_SC_FIFO_BASE_ADDR, SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR; (0x10600 | 0x0010 = 0x10610, word:0x4184)
#define  SC_FIFO_FILL_LEVEL_ADDR 				0x0000	//0x00	24-bit [23:0], [31:24] are unused
#define  SC_FIFO_ALMOST_FULL_THRESHOLD_ADDR 	0x0008	//0x02  default: FIFO depth-1  "FIFO is getting full!"
#define  SC_FIFO_ALMOST_EMPTY_THRESHOLD_ADDR 	0x000C	//0x03 "FIFO is getting empty!"
#define  SC_FIFO_CUT_THROUGH_THRESHOLD_ADDR 	0x0010	//0x04 0=Store&forward, >0=cut-through (if store&forward enabled in Qsys)
#define  SC_FIFO_DROP_ON_ERROR_ADDR			 	0x0014	//0x05 1=enable drop on error (if store&forward enabled in Qsys)

//==============================================================================
//	10G: XAUI Address Map  page 116
//==============================================================================
//#define ETH_10G_XAUI_BASE_ADDR 		0x00040000
#define	ALT_PMA_CONTROLLER_BASE_ADDR    0x00000080	// wordAddr:0x20
	#define CAL_BLK_POWERDOWN  	    		0x0084  //0x21 writing 1 to a channel-n powers down the cal blk for chan-n
	#define PMA_TX_PLL_IS_LOCKED  	   		0x0088  //0x22 Read only
#define	XAUI_RESET_CONTROL_BASE_ADDR	0x00000100	// wordAddr:0x40
	#define	XAUI_RESET_CH_BIT_MASK				0x0104	//0x41 wordAddr:0x41 default: all-1's (used for 42 and 44)
	#define	XAUI_RESET_AUTO_CONTROLSTAT			0x0108	//0x42 wordAddr:0x42 RD  Rx^1/Tx^0 reset status
	#define	XAUI_RESET_MANUAL_CONTROL			0x0110	//0x44 wordAddr:0x44
#define	ALT_PMA_CH_CONTROLLER_BASE_ADDR 0x00000180	//0x60 wordAddr:0x60
	#define SERIAL_LLPBK  	    				0x0184 //0x61 wd:0x61 set to 1 to that chan for serial lpbk
	#define XAUI_PHY_SERIALLPBK 	    		0x0184 //0x61
	#define XAUI_PMA_rx_set_locktodata 	    	0x0190 //0x64
	#define XAUI_PMA_rx_set_locktoref 	    	0x0194 //0x65
	#define XAUI_PMA_rx_is_lockedtodata 	    0x0198 //0x66
	#define XAUI_PMA_rx_is_lockedtoref 	    	0x019C //0x67
#define	ALT_XAUI_PCS_BASE_ADDR          0x00000200	// wordAddr:0x80
	#define PMA_RXINVPOL  	    				0x208 //??
	#define PMA_TXINVPOL  	    				0x20C //??

//==============================================================================
//	10G: MAC Address Map
//==============================================================================
//#define ETH_10G_MAC_BASE_ADDR 				0x00000000

//........... RX path .....................
#define  RX_BACKPRESSURE_BASE_ADDR 				0x00000000 //from csr
#define  RX_PACKET_TRANSFER_BASE_ADDR 			0x00000000
	#define  RX_TRANSFER_CONTROL_ADDR 				0x00000000	//0x00
	#define  RX_TRANSFER_STATUS_ADDR 				0x00000004	//0x01
#define  RX_CRC_PAD_REMOVER_BASE_ADDR 			0x00000100
	#define RX_PADCRC_CONTROL_ADDR					0x00000100	//0x40
#define  RX_CRC_CHECKER_BASE_ADDR 				0x00000200
	#define RX_CRCCHECK_CONTROL_ADDR				0x00000200	//0x80
#define  RX_OVERFLOW_CTRL_BASE_ADDR 			0x00000300
	#define RX_PKTOVRFLOW0_ERROR_ADDR				0x00000300	//0xC0  1st 32-bits
	#define RX_PKTOVRFLOW1_ERROR_ADDR				0x00000304	//0xC1	last 4-bits [31:4]unused (clr'd when these are read)

	#define RX_PKTOVRFLOW0_ETHSTATSDROPEVENTS_ADDR	0x00000308	//0xC2  1st 32-bits
	#define RX_PKTOVRFLOW1_ETHSTATSDROPEVENTS_ADDR	0x0000030C	//0xC3	last 4-bits [31:4]unused (clr'd when these are read)

//#define  RX_PREAMBLE_INSERTER_CONTROL_ADDR	0x00000400	//0x140 ?????	these might be swapped
//#define  RX_LANE_DECODER_CONTROL_ADDR			0x00000500	//0x100 ?????   these might be swapped


#define  RX_FRAME_DECODER_BASE_ADDR 			0x00002000
	#define RX_FRAME_CONTROL_ADDR 					0x00002000	//0x800
													// whole crap load of bits to diddle here!
		#define	EN_ALLUCAST_BIT							0x00000001 // 1=enable accept all Unicast addr's (Promiscuous mode)
		#define	EN_ALLMCAST_BIT							0x00000002 // 1=enable to accept all Multicast addr's
		#define	EN_RSRVD1_BIT							0x00000004 // not used - reserved
		#define	EN_FWD_CONTROL							0x00000008 // 1=forward control frames to client
		#define	EN_FWD_PAUSE							0x00000010 // 1=forward pause frames to client
		#define	IGNORE_PAUSE_BIT						0x00000020 // 1=Ignore Rcvd Pause_control pkts (disable flow control)
		// bits6-15 reserved
		#define	EN_SUPP0								0x00010000 // enable use of supplementary address 0
		#define	EN_SUPP1								0x00020000
		#define	EN_SUPP2								0x00040000
		#define	EN_SUPP3								0x00080000

	#define RX_FRAME_MAXLENGTH_ADDR					0x00002004	//0x801 default=1518, [15:0]=max frame length
													//...rx_error asserted if rx frame is bigger than this

	// Primary MAC address.  the MAC uses this addr to filter unicast frames when the en_allucast bit==0
	#define RX_FRAME_ADDR0_ADDR						0x00002008	//0x802 MAC0 addr, last 4-bytes of MAC
	#define RX_FRAME_ADDR1_ADDR						0x0000200C	//0x803 MAC1 addr, 1st 2 bytes of MAC
													//...if MAC: 00-1C-23-17-4A-CB
													//then   MAC1=0x001C and MAC0 = 0x23174ACB
	#define RX_FRAME_SUP0_ADDR0_ADDR				0x00002010	//0x804 supplementary0 MAC0 addr, last 4-bytes of MAC
	#define RX_FRAME_SUP0_ADDR1_ADDR				0x00002014	//0x805 supplementary0 MAC1 addr, 1st 2 bytes of MAC
	#define RX_FRAME_SUP1_ADDR0_ADDR				0x00002018	//0x806 supplementary1 MAC0 addr, last 4-bytes of MAC
	#define RX_FRAME_SUP1_ADDR1_ADDR				0x0000201C	//0x807 supplementary1 MAC1 addr, 1st 2 bytes of MAC
	#define RX_FRAME_SUP2_ADDR0_ADDR				0x00002020	//0x808 supplementary2 MAC1 addr, 1st 2 bytes of MAC
	#define RX_FRAME_SUP2_ADDR1_ADDR				0x00002024	//0x809 supplementary2 MAC1 addr, 1st 2 bytes of MAC
	#define RX_FRAME_SUP3_ADDR0_ADDR				0x00002028	//0x80A supplementary3 MAC0 addr, last 4-bytes of MAC
	#define RX_FRAME_SUP3_ADDR1_ADDR				0x0000202C	//0x80B supplementary3 MAC1 addr, 1st 2 bytes of MAC

	#define RX_PFC_CONTROL_ADDR						0x0000202C	//0x818

#define  RX_STATISTICS_BASE_ADDR 				0x00003000 // 0x0C00 ....see STAT_OFFSET def's below...

//........... TX path .....................
#define  TX_BACKPRESSURE_BASE_ADDR 				0x00004000	//from csr
#define  TX_TRANSFER_BASE_ADDR 					0x00004000	//0x1000
	#define  TX_TRANSFER_CONTROL_ADDR 				0x00004000	//0x1000
	#define  TX_TRANSFER_STATUS_ADDR 				0x00004004	//0x1001

#define  PAD_INSERTER_BASE_ADDR 				0x00004100 //csr (0x1040-107F)
	#define  TX_PADINS_CONTROL_ADDR 				0x00004100	//0x1040
#define  CRC_INSERTER_BASE_ADDR 				0x00004200
	#define  TX_CRCINS_CONTROL_ADDR 				0x00004200	//0x1080
#define  TX_PACKET_UNDERFLOW_BASE_ADDR			0x00004300
	#define  TX_PKTUNDERFLOW0_ERROR_ADDR 			0x00004300	//0x10C0 [31:0] 36-bit total
	#define  TX_PKTUNDERFLOW1_ERROR_ADDR 			0x00004304	//0x10C1 [35:32] (reg clr'd when last 4 read)
#define  TX_PREAMBLE_BASE_ADDR					0x00004400
	#define  TX_PREAMBLE_CONTROL_ADDR	 			0x00004400	//0x1100 [31:0]
#define  PAUSE_GEN_CTRL_BASE_ADDR 				0x00004500
	#define  TX_PAUSEFRAME_CONTROL_ADDR 			0x00004500	//0x1140
	#define  TX_PAUSEFRAME_QUANTA_ADDR 				0x00004504	//0x1141
	#define  TX_PAUSEFRAME_ENABLE_ADDR 				0x00004508	//0x1142
#define  TX_PFC_GENERATOR_BASE_ADDR 			0x00004600
	#define  TX_PFC_PAUSE_QUANTA0_ADDR	 			0x00004600	//0x1180
	#define  TX_PFC_PAUSE_QUANTA1_ADDR	 			0x00004604	//0x1181
	#define  TX_PFC_PAUSE_QUANTA2_ADDR	 			0x00004608	//0x1182
	#define  TX_PFC_PAUSE_QUANTA3_ADDR	 			0x0000460C	//0x1183
	#define  TX_PFC_PAUSE_QUANTA4_ADDR	 			0x00004610	//0x1184
	#define  TX_PFC_PAUSE_QUANTA5_ADDR	 			0x00004614	//0x1185
	#define  TX_PFC_PAUSE_QUANTA6_ADDR	 			0x00004618	//0x1186
	#define  TX_PFC_PAUSE_QUANTA7_ADDR	 			0x0000461C	//0x1187

	#define  TX_PFC_HOLDOFF_QUANTA0_ADDR	 		0x00004640	//0x1190
	#define  TX_PFC_HOLDOFF_QUANTA1_ADDR	 		0x00004644	//0x1191
	#define  TX_PFC_HOLDOFF_QUANTA2_ADDR	 		0x00004648	//0x1192
	#define  TX_PFC_HOLDOFF_QUANTA3_ADDR	 		0x0000464C	//0x1193
	#define  TX_PFC_HOLDOFF_QUANTA4_ADDR	 		0x00004650	//0x1194
	#define  TX_PFC_HOLDOFF_QUANTA5_ADDR	 		0x00004654	//0x1195
	#define  TX_PFC_HOLDOFF_QUANTA6_ADDR	 		0x00004658	//0x1196
	#define  TX_PFC_HOLDOFF_QUANTA7_ADDR	 		0x0000465C	//0x1197

	#define  TX_PFC_PRIORITY_ENABLE_ADDR	 		0x00004680	//0x11A0

#define  ADDRESS_INSERTER_BASE_ADDR 			0x00004800 //csr MAC0 to MAC_BASE+this_addr+0x04, MAC1 to same 'cept +0x08
	#define  TX_ADDRINS_CONTROL_ADDR	 			0x00004800	//0x1200
	#define  TX_ADDRINS_MACADDR0_ADDR	 			0x00004804	//0x1201 MAC0 LSB 4-bytes
	#define  TX_ADDRINS_MACADDR1_ADDR	 			0x00004808	//0x1202 MAC1 MSB 2-bytes

#define  TX_FRAME_DECODER_BASE_ADDR 			0x00006000 //csr
	#define  TX_FRAME_MAXLENGTH_ADDR 				0x00006004 //0x1801 [15:0] only, upper ignored - only results in tx_status_error - not in size sent through tx path!

//#define  UNDERFLOW_CTRL_BASE_ADDR 			0x00006300 //csr - underfined! ??
#define  TX_STATISTICS_BASE_ADDR 				0x00007000 // 0x1C00 ....see STAT_OFFSET def's below...

//........... Statistics Register Offsets ...............
// I.E. for Rx Stats:  RX_STATISTICS_BASE_ADDR | STATISTICS_framesOK_OFFSET; (0x3000 | 0x0008 = 0x3008, word:0x0C02)
// I.E. for Tx Stats:  TX_STATISTICS_BASE_ADDR | STATISTICS_framesOK_OFFSET; (0x7000 | 0x0008 = 0x7008, word:0x1C02)
#define	STATISTICS_OFFSET_MSB	0x0004
// every Stat register is 36-bits (a pair) Offset is [31:0], addr+0x04 is [35:32]
//   i.e. LSB = framesOK_OFFSET,   MSB = framesOK_OFFSET+STATISTICS_OFFSET_MSB
// NOTE 36-bitters passed as 32-bits to GUI clips max 64-byte frames to 28862secs (max=8hrs)
#define STATISTICS_CLEAR_OFFSET                           0x0000 // write 0x01 to clr all stat counters!
#define STATISTICS_framesOK_OFFSET                        0x0008
#define STATISTICS_framesErr_OFFSET                       0x0010	// byte:0x3010 or word:0x0C04
#define STATISTICS_framesCRCErr_OFFSET                    0x0018
#define STATISTICS_octetsOK_OFFSET                        0x0020
#define STATISTICS_pauseMACCtrlFrames_OFFSET              0x0028
#define STATISTICS_ifErrors_OFFSET                        0x0030
#define STATISTICS_unicastFramesOK_OFFSET                 0x0038
#define STATISTICS_unicastFramesErr_OFFSET                0x0040
#define STATISTICS_multicastFramesOK_OFFSET               0x0048
#define STATISTICS_multicastFramesErr_OFFSET              0x0050
#define STATISTICS_broadcastFramesOK_OFFSET               0x0058
#define STATISTICS_broadcastFramesErr_OFFSET              0x0060
#define STATISTICS_etherStatsOctets_OFFSET                0x0068
#define STATISTICS_etherStatsPkts_OFFSET                  0x0070
#define STATISTICS_etherStatsUndersizePkts_OFFSET         0x0078
#define STATISTICS_etherStatsOversizePkts_OFFSET          0x0080
#define STATISTICS_etherStatsPkts64Octets_OFFSET          0x0088
#define STATISTICS_etherStatsPkts65to127Octets_OFFSET     0x0090
#define STATISTICS_etherStatsPkts128to255Octets_OFFSET    0x0098
#define STATISTICS_etherStatsPkts256to511Octets_OFFSET    0x00A0
#define STATISTICS_etherStatsPkts512to1023Octets_OFFSET   0x00A8
#define STATISTICS_etherStatPkts1024to1518Octets_OFFSET   0x00B0
#define STATISTICS_etherStatsPkts1519toXOctets_OFFSET     0x00B8
#define STATISTICS_etherStatsFragments_OFFSET             0x00C0
#define STATISTICS_etherStatsJabbers_OFFSET               0x00C8
#define STATISTICS_etherStatsCRCErr_OFFSET                0x00D0
#define STATISTICS_unicastMACCtrlFrames_OFFSET            0x00D8
#define STATISTICS_multicastMACCtrlFrames_OFFSET          0x00E0
#define STATISTICS_broadcastMACCtrlFrames_OFFSET          0x00E8



//==============================================================================
//	TRAFFIC_CONTROLLER (Generator & Monitor) Map
//==============================================================================
#define TRAFFIC_GEN_NUM_PACKET	0x00
#define TRAFFIC_GEN_NUM_PACKET1	0x04 //0x01
#define TRAFFIC_GEN_STREAM_KEY		0x08 //0x02 (0=1st stream, 1=2nd stream, etc.)
#define TRAFFIC_GEN_START			0x0C //0x03 1-bit, 1=START generator!
#define TRAFFIC_GEN_STOP			0x10 //0x04 1-bit, 1=STOP generator!
#define TRAFFIC_GEN_GBLCONFIG		0x14 //8'h5;		// global configuration
	// GBLCFG_LINERATE_SIZ	= 2;			 // external ethernet link rate field size bits
	#define LINKRATE_MASK				0x03
	#define LINKRATE_10G				0x00
	#define LINKRATE_1G					0x01
	#define LINKRATE_100M				0x02
	#define LINKRATE_10M				0x03
#define TRAFFIC_GEN_PKT_TX_CNT    	0x24 //0x09 RD only (# Tx'd frames)
#define TRAFFIC_GEN_PKT_TX_CNT1		0x28 //0x0A

#define TRAFFIC_GEN_CONFIG	        0x200	//0x80 Config Fields for GENERATOR
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
/*
		#define LEN_MASK			0xFFFF
		#define HDRLEN_LLCSNAP		0x160000 //22 for llc snap	//0x0E0000		//14	0E
		#define HDRLEN_ETH			0x0E0000
		#define HDRLEN_VLAN_DIX		0x120000		//18	12
		#define HDRLEN_VLAN_LLC		0x1A0000		//26	1A
		#define HDRLEN_SVLAN_DIX	0x160000		//22	16
		#define HDRLEN_SVLAN_LLC	0x1E0000		//30    1E
		#define HDRLEN_IP4_LLC		0x2A0000		//42	2A
		#define HDRLEN_IP4_DIX		0x220000		//34	22
		#define HDRLEN_VLANIP4_DIX	0x260000		//38	26
		#define HDRLEN_VLANIP4_LLC	0x2E0000		//46    2E
		#define HDRLEN_SVLANIP4_LLC	0x320000		//50	32
		#define HDRLEN_SVLANIP4_DIX	0x2A0000		//42	2A
		#define HDRLEN_IP6_DIX		0x360000		//54	36
		#define HDRLEN_IP6_LLC		0x3E0000		//62	3E
		#define HDRLEN_VLANIP6_DIX	0x3A0000		//58	3A
		#define HDRLEN_VLANIP6_LLC	0x420000		//66	42
		#define HDRLEN_SVLANIP6_DIX	0x3E0000		//62	3E
		#define HDRLEN_SVLANIP6_LLC	0x460000		//70	46
		#define HDRLEN_MASK		0xFF0000			//62	3E
		#define RNDLEN_ON	   	0x01000000
		#define RNDPAY_ON	   	0x02000000
		#define FRCERR_ON	   	0x04000000
		#define CONTINUOUS_ON 	0x08000000
		#define BWRANK_MASK 	0xF0000000
*/
#define LEN_MASK			0xFFFF

#define TRAFFIC_GEN_RNDSEED	  	0x204	//0x81 Random Seed Config Fields
		#define	TRAFFIC_GEN_EMIX_OFFSET_MASK	0xFFFF0000				// EMIX ADDER OFFSET 	16:31		16 bits: Offset to EMIX table
		#define TRAFFIC_GEN_RNDSEED_DEF	0xF00D5EED	//using the old GEN verilog def

#define TRAFFIC_GEN_PKTPERCENT	 	0x208	//0x82 Packet Period Count (number of clks between packet starts)

#define TRAFFIC_GEN_EMIXUSERSIZE1   0x230 //0x8C  Used to send the 'w' value when in emix - usually 70/78
#define TRAFFIC_GEN_EMIXUSERSIZE	0x20C	//0x83 EMIX/random user packet length register (gen.v:ADDR_RNDUSR) - RNDUSR
#define TRAFFIC_GEN_EMIXSEQ0	  	0x210	//0x84EMIX/random sequence list register 0  MSNibble sent 1st - RNDSEQ0
#define TRAFFIC_GEN_EMIXSEQ1		0x214	//0x85							      - RNDSEQ1
#define TRAFFIC_GEN_EMIXSEQ2		0x218	//0x86  (up to 32 sizes)  (RNDSEQ0 = 0x01234F00 is a 5-size sequence: 64,128,256,512,1024) - RNDSEQ2
#define TRAFFIC_GEN_EMIXSEQ3		0x21C	//0x87	EMIX/random sequence list register 3  LSNibble sent last - RNDSEQ3
		// 0     a    64
		// 1     b    128
		// 2     c    256
		// 3     d    512
		// 4     e    1024
		// 5     f    1280
		// 6     g    RNDUSR_G that should be 1518, 1522, 1526 etc depending on vlan tags etc
		// 7     h    MTU uses PKTLEN
		// 8     u    RNDUSR_U
/*
		#define DEF_GEN_EMIXUSER	0x258005EE		// U=0x2580, G=0x05EE   (9600 and 1518)
		#define DEBUG_GEN_EMIXUSER	0x05EF0041		// U=1519, G=65
		#define DEF_GEN_EMIXSEQ0	0x0123456F		//MEF: 0x01234567  7=MTU our's is 9600 ignore for now 64-1518
		#define DEF_EMIX_RANDOM		 (4782.0/7.0) // default EMIX average, 0x0123456F: 64,128,256,512,1024,1280,1518 = 4782/7
		#define DEF_GEN_EMIXSEQJ	0x0022338F      // above with a 9600 jumbo stuck in
//		#define DEF_EMIX_RANDOM		(11264.0/7.0) // for 'J' test variant (w/Jumbo frame)
		#define DEF_GEN_EMIXSEQ1	0xF0000000
		#define DEF_GEN_EMIXSEQ2	0xF0000000
		#define DEF_GEN_EMIXSEQ3	0xF0000000*/


#define TRAFFIC_GEN_CONFIG2	  		0X220 //0x88 -
// CONFG2_LENOFS_SIZ		0:6			// 7 bits: Length field byte offset in HDR
										// in EMIX mode set offset to 1st byte of HDR array (ie 802.3 frame frame_len starts at byte 12 so set this to 12).
										// LENOFS needed in EMIX modes so GEN knows where to put Next FrameSize!
// CONFG2_JUMBO_SIZ		   	7			// 1 bit:  jumbo length to 0x8870 handling field size bits
// CONFG2_HCSOFS_SIZ		8:14		// 7-bits: ipv4 hcs byte offset field size bits ==> set to 0 if NOT IPv4!!!
// CONFG2_ETH2_EN			15			// 1 bit:  EthII on/off
// CONFG2_SECOND_LENGTH_ADJ	16			// 1 bit:  Enable for secondary length adjustment engine
// CONFG2_SECOND_LENGTH_ADJ_JUMBO_EN	17			// 1 bit:  Jumbo enable for secondary length adjustment engine
// CONFG2_SECOND_LENGTH_ADJ_ETH2_EN		18			// 1 bit:  EthII enable for secondary length adjustment engine
// CONFG2_SPARE2_SIZ	    19:29		// 11-bits: spare field size bits
// CONFG2_ETH2_VALUE		30:31		// 2 bits: EthII value selector
										// 0: 0x0800
										// 1: 0x88B5
										// 2: 0x88B6
										// 3: 0xFFFF
/*

		#define LENOFS_MASK		0x007F	// Payload 1st byte location (0 is 1st byte of HDR0)
		#define JUMBO_MASK		0x0080
		#define HCSOFF_MASK		0x7F00
		#define ETH2_MASK		0x8000
		#define	SECOND_LGTH_ENGINE_EN_MASK		0x10000
		#define	JUMBO_SECOND_ENGINE_MASK		0x20000
		#define	ETH2_SECOND_ENGINE_MASK			0x40000
		#define	ETH2_VALUE_MASK	0xC0000000

			#define LENOFS_LAY2			12	// 802.3, EthII, LLC, SNAP
			#define LENOFS_VLAN			16	// single VLAN only
			#define LENOFS_SVLAN		20	// Stacked VLAN  (add +4 for each VLAN tag)
			#define	LENOFS_LLC_IPV4		24	// 802.3 IPv4 Offsets
			#define	LENOFS_LLC_VLANIPV4	28
			#define	LENOFS_LLC_SVLANIPV4 32
			#define LENOFS_IPV4			16	// IPv4 only
			#define LENOFS_VLANIPV4		20	// IP w/VLAN	(add +4 for each VLAN tag)
			#define LENOFS_SVLANIPV4	24	// IP w/VLAN	(add +4 for each VLAN tag)

			#define LENOFS_MPLS			18	// MPLS
			#define LENOFS_2MPLS		22	// MPLS	(add +6 for each MPLS tag)
			#define LENOFS_MPLSIPV4		22	// IP w/VLAN	(add +6 for each MPLS tag)
			#define LENOFS_2MPLSIPV4	28	// IP w/VLAN	(add +6 for each MPLS tag)

			#define	LENOFS_LLC_IPV6		26	// 802.3 LLC IPv6 Offsets
			#define	LENOFS_LLC_VLANIPV6	30
			#define	LENOFS_LLC_SVLANIPV6	34
			#define LENOFS_IPV6			18	// IPv6 only  and (add +4 for each VLAN tag) or (add +6 for each MPLS tag)
			#define LENOFS_VLANIPV6		22	// IP w/VLAN	(add +4 for each VLAN tag)
			#define LENOFS_SVLANIPV6	26	// IP w/VLAN	(add +4 for each VLAN tag)
*/

#define TRAFFIC_GEN_CONFIG3		0X224 // 0x89
// CONFIG3_ILEN		0:15			16-bit:  intermediate length for random length recalc field size bits
// CONFIG3_IHCS	     16:31			16-bit: intermediate HCS for random length HCS recalc field size bits

// ILEN is the offset for length/type value in L2 EMIX while it is used for the payload/total length field in IPv4/6
/*
		#define ILEN_MASK			0x0000FFFF	// ILEN = payload length - 64, (payload length=pkt-len-18)
		#define IHCS_MASK			0xFFFF0000

		#define ILENOFS_LAY2			18	// 802.3, EthII, LLC, SNAP
		#define ILENOFS_VLAN			22	// single VLAN only
		#define ILENOFS_SVLAN			26	// Stacked VLAN  (add +4 for each VLAN tag)

		#define	ILENOFS_LLC_IPV4		26
		#define	ILENOFS_LLC_VLANIPV4	30
		#define	ILENOFS_LLC_SVLANIPV4	34
		#define ILENOFS_IPV4			18	// IPv4 only
		#define ILENOFS_VLANIPV4		22	// IP w/VLAN	(add +4 for each VLAN tag)
		#define ILENOFS_SVLANIPV4		26	// IP w/VLAN	(add +4 for each VLAN tag)

		#define	ILENOFS_LLC_IPV6		66
		#define	ILENOFS_LLC_VLANIPV6	70
		#define	ILENOFS_LLC_SVLANIPV6	74
		#define ILENOFS_IPV6			58	// IPv6 only
		#define ILENOFS_VLANIPV6		62	// IP w/VLAN	(add +4 for each VLAN tag)
		#define ILENOFS_SVLANIPV6		66	// IP w/VLAN	(add +4 for each VLAN tag)
*/

		#define	TRAFFIC_GEN_CONFIG4		0x228	// 0x8A
		// CONFIG4_ILEN		0:15		16 bits: same usage as CONFIG3_ILEN; intermediate length for random length recalc field size bits
		// CONFIG4_LENOFS		16:22		7 bits: same usage as CONFIG2_LENOFS_SIZ
//
//		#define	ILENOFS_SECOND_ENGE_MASK	0xFFFF
//		#define	LENOFS_SECOND_ENGE_MASK		0x7F0000


#define TRAFFIC_GEN_ERROR      	0x22C // 0x8B 	1-bit: 1=insert error
#define TRAFFIC_GEN_VERSION    	0X230 // 0x8C	RD only to see what version of GEN we're using

#define TRAFFIC_GEN_RUNNING		0x03C0 //0xF0 new: 1-bit: 1=RUNNING, 0=IDLE


//=================================================
   // the header length above determines how much of the header registers are used.
   // note that the length in MAC or ipv4 or ipv6 or etc. is only overwritten for random length packets.
   // for ipv4, the header checksum must also be overwritten, because the length changed.
   // the maximum header is 128 bytes = 32 words * 8 bytes per word.
#define STREAM_ID			  0x51D0	// last nibble is actual stream_number
#define TRAFFIC_GEN_HDR0	  0x0300  //new
#define TRAFFIC_GEN_HDR1	  0x0304  //new
#define TRAFFIC_GEN_HDR2	  0x0308  //new
#define TRAFFIC_GEN_HDR3	  0x030C  //new
#define TRAFFIC_GEN_HDR4	  0x0310  //new
#define TRAFFIC_GEN_HDR5	  0x0314  //new
#define TRAFFIC_GEN_HDR6	  0x0318  //new
#define TRAFFIC_GEN_HDR7	  0x031C  //new
#define TRAFFIC_GEN_HDR8	  0x0320  //new
#define TRAFFIC_GEN_HDR9	  0x0324  //new
#define TRAFFIC_GEN_HDR10	  0x0328  //new
#define TRAFFIC_GEN_HDR11	  0x032C  //new
#define TRAFFIC_GEN_HDR12	  0x0330  //new
#define TRAFFIC_GEN_HDR13	  0x0334  //new
#define TRAFFIC_GEN_HDR14	  0x0338  //new
#define TRAFFIC_GEN_HDR15	  0x033C  //new   
#define TRAFFIC_GEN_HDR16	  0x0340  //new
#define TRAFFIC_GEN_HDR17	  0x0344  //new
#define TRAFFIC_GEN_HDR18	  0x0348  //new
#define TRAFFIC_GEN_HDR19	  0x034C  //new
#define TRAFFIC_GEN_HDR20	  0x0350  //new
#define TRAFFIC_GEN_HDR21	  0x0354  //new
#define TRAFFIC_GEN_HDR22	  0x0358  //new
#define TRAFFIC_GEN_HDR23	  0x035C  //new
#define TRAFFIC_GEN_HDR24	  0x0360  //new
#define TRAFFIC_GEN_HDR25	  0x0364  //new
#define TRAFFIC_GEN_HDR26	  0x0368  //new
#define TRAFFIC_GEN_HDR27	  0x036C  //new
#define TRAFFIC_GEN_HDR28	  0x0370  //new
#define TRAFFIC_GEN_HDR29	  0x0374  //new
#define TRAFFIC_GEN_HDR30	  0x0378  //new
#define TRAFFIC_GEN_HDR31	  0x037C  //new
//======================================================================================
// Traffic Controller Monitor Registers Offset Definitions
//--------------------------------------------------------------------------------------
#define TRAFFIC_MON_NUM_PACKET    			  0x0000	//0x00
#define TRAFFIC_MON_PKT_RX_OK     			  0x0004	//0x01
#define TRAFFIC_MON_PKT_RX_ERR    			  0x0008	//0x02
#define TRAFFIC_MON_BYTE_RX_CNT0  			  0x000C	//0x03
#define TRAFFIC_MON_BYTE_RX_CNT1  			  0x0010	//0x04
#define TRAFFIC_MON_CYCLE_RX_CNT0 		  0x0014	//0x05
#define TRAFFIC_MON_CYCLE_RX_CNT1 		  0x0018	//0x06
#define TRAFFIC_MON_RX_CTRL_STAT			  0x001C  //0x07 ^0=1 clr cntrs & Start mon, ^1=Stop, ^2=1 rcv_done, ^3=error
		#define TRAFFIC_MON_START_BIT	                        				0x01 // start&clr bit
		#define TRAFFIC_MON_RECEIVE_START_AND_CLEAR_CNTRS_BIT	0x01 // start&clr bit
		#define TRAFFIC_MON_RECEIVE_STOP_BIT							0x02 // STOP bit
		#define TRAFFIC_MON_RECEIVE_CONTROL_DONE_STATUS_BIT		0x04 // DONE bit
		#define TRAFFIC_MON_RECEIVE_ERROR_STATUS_BIT				0x08 // rcv error flag
		// TRAFFIC_RX_CTRL_STAT_CODE						4:9			6-bit:  rcv error code
		#define ERROR_CODE_MASK							0x03F0
		// TRAFFIC_RX_CTRL_SPARE							10:14		6-bit: spare - not used
		#define TRAFFIC_MON_ACTIVE_BIT                          0x0008000
		#define TRAFFIC_MON_RECEIVE_CONTINUOUS_BIT				0x0010000 // RCV Continuous (^16)
		#define TRAFFIC_MON_RESET_BIT	                        0x80000000 //RESET bit

		// TRAFFIC_RX_CTRL_SPARE1			17:31		15-bit: spare - not used
#define TRAFFIC_MON_CYCLE_START_RX_CNT0		0x0020	//0x08 new mbp
#define TRAFFIC_MON_CYCLE_START_RX_CNT1		0x0024	//0x09 new mbp
#define TRAFFIC_MON_CYCLE_END_RX_CNT0 		0x0028	//0x0A new mbp
#define TRAFFIC_MON_CYCLE_END_RX_CNT1 		0x002C	//0x0B new mbp
#define TRAFFIC_MON_BROADCAST_RX_CNT 		0x0030	//0x0C new mbp
#define TRAFFIC_MON_RX_DEST_MAC_ADDR0 		0x0034	//0x0D new mbp - this is what the Verilog mon captured
#define TRAFFIC_MON_RX_DEST_MAC_ADDR1 		0x0038	//0x0E new mbp
//================ New Addresses ========================= 
#define TRAFFIC_MON_NUM_MONINIT    	        0x003C	//new:8'h0F; not used
#define TRAFFIC_MON_NUM_MONACTIVE  	        0x0040	//new:8'h10; not used
#define TRAFFIC_MON_NUM_MONDONE    	        0x0044	//new:8'h11; not used
#define TRAFFIC_MON_NUM_MONNONE    	        0x0048	//new:8'h12; not used
#define TRAFFIC_MON_PKT_RX_MAC_ERR	        0x004C  // count of packets with errors detected by the mac and
#define TRAFFIC_MON_PKT_RX_IPV4HCS_ERR      0x0050  // count of packets with ipv4 hcs errors (unused)
#define TRAFFIC_MON_PKT_RX_EOP_ERR          0x0054  // count of packets without a valid stream id from pkt
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR      0x0058  // count of packets with a timeout error
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR      0x005C  //0X17 X4: count of packets with no stream header match and not a
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR   0x0060  // count of packets with multiple stream header matches
#define TRAFFIC_MON_STREAM_KEY              0x0064  // stream key, which stream is the nios accessing

		#define STREAM_KEY_INDEX_MASK		0x000000FF		//  8-bit: Stream index for HDR field
		#define STREAM_KEY_SPARE			0x00FFFF00		//16-bit: spare - not used
		#define STREAM_KEY_NUM_STREAMS_MASK	0xFF000000		//  8-bit: number of Matchers synthesized (read only)

		#define TRAFFIC_MON_NUM_PACKET1             0x006c  // upper bits of the corresponding count
		#define TRAFFIC_MON_STREAM_CNTL             0x0068  // stream control bits, payload_start + stream_enable ;
		#define STREAM_CNTL_ENABLE_MASK		0x00000001		// 1-bit:  Stream Enable bit
		#define STREAM_CNTL_SPARE1_MASK		0x000000FE		// 7-bit: spare - not used
		#define STREAM_CNTL_HDRLEN_MASK		0x0003FF00		//10-bit: Stream header length
		#define STREAM_CNTL_SPARE2_MASK		0xFFFC0000		//14-bit: spare - not used

#define TRAFFIC_MON_PKT_RX_OK1              0x0070  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_ERR1             0x0074  // upper bits of the corresponding count
#define TRAFFIC_MON_BROADCAST_RX_CNT1       0x0078  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_MAC_ERR1        	0x007c  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_EOP_ERR1        	0x0080  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR1     0x0084  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR1    	0x0088  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR1	0x008c  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT     0x0098  //26 number packets left in run
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT1    0x009C  //27 upper bits of the corresponding

#define TRAFFIC_MON_CYCLE_RX_CNT0_GLOBAL                 //8'h28;        // Run time from sop to eop
#define TRAFFIC_MON_CYCLE_RX_CNT1_GLOBAL                 //8'h29;        // upper bits of the corresponding count   //
#define TRAFFIC_MON_BYTE_RX_CNT0_GLOBAL     0x00C0              //8'h30;        // Number of bytes received for all streams
#define TRAFFIC_MON_BYTE_RX_CNT1_GLOBAL		0x00C4                   //8'h31;        // upper bits of the corresponding count   //
#define TRAFFIC_MON_MATCH_PACKET_RX_CNT0_GLOBAL		0x00C8
#define TRAFFIC_MON_MATCH_PACKET_RX_CNT1_GLOBAL		0x00CC
#define TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL	0x2C	//0xB	LSB of global tx packet counter
#define TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL	0x30	//0xC	MSB of global tx packet counter


//===== currently defined ===================================    
#define TRAFFIC_MON_ADDR_RNDSEED   	       	0x0204	//0x81 random seed fields
//==========================================================================
		#define TRAFFIC_MON_RNDSEED_DEF	      0x5EED	// DBG and upper 15 bits all 0's
//_________________________________________________________________________
//#define TRAFFIC_MON_ADDR_RNDSEED  0x0204	//0x81 random seed fields
// Structure:     	   bits
// RNDSEED_PAY_SIZ	    0:15		16-bit payload segment (sequence number is the remainder) field
// RNDSEED_FRCDLY_SIZ	  16		 1-bit force delay field size bits (for debugging)
// RNDSEED_SPARE_SIZ	17:31		15-bit - spare unused bits
//__________________________________________________________________
#define TRAFFIC_MON_DELAY_MIN 	0x0208	//0x82	packet delay minimum
#define TRAFFIC_MON_DELAY_MAX 	0x020C	//0x83	packet delay maximum
#define TRAFFIC_MON_DELAY_SUM0	0x0210	//0x84	packet delay sum LSB
#define TRAFFIC_MON_DELAY_SUM1	0x0214	//0x85	packet delay sum MSB
#define TRAFFIC_MON_DELAY_SUM2	0x0218  // packet delay sum MSB
//======= New addresses ======================================   
#define TRAFFIC_MON_HDR0		0x0404  //
#define TRAFFIC_MON_HDR1		0x0400  //
#define TRAFFIC_MON_HDR2		0x040C  //
#define TRAFFIC_MON_HDR3		0x0408  //
#define TRAFFIC_MON_HDR4		0x0414  //
#define TRAFFIC_MON_HDR5		0x0410  //
#define TRAFFIC_MON_HDR6		0x041C  //
#define TRAFFIC_MON_HDR7		0x0418  //
#define TRAFFIC_MON_HDR8		0x0424  //
#define TRAFFIC_MON_HDR9		0x0420  //
#define TRAFFIC_MON_HDR10		0x042C  //
#define TRAFFIC_MON_HDR11		0x0428  //
#define TRAFFIC_MON_HDR12		0x0434  //
#define TRAFFIC_MON_HDR13		0x0430  //
#define TRAFFIC_MON_HDR14		0x043C  //
#define TRAFFIC_MON_HDR15		0x0438  //
#define TRAFFIC_MON_HDR16		0x0444  //
#define TRAFFIC_MON_HDR17		0x0440  //
#define TRAFFIC_MON_HDR18		0x044C  //
#define TRAFFIC_MON_HDR19		0x0448  //
#define TRAFFIC_MON_HDR20		0x0454  //
#define TRAFFIC_MON_HDR21		0x0450  //
#define TRAFFIC_MON_HDR22		0x045C  //
#define TRAFFIC_MON_HDR23		0x0458  //
#define TRAFFIC_MON_HDR24		0x0464  //
#define TRAFFIC_MON_HDR25		0x0460  //
#define TRAFFIC_MON_HDR26		0x046C  //
#define TRAFFIC_MON_HDR27		0x0468  //
#define TRAFFIC_MON_HDR28		0x0474  //
#define TRAFFIC_MON_HDR29		0x0470  //
#define TRAFFIC_MON_HDR30		0x047C  //
#define TRAFFIC_MON_HDR31		0x0478  //
#define TRAFFIC_MON_HDR32		0x0484  //
#define TRAFFIC_MON_HDR33		0x0480  //
#define TRAFFIC_MON_HDR34		0x048C  //
#define TRAFFIC_MON_HDR35		0x0488  //
#define TRAFFIC_MON_HDR36		0x0494  //
#define TRAFFIC_MON_HDR37		0x0490  //
#define TRAFFIC_MON_HDR38		0x049C  //
#define TRAFFIC_MON_HDR39		0x0498  //
#define TRAFFIC_MON_HDR40		0x04A4  //
#define TRAFFIC_MON_HDR41		0x04A0  //
#define TRAFFIC_MON_HDR42		0x04AC  //
#define TRAFFIC_MON_HDR43		0x04A8  //
#define TRAFFIC_MON_HDR44		0x04B4  //
#define TRAFFIC_MON_HDR45		0x04B0  //
#define TRAFFIC_MON_HDR46		0x04BC  //
#define TRAFFIC_MON_HDR47		0x04B8  //
#define TRAFFIC_MON_HDR48		0x04C4  //
#define TRAFFIC_MON_HDR49		0x04C0  //
#define TRAFFIC_MON_HDR50		0x04CC  //
#define TRAFFIC_MON_HDR51		0x04C8  //
#define TRAFFIC_MON_HDR52		0x04D4  //
#define TRAFFIC_MON_HDR53		0x04D0  //
#define TRAFFIC_MON_HDR54		0x04DC  //
#define TRAFFIC_MON_HDR55		0x04D8  //
#define TRAFFIC_MON_HDR56		0x04E4  //
#define TRAFFIC_MON_HDR57		0x04E0  //
#define TRAFFIC_MON_HDR58		0x04EC  //
#define TRAFFIC_MON_HDR59		0x04E8  //
#define TRAFFIC_MON_HDR60		0x04F4  //
#define TRAFFIC_MON_HDR61		0x04F0  //
#define TRAFFIC_MON_HDR62		0x04FC  //
#define TRAFFIC_MON_HDR63		0x04F8  //

#define TRAFFIC_MON_MSK1		0x0600  //
#define TRAFFIC_MON_MSK0		0x0604  //
#define TRAFFIC_MON_MSK3		0x0608  //
#define TRAFFIC_MON_MSK2		0x060C  //
#define TRAFFIC_MON_MSK5		0x0610  //
#define TRAFFIC_MON_MSK4		0x0614  //
#define TRAFFIC_MON_MSK7		0x0618  //
#define TRAFFIC_MON_MSK6		0x061C  //
#define TRAFFIC_MON_MSK9		0x0620  //
#define TRAFFIC_MON_MSK8		0x0624  //
#define TRAFFIC_MON_MSK10		0x062C  //
#define TRAFFIC_MON_MSK11		0x0628  //
#define TRAFFIC_MON_MSK12		0x0634  //
#define TRAFFIC_MON_MSK13		0x0630  //
#define TRAFFIC_MON_MSK14		0x063C  //
#define TRAFFIC_MON_MSK15		0x0638  //
#define TRAFFIC_MON_MSK16		0x0644  //
#define TRAFFIC_MON_MSK17		0x0640  //
#define TRAFFIC_MON_MSK18		0x064C  //
#define TRAFFIC_MON_MSK19		0x0648  //
#define TRAFFIC_MON_MSK20		0x0654  //
#define TRAFFIC_MON_MSK21		0x0650  //
#define TRAFFIC_MON_MSK22		0x065C  //
#define TRAFFIC_MON_MSK23		0x0658  //
#define TRAFFIC_MON_MSK24		0x0664  //
#define TRAFFIC_MON_MSK25		0x0660  //
#define TRAFFIC_MON_MSK26		0x066C  //
#define TRAFFIC_MON_MSK27		0x0668  //
#define TRAFFIC_MON_MSK28		0x0674  //
#define TRAFFIC_MON_MSK29		0x0670  //
#define TRAFFIC_MON_MSK30		0x067C  //
#define TRAFFIC_MON_MSK31		0x0678  //
#define TRAFFIC_MON_MSK32		0x0684  //
#define TRAFFIC_MON_MSK33		0x0680  //
#define TRAFFIC_MON_MSK34		0x068C  //
#define TRAFFIC_MON_MSK35		0x0688  //
#define TRAFFIC_MON_MSK36		0x0694  //
#define TRAFFIC_MON_MSK37		0x0690  //
#define TRAFFIC_MON_MSK38		0x069C  //
#define TRAFFIC_MON_MSK39		0x0698  //
#define TRAFFIC_MON_MSK40		0x06A4  //
#define TRAFFIC_MON_MSK41		0x06A0  //
#define TRAFFIC_MON_MSK42		0x06AC  //
#define TRAFFIC_MON_MSK43		0x06A8  //
#define TRAFFIC_MON_MSK44		0x06B4  //
#define TRAFFIC_MON_MSK45		0x06B0  //
#define TRAFFIC_MON_MSK46		0x06BC  //
#define TRAFFIC_MON_MSK47		0x06B8  //
#define TRAFFIC_MON_MSK48		0x06C4  //
#define TRAFFIC_MON_MSK49		0x06C0  //
#define TRAFFIC_MON_MSK50		0x06CC  //
#define TRAFFIC_MON_MSK51		0x06C8  //
#define TRAFFIC_MON_MSK52		0x06D4  //
#define TRAFFIC_MON_MSK53		0x06D0  //
#define TRAFFIC_MON_MSK54		0x06DC  //
#define TRAFFIC_MON_MSK55		0x06D8  //
#define TRAFFIC_MON_MSK56		0x06E4  //
#define TRAFFIC_MON_MSK57		0x06E0  //
#define TRAFFIC_MON_MSK58		0x06EC  //
#define TRAFFIC_MON_MSK59		0x06E8  //
#define TRAFFIC_MON_MSK60		0x06F4  //
#define TRAFFIC_MON_MSK61		0x06F0  //
#define TRAFFIC_MON_MSK62		0x06FC  //
#define TRAFFIC_MON_MSK63		0x06F8  //

#endif /*HARDWARE_DEF_H_*/
