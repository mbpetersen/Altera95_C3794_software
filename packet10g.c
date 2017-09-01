/*************************************************************************
 *		10G Ethernet TRAFFIC CONTROLLER									 *
 *					PACKET GEN/MON Functions 							 *
 *				   														 *
 * Copyright (c) 2015 Greenlee Communications, CA USA.    *
 * All rights reserved.                                                   *
 *************************************************************************/
#include "gigle.h"
#include "extvars.h"

#define RANDOM	1
#define FIXED	0

double SUM_DURATION_CYCLES = 0;

void run_packet_test() {
	unsigned char i;
	SETMON_STOP();
	CLRGEN_STOP(); // ensure GEN AND MON are stopped
	D(1, BUG("\n\t	================== GEN/MON STATUS BEFORE BEFORE STARTING GENERATOR ===========================\n"));
	if (GIGE10){
		D(1, BUG("\n\t	================== CLEARING 10g MAC STATUS BEFORE BEFORE STARTING GENERATOR ===========================\n"));
		CLR_10G_MAC_STATS(); // flush MAC stat's (don't care when running to TSE)
		SaveBytesLong(ConfigStat10G,SECONDS3_ptr,0); //Clear the ELAP before starting any test.
	}
	else
	{
		tseMacWriteCommandConfigAndReset(TRIPLE_SPEED_ETHERNET_0_BASE);// A reset TSE MAC for new STAT's prior to starting test!
		SaveBytesLong(ConfigStat,SECONDS3_ptr,0); //Clear the ELAP before starting any test.
	}
	//=================================
	SETMON_START(); // Start 10G Packet MONITOR
	//=================================
	if (END_TO_END_TEST) // then wait 5.1 seconds for before starting Generator
		OSTimeDlyHMSM(0, 0, 5, 100); // so far end has a buffer to start their generator
	//=================================
	SETGEN_START(); // Start 10G Packet Generator
	//=================================
	wait_for_packets();
	//==== Test Done ==================
	D(1, BUG("\n\t	EEEEEEEEEEEEEEEEEEEE End of Test EEEEEEEEEEEEEEEEEEEEEEEEEE\n"));
	DEBUG_MON_ERRMSG();
	clearTotalResults();//clears all the configstat values to be written at the beginning of a new test otherwise it gets accumulated for multiple runs
	if(MS_TEST_ACTIVE){
		for (i = 0; i < num_of_streams; i++)
		{
		//	D(1, BUG("\n\tMS: getting result of stream %d\n",i));
			read_mon_status(i);
		}
	}
	else
	{
	//	D(1, BUG("\n\tSS: getting result of stream %d\n",stream_no));
		read_mon_status(stream_no);
	}
	//	D(1, BUG("\n\t		   ================== Collecting result of stream: %d =========================== \n",i));
	DBG_MON_STATS();
	if (GIGE10){
		D(1, BUG("\n\t	================== Collecting 10g MAC STATUS BEFORE BEFORE STARTING GENERATOR ===========================\n"));
		collect_10G_MAC_test_stats();}
	else
		collect_TSE_MAC_test_stats();
	 total_num_packets = 0; // flush the total number of packets calculated after multiple runs
	 //todo remove the dumpers and you shouldnt be clearing this burst val
	// SaveBytesLong(ConfigStat,TRAFFIC_GEN_NUM_PACKETS03_ptr,0);//flushing the burst test num packets value
	// dump_gen_settings(0);//to dump emix messages // commented this out because the val of TX frames shown would get updated with that particular streams value and ruin the MS results.
	 DUMP_GEN_CONFIGS();
}

void collect_10G_MAC_test_stats() {
	read_10G_MAC_Stats();
	test_complete(); //flag GUI test has completed & results collected!
}

void collect_TSE_MAC_test_stats() {
	read_tse_stat_counters(TRIPLE_SPEED_ETHERNET_0_BASE);
	test_complete(); //flag GUI test has completed & results collected!
}

//========================================================================
void wait_for_packets() {
		unsigned long long receive_ctrl_status = 0,matched_packet_count = 0,timeout_cntr = 0,pkt_cnt=0,last_rx_packets = 0;
				long long flr=0;

		unsigned short i = 0,j = 0;
		unsigned char sec_cnt = 0,SES_cnt = 0,uas_cntin = 0,cur_sec_cnt = 0,prev_sec_cnt = 1;
		gPACKET_TEST_TIMEOUT = NO;
		gCUSTOM_TEST_STOP = NO;

	while ( (matched_packet_count != total_num_packets) && !gPACKET_TEST_TIMEOUT && !gCUSTOM_TEST_STOP ) {
		OSTimeDlyHMSM(0, 0, 0, 20); //poll every 100mS for updated data!

		if (GIGE10 && !(read_ext_10G_phy(0xD002, 1) & 0x0004)) {
			SETGEN_STOP();
			SETMON_STOP();
			gPACKET_TEST_TIMEOUT = YES;
			D(1, BUG("10G RxLOS detected - aborting test\n"));
			continue;
		}

		if (++j % 5 == 0) { //100ms
			sec_cnt++;
			receive_ctrl_status = read_traffic_reg(MON, TRAFFIC_MON_RX_CTRL_STAT);

			//......snapshot #Num Tx'd and Rx'd for elapsed % display in GUI
			//=================== Transmitted Packets ===================================
		if(GIGE10){
		/*	SaveBytesInt(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PKT_TX_CNT1));
			SaveBytesLong(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PKT_TX_CNT));*/

			SaveBytesInt(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL)& 0xFFFF);
			SaveBytesLong(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL)); // total number of packets TX by GEN

			SaveBytesInt(ConfigStat10G, TRAFFIC_MON_NUM_RCVD_PACKET11_ptr,read_traffic_reg(MON, TRAFFIC_MON_MATCH_PACKET_RX_CNT1_GLOBAL)& 0xFFFF);
			SaveBytesLong(ConfigStat10G, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,read_traffic_reg(MON, TRAFFIC_MON_MATCH_PACKET_RX_CNT0_GLOBAL)); // total number of good packets RX by MON


			/*SaveBytesLong(ConfigStat10G,TRAFFIC_NUM_STREAM_PACKET03_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET));//total number of packets to tx per stream
			SaveBytesInt(ConfigStat10G,TRAFFIC_NUM_STREAM_PACKET11_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET1));*/

			SaveBytesInt(ConfigStat10G, TRAFFIC_NUM_STREAM_PACKET11_ptr, read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET1));
			SaveBytesLong(ConfigStat10G, TRAFFIC_NUM_STREAM_PACKET03_ptr, read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET));//total number of packets to tx overall = tot number of pkts expected by the MON

			//=================== Receiving Packets =====================================
			SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_NUM_PKT_LEFT13_ptr, read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT1));
			SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_NUM_PKT_LEFT03_ptr, read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT));
		}else{
			/*SaveBytesInt(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PKT_TX_CNT1));
			SaveBytesLong(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PKT_TX_CNT));*/

			SaveBytesInt(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL) & 0xFFFF);
			SaveBytesLong(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL));

			SaveBytesInt(ConfigStat, TRAFFIC_MON_NUM_RCVD_PACKET11_ptr, read_traffic_reg(MON, TRAFFIC_MON_MATCH_PACKET_RX_CNT1_GLOBAL)& 0xFFFF);
			SaveBytesLong(ConfigStat, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr, read_traffic_reg(MON, TRAFFIC_MON_MATCH_PACKET_RX_CNT0_GLOBAL)); // total number of good packets RX by MON

			/*SaveBytesLong(ConfigStat,TRAFFIC_NUM_STREAM_PACKET03_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET));//total number of packets to tx per stream
			SaveBytesInt(ConfigStat,TRAFFIC_NUM_STREAM_PACKET11_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET1));*/

			SaveBytesInt(ConfigStat, TRAFFIC_NUM_STREAM_PACKET11_ptr, read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET1));
			SaveBytesLong(ConfigStat, TRAFFIC_NUM_STREAM_PACKET03_ptr, read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET));//total number of packets to tx overall = tot number of pkts expected by the MON

			//=================== Receiving Packets =====================================
			SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_NUM_PKT_LEFT13_ptr, read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT1));
			SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_NUM_PKT_LEFT03_ptr, read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT));
		}
			//=================== Receiving Packets =====================================
			// Keep track of matched packets received so far
			matched_packet_count = ((read_traffic_reg(MON, TRAFFIC_MON_MATCH_PACKET_RX_CNT1_GLOBAL)&0xFFFF)<<32) + read_traffic_reg(MON, TRAFFIC_MON_MATCH_PACKET_RX_CNT0_GLOBAL);
			//pkt_cnt = read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL);

			if (++i % 10 == 0)
			//	D(1, BUG("\tW:EMON_PKTS_RCVD[%d] = %ul \t receive_ctrl_status = %08X\n", i, current_packet_count, receive_ctrl_status));
			//	D(1, BUG("\tW:EMON_PKTS_RCVD[%d] = %d \t receive_ctrl_status = %08X\n", i, current_packet_count, receive_ctrl_status));
				D(1, BUG("\tW:EMON_PKTS_RCVD[%d] = %d \t receive_ctrl_status = %08X\n", i, matched_packet_count, receive_ctrl_status));
			//.....THESE ARE FOR CURRENT STREAM ONLY...........<need to bump for all pkt counts (incl error)>.....
			//	...thinking that UI will capture and display consolidated data + current stream and subsequent UI requests for selected stream data

			/*SaveBytesLong(ConfigStat10G, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,matched_packet_count);
			SaveBytesLong(ConfigStat, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,matched_packet_count);
*/

			if (matched_packet_count == last_rx_packets) {
				//D(1, BUG("rx_packets == last_rx_packet =====\n")); //DEBUG
				if (++timeout_cntr > EMON_TIMEOUT) {
					gPACKET_TEST_TIMEOUT = YES;
					SETMON_STOP();
					D(1, BUG("\n\t	================== Waiting for packets: Timed out - aborting Monitor ======================\n"));
				}
			} else {
				timeout_cntr = 0;
				last_rx_packets = matched_packet_count;
			}

		//check every second for UAS (Unavailable second) which is determined by SES (Errored second)
		if (MS_TEST_ACTIVE) {
			if (sec_cnt==10) //1sec
			{
			//	D(1, BUG("\n\tsec_cnt %d\n", sec_cnt));
				sec_cnt =0; // set the cnt to 0
				cur_sec_cnt++; // keeps track of the elapsed seconds.
			//	D(1, BUG("\n\tcur_sec_cnt %d\n",cur_sec_cnt));
				//check the frame loss ratio - If there is one then it is considered to be a ses
				pkt_cnt = (65535* read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL))+read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL);
				//flr = (total_tx_pkts - current_packet_count)/total_tx_pkts ;
				flr = (long long)( (long long)(pkt_cnt - matched_packet_count )/(long long)pkt_cnt) ;
			/*	D(1, BUG("\n\ttotal_tx_pkts %d\n",pkt_cnt));
				D(1, BUG("\n\t matched_packet_count %d\n", matched_packet_count));
				D(1, BUG("\n\tFLR %d\n",flr));*/
				if(flr>0.5) {
					SES_cnt++;//increment SES count
			//		D(1, BUG("\n\tFLR>0.5 %d\n",flr));
			//	 *** UAS due to SES HANDLER ******************************
				 if(prev_sec_cnt == cur_sec_cnt) { //check if the ses happened the very next second         // check if it was a SES //check if its the very next second.
					 if(SES_cnt==10)//If there are 10 consecutive SES for the first set of 10s then increment the UAS
					  {
						uas_cntin+=10;
					  }	 else{
						++uas_cntin;                      // Inc uas counter
						prev_sec_cnt++;					//increment the consecutive sec SES cntr.
					  }
				 }
				 else { // SES occured but not in the very next sec. //then don't increment UAS counter but only the SES counter.
					prev_sec_cnt = cur_sec_cnt+1;
				 }
				}
				else {  //There was no occurence of SES
				 if((cur_sec_cnt - prev_sec_cnt) == 10) // If there was no SES for 10 seconds then subtract 10 from UAS
					 {
					   if(uas_cntin>9)
						 uas_cntin -=10;
					   else
						   uas_cntin = 0;
					 }
				}
			}
		}
	  }

	}//End While
	SETMON_STOP();
		receive_ctrl_status = read_traffic_reg(MON, TRAFFIC_MON_RX_CTRL_STAT);
		//D(1, BUG("A:receive_ctrl_status = %04X\n",receive_ctrl_status)); // DEBUG
		if (gPACKET_TEST_TIMEOUT) {
			D(1, BUG("A:gTEST_TIMED_OUT_10G\n"));
		}
		if (gCUSTOM_TEST_STOP) {
			D(1, BUG("A:gCUSTOM_TEST_STOP\n"));
		}

		// Wait to accumulate results - do it here to prevent GUI-link from taking status before it's updated!
		// ....so the I'm DONE flag only notifies the GUI AFTER the results have been accumulated!
		if (!gPACKET_TEST_TIMEOUT) { //if in end-to-end, if rcvr timed out -- Tx WILL be finished!
			OSTimeDlyHMSM(0, 0, 0, 100); // When Done - pause to give rcvr a chance to catch up on stats
			if (END_TO_END_TEST) { // if end-to-end mode - check and/or wait patiently for our Gen to finish!
	//			receive_ctrl_status = read_traffic_reg(GEN, TRAFFIC_GEN_PKT_TX_CNT)- read_traffic_reg(GEN, TRAFFIC_GEN_NUM_PACKET);
				//pkt_cnt = GET_GEN_PKT_TX_CNT();

				pkt_cnt = (65535* read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL))+read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL);
				receive_ctrl_status =  pkt_cnt - total_num_packets;

				while (receive_ctrl_status) { // sent less than we're supposed to...so still running
					D(1, BUG("Oop's...we're still generating, ...waiting!\n"));
				//	receive_ctrl_status = read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT) - read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET);

					//pkt_cnt = GET_GEN_PKT_TX_CNT();
					pkt_cnt = (65535* read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL))+read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL);
					receive_ctrl_status =  pkt_cnt - total_num_packets;

					OSTimeDlyHMSM(0, 0, 0, 20); //poll every 20mS for updated data!
				}
			}
		}
		OSTimeDlyHMSM(0, 0, 0, 20); //wait 20mS for data to be fully updated!

		if(GIGE10){
			SaveBytesInt(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL) & 0xFFFF);
			SaveBytesLong(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL));;
			SaveBytesLong(ConfigStat10G, SDT_TOTAL_TIME3_ptr, uas_cntin);//UAS count for Availability
		}else{
			SaveBytesInt(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT1_GLOBAL) & 0xFFFF);
			SaveBytesLong(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN, TRAFFIC_GEN_PACKET_TX_CNT0_GLOBAL));
			SaveBytesLong(ConfigStat, SDT_TOTAL_TIME3_ptr, uas_cntin);//UAS count for Availability
		}

	}//wait_for_packets

