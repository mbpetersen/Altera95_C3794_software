/*************************************************************************
*		C37.94 BERT Test Functions  									 *
*				   														 *
* Copyright (c) 2017 Greenlee Communications  Vista, CA USA.    		 *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "c37dot94_def.h"



// Format is PTR PLR PSR TINV RINV PS
// The order of data must match the order in the enumeration.
static int test_pattern_data[16][7] ={
	//    PTR   PLR      PSR   Tinv Rinv PS USER_PAT     PS=1:qrw, 0:repetitive
		{0x04, 0x08, 0xffffffff, 0,  0,  1, 0},  // Poly511
		{0x08, 0x0A, 0xffffffff, 0,  0,  1, 0},  // Poly2047
		{0x0D, 0x0E, 0xffffffff, 1,  1,  1, 0},  // Poly2e15

		{0x10, 0x13, 0xffffffff, 0,  0,  1, 0},  // Poly2e20
		{0x11, 0x16, 0xffffffff, 1,  1,  1, 0},  // Poly2e23

		{0x10, 0x13, 0xffffffff, 0,  0,  3, 0},  // PolyQRSS	// include QRSS_mask

		{0x04, 0x05, 0xffffffff, 0,  0,  1, 0},  // Poly63
		{0x03, 0x06, 0xffffffff, 0,  0,  3, 0},  // PolyV54

		{0x00, 0x00, 0xffffffff, 0,  0,  0, 0x00000001},  // AIS, All-1's
		{0x00, 0x00, 0xfffffffe, 0,  0,  0, 0x00000000},  // A0S
		{0x00, 0x01, 0xfffffffe, 0,  0,  0, 0x00000001},  // Alt
		{0x00, 0x17, 0xff200022, 0,  0,  0, 0x00440004},  // 3in24
		{0x00, 0x07, 0xffffff40, 0,  0,  0, 0x00000040},  // 1in8 (1:7)
		{0x00, 0x07, 0xffffff42, 0,  0,  0, 0x00000042},  // 2in8

		{0x00, 0x05, 0xffffff10, 0,  0,  0, 0x00000010},  // 1in5
		{0x00, 0x06, 0xffffff20, 0,  0,  0, 0x00000020}   // 1in6
	};

// Write registers to select a test pattern
void set_test_pattern() {
	unsigned int temp;
    IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_MATCH, 0x24);
	// use enum value as index into test pattern table
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_PTR, test_pattern_data[BERT][0]);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_PLR, test_pattern_data[BERT][1]);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_PSR, test_pattern_data[BERT][2]);

	USER_PAT = test_pattern_data[BERT][6];//Capture USER_PAT for fixed checker

	// write RINV, TINV into ADDR_FRC_ERR
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_FRC_ERR);
	temp &= ~(FRC_RINV_MASK | FRC_TINV_MASK);
	temp |= (test_pattern_data[BERT][3] << FRC_TINV_OFS);
	temp |= (test_pattern_data[BERT][4] << FRC_RINV_OFS);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_FRC_ERR, temp);

	// write PS into ADDR_CTL
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL);
	temp &= ~(CTL_TL_MASK);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL, temp);
	temp &= ~CTL_PS_MASK;
	temp |= CTL_TL_MASK;
	temp |= (test_pattern_data[BERT][5] << CTL_PS_OFS);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL, temp);

	load_test_pattern();
}

void Invert_BERT_pattern()
{
	unsigned int temp=0;
	// write RINV, TINV into ADDR_FRC_ERR
	temp |= (test_pattern_data[BERT][3] << FRC_TINV_OFS); 	//Get defined TINV,RINV for current pattern
	temp |= (test_pattern_data[BERT][4] << FRC_RINV_OFS);
	//#define FRC_RINV_OFS                   12
	//#define FRC_RINV_MASK                  0x1000
	//#define FRC_TINV_OFS                   13
	//#define FRC_TINV_MASK                  0x2000
	if(BERT_INVERT){
		temp ^= FRC_TINV_MASK;		//INVERT TINV/RINV of selected pattern
		temp ^= FRC_RINV_MASK;
		}
	//Else set TINV/RINV to the standard for selected pattern
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_FRC_ERR, temp);
}

//#define ADDR_FRC_ERR           0x028 // (wd add 0xa)    // Force errors on Xmit, rcv blocks
//#define FRC_SBE_OFS            11
//#define FRC_SBE_MASK           0x800
//	SBE EIR.3 Single Bit Error Insert. A low to high transition will create a single bit
//	error. Must be cleared and set again for a subsequent bit error to be
//	inserted.
void Insert_BERT_biterror()
{
	transition_register_bit(ADDR_FRC_ERR, FRC_SBE_MASK);
}

void read_C3794_BERT_counts()
{
	unsigned long long bit_count_reg;
	unsigned int bit_error_count_reg;

	transition_register_bit(ADDR_CTL, CTL_LC_MASK);
	bit_count_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_BIT_COUNT_H);
	bit_count_reg = (bit_count_reg << 32);
	bit_count_reg += IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_BIT_COUNT_L);
	bit_error_count_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_BIT_ERROR_COUNT);
    D(-1, BUG("Bit count: %llu, Bit error count: %u, BER: %4.3e\n", bit_count_reg, bit_error_count_reg, (bit_error_count_reg/(double)bit_count_reg)));
}

void load_test_pattern()
{
	transition_register_bit(ADDR_CTL, CTL_TL_MASK);
}


void set_C3794_BER(unsigned char BER_value)  // 0 to 7:  No BER, 10-1 through 10-7
{
	unsigned int temp_reg;

	temp_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_FRC_ERR);
	temp_reg &= ~FRC_EIB_MASK;
	temp_reg |= (BER_value << FRC_EIB_OFS);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_FRC_ERR, temp_reg);
}

void start_BERT_test()
{
	// clear BERT counts in SW for now, until HW does the same.
	transition_register_bit(ADDR_CTL, (CTL_START_MASK | CTL_LC_MASK));
}

void stop_BERT_test()
{
	transition_register_bit(ADDR_CTL, CTL_STOP_MASK);
}

/*********************************************************/
void restart_bert()
{
	transition_register_bit(ADDR_CTL, CTL_LC_MASK); // pulse to latch counts

	SaveBytesLong(ConfigStatC37,BECR3_ptr,0);			// Clear BERT Counters
	SaveBytesInt(ConfigStatC37,BERT_ES1_ptr,0);
	SaveBytesInt(ConfigStatC37,BERT_SES1_ptr,0);
}

