/*************************************************************************
*		C37.94 Functions  									 			 *
*				   														 *
* Copyright (c) 2017  Greenlee Communications  Vista, CA USA.    		 *
* All rights reserved.                                                   *
*************************************************************************/
#include "gigle.h"
#include "extvars.h"
#include "c37dot94_def.h"



void transition_register_bit(unsigned int addr, unsigned int mask)
{
	unsigned int temp_reg;

	temp_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, addr);
	temp_reg &= ~mask;
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, addr, temp_reg);
	temp_reg |= mask;
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, addr, temp_reg);
}

void set_register_bit(unsigned int addr, unsigned int mask)
{
	unsigned int temp_reg;

	temp_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, addr);
	temp_reg |= mask;
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, addr, temp_reg);
}

void clear_register_bit(unsigned int addr, unsigned int mask)
{
	unsigned int temp_reg;

	temp_reg = IORD_32DIRECT(TOP_C37DOT94_0_BASE, addr);
	temp_reg &= ~mask;
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, addr, temp_reg);
}


void set_internal_loopback()
{
	set_register_bit(ADDR_DEBUG, DEBUG_LOOPBACK_MASK);
}
void clear_internal_loopback()
{
	clear_register_bit(ADDR_DEBUG, DEBUG_LOOPBACK_MASK);
}
void set_far_end_loopback()
{
	set_register_bit(ADDR_DEBUG, DEBUG_FAR_END_LOOPBACK_MASK);
}
void clear_far_end_loopback()
{
	clear_register_bit(ADDR_DEBUG, DEBUG_FAR_END_LOOPBACK_MASK);
}
void set_IDLE_code(unsigned char value) {
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_IDLE_CODE, value);
}

void set_XMT_N_CHANNELS(unsigned char value) {
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_XMT_N_CHANNELS, value);
	D(1, BUG("\n\t XMT N channels set to %d\n", value));
}

void set_RCV_N_CHANNELS(unsigned char value) {
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RCV_N_CHANNELS, value);
	D(1, BUG("\n\t RCV N channels set to %d\n", value));
}

void load_RCVD_FRAME()
{
	unsigned long framedata=0;
	unsigned short i=0;

	transition_register_bit(ADDR_CTL, CTL_RWLB_MASK);	// latch in new frame(s) content

	//Wait for FrameWindow Data to be ready: CTL_RWLB_MASK bit clear...
	framedata = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL);
	while((framedata&CTL_RWLB_MASK) != CTL_RWLB_MASK && ++i < 10){ // should be ready in <250uS (512 bits @ 2.048M) - give him 1500uS
		usleep(150);
		framedata = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_CTL);
		}
	if(i==10){
		D(1, BUG("\nReceived Frame Window TIMEOUT:  Not ready in 1.5mS!!"));
		}

	//HDR1_ptr +0-31  (32 total bytes)
	D(1, BUG("\nReceived Frame1 Content: "));
	for(i=0;i<8;i++){
		//SaveBytesLong(ConfigStatC37, HDR1_ptr+(i*4), IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RCV_WINDOW_BASE+i));
		framedata = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RCV_WINDOW_BASE+i);
		D(1, BUG("%0lX ", framedata));
		SaveBytesLong(ConfigStatC37, HDR1_ptr+(i*4), IORD_32DIRECT(TOP_C37DOT94_0_BASE, framedata));
		}

	get_BERT_rcv_pattern();	//Check BERT PRR register as well to confirm it matches (somewhat) the content seen in the Frame (it should)

	//Eyeball Second Frame for debugging
	D(1, BUG("\nReceived Frame2 Content: "));
	for(i=8;i<16;i++){
		framedata = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RCV_WINDOW_BASE+i);
		D(1, BUG("%0lX ", framedata));
		}
	D(1, BUG("\n"));

	get_BERT_rcv_pattern();

}