void test_complete() {
	//========================================================================
	if (gPACKET_TEST_TIMEOUT | gCUSTOM_TEST_STOP) {
		// Generator & Monitor forcibly stopped by GUI
		//CLRGEN_STOP();	// ensure GEN is stopped
		SETGEN_STOP(); // ensure GEN is stopped
		OSTimeDlyHMSM(0, 0, 0, 2); // If manual Stop - give mon a tiny bit longer to rcv last frame
		CLRGEN_STOP(); // clear GEN stop flag
		SETMON_STOP(); // stopped in waiting for Timeout but not for MANUAL_STOP!
		D(1, BUG("\n\t	================ END OF TEST - GEN and MON STOPPED ====================\n"));
		gCUSTOM_TEST_STOP = NO;
		ConfigStat10G[STARTSTOP_ptr] &= ~0x10; // ack to GUI we see the manual stop!
		ConfigStat10GB[STARTSTOP_ptr] &= ~0x10;
		ConfigStat[STARTSTOP_ptr] &= ~0x10;
		ConfigStatB[STARTSTOP_ptr] &= ~0x10;
		ConfigStat10G[STARTSTOP_ptr] |= 0x40; // flag GUI QuickTest is done
		ConfigStat10GB[STARTSTOP_ptr] |= 0x40;
		ConfigStat[STARTSTOP_ptr] |= 0x40;
		ConfigStatB[STARTSTOP_ptr] |= 0x40;
		if (!gPACKET_TEST_TIMEOUT) {
			D(1, BUG("============== Packet Test Manually Stopped:  Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));
		} else {
			D(1, BUG("============== Packet Test Stopped (Timeout):  Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));
		}
	} else {
	//	D(1, BUG("ConfigStat[STARTSTOP_ptr] before orring with 40 in test complete  Rx:%x ",ConfigStatG[STARTSTOP_ptr]));
		ConfigStat10G[STARTSTOP_ptr] |= 0x40; // flag GUI QuickTest is done
		ConfigStat10GB[STARTSTOP_ptr] |= 0x40; // flag GUI QuickTest is done
		ConfigStat[STARTSTOP_ptr] |= 0x40;
		ConfigStatB[STARTSTOP_ptr] |= 0x40;
		D(1, BUG("============(0x40) Packet Test AUTO-stop: Monitor receive done: Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));
		// do both so we're A/B neutral
	}

	ConfigStat10G[STARTSTOP_ptr] &= ~0x20; // flag GUI QuickTest is stopped
	ConfigStat10GB[STARTSTOP_ptr] &= ~0x20;
	//D(1, BUG("ConfigStat[STARTSTOP_ptr] after orring with 40 and before flag gui in test complete  Rx:%x ",ConfigStat10G[STARTSTOP_ptr]));
	ConfigStat[STARTSTOP_ptr] &= ~0x20;
	ConfigStatB[STARTSTOP_ptr] &= ~0x20;
	D(1, BUG("==============(~0x20) Packet Running Flag :  Rx:%x Tx:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr]));

	// After a MS test, set MISC_ptr 0x80 to indicate consolidated results sent
	if (MS_TEST_ACTIVE)
		ConfigStat[MISC_ptr] |= 0x80;

	if (gPACKET_TEST_TIMEOUT)
		gPACKET_TEST_TIMEOUT = NO;
}

// ===== New =================================
void read_mon_status(unsigned int stream) {
	//All mon status
	//unsigned int stream = 0; //force to 0 for debugging
	unsigned long lobytes, hibytes, locycles, hicycles, lodelay_startcycles,
			hidelay_startcycles;
	double sumbytes, cycle_clocks =0;
	//unsigned int good_pkts_rcvd, bad_pkts_rcvd, good_pkts_rcvd1, bad_pkts_rcvd1,stream_pkt_len;
	unsigned long long good_pkts_rcvd=0, bad_pkts_rcvd=0, good_pkts_rcvd1=0, bad_pkts_rcvd1=0,stream_pkt_len=0;
	//-------------------------------------------------------
	//write_traffic_reg(MON, TRAFFIC_MON_STREAM_KEY, stream);
	//D(1, BUG("\n===== STREAM in read mon packet= %u =======\n",stream));
	SET_GEN_MON_TO_STREAM(stream);
	//-------------------------------------------------------
	lobytes = read_traffic_reg(MON, TRAFFIC_MON_BYTE_RX_CNT0);
	hibytes = read_traffic_reg(MON, TRAFFIC_MON_BYTE_RX_CNT1);
	sumbytes = (4294967295.0 * hibytes) + lobytes;
	/*D(1, BUG("  ***** LOBYTES = %lu", lobytes));
	D(1, BUG("  ***** HIBYTES = %lu", hibytes));
	D(1, BUG("  ***** SUMBYTES = %.0f",sumbytes));*/
	if (GIGE10) {
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_BYTE_CNT03_ptr,(unsigned int) lobytes);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_BYTE_CNT13_ptr,(unsigned int) hibytes);
	} else {
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_BYTE_CNT03_ptr,(unsigned int) lobytes);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_BYTE_CNT13_ptr,(unsigned int) hibytes);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_BYTE_CNT03_ptr,(unsigned int) lobytes);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_BYTE_CNT13_ptr,(unsigned int) hibytes);
	}
	//------------------------------------------------------------------------------

	locycles = read_traffic_reg(MON, TRAFFIC_MON_CYCLE_RX_CNT0);
	hicycles = read_traffic_reg(MON, TRAFFIC_MON_CYCLE_RX_CNT1);
	SUM_DURATION_CYCLES = ((4294967295.0 * hicycles) + locycles) / 8.0;

	lodelay_startcycles	= read_traffic_reg(MON, TRAFFIC_MON_CYCLE_START_RX_CNT0);
	hidelay_startcycles	= read_traffic_reg(MON, TRAFFIC_MON_CYCLE_START_RX_CNT1);

	//================ for TSE  ==================================================
	// At TSE speeds add Compensation for missing (almost) 1st packet time count
	//-----------------------------------------------------------------------------
	stream_pkt_len= (IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_CONFIG))& LEN_MASK;
	//if ((traffic_gen_config & RNDLEN_ON) == RNDLEN_ON)
	if (emix==0x01)
		cycle_clocks = (double) DEF_EMIX_RANDOM; //this NEEDs to be dynamic based on setting. but for now RND is hardcoded DEF_GEN_EMIXSEQ0  0x0123456F
	else
		cycle_clocks = (double) stream_pkt_len;

	cycle_clocks = (cycle_clocks + 20) * 8.0; // total bits in frame

	if (!GIGE10) { //TSE
	//	D(1, BUG("\n*** MON total cycles = %.0f ***", SUM_DURATION_CYCLES));
		//Adjust for selected TSE interface speed	  ==bits*bps/10G MON clk time (6.4nS)
		if (PHYA_ETH_SPEED == speed_1G)
			cycle_clocks *= 1.0e-9 / 6.4e-9;
		else if (PHYA_ETH_SPEED == speed_100M)
			cycle_clocks *= 10.0e-9 / 6.4e-9;
		else if (PHYA_ETH_SPEED == speed_10M)
			cycle_clocks *= 100.0e-9 / 6.4e-9;
		SUM_DURATION_CYCLES += cycle_clocks; // add missing clk's
	//	D(1, BUG(" missing_cycles = %.0f   *** adjusted total cycles = %.0f ***",cycle_clocks,SUM_DURATION_CYCLES));
	}


	// what kind of rounding do we have? -should we pass to GUI *8 and let it handle it?
	hicycles = SUM_DURATION_CYCLES / 4294967296.0;
	locycles = SUM_DURATION_CYCLES - (hicycles * 4294967296.0);
/*	D(1, BUG("\n***** LOCYCLES = %lu", locycles));
	D(1, BUG(" ***** HICYCLES = %lu\n",hicycles));*/
	// Pass #Clks Duration to receive all packets to GUI
	if (GIGE10) {
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_CYCLE_RX_CNT03_ptr,(unsigned int) locycles);
		//SaveBytesLong(ConfigStat10GB, EMON_RX_CYCLE_CNT03_ptr, (unsigned int) locycles);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_CYCLE_RX_CNT13_ptr,(unsigned int) hicycles);
		//SaveBytesLong(ConfigStat10GB, EMON_RX_CYCLE_CNT13_ptr, (unsigned int) hicycles);
	} else {
		SaveBytesLong(ConfigStat, TRAFFIC_MON_CYCLE_RX_CNT03_ptr,(unsigned int) locycles);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_CYCLE_RX_CNT03_ptr,(unsigned int) locycles);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_CYCLE_RX_CNT13_ptr,(unsigned int) hicycles);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_CYCLE_RX_CNT13_ptr,(unsigned int) hicycles);
	}

	good_pkts_rcvd1 = read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK1);
	bad_pkts_rcvd1 = read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR1);

	good_pkts_rcvd = (good_pkts_rcvd1*4294967295.0)+(read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK));
	bad_pkts_rcvd = (bad_pkts_rcvd1*4294967295.0)+(read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR));

	if (GIGE10) {
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_OK_CNT13_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK1));
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_OK_CNT03_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK));
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR1));
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR));

	} else {
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_OK_CNT13_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK1));
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_OK_CNT13_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK1));
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_OK_CNT03_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK));
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_OK_CNT03_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK));

		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR1));
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR1));
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR));
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr,read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR));
	}

	//--------------------------------------------------------------------------------------------------------------------------
	double packets_received = good_pkts_rcvd + bad_pkts_rcvd;
	double rcvd_bytes = sumbytes + (4 * packets_received);
	double test_seconds = SUM_DURATION_CYCLES * 6.4e-9; // 1/156.25MHz = 6.4e-9
	//(org)double THRUPUT = 1.25*SUMBYTES/SUM_DURATION_CYCLES;	// 1.25 == 8bit/byte * 156.25e9Hz = 1.25e9     1.25e9 / 1e9 (to get result in Gig)

	double rate_div = 1e9; // display as Gbps
	if (!GIGE10)
		rate_div = 1e6; // if TSE display as Mbps
	double calc_throughput = (rcvd_bytes * 8.0) / test_seconds;
	calc_throughput /= rate_div;
	double cal_total_throughput = ((rcvd_bytes + (20 * packets_received)) * 8)/ test_seconds;
	cal_total_throughput /= rate_div;

	double avg_pkt_size = rcvd_bytes / (good_pkts_rcvd + bad_pkts_rcvd);
//	double measured_fps_rate = packets_received / test_seconds;
//	double max_fps_rate = 10.0e9 / ((avg_pkt_size + 20) * 8);

	double delay_sum = (double) read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM0);
	delay_sum += (double) ((unsigned long long) read_traffic_reg(MON,TRAFFIC_MON_DELAY_SUM1) << 32);
	delay_sum += (double) (read_traffic_reg(MON, TRAFFIC_MON_DELAY_SUM2))* pow(2, 64);
	//double upper8of72 =  (double) read_traffic_reg(MON,TRAFFIC_MON_DELAY_SUM2);
	//upper8of72 *= pow(2,64);

	/* D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_SUM0 	= %.0f\n",read_traffic_reg(MON,TRAFFIC_MON_DELAY_SUM0)*6.4e-9));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_SUM1 	= %.0f\n",read_traffic_reg(MON,TRAFFIC_MON_DELAY_SUM1)*6.4e-9));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_SUM2 	= %.0f\n",read_traffic_reg(MON,TRAFFIC_MON_DELAY_SUM2)*6.4e-9));
	 D(1, BUG("\t 			delay_sum 	= %.0f\n",delay_sum *6.4e-9));*/
	/***
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX)));

	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX)));

	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX)));

	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX)));

	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX)));
	 ***/

	/*double avg_packet_delay = (float) delay_sum / (float) good_pkts_rcvd;
	 hicycles = avg_packet_delay/4294967296.0;
	 locycles = avg_packet_delay - (hicycles*4294967296.0);
	 D(1, BUG("\n ***** avg_pkt_delay=%0f   **** HI_avg_pkt_delay = %lu  **** LO_avg_pkt_delay = %lu", avg_packet_delay, hicycles, locycles));
	 if(GIGE10){
	 SaveBytesLong(ConfigStat10G, EMON_RX_CYCLE_END_CNT03_ptr, (unsigned int) locycles);	//pass LO
	 SaveBytesLong(ConfigStat10G, EMON_RX_CYCLE_END_CNT13_ptr, (unsigned int) hicycles);	//pass HI
	 }
	 else{
	 SaveBytesLong(ConfigStat, EMON_RX_CYCLE_END_CNT03_ptr, (unsigned int) locycles);	//pass LO
	 SaveBytesLong(ConfigStatB, EMON_RX_CYCLE_END_CNT03_ptr, (unsigned int) locycles);	//pass LO
	 SaveBytesLong(ConfigStat, EMON_RX_CYCLE_END_CNT13_ptr, (unsigned int) hicycles);	//pass HI
	 SaveBytesLong(ConfigStatB, EMON_RX_CYCLE_END_CNT13_ptr, (unsigned int) hicycles);	//pass HI
	 }*/

	/*SaveBytesLong(ConfigStat, TRAFFIC_MON_NUM_PACKET_CNT3_ptr, read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET));
	 SaveBytesLong(ConfigStat, TRAFFIC_MON_NUM_PACKET1_ptr, read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET1));*/

	/*D(1, BUG("\n\t\t\t  _______________________________________________________\n"));
	 D(1, BUG("\t\t\t\t    MONITOR Stream = %u\n", stream));
	 D(1, BUG("\t 			-- (MONITOR) NUMBER PKTS EXPECTED 	= %0u\n",(unsigned int) read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET)));
	 D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS to TX      = %0u\n",(unsigned int)read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET)));
	 D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS act TX     = %0u\n",(unsigned int)read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT))); //
	 D(1, BUG("\t 			-- (MONITOR) GOOD PKTS RECEIVED 	= %0u\n", good_pkts_rcvd));
	 D(1, BUG("\t 			-- (MONITOR) BAD PKTS RECEIVED   	= %0u\n", bad_pkts_rcvd));
	 D(1, BUG("\t\t\t  _______________________________________________________\n"));
	 D(1, BUG("\t 			-- (MONITOR) BYTES RECEIVED 		= %.1f\n", sumbytes));
	 D(1, BUG("\t 			-- (MONITOR) TOTAL BYTES RECEIVED 	= %.1f\n", rcvd_bytes));
	 D(1, BUG("\t 			-- (MONITOR) AVG PKT SIZE 		= %.1f\n", avg_pkt_size));
	 D(1, BUG("\t 			-- (GUISYS)  CYCLES USED 		 = %.1f clks \n", SUM_DURATION_CYCLES));
	 D(1, BUG("\t 			-- (GUISYS)  TEST SECONDS 		 = %.10f secs\n", test_seconds));
	 if(GIGE10){	D(1, BUG("\t 			-- (GUISYS)  THROUGHPUT 		 = %.10f Gbps\n", calc_throughput)); }
	 else{		D(1, BUG("\t 			-- (GUISYS)  THROUGHPUT 		 = %.10f Mbps\n", calc_throughput)); }
	 D(1, BUG("\t 			-- (GUISYS)  RX FRAME-RATE 		 = %.10e FPS\n", measured_fps_rate)); // %8.4
	 D(1, BUG("\t 			-- (GUISYS)  RX FRAME-RATE 		 = %6.3f percent\n", (measured_fps_rate/max_fps_rate)*100.0)); // %8.4
	 if(GIGE10){	D(1, BUG("\t 			-- (GUISYS)  TOTAL THROUGHPUT		= %.10f Gbps\n", cal_total_throughput)); }
	 else{		D(1, BUG("\t 			-- (GUISYS)  TOTAL THROUGHPUT		= %.10f Mbps\n", cal_total_throughput)); }
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX)));
	 D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_SUM 	= %0f\n", delay_sum));
	 //  D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY avg 	= %0f\n",avg_packet_delay));

	 * DEBUG
	 D(1, BUG("\t 			-- (MONITOR) NUM_MONINIT   = %.1f\n",(double)read_traffic_reg(MON,TRAFFIC_MON_NUM_MONINIT)));
	 D(1, BUG("\t 			-- (MONITOR) NUM_MONACTIVE = %.1f\n",(double)read_traffic_reg(MON,TRAFFIC_MON_NUM_MONACTIVE)));
	 D(1, BUG("\t 			-- (MONITOR) NUM_MONDONE   = %.1f\n", (double)read_traffic_reg(MON,TRAFFIC_MON_NUM_MONDONE)));
	 D(1, BUG("\t 			-- (MONITOR) NUM_MONNONE   = %.1f\n", (double)read_traffic_reg(MON,TRAFFIC_MON_NUM_MONNONE)));
	 *
	 //old D(1, BUG("\t 			-- (MONITOR) SUM_STARTCYCLES 		= %.1f clks\t%.6e secs\n", SUM_STARTCYCLES, SUM_STARTCYCLES*6.4e-9));  // e-9sec, -6mS, -3uS, 0nS
	 //old D(1, BUG("\t 			-- (MONITOR) SUM_TOENDCYCLES 		= %.1f clks\t%.10f secs\n", SUM_TOENDCYCLES, SUM_TOENDCYCLES*6.4e-9));
	 D(1, BUG("\t 			-- (MONITOR) RX_DEST_MAC_ADDR 		= %04X %04X\n",	 (unsigned int)read_traffic_reg(MON,TRAFFIC_MON_RX_DEST_MAC_ADDR1),(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_RX_DEST_MAC_ADDR0)));
	 //...when dest:src= FFEEDDCCBBAA,C5C4C3C2C1C0 we're capturing
	 //                      DDCCBBAA,C5C4  so we're snapping two bytes late!
	 D(1, BUG("\t 			-- (MONITOR) RX_BROADCAST_CNT 		= %u\n",(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_BROADCAST_RX_CNT)));
*/

	 //todo check if int is enough?! dont think so
