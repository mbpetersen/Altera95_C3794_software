/*************************************************************************
*		C37.94 Task									 					 *
*				   														 *
* Copyright (c) 2017 Greenlee Communications  Vista, CA USA.    		 *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "c37dot94_def.h"


/* Prints "Hello World" and sleeps for three seconds */
void c37dot94_task(void* pdata)
{
	unsigned char cnt=0, old_status=0;
	unsigned char i=0;
	//unsigned long tmplong= 0;

	D(1, BUG("\n <> c37dot94_task ALIVE!\n"));

	//___________________________________________________________________________
	D(1, BUG("\n---------> C37.94 initializing 131.062M CLOCK"));
	write_ppm_offset(0);
	D(1, BUG("\nCLK initialized:"));
	if(C3794_READY){
		D(1, BUG(" CLOCK up & running! \n"));
		//OSTaskResume(C3794_TASK_PRIORITY);
//		reinit_c37dot94();
		init_c37dot94();
		init_debug_test();
		}
	else{
		D(1, BUG(" CLOCK dead - no C37.94 processing can occur! \n"));
		OSTaskDel(C3794_TASK_PRIORITY);	// 3
		}

	D(1, BUG("\n=========== STARTING C3794 TASK WHILE LOOP ======================\n"));
	//================================================================
	while (1)
	{

		if(!C3794_READY){
			D(1, BUG("\n\n CLOCK dead - SUSPENDING C37.94 Task!!! \n"));
			OSTaskSuspend(C3794_TASK_PRIORITY);
			}

		OSTimeDlyHMSM(0, 0, 0, 100);
		update_alarms_event_status();
		if(C3794_status != old_status){
			old_status = C3794_status;
			D(1, BUG("\n\n______STATUS CHANGE_____________________________ \n"));
			dump_C3794_status();
			}

		if(++cnt%1 == 0){	// every other time...
 	        if(BERT)
 	        	process_bert_sync();

 	        //need to fold in V.54 handling
			}
	}//End infinite while
}


void init_debug_test()
{
	//================================================================
	//set_far_end_loopback();	D(1, BUG("\n\t FAR END LOOPBACK ENABLED\n"));
//	set_internal_loopback();  D(1, BUG("\n\t INTL LOOPBACK ENABLED\n"));
	// set IDLE code byte to something other than default of AIS.
	set_IDLE_code((unsigned char)0xFF);	D(1, BUG("\n\t IDLE CODE SET\n"));  // 0xC3:   1100,0011
	// set xmit and rcv channel numbers to 12
	ConfigStatC37[TX_CHANS_ptr] = 6;
	ConfigStatC37[RX_CHANS_ptr] = 6;
	RxBuffer[TX_CHANS_ptr] = ConfigStatC37[TX_CHANS_ptr];
	RxBuffer[RX_CHANS_ptr] = ConfigStatC37[RX_CHANS_ptr];
	set_XMT_N_CHANNELS(ConfigStatC37[RX_CHANS_ptr]);
	set_RCV_N_CHANNELS(ConfigStatC37[RX_CHANS_ptr]);

	// wait for receive clock to lock, and good framing pattern to be found
//	wait_for_rcv_clock_locked();
//	wait_for_LOS_GOOD();

	// load a test pattern via registers
	//BERT = Poly2047;
	//set_test_pattern();
	// start the test
	//start_BERT_test();
	//D(1, BUG("\n BER=0"));

	//issue_UI_command(BERTC37_ptr,Poly2047);	//Emulate UI command
}


//Simulate UI command
void issue_UI_command(unsigned short msg_element, unsigned char value)
{
	RxBuffer[msg_element] = value;
	config_c37_hw();	// call confg_hw to execute command
}

void init_c37dot94()
{
	ConfigStatC37[C37config_ptr] = 0;
	ConfigStatC37[ID_ptr] = Packet_C3794;
	ConfigStatC37[TEST_ptr] = 0;
	ConfigStatC37[AUTO_ptr] = 0;
	ConfigStatC37[MODE_ptr] = 0;
	ConfigStatC37[CLOCK_ptr] = 0;		// 0=rcvrd
	SaveBytesInt(ConfigStatC37,CLK_PPM1_ptr,0);
	ConfigStatC37[CLK_SPR_ptr] = 0;

	ConfigStatC37[TX_CHANS_ptr] = 1;
	ConfigStatC37[RX_CHANS_ptr] = 1;
	ConfigStatC37[OVH_errors_ptr] = 0;
	ConfigStatC37[Lpbk_ptr] = 0;
	ConfigStatC37[BERTC37_ptr] = 0;
	ConfigStatC37[CLEARC37_ptr] = 0;
	ConfigStatC37[MISCC37_ptr] = 0;	// no insert, no invert
	ConfigStatC37[BER_ptr] = 0;

	ConfigStatC37[UserSize_ptr] = 8;
	SaveBytesLong(ConfigStatC37,USER3_ptr,0);

}

/*************************************************************************
* Copyright (c) 2017 Greenlee Communications  Vista, CA USA.    		*
* All rights reserved.                                                  *
*************************************************************************/
