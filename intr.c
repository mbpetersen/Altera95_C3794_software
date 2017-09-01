/*************************************************************************
*		Interrupt Service Routines										 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "loopback.h"

void handle_aggr_ISR(void *context)
{
	int i;
//	D(1, BUG("ENTER TIME: %ld\n", OSTimeGet()));

//	// IDENTITY TABLE
//	int offset = 0x100;
//	do {
//		D(1, BUG("%x: %x\n", offset/4, IORD_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, offset)));
//		offset += 4;
//	}
//	while (offset <= 0x1FC);
	if ( (IORD_8DIRECT(IRQ_AGGREGATOR_BASE, 0) & 0x10) == 0x10 ) {
			D(1, BUG("In A\n"));

			// While buckets are not empty
			unsigned short bucketIndex = 0;
			D(1, BUG("SNIFFER: %X\n", IORD_16DIRECT(SNIFFER_TSE_A_BASE, SNIFFER_ISR_REGISTER)));

			//loopbackStatPtr = 2;
			/**
			 *	Main while loop for TSE-A sniffer interrupts.
			 *	1. Check for the next filled bucket entry in the sniffer
			 *	2. Check to see if we're currently a master (sent out a request packet)
			 *		i. Check to see we have received a response packet and it's the right type (loop up/down, query)
			 *			a. If loop-up/down, then set LOOPBACKSTAT_ptr for UI and reset mastership
			 *			b. If hello, copy bucket entry into LoopbackStat
			 *		ii. If not a response packet, ignore and clear bucket entry
			 *	3. Check if see if detect remote loopback is off or if a IPv4/6 packet sent when mode is off
			 *		i. If true, ignore packet and clear bucket entry
			 *	4. If applicable, copy VLAN and IP info to swapper table for peer and self
			 *	5. Set appropriate command mode bits according to the type of request packet received
			 *		i. If loop-up request, turn on loopback and swap MAC as well as swap IP if an IP request packet
			 *		ii. If loop-down request, turn off loopback and all swappers
			 *	6. Write command to swapper to send response packet
			 *	7. Clear bucket entry
			 */
			while ( (IORD_16DIRECT(SNIFFER_TSE_A_BASE, 0x10) != 0) ) {
				D(1, BUG("while loop\n"));

				// ^6 set if entries are full; clear and continue processing the valid entries
				if ((IORD_16DIRECT(SNIFFER_TSE_A_BASE, 0x10) & 0x40) == 0x40) {
					IOWR_16DIRECT(SNIFFER_TSE_A_BASE, 0x10, 0x40);
					continue;
				}

				D(1, BUG("SNIFFER: %X\n", IORD_16DIRECT(SNIFFER_TSE_A_BASE, 0x10)));

				D(1, BUG("RxBuffer inside A: %x\n", RxBuffer[EGEN_PKT_TYPE_ptr]));

				// Go to the next filled bucket
				if ( !(IORD_16DIRECT(SNIFFER_TSE_A_BASE, SNIFFER_ISR_REGISTER) & (1 << bucketIndex)) ) {
					++bucketIndex;
					if ( bucketIndex == 6 )
						bucketIndex = 0;
					D(1, BUG("bucketIndex (empty) : %d\n", bucketIndex));
				}

//				int z = 0x100;
//				for (z = 0x100; z <= 0x1FC; ) {
//					if ((z - 0x100) % 0x28 == 0)
//						D(1, BUG("\n"));
//					D(1, BUG("%x: %x\n", z/4, IORD_32DIRECT(SNIFFER_TSE_A_BASE, z)));
//					z += 4;
//				}

				//IOWR_16DIRECT(SNIFFER_TSE_A_BASE, 0x10, 1 << bucketIndex);	//Clear entry in Sniffer ISR

				int entryBase = IDENTITY_TABLE_BASE + (BUCKET_ENTRY_SIZE * bucketIndex);
				int entryVLANBase = SWPR_IDTABLE_VLAN_BASE + (BUCKET_ENTRY_SIZE * bucketIndex);
				unsigned int oneHot = IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase) >> 16;
				D(1, BUG("bucketIndex : %d\n", bucketIndex));

				// If master, we're only looking for response packets
				int master = IORD_8DIRECT(SNIFFER_TSE_A_BASE, 0);
				D(1, BUG("master: %d\n", master));
				D(1, BUG("look for response: %d\n", LOOK_FOR_RESPONSE));
				if (master) {

					int packet_type = oneHot & ONEHOT_PKTTYPE_MASK;
					D(1, BUG("PACKET TYPE: %x\n", packet_type));

					// Check for a response packet if we're master
					if (LOOK_FOR_RESPONSE && packet_type > 0x04) {
						if (LOOK_FOR_RESPONSE == HELLO && packet_type == HELLO_RESPONSE) {
							D(1, BUG("HELLO\n"));
							// Load unit info into LoopbackStat to display
							int offset = entryBase;
							int i;
							for (i = 0; i < 0x28; i += 4) {
								SaveBytesLong(LoopbackStat, loopbackStatPtr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, offset));
								offset += 4;
								loopbackStatPtr += 4;
							}
							responseUnits++;
							D(1, BUG("response units: %d\n", responseUnits));
							if (responseUnits == 6)
								break;
						}
						else if (LOOK_FOR_RESPONSE == LOOPUP && packet_type == LOOPUP_RESPONSE) {
							D(1, BUG("SLAVE LOOPED UP\n"));
							// Display looped-up unit
							if ( (oneHot & ONEHOT_FLAG_MASK) == 0x8000 ) {
								D(1, BUG("SLAVE BUSY\n"));
								ConfigStat[LOOPBACKSTAT_ptr] = REMOTE_BUSY;
							}
							else if ( (oneHot & ONEHOT_FLAG_MASK) == 0x4000 ) {
								D(1, BUG("SLAVE ALREADY UP\n"));
								ConfigStat[LOOPBACKSTAT_ptr] = REMOTE_ALREADY_UP;
							}
							else
								ConfigStat[LOOPBACKSTAT_ptr] = REMOTE_LOOPED_UP;
							LOOK_FOR_RESPONSE = 0;
							IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0);
						}
						else if (LOOK_FOR_RESPONSE == LOOPDOWN && packet_type == LOOPDOWN_RESPONSE) {
							D(1, BUG("SLAVE LOOPED DOWN\n"));
							// Display looped-down unit
							if ( (oneHot & ONEHOT_FLAG_MASK) == 0x8000 ) {
								D(1, BUG("SLAVE BUSY\n"));
								ConfigStat[LOOPBACKSTAT_ptr] = REMOTE_BUSY;
							}
							else if ( (oneHot & ONEHOT_FLAG_MASK) == 0x4000 ) {
								D(1, BUG("SLAVE ALREADY DOWN\n"));
								ConfigStat[LOOPBACKSTAT_ptr] = REMOTE_ALREADY_DOWN;
							}
							else
								ConfigStat[LOOPBACKSTAT_ptr] = REMOTE_LOOPED_DOWN;
							LOOK_FOR_RESPONSE = 0;
							IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0);
						}
						else {
							// didn't get a response packet
							// do we get in here? erroneous packet
						}

						D(1, BUG("MASTER LOOPBACKSTAT_PTR: %X\n", ConfigStat[LOOPBACKSTAT_ptr]));
						SaveBytesLong(ConfigStat, LOOPBACKSTAT_ptr23, 0);	//clear the array
						SaveBytesLong(ConfigStat, LOOPBACKSTAT_ptr13, 0);
						SaveBytesLong(ConfigStat, LOOPBACKSTAT_ptr03, 0);
						// Copy text id field to ConfigStat for UI to display unit name
						int offset = 0x11C + (BUCKET_ENTRY_SIZE * bucketIndex);
						for (i = LOOPBACKSTAT_ptr23; i < LOOPBACKSTAT_ptr; i += 4) {
							unsigned long text_id = IORD_32DIRECT(SNIFFER_TSE_A_BASE, offset);
							D(1, BUG("text_id: %x\n", text_id));
							offset += 4;
							SaveBytesLong(ConfigStat, i, text_id);
						}