//	unsigned int expected_packets = (unsigned int) read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET); //total number of packets expected at the MON -sum of all streams
	unsigned int packets_to_transmit = (unsigned int) read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET);
	unsigned int packets_to_transmit1 = (unsigned int) read_traffic_reg(GEN,TRAFFIC_GEN_NUM_PACKET1);
	unsigned int packets_act_transmit = (unsigned int) read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT);
	unsigned int packets_act_transmit1 = (unsigned int) read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT1);
	unsigned int traffic_mon_delay_min = (unsigned int) read_traffic_reg(MON,TRAFFIC_MON_DELAY_MIN);
	unsigned int traffic_mon_delay_max = (unsigned int) read_traffic_reg(MON,TRAFFIC_MON_DELAY_MAX);
	unsigned int rx_broadcast_cnt = (unsigned int) read_traffic_reg(MON,TRAFFIC_MON_BROADCAST_RX_CNT);
	unsigned int rx_broadcast_cnt1 = (unsigned int) read_traffic_reg(MON,TRAFFIC_MON_BROADCAST_RX_CNT1);
//todo uncomment the one below
	/*D(1, BUG("\n\t\t\t  _______________________________________________________\n"));
	D(1, BUG("\t\t\t\t    MONITOR Stream = %u\n", stream));
	D(1, BUG("\t 			-- (MONITOR) NUMBER PKTS EXPECTED 	= %0u\n",expected_packets));
	D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS to TX      = %0u\n",packets_to_transmit));
	D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS act TX     = %0u\n",packets_act_transmit)); //
	D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS to TX1      = %0u\n",packets_to_transmit1));
		D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS act TX1     = %0u\n",packets_act_transmit1));
	D(1, BUG("\t 			-- (MONITOR) GOOD PKTS RECEIVED 	= %llu\n", good_pkts_rcvd));
	D(1, BUG("\t 			-- (MONITOR) BAD PKTS RECEIVED   	= %llu\n", bad_pkts_rcvd));
	D(1, BUG("\t\t\t  _______________________________________________________\n"));
	D(1, BUG("\t 			-- (MONITOR) BYTES RECEIVED 		= %.1f\n", sumbytes));
	D(1, BUG("\t 			-- (MONITOR) TOTAL BYTES RECEIVED 	= %.1f\n", rcvd_bytes));
	D(1, BUG("\t 			-- (MONITOR) AVG PKT SIZE 		= %.1f\n", avg_pkt_size));
	D(1, BUG("\t 			-- (GUISYS)  CYCLES USED 		 = %.1f clks \n", SUM_DURATION_CYCLES));
	D(1, BUG("\t 			-- (GUISYS)  TEST SECONDS 		 = %.10f secs\n", test_seconds));
	if (GIGE10) {
		D(1, BUG("\t 			-- (GUISYS)  THROUGHPUT 		 = %.10f Gbps\n", calc_throughput));
	} else {
		D(1, BUG("\t 			-- (GUISYS)  THROUGHPUT 		 = %.10f Mbps\n", calc_throughput));
	}
	D(1, BUG("\t 			-- (GUISYS)  RX FRAME-RATE 		 = %.10e FPS\n", measured_fps_rate)); // %8.4
	D(1, BUG("\t 			-- (GUISYS)  RX FRAME-RATE 		 = %0f percent\n", (measured_fps_rate/max_fps_rate)*100.0)); // %8.4
	if (GIGE10) {
		D(1, BUG("\t 			-- (GUISYS)  TOTAL THROUGHPUT		= %.10f Gbps\n", cal_total_throughput));
	} else {
		D(1, BUG("\t 			-- (GUISYS)  TOTAL THROUGHPUT		= %.10f Mbps\n", cal_total_throughput));
	}
	D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %0u\n",traffic_mon_delay_min));
	D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %0u\n",traffic_mon_delay_max));
	D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_SUM 	= %0f\n", delay_sum));
	D(1, BUG("\t 			-- (MONITOR) RX_DEST_MAC_ADDR 		= %04X %04X\n", (unsigned int)read_traffic_reg(MON,TRAFFIC_MON_RX_DEST_MAC_ADDR1),(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_RX_DEST_MAC_ADDR0)));
	D(1, BUG("\t 			-- (MONITOR) RX_BROADCAST_CNT 		= %u\n",rx_broadcast_cnt));
	D(1, BUG("\t 			-- (MONITOR) RX_BROADCAST_CNT1 		= %u\n",rx_broadcast_cnt1));
*/

	if(GIGE10){
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_MAC_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MAC_ERR));
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_MAC_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MAC_ERR1));
			//SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_IPV4HCS_ERR_CNT3_ptr,read_traffic_reg(TRAFFIC_MON_PKT_RX_IPV4HCS_ERR));
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_EOP_ERR13_ptr, read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_EOP_ERR1));
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_EOP_ERR03_ptr, read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_EOP_ERR));

			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR1));
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR));

			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_NOMATCH_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_NOMATCH_ERR1));
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_NOMATCH_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_NOMATCH_ERR));

			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR1));
			SaveBytesLong(ConfigStat10G,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR));
	}else{
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_MAC_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MAC_ERR1));
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_MAC_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MAC_ERR));
		 //SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_IPV4HCS_ERR_CNT3_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_IPV4HCS_ERR));
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_EOP_ERR13_ptr, read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_EOP_ERR1));
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_EOP_ERR03_ptr, read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_EOP_ERR));

		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR1));
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_TIMEOUT_ERR));

		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_NOMATCH_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_NOMATCH_ERR1));
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_NOMATCH_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_NOMATCH_ERR));

		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR13_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR1));
		 SaveBytesLong(ConfigStat,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR03_ptr,read_traffic_reg(MON,TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR));
	}
	// 	if(MS_PKT){
	tot_lobytes += lobytes;
	tot_hibytes += hibytes;
	tot_locycles = locycles; // don't add up the cycles because this is used to calculate the duration and not used anywhere else.
	tot_hicycles = hicycles;
	tot_packets_rcvd += packets_received;
	tot_packets_to_transmit += packets_to_transmit;
	tot_packets_to_transmit1 += packets_to_transmit1;
	tot_packets_act_transmit += packets_act_transmit;
	tot_packets_act_transmit1 += packets_act_transmit1;
	tot_good_pkts_rcvd1 += good_pkts_rcvd1;
	tot_good_pkts_rcvd += read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_OK);
	tot_bad_pkts_rcvd1 += bad_pkts_rcvd1;
	tot_bad_pkts_rcvd += read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_ERR);
	tot_sumbytes += sumbytes;
	tot_rcvd_bytes += rcvd_bytes;
	tot_avg_pkt_size += avg_pkt_size;
	tot_cycles_used += SUM_DURATION_CYCLES;
	tot_traffic_mon_delay_min += traffic_mon_delay_min;
	tot_traffic_mon_delay_max += traffic_mon_delay_max;
	tot_traffic_mon_delay_sum += delay_sum;
//	D(1, BUG("\t 			tot_traffic_mon_delay_sum 	= %.0f\n",tot_traffic_mon_delay_sum *6.4e-9));
	tot_rx_broadcast_cnt += rx_broadcast_cnt;
	tot_rx_broadcast_cnt1 += rx_broadcast_cnt1;
	tot_seconds = test_seconds;
	GetTotalResults();
	//	}
}

void GetTotalResults() {

	if (GIGE10) {
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_BYTE_CNT03_ptr,	(unsigned int) tot_lobytes);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_BYTE_CNT13_ptr,	(unsigned int) tot_hibytes);

		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_CYCLE_RX_CNT03_ptr,(unsigned int) tot_locycles);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_CYCLE_RX_CNT13_ptr,(unsigned int) tot_hicycles);

		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_OK_CNT13_ptr,tot_good_pkts_rcvd1);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_OK_CNT03_ptr,tot_good_pkts_rcvd);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr,tot_bad_pkts_rcvd1);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr,tot_bad_pkts_rcvd);

		//....should be num frames actually rcvd by mon - this is a global value hence the tot bytes rcvd is what is being sent.
		//SaveBytesLong(ConfigStat10G, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,	tot_rcvd_bytes);

		SaveBytesInt(ConfigStat10G, TRAFFIC_NUM_STREAM_PACKET11_ptr,tot_packets_to_transmit1); //total no of pkt to tx from GEN per stream
		SaveBytesLong(ConfigStat10G, TRAFFIC_NUM_STREAM_PACKET03_ptr,tot_packets_to_transmit);

		SaveBytesInt(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,tot_packets_act_transmit1);
		SaveBytesLong(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,tot_packets_act_transmit);

		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_DELAY_MIN03_ptr,tot_traffic_mon_delay_min);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_DELAY_MAX03_ptr,tot_traffic_mon_delay_max);

		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_DELAY_SUM03_ptr,tot_traffic_mon_delay_sum);

		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_BROADCAST_RX03_ptr,tot_rx_broadcast_cnt);
		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_BROADCAST_RX13_ptr,tot_rx_broadcast_cnt1);

		SaveBytesLong(ConfigStat10G, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,tot_packets_rcvd); // takes the value of the tot packets rcvd = good+bad

		/*SaveBytesLong(ConfigStat10G, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,
						tot_rcvd_bytes);*/ // rcvd bytes = goodpkt+badpkts -TRAFFIC_MON_NUM_RCVD_PACKET03_ptr used for expected packets

	} else {

		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_BYTE_CNT13_ptr,(unsigned int) tot_hibytes);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_BYTE_CNT03_ptr,(unsigned int) tot_lobytes);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_BYTE_CNT13_ptr,(unsigned int) tot_hibytes);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_BYTE_CNT03_ptr,(unsigned int) tot_lobytes);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_CYCLE_RX_CNT13_ptr,(unsigned int) tot_hicycles);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_CYCLE_RX_CNT03_ptr,(unsigned int) tot_locycles);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_CYCLE_RX_CNT13_ptr,(unsigned int) tot_hicycles);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_CYCLE_RX_CNT03_ptr,(unsigned int) tot_locycles);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_OK_CNT13_ptr,tot_good_pkts_rcvd1);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_OK_CNT03_ptr,tot_good_pkts_rcvd);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_OK_CNT13_ptr,tot_good_pkts_rcvd1);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_OK_CNT03_ptr,tot_good_pkts_rcvd);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr,	tot_bad_pkts_rcvd1);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr,tot_bad_pkts_rcvd);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_ERR_CNT13_ptr,tot_bad_pkts_rcvd1);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_RX_PKT_ERR_CNT03_ptr,tot_bad_pkts_rcvd);

		//....should be num frames actually rcvd by mon - this is a global value hence the tot bytes rcvd is what is being sent.
		/*SaveBytesLong(ConfigStat, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,tot_rcvd_bytes);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,tot_rcvd_bytes);*/

		SaveBytesLong(ConfigStat, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,tot_packets_rcvd); // takes the value of the tot packets rcvd = good+bad
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_NUM_RCVD_PACKET03_ptr,tot_packets_rcvd);

		SaveBytesInt(ConfigStat, TRAFFIC_NUM_STREAM_PACKET11_ptr,tot_packets_to_transmit1);
		SaveBytesLong(ConfigStat, TRAFFIC_NUM_STREAM_PACKET03_ptr,tot_packets_to_transmit);
		SaveBytesInt(ConfigStatB, TRAFFIC_NUM_STREAM_PACKET11_ptr,tot_packets_to_transmit1);
		SaveBytesLong(ConfigStatB, TRAFFIC_NUM_STREAM_PACKET03_ptr,tot_packets_to_transmit);

		SaveBytesInt(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,tot_packets_act_transmit1);
		SaveBytesLong(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,tot_packets_act_transmit);
		SaveBytesInt(ConfigStatB, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,tot_packets_act_transmit1);
		SaveBytesLong(ConfigStatB, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,tot_packets_act_transmit);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_DELAY_MIN03_ptr,tot_traffic_mon_delay_min);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_DELAY_MIN03_ptr,tot_traffic_mon_delay_min);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_DELAY_MAX03_ptr,tot_traffic_mon_delay_max);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_DELAY_MAX03_ptr,tot_traffic_mon_delay_max);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_DELAY_SUM03_ptr,tot_traffic_mon_delay_sum);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_DELAY_SUM03_ptr,tot_traffic_mon_delay_sum);

		SaveBytesLong(ConfigStat, TRAFFIC_MON_BROADCAST_RX03_ptr,tot_rx_broadcast_cnt);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_BROADCAST_RX03_ptr,tot_rx_broadcast_cnt);
		SaveBytesLong(ConfigStat, TRAFFIC_MON_BROADCAST_RX13_ptr,tot_rx_broadcast_cnt1);
		SaveBytesLong(ConfigStatB, TRAFFIC_MON_BROADCAST_RX13_ptr,tot_rx_broadcast_cnt1);
	}


}


void DBG_MON_STATS(){
D(1, BUG("\n\t\t\t  _______________________________________________________\n"));
		//D(1, BUG("\t\t\t\t    MONITOR Stream = %u\n", stream_no));
		D(1, BUG("\t 			-- (MONITOR) NUMBER PKTS EXPECTED 	= %0u\n",(unsigned int) read_traffic_reg(MON,TRAFFIC_MON_NUM_PACKET)));
		D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS to TX      = %0u\n",tot_packets_to_transmit));
		D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS act TX     = %0u\n",tot_packets_act_transmit)); //
		D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS to TX1      = %0u\n",tot_packets_to_transmit1));
			D(1, BUG("\t 			-- (Generator) TOTAL NUMBER PKTS act TX1     = %0u\n",tot_packets_act_transmit1));
		D(1, BUG("\t 			-- (MONITOR) GOOD PKTS RECEIVED 	= %llu\n", tot_good_pkts_rcvd));
		D(1, BUG("\t 			-- (MONITOR) BAD PKTS RECEIVED   	= %llu\n", tot_bad_pkts_rcvd));
		D(1, BUG("\t\t\t  _______________________________________________________\n"));
		D(1, BUG("\t 			-- (MONITOR) BYTES RECEIVED 		= %.1f\n", tot_sumbytes));
		D(1, BUG("\t 			-- (MONITOR) TOTAL BYTES RECEIVED 	= %.1f\n", tot_rcvd_bytes));
	//D(1, BUG("\t 			-- (MONITOR) AVG PKT SIZE 		= %.1f\n", avg_pkt_size));
		D(1, BUG("\t 			-- (GUISYS)  CYCLES USED 		 = %lu clks \n", tot_cycles_used));
		D(1, BUG("\t 			-- (GUISYS)  TEST SECONDS 		 = %.10f secs\n", tot_seconds));

		D(1, BUG("\t 			-- (GUISYS)  RX FRAME-RATE 		 = %.10e FPS\n", (tot_bad_pkts_rcvd+tot_good_pkts_rcvd)/tot_seconds)); // %8.4
		double avg_pkt_size = (tot_rcvd_bytes/(tot_bad_pkts_rcvd+tot_good_pkts_rcvd));
		long long speed =0;
		if(GIGE10)
		{
			speed = 10.0e9;
		}else{
		if(ConfigStat[GIGEMODE_ptr]==  ModeCopper100M || ConfigStat[GIGEMODE_ptr]==  ModeFiber100M)
			speed = 100.0e6;
		else if(ConfigStat[GIGEMODE_ptr]==  ModeCopper1G || ConfigStat[GIGEMODE_ptr]==  ModeFiber1G)
			speed = 1.0e9;
		else if(ConfigStat[GIGEMODE_ptr]==  ModeCopper10M)
			speed = 10.0e6;
		}

		double maxfps = speed / ((avg_pkt_size + 20) * 8);
		D(1, BUG("\t 			-- (GUISYS)  RX FRAME-RATE 		 = %0f percent\n", ((((tot_bad_pkts_rcvd+tot_good_pkts_rcvd)/tot_seconds)/maxfps)*100.0))); // %8.4

		D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MIN 	= %lu\n",tot_traffic_mon_delay_min));
		D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_MAX 	= %lu\n",tot_traffic_mon_delay_max));
		D(1, BUG("\t 			-- (MONITOR) TRAFFIC_MON_DELAY_SUM 	= %lu\n", tot_traffic_mon_delay_sum));
		D(1, BUG("\t 			-- (MONITOR) RX_DEST_MAC_ADDR 		= %04X %04X\n", (unsigned int)read_traffic_reg(MON,TRAFFIC_MON_RX_DEST_MAC_ADDR1),(unsigned int)read_traffic_reg(MON,TRAFFIC_MON_RX_DEST_MAC_ADDR0)));
		D(1, BUG("\t 			-- (MONITOR) RX_BROADCAST_CNT 		= %u\n",tot_rx_broadcast_cnt));
		D(1, BUG("\t 			-- (MONITOR) RX_BROADCAST_CNT1 		= %u\n",tot_rx_broadcast_cnt1));
}

