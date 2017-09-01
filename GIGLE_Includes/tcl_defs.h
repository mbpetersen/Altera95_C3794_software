
//# =================================================================
//# TSE0 MAC Configuration Setting
//# =================================================================
#define MAC_SCRATCH		0xaaaaaaaa
//# COMMAND_CONFIG
#define ENA_TX 				1	//#0 LSB
#define ENA_RX 				1
#define XON_GEN 			0
//#define ETH_SPEED_1G 		0	//#3 10/100Mbps = 0 & 1000Mbps = 1
#define PROMIS_EN			1	//#4 Promiscuous mode
#define PAD_EN 				0
#define CRC_FWD 			0
#define PAUSE_FWD 			0
//#define PAUSE_IGNORE		0	//#8 do NOT ignore rcvd pause frames
#define TX_ADDR_INS 		0	// overwrite MAC addr from User with primary or supplementary MAC (via TX_ADDR_SEL)
//#define HD_ENA 			0	//#10 Enable HDX (ignored if 1000 eth_speed=1)
#define EXCESS_COL 			0	// set when MAC discards a frame after detecting a collision on 16 consecutive frame retransmissions (clrd by reset)
#define LATE_COL			0	// set when MAC detects a collision after transmitting 64 bytes and discards the frame
#define SW_RESET			0
#define MHASH_SEL			0
//#define LOOP_ENA			0	//#0; 1=loop MAC Tx data back to MAC Rx
#define TX_ADDR_SEL 		0x0	// 000=0, 100=4, 101=5, 110=6, 111=7)
#define MAGIC_ENA			0
#define SLEEP	 			0
#define WAKEUP 				0
#define XOFF_GEN 			0
#define CTRL_FRM_ENA 		0	// Set to 1 to accept control frames other than pause frames (opcode = 0x0001) and forward them to the user application
#define NO_LGTH_CHECK		0	// Set to 0 to check the actual payload length of receive frames against the length/type field in receive frames
//#define ENA_10 				0 	//# 100Mbps = 0 & 10Mbps = 1
#define RX_ERR_DISC 		0	// Set to 0 to forward erroneous frames to the user application with rx_err[0] asserted
#define DISABLE_RD_TIMEOUT	0 	// rsrvd - don't fiddle with it!
#define CNT_RESET			0	// MSB

// MAC Addr hardcoded for each unit........
//relocated to vars.h
//#define MAC_0 				0x22334450	// MAC: 50-44-33-22-11-EE
//#define MAC_1 				0x0000EE11
//currently copied in var.s into source_tse_addr0/1 for EGEN use.
//#define FRM_LENGTH  		1518	//# - smallest 64(and egen/emon limit)? max 9600;
#define PAUSE_QUANT  		1000	// used with XOFF gen, NOTE: a pause quanta of 8 = 4,096ns so 1=512nS
									// - the length of time the remote end should pause when we send an XOFF!
//#define RX_SECTION_EMPTY 	496 //was 65520  4096-16=4080 (FIFO depth-16)
#define RX_SECTION_EMPTY 	TRIPLE_SPEED_ETHERNET_0_RECEIVE_FIFO_DEPTH-16 //496 //was 65520  4096-16=4080 (FIFO depth-16)
#define RX_SECTION_FULL  	16	//16: 16x32bits = 16*4byte=64bytes ** WHEN SET to 0 is store & forward ELSE "cut-through"
#define RX_SECTION_FULL_CUT 16	// cut-through (normal testing)
#define RX_SECTION_FULL_SAF 0	// store & forward (LAN-WAN mode)
//#define TX_SECTION_EMPTY 	496 //was 65520 (FIFO depth-16)
#define TX_SECTION_EMPTY 	TRIPLE_SPEED_ETHERNET_0_TRANSMIT_FIFO_DEPTH-16 //496 //was 65520 (FIFO depth-16)
#define TX_SECTION_FULL  	16
#define RX_ALMOST_EMPTY 	8
#define RX_ALMOST_FULL  	8
#define TX_ALMOST_EMPTY  	8
#define TX_ALMOST_FULL 		25
#define MDIO_ADDR0  		0
#define MDIO_ADDR1  		0

#define TX_IPG_LENGTH 		12

#define TX_OMIT_CRC 		0