//						IOWR_16DIRECT(SNIFFER_TSE_A_BASE, 0x10, 0x1);	//Clear entry in Sniffer ISR
					}
					else {
						// don't need, simply ignoring non-responses when master?
					}

					//IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x10, 0x2);	//Reset Swapper ISR
					IOWR_16DIRECT(SNIFFER_TSE_A_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
					continue;

				}

				// Cases to ignore: detect remote loopback off, ipv4/6 request packet received but button not on
				if ( !DETECT_REMOTE_LOOPBACK_TSEA || ((oneHot & ONEHOT_IPV4_MASK) && ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0x0C) != 0x04))
						|| ((oneHot & ONEHOT_IPV6_MASK) && ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0x0C) != 0x08)) ) {
					D(1, BUG("REQUEST IGNORED\n"));
					//IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x10, 0x2);	//Reset Swapper ISR
					IOWR_16DIRECT(SNIFFER_TSE_A_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
					continue;
				}


				// SEND RESPONSE
				//IOWR_16DIRECT(SNIFFER_TSE_A_BASE, 0x10, 1 << bucketIndex);	//Clear entry in Sniffer ISR

				// Writing to peer identity table (peer mac)
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC1_ptr, IORD_16DIRECT(SNIFFER_TSE_A_BASE, entryBase));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC0_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase + 4));

				// Write VLAN and IP fields
				// IP
				if (oneHot & ONEHOT_IPV4_MASK) {
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase + 0x14));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer, LOOPBACK_IP0_SRC3_ptr));
				}
				else if (oneHot & ONEHOT_IPV6_MASK) {
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP3_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase + 0x08));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP2_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase + 0x0C));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP1_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase + 0x10));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryBase + 0x14));

					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP3_ptr, BytesToLong(RxBuffer, LOOPBACK_IP0_SRC3_ptr));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP2_ptr, BytesToLong(RxBuffer, LOOPBACK_IP1_SRC3_ptr));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP1_ptr, BytesToLong(RxBuffer, LOOPBACK_IP2_SRC3_ptr));
					IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer, LOOPBACK_IP3_SRC3_ptr));


				}
				// VLAN
				// if stacked, swap the writes
				if ((oneHot & ONEHOT_STACKED_VLAN_MASK) == 0x600) {
					D(1, BUG("SNIFFER STACKED VLAN\n"));
					IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN2_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryVLANBase) >> 16);
					IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, IORD_16DIRECT(SNIFFER_TSE_A_BASE, entryVLANBase));
					IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN2_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryVLANBase) >> 16);
					IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, IORD_16DIRECT(SNIFFER_TSE_A_BASE, entryVLANBase));
				}
				else if (oneHot & ONEHOT_VLAN_MASK){
					D(1, BUG("SNIFFER VLAN\n"));
					IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryVLANBase) >> 16);
					IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, IORD_32DIRECT(SNIFFER_TSE_A_BASE, entryVLANBase) >> 16);
				}

				// Mode
				//IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x0, 0x1); //mac swap?
				// Swapper identity table - src mac
				int self_mac = ConfigStat[MAC_ADDR_MSB00_ptr] << 8;
				self_mac += ConfigStat[MAC_ADDR_MSB01_ptr];
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_MAC1_ptr, self_mac);
				self_mac = ConfigStat[MAC_ADDR_MSB02_ptr] << 24;
				self_mac += ConfigStat[MAC_ADDR_MSB03_ptr] << 16;
				self_mac += ConfigStat[MAC_ADDR_LSB10_ptr] << 8;
				self_mac += ConfigStat[MAC_ADDR_LSB11_ptr];
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_MAC0_ptr, self_mac);

				// Check what type of response packet to send and set command register
				unsigned int command = 0;

				if ( oneHot & ONEHOT_L2_MASK )	// layer-2
					command |= 0x10000;

				if ( (oneHot & ONEHOT_STACKED_VLAN_MASK) == 0x600 )	// stacked vlan
					command |= 0x180000;
				else if ( oneHot & ONEHOT_VLAN_MASK )		// vlan
					command |= 0x80000;

				if ( oneHot & ONEHOT_IPV4_MASK )	// ipv4
					command |= 0x20000;

				if ( oneHot & ONEHOT_IPV6_MASK )	// ipv6
					command |= 0x70000;

				// TODO: Set necessary flags
				D(1, BUG("TEST IN PROGRESS?: %x\n", ConfigStat[STARTSTOP_ptr]));
				if ( ConfigStat[STARTSTOP_ptr] == 0x30 && !GIGE10 )	// Test in progress, busy flag
					command |= 0x200;

				if ( ((oneHot & ONEHOT_PKTTYPE_MASK) == LOOPUP_REQUEST) ) {	// loop-up
					command |= 0x2;
					//SET_TSE_A_LOOPBACK;
					if ( (command & 0x200) != 0x200 ) {		// not busy; go into loopback
						D(1, BUG("LOOPING UP\n"));

						if ( (command & 0x20000) == 0x20000 ) 		// Check if L3
							ConfigStat[LOOPBACKSTAT_ptr] = L3_LOOPUP_RQST_RCVD;
						else										// L2
							ConfigStat[LOOPBACKSTAT_ptr] = L2_LOOPUP_RQST_RCVD;

						if ( (ConfigStat[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20 )		// already looped up, set "already" flag
							command |= 0x100;
						else {
							LOOPBACK_EVENT = LOOPUPTSEA;

							if ( (command & 0x20000) == 0x20000 )
								SWAP_IP = YES;
//							SET_TSE_A_LOOPBACK;
//							ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x20;
//
//							if ( (command & 0x20000) == 0x20000 ) {					// Turn on swap IP
//								SET_SWAP_A_TOGGLE |= 0x02;
//								SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
//								ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x80;
//							}
//
//							SET_SWAP_A_TOGGLE |= 0x01;								// Turn on swap MAC
//							SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
//							ConfigStat[CORE_SIGROUTE_MODES_ptr] |= 0x40;
						}

					}

				}
				else if ( (oneHot & ONEHOT_PKTTYPE_MASK) == LOOPDOWN_REQUEST ) {	// loop-down
					command |= 0x4;
					//ConfigStat[LOOPBACKSTAT_ptr] = 0x10;
					if ( (command & 0x200) != 0x200 ) {			// If not busy, loop down and set status bit

						if ( (command & 0x20000) == 0x20000 )		// Check if L3
							ConfigStat[LOOPBACKSTAT_ptr] = L3_LOOPDWN_RQST_RCVD;
						else
							ConfigStat[LOOPBACKSTAT_ptr] = L2_LOOPDWN_RQST_RCVD;

						if ( (ConfigStat[CORE_SIGROUTE_MODES_ptr] & 0x20) != 0x20 ) 	// already looped down, set "already" flag
							command |= 0x100;
						else {
							LOOPBACK_EVENT = LOOPDOWNTSEA;
//							SET_TSE_A_NORMAL_TEST_MODE;
//							ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x20;
//
//							SET_SWAP_A_TOGGLE &= ~0x02;						// Turn off swap IP
//							SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
//							ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
//
//							SET_SWAP_A_TOGGLE &= ~0x01;						// Turn off swap MAC
//							SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
//							ConfigStat[CORE_SIGROUTE_MODES_ptr] &= ~0x40;
						}
					}

				}
				else if ( (oneHot & ONEHOT_PKTTYPE_MASK) == HELLO_REQUEST )	// hello
					command |= 0x1;

				D(1, BUG("COMMAND: %X\n", command));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0xC0, command);

				D(1, BUG("LOOPBACKSTAT_PTR: %X\n", ConfigStat[LOOPBACKSTAT_ptr]));

				// Copy text id field to ConfigStat to display in UI
				int offset = 0x11C;
				for (i = LOOPBACKSTAT_ptr23; i < LOOPBACKSTAT_ptr; i += 4) {
					unsigned long text_id = IORD_32DIRECT(SNIFFER_TSE_A_BASE, offset);
					offset += 4;
					SaveBytesLong(ConfigStat, i, text_id);
				}

				IOWR_16DIRECT(SNIFFER_TSE_A_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
			}

			// Once done processing hello responses, send LoopbackStat array and clear mastership
//			if (LOOK_FOR_RESPONSE == 3 && loopbackStatPtr != 2) {
//				SEND_LPBK_DATA = YES;
//				LOOK_FOR_RESPONSE = 0;
//				IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0);
//			}

			//IOWR_32DIRECT(SNIFFER_TSE_A_BASE, 0, 0);
			//IOWR_8DIRECT(IRQ_MASK_BASE, 0, 0x30);	// Unmask port A sniffer again
	}

	else if ( (IORD_8DIRECT(IRQ_AGGREGATOR_BASE, 0) & 0x20) == 0x20 ) {
		D(1, BUG("In B\n"));

		unsigned short bucketIndex = 0;
		while ( IORD_16DIRECT(SNIFFER_TSE_B_BASE, SNIFFER_ISR_REGISTER) != 0 ) {
			D(1, BUG("while loop (B)\n"));
			D(1, BUG("SNIFFER (B): %X\n", IORD_16DIRECT(SNIFFER_TSE_B_BASE, 0x10)));
			D(1, BUG("(1-HOT): %X\n", IORD_32DIRECT(SNIFFER_TSE_B_BASE, 0x100)));

			// TODO: how to test multiple bucket entries?
			if ( !(IORD_16DIRECT(SNIFFER_TSE_B_BASE, SNIFFER_ISR_REGISTER) & (1 << bucketIndex)) ) {
				++bucketIndex;
				if ( bucketIndex == 6 )
					bucketIndex = 0;
				D(1, BUG("bucketIndex (empty) : %d\n", bucketIndex));
			}

			int entryBase = IDENTITY_TABLE_BASE + (BUCKET_ENTRY_SIZE * bucketIndex);
			int entryVLANBase = SWPR_IDTABLE_VLAN_BASE + (BUCKET_ENTRY_SIZE * bucketIndex);
			unsigned int oneHot = IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryBase) >> 16;
			D(1, BUG("bucketIndex : %d\n", bucketIndex));

			// TODO: if detect remote loopback is off, ignore request packets
			if (!DETECT_REMOTE_LOOPBACK_TSEB || ((oneHot & ONEHOT_IPV4_MASK) && ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0x0C) != 0x04))
					|| ((oneHot & ONEHOT_IPV6_MASK) && ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0x0C) != 0x08)) ) {
				IOWR_16DIRECT(SNIFFER_TSE_B_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
				continue;
			}

			// SEND RESPONSE
			// Writing to peer identity table
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC1_ptr, IORD_16DIRECT(SNIFFER_TSE_B_BASE, 0x100));
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC0_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, 0x104));

			// Write VLAN and IP fields
			// IP
