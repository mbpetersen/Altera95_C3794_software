/*************************************************************************
*		SFP Functions										*
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "c37dot94_def.h"

alt_alarm one_sec_alarm;
alt_u32 handle_one_sec_alarm();

alt_u32 handle_one_sec_alarm(void)
{
	unsigned long tmplong=0;


	//**********************************************************
	//*** ETH Port-A One Second Elapsed Timer   ****************
	//**********************************************************
	if(TestStartGuiBit || ((ConfigStat[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20)){	// If test is started or in lpbk, tick ELAP!
		tmplong = BytesToLong(ConfigStat,SECONDS3_ptr); 	// get and INC ET SECONDS
		tmplong++;
		//D(1, BUG("...sending secs %d\n",tmplong));
		if(tmplong > 0xFFFF0000)	// clamp max value
			tmplong = 0xFFFF0000;
		SaveBytesLong(ConfigStat,SECONDS3_ptr,tmplong);
	}	// end Test is running

	//**********************************************************
	//*** Eth Port-B One Second Elapsed Timer   ****************
	//**********************************************************
	tmplong = BytesToLong(ConfigStatB,SECONDS3_ptr); 	// get and INC ET SECONDS
	if(++tmplong > 0xFFFF0000)	// clamp max value
		tmplong = 0xFFFF0000;
	SaveBytesLong(ConfigStatB,SECONDS3_ptr,tmplong);


	if(TestStartGuiBit10 || ((ConfigStat10G[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20)){	// If test is started, tick ELAP!
		tmplong = BytesToLong(ConfigStat10G,SECONDS3_ptr); 	// get and INC ET SECONDS
		if(++tmplong > 0xFFFF0000)	// clamp max value
			tmplong = 0xFFFF0000;
		SaveBytesLong(ConfigStat10G,SECONDS3_ptr,tmplong);
	}

	if(!GIGE10 && GIGE_A && (ConfigStat[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20)  //only when in LPBK && User is looking at it!
		read_tse_stat_counters(TRIPLE_SPEED_ETHERNET_0_BASE);

	if(GIGE10 && SFP_10G_PRESENT && (ConfigStat10G[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20)   //only when in LPBK && User is looking at it!
		read_10G_MAC_Stats();

	if(!GIGE10 && !GIGE_A)  // if in TSE port-B poll TSE MAC B-stat's only when User is looking at it!
		read_tse_stat_counters(TRIPLE_SPEED_ETHERNET_1_BASE);


	//_____________________________________________________________________________
	if(C3794_READY){  // only process C37 if clock is up & running
		//**********************************************************
		//***  C37.94 One Second Elapsed Timer   ****************
		//**********************************************************
		tmplong = BytesToLong(ConfigStatC37,C37SECONDS3_ptr); 	// get and INC ET SECONDS
		if(++tmplong > 0xFFFF0000)	// clamp max value
			tmplong = 0xFFFF0000;
		SaveBytesLong(ConfigStatC37,C37SECONDS3_ptr,tmplong);

/*
		if(tmplong==6){
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			issue_UI_command(BERTC37_ptr,Poly2047);	//Emulate UI command
			}

		else if(tmplong==4){
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			clear_C3794_counters();
			}
		//
		else if(tmplong==8){	// 24secs in...
			D(1, BUG("\n\t....STOP BERT...")); // TMP DEBUG DISPLAY
			//stop_BERT_test();
			issue_UI_command(BERTC37_ptr,BertOFF);	//Emulate UI command
			}
		//
		else if(tmplong==9){	// 24secs in...
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			//start_BERT_test();
			issue_UI_command(BERTC37_ptr,Poly511);	//Emulate UI command
			}
		else if(tmplong==12){	// 24secs in...
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			//start_BERT_test();
			issue_UI_command(BERTC37_ptr,Alt);	//Emulate UI command
			}
		else if(tmplong==16){	// 24secs in...
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			//start_BERT_test();
			issue_UI_command(BERTC37_ptr,OneIn8);	//Emulate UI command
			}
		else if(tmplong==21){	// 24secs in...
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			//start_BERT_test();
			issue_UI_command(BERTC37_ptr,Poly2047);	//Emulate UI command
			}
//
		else if(tmplong==36){	// 23secs in...
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			clear_internal_loopback();
			D(1, BUG("\n\t....CLR INTL LPBK...")); // TMP DEBUG DISPLAY
			}
		else if(tmplong==38){	// 24secs in...
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			D(1, BUG("\n\t....SET INTL LPBK...")); // TMP DEBUG DISPLAY
			set_internal_loopback();
			//clear_C3794_counters();
			}
//
 *
 */


		//update_alarms_event_status();// moved to main task....
		if(ERRORS_LED == ON)	// if we had an error previously then turn it off
			ERRORS_LED = OFF;


		if(tmplong > 10000 && tmplong%10==0){
//			RxBuffer[MISCC37_ptr] ^= 0x80;	// "set" UI insert Error bit
//			issue_UI_command(MISCC37_ptr, RxBuffer[MISCC37_ptr]);	//Emulate UI command
			D(1, BUG("\nELAP: %lu secs", tmplong)); // TMP DEBUG DISPLAY
			dump_C3794_status(); // TMP DEBUG DISPLAY
			}

		process_alarms_events();

		if(BERT != 0){			// if BERT is on and running poll errors
			process_bert_errors();
			}

		update_history();
	}


	return alt_ticks_per_second();
}