#define TX_SHIFT16 			0
#define RX_SHIFT16 			0

// Combine Configuration Value MAC
#define COMMAND_CONFIG_VALUE (0 \
		| (ENA_TX << 0) \
		| (ENA_RX << 1) \
		| (XON_GEN << 2) \
		| (ETH_SPEED_1G << 3) \
		| (PROMIS_EN << 4) \
		| (PAD_EN << 5) \
		| (CRC_FWD << 6) \
		| (PAUSE_FWD << 7) \
		| (PAUSE_IGNORE	<< 8) \
		| (TX_ADDR_INS << 9) \
		| (HD_ENA << 10) \
		| (EXCESS_COL << 11) \
		| (LATE_COL	<< 12) \
		| (SW_RESET << 13) \
		| (MHASH_SEL << 14) \
		| (LOOP_ENA	<< 15) \
		| (TX_ADDR_SEL << 18) \
		| (MAGIC_ENA << 19) \
		| (SLEEP << 20) \
		| (WAKEUP << 21) \
		| (XOFF_GEN << 22) \
		| (CTRL_FRM_ENA << 23) \
		| (NO_LGTH_CHECK << 24) \
		| (ENA_10 << 25) \
		| (RX_ERR_DISC << 26) \
		| (DISABLE_RD_TIMEOUT << 27) \
		| (CNT_RESET << 31) \
		)

#define TX_CMD_STAT_VALUE (0x00 | (TX_OMIT_CRC << 17) | (TX_SHIFT16 << 18))
#define RX_CMD_STAT_VALUE (0x00 | (RX_SHIFT16 << 25))

//=======================================================================================================
//=======================================================================================================
//# ====================================================================
//# TSE1 MAC Configuration Setting
//# ====================================================================
#define MAC_SCRATCHB		0xaaaaaaaa
//# COMMAND_CONFIG
#define ENA_TXB 			1
#define ENA_RXB 			1
#define XON_GENB 			0
//#define ETH_SPEEDB 			0	//# 10/100Mbps = 0 & 1000Mbps = 1
#define PROMIS_ENB			1
#define PAD_ENB 			0
#define CRC_FWDB 			0
#define PAUSE_FWDB 			0
#define PAUSE_IGNOREB		0
#define TX_ADDR_INSB 		0
//#define HD_ENAB 			0
#define EXCESS_COLB 		0
#define LATE_COLB			0
#define SW_RESETB			0
#define MHASH_SELB			0
//#define LOOP_ENAB			0	//#0; 1=loop MAC Tx data back to MAC Rx
#define TX_ADDR_SELB 		0x0
#define MAGIC_ENAB			0
#define SLEEPB	 			0
#define WAKEUPB 			0
#define XOFF_GENB 			0
#define CTRL_FRM_ENAB 		0
#define NO_LGTH_CHECKB		0
//#define ENA_10B 			0 //# 100Mbps = 0 & 10Mbps = 1
#define RX_ERR_DISCB 		0
#define DISABLE_RD_TIMEOUTB	0 // rsrvd - don't fiddle with it!
#define CNT_RESETB			0

// MAC Addr hardcoded for each unit (here, for now)........
//relocated to vars.h
//#define MAC_0B 				0x33445560	// MAC: 60-55-44-33-11-BB
//#define MAC_1B 				0x0000BB11
//#define FRM_LENGTHB  		1518	//# max 9600;
#define PAUSE_QUANTB  		1000
//#define RX_SECTION_EMPTYB 	496	//48 // 64-16 = 48
#define RX_SECTION_EMPTYB 	TRIPLE_SPEED_ETHERNET_1_RECEIVE_FIFO_DEPTH-16 //496 //was 65520  4096-16=4080 (FIFO depth-16)
#define RX_SECTION_FULLB	  0
#define RX_SECTION_FULLB_CUT  16	// cut-through (normal testing & REQUIRED for JUMBO)
#define RX_SECTION_FULLB_SAF  0		// store & forward (LAN-WAN mode)
//#define TX_SECTION_EMPTYB 	496	//48
#define TX_SECTION_EMPTYB 	TRIPLE_SPEED_ETHERNET_1_TRANSMIT_FIFO_DEPTH-16 //496 //was 65520 (FIFO depth-16)
#define TX_SECTION_FULLB  	16
#define RX_ALMOST_EMPTYB 	8
#define RX_ALMOST_FULLB  	8
#define TX_ALMOST_EMPTYB  	8
#define TX_ALMOST_FULLB 	3
#define MDIO_ADDR0B  		0
#define MDIO_ADDR1B  		0