//			D(1, BUG("ipv4B_active: %d\n", ipv4B_active));
			if (oneHot & ONEHOT_IPV4_MASK) {
				unsigned long ip_address = BytesToLong(ConfigStat, LOOPBACK_IP0_SRC3_ptr);
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryBase + 0x14));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, ip_address == 255 ? --ip_address : ++ip_address);
			}
			else if (oneHot & ONEHOT_IPV6_MASK){
				unsigned long ip_address = BytesToLong(ConfigStat, LOOPBACK_IP3_SRC3_ptr);

				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP3_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryBase + 0x08));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP2_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryBase + 0x0C));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP1_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryBase + 0x10));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryBase + 0x14));

				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP3_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP2_ptr, BytesToLong(RxBuffer,LOOPBACK_IP1_SRC3_ptr));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP1_ptr, BytesToLong(RxBuffer,LOOPBACK_IP2_SRC3_ptr));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, ((ip_address & 0xFFFF) == 0xFFFF) ? --ip_address : ++ip_address);
			}

			// VLAN
			// if stacked, swap the writes
			if ((oneHot & ONEHOT_STACKED_VLAN_MASK) == 0x600) {
				D(1, BUG("SNIFFER STACKED VLAN\n"));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN2_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryVLANBase) >> 16);
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, IORD_16DIRECT(SNIFFER_TSE_B_BASE, entryVLANBase));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN2_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryVLANBase) >> 16);
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, IORD_16DIRECT(SNIFFER_TSE_B_BASE, entryVLANBase));
			}
			else if (oneHot & ONEHOT_VLAN_MASK){
				D(1, BUG("SNIFFER VLAN\n"));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryVLANBase) >> 16);
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, IORD_32DIRECT(SNIFFER_TSE_B_BASE, entryVLANBase) >> 16);
			}

			// Mode
			//IOWR_32DIRECT(TSE_ADDR_SWAPPER_B_BASE, 0x0, 0x1); //mac swap?
			// Swapper identity table - src mac
			int self_mac = ConfigStatB[MAC_ADDR_MSB00_ptr] << 8;
			self_mac += ConfigStatB[MAC_ADDR_MSB01_ptr];
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x120, self_mac);
			self_mac = ConfigStatB[MAC_ADDR_MSB02_ptr] << 24;
			self_mac += ConfigStatB[MAC_ADDR_MSB03_ptr] << 16;
			self_mac += ConfigStatB[MAC_ADDR_LSB10_ptr] << 8;
			self_mac += ConfigStatB[MAC_ADDR_LSB11_ptr];
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x124, self_mac);

			D(1, BUG("ONEHOT0 (full): %X\n", IORD_32DIRECT(SNIFFER_TSE_B_BASE, 0x100)));
			D(1, BUG("ONEHOT0: %X\n", oneHot & ONEHOT_PKTTYPE_MASK));

			// Check what type of response packet to send
			unsigned int command = 0;

			if ( oneHot & ONEHOT_L2_MASK )	// layer-2
				command |= 0x10000;

			if ( (oneHot & ONEHOT_STACKED_VLAN_MASK) == 0x600 )	// stacked vlan
				command |= 0x180000;
			else if ( oneHot & ONEHOT_VLAN_MASK )		// vlan
				command |= 0x80000;

			if ( oneHot & ONEHOT_IPV4_MASK )	// ipv4
				command |= 0x20000;

			if ( oneHot & ONEHOT_IPV6_MASK )	// ipv6
				command |= 0x70000;

			if ( ((oneHot & ONEHOT_PKTTYPE_MASK) == LOOPUP_REQUEST) ) {	// loop-up
				command |= 0x2;

				if ( (command & 0x20000) == 0x20000 )		// L3
					ConfigStatB[LOOPBACKSTAT_ptr] = L3_LOOPUP_RQST_RCVD;
				else
					ConfigStatB[LOOPBACKSTAT_ptr] = L2_LOOPUP_RQST_RCVD;

				if ( (ConfigStatB[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20 )		// already looped up, set "already" flag
					command |= 0x100;
				else {
					LOOPBACK_EVENT = LOOPUPTSEB;

					if ( (command & 0x20000) == 0x20000 )
						SWAP_IP = YES;

//					SET_TSE_B_LOOPBACK;
//					ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x20;
//
//					if ( (command & 0x20000) == 0x20000 ) {					// Turn on swap IP
//						SET_SWAP_B_TOGGLE |= 0x02;
//						SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
//						ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x80;
//					}
//
//					SET_SWAP_B_TOGGLE |= 0x01;								// Turn on swap MAC
//					SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
//					ConfigStatB[CORE_SIGROUTE_MODES_ptr] |= 0x40;
				}

			}
			else if ( (oneHot & ONEHOT_PKTTYPE_MASK) == LOOPDOWN_REQUEST ) {	// loop-down
				command |= 0x4;
				if ( (command & 0x20000) == 0x20000 )		// L3
					ConfigStatB[LOOPBACKSTAT_ptr] = L3_LOOPDWN_RQST_RCVD;
				else
					ConfigStatB[LOOPBACKSTAT_ptr] = L2_LOOPDWN_RQST_RCVD;

				if ( (ConfigStatB[CORE_SIGROUTE_MODES_ptr] & 0x20) != 0x20 ) 	// already looped down, set "already" flag
					command |= 0x100;
				else {
					LOOPBACK_EVENT = LOOPDOWNTSEB;

//					SET_TSE_B_TX_3MUX_TO_NO_OUTPUT;
//					ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x20;
//
//					SET_SWAP_B_TOGGLE &= ~0x02;							// Disable swap IP
//					SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
//					ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
//
//					SET_SWAP_B_TOGGLE &= ~0x01;							// Disable swap MAC
//					SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
//					ConfigStatB[CORE_SIGROUTE_MODES_ptr] &= ~0x40;
				}
			}
			else if ( (oneHot & ONEHOT_PKTTYPE_MASK) == HELLO_REQUEST )	// hello
				command |= 0x1;

			D(1, BUG("COMMAND: %X\n", command));
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_B_BASE, 0xC0, command);

			// Copy text id field to ConfigStat to display in UI
			int offset = 0x11C;
			for (i = LOOPBACKSTAT_ptr23; i < LOOPBACKSTAT_ptr; i += 4) {
				unsigned long text_id = IORD_32DIRECT(SNIFFER_TSE_B_BASE, offset);
				offset += 4;
				SaveBytesLong(ConfigStatB, i, text_id);
			}

			IOWR_16DIRECT(SNIFFER_TSE_B_BASE, SNIFFER_ISR_REGISTER, 0x01);	//Clear entry in Sniffer ISR
		}

	}

	else if ( (IORD_8DIRECT(IRQ_AGGREGATOR_BASE, 0) & 0x8) == 0x8 ) {
		D(1, BUG("In 10G-A\n"));

		// While buckets are not empty
		unsigned short bucketIndex = 0;
		D(1, BUG("SNIFFER: %X\n", IORD_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER)));

		//loopbackStatPtr = 2;
		/**
		 *	Main while loop for 10G-A sniffer interrupts.
		 *	1. Check for the next filled bucket entry in the sniffer
		 *	2. Check to see if we're currently a master (sent out a request packet)
		 *		i. Check to see we have received a response packet and it's the right type (loop up/down, query)
		 *			a. If loop-up/down, then set LOOPBACKSTAT_ptr for UI and reset mastership
		 *			b. If hello, copy bucket entry into LoopbackStat
		 *		ii. If not a response packet, ignore and clear bucket entry
		 *	3. Check if see if detect remote loopback is off or if a IPv4/6 packet sent when mode is off
		 *		i. If true, ignore packet and clear bucket entry
		 *	4. If applicable, copy VLAN and IP info to swapper table for peer and self
		 *	5. Set appropriate command mode bits according to the type of request packet received
		 *		i. If loop-up request, turn on loopback and swap MAC as well as swap IP if an IP request packet
		 *		ii. If loop-down request, turn off loopback and all swappers
		 *	6. Write command to swapper to send response packet
		 *	7. Clear bucket entry
		 */
		while ( IORD_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER) != 0 ) {
			D(1, BUG("while loop\n"));

			// ^6 set if entries are full; clear and continue processing the valid entries
			if ((IORD_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER) & 0x40) == 0x40) {
				IOWR_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER, 0x40);
				continue;
			}

			D(1, BUG("SNIFFER: %X\n", IORD_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER)));

			// Go to the next filled bucket
			if ( !(IORD_16DIRECT(SNIFFER64_10GE_BASE, 0x10) & (1 << bucketIndex)) ) {
				++bucketIndex;
				if ( bucketIndex == 6 )
					bucketIndex = 0;
				D(1, BUG("bucketIndex (empty) : %d\n", bucketIndex));
			}

			//IOWR_16DIRECT(SNIFFER64_10GE_BASE, 0x10, 1 << bucketIndex);	//Clear entry in Sniffer ISR

			int entryBase = IDENTITY_TABLE_BASE + (BUCKET_ENTRY_SIZE * bucketIndex);
			int entryVLANBase = SWPR_IDTABLE_VLAN_BASE + (BUCKET_ENTRY_SIZE * bucketIndex);
			unsigned int oneHot = IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase) >> 16;
			D(1, BUG("bucketIndex : %d\n", bucketIndex));

			// If master, we're only looking for response packets
			int master = IORD_8DIRECT(SNIFFER64_10GE_BASE, 0);
			D(1, BUG("master: %d\n", master));
			D(1, BUG("look for response: %d\n", LOOK_FOR_RESPONSE));
			if ( (master & 0x1) == 1 ) {

				int packet_type = oneHot & ONEHOT_PKTTYPE_MASK;
				D(1, BUG("PACKET TYPE: %x\n", packet_type));

				// Check for a response packet if we're master
				if (LOOK_FOR_RESPONSE && packet_type > 0x04) {
					if (LOOK_FOR_RESPONSE == HELLO && packet_type == HELLO_RESPONSE) {
						D(1, BUG("HELLO\n"));
						// Load unit info into LoopbackStat to display
						int offset = entryBase;
						int i;
						for (i = 0; i < BUCKET_ENTRY_SIZE; i += 4) {
							SaveBytesLong(LoopbackStat10G, loopbackStatPtr, IORD_32DIRECT(SNIFFER64_10GE_BASE, offset));
							offset += 4;
							loopbackStatPtr += 4;
						}
						responseUnits++;
					}
					else if (LOOK_FOR_RESPONSE == LOOPUP && packet_type == LOOPUP_RESPONSE) {
						D(1, BUG("SLAVE LOOPED UP\n"));
						// Display looped-up unit
						if ( (oneHot & ONEHOT_FLAG_MASK) == 0x8000 ) {
							D(1, BUG("SLAVE BUSY\n"));
							ConfigStat10G[LOOPBACKSTAT_ptr] = REMOTE_BUSY;
						}
						else if ( (oneHot & ONEHOT_FLAG_MASK) == 0x4000 ) {
							D(1, BUG("SLAVE ALREADY UP\n"));
							ConfigStat10G[LOOPBACKSTAT_ptr] = REMOTE_ALREADY_UP;
						}
						else
							ConfigStat10G[LOOPBACKSTAT_ptr] = REMOTE_LOOPED_UP;
						LOOK_FOR_RESPONSE = 0;
						IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0);
					}
					else if (LOOK_FOR_RESPONSE == LOOPDOWN && packet_type == LOOPDOWN_RESPONSE) {
						D(1, BUG("SLAVE LOOPED DOWN\n"));
						// Display looped-down unit
						if ( (oneHot & ONEHOT_FLAG_MASK) == 0x8000 ) {
							D(1, BUG("SLAVE BUSY\n"));
							ConfigStat10G[LOOPBACKSTAT_ptr] = REMOTE_BUSY;
						}
						else if ( (oneHot & ONEHOT_FLAG_MASK) == 0x4000 ) {
							D(1, BUG("SLAVE ALREADY DOWN\n"));
							ConfigStat10G[LOOPBACKSTAT_ptr] = REMOTE_ALREADY_DOWN;
						}
						else
							ConfigStat10G[LOOPBACKSTAT_ptr] = REMOTE_LOOPED_DOWN;
						LOOK_FOR_RESPONSE = 0;
						IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0);
					}
					else {
						// didn't get a response packet
						// do we get in here? erroneous packet
					}

					D(1, BUG("MASTER LOOPBACKSTAT_PTR: %X\n", ConfigStat10G[LOOPBACKSTAT_ptr]));
					SaveBytesLong(ConfigStat10G, LOOPBACKSTAT_ptr23, 0);	//clear the array
					SaveBytesLong(ConfigStat10G, LOOPBACKSTAT_ptr13, 0);
					SaveBytesLong(ConfigStat10G, LOOPBACKSTAT_ptr03, 0);
					// Copy text id field to ConfigStat for UI to display unit name
					int offset = 0x11C + (BUCKET_ENTRY_SIZE * bucketIndex);
					for (i = LOOPBACKSTAT_ptr23; i < LOOPBACKSTAT_ptr; i += 4) {
						unsigned long text_id = IORD_32DIRECT(SNIFFER64_10GE_BASE, offset);
						D(1, BUG("text_id: %x\n", text_id));
						offset += 4;
						SaveBytesLong(ConfigStat10G, i, text_id);
					}