/***
//********************************************************************************
//   BERT_STATE
 *   0x00: NO SYNC
 *   0x80: PAT SYNC  (1st SYNC)
 *   0x40: SYNC LOSS (Lost SYNC)
 *   0xC0: PAT LOST  (2nd SYNC - PAT reSYNC'd
 *
 * ConfigStatC37[BSR_ptr] = BERT_STATE;

//**** ^7=IN-SYNC (1=InSYNC),  ^6=2+SYNC: (0=1stSYNC,1=subsequentSYNC)
//*** gPatSync	(gStatus[BSR_ptr]&0xC0)	*******	if !([BSR_ptr]&0x80) is NO SYNC
//												if [BSR_ptr]&0xC0==0x80 is SYNC
//												if [BSR_ptr]&0xC0==0xC0 is PatLOST
//********************************************************************************

//***********************************************************************
void process_bert_sync()		//** BERT SYNC verification Processing **
//***********************************************************************

{

	//******************************************************
	//***** BERT SYNC VERIFICATION ROUTINE *****************
	//******************************************************
	//*** First Handle going IN and Falling OUT of SYNC  ***
	//******************************************************
	BERT_STATUS = MVAR(char,BERTSRADDR);		// Read BERT Status Register
	//PriStat[PRIRESULT_ptr] = BERT_STATUS;	// DEBUG save into result for eyeballing!
	//if(BERT_STATUS&0x08){					// if BED is set toggle LED!
	//	LINKFAULT_LED = LED_ON;	  				// DEBUG ...Turn ON FAULT LED
	//	MVAR(char,LEDADDR)=LED;					// DEBUG Update LED states
	//	 }

	//**************************************************************************
	if(RLOS){ 							// If RLOS and...
		if(BERT_STATE&0x80){   		// if we were in SYNC (or PatLOST)
			BERT_STATE |= 0x40; 		// flag loss of LOCK condition
			PMisc_stat |= 0x01;      // Send a Bleep in PDA
			PriStat[PMISC_STAT_ptr] = PMisc_stat;
			}
		}
	//***************************************************************************
	if(SYNC){      // if 2172 thinks we're in SYNC then we'd better check!
		if(RA1A0){ 	// If we're receiving All1's or All0's...check if we're sending ALL1's or ALL0's
			if(RALL1 && BERT==7)  		// if we see All1 and we're sending ALL1, then...
				BERT_SYNC = YES;  			// VALID A1+A0 SYNC
			else if(RALL0 && BERT==8)  // if we see All0 and we're sending ALL0, then...
				BERT_SYNC = YES;  			// VALID A1+A0 SYNC
			else{
				BERT_SYNC = NO;   			// invalid A0+A1 SYNC
				}
			incntr = SYNCINCNT;        		// set-up going in  cntr to 5 consecutive
			outcntr = 1;         			// set-up going out cntr to 1 consecutive
			}
		else if(BERT_CONTROL&0x20){		// if QRW and SYNC then we're in SYNC!
			BERT_SYNC = YES;     			// VALID QRW SYNC
			incntr = SYNCINCNT;  			// set-up going in  cntr to 5 consecutive
			outcntr = 1;         			// set-up going out cntr to 1 consecutive
			}
		else{								// If NOT ALL1's, ALL0's or a QRW then....
			//for(i=0;i<5;i++)// huh??? hmmmm...let's speed up this puppy for DS3+ rates!
			check_fixed();       		// validate fixed pattern sync
			}
		}

	//***** NOT IN SYNC or we have LOS (and we're not in OOF ??? or self-lpbk ???) *****
	else{  // if we've got an intr and it's NOT SYNC it must be RLOS
		incntr = SYNCINCNT;         // set-up going in cntr to 5 consecutive
		outcntr = SYNCOUTCNT;       // set-up going out cntr to 5 consecutive
		BERT_SYNC = NO;
		MVAR(char,PCR_ADDR)  = BERT_CONTROL|0x02; // force RESYNC
		MVAR(char,PCR_ADDR)  = BERT_CONTROL;
		}


	//********************************************************
	//*** Then Handle BERT SYNC state changes and flag PDA ***
	//********************************************************************************
	//**** ^7=IN-SYNC (1=InSYNC),  ^6=2+SYNC: (0=1stSYNC,1=subsequentSYNC)
	//*** gPatSync	(gStatus[BSR_ptr]&0xC0)	*******	if !([BSR_ptr]&0x80) is NO SYNC
	//												if [BSR_ptr]&0xC0==0x80 is SYNC
	//												if [BSR_ptr]&0xC0==0xC0 is PatLOST
	//********************************************************************************
	if(BERT && !(BERT&0x80)){ //*** if BERT is ON (and we're not doing a lpbk), handle in/out sync ***
		if(BERT_SYNC){         //******** LOCKED ***********************************
			if(!(BERT_STATE&0x80)){ 	// If we're not already in SYNC then...
				BERT_STATE |= 0x80;		// Set SYNC flag.
				restart_bert();			// Reset all BERT Counters
				}
			}
		else{ //******** NOT LOCKED *******************************
			if(BERT_STATE&0x80){      	 // if we were in SYNC (or PatLOST)
				BERT_STATE &= ~0x80;    // Clear SYNC flag.
				BERT_STATE |= 0x40;     // flag loss of LOCK condition
				PMisc_stat |= 0x01;      // Send a Bleep in PDA
				PriStat[PMISC_STAT_ptr] = PMisc_stat;
				}
			}

		PriStat[BSR_ptr] = BERT_STATE;				// pass current SYNC state
		}// END if PRI BERT IS ON
	//**************************************************
	//*********** END BERT-PRI SYNC HANDLING ***********
	//**************************************************
}
**/