void clearTotalResults() {
	tot_lobytes = 0;
	tot_hibytes = 0;
	tot_locycles = 0;
	tot_hicycles = 0;
	tot_packets_rcvd = 0;
	tot_packets_to_transmit = 0;
	tot_packets_to_transmit1 = 0;
	//D(1, BUG("tot_packets_to_transmit = %d stream_no = %d",tot_packets_to_transmit,stream));
	tot_packets_act_transmit = 0;
	tot_packets_act_transmit1 = 0;
	tot_good_pkts_rcvd = 0;
	tot_good_pkts_rcvd1 = 0;
	tot_bad_pkts_rcvd = 0;
	tot_bad_pkts_rcvd1 = 0;
	tot_sumbytes = 0;
	tot_rcvd_bytes = 0;
	tot_avg_pkt_size = 0;
	tot_cycles_used = 0;
	tot_traffic_mon_delay_min = 0;
	tot_traffic_mon_delay_max = 0;
	tot_traffic_mon_delay_sum = 0;
	tot_rx_broadcast_cnt = 0;
	tot_rx_broadcast_cnt1 = 0;
}

/*
void dump_gen_settings(unsigned int stream) {
	SET_GEN_MON_TO_STREAM(stream);
	//D(1, BUG("\t header Length = [%d] at %d \n", header_length,TRAFFIC_GEN_CONFIG02_ptr));
				D(1, BUG("\t egen_line_load_rate = [%f]\n",egen_line_load_rate));
				D(1, BUG("\t gen_number_seconds = [%d]\n", gen_number_seconds));
				D(1, BUG("\t traffic_gen_pkt_length = [%d]\n", traffic_gen_pkt_length));

	//unsigned int transmitted_frames = read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT);
				//todo check whats wrong with this - need to x?
	unsigned long long transmitted_frames = read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT1);
	//	transmitted_frames  = (transmitted_frames<<16) +read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT);
	transmitted_frames  = (transmitted_frames*65535) +read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT);
	if (GIGE10) {
		SaveBytesInt(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT1));
		SaveBytesLong(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT));


		//	SaveBytesLong(ConfigStat10G, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,transmitted_frames);
		//SaveBytesLong(ConfigStat10GB, TRAFFIC_GEN_TX_PKT_SENT_CNT3_ptr, transmitted_frames);
	} else {
		//SaveBytesLong(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,transmitted_frames);

		SaveBytesLong(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT));
		SaveBytesLong(ConfigStatB, TRAFFIC_GEN_TX_PKT_SENT_CNT03_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT));
		SaveBytesInt(ConfigStat, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,	read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT1));
		SaveBytesInt(ConfigStatB, TRAFFIC_GEN_TX_PKT_SENT_CNT11_ptr,read_traffic_reg(GEN,TRAFFIC_GEN_PKT_TX_CNT1));
	}

	//double calc_transmit_seconds = (((double) transmitted_frames*((double) egen_10G_pkt_length+20.0)*8.0))/10e9;	// -(12.0*8.0)
	//double calc_transmit_cycles = calc_transmit_seconds*156.25e+6;  // determine how many cycles it takes to Tx (in a perfect world)

	// FOR NEW GEN - and include calc for sub-100%
	double pkt_clocks = (double) traffic_gen_pkt_length + 20.0;
	//if ((traffic_gen_config & RNDLEN_ON) == RNDLEN_ON)
	if (emix==0x01){
		D(1, BUG("\t EMIX EMIX in dump \n"));
		pkt_clocks = (double) (DEF_EMIX_RANDOM + FRAME_OVERHEADA);
	}
	if (!GIGE10) {
		// Adjust for selected TSE interface speed
		if (PHYA_ETH_SPEED == speed_1G)
			pkt_clocks *= 10.0;
		else if (PHYA_ETH_SPEED == speed_100M)
			pkt_clocks *= 100.0;
		else if (PHYA_ETH_SPEED == speed_10M)
			pkt_clocks *= 1000.0;
	}
	pkt_clocks *= 100.0 / ((double) egen_line_load_rate); //90% eg 1708.88888889
	long num_clocks = round(pkt_clocks); //    eg 1709 1/8-clks
	double calc_transmit_cycles = (double) num_clocks / 8.0; //(clks/Frame) 1709/8=213.625
	//end new______ calc's________
	calc_transmit_cycles *= (double) transmitted_frames; //(total clocks)
	double calc_transmit_seconds = calc_transmit_cycles * (1.0 / 156.25e6); //10G GenTime for NumOfPackets

	double maxFPS = ((double) traffic_gen_pkt_length + 20.0) * 8.0;
//	if ((traffic_gen_config & RNDLEN_ON) == RNDLEN_ON)
	if (emix==0x01)
		maxFPS = ((double) DEF_EMIX_RANDOM + FRAME_OVERHEADA) * 8.0;

	if (GIGE10)
		maxFPS = 10.0e9 / maxFPS;
	else {
		// Adjust for selected TSE interface speed
		if (PHYA_ETH_SPEED == speed_1G)
			maxFPS = 1.0e9 / maxFPS;
		else if (PHYA_ETH_SPEED == speed_100M)
			maxFPS = 100.0e6 / maxFPS;
		else if (PHYA_ETH_SPEED == speed_10M)
			maxFPS = 10.0e6 / maxFPS;
	}

	D(1, BUG("\t 			maxFPS		= %f percent\n",maxFPS));
	D(1, BUG("\t 			maxFPS		= %f percent\n",maxFPS));
	D(1, BUG("\t 			calc_transmit_seconds		= %d \n",calc_transmit_seconds));
	D(1, BUG("\t 		calc_transmit_seconds	= %d \n",calc_transmit_seconds));

	double effectiveTxFrameRate = ((double) transmitted_frames)/ calc_transmit_seconds;

	D(1, BUG("\t 			effectiveTxFrameRate		= %d \n",effectiveTxFrameRate));
	D(1, BUG("\t 	effectiveTxFrameRate	= %d \n",effectiveTxFrameRate));

	double calc_transmit_rate = (effectiveTxFrameRate / maxFPS) * 100.0;

	D(1, BUG("\t 			| ACTUAL TX FPS RATE 		= %f percent\n",calc_transmit_rate));
	D(1, BUG("\n\t =================================================================\n"));
	D(1, BUG("\t			| 10G GENERATOR CONFIGURATION AND STATUS\n"));
	D(1, BUG("\t   =================================================================\n"));
	D(1, BUG("\t 			| CURRENT START STATE 		= %04X\n", read_traffic_reg(GEN, TRAFFIC_GEN_START)));
	D(1, BUG("\t 			| CURRENT STOP STATE 		= %04X\n", read_traffic_reg(GEN, TRAFFIC_GEN_STOP)));
	D(1, BUG("\t 			| NUM OF FRAMES_TO_SEND 		= %0u\n", read_traffic_reg(GEN, TRAFFIC_GEN_NUM_PACKET)));
	D(1, BUG("\t 			| NUM OF FRAMES_TRANSMITTED		= %0llu\n", transmitted_frames));
	D(1, BUG("\t 			| CALC TRANSMITTED TIME		= %.10f secs\n", calc_transmit_seconds));
	D(1, BUG("\t 			| CALC TRANSMITTED CYCLES		= %.1f clks\n", calc_transmit_cycles));
	D(1, BUG("\t 			|	(MONITOR) CYCLES USED 	= %.1f \n", SUM_DURATION_CYCLES));
	D(1, BUG("\t 			|	(DELTA TX-RX) CYCLES 	= %f  (diff: %f  percent: %.5f)\n", calc_transmit_cycles-SUM_DURATION_CYCLES, (calc_transmit_cycles-SUM_DURATION_CYCLES), ((calc_transmit_cycles-SUM_DURATION_CYCLES)/calc_transmit_cycles)*100.0));
	D(1, BUG("\t 			| FPS RATE SELECTED			= %f percent\n", egen_line_load_rate));
	D(1, BUG("\t 			| ACTUAL TX FPS RATE 		= %f percent\n",calc_transmit_rate));
	D(1, BUG("\t 			| TRANSMITTED FRAME RATE		= %.10e FPS\n", ((double) transmitted_frames)/calc_transmit_seconds));
	//if ((traffic_gen_config & RNDLEN_ON) == RNDLEN_ON) {
	if (emix==0x01){
		D(1, BUG("\t 			| FRAME_LENGTH_RND_EMIX		= %.2f\n", DEF_EMIX_RANDOM));
	} else {
		D(1, BUG("\t 			| FRAME_LENGTH_CONFIGURED		= %0u\n", traffic_gen_config&0xFFFF));
	}
	D(1, BUG("\t			| TRAFFIC_GEN_PKTPERCENT		= %0u\n",read_traffic_reg(GEN, TRAFFIC_GEN_PKTPERCENT)));

	//D(1, BUG("\t 			| RANDOM_LENGTH_CONFIGURATION		= %s\n", (traffic_gen_config&RNDLEN_ON) ? "yes" : "no"));
	D(1, BUG("\t 			| RANDOM_LENGTH_CONFIGURATION		= %s\n", (emix==0x01) ? "yes" : "no"));

	D(1, BUG("\t 			| RANDOM_PAYLOAD_CONFIGURATION		= %s\n", (traffic_gen_config&0x02000000) ? "yes" : "no"));
	D(1, BUG("\t 			| FRAME HEADER LENGTH		= %0X  %0u\n", traffic_gen_config&0xFF0000, (traffic_gen_config&0xFF0000)/0x10000));
	//  	D(1, BUG("\t 			| TRAFFIC_GEN_CNTCLKCNTR 		= %u\n", read_traffic_reg(GEN, TRAFFIC_GEN_CNTCLKCNTR)));
	//   D(1, BUG("\t 			| TRAFFIC_GEN_CNTPAUSECNT 		= %u\n", read_traffic_reg(GEN, TRAFFIC_GEN_CNTPAUSECNT)));
	//  	D(1, BUG("\t 			| TRAFFIC_GEN_CNTBUILDCNT 		= %u\n", read_traffic_reg(GEN, TRAFFIC_GEN_CNTBUILDCNT)));
}
*/