//						IOWR_16DIRECT(SNIFFER64_10GE_BASE, 0x10, 0x1);	//Clear entry in Sniffer ISR
				}
				else {
					// don't need, simply ignoring non-responses when master?
				}

				//IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x10, 0x2);	//Reset Swapper ISR
				IOWR_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
				continue;

			}

			// Cases to ignore: detect remote loopback off, ipv4/6 request packet received but button not on
			if ( !DETECT_REMOTE_LOOPBACK_10GA || ((oneHot & ONEHOT_IPV4_MASK) && ((ConfigStat10G[EGEN_PKT_TYPE_ptr] & 0x0C) != 0x04))
					|| ((oneHot & ONEHOT_IPV6_MASK) && ((ConfigStat10G[EGEN_PKT_TYPE_ptr] & 0x0C) != 0x08)) ) {
				D(1, BUG("REQUEST IGNORED\n"));
				//IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x10, 0x2);	//Reset Swapper ISR
				IOWR_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
				continue;
			}

				D(1,BUG("\nsend response\n"));
			// SEND RESPONSE
			//IOWR_16DIRECT(SNIFFER64_10GE_BASE, 0x10, 1 << bucketIndex);	//Clear entry in Sniffer ISR

			// Writing to peer identity table (peer mac)
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC1_ptr, IORD_16DIRECT(SNIFFER64_10GE_BASE, entryBase));
			IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_MAC0_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase + 4));

			// Write VLAN and IP fields
			// IP
			if (oneHot & ONEHOT_IPV4_MASK) {
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase + 0x14));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));
			}
			else if (oneHot & ONEHOT_IPV6_MASK) {
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP3_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase + 0x08));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP2_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase + 0x0C));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP1_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase + 0x10));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_IP0_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryBase + 0x14));

				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP3_ptr, BytesToLong(RxBuffer,LOOPBACK_IP0_SRC3_ptr));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP2_ptr, BytesToLong(RxBuffer,LOOPBACK_IP1_SRC3_ptr));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP1_ptr, BytesToLong(RxBuffer,LOOPBACK_IP2_SRC3_ptr));
				IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_IP0_ptr, BytesToLong(RxBuffer,LOOPBACK_IP3_SRC3_ptr));
			}
			// VLAN
			// if stacked, swap the writes
			if ((oneHot & ONEHOT_STACKED_VLAN_MASK) == 0x600) {
				D(1, BUG("SNIFFER STACKED VLAN\n"));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN2_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryVLANBase) >> 16);
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, IORD_16DIRECT(SNIFFER64_10GE_BASE, entryVLANBase));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN2_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryVLANBase) >> 16);
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, IORD_16DIRECT(SNIFFER64_10GE_BASE, entryVLANBase));
			}
			else if (oneHot & ONEHOT_VLAN_MASK){
				D(1, BUG("SNIFFER VLAN\n"));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_PEER_VLAN1_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryVLANBase) >> 16);
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE, SWAPPER_SELF_VLAN1_ptr, IORD_32DIRECT(SNIFFER64_10GE_BASE, entryVLANBase) >> 16);
			}

			// Mode
			//IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0x0, 0x1); //mac swap?
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

			// Check what type of response packet to send and set command register
			unsigned int command = 0;

			if ( oneHot & ONEHOT_L2_MASK )	// layer-2
				command |= 0x10000;

			if ( (oneHot & ONEHOT_STACKED_VLAN_MASK) == 0x600 )	// stacked vlan
				command |= 0x180000;
			else if ( oneHot & ONEHOT_VLAN_MASK )		// vlan
				command |= 0x80000;

			if ( oneHot & ONEHOT_IPV4_MASK )	// ipv4
				command |= 0x20000;

			if ( oneHot & ONEHOT_IPV6_MASK )	// ipv6
				command |= 0x70000;

			// TODO: Set necessary flags
			D(1, BUG("TEST IN PROGRESS?: %x\n", ConfigStat10G[STARTSTOP_ptr]));
			if ( ConfigStat10G[STARTSTOP_ptr] == 0x30 && GIGE10 )	// Test in progress, busy flag
				command |= 0x200;

			if ( ((oneHot & ONEHOT_PKTTYPE_MASK) == LOOPUP_REQUEST) ) {	// loop-up
				command |= 0x2;
				//SET_TSE_A_LOOPBACK;
				if ( (command & 0x200) != 0x200 ) {		// not busy; go into loopback
					D(1, BUG("LOOPING UP\n"));

					if ( (command & 0x20000) == 0x20000 ) 		// Check if L3
						ConfigStat10G[LOOPBACKSTAT_ptr] = L3_LOOPUP_RQST_RCVD;
					else										// L2
						ConfigStat10G[LOOPBACKSTAT_ptr] = L2_LOOPUP_RQST_RCVD;

					if ( (ConfigStat10G[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20 )		// already looped up, set "already" flag
						command |= 0x100;
					else {
						LOOPBACK_EVENT = LOOPUP10G;

						if ( (command & 0x20000) == 0x20000 )
							SWAP_IP = YES;


//						SET_10G_TX_TO_NW_LOOPBACK;		// Enable LPBK
//						SET_10G_RX_3SPLITTER_TO_10G_TX;
//						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |= 0x20;
//
//						write_ext_10G_phy(0xC001, 1, (read_ext_10G_phy(0xC001, 1))|0x0200); // set FTX_LTIME_EN ON - to use Recovered Clock as Tx Clock
//
//						set_reset_10GMAC_FIFOS();
//						//SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD;
//						usleep(1);
//						clear_reset_10GMAC_FIFOS();
//
//						init_10G_SC_FIFO(ETH_10G_SCFIFO_STORE_FORWARD);	//init & set RxFIFO to Store & Fwd Mode for Lpbk
//
//
//						if ( (command & 0x20000) == 0x20000 ) {					// Turn on swap IP
//							SET_SWAP_10GA_TOGGLE |= 0x02;
//							SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
//							ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |= 0x80;
//						}
//
//						SET_SWAP_10GA_TOGGLE |= 0x01;				// Turn on swap MAC
//						SET_10GA_MAC_ADDR_SWAP(SET_SWAP_10GA_TOGGLE);
//						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] |= 0x40;
					}

				}

			}
			else if ( (oneHot & ONEHOT_PKTTYPE_MASK) == LOOPDOWN_REQUEST ) {	// loop-down
				command |= 0x4;
				//ConfigStat[LOOPBACKSTAT_ptr] = 0x10;
				if ( (command & 0x200) != 0x200 ) {			// If not busy, loop down and set status bit

					if ( (command & 0x20000) == 0x20000 )		// Check if L3
						ConfigStat10G[LOOPBACKSTAT_ptr] = L3_LOOPDWN_RQST_RCVD;
					else
						ConfigStat10G[LOOPBACKSTAT_ptr] = L2_LOOPDWN_RQST_RCVD;

					if ( (ConfigStat10G[CORE_SIGROUTE_MODES_ptr] & 0x20) != 0x20 ) 	// already looped down, set "already" flag
						command |= 0x100;
					else {
						LOOPBACK_EVENT = LOOPDOWN10G;
//						SET_10G_NORMAL_TEST_MODE;
//						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &= ~0x20;
//
//						write_ext_10G_phy(0xC001, 1, (read_ext_10G_phy(0xC001, 1))&0xFDFF); // set FTX_LTIME_EN OFF - to use local OSC as Tx Clock
//
//						set_reset_10GMAC_FIFOS();
//						//SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;
//						usleep(1);
//						clear_reset_10GMAC_FIFOS();
//
//						init_10G_SC_FIFO(ETH_10G_SCFIFO_CUT_THRU);		//init & set RxFIFO to Cut-thru mode for NORMAL mode
//
//
//						SET_SWAP_10GA_TOGGLE &= ~0x02;						// Turn off swap IP
//						SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
//						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &= ~0x80;
//
//						SET_SWAP_10GA_TOGGLE &= ~0x01;						// Turn off swap MAC
//						SET_TSE_A_MAC_ADDR_SWAP(SET_SWAP_A_TOGGLE);
//						ConfigStat10G[CORE_SIGROUTE_MODES_ptr] &= ~0x40;
					}
				}

			}
			else if ( (oneHot & ONEHOT_PKTTYPE_MASK) == HELLO_REQUEST )	// hello
				command |= 0x1;

			D(1, BUG("COMMAND: %X\n", command));
			IOWR_32DIRECT(SWAPPER64_10G_BASE, 0xC0, command);

			D(1, BUG("LOOPBACKSTAT_PTR: %X\n", ConfigStat10G[LOOPBACKSTAT_ptr]));

			// Copy text id field to ConfigStat to display in UI
			int offset = 0x11C;
			for (i = LOOPBACKSTAT_ptr23; i < LOOPBACKSTAT_ptr; i += 4) {
				unsigned long text_id = IORD_32DIRECT(SNIFFER64_10GE_BASE, offset);
				offset += 4;
				SaveBytesLong(ConfigStat10G, i, text_id);
			}

			IOWR_16DIRECT(SNIFFER64_10GE_BASE, SNIFFER_ISR_REGISTER, 1 << bucketIndex);	//Clear entry in Sniffer ISR
		}

		// Once done processing hello responses, send LoopbackStat array and clear mastership
//			if (LOOK_FOR_RESPONSE == 3 && loopbackStatPtr != 2) {
//				SEND_LPBK_DATA = YES;
//				LOOK_FOR_RESPONSE = 0;
//				IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0);
//			}

		//IOWR_32DIRECT(SNIFFER64_10GE_BASE, 0, 0);
		//IOWR_8DIRECT(IRQ_MASK_BASE, 0, 0x30);	// Unmask port A sniffer again
	}

	D(1, BUG("EXIT TIME: %ld\n", OSTimeGet()));

}


