/*************************************************************************
*		TASK: Config GUI<-->GIGE Communication Link					     *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"


/* Prints "Hello World" and sleeps for three seconds */
void onesec_task(void* pdata)
{
	unsigned short cntr=0;
	//unsigned int tmpdata=0;
	unsigned long tmplong= 0;

	D(1, BUG("\n <> onesec_task ALIVE!\n"));

	//D(1, BUG("\nEnabling PRBS31 out optical port\n"));
	//write_ext_10G_phy(0xC000, 3, 0);	//Ensure we're using 16-bit counter!
	//write_ext_10G_phy(0x002A, 3, 0x30);	//0x10=Enable PRBS31 Generator 0x20=enable PRBS31 Monitor

	while (1)
	{
    	OSTimeDlyHMSM(0, 0, 0, 250);	// poll every 250mS
    	//D(1, BUG("In link_task loop\n");

		// *********************************************************************
		// *** INC One Second Elapsed Timer & Poll PHY's and SFP's *************
		// *********************************************************************
		if(++cntr == 4){
			cntr=0;

			//D(1, BUG("1.0001 ^2=PMA LinkStat: %02lX\t   1.000A ^0=PMD Signal Detect: %02lX\t",read_ext_10G_phy(0x0001, 1),read_ext_10G_phy(0x000A, 1)));
			//D(1, BUG("1.9003 : %02lX\n",read_ext_10G_phy(0x9003, 1)));
			//D(1, BUG("1.C002 : %02lX\n",read_ext_10G_phy(0xC002, 1)));

			//D(1, BUG("1.D006 GPIO-LED1: %02lX\t   1.D007 GPIO-LED2: %02lX\t",read_ext_10G_phy(0xD006, 1),read_ext_10G_phy(0xD007, 1)));
			//D(1, BUG("1.D008 GPIO-LED3: %02lX\n",read_ext_10G_phy(0xD008, 1)));

			//D(1, BUG("3.0020: %02lX\t   3.0021: %02lX\n",read_ext_10G_phy(0x0020, 3),read_ext_10G_phy(0x0021, 3)));

			if(TestStartGuiBit){	// If test is started, tick ELAP!
				tmplong = BytesToLong(ConfigStat,SECONDS3_ptr); 	// get and INC ET SECONDS
				tmplong++;
				if(tmplong > 0xFFFF0000)	// clamp max value
					tmplong = 0xFFFF0000;
				SaveBytesLong(ConfigStat,SECONDS3_ptr,tmplong);
				}// end Test is running

			tmplong = BytesToLong(ConfigStatB,SECONDS3_ptr); 	// get and INC ET SECONDS
			if(++tmplong > 0xFFFF0000)	// clamp max value
				tmplong = 0xFFFF0000;
			SaveBytesLong(ConfigStatB,SECONDS3_ptr,tmplong);

			if(TestStartGuiBit10){	// If test is started, tick ELAP!
				tmplong = BytesToLong(ConfigStat10G,SECONDS3_ptr); 	// get and INC ET SECONDS
				if(++tmplong > 0xFFFF0000)	// clamp max value
					tmplong = 0xFFFF0000;
				SaveBytesLong(ConfigStat10G,SECONDS3_ptr,tmplong);
				}

			//===Poll 10G PHY PRBS31 pattern error register
			//err_cnt = read_ext_10G_phy(0x002B, 3);
			//D(1, BUG("PHY pattern errors: %ld\n",err_cnt));
			}

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
