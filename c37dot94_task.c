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
	//unsigned short cntr=0;
	//unsigned char i=0;
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
		}
	else{
		D(1, BUG(" CLOCK dead - no C37.94 processing can occur! \n"));
		//OSTaskDel(C3794_TASK_PRIORITY);	// 3
		}


	while (1)
	{
		if(!C3794_READY)
			OSTaskSuspend(C3794_TASK_PRIORITY);

		OSTimeDlyHMSM(0, 0, 0, 100);



		//================================================================

	}//End infinite while
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