//************************************************************************
//**** BERT fixed pattern Validation routine *****************************
//                                                                		//
//  The DS1's we use the internal 2155 BERT features which do not,   	//
//			require this validation procedure.                          //
//                                                                   	//
//	 The PRI-ISDN module utilizes the onboard DS2172/3 Chip,            //
//			which requires this function to verify fixed patterns!      //
//************************************************************************
/**
void check_fixed(void)
{
	_Bool MATCHED;
   unsigned char i, pat_length;
   unsigned long rotptr,mask,rcvword,temp;

   rcvword  = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_PRR);
   //MVAR(char,PCR_ADDR)  = BERT_CONTROLP|0x08; // reload rcv pattern regs
   //MVAR(char,PCR_ADDR)  = BERT_CONTROLP;
   temp  = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL);
   IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL, temp|CTL_RL_MASK);  //reload rcv pattern regs
   IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL, temp);

   rotptr = 0x00000001;
   mask   = rotptr;

   pat_length = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_PLR);
   for(i=0;i<pat_length;i++)       // identify rotate bit position
      rotptr <<= 1;
   rotptr <<= 1;
   mask = rotptr-1;

   MATCHED = NO;

   if((rcvword&mask) == (USER_PATP&mask)){
      MATCHED = YES;
      }
   else{
      rcvword &= mask;              // only shift pertinent bits
      for(i=0;i<=pat_length;i++){
         rcvword <<= 1;             // else, leave LSB '0'
         if(rcvword&rotptr)         // if MSB of PAT is '1'
            rcvword++;              // add it into LSB!

         if((rcvword&mask) == (USER_PATP&mask)){
            MATCHED = YES;
            break;
            }
         }// end rotate for-loop
      }

   //**************************************************
   //*** BERT IN/OUT SYNC DEBOUNCER 5/5 consecutive ***
   //**************************************************
   //unsigned long   USER_PATP cfg_hw
   if(BERT){
      if(MATCHED){
         outcntr = SYNCOUTCNT;         // set-up going out cntr to 5 consecutive
         if(!BERT_SYNCP){
            if(--incntr == 0){
               BERT_SYNCP = YES;
               incntr = 1;             // dont let it go to 0xFF
               }
            }
         }
      else{
         incntr = SYNCINCNT;           // set-up going in cntr to 5 consecutive
         if(BERT_SYNCP){
            if(--outcntr == 0)
               BERT_SYNCP = NO;
            }
         }
      }
   //*********************************************
}
*/