//______________________________________________________________________________________________________________________________
//___START NEW FUNCTIONS_______________________________________________________________
//______________________________________________________________________________________________________________________________
//#define TRAFFIC_GEN_CONFIG        0x200 //0x80 Config Fields for GENERATOR
// Structure:     bits
// CONFIG_PKLEN	 0:15		16-bit packet length 64-9600: 0x40-0x2580 (or mask in RND len modes (e.g. 0xff - clamps size to 255)
// CONFIG_HDRLEN  16:23		 8-bit header length (header can be 0 to 255 bytes) (note MAC, IPv4, IPv6 overwritten with correct length by GEN)
// CONFIG_RNDLEN  24		 1-bit Random Length packets mode: 1=enable Random mode, 0=fixed length frames
// CONFIG_RNDPAY	25		 1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
// CONFIG_FRCERR	26		 1-bit Force Payload data error (field size bits) DEBUGGING ONLY
// CONFIG_SPARE   27:31		 5-bits Spares - not currently used
/*void SET_TRAFFIC_GEN_CONFIG_BY_STREAM() {
	unsigned int prev = read_traffic_reg(GEN, TRAFFIC_GEN_CONFIG);
	write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG, traffic_gen_config);
	D(1, BUG("\tTRAFFIC_GEN_CONFIG=%08X (prev:%08X)\n",read_traffic_reg(GEN, TRAFFIC_GEN_CONFIG), prev));
}*/
// currently: set line rate to GEN
void SET_TRAFFIC_GEN_GBLCONFIG(unsigned short line_rate) {
	unsigned int value = read_traffic_reg(GEN, TRAFFIC_GEN_GBLCONFIG);
	D(1, BUG("\t TRAFFIC_GEN_GBLCONFIG: was=%08X  ",value));
	value &= ~LINKRATE_MASK;
	value |= line_rate;
	write_traffic_reg(GEN, TRAFFIC_GEN_GBLCONFIG, value);
	D(1, BUG("\t  now=%08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_GBLCONFIG)));
}


//egen_line_load_rate
void set_generator_tse_load(unsigned short packet_length, float load_rate) // pass TSE percentage desire 90=90%, 1 to 100
{
	double full = 0; // 		  full: 29-bits.  shift up 3 bits (*8) to make room for rem at bottom (max=1F FF FF FF = 536,870,911)
	unsigned int rem = 0; //remainder:   3-bits.  0=7
	double pktpercent = 0; //added by p
	double tmpVal = 0;

	if (emix==0x01){
		D(1, BUG("\tSetting FPSLOAD_BY_STREAM  EMIX_mode, Load=%f\n", load_rate));
		D(1, BUG("\tDEF_EMIX_RANDOM in set_generator_tse_load \n"));
		tmpVal = (float) DEF_EMIX_RANDOM + (float) FRAME_OVERHEADA; // use avg frame_size for def EMIX sequence
	} else
		tmpVal = packet_length + FRAME_OVERHEADA; // // TSE: egen_pkt_length (or EMIX - calculated and passed to this function)


	// Adjust for selected TSE interface speed
	if(GIGE10)
		{
		D(1, BUG("\tGIGE10 load"));
			SET_TRAFFIC_GEN_GBLCONFIG(LINKRATE_10G);
		}
	else
		{
			if(load_rate==100.0)
			{
				load_rate = 99.990; //TSE cannot handle 100% load
			}

			if (PHYA_ETH_SPEED == speed_1G) {
			tmpVal *= 10;
			SET_TRAFFIC_GEN_GBLCONFIG(LINKRATE_1G);
		} else if (PHYA_ETH_SPEED == speed_100M) {
			tmpVal *= 100;
			SET_TRAFFIC_GEN_GBLCONFIG(LINKRATE_100M);
		} else if (PHYA_ETH_SPEED == speed_10M) {
			tmpVal *= 1000;
			SET_TRAFFIC_GEN_GBLCONFIG(LINKRATE_10M);
		}
	}

	tmpVal *= (100.0 / load_rate); //calculate total clks in float

	full = tmpVal / 8.0;

	// note round() is defined somewhere - but I don't know where so I made my own! - forget it - found it where it should be;  Math.h
	rem = round(tmpVal - (full * 8.0)); // get x:8 remainder (val:0-7),  round up or down to get closest whole value
	pktpercent = full * 8 + rem;
//	D(1, BUG("\tcalculated (TSE) PKPERCENT: full=%lf, rem=%0u, full*8=%lf, pktpercent=%lf  for selected %7.4fpercent\n",full, rem, full*8, pktpercent, load_rate));
	//pktpercent -= 4; //DEBUG always 4 less!
	//D(1, BUG("\tDEBUG pktpercent-=4, PKPERCENT: pktpercent=%0u\n",pktpercent));
	SETGEN_TRAFFIC_GEN_PKTPERCENT(pktpercent);
}


// PKTPERCENT_REMAINDER	 0:2	     3-bit  Fraction of clock period (value 0-7) or Remainder bytes to send
// PKTPERCENT_FULL			 3:31  29-bit  Full or Wholly-filled clk periods (8-bytes sent ea clk)
//converted unsigned int to float - type of parameter passed
void SETGEN_TRAFFIC_GEN_PKTPERCENT(float value) {

	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_PKTPERCENT, value);
	D(1, BUG("\tTRAFFIC_GEN_PKTPERCENT=%0u (full=%lf,rem=%0lf, egen_line_load_rate=%f)\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_PKTPERCENT),value/8,value-((value/8)*8),egen_line_load_rate));
}

//___new____________________________________________________________
//TRAFFIC_GEN_RNDSEED		0x204	//0x81 Random Seed Config Fields
// 	Structure:			bits
// RNDSEED_PAY		 0:15		16-bit payload segment field size (sequence number is the rest)
//					Note: Upper-nibble must be non-zero to differentiate incrementing data from random data in 1st packet byte.
// RNDSEED_LEN		16:31		16-bit packet length segment field size
//#define TRAFFIC_GEN_RNDSEED_DEF	0xF00D5EED	//this is the GEN verilog default
//____________________________________________________________________
//	There are two random seeds: 1) 69 bit seed for the payload data, and; 2) 23 bit seed for the packet length.
//
//	The 69 bit payload data seed is composed of 5 bits of 0, 16 bits from the register (RNDSEED_PAY), and 48 bits from the sequence number.
//            69bits:   [5bits:0] [16bits:RNDSEED_PAY][48bits:SEQ_NUM]
//	In addition, 8 register bits are xored with sequence number bits 3-0, and 1 register bit is xored with a force error flag.
//	*** The sequence number makes sure the seed is different for each packet.
//	When random payload is selected (CONFIG_RNDPAY), this seed generates the pseudo-random payload data sequence.
//
//	The 23 bit packet length seed is composed of 7 bits of 0 and 16 bits from the register (RNDSEED_LEN).
//		   23bits: [7bits:0][16bits:RNDSEED_LEN]
//	This seed generates the pseudo-random packet length.
//____________________________________________________________________
void SETGEN_TRAFFIC_GENMON_RNDSEED_BY_STREAM(unsigned int value) //written in GEN with def 0xF00D5EED: only need to write if want to change!
{
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_RNDSEED, value);
	D(1, BUG("\tTRAFFIC_GEN_RNDSEED=%0u (value=%0u)\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_RNDSEED),value));

	//===== currently defined ===================================
	//#define TRAFFIC_MON_ADDR_RNDSEED   	       	0x0204	//0x81 random seed fields
	//==========================================================================
	//		#define TRAFFIC_MON_RNDSEED_DEF	      0x5EED	// DBG and upper 15 bits all 0's

	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_ADDR_RNDSEED, value&0x0000FFFF);
	D(1, BUG("\tTRAFFIC_GEN_RNDSEED=%0u (value=%0u)\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_ADDR_RNDSEED), value&0x0000FFFF));
}

//___END NEW FUNCTIONS_______________________________________________________________
void SETGEN_STREAM_NUMPKTS(unsigned long value,
		unsigned short number_packet_msb) {
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_NUM_PACKET1, number_packet_msb);
	//D(1, BUG("\tTRAFFIC_GEN_NUM_PACKETS msb =%d\n",TRAFFIC_GEN_NUM_PACKET1));
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_NUM_PACKET, value);
	//D(1, BUG("\tTRAFFIC_GEN_NUM_PACKETS lsb =%lu\n",TRAFFIC_GEN_NUM_PACKET));
	D(1, BUG("\tTRAFFIC_GEN_NUM_PACKETS=%0u\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_NUM_PACKET)));
}
void SETGEN_START() {
	D(1, BUG("\n\t STARTING GENERATOR \n"));
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_START, 0x0001);
} // 0x00000001
void SETGEN_STOP() {
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_STOP, 0x0001);
} // 0x00000001
void CLRGEN_STOP() {
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_STOP, 0x0000);
}

void SET_GEN_MON_TO_STREAM(unsigned int stream) {
	//D(1, BUG("\n===== SET STREAM: %u =======\n",stream));
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_STREAM_KEY, stream);
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_STREAM_KEY, stream); // point to stream 0 (1st stream)
}
void SETMON_L2_HEADER_MASK_MACFRAME() //unsigned int start_point, unsigned int number_of_bytes  MSK addr 0x600-0x6FC  (32bit addr's: 0x180-0x1BF  0x3F (64 total, 0-63))
// create function to write 0xFF into number_of_byte starting at start_point
// addr order: odd/even (1:0, 3:2, 5:4, ... 63:62   1=0x180,0=0x181,  3=0x182,2=0x183)
{
	CLR_MON_MASK_BY_STREAM(TRAFFIC_MON_MSK1, TRAFFIC_MON_MSK62);
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_MSK0, 0xFFFFFFFF);
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_MSK1, 0xFFFF0000);
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_MSK2, 0x00000000);
}
//==================================================================================================
//MSB3:0 == MAC10G_0 => dest_addr0 = 0x33445566;	// MAC: 11-22-33-44-55-66
//LSB1:0 == MAC10G_1  => dest_addr1 = 0x00001122;
//SETGEN_MAC_DEST_ADDR(gen_destination_addr1, gen_destination_addr0);
//gen_destination_addr0	= MAC10G_0;	// usually this IS the same as the MAC (sourcing the EGEN)
//gen_destination_addr1	= MAC10G_1;	// init to Port-A MAC addr (our EGEN)
//_______ NEW_________________
// MAC10G_1= 11 22   MAC10G_0=33 44 55 66
// gen_destination_addr1 = MAC10G_1 = mac_address1
// gen_destination_addr0 = MAC10G_0 = mac_address0
//DEST_ADDR: 11 22 33 44 55 66
void SETGENMONHDR_10GMAC_DEST_ADDR(unsigned int mac_address1,
		unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = mac_address1; // 1122 => 00001122
	tmpLong *= 0x10000; // 00001122 => 11220000
	tmpLong += (mac_address0 / 0x10000); // 33445566 => 00003344,  00003344+11220000=11223344
	//HDR0 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR0, tmpLong);
//	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR0, tmpLong);//?????

	tmpLong = (mac_address0 & 0x0000FFFF); // 33445566=> 00005566
	tmpLong *= 0x10000; // 00005566=> 55660000
	tmpLong += (source_10g_addr1 & 0x0000FFFF);// 55667788   last 2 of DEST, 1st 2 of SRC
	//HDR1 = tmpLong;
	//todo random setmon stop while setting address?!why was it here?
	//SETMON_STOP();
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1, tmpLong);
//	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1, tmpLong);//?????
	D(1, BUG("\t...set TRAFFIC_GENMON_DEST_ADDRESS TO 10G 1&2: %08X %08lX (10Gsrc1:%08X,%08X)\n", mac_address1, mac_address0, source_10g_addr1,source_10g_addr0));
}
void SETGENMONHDR_10GMAC_SOURCE_ADDR(unsigned int mac_address1,
		unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = (destination_10g_addr0 & 0x0000FFFF);// 33445566=> 00005566
	tmpLong *= 0x10000; // 00005566=> 55660000
	tmpLong += (mac_address1 & 0x0000FFFF); // 55667788   last 2 of DEST, 1st 2 of SRC
	//HDR1 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1, tmpLong);
//	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1, tmpLong);//?????
	tmpLong = mac_address0; // 99aabbcc
	//HDR2 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR2, tmpLong);
//	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR2, tmpLong);//?????
	D(1, BUG("\t...set TRAFFIC_GENMON_SRC_ADDRESS TO 10G 1&2: %08X %08lX (10Gdest1:%08X,%08X)\n", mac_address1, mac_address0, destination_10g_addr1, destination_10g_addr0));
	D(1, BUG("\t\tHDR0 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR0)));
	D(1, BUG("\t\tHDR1 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1)));
	D(1, BUG("\t\tHDR2 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR2)));
}
//-------------------------------------------------------
void SETMONHDR_10GMAC_DEST_ADDR(unsigned int mac_address1,
		unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = mac_address1; // 1122 => 00001122
	tmpLong *= 0x10000; // 00001122 => 11220000
	tmpLong += (mac_address0 / 0x10000); // 33445566 => 00003344,  00003344+11220000=11223344
	//HDR0 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR0, tmpLong);
	tmpLong = (mac_address0 & 0x0000FFFF); // 33445566=> 00005566
	tmpLong *= 0x10000; // 00005566=> 55660000
	tmpLong += (source_10g_addr1 & 0x0000FFFF);// 55667788   last 2 of DEST, 1st 2 of SRC
	//HDR1 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1, tmpLong);
	D(1, BUG("\t...set TRAFFIC_MON_DEST_ADDRESS TO 10G 1&2: %08X %08lX (10Gsrc:%08X)\n", mac_address1, mac_address0, source_10g_addr1));
	D(1, BUG("\t\tHDR0 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR0)));
	D(1, BUG("\t\tHDR1 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1)));
	D(1, BUG("\t\tHDR2 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR2)));
}


void SETMON_TSEMAC_SOURCE_ADDR(unsigned int mac_address1,
		unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = destination_tse_addr1 << 24; // 00-00-66-55-->55-00-00-00
	tmpLong |= (destination_tse_addr1 << 8) & 0x00FF0000; //		-->55-66-00-00
	//src_addr0:	44-33-22-11
	tmpLong |= (mac_address0 << 8) & 0x0000FF00; //		-->55-66-11-00
	tmpLong |= (mac_address0 >> 8) & 0x000000FF; //		-->55-66-11-22
	//HDR1 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1, tmpLong);

	tmpLong = (mac_address0 << 8) & 0xFF000000; // 44-33-22-11-->33-00-00-00
	tmpLong |= (mac_address0 >> 8) & 0x00FF0000; //		-->33-44-00-00
	tmpLong |= (mac_address1 << 8) & 0x0000FF00; //		-->33-44-55-00
	tmpLong |= (mac_address1 >> 8) & 0x000000FF; //		-->33-44-55-66
	//HDR2 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR2, tmpLong);
	/*D(1, BUG("\t...set TRAFFIC_SRC_DEST_ADDRESS TO TSE 1&2: %08X %08lX\n", mac_address1, mac_address0));
	D(1, BUG("\t\tHDR0 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR0)));
	D(1, BUG("\t\tHDR1 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1)));
	D(1, BUG("\t\tHDR2 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR2)));*/
}
void SETMON_MAC_DEST_ADDR(unsigned int mac_address1, unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = mac_address0 << 24; // 44-33-22-11-->11-00-00-00
	tmpLong |= (mac_address0 << 8) & 0x00FF0000; //		-->11-22-00-00
	tmpLong |= (mac_address0 >> 8) & 0x0000FF00; // 		-->11-22-33-00
	tmpLong |= (mac_address0 >> 24) & 0x000000FF; // 		-->11-22-33-44

	// HDR0
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR0, tmpLong);
	tmpLong = mac_address1 << 24; // 00-00-66-55-->55-00-00-00
	tmpLong |= (mac_address1 << 8) & 0x00FF0000; //		-->55-66-00-00
	//src_addr0:	44-33-22-11
	tmpLong |= (source_tse_addr0 << 8) & 0x0000FF00; //		-->55-66-11-00
	tmpLong |= (source_tse_addr0 >> 8) & 0x000000FF; //		-->55-66-11-22

	//HDR1
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1, tmpLong);
	/*D(1, BUG("\t...set TRAFFIC_MON_DEST_ADDRESS TO TSE 0&1: %08X %08lX\n", mac_address1, mac_address0));
	D(1, BUG("\t\tHDR0 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR0)));
	D(1, BUG("\t\tHDR1 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR1)));
	D(1, BUG("\t\tHDR2 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_HDR2)));*/
}

void SETGEN_TSEMAC_DEST_ADDR(unsigned int mac_address1,
		unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = mac_address0 << 24; // 44-33-22-11-->11-00-00-00
	tmpLong |= (mac_address0 << 8) & 0x00FF0000; //		-->11-22-00-00
	tmpLong |= (mac_address0 >> 8) & 0x0000FF00; // 		-->11-22-33-00
	tmpLong |= (mac_address0 >> 24) & 0x000000FF; // 		-->11-22-33-44

	// HDR0
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR0, tmpLong);
	tmpLong = mac_address1 << 24; // 00-00-66-55-->55-00-00-00
	tmpLong |= (mac_address1 << 8) & 0x00FF0000; //		-->55-66-00-00
	//src_addr0:	44-33-22-11
	tmpLong |= (source_tse_addr0 << 8) & 0x0000FF00; //		-->55-66-11-00
	tmpLong |= (source_tse_addr0 >> 8) & 0x000000FF; //		-->55-66-11-22

	//HDR1
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1, tmpLong);
	/*D(1, BUG("\t...set TRAFFIC_GEN_DEST_ADDRESS TO TSE 0&1: %08X %08lX\n", mac_address1, mac_address0));
	D(1, BUG("\t\tHDR0 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR0)));
	D(1, BUG("\t\tHDR1 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1)));
	D(1, BUG("\t\tHDR2 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR2)));*/
}
// source_addr:		11-22-33-44-55-66
// tse  dest/src_addr0:	44-33-22-11
// tse  dest/src_addr1:  00-00-66-55
void SETGEN_TSEMAC_SOURCE_ADDR(unsigned int mac_address1,
		unsigned long mac_address0) {
	unsigned long tmpLong = 0;
	tmpLong = destination_tse_addr1 << 24; // 00-00-66-55-->55-00-00-00
	tmpLong |= (destination_tse_addr1 << 8) & 0x00FF0000; //		-->55-66-00-00
	//src_addr0:	44-33-22-11
	tmpLong |= (mac_address0 << 8) & 0x0000FF00; //		-->55-66-11-00
	tmpLong |= (mac_address0 >> 8) & 0x000000FF; //		-->55-66-11-22
	//HDR1 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1, tmpLong);

	tmpLong = (mac_address0 << 8) & 0xFF000000; // 44-33-22-11-->33-00-00-00
	tmpLong |= (mac_address0 >> 8) & 0x00FF0000; //		-->33-44-00-00
	tmpLong |= (mac_address1 << 8) & 0x0000FF00; //		-->33-44-55-00
	tmpLong |= (mac_address1 >> 8) & 0x000000FF; //		-->33-44-55-66
	//HDR2 = tmpLong;
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR2, tmpLong);
/*	D(1, BUG("\t...set TRAFFIC_SRC_DEST_ADDRESS TO TSE 1&2: %08X %08lX\n", mac_address1, mac_address0));
	D(1, BUG("\t\tHDR0 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR0)));
	D(1, BUG("\t\tHDR1 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR1)));
	D(1, BUG("\t\tHDR2 = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_HDR2)));*/
}

void SETMON_MAC_SOURCE_DEST_ADDR() {
	unsigned long hdr0 = read_traffic_reg(GEN, TRAFFIC_GEN_HDR0);
//	D(1, BUG("\nHDR0: %x\n", hdr0));
	unsigned long hdr1 = read_traffic_reg(GEN, TRAFFIC_GEN_HDR1);
//	D(1, BUG("HDR1: %x\n", hdr1));
	unsigned long hdr2 = read_traffic_reg(GEN, TRAFFIC_GEN_HDR2);
//	D(1, BUG("HDR2: %x\n", hdr2));
	write_traffic_reg(MON, TRAFFIC_MON_HDR0, ((hdr1 & 0xFFFF) * 0x10000) + ((hdr2 & 0xFFFF0000) >> 16));
	write_traffic_reg(MON, TRAFFIC_MON_HDR1, ((hdr2 & 0xFFFF) * 0x10000) + ((hdr0 & 0xFFFF0000) >> 16));
	write_traffic_reg(MON, TRAFFIC_MON_HDR2, ((hdr0 & 0xFFFF) * 0x10000) + ((hdr1 & 0xFFFF0000) >> 16));
}

//*********************************************************************************************** was unsigned long number_packet previously
void SETMON_TOTALNUMPKTS(unsigned long long number_packet,
		unsigned short number_packet_msb) {
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_NUM_PACKET1, number_packet_msb); // upper 8-bits - original
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_NUM_PACKET, number_packet); // Lower 32-bits
	D(1, BUG("\t\tMON TOTAL NUMPKTS = %llu  %08X \tNUMPKTS1 = %08X\n",number_packet, IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_NUM_PACKET),IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_NUM_PACKET1)));
}
void SETMON_STOP() {
	D(1, BUG("....stopping [user requested] Packet Checker\n"));
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_RX_CTRL_STAT, TRAFFIC_MON_RECEIVE_STOP_BIT);
}
void SETMON_START() {
	D(1, BUG("....resetting monitor Packet Counters and starting 10G Packet Monitor\n"));
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_RX_CTRL_STAT, TRAFFIC_MON_START_BIT);
	D(1, BUG("\tTRAFFIC_MON_RX_CTRL_STAT=%04x\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_RX_CTRL_STAT)));
}

void SETMON_CLEAR_CNTS_AND_START() {
	D(1, BUG("....resetting monitor Packet Counters and starting 10G Packet Monitor\n"));
	IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR,TRAFFIC_MON_RX_CTRL_STAT, TRAFFIC_MON_RECEIVE_START_AND_CLEAR_CNTRS_BIT);
	D(1, BUG("\tTRAFFIC_MON_RX_CTRL_STAT=%04x\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, TRAFFIC_MON_RX_CTRL_STAT)));
}

void CLR_MON_MASK_BY_STREAM(unsigned int start_msk_addr,
		unsigned int end_msk_addr) {
	unsigned int mon_msk_addr;
	for (mon_msk_addr = start_msk_addr; mon_msk_addr <= end_msk_addr; mon_msk_addr
			= mon_msk_addr + 4) {
		IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, mon_msk_addr, 0);
	}
}
void DISABLE_MON_MASK() {
	write_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL, 0);
} // disable selected stream

void CLR_GEN_HEADER_BY_STREAM(unsigned int start_hdr_addr,
		unsigned int end_hdr_addr) {
	unsigned int gen_hdr_addr;
	for (gen_hdr_addr = start_hdr_addr; gen_hdr_addr <= end_hdr_addr; gen_hdr_addr
			= gen_hdr_addr + 4) {
		IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, gen_hdr_addr, 0);
	}
}

void CLR_MON_HEADER_BY_STREAM(unsigned int start_hdr_addr, unsigned int end_hdr_addr)
 {

	unsigned int mon_hdr_addr;
	for (mon_hdr_addr = start_hdr_addr; mon_hdr_addr <= end_hdr_addr; mon_hdr_addr
			= mon_hdr_addr + 4) {
		IOWR_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_MON_BASE_ADDR, mon_hdr_addr, 0);
	}
}

