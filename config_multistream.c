#include "gigle.h"
#include "extvars.h"

void config_multistream()
{
	int j;
//	hdrOffset=97;//position for header 3 val in ms
	stream_offset=12;//12
	ConfigChecksum2=(MaxConfig_ms-1);
		//stream_no = i; // the stream_no should update in the confstat to get the appropriate value
	D(1, BUG( "\n<> Total stream nos %d\n",num_of_streams));
	//D(1, BUG( "\n<> Total stream offset %d\n",stream_offset));
	//D(1, BUG( "\n<> copying MS CONFIG \n"));
	for(j=0;j<MaxConfig_ms;j++){    // load new config into ConfigStat
		ConfigMS[j] = RxBuffer[j];
	}
//	D(1, BUG("\tTxBuffer[TRAFFIC_GEN_CONFIG01_ptr]- packet length stream from config_ms 0%0d\n",BytesToInt(ConfigMS,TRAFFIC_GEN_CONFIG01_ptr)));
//	D(1, BUG( "\n<> ms ALIVE!copying MS CONFIG)\n"));
	D(1, BUG( "\n<>START TEST\n"));
	D(1, BUG(">> Start Stop Rx:%x\n",RxBuffer[STARTSTOP_ptr]));
		StartTest();
}