int LOF_WATCHDOG_LIMIT = 500;
void wait_for_FrameSync()
{
	unsigned int watchdog_timer = 0;

	update_alarms_event_status();
	D(1, BUG("\n\tRCVCLOCK_LOCKED:%d  TEST_ACTIVE:%d  LOF_ACTV:%d  RDI_ACTV:%d  BERT_INSYNC:%d\n",RCVCLOCK_LOCKED, TEST_ACTIVE, LOF_ACTIVE, RDI_ACTIVE, BERT_INSYNC));
	//while ((status_reg & LOF_STATUS_MASK) != (LOF_INACTIVE << LOF_STATUS_OFS)) {
	while (LOF_ACTIVE) { //while LOF is Active ( == 0x04)
		if (watchdog_timer >= LOF_WATCHDOG_LIMIT) {
			D(1, BUG("\n\t	FRAME SYNC TIMEOUT"));
			break;
			}
		OSTimeDlyHMSM(0, 0, 0, 10);  //wait 10mS
		watchdog_timer++;
		update_alarms_event_status();
		}
	if(LOF_ACTIVE){
		D(1, BUG("\n\t	CANNOT FRAME SYNC!  (in %d mS)",watchdog_timer*10));
		}
	else{
		D(1, BUG("\n\t	FRAME SYNC Obtained!  (in %d mS)",watchdog_timer*10));
		}
	D(1, BUG("\n\tRCVCLOCK_LOCKED:%d  TEST_ACTIVE:%d  LOF_ACTV:%d  RDI_ACTV:%d  BERT_INSYNC:%d\n",RCVCLOCK_LOCKED, TEST_ACTIVE, LOF_ACTIVE, RDI_ACTIVE, BERT_INSYNC));
}

int RCV_CLOCK_WATCHDOG_LIMIT = 500;
void wait_for_rcv_clock_locked()
{
	unsigned int watchdog_timer = 0;

	update_alarms_event_status();
	D(1, BUG("\n\tRCVCLOCK_LOCKED:%d  TEST_ACTIVE:%d  LOF_ACTV:%d  RDI_ACTV:%d  BERT_INSYNC:%d\n",RCVCLOCK_LOCKED, TEST_ACTIVE, LOF_ACTIVE, RDI_ACTIVE, BERT_INSYNC));
	while (RCVCLOCK_NOTLOCKED) {
		if (watchdog_timer >= RCV_CLOCK_WATCHDOG_LIMIT) {
			D(1, BUG("\n\t	RCV_CLOCK_WATCHDOG TIMEOUT"));
			break;
			}
		OSTimeDlyHMSM(0, 0, 0, 10);  //wait 10mS
		watchdog_timer++;
		update_alarms_event_status();
		}
	if(RCVCLOCK_LOCKED){
		D(1, BUG("\n\t	RCV_CLOCK Locked!  (in %d mS)",watchdog_timer*10));
		}
	else{
		D(1, BUG("\n\t	No LOCK on RCV Clock!  (in %d mS)",watchdog_timer*10));
		}
	D(1, BUG("\n\tRCVCLOCK_LOCKED:%d  TEST_ACTIVE:%d  LOF_ACTV:%d  RDI_ACTV:%d  BERT_INSYNC:%d\n",RCVCLOCK_LOCKED, TEST_ACTIVE, LOF_ACTIVE, RDI_ACTIVE, BERT_INSYNC));
}


void verify_register(unsigned int addr, unsigned int expected, unsigned int mask)
{
	unsigned int actual;

	actual = IORD_32DIRECT(TOP_C37DOT94_0_BASE, addr);
	if ((actual & mask) != (expected & mask))
		D(1, BUG("\tERROR: verify_register: actual(%0x) != expected(%0x) with mask(%0x)\n", actual, expected, mask));

}

//Update Alarm & Events Flags
/*
#define RCVCLOCK_LOCKED	(C3794_status&CLOCK_STATUS_MASK) == CLOCK_STATUS_MASK
#define TEST_ACTIVE		(C3794_status&TEST_STATUS_MASK) == TEST_STATUS_MASK
#define RDI_ACTIVE		(C3794_status&RDI_STATUS_MASK) != RDI_STATUS_MASK	// ActvL
#define BERT_INSYNC		(C3794_status&SYNC_STATUS_MASK) == SYNC_STATUS_MASK
#define LOF_ACTIVE		(C3794_status&LOF_STATUS_MASK) != LOF_STATUS_MASK	// ActvL
 */
void update_alarms_event_status()
{
	C3794_status = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_STATUS);
	//D(1, BUG("\nC3794_status_reg: %0X", C3794_status));
}

//This Register may be something completely different:
//AWAITING DEFINITION...........
void update_bert_status()
{
	BERT_STATUS = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_PSIR);
	//D(1, BUG("\nC3794_status_reg: %0X", C3794_status));
}