//=====================================================================
// TIMER is used by uCOS!!!!
// TIMER_1
// 1ms Tick Interrupt: Actual time is 0.999996mS  (83.333e6 sys clock) or .999948 using 83,330,000Hz
//=====================================================================
void timer1_ISR(void *context)
{
	unsigned short i=0;
	// ************************************************************
	// *** Rx BUFFER TIMEOUT  *************************************
	// ************************************************************
	if(RxTIMEOUT){             			// If Rx BUFFER TIMEOUT Timer Running
		if(--RxTIMEOUT == 0){			// If it has Timed-OUT.....
			//IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,0x03);	// set LED ON
			D(1, BUG("\n>>> SIO RxBuffer TIMEOUT <<<!\n"));
			i = IORD_ALTERA_AVALON_UART_RXDATA(UART_0_BASE);				  // read RcvData reg to clear any old data
			UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_RRDY_MSK;		  // Enable the serial port Receive interrupt
			IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
			buff_ptr = 0;
			ETH_PACKET = NO;			// flush for new packet
			RCVING_MS_PKT = NO;			// flush for new packet
			// in one_sec.c we go 3 consecutive seconds w/o a (good??) packet - switch to VT100.
			// and turn OFF link fault LED in VT100 modes
			}
		}
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_1_BASE, 0); // clear timer (for next TO event!)
}