#define TX_IPG_LENGTHB 		12

#define TX_OMIT_CRCB 		0

#define TX_SHIFT16B 		0
#define RX_SHIFT16B 		0

#define MAC_CLEAR_COUNTERS	0x8000

// Combine Configuration Value MAC
#define COMMAND_CONFIG_VALUEB (0 \
		| (ENA_TXB << 0) \
		| (ENA_RXB << 1) \
		| (XON_GENB << 2) \
		| (ETH_SPEEDB << 3) \
		| (PROMIS_ENB << 4) \
		| (PAD_ENB << 5) \
		| (CRC_FWDB << 6) \
		| (PAUSE_FWDB << 7) \
		| (PAUSE_IGNOREB	<< 8) \
		| (TX_ADDR_INSB << 9) \
		| (HD_ENAB << 10) \
		| (EXCESS_COLB << 11) \
		| (LATE_COLB	<< 12) \
		| (SW_RESETB << 13) \
		| (MHASH_SELB << 14) \
		| (LOOP_ENAB	<< 15) \
		| (TX_ADDR_SELB << 18) \
		| (MAGIC_ENAB << 19) \
		| (SLEEPB << 20) \
		| (WAKEUPB << 21) \
		| (XOFF_GENB << 22) \
		| (CTRL_FRM_ENAB << 23) \
		| (NO_LGTH_CHECKB << 24) \
		| (ENA_10B << 25) \
		| (RX_ERR_DISCB << 26) \
		| (DISABLE_RD_TIMEOUTB << 27) \
		| (CNT_RESETB << 31) \
		)

#define TX_CMD_STAT_VALUEB (0x00 | (TX_OMIT_CRCB << 17) | (TX_SHIFT16B << 18))
#define RX_CMD_STAT_VALUEB (0x00 | (RX_SHIFT16B << 25))


#define CLR_MAC0_COUNTERS	IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_0_BASE,COMMAND_CONFIG_VALUE|MAC_CLEAR_COUNTERS)
#define CLR_MAC1_COUNTERS	IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_1_BASE,COMMAND_CONFIG_VALUEB|MAC_CLEAR_COUNTERS)
#define RESET_MAC0_COUNTERS	IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_0_BASE,COMMAND_CONFIG_VALUE|ALTERA_TSEMAC_CMD_SW_RESET_MSK)
#define RESET_MAC1_COUNTERS	IOWR_ALTERA_TSEMAC_CMD_CONFIG(TRIPLE_SPEED_ETHERNET_1_BASE,COMMAND_CONFIG_VALUEB|ALTERA_TSEMAC_CMD_SW_RESET_MSK)


//=======================================================================================================
// PHY Variables
#define PHYA_ADDR 			0x01	//# PHY port-A Address  (was 0)
#define PHYB_ADDR 			0x02	//# PHY port-B Address (addr for A+1)
//========================================
// PHY Configuration Setting
//========================================
#define PHYA_ENABLE 		1		//# Enable PHY Port 0
#define PHYB_ENABLE 		1		//# Enable PHY Port 1

//========================================
// PHY loopbacks.................
#define PHY_LOOPBACK		0
#define PHYA_LOOPBACK		0		//# Enable PHY-A Loopback (self-loop) THESE HAVE NEVER WORKED! ???
#define PHYB_LOOPBACK		0		//# Enable PHY-B Loopback (self-loop) ---> THEY only work at 1gig copper/fiber
// Lineside (loopback NW)
#define PHYA_LINESIDE_LOOPBACK		0	//# Enable PHY-A Lineside Loopback (NW-loop)
#define PHYB_LINESIDE_LOOPBACK		0	//# Enable PHY-B Lineside Loopback (NW-loop)
// PHY Enable External Loopback
#define PHY_EXT_LPBK_MODE			0
#define PHYA_EXT_LPBK_MODE			0	// When in 1000/100* speed mode: 1= TURN ON when using an EXTERNAL loopback
#define PHYB_EXT_LPBK_MODE			0	// When in 1000/100* speed mode: 1= TURN ON when using an EXTERNAL loopback

