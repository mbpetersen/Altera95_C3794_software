//*****************************************************************
//*** CONFIG TASK; Process Updated CONFIG array data from GUI *****
//*****************************************************************
#include "gigle.h"
#include "extvars.h"


void config_c37_hw()
{
unsigned char i=0,tmpint=0;
unsigned long LongInt=0;

   
	// execute the new configuration, byte-by-byte: only on changed Config bytes
	//for(i=1;i<MaxConfig-1;i++){
	for(i=2;i<22;i++){
		if(RxBuffer[i] != ConfigStatC37[i]){
  	 		switch(i){
  	 			case C37config_ptr:		//0
  	 				//ConfigStatC37[C37config_ptr] = 0;
					break;
  	 			case ID_ptr: 			//1	// Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
  	 				//ConfigStatC37[ID_ptr] = Packet_C3794;
					break;
  	 			case TEST_ptr: 			//2
  	 				//ConfigStatC37[TEST_ptr] = 0;
  	 				break;
  	 			case AUTO_ptr:    		//3	// Auto/Stress Test
  	 				//ConfigStatC37[AUTO_ptr] = 0;
  	 				break;
  	 			case MODE_ptr: 			//4	//
  	 				//ConfigStatC37[MODE_ptr] = 0;
  	 				break;

  	 			// ^4:CLK_MODE: 0=NW/recovered, 1=Internal OSC,
  	 			case CLOCK_ptr:         //5	NW/recovered or internal
  	 				//ConfigStatC37[CLOCK_ptr] = 0;
					if(((RxBuffer[CLOCK_ptr])^0x08) != ((ConfigStatC37[CLEARC37_ptr])&0x08)){  //NW/RECOVERED....
						if(((RxBuffer[CLOCK_ptr])^0x08) != 0){	// NW/Recovered Clock
							;//Recovered
							}
						else{  				//==1
							;//Internal
							//write_ppm_offset(PPM_offset);
							//Do something if NO CLOCK...
							//C3794_READY		(C3794_Status&0x80)
							}
						}
  	 				break;
  	 			case CLK_PPM1_ptr:		//6	 0 to 255 when written to FPGA it is masked to 9-bits (2's complement)
  	 			case CLK_PPM0_ptr:		//7	 	e.g.: +75PPM = 0x04B,  -100ppm = 0xFF9C, 9-bit's=0x19C
  	 				//SaveBytesInt(ConfigStatC37,CLK_PPM1_ptr,0);
  	 				if(BytesToInt(RxBuffer, CLK_PPM1_ptr) != BytesToInt(ConfigStatC37, CLK_PPM1_ptr)){
  	 					PPM_offset = BytesToInt(ConfigStatC37, CLK_PPM1_ptr); // maintain signed value - truncated in driver
  	 					write_ppm_offset(PPM_offset);
  	 					//Do something if NO CLOCK...
  	 					//C3794_READY		(C3794_Status&0x80)
  	 					}
  	 				break;
  	 			case CLK_SPR_ptr:		//8	// Spare
  	 				//ConfigStatC37[CLK_SPR_ptr] = 0;
  	 				break;

  	 			case TX_CHANS_ptr:    	//9	range:1-12
  	 				//ConfigStatC37[TX_CHANS_ptr] = 1;
  	 				set_N_CHANNELS(RxBuffer[i]);
  	 				break;
  	 			case RX_CHANS_ptr:    	//10 range:1-12
  	 				//ConfigStatC37[RX_CHANS_ptr] = 1;
  	 				set_RCV_N_CHANNELS(RxBuffer[i]);
  	 				break;
  	 			case OVH_errors_ptr:	//11	// insert OVH errors: F-bits/BPV's, etc.
  	 				//ConfigStatC37[OVH_errors_ptr] = 0;
  	 				break;
  	 			case Lpbk_ptr:      	//12	// Loopback Up/Down
  	 				//ConfigStatC37[Lpbk_ptr] = 0;
  	 				break;


  	 			case BERTC37_ptr: 			//13	// pattern
  	 				//ConfigStatC37[BERTC37_ptr] = 0;
					BERT = RxBuffer[BERTC37_ptr];
					//write_bertP();
					set_test_pattern();
					restart_bert(); // flush errors and set ELAP to 0
  	 				break;

  	 			case CLEARC37_ptr:    		//14	// RESTART^0, clear gTimesUp flag^1, PDA_ACK^7 to LOCAL_CHG (misc_stat^6)
  	 				//ConfigStatC37[CLEARC37_ptr] = 0;
					if(((RxBuffer[CLEARC37_ptr])&0x01) != ((ConfigStatC37[CLEARC37_ptr])&0x01)){
				   		restart_bert();
				     	ConfigStatC37[BSR_ptr] = BERT_STATE;
				   		//restart_history();	// use below for now....
				   		SaveBytesLong(ConfigStatC37,C37SECONDS3_ptr,0); // Clear ET Clock
				   		}
  	 				break;

  	 			case MISCC37_ptr:     		//15	// Insert BERT err^8, ?^0 , Power-Down? ^3;0x08=INVERT_BERT
  	 				//ConfigStatC37[MISCC37_ptr] = 0;
					//*** if INSERT LOGIC ERROR .....do it!
					if(((RxBuffer[MISCC37_ptr])&0x80) != ((ConfigStatC37[MISCC37_ptr])&0x80)) // if MISC - Insert Logic error is set
						Insert_BERT_biterror();


					//*********************************************************************************
					// RINV = EIR^4 Receive Data Inversion Select 0=do not invert RDATA 1=invert RDATA
					// TINV = EIR^5 Transmit Data Inversion Select 0=do not invert TDATA 1=invert TDATA
					//*********************************************************************************
					if(((RxBuffer[i])&0x08) != ((ConfigStatC37[MISCC37_ptr])&0x08)){
						if(((RxBuffer[i])&0x08) == 0x08){	// if INVERT BERT pattern is set
							BERT_INVERT = YES;
							Invert_BERT_pattern();
							}
						else{								// if INVERT BERT pattern is clear
							BERT_INVERT = NO;
							Invert_BERT_pattern();
							}
						}
  	 				break;

  	 			case BER_ptr:			//16	// set BER
  	 				//ConfigStatC37[BER_ptr] = 0;
  	 				set_C3794_BER((RxBuffer[i])&0x07);	// Clip to: 0 through 7,  10^0->10^-7
  	 				break;
  	 			case UserSize_ptr:		//17	// User BERT pattern size (1-32 bits)
  	 				//ConfigStatC37[UserSize_ptr] = 8;
  	 				break;
  	 			case USER3_ptr:  	   	//18	// SAVE & Execute USER BERT Pattern
  	 			case USER2_ptr:    		//19
  	 			case USER1_ptr:   		//20
  	 			case USER0_ptr:   		//21
  	 				//SaveBytesLong(ConfigStatC37,USER3_ptr,0);
  	 				break;

				default:
              			break;
            	}// End Switch 
         	}//*** End if New Config byte (RxBuffer != ConfigStat3)
   		}//* End For Loop
		
	//*** if Config Updated Copy Rxbuffer into Config portion of ConfigStatC37 array
	for(i=2;i<MaxConfig-1;i++)       	// load new config into ConfigStatC37 (1st 75 bytes)
		ConfigStatC37[i] = RxBuffer[i];	// don't load ExpMODconfig_ptr (#0), packet ID byte (1)  and Checksum (#74) byte
	/************************************************************************/	
      
}// End Config hardware function