void CLR_GEN_MON_HDRS_BY_STREAM() {
	//SET_STREAM(stream);  // Point to selected Stream (stream[0] is 1st stream)
	CLR_GEN_HEADER_BY_STREAM(TRAFFIC_GEN_HDR0, TRAFFIC_GEN_HDR31);
	//CLR_MON_HEADER_BY_STREAM(TRAFFIC_MON_HDR1, TRAFFIC_MON_HDR62);
	CLR_MON_HEADER_BY_STREAM(TRAFFIC_MON_HDR0, TRAFFIC_MON_HDR63);
}

void SET_GENMON_MAC_ADDR_BY_STREAM() // set HDR0, HDR1 & HDR2
{
	//replaced the existing methos - ref old fw code for comparison
	D(1, BUG("\tSetting MAC_ADDRS_BY_STREAM\n"));
	if (GIGE10) {
		SETGENMONHDR_10GMAC_DEST_ADDR(destination_10g_addr1,destination_10g_addr0);
		SETGENMONHDR_10GMAC_SOURCE_ADDR(source_10g_addr1, source_10g_addr0); // source & destination get setup from port-A MAC10G's in init.c
		SETMON_MAC_SOURCE_DEST_ADDR();

//		SETGEN_TSEMAC_DEST_ADDR(destination_10g_addr1,destination_10g_addr0);
//		SETGEN_TSEMAC_SOURCE_ADDR(source_10g_addr1, source_10g_addr0);
//		//Swap Destination and Source MAC address to Mask accordingly
//		SETMON_MAC_DEST_ADDR(source_10g_addr1,source_10g_addr0);
//		SETMON_TSEMAC_SOURCE_ADDR(destination_10g_addr1,destination_10g_addr0);

		/*SETGEN_TSEMAC_DEST_ADDR(BytesToInt(ConfigStat10G,TRAFFIC_HDR2_1_ptr),BytesToLong(ConfigStat10G,TRAFFIC_HDR1_1_ptr));
		SETGEN_TSEMAC_SOURCE_ADDR(BytesToInt(ConfigStat10G,TRAFFIC_HDR1_3_ptr),BytesToLong(ConfigStat10G,TRAFFIC_HDR0_3_ptr));
		//Swap Destination and Source MAC address to Mask accordingly
		SETMON_MAC_DEST_ADDR(BytesToInt(ConfigStat10G,TRAFFIC_HDR1_3_ptr),BytesToLong(ConfigStat10G,TRAFFIC_HDR1_1_ptr));
		SETMON_TSEMAC_SOURCE_ADDR(BytesToInt(ConfigStat10G,TRAFFIC_HDR1_3_ptr),BytesToLong(ConfigStat10G,TRAFFIC_HDR0_3_ptr));
*/
			} else {

			/*	D(1, BUG("Dest MAC address is: %04X%04X\n",destination_tse_addr1, destination_tse_addr0));
				if(MS_PKT){
				D(1, BUG("Dest MAC address is from headers: %04X%04X\n",BytesToLong(ConfigMS,TRAFFIC_HDR1_1_ptr),BytesToInt(ConfigStat,TRAFFIC_HDR2_1_ptr)));
				D(1, BUG("Source MAC address is from headers: %04X%04X\n",BytesToLong(ConfigMS,TRAFFIC_HDR0_3_ptr),BytesToInt(ConfigStat,TRAFFIC_HDR1_3_ptr)));
				}else{D(1, BUG("Dest MAC address is from headers: %04X%04X\n",BytesToLong(ConfigStat,TRAFFIC_HDR1_1_ptr),BytesToInt(ConfigStat,TRAFFIC_HDR2_1_ptr)));
				D(1, BUG("Source MAC address is from headers: %04X%04X\n",BytesToLong(ConfigStat,TRAFFIC_HDR0_3_ptr),BytesToInt(ConfigStat,TRAFFIC_HDR1_3_ptr)));
				}*/
				SETGEN_TSEMAC_DEST_ADDR(destination_tse_addr1,destination_tse_addr0);
				SETGEN_TSEMAC_SOURCE_ADDR(source_tse_addr1, source_tse_addr0);
				//Swap Destination and Source MAC address to Mask accordingly
				SETMON_MAC_SOURCE_DEST_ADDR();
//				SETMON_MAC_DEST_ADDR(source_tse_addr1, source_tse_addr0);
//				SETMON_TSEMAC_SOURCE_ADDR(destination_tse_addr1, destination_tse_addr0);

			/*	SETGEN_TSEMAC_DEST_ADDR(BytesToInt(ConfigStat,TRAFFIC_HDR2_1_ptr),BytesToLong(ConfigStat,TRAFFIC_HDR1_1_ptr));
				SETGEN_TSEMAC_SOURCE_ADDR(BytesToInt(ConfigStat,TRAFFIC_HDR1_3_ptr),BytesToLong(ConfigStat,TRAFFIC_HDR0_3_ptr));
				//Swap Destination and Source MAC address to Mask accordingly
				SETMON_MAC_DEST_ADDR(BytesToInt(ConfigStat,TRAFFIC_HDR1_3_ptr),BytesToLong(ConfigStat,TRAFFIC_HDR1_1_ptr));
				SETMON_TSEMAC_SOURCE_ADDR(BytesToInt(ConfigStat,TRAFFIC_HDR2_1_ptr),BytesToLong(ConfigStat,TRAFFIC_HDR0_3_ptr));*/
			}
}

//Note: this sets num packets for each stream.  Pre-Start-GEN must calculate total PKT_NUM for all active streams!
//CALL on: Frame Size, TestSecs, FPS change or SPEED change
void SET_NUMPKTS_BY_STREAM(unsigned int packet_length, unsigned long seconds,
		float stream_percent) {
	unsigned long long num_packets = 0;
	unsigned long num_packets_lsb = 0;
	unsigned short num_packet_msb = 0;
	if (emix==0x01){
		D(1, BUG("\tSetting NUM_PKTS_BY_STREAM  EMIX_mode, Secs=%lu, Load=%f\n" ,seconds, stream_percent));
		packet_length = DEF_EMIX_RANDOM; //set the packet length to the average packet length calculated
	}
	num_packets = set_num_test_packets_by_secs(packet_length, seconds,
			stream_percent);
	num_packets_lsb = num_packets & 0x00FFFFFFFF; //get lower 32-bits of 40
	num_packet_msb = (num_packets - num_packets_lsb) >> 32; // get upper 8-bits of 40
	num_packet_msb &= 0x00FF;
	SETGEN_STREAM_NUMPKTS(num_packets_lsb, num_packet_msb);
}

//CALL on: Frame Size, FPS change or SPEED change
void SET_FPSLOAD_BY_STREAM(unsigned int packet_length, float stream_percent) {
	//set GEN FPS load (by stream)
//	D(1, BUG("...Setting SET_FPSLOAD_BY_STREAM %f at %d\n",stream_percent,packet_length));
	/*if (GIGE10)
		set_generator_10G_load(packet_length, stream_percent);
	else*/
		set_generator_tse_load(packet_length, stream_percent);
}


void SET_GEN_EMIX_BY_STREAM() {

	if(MS_TEST_ACTIVE){
		DEF_GEN_EMIXSEQ0 = BytesToLong(ConfigMS, TRAFFIC_GEN_EMIXSEQ33_ptr);
		DEF_GEN_EMIXSEQ1 = BytesToLong(ConfigMS, TRAFFIC_GEN_EMIXSEQ23_ptr);
		DEF_GEN_EMIXSEQ2 = BytesToLong(ConfigMS, TRAFFIC_GEN_EMIXSEQ13_ptr);
		DEF_GEN_EMIXSEQ3 = BytesToLong(ConfigMS, TRAFFIC_GEN_EMIXSEQ03_ptr);
		DEF_GEN_EMIXUSER = BytesToLong(ConfigMS, TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr);
		DEF_GEN_EMIXUSER1 = BytesToInt(ConfigMS, TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr);

		}
	else{
		if(GIGE10)
		{
			DEF_GEN_EMIXSEQ0 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_EMIXSEQ33_ptr);
			DEF_GEN_EMIXSEQ1 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_EMIXSEQ23_ptr);
			DEF_GEN_EMIXSEQ2 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_EMIXSEQ13_ptr);
			DEF_GEN_EMIXSEQ3 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_EMIXSEQ03_ptr);
			DEF_GEN_EMIXUSER = BytesToLong(ConfigStat10G, TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr);
			DEF_GEN_EMIXUSER1 = BytesToInt(ConfigStat10G, TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr);

		}else{
		DEF_GEN_EMIXSEQ0 = BytesToLong(ConfigStat, TRAFFIC_GEN_EMIXSEQ33_ptr);
		DEF_GEN_EMIXSEQ1 = BytesToLong(ConfigStat, TRAFFIC_GEN_EMIXSEQ23_ptr);
		DEF_GEN_EMIXSEQ2 = BytesToLong(ConfigStat, TRAFFIC_GEN_EMIXSEQ13_ptr);
		DEF_GEN_EMIXSEQ3 = BytesToLong(ConfigStat, TRAFFIC_GEN_EMIXSEQ03_ptr);
		DEF_GEN_EMIXUSER = BytesToLong(ConfigStat, TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr);
		DEF_GEN_EMIXUSER1 = BytesToInt(ConfigStat, TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr);
		}
	}

	if(mplsTags==0)mplsTags=1;
	SET_EMIX_OFFSET((vlanTags+(mplsTags-1))*4);

	//this helps in generating packets with random length
	write_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ0, DEF_GEN_EMIXSEQ0);
	write_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ1, DEF_GEN_EMIXSEQ1);
	write_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ2, DEF_GEN_EMIXSEQ2);
	write_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ3, DEF_GEN_EMIXSEQ3);
	write_traffic_reg(GEN, TRAFFIC_GEN_EMIXUSERSIZE, DEF_GEN_EMIXUSER);
	write_traffic_reg(GEN, TRAFFIC_GEN_EMIXUSERSIZE1, DEF_GEN_EMIXUSER1);

	D(1, BUG("\tTRAFFIC_GEN_EMIXSEQ0 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ0)));
	D(1, BUG("\tTRAFFIC_GEN_EMIXSEQ1 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ1)));
	D(1, BUG("\tTRAFFIC_GEN_EMIXSEQ2 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ2)));
	D(1, BUG("\tTRAFFIC_GEN_EMIXSEQ3 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_EMIXSEQ3)));
	D(1, BUG("\t TRAFFIC_GEN_EMIXUSERSIZE = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_EMIXUSERSIZE)));
	D(1, BUG("\t TRAFFIC_GEN_EMIXUSERSIZE1 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_EMIXUSERSIZE1)));
}

void SET_EMIX_OFFSET(unsigned char offset)
{
	unsigned int current_offset = read_traffic_reg(GEN, TRAFFIC_GEN_RNDSEED);
	current_offset &= ~TRAFFIC_GEN_EMIX_OFFSET_MASK;
	current_offset |= (offset * 0x10000) & TRAFFIC_GEN_EMIX_OFFSET_MASK;
	D(1, BUG("EMIX ADDER: %X\n", current_offset));
	write_traffic_reg(GEN, TRAFFIC_GEN_RNDSEED, current_offset);
	D(1, BUG("EMIX ADDER: %X\n", read_traffic_reg(GEN, TRAFFIC_GEN_RNDSEED)));
}

void SET_MON_BY_STREAM(unsigned long header_length)
{
	CLR_MON_MASK_BY_STREAM(TRAFFIC_MON_MSK1, TRAFFIC_MON_MSK62);
	write_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL, (header_length / 0x100)	+ 0x0001); // enable selected stream
	D(1, BUG("\tTRAFFIC_MON_STREAM_CNTL = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL)));
}

// parameters are: steam_num, pkt_length, seconds, FPS_rate, header-length
// SET_GEN_MON_HDRS_BY_STREAM(0, 1518, 1, 100, HDRLEN_ETH);   // do our current setup
// new multi-stream test setup.....
// SET_GEN_MON_HDRS_BY_STREAM(0, 1518, 1, 30, HDRLEN_ETH);   // setup Stream 1 at 1518  30% (of num packets) set by UI
// SET_GEN_MON_HDRS_BY_STREAM(1, 256, 1, 50, HDRLEN_ETH);   // do our current setup
//<set> 	header_length = HDRLEN_ETH  or other defined HEADER type.
void SET_GEN_MON_TEST_BY_STREAM(unsigned int stream,
		unsigned int packet_length, unsigned long seconds,
		float stream_percent, unsigned long header_length) {
	unsigned int streamCtrlData = (header_length << 8) | 0x0001; //enable the stream + setting the header length //
	//if not emix do all this otherwise set rndln(on) handled in genconfig1 - why is the no. of packets calculated here?
	if(!MS_TEST_ACTIVE) {
		if(!GIGE10) { //TSE
			if ((burst_packets != 0) & (emix !=0x01))//If it is a burst test and not emix then the ui tells the fw how many packets to tx
			{
				number_packet_lsb = BytesToLong(ConfigStat,TRAFFIC_GEN_NUM_BURST_PACKETS03_ptr);
				D(1, BUG("\tTOTAL GEN PKTS from set_gen_mon BURST TEST = %d\n",number_packet_lsb));
				SETGEN_STREAM_NUMPKTS(number_packet_lsb, 0); // DBUGmsg in function
				SETMON_TOTALNUMPKTS(number_packet_lsb, 0); // set MON num pkts to expect
			}
			/*D(1, BUG("\tlenofs= %x\n",ConfigStat[TRAFFIC_GEN_CONFIG10_ptr]));
		\
			ConfigStat[TRAFFIC_GEN_CONFIG10_ptr] = 12;
			D(1, BUG("\tlenofs= %x\n",ConfigStat[TRAFFIC_GEN_CONFIG10_ptr]));
*/
			traffic_gen_config = BytesToLong(ConfigStat, TRAFFIC_GEN_CONFIG03_ptr);
			traffic_gen_config2 = BytesToLong(ConfigStat, TRAFFIC_GEN_CONFIG13_ptr);
			traffic_gen_config3 = BytesToLong(ConfigStat, TRAFFIC_GEN_CONFIG23_ptr);
			traffic_gen_config4 = BytesToLong(ConfigStat, TRAFFIC_GEN_CONFIG33_ptr);

			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG, traffic_gen_config);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG2, traffic_gen_config2);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG3, traffic_gen_config3);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG4, traffic_gen_config4);

			SaveBytesLong(ConfigStat,TRAFFIC_GEN_CONFIG03_ptr,traffic_gen_config);
			SaveBytesLong(ConfigStat,TRAFFIC_GEN_CONFIG13_ptr,traffic_gen_config2);
			SaveBytesLong(ConfigStat,TRAFFIC_GEN_CONFIG23_ptr,traffic_gen_config3);
			SaveBytesLong(ConfigStat,TRAFFIC_GEN_CONFIG33_ptr,traffic_gen_config4);

			destination_tse_addr0 = BytesToLong(ConfigStat, TRAFFIC_HDR1_1_ptr);
			destination_tse_addr1 = (unsigned int) BytesToInt(ConfigStat, TRAFFIC_HDR2_1_ptr);
			source_tse_addr0 = BytesToLong(ConfigStat, TRAFFIC_HDR0_3_ptr);
			source_tse_addr1 = (unsigned int) BytesToInt(ConfigStat, TRAFFIC_HDR1_3_ptr);

			D(1, BUG("packet10g: destination_tse_addr0: %x\n", destination_tse_addr0));
			D(1, BUG("packet10g: destination_tse_addr1: %x\n", destination_tse_addr1));
			D(1, BUG("packet10g: source_tse_addr0: %x\n", source_tse_addr0));
			D(1, BUG("packet10g: source_tse_addr1: %x\n", source_tse_addr1));
		} else {//10G
			if ((burst_packets != 0) & (emix !=0x01))//If it is a burst test and not emix then the ui tells the fw how many packets to tx
			{
				number_packet_lsb = BytesToLong(ConfigStat10G,TRAFFIC_GEN_NUM_BURST_PACKETS03_ptr);
				D(1, BUG("\tTOTAL GEN PKTS from set_gen_mon BURST TEST = %d\n",number_packet_lsb));
				SETGEN_STREAM_NUMPKTS(number_packet_lsb, 0); // DBUGmsg in function
				SETMON_TOTALNUMPKTS(number_packet_lsb, 0); // set MON num pkts to expect
			}

			/*D(1, BUG("\tlenofs= %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG10_ptr]));
			ConfigStat10G[TRAFFIC_GEN_CONFIG10_ptr] = 12;
			D(1, BUG("\tlenofs= %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG10_ptr]));*/

			traffic_gen_config = BytesToLong(ConfigStat10G, TRAFFIC_GEN_CONFIG03_ptr);
			traffic_gen_config2 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_CONFIG13_ptr);
			traffic_gen_config3 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_CONFIG23_ptr);
			traffic_gen_config4 = BytesToLong(ConfigStat10G, TRAFFIC_GEN_CONFIG33_ptr);

			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG, traffic_gen_config);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG2, traffic_gen_config2);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG3, traffic_gen_config3);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG4, traffic_gen_config4);

		/*	D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG00_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG00_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG01_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG01_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG02_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG02_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG03_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG03_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG10_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG10_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG11_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG11_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG12_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG12_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG13_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG13_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG20_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG20_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG21_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG21_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG22_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG22_ptr]));
			D(1, BUG("\tConfigStat10G[TRAFFIC_GEN_CONFIG23_ptr] %x\n",ConfigStat10G[TRAFFIC_GEN_CONFIG23_ptr]));*/

			SaveBytesLong(ConfigStat10G,TRAFFIC_GEN_CONFIG03_ptr,traffic_gen_config);
			SaveBytesLong(ConfigStat10G,TRAFFIC_GEN_CONFIG13_ptr,traffic_gen_config2);
			SaveBytesLong(ConfigStat10G,TRAFFIC_GEN_CONFIG23_ptr,traffic_gen_config3);
			SaveBytesLong(ConfigStat10G,TRAFFIC_GEN_CONFIG33_ptr,traffic_gen_config4);

			//todo for 10G change these values there are special 10G addresses
			destination_10g_addr0 = BytesToLong(ConfigStat10G,TRAFFIC_HDR1_1_ptr);
			destination_10g_addr1 = (unsigned int) BytesToInt(ConfigStat10G,TRAFFIC_HDR2_1_ptr);
			source_10g_addr0 = BytesToLong(ConfigStat10G, TRAFFIC_HDR0_3_ptr);
			source_10g_addr1 = (unsigned int) BytesToInt(ConfigStat10G, TRAFFIC_HDR1_3_ptr);

			D(1, BUG("packet10g: destination_10g_addr0: %x\n", destination_10g_addr0));
			D(1, BUG("packet10g: destination_10g_addr1: %x\n", destination_10g_addr1));
			D(1, BUG("packet10g: source_10g_addr0: %x\n", source_10g_addr0));
			D(1, BUG("packet10g: source_10g_addr1: %x\n", source_10g_addr1));
		}
	}
	else {	// MS test
			traffic_gen_config = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG03_ptr);
			traffic_gen_config2 = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG13_ptr);
			traffic_gen_config3 = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG23_ptr);
			traffic_gen_config4 = BytesToLong(ConfigMS, TRAFFIC_GEN_CONFIG33_ptr);

			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG, traffic_gen_config);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG2, traffic_gen_config2);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG3, traffic_gen_config3);
			write_traffic_reg(GEN, TRAFFIC_GEN_CONFIG4, traffic_gen_config4);

			SaveBytesLong(ConfigMS, TRAFFIC_GEN_CONFIG03_ptr,traffic_gen_config);
			SaveBytesLong(ConfigMS, TRAFFIC_GEN_CONFIG13_ptr,traffic_gen_config2);
			SaveBytesLong(ConfigMS, TRAFFIC_GEN_CONFIG23_ptr,traffic_gen_config3);
			SaveBytesLong(ConfigMS, TRAFFIC_GEN_CONFIG33_ptr,traffic_gen_config4);

			/*D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG00_ptr] %x\n",traffic_gen_config));
			D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG01_ptr] %x\n",traffic_gen_config2));
			D(1, BUG("\tConfigMS[TRAFFIC_GEN_CONFIG02_ptr] %x\n",traffic_gen_config3));*/

			destination_tse_addr0 = BytesToLong(ConfigMS,TRAFFIC_HDR1_1_ptr);
			destination_tse_addr1 = (unsigned int) BytesToInt(ConfigMS,TRAFFIC_HDR2_1_ptr);
			source_tse_addr0 = BytesToLong(ConfigMS, TRAFFIC_HDR0_3_ptr);
			source_tse_addr1 = (unsigned int) BytesToInt(ConfigMS, TRAFFIC_HDR1_3_ptr);

			D(1, BUG("packet10g: destination_tse_addr0: %x\n", destination_tse_addr0));
			D(1, BUG("packet10g: destination_tse_addr1: %x\n", destination_tse_addr1));
			D(1, BUG("packet10g: source_tse_addr0: %x\n", source_tse_addr0));
			D(1, BUG("packet10g: source_tse_addr1: %x\n", source_tse_addr1));
	}