void dump_C3794_status() {
	unsigned int temp;

	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_STATUS);
	D(1, BUG("\n\tINFO: C3794 STATUS register: 0x%02x", temp));
	D(1, BUG("\n\tRCVCLOCK_LOCKED:%d  TEST_ACTIVE:%d  LOF_ACTV:%d  RDI_ACTV:%d  BERT_INSYNC:%d\n",RCVCLOCK_LOCKED, TEST_ACTIVE, LOF_ACTIVE, RDI_ACTIVE, BERT_INSYNC));
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_LOF_DETECT_CNT);
	D(1, BUG("\tINFO: C3794 LOF_DETECT_CNT_  register: %d\n", temp));
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_LOF_NUMFRMS_ACTIVE);
	D(1, BUG("\tINFO: C3794 LOF_ACTIVE_#FRMS register: %d\n", temp));
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RDI_DETECT_CNT);
	D(1, BUG("\tINFO: C3794 RDI_DETECT_CNT_  register: %d\n", temp));
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RDI_NUMFRMS_ACTIVE);
	D(1, BUG("\tINFO: C3794 RDI_ACTIVE_#FRMS register: %d\n", temp));
	temp = IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_NUM_FRAMES_TX);
	D(1, BUG("\tINFO: C3794 FRAMES_TX'D: %d	FRAMES_RX'D: %d \n", temp, IORD_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_NUM_FRAMES_RX)));
}
void clear_C3794_counters()
{
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_LOF_DETECT_CNT, 0);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_LOF_NUMFRMS_ACTIVE, 0);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RDI_DETECT_CNT, 0);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_RDI_NUMFRMS_ACTIVE, 0);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_NUM_FRAMES_RX, 0);
	IOWR_32DIRECT(TOP_C37DOT94_0_BASE, ADDR_NUM_FRAMES_TX, 0);
}

/// TODO
//_________________________________________________________________________________
void reset_C3794_blocks()
{
	C3794_State = 0;	//C37.94 Logic NOT Available - Do NOT R/W or lock-up will occur
	PIO3_Content = IORD_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE);
	PIO3_Content |= RESET_C3794_mask;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content);
}
void gate_C3794_blocks()
{
	PIO3_Content = IORD_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE);
	PIO3_Content &= RESET_C3794_mask;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3IO_BASE, PIO3_Content);
}
// Clock must be written to start 131MHz Clock and allow C37.94 functions
// (C3794_Status&0x80) must be TRUE before any C37.94 logical write.
void write_ppm_offset(signed short ppm_value)
{
	reset_C3794_blocks();							//Ensure C37.94 is off-line

	CLKCTRL_register &= ~CLKCTRL_REG_PPM_MASK;		//clear PPM value
	D(1, BUG("\nC37.94 new ppm value: %d (%0X)\n", ppm_value, ppm_value&CLKCTRL_REG_PPM_MASK));
	ppm_value &= CLKCTRL_REG_PPM_MASK;				// mask off unused bits
	CLKCTRL_register |= ppm_value;					// update Register settings with new ppm value
	//IOWR_32DIRECT(CLKCTRL_BASE, CLKCTRL_REG, CLKCTRL_register);
	IOWR_32DIRECT(CLKCTRL_BASE, CLKCTRL_REG, CLKCTRL_register | CLKCTRL_REG_STROBE_MASK); // strobe to latch in new ppm

	if(wait_for_C3794_clk() != 0){
		gate_C3794_blocks();	// Take C37.94 verilog OUT of reset
		C3794_State = 0x80;	//C37.94 Logic Available
		//_configure/set C37.94 blocks
		D(1, BUG("C37.94 AVAILABLE!\n"));
		}
	else{
		C3794_State = 0;	//C37.94 Logic NOT Available - Do NOT R/W or lock-up will occur
		D(1, BUG("C37.94 NOT AVAILABLE:  Clock Failed to Start!\n"));
		}
}