//==================================================================
/*** 10G ************************************************************
	____I2C Hotplug Upload Status 3.D110 FW_SFP_MIRROR_STATUS_REG___
	1:0		FW_ROM_A0_STATUS	RO	0		--A0 ROM Upload Status--
											00: No module yet detected
											01: ROM upload completed
											10: ROM upload started
											11: ROM upload error
	3:2		FW_ROM_A2_STATUS	RO	0		--A2 ROM Upload Status--
											00: No module yet detected
											01: ROM upload completed
											10: ROM upload started
											11: ROM upload error
	15:4	RSVD				RO	0		Reserved
 **************************************************************/
/*** 10G ************************************************************
	____UC_STATUS	3.D7FD[7:0]
	____ 		_________Description_____________
	0x00		Firmware not yet running
	0x10		Customer configuration attained
	0x20		Saving code RAM to EEPROM
	0x30		No module detected - waiting for module
	0x31		Module detected- updating NVR	- firmware will remain in this state if the module is unsupported
	0x32		Port disabled, chip powered down except for micro (DISABLE_PORT = 1)
	0x40		Preparing to start Acquisition
	0x50		Acquisition - attempting to acquire and lock to incoming signal
	0x60		Acquisition completed
	0x70		Tracking incoming signal
	0xF0		Checksum failed
	0xF8		Device version and firmware version are not compatible
 **************************************************************/
