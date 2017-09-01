/*************************************************************************
*		TASK: Config GUI<-->GIGE Communication Link					     *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"


/* Prints "Hello World" and sleeps for three seconds */
void link_task(void* pdata)
{
	//unsigned short cntr=0;
	//unsigned char i=0;
	//unsigned long tmplong= 0;

	D(1, BUG("\n <> link_task ALIVE!\n"));
	while (1)
	{
		OSTaskSuspend(LINK_TASK_PRIORITY);	// LINK_TASK_PRIORITY      2
		config_gui_link();


		//================================================================
		// Poll Port 10G, A&B PHY Link status
		//================================================================
		read_10G_phy_operating_mode();
		read_phy_operating_mode(TRIPLE_SPEED_ETHERNET_0_BASE);
		read_phy_operating_mode(TRIPLE_SPEED_ETHERNET_1_BASE);

		//DEBUG 100FX:
		//read_phy_operating_mode_printf(TRIPLE_SPEED_ETHERNET_0_BASE);




		//================================================================
		// Poll SFP Rx Power Measurements
		//================================================================
		if((ConfigStat[SFP_STAT_ptr]&0x02) == 0)	// if an SFP is installed in A
			read_sfp_power(PortA);	// when Packet test is running - don't poll sfp ??

		//D(1, BUG("ConfigStat[PHY_OPT_POWER1&0_ptr]=%02X %02X\n",ConfigStat[PHY_OPT_POWER1_ptr],ConfigStat[PHY_OPT_POWER0_ptr]));
		if((ConfigStat[SFP_STAT_ptr]&0x04) == 0)	// if an SFP is installed in B
			read_sfp_power(PortB);

		//if(GIGE10){
			if((read_ext_10G_phy(0xD002, 1))&0x0004) // if Optical Signal Detected
				read_10G_sfp_power(PortA);
		//	}

	//================================================================
	// Poll port-A,B,10G MAC stat's and load for GUI
	//================================================================

//		if(!GIGE10 && GIGE_A && (ConfigStat[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20)  //only when in LPBK && User is looking at it!
//						read_tse_stat_counters(TRIPLE_SPEED_ETHERNET_0_BASE);
//
//		if(GIGE10 && SFP_10G_PRESENT && (ConfigStat10G[CORE_SIGROUTE_MODES_ptr] & 0x20) == 0x20)   //only when in LPBK && User is looking at it!
//				read_10G_MAC_Stats();
//
//		if(!GIGE10 && !GIGE_A)  // if in TSE port-B poll TSE MAC B-stat's only when User is looking at it!
//				read_tse_stat_counters(TRIPLE_SPEED_ETHERNET_1_BASE);

	}//End infinite while
}


/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