//Update Alarm & Events Flags
/**
#define RCVCLOCK_LOCKED	(C3794_status&CLOCK_STATUS_MASK) == CLOCK_STATUS_MASK
#define TEST_ACTIVE		(C3794_status&TEST_STATUS_MASK) == TEST_STATUS_MASK
#define RDI_ACTIVE		(C3794_status&RDI_STATUS_MASK) != RDI_STATUS_MASK	// ActvL
#define BERT_INSYNC		(C3794_status&SYNC_STATUS_MASK) == SYNC_STATUS_MASK
#define LOS_ACTIVE		(C3794_status&LOS_STATUS_MASK) != LOS_STATUS_MASK	// ActvL
 */
void update_alarms_event_status()
{
	C3794_status = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_STATUS);
	//D(1, BUG("\nC3794_status_reg: %0X", C3794_status));
}

//_____________________________________________________________________________
void process_bert_errors()
{
	//unsigned long long bit_count_reg;
	unsigned long tmplong=0;
	unsigned int tmpint=0;


	if(LOS_ACTIVE){	// If we're in LOS
		if(BERT_STATE&0x80){   		// if we were in SYNC (or PatLOST)
			D(1, BUG("\nIn Process_BERT errors(): LOS_ACTIVE so Dropping BERT SYNC"));
			BERT_STATE |= 0x40; 	// flag loss of LOCK condition
			Misc_stat37 |= 0x01;    // Send a Bleep in PDA
			ConfigStatC37[MISC_STATC37_ptr] = Misc_stat37;
			}
		}

	// BERT is ON
	if(BERT_STATE&0x80){	// If BERT ON and we're in qualified SYNC
		transition_register_bit(ADDR_CTL, CTL_LC_MASK);		// load error counters
		//bit_count_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_BIT_COUNT_H);
		//bit_count_reg = (bit_count_reg << 32);
		//bit_count_reg += IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_BIT_COUNT_L);
		tmplong = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_BIT_ERROR_COUNT); // read new bit errors


		if(tmplong){         								// If New errors then ES++
			tmpint = BytesToInt(ConfigStatC37,BERT_ES1_ptr); 	// get current ES CNT
			tmpint++;							 	  		// Inc ES CNT
			SaveBytesInt(ConfigStatC37,BERT_ES1_ptr,tmpint); 	// Save into Status array
			ERRORS_LED = ON;
			HISTORY_LED = ON;
			//History = YES; *** this is NOT "HISTORY" to the GUI......
			//MISC_STATC37_ptr	MaxConfig+123	// Bleep^0, Error^1, History^2, tweedle^3, TimesUP^4, FAIL^5^, LOCAL_CHG^6
			Misc_stat37 |= 0x01;      // Send a Bleeeeeeep (bit^0)

			//**** UAS due to SES HANDLER *******************************
			/***
			if(tmplong > ds3_bert_ses){         // check if it was a SES
				++uas_cntinDC;					// Inc uas counter
				uas_cntoutDC = 0;					// Reset out cntr

				tmpint = BytesToInt(DatacommStat,BERT_SES13_ptr); 	// get current SES
				tmpint++;							 	  			// Inc SES CNT
				SaveBytesInt(DatacommStat,BERT_SES13_ptr,tmpint); 	// Save into Status array
				}
			else{
				if(uas_cntinDC > 9){
					if(++uas_cntoutDC > 9)
						uas_cntinDC = 0;			// Reset going in UAS cntr
					else
						++uas_cntinDC;
					}
				else
					uas_cntinDC = 0;
				}
			//***********************************************************
			****/
			//only update bit errs to status if new errors!
			tmplong += BytesToLong(ConfigStatC37,BECR3_ptr);	// Add new errors (tmplong) to total errors ConfigStatC37[BECR3_ptr]
			SaveBytesLong(ConfigStatC37,BECR3_ptr,tmplong); 	// Save total 32-bit CNT into Status array
			D(1, BUG("\nNew Bit errors: %lu", tmplong));
			}
			/*****
		else{ 								// Else if we no new errors
			if(uas_cntinDC > 9){
				if(++uas_cntoutDC > 9)
					uas_cntinDC = 0;		// Reset going in UAS cntr
				else
					++uas_cntinDC;
				}
			else
				uas_cntinDC = 0;
			}
		***/
		}
}