/*** 10G ************************************************************
	____Register 3.D70Ch Definition for Indicating SFP+ Self-configure State
	Bit		____Name_______		_____Description__________
	7:0		OP_MODE_CURRENT		1 = 10GBASE-LRM
								2 = 10GBASE-SR
								3 = 10GBASE-LR
								4 = SFP+ passive copper direct attach cable
								5 = 1000BASE-SX
								6 = 1000BASE-LX
								7 = Invalid Module Type
								8 = 1000BASE-CX (1GE copper cable)
								9 = 1000BASE-T
								10 (0xA) = 10GBASE-ER
								11 (0xB) = SFP+ active copper direct attach cable
								>11 reserved for future use
	15:8	RSVD				Reserved.
 **************************************************************/
//==================================================================
void handle_sfp_ISR(void* context)
{
    //Cast context to edge_capture's type. It is important that this be
    // declared volatile to avoid unwanted compiler optimization.
    volatile int* pio_edge_capture_ptr = (volatile int*) context;
    // Store the value in the SFP's edge capture register in *context.
    *pio_edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE);

    //unsigned int tmpdata = 0;
    unsigned int pio_sfp_state = IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE);	// snag state of sfp detect pins

    //june13 - removed else(s) to all if's to allow multiple detections/checks at once - should they occur!
    if(pio_edge_capture&0x01){		// TSE PortA sfp inserted or removed
    	if(pio_sfp_state&0x01){		// High means SFP in PortA was just REMOVED!
    		D(1, BUG("Whoop's!  SFP in 1G PORT-A was REMOVED!\n"));
    		}
    	else{						// Low means it was just INSERTED!
    		READ_SFPA_DATA = YES;	// flag main.c to read SFP and then pass info up to GUI
    		D(1, BUG("Nice!  an SFP in 1G PORT-A was just INSTALLED!\n"));
    		}
    	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x01);	// Reset the SFP's edge capture register
    	}

    if(pio_edge_capture&0x02){	// TSE PortB sfp inserted or removed
    	if(pio_sfp_state&0x02){		// High means SFP in PortA was just REMOVED!
    		D(1, BUG("Whoop's!  SFP in 1G PORT-B was REMOVED!\n"));
    		}
    	else{						// Low means it was just INSERTED!
    		READ_SFPB_DATA = YES;	// flag main.c to read SFP and then pass info up to GUI
    		D(1, BUG("Nice!  an SFP in 1G PORT-B was just INSTALLED!\n"));
    		}
    	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x02);	// Reset the SFP's edge capture register
    	}

    ConfigStat[SFP_STAT_ptr] &= SFP_STATE_MASK;	// SFP changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
    ConfigStat[SFP_STAT_ptr] |= (pio_sfp_state&0x03)<<1; // set SFP A&B in/out states - no 10G port-B yet!
    ConfigStat[SFP_STAT_ptr] ^= 0x01;	// toggle bit 1 to indicate that something has changed with the SFP's
    ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr]; // save in 10G stat as well!
    ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];

    // Read the PIO to delay ISR exit. This is done to prevent a spurious
    // interrupt in systems with high processor -> pio latency and fast interrupts.
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE);
}