unsigned char wait_for_C3794_clk()
{
	unsigned int reg_val=0;
	unsigned short i=0;
	reg_val = IORD_32DIRECT(CLKCTRL_BASE, CLKCTRL_REG);
	D(1, BUG("1st CLKCTRL_REG = %0X\n", reg_val));
	while(((reg_val&CLKCTRL_REG_ALIVE_MASK) == 0) && ++i < 55){ 	  // wait for IDT CLK to run: wait up to 550mS
		OSTimeDlyHMSM(0, 0, 0, 10);
		reg_val = IORD_32DIRECT(CLKCTRL_BASE, CLKCTRL_REG);
		D(1, BUG("CLKCTRL_REG = %0X\n", reg_val));
		}

	if(i>50){
		D(1, BUG("C37.94 IDT CLK:  Failed to Start! (%dmS)\n", i*10));
		return(0);
		}
	else{
		D(1, BUG("C37.94 IDT CLK:  Started in %dmS!!\n", i*10));
		return(1);
		}
}


//*********************************************************
/*
void restart_history()
{
unsigned int tmpint;	//was char in prev ver

	SaveBytesLong(ConfigStatC37,C37SECONDS3_ptr,0); // Clear ET Clock

	tmpint = MVAR(int,FOSCR1_ADDR); 			// Fbits: FOSCR 1 & 2
	tmpint = MVAR(int,LCVCR1_ADDR); 			// BPV:   LCVR  1 & 2
	tmpint = MVAR(int,PCVCR1_ADDR);				// CRC:   PCVCR 1 & 2

	SaveBytesLong(ConfigStat,SLIP3_ptr,0);		// Clear SLIPs Counter
	SaveBytesLong(ConfigStatB,SLIP3_ptr,0); 	// Clear SLIPs Counter

	SaveBytesInt(ConfigStat,EXZ_ES1_ptr,0);		// Clear EXZ count
	SaveBytesLong(ConfigStat,FBIT_COUNT3_ptr,0);// Clear Frame Bit Count
	if(!E1_MODE)	SaveBytesInt(ConfigStat,Den1_ES1_ptr,0);	// Clear P-bit Count
	SaveBytesLong(ConfigStat,CRC_COUNT3_ptr,0); // Clear C-bit Count
	SaveBytesLong(ConfigStat,BPV_COUNT3_ptr,0); // Clear BPV Count

	SaveBytesInt(ConfigStat,FBIT_ES1_ptr,0);
	SaveBytesInt(ConfigStat,CRC_ES1_ptr,0);
	SaveBytesInt(ConfigStat,BPV_ES1_ptr,0);

	SaveBytesInt(ConfigStat,FBIT_SES1_ptr,0);
	SaveBytesInt(ConfigStat,CRC_SES1_ptr,0);
	SaveBytesInt(ConfigStat,BPV_SES1_ptr,0);

	LOSCNT = 0;                         		// Clear Alarm Counters and SecCntrs
	OOFCNT = 0;
	AISCNT = 0;
	YELCNT = 0;
	//IDLECNT = 0;
	SaveBytesInt(ConfigStat,LOS_COUNT1_ptr,0);
	SaveBytesInt(ConfigStat,OOF_COUNT1_ptr,0);
	SaveBytesInt(ConfigStat,AIS_COUNT1_ptr,0);
	SaveBytesInt(ConfigStat,YEL_COUNT1_ptr,0);
	//SaveBytesInt(ConfigStat,IDLE_COUNT1_ptr,0);

	SaveBytesInt(ConfigStat,LOS_SEC1_ptr,0);
	SaveBytesInt(ConfigStat,OOF_SEC1_ptr,0);
	SaveBytesInt(ConfigStat,AIS_SEC1_ptr,0);
	SaveBytesInt(ConfigStat,YEL_SEC1_ptr,0);
	//SaveBytesInt(ConfigStat,IDLE_SEC1_ptr,0);

	uas_cntin = 0; uas_cntout = 0;
	SaveBytesInt(ConfigStat,UAS1_ptr,0);
	//SaveBytesLong(ConfigStat,SECONDS3_ptr,0); // Clear ET Clock

	HISTORY_LED = LED_OFF;
	History = NO;
	ERRORS_LED = LED_OFF;
	AUTO_ERROR = NO;			// clear AUTO-TEST latched ERROR flag
	MVAR(char,XIL_DS1_LED)=LED;	// Update LED states
	Misc_stat &= ~0x96;			// Force Clear UAS, HISTORY & ERRORS BIT ..and TimesUp! flag
	ConfigStat[MISC_STAT_ptr] = Misc_stat;	// Send to PDA
}
*/


//*************************************************************************
//* Copyright (c) 2017 Greenlee Communications Vista, CA USA.    		  *
//* All rights reserved.                                                  *
//*************************************************************************