//	if(stream==0){ConfigStat[TRAFFIC_HDR5_2_ptr] = 0;}else{ConfigStat[TRAFFIC_HDR5_2_ptr] = 1;}
	Write_Gen_Traffic(header_length-12);
	Write_Mon_Mask(header_length);
	DUMP_GEN_HDRS_BY_STREAM();
	if(rndPayload==0x02){SETGEN_TRAFFIC_GENMON_RNDSEED_BY_STREAM(TRAFFIC_GEN_RNDSEED_DEF);}
	//------ set MON --------------------------------------------------
	write_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL, streamCtrlData); // enable selected stream
	//D(1, BUG("\tTRAFFIC_MON_STREAM_CNTL = %0X\n",read_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL)));
	DUMP_MON_HDRS_BY_STREAM();
	DUMP_MON_MASK_BY_STREAM();
	DUMP_GEN_CONFIGS();
}



	unsigned long long set_num_test_packets_by_secs(unsigned int packet_length,
			unsigned long seconds, float stream_percent) {
		// perform calculations to determine Frames/Sec Max
		double total_eth_frame_bits;
		if(emix == 0x01)
			total_eth_frame_bits= DEF_EMIX_RANDOM;
		else
			total_eth_frame_bits= packet_length;

		D(1, BUG("total_eth_frame_bits: %lf\n", total_eth_frame_bits));

		total_eth_frame_bits += 20; // add Frame overhead; 8 + 12
		total_eth_frame_bits *= 8.0; // total frame length including overhead in bits

		double speed = 10.0e9; // tmp 10G only!
		if (!GIGE10) {
			if (PHYA_ETH_SPEED == speed_1G)
				speed = 1000.0e6;
			else if (PHYA_ETH_SPEED == speed_100M)
				speed = 100.0e6;
			else if (PHYA_ETH_SPEED == speed_10M)
				speed = 10.0e6;
		}

		double eth_frames_per_sec = (speed / total_eth_frame_bits);
		eth_frames_per_sec *= (stream_percent / 100.0); // scale frame rate by selected FPS

		D(1, BUG("eth_frames_per_Sec: %lf\n", eth_frames_per_sec));

		//Vars.FrameRateCalculatedMax = (speed/total_eth_frame_bits);//used by QuickTest results
		double NumFrames = (double) (eth_frames_per_sec * (double) seconds);
		if (NumFrames > 1.0995116e12) { // clip to our max num of packets (40-bits) 0xFFFFFFFFFF
			NumFrames = 1.099511e12;
			seconds = (long) ((double) NumFrames / eth_frames_per_sec); // Calc actual clipped secs (max is 0xFFFF F = 1048575)
			if (seconds > 0xFFFFF) { // if that num of frames exceeds our Max Secs - then clip to max and recalculate num of packets!
				seconds = 0xFFFFF;
				NumFrames = (double) (eth_frames_per_sec * (double) seconds);
			}
		}
		D(1, BUG("NumFrames: %lf\n", NumFrames));
		return ((unsigned long long) NumFrames); // return tmpSecs in case it has been clipped!
	}

