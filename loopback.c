/************************************************************************
*		LOOPBACK FUNCTIONALITY											*
*																		*
* Copyright (c) 2016 Greenlee Communications, CA USA.					*
* All rights reserved.													*
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "loopback.h"

//#define LOOP_UP 1
//#define LOOP_DOWN 0


void SEND_LOOPBACK_PACKET() {

	D(1, BUG("LOOPUP_PREV: %d\n", loopup_prev));
	D(1, BUG("LOOPDOWN_PREV: %d\n", loopdown_prev));
	D(1, BUG("!LOOPUP_PREV: %d\n", !loopup_prev));
	D(1, BUG("!LOOPDOWN_PREV: %d\n", !loopdown_prev));

	// Swapper identity table - dest mac
	int peer_mac = 0;
	if (!GIGE10) {
		peer_mac = (destination_tse_addr0 & 0xFF) << 8;
		peer_mac += (destination_tse_addr0 & 0xFF00) >> 8;
	}
	else
		peer_mac = destination_10g_addr1;
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC1_ptr, peer_mac);

	if (!GIGE10) {
		peer_mac = (destination_tse_addr0 & 0xFF0000) << 8;
		peer_mac += (destination_tse_addr0 & 0xFF000000) >> 8;
		peer_mac += (destination_tse_addr1 & 0xFF) << 8;
		peer_mac += (destination_tse_addr1 & 0xFF00) >> 8;
	}
	else
		peer_mac = destination_10g_addr0;
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC0_ptr, peer_mac);

	// Swapper identity table - src mac
	int self_mac = 0;
	if (!GIGE10) {
		self_mac = ConfigStat[MAC_ADDR_MSB00_ptr] << 8;
		self_mac += ConfigStat[MAC_ADDR_MSB01_ptr];
	}
	else
		self_mac = source_10g_addr1;
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_MAC1_ptr, self_mac);
	if (!GIGE10) {
		self_mac = ConfigStat[MAC_ADDR_MSB02_ptr] << 24;
		self_mac += ConfigStat[MAC_ADDR_MSB03_ptr] << 16;
		self_mac += ConfigStat[MAC_ADDR_LSB10_ptr] << 8;
		self_mac += ConfigStat[MAC_ADDR_LSB11_ptr];
	}
	else
		self_mac = source_10g_addr0;
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_MAC0_ptr, self_mac);

	unsigned int command = 0;

	// If VLAN tags are enabled, then write the tag info to the swapper
	if ( (RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02 ) { //SVLAN
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN2_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR4_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN2_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR4_1_ptr));
	}
	else if ( (RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01 ) { //VLAN
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
	}


	// Check to see what type of frame configuration you have to determine the type of request packet to send
	if ((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04) {		//ipv4
		// Write the IP addresses to the swapper if we're in IPv4
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_DEST3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));

		if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x00)) { 	//ipv4 no vlan
			command = 0x30000;
			D(1, BUG("SENDING IPV4 LOOPBACK PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04)  & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01)) { 	//ipv4 vlan
			command = 0xB0000;
			D(1, BUG("SENDING IPV4 VLAN LOOPBACK PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02)){ 	//ipv4 stacked vlan
			command = 0x1B0000;
			D(1, BUG("SENDING IPV4 STACKED VLAN LOOPBACK PACKET\n"));
		}
	}
	else if ((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x08) {	// IPv6
		// Write the IP addresses to the swapper if we're in IPv6
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP3_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_DEST3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP2_ptr, BytesToLong(RxBuffer,LOOPBACK_IP1_DEST3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP1_ptr, BytesToLong(RxBuffer,LOOPBACK_IP2_DEST3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP3_DEST3_ptr));

		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP3_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP2_ptr, BytesToLong(RxBuffer,LOOPBACK_IP1_SRC3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP1_ptr, BytesToLong(RxBuffer,LOOPBACK_IP2_SRC3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP3_SRC3_ptr));

		if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x08)& ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x00)) {		// Straight IP - no VLAN
			command = 0x70000;
			D(1, BUG("SENDING IPV6 LOOPBACK PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr]& 0x0C) == 0x08) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01)) {	// IPv6 + VLAN
			command = 0xF0000;
			D(1, BUG("SENDING IPV6 VLAN LOOPBACK PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02)) {
			command = 0x1F0000;
			D(1, BUG("SENDING IPV6 STACKED VLAN LOOPBACK PACKET\n"));
		}
	}
	else if ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01) { //vlan
		command = 0x90000;
		D(1, BUG("SENDING L2 VLAN LOOPBACK PACKET\n"));
	}
	else if ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02) { //stacked vlan
		command = 0x190000;
		D(1, BUG("SENDING L2 STACKED VLAN LOOPBACK PACKET\n"));
	}
	else {												// layer 2
		command = 0x10000;
		D(1, BUG("SENDING L2 LOOPBACK PACKET\n"));
	}

	// Check to see what button was toggled to determine if we're sending a loop-up or loop-down request
	// Set LOOK_FOR_RESPONSE so we can ensure we get the right response in our interrupt function
	if ( ((RxBuffer[CORE_SIGROUTE_MODES_ptr] & 0x04) >> 2) == !loopup_prev) {
		loopup_prev = !loopup_prev;

		// Set response to look for loop-up
		LOOK_FOR_RESPONSE = LOOPUP;

		// Set command
		command |= 0x10;
		D(1, BUG("LOOPUP\n"));
	}
	else if ( ((RxBuffer[CORE_SIGROUTE_MODES_ptr] & 0x08) >> 3) == !loopdown_prev) {
		loopdown_prev = !loopdown_prev;

		// Set response to look for loop-down
		LOOK_FOR_RESPONSE = LOOPDOWN;

		// Set command
		command |= 0x20;
		D(1, BUG("LOOPDOWN\n"));
	}

	D(1, BUG("COMMAND: %X\n", command));

	// Set mastership bit
	if (GIGE10)
		IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0x1);
	else
		IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0x1);

	if (GIGE10)
		IOWR_32DIRECT(SWAPPER64_10G_BASE, 0xC0, command);
	else
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0xC0, command);

	//while ( (IORD_32DIRECT(SWAPPER64_10G_BASE, 0xD0) & 0x1) );

	// Swapper identity table
//	offset = 0x100;
//	do {
//		D(1, BUG("%x: %x\n", offset/4, IORD_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, offset)));
//		offset += 4;
//	}
//	while (offset <= 0x1FC);

	//IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0, 0);
	//IOWR_8DIRECT(IRQ_MASK_BASE, 0, 0x30);
	requestTimer = 5000;		// Set our timeout duration to be 5 seconds before we stop looking for a response
}



void SEND_HELLO_PACKET() {
	// Broadcast Destination MAC
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC1_ptr, 0xFFFF);
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC0_ptr, 0xFFFFFFFF);

	// Swapper identity table - src mac
	int self_mac;
	if (!GIGE10) {
		self_mac = ConfigStat[MAC_ADDR_MSB00_ptr] << 8;
		self_mac += ConfigStat[MAC_ADDR_MSB01_ptr];
	}
	else
		self_mac = source_10g_addr1;
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_MAC1_ptr, self_mac);

	D(1, BUG("Flash: TSE MAC-A address from hello is: %04lX%04X\n",BytesToLong(ConfigStat,MAC_ADDR_MSB03_ptr),BytesToInt(ConfigStat,MAC_ADDR_LSB11_ptr)));

	if (!GIGE10) {
		self_mac = ConfigStat[MAC_ADDR_MSB02_ptr] << 24;
		self_mac += ConfigStat[MAC_ADDR_MSB03_ptr] << 16;
		self_mac += ConfigStat[MAC_ADDR_LSB10_ptr] << 8;
		self_mac += ConfigStat[MAC_ADDR_LSB11_ptr];

	}
	else
		self_mac = source_10g_addr0;
	IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_MAC0_ptr, self_mac);

	int swapper_addr = GIGE10 ? SWAPPER64_10G_BASE : TSE_ADDR_SWAPPER_A_BASE;
	int sniffer_addr = GIGE10 ? SNIFFER64_10GE_BASE : SNIFFER_TSE_A_BASE;

	IOWR_32DIRECT(sniffer_addr, 0, 0x1);		// set master bit
	LOOK_FOR_RESPONSE = HELLO;

	// If VLAN tags are enabled, then write the tag info to the swapper
	if ( (RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02 ) { //SVLAN
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN2_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR4_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN2_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR4_1_ptr));
	}
	else if ( (RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01 ) { //VLAN
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
		IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, BytesToInt(RxBuffer,TRAFFIC_HDR3_1_ptr));
	}

	// Check to see what type of frame configuration you have to determine the type of hello packet to send
	if ((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04) {		//ipv4
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, 0xFFFFFFFF);
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));

		if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x00)) { 	//ipv4 no vlan
			IOWR_32DIRECT(swapper_addr, 0xC0, 0x30008);
			D(1, BUG("SENDING IPV4 HELLO PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04)  & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01)) { 	//ipv4 vlan
			IOWR_32DIRECT(swapper_addr, 0xC0, 0xB0008);
			D(1, BUG("SENDING IPV4 VLAN HELLO PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x04)  & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02)) { 	//ipv4 stacked vlan
			IOWR_32DIRECT(swapper_addr, 0xC0, 0x1B0008);
			D(1, BUG("SENDING IPV4 STACKED VLAN HELLO PACKET\n"));
		}
	}
	else if ((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x08) {	// IPv6
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP3_ptr, 0xFFFFFFFF);
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP2_ptr, 0xFFFFFFFF);
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP1_ptr, 0xFFFFFFFF);
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, 0xFFFFFFFF);

		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP3_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP2_ptr, BytesToLong(RxBuffer,LOOPBACK_IP1_SRC3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP1_ptr, BytesToLong(RxBuffer,LOOPBACK_IP2_SRC3_ptr));
		IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP3_SRC3_ptr));

		if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x08)& ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x00)) {		// Straight IP - no VLAN
			IOWR_32DIRECT(swapper_addr, 0xC0, 0x70008);
			D(1, BUG("SENDING IPV6 HELLO PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr]& 0x0C) == 0x08) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01)) {	// IPv6 + VLAN
			IOWR_32DIRECT(swapper_addr, 0xC0, 0xF0008);
			D(1, BUG("SENDING IPV6 VLAN HELLO PACKET\n"));
		}
		else if (((RxBuffer[EGEN_PKT_TYPE_ptr] & 0x0C) == 0x08) & ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02)) {
			IOWR_32DIRECT(swapper_addr, 0xC0, 0x1F0008);
			D(1, BUG("SENDING IPV6 STACKED VLAN HELLO PACKET\n"));
		}
	}
	else if ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x01) { //vlan
		IOWR_32DIRECT(swapper_addr, 0xC0, 0x90008);
		D(1, BUG("SENDING L2 VLAN HELLO PACKET\n"));
	}
	else if ((RxBuffer[EGEN_PKT_TYPE_ptr_1] & 0x03) == 0x02) { //stacked vlan
		IOWR_32DIRECT(swapper_addr, 0xC0, 0x190008);
		D(1, BUG("SENDING L2 STACKED VLAN HELLO PACKET\n"));
	}
	else {												// layer 2
		IOWR_32DIRECT(swapper_addr, 0xC0, 0x10008);
		D(1, BUG("SENDING L2 HELLO PACKET\n"));
	}

	//OSTimeDlyHMSM(0, 0, 1, 0);

	D(1, BUG("SWAPPER REGISTER 0: %d\n", IORD_16DIRECT(swapper_addr, 0)));
	//IOWR_16DIRECT(swapper_addr, 0, 0);
	//IOWR_8DIRECT(IRQ_MASK_BASE, 0, 0x20);
	requestTimer = 5000;
}

void ENTER_LOOPBACK(_Bool swapIP) {

	if (LOOPBACK_EVENT == LOOPUP10G) {
		SET_10G_TX_TO_NW_LOOPBACK;		// Enable LPBK
		SET_10G_RX_3SPLITTER_TO_10G_TX;
		D(1, BUG("10GA:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_10GE_BASE, ST_MUX_REG)));
		D(1, BUG("10GA:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_0_BASE, ST_SPLITTER_REG)));
		ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |=0x20;

		SET_SWAP_10GA_TOGGLE |= 0x08; 	// also tell swapper we're in lpbk
		SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
		D(1, BUG("10A:SET_SWAP_10GA_TOGGLE STATE - LPBK ON: 0x%02X\n",SET_SWAP_10GA_TOGGLE));
		D(1, BUG("10A:ADDR_SWAPPER STATE - LPBK ON: 0x%04X\n", IORD_16DIRECT(SWAPPER64_10G_BASE, TSE_SWAPPER_REG)));

		D(1, BUG("\nSetting PHY TxClk to FTX_LTIME_EN...(use Recovered Clock)\n"));
		write_ext_10G_phy(0xC001, 1, (read_ext_10G_phy(0xC001, 1))|0x0200); // set FTX_LTIME_EN ON - to use Recovered Clock as Tx Clock

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

		// SWAP MAC
		SET_SWAP_10GA_TOGGLE |= 0x01;
		SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
		ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |= 0x40;

		// SWAP IP
		if (swapIP) {
			SET_SWAP_10GA_TOGGLE |= 0x02;
			SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
			ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |= 0x80;
		}
	}
	else {		// TSE
		if (LOOPBACK_EVENT == LOOPUPTSEB) {	// Port B
			D(1, BUG("\nEnable LPBK\n"));
			//SET_TSE_B_TX_TO_NW_LOOPBACK;	// Enable LPBK
			SET_TSE_B_LOOPBACK;
			D(1, BUG("B:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));
			D(1, BUG("B:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x20;

			D(1, BUG("\n enable Swap mac\n"));
			SET_SWAP_B_TOGGLE |= 0x01;
			SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
			D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x40;

			if (swapIP) {
				D(1, BUG("\n swap IP\n"));
				SET_SWAP_B_TOGGLE |= 0x02;
				D(1, BUG("B:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_B_TOGGLE));
				SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
				D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
				ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x80;
			}
		}
		else {				// Port A
			SET_TSE_A_LOOPBACK;
			D(1, BUG("A:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_1GE_A_BASE, ST_MUX_REG)));
			D(1, BUG("A:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_SPLITTER_REG)));
			ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x20;

			// SWAP MAC
			SET_SWAP_A_TOGGLE |= 0x01;
			D(1, BUG("\nEnable swap mac\n"));
			D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
			SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
			D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
			ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x40;

			if (swapIP) {
				D(1, BUG("\n swap IP\n"));
				SET_SWAP_A_TOGGLE |= 0x02;
				D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
				SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
				D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
				ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x80;
			}
		}
	}

}

void EXIT_LOOPBACK() {

	if (LOOPBACK_EVENT == LOOPDOWN10G) {
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

		D(1, BUG("\nSetting PHY TxClk to FPLL_OUT...(use Local OSC, FPLL_OUT)\n"));
		write_ext_10G_phy(0xC001, 1, (read_ext_10G_phy(0xC001, 1))&0xFDFF); // set FTX_LTIME_EN OFF - to use local OSC as Tx Clock

		set_reset_10GMAC_FIFOS();
		if (SFP_10G_PRESENT) {
			SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;
			D(1, BUG("SWITCHING TO TXFPLL\n"));

//		else {
//			SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD;
//			D(1, BUG("SWITCHING TO RCVRD\n"));
//		}
			usleep(1);
			clear_reset_10GMAC_FIFOS();
			init_10G_MAC();
			init_10G_SC_FIFO(ETH_10G_SCFIFO_CUT_THRU);		//init & set RxFIFO to Cut-thru mode for NORMAL mode
			int reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
			reg_val |= 0x01;  // Activate Tx Digital Reset
			IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT, reg_val);
			init_10G_xaui(); // no need when in auto-CDR lock it's back everytime!
		}

		SET_SWAP_10GA_TOGGLE &= ~0x02;						// Turn off swap IP
		SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
		ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &= ~0x80;

		SET_SWAP_10GA_TOGGLE &= ~0x01;						// Turn off swap MAC
		SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
		ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &= ~0x40;
	}
	else {
		if (LOOPBACK_EVENT == LOOPDOWNTSEB) {
			D(1, BUG("\nDisable LPBK\n"));
			SET_TSE_B_TX_3MUX_TO_NO_OUTPUT; // 'disable' LPBK
			//splitter stays at 1
			D(1, BUG("B:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(TSE_B_TX_3MUX, ST_MUX_REG)));
			D(1, BUG("B:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(TSE_B_RX_3SPLITTER, ST_SPLITTER_REG)));
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x20;

			D(1, BUG("\n disable swap mac \n"));
			SET_SWAP_B_TOGGLE &= ~0x01;
			SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
			D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x40;

			SET_SWAP_B_TOGGLE &= ~0x02;
			D(1, BUG("B:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_B_TOGGLE));
			SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
			D(1, BUG("\n disable swap IP\n"));
			D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE, TSE_SWAPPER_REG)));
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
		}
		else {
			D(1, BUG("\nDisable LPBK\n"));
			SET_TSE_A_NORMAL_TEST_MODE;//disable LPBK - MUX TO EGEN

			D(1, BUG("A:MUX STATE: 0x%04X\n (3=disabled,0=enabled)", IORD_16DIRECT(MUX31_ST_1GE_A_BASE, ST_MUX_REG)));
			D(1, BUG("A:SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_SPLITTER_REG)));
			ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x20;

			D(1, BUG("\nDisable swap mac\n"));		// Turn off swap MAC
			SET_SWAP_A_TOGGLE &= ~0x01;
			D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
			SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
			D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
			ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x40;

			SET_SWAP_A_TOGGLE &= ~0x02;				// Turn off swap IP
			D(1, BUG("\n disable swap IP\n"));
			D(1, BUG("A:SET_SWAP_A_TOGGLE  STATE: 0x%02X\n",SET_SWAP_A_TOGGLE));
			SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
			D(1, BUG("A:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, TSE_SWAPPER_REG)));
			ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
		}
	}

}

// called in main() expects 100mS intervals
void loopback_event_processing()
{
	if (LOOPBACK_EVENT == LOOPUPTSEA || LOOPBACK_EVENT == LOOPUPTSEB || LOOPBACK_EVENT == LOOPUP10G) {
		ENTER_LOOPBACK(SWAP_IP);
		LOOPBACK_EVENT = 0;
		SWAP_IP = NO;
	}
	else if (LOOPBACK_EVENT == LOOPDOWNTSEA || LOOPBACK_EVENT == LOOPDOWNTSEB || LOOPBACK_EVENT == LOOPDOWN10G){
		//D(1, BUG("Loopback Event: %d\n", LOOPBACK_EVENT));
		EXIT_LOOPBACK();
		LOOPBACK_EVENT = 0;
	}

	/*unsigned short new_clk = IORD_8DIRECT(CLK_MUXER_10GE_LOOP_BASE, 0X0);
	if ( new_clk != old_clk ) {
		D(1, BUG("clkmux: %x\n", new_clk));
		D(1, BUG("clk_muxer_ingress: %x\n", IORD_8DIRECT(CLK_MUXER_INGRESS_BASE, 0X0)));
		D(1, BUG("clk_muxer_egress: %x\n", IORD_8DIRECT(CLK_MUXER_EGRESS_BASE, 0X0)));
		old_clk = new_clk;
	}*/

	if (LOOK_FOR_RESPONSE)
	{
		requestTimer -= 100;
	}

	// 5 second timeout after sending a request packet
	if (LOOK_FOR_RESPONSE && requestTimer <= 0) {
		if (responseUnits) {
			if (GIGE10) {
				SEND_LPBK10G_DATA = YES;
				int t;
				for (t = 2; t < 40; t++) {
					D(1, BUG("%x\n", LoopbackStat10G[t]));
					if (t % 4 == 1) D(1, BUG("\n"));
				}
				IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0);		//clear master
			}
			else {
				SEND_LPBK_DATA = YES;
				IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0);		//clear master
			}
			LOOK_FOR_RESPONSE = 0;
			responseUnits = 0;
			loopbackStatPtr = 2;
		}
		else {
			LOOK_FOR_RESPONSE = 0;
			D(1, BUG("TIMED OUT\tRequest Timer: %d\n", requestTimer));
			// Display timed out message? depending on what we sent out
			if (GIGE10) {
				ConfigStat10G[LOOPBACKSTAT_ptr] = TIMEOUT;
				// Clear mastership bit
				IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0);
			}
			else {
				ConfigStat[LOOPBACKSTAT_ptr] = TIMEOUT;
				// Clear mastership bit
				IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0);
			}

		}
	}
}