//=== for BOTH PHY's (generic_phy_config(base)..........
//#define PHY_ETH_SPEED 		100
//#define PHY_ENABLE_AN 		1
//#define PHY_FIBER_ENABLE_AN 1
//#define PHY_DUPLEX			1
//=== PHY-A Configuration
////#define PHYA_ETH_SPEED 		100 //100	//# 10Mbps or 100Mbps or 1000Mbps <<<< Also used by set_TxClkX_speed()
//#define PHYA_COPPER_ENABLE_AN 1		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
//#define PHYA_FIBER_ENABLE_AN 1		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
//#define PHYA_COPPER_FULLDUPLEX	1	//# FDX = 1 and HDX = 0
////#define PHYA_FIBER_DUPLEX	1		//# FDX = 1 and HDX = 0
//==== PHY-B Configuration
////#define PHYB_ETH_SPEED 		100	//100 //# 10Mbps or 100Mbps or 1000Mbps <<<< Also used by set_TxClkX_speed()
//#define PHYB_COPPER_ENABLE_AN 1		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
//#define PHYB_FIBER_ENABLE_AN 1		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
//#define PHYB_COPPER_FULLDUPLEX	1	//# FDX = 1 and HDX = 0
////#define PHYB_FIBER_DUPLEX	1		//# FDX = 1 and HDX = 0


//========================================
// PHY-A RGMII TIMING
#define PHYA_RGMII_TX_TIMING_CTRL_ON 	1	//# Enable RGMII TX Timing Control
#define PHYA_RGMII_RX_TIMING_CTRL_ON 	0	//# Enable RGMII RX Timing Control
// PHY-B RGMII TIMING
#define PHYB_RGMII_TX_TIMING_CTRL_ON 	1	//# Enable RGMII TX Timing Control
#define PHYB_RGMII_RX_TIMING_CTRL_ON 	0	//# Enable RGMII RX Timing Control
//======== BOTH A & B
#define PHY_RGMII_TX_TIMING_CTRL_ON 	1	//# Enable RGMII TX Timing Control
#define PHY_RGMII_RX_TIMING_CTRL_ON 	0	//# Enable RGMII RX Timing Control

//=======================================================================================================
//=======================================================================================================
#define PREAMBLE_SFD		8
#define FRAME_OVERHEADA		(PREAMBLE_SFD+TX_IPG_LENGTH)
#define FRAME_OVERHEADB		(PREAMBLE_SFD+TX_IPG_LENGTHB)
//======
#define ACTUAL_CPU_FREQ 83333333.3
#define LATENCY_CPU_FREQ 83.3333333	// for result in uS
#define JITTER_CPU_FREQ .0833333333	// for result in nS
//========================================================
//   Ethernet Generator Configuration Setting
//========================================================
//#define number_packet_lsb		100000		//# Number of packets (1Mframes@1G=~18secs)  4250700300
//#define eth_gen 			1				//# Off = 0 & On = 1
//#define length_sel 		0				//# Fixed = 0	& Random = 1
//
//#define pkt_length 			1518			//# Ranges from 24 to 9600  ( 46 & 3000 doesn't work!) ???
//	A fixed length ranges between 24 to 9,600 bytes; a random length ranges between 24 and 1,518.
//
// The Triple-Speed Ethernet IP core pads undersized packets to meet the minimum required length, 64 bytes.
#define pattern_sel 		1				//# Incremental = 0 & Random = 1 (Incremental starts at zero and increments by one)
//#define rand_seed 			0x123456789ABC	//# random seed
#define rand_seed0 			0x56789ABC		//# random seed
#define rand_seed1 			0x1234			//# random seed
//#define source_addr 		0xEE1122334450	//# MAC destination address
//#define source_tse_addr0 		0x22334450		//# MAC destination address
//#define source_tse_addr1 		0xEE11			//# MAC destination address
//#define destination_addr 	0xEE1122334450	//# MAC source address
//#define destination_tse_addr0 	0x22334450		//# MAC source address
//#define destination_tse_addr1 	0xEE11			//# MAC source address
