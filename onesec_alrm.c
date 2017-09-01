/*************************************************************************
*		SFP Functions										*
*************************************************************************/
#include "gigle.h"
#include "extvars.h"

alt_alarm one_sec_alarm;
alt_u32 handle_one_sec_alarm();

alt_u32 handle_one_sec_alarm(void)
{
	unsigned long tmplong=0;
	unsigned int tmpint=0;

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


	if(C3794_READY){
		;// only process C37 if clock is up & running
	}


	return alt_ticks_per_second();
}



/*************************************************************************
* Copyright (c) 2017 Greenlee Communications Vista, CA USA.    			 *
* All rights reserved.                                                   *
*************************************************************************/