//********************************************************************************
// Serial Port Interrupt Service Routine
//	Communication Link between: GIGE and GUI
//********************************************************************************
void serial_ISR(void *context)
{
  int sr = IORD_ALTERA_AVALON_UART_STATUS (UART_0_BASE);	// Save the status register value
  int cr = IORD_ALTERA_AVALON_UART_CONTROL (UART_0_BASE);// Save the control register value

  	//******************************************************************
  	//*** Transmit Interrupt *******************************************
	if(sr & ALTERA_AVALON_UART_STATUS_TRDY_MSK)				// if TxRDY
	{
		if(cr & ALTERA_AVALON_UART_CONTROL_TRDY_MSK)		// if Tx Intr is Enabled
		{
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE ,TxBuffer[tbuff_ptr++]);
			 if(SENDING_MS_CONFIG){
			          if (tbuff_ptr == MaxConfig_ms){           //If sent last byte, then...
			        	  D(1, BUG( "\n<> MS pkt SENT \n"));
			        	  UART_CONTROL_REG &= ~ALTERA_AVALON_UART_CONTROL_TRDY_MSK;           // Disable the serial port Transmit interrupt
			                IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
			                SEND_MS_PACKET = NO; SENDING_MS_CONFIG = NO;
			                }
			          }
			 else if(SENDING_STD_PKT){
			          if (tbuff_ptr == MaxStatus){           //If sent last byte, then...
			        	  UART_CONTROL_REG &= ~ALTERA_AVALON_UART_CONTROL_TRDY_MSK;           // Disable the serial port Transmit interrupt
			                IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
			                SENDING_STD_PKT = NO;
			                }
			          }
			    else{ //sending std Eth ConfStat pkt
			          if (tbuff_ptr == MaxStatus2){       //If sent last byte, then...
			                UART_CONTROL_REG &= ~ALTERA_AVALON_UART_CONTROL_TRDY_MSK;           // Disable the serial port Transmit interrupt
			                IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
			                }
			          }
		}
	}//End of Tx interrupt processing


  //******************************************************************
  //*** Receive Interrupt ********************************************
  if(sr & ALTERA_AVALON_UART_STATUS_RRDY_MSK)		// if RxRDY
  {
	RxBuffer[buff_ptr++] = IORD_ALTERA_AVALON_UART_RXDATA(UART_0_BASE);	// Load new Rx data into RxBuffer
	if(buff_ptr==2){
	//	D(1, BUG( "\n<> buffer_ptr %d RxBuffer[1]:= %x\n",buff_ptr,RxBuffer[1]));
		if(RxBuffer[1]==Packet_MultiStream){
			D(1, BUG( "\n<> MS pkt YES \n"));
			RCVING_MS_PKT = YES;
			ETH_PACKET = NO;
			RxTIMEOUT = 1200;					//(1.2s timeout for 5470 byte Rx Packet - multistream
			}
		else if(((RxBuffer[1])&0xF0) == Packet_GIGEA){ //if we're receiving an Ethernet Packet (any kind)
			ETH_PACKET = YES;
			//D(1, BUG( "\n<> ETH pkt YES \n"));
			RCVING_MS_PKT = NO;
			RxTIMEOUT = 250;
			}
		else{	//Packet_C3794
			//D(1, BUG( "\n<> C37.94 pkt  \n"));
			RCVING_MS_PKT = NO;
			ETH_PACKET = NO;
			RxTIMEOUT = 175;
			}
		}
	else{
		if(RCVING_MS_PKT){
		//	D(1, BUG( "\n<> mspkt %d RxBuffer[0]:= %x RxBuffer[1]:= %x\n",buff_ptr,RxBuffer[0],RxBuffer[1]));
			if(buff_ptr == MaxConfig_ms){
				D(1, BUG( "\n<> MS rcvd \n"));
				UART_CONTROL_REG &= ~ALTERA_AVALON_UART_CONTROL_RRDY_MSK;		// Disable the serial port Receive interrupt
				IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);	// write to CONTROL_REGISTER
				RxTIMEOUT = 0;					// We've seen the whole PACKET so STOP Timeout
				RxBuffer_RDY = 1; 	 			// WAKE up sleeping CONFIG task
				MS_PKT = YES;
				OSTaskResume(2);	//#define LINK_TASK_PRIORITY      2
				}
			}
		else if(ETH_PACKET){
			if(buff_ptr == MaxConfig2){	// MaxConfig: receiving 75 out of 150 transmitted it still works!  ...switching to full 150byte config2 array!
				//D(1, BUG( "\n<> SS_pkt rcvd buffer_ptr %d RxBuffer[0]:= %x RxBuffer[1]:= %x\n",buff_ptr,RxBuffer[0],RxBuffer[1]));
				UART_CONTROL_REG &= ~ALTERA_AVALON_UART_CONTROL_RRDY_MSK;		// Disable the serial port Receive interrupt
				IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);	// write to CONTROL_REGISTER
				RxTIMEOUT = 0;					// We've seen the whole PACKET so STOP Timeout
				RxBuffer_RDY = 1; 	 			// WAKE up sleeping CONFIG task
				ETH_PKT = YES;
				OSTaskResume(2);				//#define LINK_TASK_PRIORITY      2
				}
			}
		else{	// std pkt, Packet_C3794
			if(buff_ptr == MaxConfig){	// MaxConfig: receiving 75 out of 150 transmitted it still works!  ...switching to full 150byte config2 array!
				UART_CONTROL_REG &= ~ALTERA_AVALON_UART_CONTROL_RRDY_MSK;		// Disable the serial port Receive interrupt
				IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);	// write to CONTROL_REGISTER
				RxTIMEOUT = 0;					// We've seen the whole PACKET so STOP Timeout
				RxBuffer_RDY = 1; 	 			// WAKE up sleeping CONFIG task
				OSTaskResume(2);				//#define LINK_TASK_PRIORITY      2
				}
			}
		}
  }//End of Rx interrupt processing
}

/****
		// *************************************************************
		// ******** TRANSMIT A (to PDA) ********************************
 		// *************************************************************
		if((MVAR(char,MASTER+UART_ISRADDR))&0x01){    			// If Tx Ready!
			MVAR(char,MASTER+UART_THRADDR) = TxBuffer[tbuff_ptr++];
			if(tbuff_ptr == MaxStatus){      					// If we just wrote out the last byte, then...
				MVAR(char,MASTER+UART_CRADDR) = XMT_OFF;		// disable Tx
				UART_INTR &= ~0x01;          					// disable A Tx INTR Only!
				MVAR(char,MASTER+UART_IMRADDR) = UART_INTR;
				}
			}
  		// *************************************************************
***/

/****
 * 	RxBuffer[buff_ptr++] = ASC0_RBUF;

	if(buff_ptr==1)							// Just got our 1st BYTE of PACKET so START Timeout TIMER
		RxTIMEOUT = 150;					//(150) 60mS timeout for 43.4mS 250 byte Rx Packet
 	// *** when at #2 we'd just rcvd buff_ptr == 1 (ID_ptr) then let run until MaxMacro SIZE !!! *******
	else if(buff_ptr==2 && RxBuffer[1]&0x80)// if we're receiving a macro then
		NEW_MACRO = YES;

  	if(NEW_MACRO){
		if(buff_ptr == MaxMacro){	      	// MaxConfig when we have 6 flag a new buffer
			ASC0_CON_REN = OFF;      		// disable RCVR
			ASC0_RIC_IE = OFF;      		// disable Rx INTR
			RxTIMEOUT = 0;					// We've seen the whole PACKET so STOP Timeout
			os_send_signal(CONFIG); 	 	// WAKE up sleeping CONFIG task
  			}
		}
	else{	// normal packet!
		if(buff_ptr == MaxConfig){	      	// MaxConfig when we have 6 flag a new buffer
			ASC0_CON_REN = OFF;      		// disable RCVR                 *
			ASC0_RIC_IE = OFF;      		// disable Rx INTR
			RxTIMEOUT = 0;					// We've seen the whole PACKET so STOP Timeout
			os_send_signal(CONFIG); 	 	// WAKE up sleeping CONFIG task
			}
		}
 */


/****
 * void serial_ISR(void *context)
{
  int sr = IORD_ALTERA_AVALON_UART_STATUS (SERIAL_IO_BASE);	// Save the status register value
  int cr = IORD_ALTERA_AVALON_UART_CONTROL (SERIAL_IO_BASE);// Save the control register value

  // *** Transmit Interrupt ********************************************
  if(sr & ALTERA_AVALON_UART_STATUS_TRDY_MSK)				// if TxRDY
  {
	  if(cr & ALTERA_AVALON_UART_CONTROL_TRDY_MSK)			// if Tx Intr is Enabled
	  {
		  if (TxEnd != TxStart)								// if transmit buffer is empty
		  {
			  IOWR_ALTERA_AVALON_UART_TXDATA(SERIAL_IO_BASE ,TxBuf[TxEnd]); // Tx the last char in the TxBuffer
			  if (++TxEnd > (TXBUFSIZE - 1))		        //If end of buffer, set Tx buffer to 0
				  TxEnd = 0;
          }
          else	// Else if no data to Tx, disable transmit interrupt
          {
        	  IOWR_ALTERA_AVALON_UART_CONTROL(SERIAL_IO_BASE, 0x80|0x10);
          }
      }
  }//End of Tx interrupt processing


  // *** Receive Interrupt ********************************************
  if(sr & ALTERA_AVALON_UART_STATUS_RRDY_MSK)				// if RxRDY
  {
	  RxBuf[RxStart] = IORD_ALTERA_AVALON_UART_RXDATA(SERIAL_IO_BASE);	// Load new Rx data into RxBuffer

      if ((++RxStart) > (RXBUFSIZE - 1))	//if the RxStart index value to the end of the array return array head
    	  RxStart = 0;

      IOWR_ALTERA_AVALON_UART_STATUS(SERIAL_IO_BASE, 0);	// disable Receive interrupt
  } //End of Rx interrupt processing
}
 *
 * **************************/


