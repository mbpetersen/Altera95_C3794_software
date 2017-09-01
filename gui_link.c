/*************************************************************************
 *		Config GUI<-->GIGE Communication Link							 *
 *				   														 *
 * Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
 * All rights reserved.                                                   *
 *************************************************************************/
#include "gigle.h"
#include "extvars.h"

//********************************************************************************
//
//********************************************************************************
void config_gui_link() {
	unsigned short i = 0;
	//static _Bool toggle_flag=0;
	_Bool good = NO;

//	D(1, BUG("gui_link (gige config): %x\n", RxBuffer[GIGEconfig_ptr]));

	//=********************************************************************
	//=*** Run a checksum test on the new buffer with the checksum byte ***
	//=********************************************************************
	good = NO;
	//	if(toggle_flag^=ON)
	//		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,0x03);	// set LED ON
	//	else
	//		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,0x00);	// set LED Off
	if (MS_PKT) {// the test can be running so ms_pkt  =true but you can also rcv smaller packets so can't use (ms_pkt) here
		//D(1, BUG( "\n<> MS packet true \n"));
		if (CalcChecksum(RxBuffer, MaxConfig_ms) == 0) { // matching checksums is 0 !
			D(1, BUG( "\n<> MS good packet \n"));
			good = YES;
		}
	} else if (ETH_PKT){
		if(CalcChecksum(RxBuffer, MaxConfig2) == 0) { // matching checksums is 0 !
			good = YES;
		//	D(1, BUG("GOOD incoming GUI config packet\n"));

			if (Rx_PACKET_ID != RxBuffer[ID_ptr]) //*** If we have a NEW/different PACKET TYPE then...
			{	D(1, BUG("ConfigStat[GIGEconfig_ptr] |= 0x10; // Send Re-Sync LINK flag pDA\n"));
				ConfigStat[GIGEconfig_ptr] |= 0x10; // Send Re-Sync LINK flag to PDA
			}
			Rx_PACKET_ID = RxBuffer[ID_ptr]; // Grab and save Rcvd PACKET ID!
			//	D(1, BUG("MISC pointer value (link:%02X)\n",RxBuffer[67]));
			if (Rx_PACKET_ID == Packet_GIGE10A) {
				GIGE10 = YES;
				GIGE_A = NO;
			}
			else{
				GIGE10 = NO;
				if (Rx_PACKET_ID == Packet_GIGEA) {
					GIGE_A = YES;
					set_10G_clock_source(INT156_CLOCK);
				} else
					GIGE_A = NO;
			}
		}

		//LINKFAULTLED_OFF();
		//IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,0x00);	// set LED Off
	} else if(CalcChecksum(RxBuffer, MaxConfig) == 0) { // matching checksums is 0 ! { // If Checksum fails - hold LED ON
		D(1, BUG("Incoming STD config packet (C37.94)\n"));
		good = YES;
		Rx_PACKET_ID = Packet_C3794;
	} else{
		//LINKFAULTLED_ON();
		//IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,0x03);	// set LED ON
		D(1, BUG("Incoming GUI config packet: BAD checksum!\n"));
	}
	//=********************************************************************


	//=*******************************************************************
	//=*** Handle DirtyBits:  GUI <=> MOD   to ensure Same Config's ******
	//=*******************************************************************
	//=*******************************************************************
	if (Rx_PACKET_ID == Packet_GIGEA) { // establish link ONLY on Correct packets
		//=*******************************************************************
		//	D(1, BUG("GIGEconfig_ptr value RCVD:0%02x\n",ConfigStat[GIGEconfig_ptr]));
		if (ReSyncFlag || RxReSyncFlag) { // If Tx ReSync or Rx ReSync...
			if (RxReSyncFlag) {
				// If We see the PDA saw it, then...
				//D(1, BUG("GIGEconfig_ptr value before 40:0%02x\n",ConfigStat[GIGEconfig_ptr]));
				ConfigStat[GIGEconfig_ptr] |= 0x40; // we know DirtyPDA is clear so start pump (set DirtyMOD)
				//D(1, BUG("GIGEconfig_ptr value after 40:0%02x\n",ConfigStat[GIGEconfig_ptr]));
				ConfigStat[GIGEconfig_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				//	D(1, BUG("GIGEconfig_ptr value after clear 80:0%02x\n",ConfigStat[GIGEconfig_ptr]));
				ConfigStat[GIGEconfig_ptr] &= ~0x10; // ...and Clear Tx ReSyncFlag
				//	D(1, BUG("GIGEconfig_ptr value clear 10:0%02x\n",ConfigStat[GIGEconfig_ptr]));
				D(1, BUG("A: RxReSyncFlag. LINK:0%02x\n",ConfigStat[GIGEconfig_ptr]));
			}
		} else if (good) { // Only DO DirtyBIT handling if Good Packet
			if (RxDirtyPDABit) { // if RxDirtyBit is Set, PDA is current to Module Config
				if (RxDirtyModBit) { // if DirtyModBit Set Clear it as PDA has already updated it's config to match MOD
					//	D(1, BUG("GIGEconfig_ptr value ReSync GOOD:0%02x\n",ConfigStat[GIGEconfig_ptr]));
					ConfigStat[GIGEconfig_ptr] &= ~0x40;
					D(1, BUG("A: RxDirtyGUIBit+RxDirtyModBit. LINK:0%02x\n",ConfigStat[GIGEconfig_ptr]));

					//On speed change from 10G to TSE.....
					//reset num_packets & load% calculations...........
					//SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		//Call on: Frame Size, TestSecs, FPS change or SPEED change
					//SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);							// called on Frame Size, FPS change or SPEED
				} else { // If DirtyGUI=1 && DirtyMOD==0
					if (RxUpdateFlag) { // and RxUpdateFlag then.....
						ConfigStat[GIGEconfig_ptr] |= 0x20; // Re turn UpdateFlag in same state as received
					//	D(1, BUG("A: RxUpdateFlag. LINK:0%02x\n",ConfigStat[GIGEconfig_ptr]));

					} else {
						ConfigStat[GIGEconfig_ptr] &= ~0x20; // Return UpdateFlag in same state as received
						//D(1, BUG("NOT RxUpdateFlag. LINK:0%02x\n",ConfigStat[GIGEconfig_ptr]));
					}
				}
				ConfigStat[GIGEconfig_ptr] |= 0x80; // Return DirtyPDA in same state as received
			//	D(1, BUG("Return DirtyPDA in same state as received. LINK:0%02x\n",ConfigStat[GIGEconfig_ptr]));
			} else { // if RxDirtyGUIBit clear then set DirtyModBit to Tell GUI to ReConfig to MOD's config
				ConfigStat[GIGEconfig_ptr] |= 0x40; // RxDirtyGUI Clear so Set DirtyMODBit to GUI
				ConfigStat[GIGEconfig_ptr] &= ~0x80; // Return DirtyGUI in same state as received
				//D(1, BUG("A: Not DirtyGUI. LINK:0%02x\n",ConfigStat[GIGEconfig_ptr]));
				D(1, BUG("A: LINK UP:0%02x\n",ConfigStat[GIGEconfig_ptr]));
				if(MS_PKT_RCVD){SEND_MS_PACKET=YES;} //send out a big packet on link up
			}
		}
	}// End establish link on GIGEA packets
	//==================================================
	//=*******************************************************************
	else if (Rx_PACKET_ID == Packet_GIGEB) { // establish link ONLY on Correct packets
		//=*******************************************************************
		if (ReSyncFlagB || RxReSyncFlag) { // If Tx ReSync or Rx ReSync...
			if (RxReSyncFlag) { // If We see the GUI saw it, then...
				ConfigStatB[GIGEconfig_ptr] |= 0x40; // we know DirtyPDA is clear so start pump (set DirtyMOD)
				ConfigStatB[GIGEconfig_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				ConfigStatB[GIGEconfig_ptr] &= ~0x10; // ...and Clear Tx ReSyncFlag
				D(1, BUG("B: RxReSyncFlag. LINK:0%02x\n",ConfigStatB[GIGEconfig_ptr]));
			}
		} else if (good) { // Only DO DirtyBIT handling if Good Packet
			if (RxDirtyPDABit) { // if RxDirtyBit is Set, PDA is current to Module Config
				if (RxDirtyModBit) { // if DirtyModBit Set Clear it as PDA has already updated it's config to match MOD
					ConfigStatB[GIGEconfig_ptr] &= ~0x40;
					D(1, BUG("B: RxDirtyGUIBit+RxDirtyModBit. LINK:0%02x\n",ConfigStatB[GIGEconfig_ptr]));
				} else { // If DirtyPDA=1 && DirtyMOD==0
					if (RxUpdateFlag) { // and RxUpdateFlag then.....
						ConfigStatB[GIGEconfig_ptr] |= 0x20; // Return UpdateFlag in same state as received
						D(1, BUG("B: RxUpdateFlag. LINK:0%02x\n",ConfigStatB[GIGEconfig_ptr]));
					} else
						ConfigStatB[GIGEconfig_ptr] &= ~0x20; // Return UpdateFlag in same state as received
				}
				ConfigStatB[GIGEconfig_ptr] |= 0x80; // Return DirtyPDA in same state as received
			} else { // if RxDirtyPDABit clear then set DirtyModBit to Tell PDA to ReConfig to MOD's config
				ConfigStatB[GIGEconfig_ptr] |= 0x40; // RxDirtyPDA Clear so Set DirtyMODBit to PDA
				ConfigStatB[GIGEconfig_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				D(1, BUG("B: Not DirtyGUI. LINK:0%02x\n",ConfigStatB[GIGEconfig_ptr]));

			}
		}
	}// End establish link on GIGEB packets
	//==================================================
	//=*******************************************************************
	else if (Rx_PACKET_ID == Packet_GIGE10A) { // establish link ONLY on Correct packets
		//=*******************************************************************
		if (ReSyncFlag10G || RxReSyncFlag) { // If Tx ReSync or Rx ReSync...
			if (RxReSyncFlag) { // If We see the PDA saw it, then...
				ConfigStat10G[GIGEconfig_ptr] |= 0x40; // we know DirtyPDA is clear so start pump (set DirtyMOD)
				ConfigStat10G[GIGEconfig_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				ConfigStat10G[GIGEconfig_ptr] &= ~0x10; // ...and Clear Tx ReSyncFlag
				D(1, BUG("10G-A: RxReSyncFlag. LINK:0%02x\n",ConfigStat10G[GIGEconfig_ptr]));
			}
		} else if (good) { // Only DO DirtyBIT handling if Good Packet
			if (RxDirtyPDABit) { // if RxDirtyBit is Set, PDA is current to Module Config
				if (RxDirtyModBit) { // if DirtyModBit Set Clear it as PDA has already updated it's config to match MOD
					ConfigStat10G[GIGEconfig_ptr] &= ~0x40;
					D(1, BUG("10G-A: RxDirtyGUIBit+RxDirtyModBit. LINK:0%02x\n",ConfigStat10G[GIGEconfig_ptr]));
					//On speed change from TSE to 10G.....
					//reset num_packets & load% calculations...........
					//SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		//Call on: Frame Size, TestSecs, FPS change or SPEED change
					//SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);							// called on Frame Size, FPS change or SPEED
				} else { // If DirtyPDA=1 && DirtyMOD==0
					if (RxUpdateFlag) { // and RxUpdateFlag then.....
						ConfigStat10G[GIGEconfig_ptr] |= 0x20; // Return UpdateFlag in same state as received
						D(1, BUG("10G-A: RxUpdateFlag. LINK:0%02x\n",ConfigStat10G[GIGEconfig_ptr]));
					} else
						ConfigStat10G[GIGEconfig_ptr] &= ~0x20; // Return UpdateFlag in same state as received
				}
				ConfigStat10G[GIGEconfig_ptr] |= 0x80; // Return DirtyPDA in same state as received
			} else { // if RxDirtyPDABit clear then set DirtyModBit to Tell PDA to ReConfig to MOD's config
				ConfigStat10G[GIGEconfig_ptr] |= 0x40; // RxDirtyPDA Clear so Set DirtyMODBit to PDA
				ConfigStat10G[GIGEconfig_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				D(1, BUG("10G-A: Not DirtyGUI. LINK:0%02x\n",ConfigStat10G[GIGEconfig_ptr]));

			}
		}
	}// End establish link on GIGE 10G Port A packets
	else if (Rx_PACKET_ID == Packet_C3794) { // establish link ONLY on Correct packets
		//=*******************************************************************
		if (ReSyncFlagC37 || RxReSyncFlag) { // If Tx ReSync or Rx ReSync...
			if (RxReSyncFlag) { // If We see the GUI saw it, then...
				ConfigStatC37[C37config_ptr] |= 0x40; // we know DirtyPDA is clear so start pump (set DirtyMOD)
				ConfigStatC37[C37config_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				ConfigStatC37[C37config_ptr] &= ~0x10; // ...and Clear Tx ReSyncFlag
				D(1, BUG("B: RxReSyncFlag. LINK:0%02x\n",ConfigStatB[C37config_ptr]));
			}
		} else if (good) { // Only DO DirtyBIT handling if Good Packet
			if (RxDirtyPDABit) { // if RxDirtyBit is Set, PDA is current to Module Config
				if (RxDirtyModBit) { // if DirtyModBit Set Clear it as PDA has already updated it's config to match MOD
					ConfigStatC37[C37config_ptr] &= ~0x40;
					D(1, BUG("B: RxDirtyGUIBit+RxDirtyModBit. LINK:0%02x\n",ConfigStatC37[C37config_ptr]));
				} else { // If DirtyPDA=1 && DirtyMOD==0
					if (RxUpdateFlag) { // and RxUpdateFlag then.....
						ConfigStatC37[C37config_ptr] |= 0x20; // Return UpdateFlag in same state as received
						D(1, BUG("B: RxUpdateFlag. LINK:0%02x\n",ConfigStatC37[C37config_ptr]));
					} else
						ConfigStatC37[C37config_ptr] &= ~0x20; // Return UpdateFlag in same state as received
				}
				ConfigStatC37[C37config_ptr] |= 0x80; // Return DirtyPDA in same state as received
			} else { // if RxDirtyPDABit clear then set DirtyModBit to Tell PDA to ReConfig to MOD's config
				ConfigStatC37[C37config_ptr] |= 0x40; // RxDirtyPDA Clear so Set DirtyMODBit to PDA
				ConfigStatC37[C37config_ptr] &= ~0x80; // Return DirtyPDA in same state as received
				D(1, BUG("B: Not DirtyGUI. LINK:0%02x\n",ConfigStatC37[C37config_ptr]));

			}
		}
	}// End establish link on C37.94 packets
	//==================================================
	else{
		D(1, BUG("NON-gigE ID Rcvd, ID:%02x, LINK A:0%02x\n",RxBuffer[ID_ptr],ConfigStat[GIGEconfig_ptr]));
		D(1, BUG("NON-gigE ID Rcvd, ID:%02x, LINK 10G:0%02x\n",RxBuffer[ID_ptr],ConfigStat10G[GIGEconfig_ptr]));
		D(1, BUG("NON-gigE ID Rcvd, ID:%02x, LINK B:0%02x\n",RxBuffer[ID_ptr],ConfigStatB[GIGEconfig_ptr]));
	}
	//	D(1, BUG("Rcvd val of strtstop:%x and val of tststoppedbit (&0x40):%x\n",ConfigStat[STARTSTOP_ptr],TestStoppedModBit));}

	//=***********************************************************************
	//=*** Update GigE Board Boot & Init & ? Fault Codes: GIGE_STATUS     ****
	//=***********************************************************************
	ConfigStat[GIGERESULT_ptr] = GIGE_STATUS;
	ConfigStatB[GIGERESULT_ptr] = GIGE_STATUS;
	ConfigStat10G[GIGERESULT_ptr] = GIGE_STATUS;
	//=***********************************************************************


	//=***********************************************************************
	//=*** Send out ConfigStat array to the PDA ** IF it's a valid ID ptr ****
	//=***********************************************************************
	//todo dont we have to add the if sent then clear part of the code for this?

	//=***********************************************************************

	if(Rx_PACKET_ID == Packet_GIGEA) {
		//=***********************************************************************
		// if REQ SFP info DUMP, read and dump SpecialStat packet to GUI, and...
		// ...if LINK is not fully up Wait till it is & then Xmt specialStat!
		//  D(1, BUG("inA:----> B4 MS_PKT_REQ && Config==(link:%02X)\n",RxBuffer[0]));
		 if(SEND_MS_PACKET && ValidConfig==0x80){  //chk MS_PKT_RVCD b4 setting SEND_MS_CONFIG
			if(!SENDING_MS_CONFIG){
			  D(1, BUG("A:----> MS_PKT_REQ && ValidConfig==0x80(link:%02X)\n",RxBuffer[0]));
			  ConfigMS[GIGEconfig_ptr] = ConfigStat[GIGEconfig_ptr];      // Maintain LINK handler byte
			  ConfigMS[ID_ptr] = Packet_MultiStream;                            // Send out MISC Stat Packet ID
			  for(i=0;i<MaxConfig_ms;i++)                     //load digit array into TxBuffer
					TxBuffer[i] = ConfigMS[i];
			  TxBuffer[MaxConfig_ms-1] = CalcChecksum(TxBuffer,MaxConfig_ms-1);// calculate extended checksum location too
			  SENDING_MS_CONFIG = YES;
			  //TxBuffer[EGEN_NUM_SECS3_ptr]
				D(1, BUG("A:----> Sending MS Packet on link up (link:%02X)\n",RxBuffer[0]));
			/*	D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG01_ptr] 0%0d\n",TRAFFIC_GEN_CONFIG01_ptr));
				stream_offset = 12;
				update_headers(0);
				D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG01_ptr]- packet length stream 0%0d\n",BytesToInt(ConfigMS,TRAFFIC_GEN_CONFIG01_ptr)));
				D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG01_ptr]- packet length stream 1%0d\n",BytesToInt(ConfigMS,TRAFFIC_GEN_CONFIG01_ptr)));
				D(1, BUG("\tTxBuffer[TRAFFIC_GEN_CONFIG01_ptr]- packet length stream 0%0d\n",BytesToInt(TxBuffer,TRAFFIC_GEN_CONFIG01_ptr)));
				D(1, BUG("\tTxBuffer[TRAFFIC_GEN_CONFIG01_ptr]- packet length stream 1%0d\n",BytesToInt(TxBuffer,TRAFFIC_GEN_CONFIG01_ptr)));
				D(1, BUG("\tTxBuffer[TRAFFIC_RATE_3_ptr]- packet length stream %0x\n",BytesToInt(TxBuffer,TRAFFIC_RATE3_ptr)));
				D(1, BUG("\tTxBuffer[TRAFFIC_RATE_2_ptr]- packet length stream %0x\n",BytesToInt(TxBuffer,TRAFFIC_RATE2_ptr)));
				D(1, BUG("\tTxBuffer[TRAFFIC_RATE_1_ptr]- packet length stream %0x\n",BytesToInt(TxBuffer,TRAFFIC_RATE1_ptr)));
				D(1, BUG("\tTxBuffer[TRAFFIC_RATE_0_ptr]- packet length stream %0x\n",BytesToInt(TxBuffer,TRAFFIC_RATE0_ptr)));
				D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG01_ptr] 0%0d\n",TRAFFIC_GEN_CONFIG01_ptr));
				stream_offset = 101;*/
			/*	int op=0;
				for(op=0;op<2;op++){
				update_headers(op);
				traffic_gen_config = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG03_ptr);
				traffic_gen_config2 = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG13_ptr);
				traffic_gen_config3 = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG23_ptr);
				D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG03_ptr] %x\n",traffic_gen_config));
				D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG13_ptr] %x\n",traffic_gen_config2));
				D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG23_ptr] %x\n",traffic_gen_config3));

				traffic_gen_config = BytesToLong(TxBuffer, TRAFFIC_GEN_CONFIG03_ptr);
				traffic_gen_config2 = BytesToLong(TxBuffer, TRAFFIC_GEN_CONFIG13_ptr);
				traffic_gen_config3 = BytesToLong(TxBuffer, TRAFFIC_GEN_CONFIG23_ptr);
				D(1, BUG("\tTxBuffer[TRAFFIC_GEN_CONFIG03_ptr] %x\n",traffic_gen_config));
				D(1, BUG("\tTxBuffer[TRAFFIC_GEN_CONFIG13_ptr] %x\n",traffic_gen_config2));
				D(1, BUG("\tTxBuffer[TRAFFIC_GEN_CONFIG23_ptr] %x\n",traffic_gen_config3));
				}*/
				//D(1, BUG("Gigeconfig_ptr[ID_ptr]:0%02x value SENT\n",TxBuffer[EGEN_NUM_SECS3_ptr]));
				tbuff_ptr = 0; // RESET Tx Data to GUI buffer pointer
				IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE,TxBuffer[tbuff_ptr++]);// prime the pump!
				UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_TRDY_MSK; // Enable the serial port Transmit interrupt
				IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER*/
			  }
			}
		else if(SEND_SFPA_DATA && ValidConfig == 0x80) {
			SpecialStat[GIGEconfig_ptr] = ConfigStat[GIGEconfig_ptr]; // Maintain LINK handler byte
			SpecialStat[ID_ptr] = Packet_GIGEA_MISC; // Send out MISC Stat Packet ID
			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = SpecialStat[i];
			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFPA_DATA = NO;
			D(1, BUG("A: Sending Port-A SpecialStat[] Packet (link:%02X)\n",RxBuffer[0]));
		}//End Async MISC-STAT (Port-A SFP data) reply packet
		else if(SEND_SFPB_DATA && ValidConfig == 0x80) {
			SpecialStatB[GIGEconfig_ptr] = ConfigStat[GIGEconfig_ptr]; // Maintain LINK handler byte
			SpecialStatB[ID_ptr] = Packet_GIGEB_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = SpecialStatB[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFPB_DATA = NO;
			D(1, BUG("A: Sending Port-B SpecialStatB[] Packet (link:%02X)\n",RxBuffer[0]));
		}//End Async MISC-STAT (Port-B SFP data) reply packet
		else if(SEND_SFP10A_DATA && ValidConfig == 0x80) {
			Special10GStat[GIGEconfig_ptr] = ConfigStat[GIGEconfig_ptr]; // Maintain LINK handler byte
			Special10GStat[ID_ptr] = Packet_GIGE10A_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = Special10GStat[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFP10A_DATA = NO;
			D(1, BUG("A:----> Sending 10G Port-A Special10GStat[] Packet (link:%02X)\n",RxBuffer[0]));
		}//End Async MISC-STAT (Port-A SFP data) reply packet

		//=============================================*/
		else if(SEND_LPBK_DATA && ValidConfig == 0x80){
			LoopbackStat[GIGEconfig_ptr] = ConfigStat[GIGEconfig_ptr];	// Maintain LINK handler byte
			LoopbackStat[ID_ptr] = Packet_LOOPBACK;  					// Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) 				//load digit array into TxBuffer
				TxBuffer[i] = LoopbackStat[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_LPBK_DATA = NO;
			// Clear LoopbackStat array
			loopbackStatPtr = 0;
			while (loopbackStatPtr < 242) {
//				int z;
//				D(1, BUG("loopbackstat[%d]: ", loopbackStatPtr));
//				for (z = loopbackStatPtr; z < loopbackStatPtr + 4; z++) {
//					D(1, BUG("%02x", LoopbackStat[z]));
//				}
//				D(1, BUG("\n"));
				SaveBytesLong(LoopbackStat, loopbackStatPtr, 0);
				loopbackStatPtr += 4;
			}
			loopbackStatPtr = 2;
			D(1, BUG("LOOPBACKSTAT CLEARED\n"));

			D(1, BUG("A: Sending Port-A LoopbackStat[] Packet\n"));
		}//End Async MISC-STAT (Port-A SFP data) reply packet
		else { // Transmit normal GigE ConfigStat packet
			//D(1, BUG("Transmit normal GigE ConfigStat packet\n"));
			ConfigStat[ID_ptr] = Packet_GIGEA; // Send out ConfigStatus Packet ID

//			if(((RxBuffer[STARTSTOP_ptr] & 0x30) == 0) && (TxBuffer[STARTSTOP_ptr] == 0x50)) {	// Prevent test results from being displayed twice
			if (RxTestDoneBit) {
				ConfigStat[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag(need to clear otherwise its stuck at x50)
//				D(1, BUG(">>(0x40) Test AUTO-stop Flag's cleared (A)Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));
			}

		//	D(1, BUG("GIGEconfig_ptr value SENT:0%02x\n",ConfigStat[0]));

			for(i=1;i<MaxStatus2;i++) // load ConfigStat array into TxBuffer
				TxBuffer[i] = ConfigStat[i];

//			D(1, BUG("ConfigStat[TRAFFIC_MON_NUM_RCVD_PACKET03_ptr]: %x\n", BytesToLong(ConfigStat, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr)));
//			D(1, BUG("TxBuffer[TRAFFIC_MON_NUM_RCVD_PACKET03_ptr]: %x\n", BytesToLong(TxBuffer, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr)));
//
//			D(1, BUG("ConfigStat[TRAFFIC_NUM_OF_STREAMS_ptr]: %x\n", (ConfigStat[TRAFFIC_NUM_OF_STREAMS])));
//			D(1, BUG("RxBuffer[TRAFFIC_NUM_OF_STREAMS_ptr]: %x\n", (RxBuffer[TRAFFIC_NUM_OF_STREAMS])));

			TxBuffer[GIGEconfig_ptr] = ConfigStat[GIGEconfig_ptr]; // Maintain DS1A link handler
			//	D(1, BUG("GIGEconfig_ptr Maintain DS1A link handler:0%02x\n",ConfigStat[GIGEconfig_ptr]));
			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			//	D(1, BUG("905 sent"));

//			D(1, BUG("RxBuffer[gigeCONFIG_PTR]: %x\n", RxBuffer[GIGEconfig_ptr]));
//			D(1, BUG("TxBuffer[GIGEconfig_ptr]: %x\n", TxBuffer[GIGEconfig_ptr]));

//			D(1, BUG("TXBUFFER STARTSTOP: %x\n\n", TxBuffer[STARTSTOP_ptr]));
//			D(1, BUG("CONFIGSTAT STARTSTOP: %x\n\n", ConfigStat[STARTSTOP_ptr]));

			// TODO
			// don't clear here - clear when rcv an ack from GUI that it's been seen....
			// temp commmented out for test....
			//D(1, BUG(">>(0x40) Test stopped mod bit val of strtstop:%x and val of tststoppedbit (&0x40):%x\n",ConfigStat[STARTSTOP_ptr],TestStoppedModBit));
			//This is not required as the FW can keep sending 0x40 and its the UI's job to recognize the 0x40 and then send a 0x00 to indicate the job. Clearing this value makes the FW Tx 0x10 which might be detected by the UI as start of a test.
			/*if(TestStoppedModBit) {
				ConfigStat[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag(need to clear otherwise its stuck at x50)
				D(1, BUG(">>(0x40) Test AUTO-stop Flag's cleared (A)Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));
			}
			if(TestStoppedModBitB) {
				ConfigStatB[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag
			}*/
			//if(TestStoppedModBit10)
			//	ConfigStat10G[STARTSTOP_ptr] &= ~0x40;	// if SENT, clear Test DONE Flag
			/**************************************************************/
		}//End Standard ConfigStat reply packet

		if(!SENDING_MS_CONFIG){
			tbuff_ptr = 0; // RESET Tx Data to GUI buffer pointer
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE,TxBuffer[tbuff_ptr++]);// prime the pump!
			UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_TRDY_MSK; // Enable the serial port Transmit interrupt
			IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
		}
	}
	//**************************************************************************
	else if(Rx_PACKET_ID == Packet_GIGEB) {
		//=***********************************************************************
		// if REQ SFP info DUMP, read and dump SpecialStat packet to GUI, and...
		// ...if LINK is not fully up Wait till it is & then Xmt specialStat!
		if(SEND_SFPA_DATA && ValidConfig == 0x80) {
			SpecialStat[GIGEconfig_ptr] = ConfigStatB[GIGEconfig_ptr]; // Maintain LINK handler byte
			SpecialStat[ID_ptr] = Packet_GIGEA_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = SpecialStat[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFPA_DATA = NO;
			D(1, BUG("B: Sending Port-A SpecialStat[] Packet\n"));
		}//End Async MISC-STAT (Port-A SFP data) reply packet
		else if(SEND_SFPB_DATA && ValidConfig == 0x80) {
			SpecialStatB[GIGEconfig_ptr] = ConfigStatB[GIGEconfig_ptr]; // Maintain LINK handler byte
			SpecialStatB[ID_ptr] = Packet_GIGEB_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = SpecialStatB[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFPB_DATA = NO;
			D(1, BUG("B: Sending Port-B SpecialStatB[] Packet\n"));
		}//End Async MISC-STAT (Port-B SFP data) reply packet
		else if(SEND_SFP10A_DATA && ValidConfig == 0x80) {
			Special10GStat[GIGEconfig_ptr] = ConfigStatB[GIGEconfig_ptr]; // Maintain LINK handler byte
			Special10GStat[ID_ptr] = Packet_GIGE10A_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = Special10GStat[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFP10A_DATA = NO;
			D(1, BUG("B: ----> Sending 10G Port-A Special10GStat[] Packet\n"));
		}//End Async MISC-STAT (Port-A SFP data) reply packet
		//=============================================
		else { // Transmit normal GigE ConfigStat packet
			ConfigStatB[ID_ptr] = Packet_GIGEB; // Send out ConfigStatus Packet ID

			for(i=1;i<MaxStatus2;i++) // load ConfigStat array into TxBuffer
			TxBuffer[i] = ConfigStatB[i];

			TxBuffer[GIGEconfig_ptr] = ConfigStatB[GIGEconfig_ptr]; // Maintain DS1A link handler
			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			/**************************************************************/

		/*	if(TestStoppedModBitB) {
				ConfigStatB[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag
				D(1, BUG(">>(0x40) Test AUTO-stop Flag's cleared (B) Tx:%x\n",ConfigStatB[STARTSTOP_ptr]));
			}
			if(TestStoppedModBit) {
				ConfigStat[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag
			}*/
			//if(TestStoppedModBit10)
			//	ConfigStat10G[STARTSTOP_ptr] &= ~0x40;	// if SENT, clear Test DONE Flag
		}

		tbuff_ptr = 0; // RESET Tx Data to GUI buffer pointer
		IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE,TxBuffer[tbuff_ptr++]);// prime the pump!
		UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_TRDY_MSK; // Enable the serial port Transmit interrupt
		IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
	}
	//=***********************************************************************
	//**************************************************************************
	else if(Rx_PACKET_ID == Packet_GIGE10A) {
		//=***********************************************************************
		// if REQ SFP info DUMP, read and dump SpecialStat packet to GUI, and...
		// ...if LINK is not fully up Wait till it is & then Xmt specialStat!
		if(SEND_SFPA_DATA && ValidConfig == 0x80) {
			SpecialStat[GIGEconfig_ptr] = ConfigStat10G[GIGEconfig_ptr]; // Maintain LINK handler byte
			SpecialStat[ID_ptr] = Packet_GIGEA_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = SpecialStat[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFPA_DATA = NO;
			D(1, BUG("10A: Sending Port-A SpecialStat[] Packet\n"));
		}//End Async MISC-STAT (Port-A SFP data) reply packet
		else if(SEND_SFPB_DATA && ValidConfig == 0x80) {
			SpecialStatB[GIGEconfig_ptr] = ConfigStat10G[GIGEconfig_ptr]; // Maintain LINK handler byte
			SpecialStatB[ID_ptr] = Packet_GIGEB_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = SpecialStatB[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFPB_DATA = NO;
			D(1, BUG("10A: Sending Port-B SpecialStatB[] Packet\n"));
		}//End Async MISC-STAT (Port-B SFP data) reply packet
		else if(SEND_SFP10A_DATA && ValidConfig == 0x80) {
			Special10GStat[GIGEconfig_ptr] = ConfigStat10G[GIGEconfig_ptr]; // Maintain LINK handler byte
			Special10GStat[ID_ptr] = Packet_GIGE10A_MISC; // Send out MISC Stat Packet ID

			for(i=0;i<MaxStatus2;i++) //load digit array into TxBuffer
			TxBuffer[i] = Special10GStat[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_SFP10A_DATA = NO;
			D(1, BUG("10A:----> Sending 10G Port-A Special10GStat[] Packet\n"));
		}
		//End Async MISC-STAT (Port-A SFP data) reply packet
		/*** incorporate when we have a 10G B port
		 else if(SEND_SFP10B_DATA && ValidConfig == 0x80){
		 Special10GStatB[GIGEconfig_ptr] = ConfigStat10G[GIGEconfig_ptr];	// Maintain LINK handler byte
		 Special10GStatB[ID_ptr] = Packet_GIGE10B_MISC;  					// Send out MISC Stat Packet ID

		 for(i=0;i<MaxStatus2;i++) 				//load digit array into TxBuffer
		 TxBuffer[i] = Special10GStatB[i];

		 TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
		 SEND_SFP10B_DATA = NO;
		 D(1, BUG("Sending 10G Port-B Special10GStatB[] Packet\n"));
		 }//End Async MISC-STAT (Port-B SFP data) reply packet
		 ***/
		else if(SEND_LPBK10G_DATA && ValidConfig == 0x80){
			LoopbackStat10G[GIGEconfig_ptr] = ConfigStat10G[GIGEconfig_ptr];	// Maintain LINK handler byte
			LoopbackStat10G[ID_ptr] = Packet_LOOPBACK;  					// Send out MISC Stat Packet ID

			for(i=0; i<MaxStatus2; i++) 				//load digit array into TxBuffer
				TxBuffer[i] = LoopbackStat10G[i];

			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			SEND_LPBK10G_DATA = NO;

			int loopbackStatPtr = 0;
			while (loopbackStatPtr < 242) {
				SaveBytesLong(LoopbackStat10G, loopbackStatPtr, 0);
				loopbackStatPtr += 4;
			}
			D(1, BUG("LOOPBACKSTAT10G CLEARED\n"));

			D(1, BUG("10A: Sending 10G Port-A LoopbackStat10G[] Packet\n"));
		}//End Async MISC-STAT (Port-B SFP data) reply packet
		//=============================================
		else { // Transmit normal GigE ConfigStat packet
			ConfigStat10G[ID_ptr] = Packet_GIGE10A; // Send out ConfigStatus Packet ID

			//if(((RxBuffer[STARTSTOP_ptr] & 0x30) == 0) && (TxBuffer[STARTSTOP_ptr] == 0x50)) {	// Prevent test results from being displayed twice
			if (RxTestDoneBit) {
				ConfigStat10G[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag(need to clear otherwise its stuck at x50)
//				D(1, BUG(">>(0x40) Test AUTO-stop Flag's cleared (A)Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));
			}

			for(i=1;i<MaxStatus2;i++) // load ConfigStat array into TxBuffer
				TxBuffer[i] = ConfigStat10G[i];

//			D(1, BUG("TXBUFFER STARTSTOP: %x\n\n", TxBuffer[STARTSTOP_ptr]));

			TxBuffer[GIGEconfig_ptr] = ConfigStat10G[GIGEconfig_ptr]; // Maintain DS1A link handler
			TxBuffer[StatusChecksum2_ptr] = CalcChecksum(TxBuffer,MaxStatus2-1);// calculate extended checksum location too
			/**************************************************************/

			/*if(TestStoppedModBit10) {
				ConfigStat10G[STARTSTOP_ptr] &= ~0x40; // if SENT, clear Test DONE Flag
				D(1, BUG(">>(0x40) 10G Test AUTO-stop Flag's cleared (A) Tx:%x\n",ConfigStatB[STARTSTOP_ptr]));
			}*/
			//if(TestStoppedModBit){
			//	ConfigStat[STARTSTOP_ptr] &= ~0x40;		// if SENT, clear Test DONE Flag
			//	}
			//if(TestStoppedModBitB){
			//	ConfigStatB[STARTSTOP_ptr] &= ~0x40;	// if SENT, clear Test DONE Flag
			//	}
		}

		tbuff_ptr = 0; // RESET Tx Data to GUI buffer pointer
		IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE,TxBuffer[tbuff_ptr++]);// prime the pump!
		UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_TRDY_MSK; // Enable the serial port Transmit interrupt
		IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
	}
	//=***********************************************************************        
	//**************************************************************************
	//	else if(Rx_PACKET_ID == Packet_GIGE10B){  /not implemented yet....
	//=***********************************************************************
	//		}
	//=***********************************************************************
	//=********************************************************************
	if(MS_PKT) {
		//if(good && !ReSyncFlag){						// We have a VALID (good checksum) Config Packet from the PDA
		if(good) {
			//	if(RxDirtyPDABit && RxUpdateFlag){  		// if RxDirtyBit is Set, PDA is current to Module Config
			D(1, BUG( "\n<> going into config_ms \n"));
			config_multistream(); // If UpdateFlag is set: we have a NEW config to perform
			MS_TEST_ACTIVE = YES;
			MS_PKT = NO;
			//	}
		}//End DS1A packet execution

	}
	else if(ETH_PKT){
		ETH_PKT = NO;
		//=********************************************************************
		if(Rx_PACKET_ID == Packet_GIGEA) { // GIGE Packet - Port A
			//=********************************************************************
			if(good && !ReSyncFlag) { // We have a VALID (good checksum) Config Packet from the PDA
				if(RxDirtyPDABit && RxUpdateFlag) { // if RxDirtyBit is Set, PDA is current to Module Config
					D(1, BUG("RXBUFFER GIGEconfig: %x\n", RxBuffer[GIGEconfig_ptr]));
					D(1, BUG( "\n<> going into config_hardware \n"));
					config_hardware(); // If UpdateFlag is set: we have a NEW config to perform
				}
			}//End DS1A packet execution
		}// END VALID ID
		//=********************************************************************
		else if(Rx_PACKET_ID == Packet_GIGEB) { // GIGE Packet - Port B
			//=********************************************************************
			if(good && !ReSyncFlagB) { // We have a VALID (good checksum) Config Packet from the PDA
				if(RxDirtyPDABit && RxUpdateFlag) { // if RxDirtyBit is Set, PDA is current to Module Config
					config_hardware(); // If UpdateFlag is set: we have a NEW config to perform
				}
			}//End DS1A packet execution
		}// END VALID ID
		//=********************************************************************
		else if(Rx_PACKET_ID == Packet_GIGE10A) { // GIGE 10G Packet - Port A
			//=********************************************************************
			if(good && !ReSyncFlag10G) { // We have a VALID (good checksum) Config Packet from the PDA
				if(RxDirtyPDABit && RxUpdateFlag) { // if RxDirtyBit is Set, PDA is current to Module Config
					D(1, BUG( "\n<> going into 10 G config_hardware \n"));
					config_hardware10G(); // If UpdateFlag is set: we have a NEW config to perform
				}
			}//End DS1A packet execution
		}// END VALID ID
		//=********************************************************************
	}
	else{
		;//C37.94 pkt
		//SENDING_STD_PKT
	}

	//=***************************************************************************************************************
	// If in LAN-WAN mode - make sure FPGA routing is configured properly if we toggle mode between TSE and 10G on A
	//=***************************************************************************************************************
	if(good && Rx_PACKET_ID == Packet_GIGEA) { // GIGE Packet - Port A
		if(ConfigStat10G[CORE_SIGROUTE_MODES_ptr] == 2) { //0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK), 1=TSE_LANWAN_MODE, (2) ^1:0=10G_NORMAL_MODE,1=10G_LANWAN_MODE (10G+B=LANWAN,A=GEN/MON)
			ConfigStat[CORE_SIGROUTE_MODES_ptr] = 1; //FORCE to 1, TSE_LANWAN_MODE (10G NORMAL)
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] = 1; // make sure everyone knows
			ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = 1;
			SET_TSE_LANWAN_MODE;// TSE A&B in LAN-WAN mode, sets 10G to normal and TSE-B SWAP OFF!
			SET_SWAP_B_TOGGLE = 0;
			SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE); //swap OFF
		}
	}
	else if(good && Rx_PACKET_ID == Packet_GIGE10A) { // GIGE 10G Packet - Port A
		if(ConfigStat[CORE_SIGROUTE_MODES_ptr] == 1) { //0=TSE_NORMAL_MODE (A=GEN/MON,B=LPBK), 1=TSE_LANWAN_MODE, (2) ^1:0=10G_NORMAL_MODE,1=10G_LANWAN_MODE (10G+B=LANWAN,A=GEN/MON)
			ConfigStat[CORE_SIGROUTE_MODES_ptr] = 2; //FORCE to 2, SET_10G_LANWAN_MODE (Port-A NORMAL)
			ConfigStatB[CORE_SIGROUTE_MODES_ptr] = 2; // make sure everyone knows
			ConfigStat10G[CORE_SIGROUTE_MODES_ptr] = 2;
			SET_10G_LANWAN_MODE;// 10G & B in LAN-WAN mode, sets TSE-A to normal
			SET_SWAP_B_TOGGLE = 0;
			SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);
		}
	}
	//=********************************************************************


	//=********************************************************************
	//=***** GATE the UART intr to Rx a new CONFIG packet *******************
	//=********************************************************************
	buff_ptr = 0;
	i = IORD_ALTERA_AVALON_UART_RXDATA(UART_0_BASE); // read RcvData reg to clear any old data
	UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_RRDY_MSK; // Enable the serial port Receive interrupt
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);// write to CONTROL_REGISTER
	RxBuffer_RDY = NO; // flag main routine to wait until next new packet
	//=********************************************************************
}

/*************************************************************************
 * Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
 * All rights reserved.                                                   *
 *************************************************************************/