//_______________________________________________________________________________________________________________
void run_C3794_BERT_test1()
{
unsigned int i=0;
//	unsigned short c3794_delay_cnt=0;

	D(1, BUG("\n\t	=========== C3794 BERT TEST1, Applied BER: 1.0e-7 (8 minute test) ======================\n"));

	//D(1, BUG("\n\t FAR END LOOPBACK ENABLED\n"));
	//set_far_end_loopback();
	set_internal_loopback();  //D(1, BUG("\n\t INTL LOOPBACK ENABLED\n"));
	// set IDLE code byte to something other than default of AIS.
	set_IDLE_code((unsigned char)0xc3);	//D(1, BUG("\n\t IDLE CODE SET\n"));  0xC3:   1100,0011
	// set xmit and rcv channel numbers to 12
	set_N_CHANNELS(9);  //D(1, BUG("\n\t N channels set\n"));
	set_RCV_N_CHANNELS(9);   //D(1, BUG("\n\t N RCV channels set\n"));

	// wait for receive clock to lock, and good framing pattern to be found
	wait_for_rcv_clock_locked();  //D(-1, BUG("\n\t RCV clock locked\n"));
	wait_for_LOS_GOOD();    //D(1, BUG("\n\t LOS GOOD\n"));

	// load a test pattern via registers
	BERT = Poly2047;
	set_test_pattern();

	// start the test
	start_BERT_test();
	D(1, BUG("\n BER=0"));

	// run 10 seconds with no errors
	//OSTimeDlyHMSM(0, 2, 0, 0);
	read_C3794_BERT_counts();
	verify_register(ADDR_BIT_ERROR_COUNT, 0x0, 0xffffffff);
	for(i=0;i<24;i++){
		OSTimeDlyHMSM(0, 0, 5, 0);
		read_C3794_BERT_counts();
		}


	// force errors at 1.0e-7
	set_C3794_BER(0x7);  D(1, BUG("\n Setting BER=1.0e-7"));

	D(1, BUG("\n\tReading 1: "));
	for(i=0;i<24;i++){
		OSTimeDlyHMSM(0, 0, 5, 0);
		read_C3794_BERT_counts();
		}

	D(1, BUG("\n\n\tReading 2: "));
	for(i=0;i<24;i++){
		OSTimeDlyHMSM(0, 0, 5, 0);
		read_C3794_BERT_counts();
		}

	OSTimeDlyHMSM(0, 2, 0, 0);

	stop_BERT_test();

	D(1, BUG("\n\n\tReading 2: "));
	for(i=0;i<24;i++){
		OSTimeDlyHMSM(0, 0, 5, 0);
		read_C3794_BERT_counts();
		}

	D(1, BUG("\n\n\tStopping: "));
	read_C3794_BERT_counts();
}