////     Status counters (R/clear on Write)
//#define ADDR_LOS_DETECT    0x040 // (wd add 0x10)    // Number of times Loss of Signal condition detected
//#define ADDR_LOS_ACTIVE    0x044 // (wd add 0x11)    // Number of frames where LOS is active
//#define ADDR_RDI_DETECT    0x048 // (wd add 0x12)    // Number of times Remote Defect Indication condition detected
//#define ADDR_RDI_ACTIVE    0x04c // (wd add 0x13)    // Number of frames where RDI is active
//#define ADDR_FRAMES_RX     0x050 // (wd add 0x14)    // Number of frames received during active tests
//#define ADDR_FRAMES_TX     0x054 // (wd add 0x15)    // Number of frames transmitted during active tests
void process_alarms_events()
{
	unsigned long tmplong=0;
	unsigned int tmpint=0;

	//dump_C3794_status(); // TMP DEBUG DISPLAY

	//***************************************************************
	//*** Handle the ALARM counters (inc'd real-time in the intr) ***
	//***************************************************************
	SaveBytesInt(ConfigStatC37,LOS_COUNT1_ptr,LOSCNT); 	// ALARM CNTR's inc in intr

	if(LOS_ACTIVE){                    // We have LOS so...
		tmpint = BytesToInt(ConfigStatC37,LOS_SEC1_ptr); 	// get current CNT
		tmpint++;							 				// Inc LOS-seconds CNT
		SaveBytesInt(ConfigStatC37,LOS_SEC1_ptr,tmpint); 	// Save into Status array
		ERRORS_LED = LED_ON;
		HISTORY_LED = LED_ON;
		//History = YES; 	//this history is used as history of only alarms & events (excluding BERTS)
		}
	SaveBytesInt(ConfigStatC37,OOF_COUNT1_ptr,OOFCNT);	// ALARM CNTR's inc in intr
	SaveBytesInt(ConfigStatC37,YEL_COUNT1_ptr,YELCNT);	// ALARM CNTR's inc in intr

//#define ADDR_BAD_FRAMES_RX          0x078 // (wd add 0x1e)    // Found error in any frame
//#define ADDR_BAD_FRAMES_PER_SEC     0x07c // (wd add 0x1f)    // Bad frames per second, always updating
/** definition pending clarification from OnCore
	if(InOOF){
		tmpint = BytesToInt(ConfigStatC37,OOF_SEC1_ptr); 	// get current CNT
		tmpint++;							 			// Inc OOF-seconds CNT
		SaveBytesInt(ConfigStatC37,OOF_SEC1_ptr,tmpint); 	// Save into Status array
		ERRORS_LED = LED_ON;
		HISTORY_LED = LED_ON;
		//History = YES; 	//this history is used as history of only alarms & events (excluding BERTS)
		}
**/

	if(RDI_ACTIVE){	// In Yellow Alarm
		tmpint = BytesToInt(ConfigStatC37,YEL_SEC1_ptr); 	// get current CNT
		tmpint++;							 			// Inc YEL-seconds CNT
		SaveBytesInt(ConfigStatC37,YEL_SEC1_ptr,tmpint); 	// Save into Status array
		ERRORS_LED = LED_ON;
		HISTORY_LED = LED_ON;
		//History = YES; 	//this history is used as history of only alarms & events (excluding BERTS)
		}


/** definition pending clarification from OnCore
#define ADDR_MSMTCH_ERR_HDR_BITS_L     0x080 // (wd add 0x20)    // Number of mismatch errors on headers, lower 32 bits
#define ADDR_MSMTCH_ERR_HDR_BITS_H     0x084 // (wd add 0x21)    // Number of mismatch errors on headers, upper 8 bits
#define ADDR_MSMTCH_ERR_HDR_FRAMES     0x088 // (wd add 0x22)    // Number of frames with mismatch errors on headers
#define ADDR_MSMTCH_ERR_OV_N_BITS_L    0x08c // (wd add 0x23)    // Number of mismatch errors on overhead N, lower 32 bits
#define ADDR_MSMTCH_ERR_OV_N_BITS_H    0x090 // (wd add 0x24)    // Number of mismatch errors on overhead N, upper 8 bits
#define ADDR_MSMTCH_ERR_OV_N_FRAMES    0x094 // (wd add 0x25)    // Number of frames with mismatch errors on overhead N
#define ADDR_MSMTCH_ERR_OV_FIX_BITS_L  0x098 // (wd add 0x26)    // Number of mismatch errors on overhead fixed, lower 32 bits
#define ADDR_MSMTCH_ERR_OV_FIX_BITS_H  0x09c // (wd add 0x27)    // Number of mismatch errors on overhead fixed, upper 8 bits
#define ADDR_MSMTCH_ERR_OV_FIX_FRAMES  0x0a0 // (wd add 0x28)    // Number of frames with mismatch errors on overhead fixed
****
	// Framing Errors
	tmplong = ReadBytesToInt(FOSCR1_ADDR);    		//Count Frame errors in ESF Framing Mode from the FOSCR register
	if(tmplong){
		if(ConfigStatC37[FRAME_ptr] == ds1ESF){		//Only count CRC errors in ESF Framing Mode
			if(tmplong > ds1_ESFfrm_ses){                 			// check if it was a SES
				tmpint = BytesToInt(ConfigStatC37,FBIT_SES1_ptr); 	// get current SES
				tmpint++;							 	  			// Inc SES CNT
				SaveBytesInt(ConfigStatC37,FBIT_SES1_ptr,tmpint); 	// Save into Status array
			}
		}

		tmplong += BytesToLong(ConfigStatC37,FBIT_COUNT3_ptr); 		// get current 32-bit CNT
		SaveBytesLong(ConfigStatC37,FBIT_COUNT3_ptr,tmplong); 		// Save total 32-bit CNT into Status array
		tmpint = BytesToInt(ConfigStatC37,FBIT_ES1_ptr); 			// get current CNT
		tmpint++;							 	  					// Inc ES CNT
		SaveBytesInt(ConfigStatC37,FBIT_ES1_ptr,tmpint); 			// Save into Status array
		ERRORS_LED = LED_ON;
		HISTORY_LED = LED_ON;
		History = YES;
		Misc_stat |= 0x01;      		// Send a Bleeeeeeep (bit^0)
		}


	//*********************************************************
	//***  Now lets handle the CODE ERR counters 			***
	//***	Note these are updated on the one_second tick   ***
	//*********************************************************
	tmplong = 0;
	tmplong = ReadBytesToInt(LCVCR1_ADDR);					// get new count
	if(tmplong){											// If additional errors then...
		if(tmplong > ds1_bpv_ses){                      	// check if it was a SES
			tmpint = BytesToInt(ConfigStatC37,BPV_SES1_ptr); 	// get current SES
			tmpint++;							 	  					// Inc SES CNT
			SaveBytesInt(ConfigStatC37,BPV_SES1_ptr,tmpint); 	// Save into Status array
			}

		tmplong += BytesToLong(ConfigStatC37,BPV_COUNT3_ptr); 	// get current 32-bit CNT
		SaveBytesLong(ConfigStatC37,BPV_COUNT3_ptr,tmplong); 	// Save total 32-bit CNT into Status array
		tmpint = BytesToInt(ConfigStatC37,BPV_ES1_ptr); 		// get current CNT
		tmpint++;							 	  					   // Inc ES CNT
		SaveBytesInt(ConfigStatC37,BPV_ES1_ptr,tmpint); 		// Save into Status array
  		ERRORS_LED = LED_ON;
		HISTORY_LED = LED_ON;
		History = YES;
  		Misc_stat |= 0x01;      // Send a Bleeeeeeep (bit^0)
		}

**/
}



void update_history()
{
	//*****************************************************************************************************
	//******* NO ONE ELSE SHOULD DIDDLE WITH ERRORS & HISTORY LED (unless they're turning them on) ********
	Misc_stat37 &= 0xF9;			// Clear LED bits sent to PDA in Misc_stat byte
	if(ERRORS_LED == ON)			// set to current state of ERRORS and HISTORY
		Misc_stat37 |= 0x02;      	// To GUI as LED ON = 1 and add a Bleeeeeeep (bit^0)
	//if(History)					// THIS history is ALL EVENTS and COUNTS Except BERT errors.
	if(HISTORY_LED)					// THIS history is ALL EVENTS, COUNTS And BERT errors.
		Misc_stat37 |= 0x04;
	else if((BERT_STATE&0x80)==0 && HISTORY_LED == ON)   // if NO History and BERT is OFF but the History LED is ON
		HISTORY_LED = LED_OFF;              	// ....then we only took a BIT error when BERT used to be on so CLEAR it out
	ConfigStatC37[MISC_STATC37_ptr] = Misc_stat37;
}
/*************************************************************************
* Copyright (c) 2017 Greenlee Communications Vista, CA USA.    			 *
* All rights reserved.                                                   *
*************************************************************************/