unsigned long long set_num_test_packets_by_secs_precision
(
	unsigned int packet_length, unsigned long seconds, float stream_percent) {
	// perform calculations to determine Frames/Sec Max
	double total_eth_frame_bits = packet_length;

		total_eth_frame_bits += 20; // add Frame overhead; 8 + 12
		total_eth_frame_bits *= 8.0; // total frame length including overhead in bits

		double speed = 10.0e9; // tmp 10G only!

		double eth_frames_per_sec = (speed / total_eth_frame_bits);
		eth_frames_per_sec *= (stream_percent / 100.0); // scale frame rate by selected FPS

		double NumFrames = (double) (eth_frames_per_sec * (double) seconds);
		if (NumFrames > 1.0995116e12) { // clip to our max num of packets (40-bits) 0xFFFFFFFFFF
			NumFrames = 1.099511e12;
			seconds = (long) ((double) NumFrames / eth_frames_per_sec); // Calc actual clipped secs (max is 0xFFFF = 65535) 18h.12m.15s
			if (seconds > 0xFFFF) { // if that num of frames exceeds our Max Secs - then clip to max and recalculate num of packets!
				seconds = 0xFFFF;
				NumFrames = (double) (eth_frames_per_sec * (double) seconds);
			}
		}
		return ((unsigned long long) NumFrames);
	}

	//***
	void DUMP_GEN_CONFIGS() {
		D(1, BUG("\t TRAFFIC_GEN_GBLCONFIG: %08X \n", read_traffic_reg(GEN, TRAFFIC_GEN_GBLCONFIG)));
		D(1, BUG("\t TRAFFIC_GEN_CONFIG:       %08X (EMIX^24,HDRLEN[16:23],PKLEN[0:15]) \n", read_traffic_reg(GEN, TRAFFIC_GEN_CONFIG)));
		D(1, BUG("\t TRAFFIC_GEN_CONFIG2:     %08X  (ETH2_VALUE[30:31],Eth2EMixEN^18, Eth2en^15,HCSOFF[8:14], Jumbo^7, LENOFS[0:6])\n", read_traffic_reg(GEN, TRAFFIC_GEN_CONFIG2)));
		D(1, BUG("\t TRAFFIC_GEN_CONFIG3:      %08X  (IHCS[16:31],ILEN[0:15])\n", read_traffic_reg(GEN, TRAFFIC_GEN_CONFIG3)));
		D(1, BUG("\t TRAFFIC_GEN_CONFIG4:      %08X  (LENOFS[16:22],ILEN[0:15])\n", read_traffic_reg(GEN, TRAFFIC_GEN_CONFIG4)));
	}
	//***/
	void DUMP_MON_HDRS_BY_STREAM() {
		D(1, BUG("\n________________MON CONFIG_________________\n"));
		D(1, BUG("\tMONITOR STREAM KEY  = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_STREAM_KEY)));
		D(1, BUG("\tMONITOR STREAM CNTL = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL)));
		D(1, BUG("\n________________MON HEADERS________________\n"));
		D(1, BUG("\tHDR0 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR0)));
		D(1, BUG("\tHDR1 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR1)));
		D(1, BUG("\tHDR2 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR2)));
		D(1, BUG("\tHDR3 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR3)));
		D(1, BUG("\tHDR4 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR4)));
	D(1, BUG("\tHDR5 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR5)));
		 D(1, BUG("\tHDR6 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR6)));
		 D(1, BUG("\tHDR7 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR7)));
		 D(1, BUG("\tHDR8 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR8)));
		 D(1, BUG("\tHDR9 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR9)));
		 D(1, BUG("\tHDR10= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR10)));
		 D(1, BUG("\tHDR11= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR11)));
		 D(1, BUG("\tHDR12= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR12)));
		 D(1, BUG("\tHDR13= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR13)));
		 D(1, BUG("\tHDR14= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR14)));
		 D(1, BUG("\tHDR15= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_HDR15)));
	}
	void DUMP_MON_MASK_BY_STREAM() {
		D(1, BUG("\n________________MON MASK REGS________________\n"));
		D(1, BUG("\tMSK0 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK0)));
		D(1, BUG("\tMSK1 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK1)));
		D(1, BUG("\tMSK2 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK2)));
		D(1, BUG("\tMSK3 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK3)));
		D(1, BUG("\tMSK4 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK4)));
	D(1, BUG("\tMSK5 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK5)));
		 D(1, BUG("\tMSK6 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK6)));
		 D(1, BUG("\tMSK7 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK7)));
		 D(1, BUG("\tMSK8 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK8)));
		 D(1, BUG("\tMSK9 = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK9)));
		 D(1, BUG("\tMSK10= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK10)));
		 D(1, BUG("\tMSK11= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK11)));
		 D(1, BUG("\tMSK12= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK12)));
		 D(1, BUG("\tMSK13= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK13)));
		 D(1, BUG("\tMSK14= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK14)));
		 D(1, BUG("\tMSK15= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_MSK15)));
	}
	void DUMP_GEN_HDRS_BY_STREAM() {
		D(1, BUG("\n________________GEN HEADERS________________\n"));
		D(1, BUG("\tHDR0 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR0)));
		D(1, BUG("\tHDR1 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR1)));
		D(1, BUG("\tHDR2 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR2)));
		D(1, BUG("\tHDR3 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR3)));
		D(1, BUG("\tHDR4 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR4)));
		D(1, BUG("\tHDR5 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR5)));
			 D(1, BUG("\tHDR6 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR6)));
		 D(1, BUG("\tHDR7 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR7)));
		 D(1, BUG("\tHDR8 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR8)));
		 D(1, BUG("\tHDR9 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR9)));
		 D(1, BUG("\tHDR10= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR10)));
		 D(1, BUG("\tHDR11= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR11)));
		 D(1, BUG("\tHDR12= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR12)));
		 D(1, BUG("\tHDR13= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR13)));
		 D(1, BUG("\tHDR14= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR14)));
		 D(1, BUG("\tHDR15= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR15)));
			/*		 	 D(1, BUG("\tHDR16 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR16)));
		 D(1, BUG("\tHDR17 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR17)));
		 D(1, BUG("\tHDR18 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR18)));
		 D(1, BUG("\tHDR19 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR19)));
		 D(1, BUG("\tHDR20 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR20)));
		 D(1, BUG("\tHDR21 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR21)));
		 D(1, BUG("\tHD22 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR22)));
		 D(1, BUG("\tHDR23 = %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR23)));
		 D(1, BUG("\tHDR24= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR24)));
		 D(1, BUG("\tHDR25= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR25)));
		 D(1, BUG("\tHDR26= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR26)));
		 D(1, BUG("\tHDR27= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR27)));
		 D(1, BUG("\tHDR28= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR28)));
		 D(1, BUG("\tHDR29= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR29)));
		 D(1, BUG("\tHDR30= %08X\n",read_traffic_reg(GEN, TRAFFIC_GEN_HDR30)));*/
	}
	void DEBUG_GEN_MSG(unsigned int stream) {
		SET_GEN_MON_TO_STREAM(stream);
		D(1, BUG("\t	====================== GENERATOR STATUS ======================================================\n"));
		D(1, BUG("\t GEN CURRENT START STATE = %04X\n", IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR ,TRAFFIC_GEN_START)));
		D(1, BUG("\t GEN CURRENT STOP STATE	 = %04X\n", IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR ,TRAFFIC_GEN_STOP)));
		D(1, BUG("\t GENERATOR STREAM KEY	  = %08X\n",IORD_32DIRECT(TRAFFIC_CONTROLLER_GUISYS_0_BASE|TRAFFIC_CONTROLLER_GEN_BASE_ADDR, TRAFFIC_GEN_STREAM_KEY)));
		DUMP_GEN_HDRS_BY_STREAM();
	}

	void DEBUG_MON_STAT_BY_STREAM(unsigned int stream) {
		SET_GEN_MON_TO_STREAM(stream);
		D(1, BUG("\n\t	====================== MONITOR STATUS =====================================================\n"));
		D(1, BUG("\t			| MONITOR STREAM KEY  = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_STREAM_KEY)));
		D(1, BUG("\t			| MONITOR STREAM CNTL = %08X\n",read_traffic_reg(MON, TRAFFIC_MON_STREAM_CNTL)));
		D(1, BUG("\t			| RX_CTRL_STAT	= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_RX_CTRL_STAT)));

		DUMP_MON_HDRS_BY_STREAM();
		DUMP_MON_MASK_BY_STREAM();
	}

	void DEBUG_MON_ERRMSG() {
		D(1, BUG("\t	==================== MONITOR ERROR STATUS =================================================\n"));
		D(1, BUG("\t			| MAC_ERR   	= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_MAC_ERR)));
		D(1, BUG("\t			| IPV4_ERR		= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_IPV4HCS_ERR)));
		D(1, BUG("\t			| EOP_ERR		= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_EOP_ERR)));
		D(1, BUG("\t			| TIME_OUT_ERR	= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_TIMEOUT_ERR)));
		D(1, BUG("\t			| NOMATCH_ERR	= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_NOMATCH_ERR)));
		D(1, BUG("\t			| MULTI_MATCH	= %08X\n",read_traffic_reg(MON, TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR)));
	}

	int TRAFFIC_GEN_HDR[] = { TRAFFIC_GEN_HDR0, TRAFFIC_GEN_HDR1,
			TRAFFIC_GEN_HDR2, TRAFFIC_GEN_HDR3, TRAFFIC_GEN_HDR4,
			TRAFFIC_GEN_HDR5, TRAFFIC_GEN_HDR6, TRAFFIC_GEN_HDR7,
			TRAFFIC_GEN_HDR8, TRAFFIC_GEN_HDR9, TRAFFIC_GEN_HDR10,
			TRAFFIC_GEN_HDR11, TRAFFIC_GEN_HDR12, TRAFFIC_GEN_HDR13,
			TRAFFIC_GEN_HDR14, TRAFFIC_GEN_HDR15, TRAFFIC_GEN_HDR16,
			TRAFFIC_GEN_HDR17, TRAFFIC_GEN_HDR18, TRAFFIC_GEN_HDR19,
			TRAFFIC_GEN_HDR20, TRAFFIC_GEN_HDR21, TRAFFIC_GEN_HDR22,
			TRAFFIC_GEN_HDR23, TRAFFIC_GEN_HDR24, TRAFFIC_GEN_HDR25,
			TRAFFIC_GEN_HDR26, TRAFFIC_GEN_HDR27, TRAFFIC_GEN_HDR28,
			TRAFFIC_GEN_HDR29, TRAFFIC_GEN_HDR30, TRAFFIC_GEN_HDR31 };

	int TRAFFIC_MON_MASK[] = { TRAFFIC_MON_MSK0, TRAFFIC_MON_MSK1,
			TRAFFIC_MON_MSK2, TRAFFIC_MON_MSK3, TRAFFIC_MON_MSK4,
			TRAFFIC_MON_MSK5, TRAFFIC_MON_MSK6, TRAFFIC_MON_MSK7,
			TRAFFIC_MON_MSK8, TRAFFIC_MON_MSK9, TRAFFIC_MON_MSK10,
			TRAFFIC_MON_MSK11, TRAFFIC_MON_MSK12, TRAFFIC_MON_MSK13,
			TRAFFIC_MON_MSK14, TRAFFIC_MON_MSK15, TRAFFIC_MON_MSK16,
			TRAFFIC_MON_MSK17, TRAFFIC_MON_MSK18, TRAFFIC_MON_MSK19,
			TRAFFIC_MON_MSK20, TRAFFIC_MON_MSK21, TRAFFIC_MON_MSK22,
			TRAFFIC_MON_MSK23, TRAFFIC_MON_MSK24, TRAFFIC_MON_MSK25,
			TRAFFIC_MON_MSK26, TRAFFIC_MON_MSK27, TRAFFIC_MON_MSK28,
			TRAFFIC_MON_MSK29, TRAFFIC_MON_MSK30, TRAFFIC_MON_MSK31,
			TRAFFIC_MON_MSK32, TRAFFIC_MON_MSK33, TRAFFIC_MON_MSK34,
			TRAFFIC_MON_MSK35, TRAFFIC_MON_MSK36, TRAFFIC_MON_MSK37,
			TRAFFIC_MON_MSK38, TRAFFIC_MON_MSK39, TRAFFIC_MON_MSK40,
			TRAFFIC_MON_MSK41, TRAFFIC_MON_MSK42, TRAFFIC_MON_MSK43,
			TRAFFIC_MON_MSK44, TRAFFIC_MON_MSK45, TRAFFIC_MON_MSK46,
			TRAFFIC_MON_MSK47, TRAFFIC_MON_MSK48, TRAFFIC_MON_MSK49,
			TRAFFIC_MON_MSK50, TRAFFIC_MON_MSK51, TRAFFIC_MON_MSK52,
			TRAFFIC_MON_MSK53, TRAFFIC_MON_MSK54, TRAFFIC_MON_MSK55,
			TRAFFIC_MON_MSK56, TRAFFIC_MON_MSK57, TRAFFIC_MON_MSK58,
			TRAFFIC_MON_MSK59, TRAFFIC_MON_MSK60, TRAFFIC_MON_MSK61,
			TRAFFIC_MON_MSK62, TRAFFIC_MON_MSK63 };
	int TRAFFIC_MON_HDR[] = { TRAFFIC_MON_HDR0, TRAFFIC_MON_HDR1,
			TRAFFIC_MON_HDR2, TRAFFIC_MON_HDR3, TRAFFIC_MON_HDR4,
			TRAFFIC_MON_HDR5, TRAFFIC_MON_HDR6, TRAFFIC_MON_HDR7,
			TRAFFIC_MON_HDR8, TRAFFIC_MON_HDR9, TRAFFIC_MON_HDR10,
			TRAFFIC_MON_HDR11, TRAFFIC_MON_HDR12, TRAFFIC_MON_HDR13,
			TRAFFIC_MON_HDR14, TRAFFIC_MON_HDR15, TRAFFIC_MON_HDR16,
			TRAFFIC_MON_HDR17, TRAFFIC_MON_HDR18, TRAFFIC_MON_HDR19,
			TRAFFIC_MON_HDR20, TRAFFIC_MON_HDR21, TRAFFIC_MON_HDR22,
			TRAFFIC_MON_HDR23, TRAFFIC_MON_HDR24, TRAFFIC_MON_HDR25,
			TRAFFIC_MON_HDR26, TRAFFIC_MON_HDR27, TRAFFIC_MON_HDR28,
			TRAFFIC_MON_HDR29, TRAFFIC_MON_HDR30, TRAFFIC_MON_HDR31,
			TRAFFIC_MON_HDR32, TRAFFIC_MON_HDR33, TRAFFIC_MON_HDR34,
			TRAFFIC_MON_HDR35, TRAFFIC_MON_HDR36, TRAFFIC_MON_HDR37,
			TRAFFIC_MON_HDR38, TRAFFIC_MON_HDR39, TRAFFIC_MON_HDR40,
			TRAFFIC_MON_HDR41, TRAFFIC_MON_HDR42, TRAFFIC_MON_HDR43,
			TRAFFIC_MON_HDR44, TRAFFIC_MON_HDR45, TRAFFIC_MON_HDR46,
			TRAFFIC_MON_HDR47, TRAFFIC_MON_HDR48, TRAFFIC_MON_HDR49,
			TRAFFIC_MON_HDR50, TRAFFIC_MON_HDR51, TRAFFIC_MON_HDR52,
			TRAFFIC_MON_HDR53, TRAFFIC_MON_HDR54, TRAFFIC_MON_HDR55,
			TRAFFIC_MON_HDR56, TRAFFIC_MON_HDR57, TRAFFIC_MON_HDR58,
			TRAFFIC_MON_HDR59, TRAFFIC_MON_HDR60, TRAFFIC_MON_HDR61,
			TRAFFIC_MON_HDR62, TRAFFIC_MON_HDR63 };

// Write Swapped IP addresses to MON HDRs
void SET_MON_IP_ADDR_BY_STREAM()
{
	unsigned int ip_offset = 14;
	unsigned char numTags = 0;
	int i = 0;

	// VLAN and MPLS tags
//	D(1, BUG("EGEN_PKT_TYPE_PTR_1: %x\n", ConfigStat[EGEN_PKT_TYPE_ptr_1]));
	if (!MS_TEST_ACTIVE) {
		if (GIGE10) {
			numTags = (ConfigStat10G[EGEN_PKT_TYPE_ptr_1] & 0xFFFF) + ((ConfigStat10G[EGEN_PKT_TYPE_ptr_1] & 0xFFFF0000) >> 16);
			ip_offset += (numTags * 4);
			if ((ConfigStat10G[EGEN_PKT_TYPE_ptr] & 0x2))	// 802.3 LLC
				ip_offset += 8;

			// IPv4 Source is 13th byte, Destination is 17th byte
			// IPv6 Source is 9th byte, Destination is 25th byte
			if ((ConfigStat10G[EGEN_PKT_TYPE_ptr] & 0xC) == 0x4) {
//				D(1, BUG("10G IPv4\n"));
				ip_offset += 12;
				// Write source IP into destination IP field
				i = ip_offset / 4;		// find starting hdr value
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+1], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+1], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i]));
			}
			else if ((ConfigStat10G[EGEN_PKT_TYPE_ptr] & 0xC) == 0x8) {
//				D(1, BUG("10G IPv6\n"));
				ip_offset += 8;
				// Write source IP into destination IP field
				i = ip_offset / 4;
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i]));
			}
		}
		else {
			numTags = (ConfigStat[EGEN_PKT_TYPE_ptr_1] & 0xF) + ((ConfigStat[EGEN_PKT_TYPE_ptr_1] & 0xF0) >> 4);
			ip_offset += (numTags * 4);
			if ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0x3) == 0)	// 802.3 LLC
				ip_offset += 8;

			// IPv4 Source is 13th byte, Destination is 17th byte
			// IPv6 Source is 9th byte, Destination is 25th byte
			if ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0xC) == 0x4) {			// IPv4
//				D(1, BUG("TSE IPv4\n"));
				ip_offset += 12;
				// Write source IP into destination IP field
				i = ip_offset / 4;		// find starting hdr value
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+1], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+1], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i]));
			}
			else if ((ConfigStat[EGEN_PKT_TYPE_ptr] & 0xC) == 0x8) {		// IPv6
//				D(1, BUG("TSE IPv6\n"));
				ip_offset += 8;
				// Write source IP into destination IP field
				i = ip_offset / 4;
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
				write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i]));
			}
		}
	}
	else {
		numTags = (ConfigMS[EGEN_PKT_TYPE_ptr_1] & 0xF) + ((ConfigMS[EGEN_PKT_TYPE_ptr_1] & 0xF0) >> 4);
		ip_offset += (numTags * 4);
		if ((ConfigMS[EGEN_PKT_TYPE_ptr] & 0x3) == 0)	// 802.3 LLC
			ip_offset += 8;

		// IPv4 Source is 13th byte, Destination is 17th byte
		// IPv6 Source is 9th byte, Destination is 25th byte
		if ((ConfigMS[EGEN_PKT_TYPE_ptr] & 0xC) == 0x4) {
//			D(1, BUG("MS IPv4\n"));
			ip_offset += 12;
			// Write source IP into destination IP field
			i = ip_offset / 4;		// find starting hdr value
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+1], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+1], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i]));
		}
		else if ((ConfigMS[EGEN_PKT_TYPE_ptr] & 0xC) == 0x8) {
//			D(1, BUG("MS IPv6\n"));
			ip_offset += 8;
			// Write source IP into destination IP field
			i = ip_offset / 4;
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i++]));
			write_traffic_reg(MON, TRAFFIC_MON_HDR[i+4], read_traffic_reg(GEN, TRAFFIC_GEN_HDR[i]));
		}
	}

}


	void Write_Gen_Traffic(unsigned int header_len) {
		unsigned char i;
		unsigned char j = 3;
		unsigned long hdr;

		unsigned short hdrval = TRAFFIC_HDR3_3_ptr; // because first six bytes have the src and mac address

		//Set Src and Dest MAC - First 12 bytes - needs reordering hence written separately
		SET_GENMON_MAC_ADDR_BY_STREAM();

		for (i = 0; i < header_len; i = i + 4) //frame length should be in one of the gen_configs
		{
			if (!MS_TEST_ACTIVE) {
				if(GIGE10)
				{
					hdr = BytesToInt(ConfigStat10G, hdrval) * 0x10000;
					hdr += BytesToInt(ConfigStat10G, hdrval + 2);
				}else{
				hdr = BytesToInt(ConfigStat, hdrval) * 0x10000;
				hdr += BytesToInt(ConfigStat, hdrval + 2);}
			}
			else {
				hdr = BytesToInt(ConfigMS, hdrval) * 0x10000;
				hdr += BytesToInt(ConfigMS, hdrval + 2);
			}

			write_traffic_reg(GEN, TRAFFIC_GEN_HDR[j],hdr);
			write_traffic_reg(MON, TRAFFIC_MON_HDR[j],hdr);
			j++;
			hdrval = hdrval + 4;
		}

		SET_MON_IP_ADDR_BY_STREAM();

	}

	//copying all the mask values into the MON_mask
	void Write_Mon_Mask(unsigned int header_len)
	 {
	 unsigned char i=0;
	 unsigned char j = 0;
	 unsigned short hdrval = TRAFFIC_MSK0_3_ptr;
	 unsigned long hdr;
		for (i = 0; i < header_len; i = i + 4) //frame length should be in one of the gen_configs
		 {
			if (!MS_TEST_ACTIVE) {
				if(GIGE10)
				{
					hdr = BytesToInt(ConfigStat10G, hdrval) * 0x10000;
					hdr += BytesToInt(ConfigStat10G, hdrval + 2);
				}else{
				hdr = BytesToInt(ConfigStat, hdrval) * 0x10000;
				hdr += BytesToInt(ConfigStat, hdrval + 2);}
			}
			else {
				hdr = BytesToInt(ConfigMS, hdrval) * 0x10000;
				hdr += BytesToInt(ConfigMS, hdrval + 2);
			}

		write_traffic_reg(MON, TRAFFIC_MON_MASK[j],hdr);
		j++;
		hdrval = hdrval + 4;
		}
	 }



	void StartTest() {
		if (GIGE_A) {
			SET_TRAFFIC_TO_TSE; // point GEN & MON to TSE port-A
			D(1, BUG(">> TRAFFIC ENGINE switched to TSE-A port!\n"));
			gCUSTOM_TEST_START = YES;
		}else if(GIGE10)
		{
			SET_TRAFFIC_TO_10G; // point GEN & MON to TSE port-A
			D(1, BUG(">> TRAFFIC ENGINE switched to 10GA port!\n"));
		}

		gCUSTOM_TEST_START = YES;

		//can't use Rx..Toggle because it refers to the RxBuffer so the values go awry.
		ConfigStat[STARTSTOP_ptr] = ConfigStat[STARTSTOP_ptr]&0x01;
		ConfigStatB[STARTSTOP_ptr] = ConfigStat[STARTSTOP_ptr]&0x01;// if Test = START
		ConfigStat10G[STARTSTOP_ptr] = ConfigStat[STARTSTOP_ptr]&0x01;


		ConfigStat[STARTSTOP_ptr] |= 0x20 | 0x10; // reflect MOD start and return GUI start
		ConfigStatB[STARTSTOP_ptr] |= 0x20 | 0x10;

		ConfigStat10G[STARTSTOP_ptr] |= 0x20 | 0x10; // reflect MOD start and return GUI start
		ConfigStat10GB[STARTSTOP_ptr] |= 0x20 | 0x10;
		D(1, BUG(">> Start TEST cmd rcvd  Rx:%x Tx [A]:%x Tx [10GA]:%x\n",RxBuffer[STARTSTOP_ptr],ConfigStat[STARTSTOP_ptr],ConfigStat10G[STARTSTOP_ptr]));

	}

	/*************************************************************************
	 * Copyright (c) 2015 Greenlee Communications Vista, USA.    *
	 * All rights reserved.                                                   		*
	 *************************************************************************/