void run_C3794_BERT_test2()
{

	unsigned short c3794_delay_cnt=0;

	D(-1, BUG("\n\t	=========== C3794 BERT TEST2, applied BER: 1.0e-5 (20 second test) ======================\n"));

	set_internal_loopback();
	// set IDLE code byte to something other than default of AIS.
	set_IDLE_code((unsigned char)0xc3);
	// set xmit and rcv channel numbers to 12
	set_N_CHANNELS(1);
	set_RCV_N_CHANNELS(1);

	// wait for receive clock to lock, and good framing pattern to be found
	wait_for_rcv_clock_locked();
	wait_for_LOS_GOOD();

	// load a test pattern via registers
	BERT = ThreeIn24;
	set_test_pattern();

	// force errors at 10e-5
	set_C3794_BER(0x5);

	// start the test
	start_BERT_test();

	// run for 20 seconds
	do {
		OSTimeDlyHMSM(0, 0, 0, 100);
	} while (c3794_delay_cnt++ < 200);

	stop_BERT_test();

	read_C3794_BERT_counts();

	OSTimeDlyHMSM(0, 0, 0, 1000);

	// no bit errors should be recorded when test is over
	read_C3794_BERT_counts();
	verify_register(ADDR_BIT_ERROR_COUNT, 0x0, 0xffffffff);

}


void run_C3794_BERT_test3()
{

	unsigned short c3794_delay_cnt=0;

	D(-1, BUG("\n\t	=========== C3794 BERT TEST3, Single bit error, BER: 2.64e-7 (60 second test) ======================\n"));

	set_internal_loopback();
	// set IDLE code byte to something other than default of AIS.
	set_IDLE_code((unsigned char)0xc3);
	// set xmit and rcv channel numbers to 12
	set_N_CHANNELS(6);
	set_RCV_N_CHANNELS(6);

	// wait for receive clock to lock, and good framing pattern to be found
	wait_for_rcv_clock_locked();
	wait_for_LOS_GOOD();

	// load a test pattern via registers
	BERT = ThreeIn24;
	set_test_pattern();
	set_C3794_BER(0x0);


	// start the test
	start_BERT_test();

	// run for 60 seconds. Apply SBE every 10 seconds, on the 5's.
	do {
		OSTimeDlyHMSM(0, 0, 0, 100);
		if ((c3794_delay_cnt % 10) == 5)
			transition_register_bit(ADDR_FRC_ERR, FRC_SBE_MASK);
	} while (c3794_delay_cnt++ < 600);

	stop_BERT_test();

	read_C3794_BERT_counts();
}


/*************************************************************************
* Copyright (c) 2017 Greenlee Communications  Vista, CA USA.    		*
* All rights reserved.                                                  *
*************************************************************************/
