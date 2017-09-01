/************************************************************************
*		TASK: Main routine									 *
*************************************************************************
* Copyright (c) 2015 Greenlee Communications Vista, CA USA.*
* All rights reserved.                                                   		 *
*************************************************************************/
#include "gigle.h"
#include "vars.h"
#include "loopback.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE		2048
OS_STK    main_task_stk[TASK_STACKSIZE];
OS_STK    link_task_stk[TASK_STACKSIZE];
OS_STK    c3794_task_stk[TASK_STACKSIZE];	//temp for testing....



extern alt_alarm one_sec_alarm;
extern alt_u32 handle_one_sec_alarm();

/* Prints "Hello World" and sleeps for three seconds */
void main_task(void* pdata)
{

	unsigned short tmpdata1 = 0, tmpdata2 = 0;
	unsigned char sfpa_delay_cnt = 0, sfpb_delay_cnt = 0;//, sfp10a_delay_cnt = 0;
	unsigned int one_sec_rate = alt_ticks_per_second();

	reset_C3794_blocks();	// hold C3794 blocks in reset until desired (& clk ready)

	D(1, BUG( "\n<> main_task ALIVE! - USE_PRINT defined (Debug Config)\n"));
	OSTaskSuspend(LINK_TASK_PRIORITY);
	OSTaskSuspend(C3794_TASK_PRIORITY);

	GIGE_STATUS=0; 		// passed into [GIGERESULT_ptr]

	read_flash_parameters();				// read System Parameters from Flash
	load_flashparameters_into_systemvars();	// Load them into the System var's
	//Note that subsequent MAC config'ing will WRITE these recovered MAC Addresses (or FF's/0's if none)
	// Note: if GUI see's that MAC's are 0xFF's or 0x00's then it pop's up dialog to enter sys info (MAC's, options)

	enable_PHY_clock(YES);
    init_10G_external_phy(YES); // yes; means load FW

    D(1, BUG("\t Rx XAUI Align Status  (4.0018)=%04lX\n", read_ext_10G_phy(0x0018, 4)));
    D(1, BUG("\t  Rx XAUI CDR Lock     (4.C000)=%04lX\n", read_ext_10G_phy(0xC000, 4)));
    D(1, BUG("\t Rx XAUI Decode Error (4.C006)=%04lX\n", read_ext_10G_phy(0xC006, 4)));

    EXTERNAL_PHY_BOOT_COMPLETE = YES;

	init_gige();

	//================================================================================
	alt_ic_isr_register(UART_0_IRQ_INTERRUPT_CONTROLLER_ID, UART_0_IRQ, serial_ISR, 0, 0x0);
    UART_CONTROL_REG |= ALTERA_AVALON_UART_CONTROL_RRDY_MSK;			// Enable the serial port receive interrupt
    IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, UART_CONTROL_REG);		// write to CONTROL_REGISTER
    //================================================================================

    alt_ic_isr_register(TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_1_IRQ, timer1_ISR, 0, 0x0);
    TIMER1_CONTROL_REG |= ALTERA_AVALON_TIMER_CONTROL_ITO_MSK;			// set to generate intr on each interval
    TIMER1_CONTROL_REG |= ALTERA_AVALON_TIMER_CONTROL_CONT_MSK;			// set to run continuously
    TIMER1_CONTROL_REG |= ALTERA_AVALON_TIMER_CONTROL_START_MSK;		// start timer!
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE, TIMER1_CONTROL_REG);	// write to Timer control register
    //IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_1_BASE, 0); // clear timer (for next TO event!)

    //========= init_sfp_pio() ======================================================
    // Recast the edge_capture pointer to match the alt_irq_register() function prototype
    void* pio_edge_capture_ptr = (void*) &pio_edge_capture;
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x03); // 3=1G A&B, 4=10G-A, 8=10G-B
    //IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x0F);	// Enable the two SFP 'installed-line' interrupts TSE:0,1  10G:2,3
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x0);	// Reset the edge capture register
    alt_ic_isr_register(PIO_1_IRQ_INTERRUPT_CONTROLLER_ID, PIO_1_IRQ, handle_sfp_ISR, pio_edge_capture_ptr, 0x0);// Register the interrupt handler
    //================================================================================

    //==========================SNIFFER/SWAPPER IRQ ==================================
    void* irq_aggr_capture_ptr = (void*) &irq_aggr_capture;
    IOWR_8DIRECT(IRQ_MASK_BASE, 0, 0x38);
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(IRQ_AGGREGATOR_BASE, 0x38);
    //IOWR_ALTERA_AVALON_PIO_IRQ_MASK(IRQ_MASK_BASE, 0x0);
    alt_ic_isr_register(IRQ_AGGREGATOR_IRQ_INTERRUPT_CONTROLLER_ID, IRQ_AGGREGATOR_IRQ, handle_aggr_ISR, irq_aggr_capture_ptr, 0x0);
    //================================================================================

    configure_TxClkA_speed();
    configure_TxClkB_speed();

    //DEBUG-NEW NEW NEW
    set_10G_clock_source(INT156_CLOCK);	//set system to Intl156.25 for TSE (in case no 10G module, no FPLLOUT)

    configure_tse_mac(TRIPLE_SPEED_ETHERNET_0_BASE);
    configure_tse_mac(TRIPLE_SPEED_ETHERNET_1_BASE);


    //__________________________________________________________________________________________
    //TEMP DEBUG!.....................................................
    /***
    IOWR_16DIRECT(ST_MUX_2_TO_1_0_BASE, ST_MUX_REG, 0x00);		// TSE-A-TX from: 0:EGEN,  1:B-RX
    IOWR_16DIRECT(ST_MUX_3_TO_1_1_BASE, ST_MUX_REG, 0x00);		// TSE-B-TX from: 0:B-RX(B-lpbk), 1:A-Rx, 2:10G-Rx
    IOWR_16DIRECT(TSE_ADDR_SWAPPER_0_BASE, ST_MUX_REG, 0x01);	// TSE-B-TX:      0:NO-SWAP, 1:SWAP  (swap's MAC for mux'd outgoing signal)
    IOWR_16DIRECT(ST_MUX10G_3_TO_1_0_BASE, ST_MUX_REG, 0x00);	//   10G-TX from: 0:TGEN,  1:B-Rx,  2:10G-Rx (10G-lpbk)
    IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_0_BASE, ST_SPLITTER_REG, 0x00);	//  10G-Rx to: 0:TMON, 1:B-Tx  -- Select which RDY to pass to 10G-Rx (rather than AND of all the output RDY-ins's)
    IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_SPLITTER_REG, 0x00);	//TSE-A-RX to: 0:EMON, 1:B-Tx  -- Select which RDY to pass (rather than AND of all the output RDY-ins's)
    IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_2_BASE, ST_SPLITTER_REG, 0x01);	//TSE-B-RX to: 0:A-Tx,1:B-Tx, 2:10G-Tx  -- Select which RDY to pass (rather than AND of all the output RDY-ins's)
	****/
    //ConfigStat[CORE_SIGROUTE_MODES_ptr]  = 0;	//(set in init) TSE A&B and 10G all in NORMAL operating MODE
    SET_TSE_A_NORMAL_TEST_MODE;
    SET_TSE_B_LOOPBACK;						//TSE_B_LOOPBACK = YES;
    //SET_SWAP_B_TOGGLE|=0x03; //swap IP,MAC
    SET_TSE_B_MAC_ADDR_SWAP(SET_SWAP_B_TOGGLE);	//TSE_B_SWAP_ADDR = YES;
    SET_10G_NORMAL_TEST_MODE;

    //new...GEN & MON splitter/mux between 10G port and TSE port
    //IOWR_16DIRECT(GUISYS_AVALON_ST_SPLITTER_3_BASE, ST_SPLITTER_REG, SET_PORT_0);
    //IOWR_16DIRECT(ST_MUX10G_2_TO_1_ERROR6_0_BASE, ST_MUX_REG, SET_PORT_0);
    //SET_TRAFFIC_TO_10G;
    //commented 10g and changed to tse
    SET_TRAFFIC_TO_TSE;
    //SET_10GLOOPPATH_CLOCKMUX_TO_OSC;	// default: 10G clocked by Local OSC

   /* D(1, BUG("\n_____________MUX_STATES_____________\n"));
    D(1, BUG("A:ST_MUX_2_TO_1     STATE: 0x%04X\n", IORD_16DIRECT(ST_MUX_2_TO_1_0_BASE, ST_MUX_REG)));
    D(1, BUG("A:2-port SPLITTER   STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_MUX_REG)));
    D(1, BUG("B:ST_MUX_3_TO_1     STATE: 0x%04X\n", IORD_16DIRECT(ST_MUX_3_TO_1_1_BASE, ST_MUX_REG)));
    D(1, BUG("B:ADDR_SWAPPER      STATE: 0x%04X\n", IORD_16DIRECT(TSE_ADDR_SWAPPER_0_BASE, TSE_SWAPPER_REG)));
    D(1, BUG("B:3-port SPLITTER   STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_1_BASE, ST_MUX_REG)));
    D(1, BUG("10G:ST_MUX_3_TO_1   STATE: 0x%04X\n", IORD_16DIRECT(ST_MUX10G_3_TO_1_0_BASE, ST_MUX_REG)));
    D(1, BUG("10G:3-port SPLITTER STATE: 0x%04X\n", IORD_16DIRECT(GUISYS_AVALON_ST_SPLITTER_0_BASE, ST_MUX_REG)));
    D(1, BUG("____________________________________\n"));
    //__________________________________________________________________________________________

*/
    configure_tse0_to_phya();	// run this after configuring the MAC (reset clears MDIO setting)
    configure_tse1_to_phyb();	// run this after configuring the MAC (reset clears MDIO setting)

    if(PHYA_ENABLE == 1){
    	configure_phya_fiber();
    	configure_phya_copper();
    	select_copper_operation(TRIPLE_SPEED_ETHERNET_0_BASE);
    	}
    if(PHYA_LINESIDE_LOOPBACK){
    	PHY_LINESIDE_LOOPBACK_ON(TRIPLE_SPEED_ETHERNET_0_BASE);
    	D(1, BUG("NOTE: PHY-A in Lineside (NW) Loopback!\n"));
    	}

    if(PHYB_ENABLE == 1){
    	configure_phyb_fiber();
    	configure_phyb_copper();
    	select_copper_operation(TRIPLE_SPEED_ETHERNET_1_BASE);
    	}
    if(PHYB_LINESIDE_LOOPBACK){
    	PHY_LINESIDE_LOOPBACK_ON(TRIPLE_SPEED_ETHERNET_1_BASE);
    	D(1, BUG("NOTE: PHY-B in Lineside (NW) Loopback!\n"));
    	}
    else		//  if no PHYB lineside loopback, then....
    	D(1, BUG("\nNOTE: ETHERNET Port-B in ST Loopback toward the NW (from behind the MAC - TSE1)!!\n\n"));
    //=============================================================================
    // note if any SFP's are installed and collect data if they are!
    // ...pass SFP state to GUI
    unsigned int pio_sfp_state = IORD_ALTERA_AVALON_PIO_DATA(PIO_1_BASE);	// snag state of sfp detect pins
    ConfigStat[SFP_STAT_ptr] &= SFP_STATE_MASK;	// SFP changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
    ConfigStat[SFP_STAT_ptr] |= (pio_sfp_state&0x03)<<1; // set SFP A&B in/out states
    ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr]; // cc to 10G status
    ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
    if((pio_sfp_state&0x01) == 0){	// SFP Detect Low means SFP in 1gig PortA is installed!
        D(1, BUG("An SFP is Installed in 1gig PORT-A!\n"));
        READ_SFPA_DATA = YES;	// read and upload SFPinfo to GUI
    	}
    if((pio_sfp_state&0x02) == 0){	// SFP Detect Low means SFP in 1gig PortB is installed!
        D(1, BUG("An SFP is Installed in 1gig PORT-B!\n"));
        READ_SFPB_DATA = YES;	// read and upload SFPinfo to GUI
    	}
    //=============================================================================
    ConfigStat[SFP_STAT_ptr] &= SFP10G_STATE_MASK; // init: assume no SFP+ installed
    ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr]; // cc to 10G status
    ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
    if(EXTERNAL_PHY_BOOT_COMPLETE){
	     D(1, BUG("Checking for installed SFP+ module in 10G Port-A...\n"));
		if(read_ext_10G_phy(0xD7FD, 3) == 0x30 || read_ext_10G_phy(0xD70C, 3) == 7){ // if UC_status= NO module OR it's an unsuported Module detected
			//added a boolean check for sfp
			SFP_10G_PRESENT = NO;
			//SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD;
			D(1, BUG("\tNO SFP+ INSTALLED in 10G Port-A,  UC_STATUS=%04lX  (module type: %ld) 3.D703=%04lX \n", read_ext_10G_phy(0xD7FD, 3), read_ext_10G_phy(0xD70C, 3), read_ext_10G_phy(0xD703,3)));
			write_ext_10G_phy(0xF052, 1, 0x0022);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX PWDN!
			write_ext_10G_phy(0xC30A, 1, 0x007B);   // RE-Enable FPLLOUT^1
			D(1, BUG("\tPATCH Reg (1.C30A)=%04lX  (1.F052)=%04lX\n",read_ext_10G_phy(0xC30A, 1), read_ext_10G_phy(0xF052, 1)));
			// SFP+ changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
			ConfigStat[SFP_STAT_ptr] |= 0x08;   // set SFP+ A out states
			//ConfigStat[SFP_STAT_ptr] ^= 0x01;	// toggle bit 1 to indicate that something has changed with the SFP's
			ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
			ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
			}
		else{
			//===Read 10G PHY status register to see what type of Module has been inserted!
			D(1, BUG("\tAn SFP+ is INSTALLED in 10G Port-A,  UC_STATUS=%04lX  (module type: %ld) \n", read_ext_10G_phy(0xD7FD, 3), read_ext_10G_phy(0xD70C, 3)));

			SFP_10G_PRESENT = YES;
			//SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;
			write_ext_10G_phy(0xF052, 1, 0x0020);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX & RFX PWDN!
			D(1, BUG("\tPATCH  (1.F052)=%04lX\n", read_ext_10G_phy(0xF052, 1)));
			config_10G_external_phy(); // reconfigure PHY CLKS when Module inserted!

			usleep(10);
			set_reset_10GMAC_FIFOS();
			SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;
			D(1, BUG("SWITCHING TO TXFPLL\n"));
			usleep(1);
			clear_reset_10GMAC_FIFOS();
			init_10G_MAC();
			init_10G_SC_FIFO(ETH_10G_SCFIFO_CUT_THRU);
			int reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
			reg_val |= 0x01;  // Activate Tx Digital Reset
			IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT, reg_val);
			init_10G_xaui(); // no need when in auto-CDR lock it's back everytime!

			READ_SFP10A_DATA = YES;	// Read SFP+ info into array
			// SFP+ changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
			// STAT left at 0 to indicate set SFP+ A is IN state
			//ConfigStat[SFP_STAT_ptr] ^= 0x01;	// toggle bit 1 to indicate that something has changed with the SFP's
			ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
			ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
			}
    	}//Ext PHY has completed BOOT

	//==============================================================================
	//                 initialize new GEN registers
	//==============================================================================
      SET_GEN_MON_TO_STREAM(0);  // Point to selected Stream (stream[0] is 1st stream)
 	//==============================================================================

	//============================ Loopback Initialization =========================

	DETECT_REMOTE_LOOPBACK_TSEA = 1;
	DETECT_REMOTE_LOOPBACK_TSEB = 1;
	DETECT_REMOTE_LOOPBACK_10GA = 1;

	// Clear
	IOWR_16DIRECT(SNIFFER_TSE_A_BASE, 0x10, 0xFFFF); //TSE-A
	IOWR_16DIRECT(SNIFFER_TSE_B_BASE, 0x10, 0xFFFF); //TSE-B
	IOWR_16DIRECT(SNIFFER64_10GE_BASE, 0x10, 0xFFFF); //10G-A

	//set_10G_clock_source(FPLLOUT_CLOCK);
	//==============================================================================


	SaveBytesLong(ConfigStat,SECONDS3_ptr,0x05); 	// init: TSE clock to show 5secs
	SaveBytesLong(ConfigStatB,SECONDS3_ptr,0x03);
	SaveBytesLong(ConfigStat10G,SECONDS3_ptr,0x06);	// 10secs on 10G
	OSTaskResume(LINK_TASK_PRIORITY);

	XAUI_SERIAL_SELFLPBK(OFF);				// this is OFF after reset
	loopback_xgxs_sys_10G_external_phy(OFF);	// this is OFF after reset
	alt_alarm_start(&one_sec_alarm, one_sec_rate, handle_one_sec_alarm, 0);
	alt_alarm_stop(&one_sec_alarm);
	//tmpdata1 = read_ext_10G_phy(0xC001, 1);
	//write_ext_10G_phy(0xC001, 1, tmpdata1 | 0x0080);	// use SREFCLK as input
	//D(1, BUG("BOOOP:Checking PMA Vendor Specific Reg (1.C001)=%04lX\n", read_ext_10G_phy(0xC001, 1)));

			//IOWR_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0xC0, 0x0); // One packet stuck in queue?
		//	D(1, BUG("\nmain: Command Register (A): %X\n", IORD_32DIRECT(TSE_ADDR_SWAPPER_A_BASE, 0xC0)));
		//D(1, BUG("\nISR: %X\n", IORD_8DIRECT(SNIFFER_TSE_A_BASE, 0)));
		//SET_TSE_A_MAC_ADDR_SWAP(0);

	//	D(1, BUG("LOOPBACK_PTR: %X\n\n", ConfigStat[LOOPBACK_ptr]));

	///TODO ___________________________________________________________________________
	init_c37dot94();
	OSTaskResume(C3794_TASK_PRIORITY);

	///TODO ___________________________________________________________________________
	//	unsigned short old_clk = 5342;
	int i=0;
	while (1){ // If something changes
		//FTX_PLL_LCK at 1.C001:0
//		D(1, BUG("Checking PMA Vendor Specific Reg (1.C001)=%04lX\n", read_ext_10G_phy(0xC001, 1)));
		OSTimeDlyHMSM(0, 0, 0, 100);


		// CLOCK HANDLER
		// change only on a state change so function isn't called everytime
		if (GIGE10) {
			if (SFP_10G_PRESENT) {
				set_10G_clock_source(FPLLOUT_CLOCK);
//				D(1, BUG("MAIN: GO TO FPLLOUT\n"));
			}
			else {
				set_10G_clock_source(INT156_CLOCK);
//				D(1, BUG("MAIN: GO TO INT156\n"));
			}
		}
		else if (GIGE_A) {
			set_10G_clock_source(INT156_CLOCK);
		}

		//_____________________________
		loopback_event_processing();


		//================================================================
		if (gCUSTOM_TEST_START) {
			gCUSTOM_TEST_START = NO;
			unsigned char i = 0;
			for(i = 0; i < 16; i++) // clear everything before starting any test for all streams
			{
				SET_GEN_MON_TO_STREAM(i);
				CLR_GEN_MON_HDRS_BY_STREAM();
				CLR_MON_MASK_BY_STREAM(TRAFFIC_MON_MSK1, TRAFFIC_MON_MSK62);
				DISABLE_MON_MASK();
				SETGEN_STREAM_NUMPKTS(0, 0);
			}
			if(MS_TEST_ACTIVE){
				if (GIGE10)
					num_of_streams = ((unsigned char) ConfigStat10G[TRAFFIC_NUM_OF_STREAMS] & 0x0F) + 1;
				else
					num_of_streams = ((unsigned char) ConfigStat[TRAFFIC_NUM_OF_STREAMS] & 0x0F) + 1;
				D(1, BUG("\n<>num_of_streams %d\n", num_of_streams));
				for(i = 0; i < num_of_streams; i++)
				{
					update_headers(i);
					CONFIGURE_TEST_STREAMS(i);
				}
				SETMON_TOTALNUMPKTS(number_packet_lsb, number_packet_msb);	  // set MON num pkts to expect  MUST BE SUM of ALL PKTS from ALL streams...
				D(1, BUG("\n<>RUN PACKET TEST\n"));
				run_packet_test();
				MS_TEST_ACTIVE=NO; 		   // remove this after initial link up test if you find a better place
				MS_PKT_RCVD = YES;	  // MS test was run once so send out the big packet on link up
				D(1, BUG("\n<>MS_TEST_ACTIVE = NO & stream_offset set back to quick test\n"));
			}
			else {	// Single Stream QUICK TEST!
				num_of_streams = 1;//making sure its 1
				if (GIGE10)
					stream_no = (unsigned char) (ConfigStat10G[TRAFFIC_NUM_OF_STREAMS] >> 4) & 0xF; // get the exact stream
				else
					stream_no = (unsigned char) (ConfigStat[TRAFFIC_NUM_OF_STREAMS] >> 4) & 0xF; // get the exact stream

				D(1, BUG("\n<>SS TEST on stream %d\n", stream_no));
				update_headers(0);
				CONFIGURE_TEST_STREAMS(stream_no);
				SETMON_TOTALNUMPKTS(number_packet_lsb, number_packet_msb);	  // set MON num pkts to expect  MUST BE SUM of ALL PKTS from ALL streams...
				run_packet_test();
			}
			stream_offset = 99;//set back the stream_offset to SS
			ConfigChecksum2 = (MaxConfig2-1);
			update_headers(0);
		}
		//================================================================
		// SFP intr see's SFP just installed so...read it and pass it up!
		//================================================================
		if (READ_SFPA_DATA){
			if(sfpa_delay_cnt++ == 4) {	// delay 4 cntrs (400mS) before reading so sfp can boot & initialize
				D(1, BUG("!READ_SFPA_DATA main\t"));
				D(1, BUG("SFP_STAT: %x\n", ConfigStat[SFP_STAT_ptr]));
				read_sfp_info(PortA);	// read names (for now) of sfp to confirm sfp driver operation
				READ_SFPA_DATA = NO;	// don't read again, until requested.
				SEND_SFPA_DATA = YES;	// flag gui_link to send SFP packet
				sfpa_delay_cnt = 0;
			}
		}
		if (READ_SFPB_DATA) {
			if(sfpb_delay_cnt++ == 4) {	// delay 4 cntrs (400mS) before reading so sfp can boot & initialize
				read_sfp_info(PortB);	// read names (for now) of sfp to confirm sfp driver operation
				READ_SFPB_DATA = NO;	// don't read again, until requested.
				SEND_SFPB_DATA = YES;	// flag gui_link to send SFP packet
				sfpb_delay_cnt = 0;
			}
		}


		if (GIGE10) {
			unsigned short mismatch10G = IORD_16DIRECT(SWAPPER64_10G_BASE,MISMATCH_DETECTOR_10G);
			if((mismatch10G & 0x01) == 0x01) // there is a mismatch detected
			{
				ConfigStat10G[MISC_ptr]|=0x8;
				D(1, BUG("mismatch10G!\n %08X",mismatch10G));
				IOWR_16DIRECT(SWAPPER64_10G_BASE,MISMATCH_DETECTOR_10G,0x01); //ack the bit set
			}
			else
			{
				ConfigStat10G[MISC_ptr]&=~0x8;
			}
		}
		else {
			unsigned short mismatchA = IORD_16DIRECT(TSE_ADDR_SWAPPER_A_BASE,TSE_A_MISMATCH_DETECTOR);
			unsigned short mismatchB = IORD_16DIRECT(TSE_ADDR_SWAPPER_B_BASE,TSE_B_MISMATCH_DETECTOR);

			if((mismatchA & 0x01) == 0x01) // there is a mismatch detected
			{
				D(1, BUG("mismatchA\n %08X", mismatchA));
				ConfigStat[MISC_ptr]|=0x8;
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_A_BASE,TSE_A_MISMATCH_DETECTOR,0x01); //ack the bit set
			}
			else if((mismatchB & 0x01) == 0x01) // there is a mismatch detected
			{
				D(1, BUG("mismatchB\n %08X", mismatchB));
				IOWR_16DIRECT(TSE_ADDR_SWAPPER_B_BASE,TSE_B_MISMATCH_DETECTOR,0x01);//ack the bit set
				ConfigStatB[MISC_ptr]|=0x8;
			}
			else {
				ConfigStatB[MISC_ptr]&=~0x8;
				ConfigStat[MISC_ptr]&=~0x8;
		
				}
			}

		//=======================================================================================================
		if (EXTERNAL_PHY_BOOT_COMPLETE){
			tmpdata1 = read_ext_10G_phy(0x9002, 1);	// bit^7=LASI_TST_EN    1=module Event ...LASI TST DATA Enable
			tmpdata2 = read_ext_10G_phy(0x9005, 1);	// bit^7=LASI_TST_DATA  0=module Event ...LASI TST DATA Enable
			if ((tmpdata1&0x80) && (tmpdata2&0x80)==0) {
				D(1, BUG("See 10G Module (LASI_TST_EN) Event!\t"));
				tmpdata1 &= ~0x80;	// clear LASI-TST_EN bit
				write_ext_10G_phy(0x9002, 1, tmpdata1);

				//tmpdata2 = read_ext_10G_phy(0x9005, 1);	// bit^7=LASI_TST_DATA  0=module Event ...LASI TST DATA Enable
				//D(1, BUG("LASI TST DATA = %0lX\n",read_ext_10G_phy(0x9005, 1)));
				tmpdata1 = read_ext_10G_phy(0xD7FD, 3);	// check UC_status - when all good it's not 0x10 or 0!
				tmpdata2 = 0;
				//while(++tmpdata2 < 25 && (tmpdata1 < 0x50)){  // could take 2seconds! - give him 2.5 seconds!
				while (++tmpdata2 < 25 && (tmpdata1 == 0x10 || tmpdata1 == 0)) {  // could take 2seconds! - give him 2.5 seconds!
					OSTimeDlyHMSM(0, 0, 0, 100);  //wait 100mS for updated data!
					tmpdata1 = read_ext_10G_phy(0xD7FD, 3);
					}
				if (tmpdata2 < 25) {
					//(1, BUG("UC_STATUS Complete!!  ...it took %3.2fsecs, (3.D7FD) Status=%04X\n",((float) tmpdata2/10.0),tmpdata1));
					tmpdata2 = read_ext_10G_phy(0xD70C, 3); // read Module type detected
					if (tmpdata1 == 0x30 || tmpdata2 == 7) { // if UC_status = NO module OR it's an unsuported Module
						D(1, BUG("SFP+ REMOVED!  10G Port-A,  UC_STATUS=%04X  (module type: %d)  [1.C30A:%04lX]\n",tmpdata1, tmpdata2,read_ext_10G_phy(0xC30A, 1)));
						SFP_10G_PRESENT = NO;

						//TEMP....PATCH...to fix loss of 156.25M clk when SFP+ module removed!
						write_ext_10G_phy(0xF052, 1, 0x0022);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX PWDN!
						write_ext_10G_phy(0xC30A, 1, 0x007B);   // RE-Enable FPLLOUT^1
						D(1, BUG("\tPATCH Reg (1.C30A)=%04lX  (1.F052)=%04lX\n",read_ext_10G_phy(0xC30A, 1), read_ext_10G_phy(0xF052, 1)));
						ConfigStat[SFP_STAT_ptr] &= SFP10G_STATE_MASK;	// SFP+ changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
						ConfigStat[SFP_STAT_ptr] |= 0x08;   // set SFP+ A out states
						ConfigStat[SFP_STAT_ptr] ^= 0x01;	// toggle bit 1 to indicate that something has changed with the SFP's
						ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
						ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
					}
					else {
						D(1, BUG("enterTime: %lu\n", OSTimeGet()));
						SFP_10G_PRESENT = YES;

						//===Read 10G PHY status register to see what type of Module has been inserted!
						D(1, BUG("SFP+ INSERTED!  10G Port-A,  UC_STATUS=%04X,  PHY SelfConfigured to module type: %d\n",tmpdata1, tmpdata2));
						write_ext_10G_phy(0xF052, 1, 0x0020);		// FTX/FRX_PWDN^21 getting enabled when module removed (ie goes to 0x0026) So disable FTX & RFX PWDN!
						D(1, BUG("\tPATCH  (1.F052)=%04lX\n", read_ext_10G_phy(0xF052, 1)));
						config_10G_external_phy(); // reconfigure PHY CLKS when Module inserted!

						usleep(10);
						set_reset_10GMAC_FIFOS();
						if (((ConfigStat10G[CORE_SIGROUTE_MODES_ptr])&0x20) == 0x20) {
							SET_10GLOOPPATH_CLOCKMUX_TO_RCVRD;
							D(1, BUG("SWITCHING TO RCVRD\n"));
						}
						else {
							SET_10GLOOPPATH_CLOCKMUX_TO_TXFPLL;
							D(1, BUG("SWITCHING TO TXFPLL\n"));
						}
						usleep(1);
						clear_reset_10GMAC_FIFOS();
						init_10G_MAC();
						init_10G_SC_FIFO(ETH_10G_SCFIFO_CUT_THRU);
						int reg_val = IORD_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT);
						reg_val |= 0x01;  // Activate Tx Digital Reset
						IOWR_32DIRECT(ETH_10G_DESIGN_EXAMPLE_0_BASE|ETH_10G_XAUI_BASE_ADDR, XAUI_RESET_AUTO_CONTROLSTAT, reg_val);
						init_10G_xaui(); // no need when in auto-CDR lock it's back everytime!

						READ_SFP10A_DATA = YES;	// Read SFP+ info into array
						ConfigStat[SFP_STAT_ptr] &= SFP10G_STATE_MASK;	// SFP+ changeEvent^0 SFP-A:out^1, SFP-B:out^2__10GSFP-A: out^3, 10GSFP-B: out^4
						// leave at 0 to indicate set SFP+ A in state
						ConfigStat[SFP_STAT_ptr] ^= 0x01;	// toggle bit 1 to indicate that something has changed with the SFP's
						ConfigStat10G[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];
						ConfigStatB[SFP_STAT_ptr] = ConfigStat[SFP_STAT_ptr];

						if (((ConfigStat10G[CORE_SIGROUTE_MODES_ptr]) & 0x20) == 0x20) {	// If loopback was turned on then enable it now
							LOOPBACK_EVENT = LOOPUP10G;
							if ((ConfigStat10G[CORE_SIGROUTE_MODES_ptr] & 0x80) == 0x80)	// Check if swap IP was on or not
								ENTER_LOOPBACK(YES);
							else
								ENTER_LOOPBACK(NO);
						}

						D(1, BUG("\t Rx XAUI Align Status  (4.0018)=%04lX\n", read_ext_10G_phy(0x0018, 4)));
						D(1, BUG("\t  Rx XAUI CDR Lock     (4.C000)=%04lX\n", read_ext_10G_phy(0xC000, 4)));
						D(1, BUG("\t Rx XAUI Decode Error (4.C006)=%04lX\n", read_ext_10G_phy(0xC006, 4)));
						D(1, BUG("exitTime: %lu\n", OSTimeGet()));
					}
				}
				else {
					D(1, BUG("TIMEOUT!!! ...PHY uP init incomplete (3.D7FD), Status=%04X\n",tmpdata2));
				}
			}
		} //End External PHY has completed the boot sequence
		//=======================================================================================================
		if (READ_SFP10A_DATA) {
			read_10G_sfp_info(PortA);	// read names (for now) of sfp to confirm sfp driver operation
			READ_SFP10A_DATA = NO;	// don't read again, until requested.
			SEND_SFP10A_DATA = YES;	// flag gui_link to send SFP+ packet
		}
		// READ_SFP10B_DATA  SEND_SFP10B_DATA


	}//End: infinite while

	UART_CONTROL_REG = 0;							// Disable All Serial Port Interrupts
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE,UART_CONTROL_REG);
	//alt_irq_register(UART_0_IRQ,0,0);				// un-Register the Serial Port ISR
    alt_ic_isr_register(UART_0_IRQ_INTERRUPT_CONTROLLER_ID,UART_0_IRQ,0,0,0);
}

//_________________________________________________________________________________________________

void CONFIGURE_TEST_STREAMS(unsigned int stream)
{
		SET_GEN_MON_TO_STREAM(stream);
		SET_TEST_PARAMS();
		unsigned short header_length;
		if(MS_TEST_ACTIVE ){
			 header_length=(unsigned short) ConfigMS[TRAFFIC_GEN_CONFIG02_ptr];	//128 //	//gen_config
		}
		else {
			if (GIGE10)
				header_length =(unsigned short) ConfigStat10G[TRAFFIC_GEN_CONFIG02_ptr];	//128 //	//gen_config
			else
				header_length =(unsigned short) ConfigStat[TRAFFIC_GEN_CONFIG02_ptr];	//128 //	//gen_config
		}
		SET_GEN_MON_TEST_BY_STREAM(stream, traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate,header_length);
		total_num_packets += set_num_test_packets_by_secs(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);//egen_line_load_rate
		number_packet_lsb = total_num_packets & 0x00FFFFFFFF; 	              // update global pkt_count (usually from UI) to our test number
		number_packet_msb = (total_num_packets - number_packet_lsb)>>32;
		number_packet_msb &= 0x00FF;
}


void SET_TEST_PARAMS()
{
	if(MS_TEST_ACTIVE){
			emix = ConfigMS[TRAFFIC_GEN_CONFIG03_ptr]&0X01;
			rndPayload =ConfigMS[TRAFFIC_GEN_CONFIG03_ptr]&0X02;
			vlanTags = ConfigMS[EGEN_PKT_TYPE_ptr_1]&0x0F;
			D(1, BUG("\tNo. of VLAN tags in EMIX %d\n" ,vlanTags));
			mplsTags = (ConfigMS[EGEN_PKT_TYPE_ptr_1]>>4)&0xF;
			D(1, BUG("\tNo. of MPLS tags in EMIX %d\n" ,mplsTags));}
	else {
		if(GIGE10) { //10G
			emix = ConfigStat10G[TRAFFIC_GEN_CONFIG03_ptr]&0X01;
			rndPayload =ConfigStat10G[TRAFFIC_GEN_CONFIG03_ptr]&0X02;
			vlanTags = ConfigStat10G[EGEN_PKT_TYPE_ptr_1]&0x0F;
			mplsTags = (ConfigStat10G[EGEN_PKT_TYPE_ptr_1]>>4)&0xF;
		}
		else {//TSE
			emix = ConfigStat[TRAFFIC_GEN_CONFIG03_ptr]&0X01;
			rndPayload =ConfigStat[TRAFFIC_GEN_CONFIG03_ptr]&0X02;
			vlanTags = ConfigStat[EGEN_PKT_TYPE_ptr_1]&0x0F;
			mplsTags = (ConfigStat[EGEN_PKT_TYPE_ptr_1]>>4)&0xF;
		}
	}

	SET_GEN_PACKET_LENGTH();
	SET_LINE_LOAD_RATE();
}

//EGEN: A fixed length ranges between 24 to 9,600 bytes; a random length ranges between 24 and 1518
// 		we're default on fixed length packets
void SET_GEN_PACKET_LENGTH()
{

	if (emix==0x01) { //if emix
		GET_EMIX_AVG_PKTLEN();
		SET_GEN_EMIX_BY_STREAM();
	}
	else {
		if(MS_TEST_ACTIVE)
		{
			traffic_gen_pkt_length = BytesToInt(ConfigMS,  TRAFFIC_GEN_CONFIG01_ptr);
			SaveBytesInt(ConfigMS,TRAFFIC_GEN_CONFIG01_ptr,traffic_gen_pkt_length);
			D(1, BUG("...Setting MS PKTLENGTH_BY_STREAM(traffic_gen_pkt_length) %0d \n",traffic_gen_pkt_length));
		}
		else //tse pkt
		{
			if(!GIGE10){
				traffic_gen_pkt_length = BytesToInt(ConfigStat, TRAFFIC_GEN_CONFIG01_ptr);
			}
			else{
				traffic_gen_pkt_length = BytesToInt(ConfigStat10G, TRAFFIC_GEN_CONFIG01_ptr);
			}

			SaveBytesInt(ConfigStat,TRAFFIC_GEN_CONFIG01_ptr,traffic_gen_pkt_length);
			SaveBytesInt(ConfigStat10G,TRAFFIC_GEN_CONFIG01_ptr, traffic_gen_pkt_length);
			D(1, BUG("...Setting TSE PKTLENGTH_BY_STREAM(traffic_gen_pkt_length) %0d \n",traffic_gen_pkt_length));
		}
	}

}

void SET_LINE_LOAD_RATE()
{
	float output =100.0;//initial val
	burst_packets = 0;
	if(MS_TEST_ACTIVE){
	//	D(1, BUG("!!!!!!*****...Setting load from *********!!!!!!!\n"));
		*((unsigned char*)(&output) + 3) = ConfigMS[TRAFFIC_RATE3_ptr];
		*((unsigned char*)(&output) + 2) = ConfigMS[TRAFFIC_RATE2_ptr];
		*((unsigned char*)(&output) + 1) = ConfigMS[TRAFFIC_RATE1_ptr];
		*((unsigned char*)(&output) + 0) = ConfigMS[TRAFFIC_RATE0_ptr];
		egen_line_load_rate = output;

	//	SaveBytesLong(ConfigMS,TRAFFIC_RATE3_ptr,egen_line_load_rate);
		D(1, BUG("...Setting MS NUM PKTs and LOAD to %f at %d\n",egen_line_load_rate,traffic_gen_pkt_length));
		SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
		SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);	// called on Frame Size or FPS change
	}
	else {
		if(!GIGE10) {
		*((unsigned char*)(&output) + 3) = ConfigStat[TRAFFIC_RATE3_ptr];
		*((unsigned char*)(&output) + 2) = ConfigStat[TRAFFIC_RATE2_ptr];
		*((unsigned char*)(&output) + 1) = ConfigStat[TRAFFIC_RATE1_ptr];
		*((unsigned char*)(&output) + 0) = ConfigStat[TRAFFIC_RATE0_ptr];
		burst_packets = BytesToLong(ConfigStat,TRAFFIC_GEN_NUM_BURST_PACKETS03_ptr);}
		else{
			*((unsigned char*)(&output) + 3) = ConfigStat10G[TRAFFIC_RATE3_ptr];
					*((unsigned char*)(&output) + 2) = ConfigStat10G[TRAFFIC_RATE2_ptr];
					*((unsigned char*)(&output) + 1) = ConfigStat10G[TRAFFIC_RATE1_ptr];
					*((unsigned char*)(&output) + 0) = ConfigStat10G[TRAFFIC_RATE0_ptr];
					burst_packets = BytesToLong(ConfigStat10G,TRAFFIC_GEN_NUM_BURST_PACKETS03_ptr);
		}
		egen_line_load_rate = output;

		SaveBytesLong(ConfigStat,TRAFFIC_RATE3_ptr,egen_line_load_rate);
		SaveBytesLong(ConfigStatB,TRAFFIC_RATE3_ptr,egen_line_load_rate);
		SaveBytesLong(ConfigStat10G,TRAFFIC_RATE3_ptr,egen_line_load_rate);


		if(burst_packets==0){//if NOT burst test - no burst in msConfig
			D(1, BUG("...Setting NUM PKTs and LOAD to %f at %d\n",egen_line_load_rate,traffic_gen_pkt_length));
			SET_NUMPKTS_BY_STREAM(traffic_gen_pkt_length, gen_number_seconds, egen_line_load_rate);		// called on Frame Size, TestSecs, FPS change or SPEED change
			SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);	// called on Frame Size or FPS change
		}
		else {//set fps if it is a burst test
			D(1, BUG("...Setting BURST BURST LOAD to %f at %d\n",egen_line_load_rate,traffic_gen_pkt_length));
			SET_FPSLOAD_BY_STREAM(traffic_gen_pkt_length, egen_line_load_rate);	// called on Frame Size or FPS change
		}
	}
}

//SET AVG PACKETS TO BE GENERATED in EMIX
void GET_EMIX_AVG_PKTLEN()
{
		unsigned short emixByteVal = TRAFFIC_GEN_EMIXSEQ33_ptr;
		unsigned short val;
		double emixAvg = 0;
		double emixSum = 0;
		unsigned char i;
		unsigned char j =0;
		unsigned short totalFrames = 0;//The number of frames configured in EMIX to calculate the avg.
		unsigned short usersize_G = 0;
		unsigned short usersize_U = 0;
		unsigned short mtu = 0;
		unsigned short w = 0;
		if(MS_TEST_ACTIVE){
			usersize_G = BytesToInt(ConfigMS,TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr);
			usersize_U = BytesToInt(ConfigMS,TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr);
			mtu = BytesToInt(ConfigMS, MAC_FRAME_LEN1_ptr);
			w = BytesToInt(ConfigMS,TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr);
			//EGEN_PKT_TYPE_ptr_1 // 0000,0000:  ^7654:MPLS, ^3210:VLAN 	(0=none, N=# of VLAN's or MPLS tags)
		}
		else{
			if(GIGE10) {
				usersize_G = BytesToInt(ConfigStat10G,TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr);
				usersize_U = BytesToInt(ConfigStat10G,TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr);
				mtu = BytesToInt(ConfigStat10G, MAC_FRAME_LEN1_ptr);
				w = BytesToInt(ConfigStat10G,TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr);
			}
			else {
				usersize_G = BytesToInt(ConfigStat,TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr);
				usersize_U = BytesToInt(ConfigStat,TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr);
				mtu = BytesToInt(ConfigStat, MAC_FRAME_LEN1_ptr);
				w = BytesToInt(ConfigStat,TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr);
			}
		}

		for(i = 1; i < 33; i++) //there are 4 words of emix
		{
				if(MS_TEST_ACTIVE) {
					val = (ConfigMS[emixByteVal] >> j) & 0x0F;
				}
				else {
					if(GIGE10)
						val = (ConfigStat10G[emixByteVal] >> j) & 0x0F;
					else
						val = (ConfigStat[emixByteVal] >> j) & 0x0F;
				}

				if(val != 0xF) { //if not terminate flag
					switch(val)
					{
						case 0:emixSum+= 64;totalFrames++; break;
						case 1:emixSum+= 128;totalFrames++;break;
						case 2:emixSum+= 256;totalFrames++;break;
						case 3:emixSum+= 512;totalFrames++;break;
						case 4:emixSum+= 1024;totalFrames++;break;
						case 5:emixSum+= 1280;totalFrames++;break;
						case 6:emixSum+= usersize_G;totalFrames++;break; // user defined
						case 7:emixSum+= mtu;totalFrames++;break;//mtu
						case 8:emixSum+= usersize_U;totalFrames++;break; //user defined
						case 9:emixSum+= w;totalFrames++;break;
					}
				}

				if(j == 0)
					j = 4;
				else {
					j = 0;
					emixByteVal++;
				}// value to shift the bits
				//D(1, BUG("...[emixByteVal] after switch EMIX: %d \n",emixByteVal));
		}

		//D(1, BUG("\tNo. of MPLS tags in EMIX %d\n" ,mplsTags));
		if(mplsTags == 0)
			mplsTags = 1;

		if(vlanTags || mplsTags > 1)
			emixSum = emixSum +(4 * totalFrames * (vlanTags + (mplsTags - 1)));
		else
			emixSum = emixSum + (4 * totalFrames);

		emixAvg = emixSum / totalFrames;
		DEF_EMIX_RANDOM = emixAvg;
		D(1, BUG("usersize_G: %d\n", usersize_G));
		D(1, BUG("usersize_U: %d\n", usersize_U));
//		D(1, BUG("\tNo. of VLAN tags in EMIX %d\n" ,vlanTags));
//		D(1, BUG("\tNo. of MPLS tags in EMIX %d\n" ,mplsTags));
		D(1, BUG("...Setting emixSum	: %d \n",emixSum));
		D(1, BUG("...Setting totalFrames	: %d \n",totalFrames));
		D(1, BUG("...Setting emixAvg	: %d \n",emixAvg));
		D(1, BUG("...Setting DEF_EMIX_RANDOM	: %lf \n",DEF_EMIX_RANDOM));

}

//_________________________________________________________________________________________________


// The main function creates two task and starts multi-tasking
int main(void)
{
  OSTaskCreateExt(main_task,
				  NULL,
				  (void *)&main_task_stk[TASK_STACKSIZE-1],
				  MAIN_TASK_PRIORITY,
				  MAIN_TASK_PRIORITY,
				  main_task_stk,
				  TASK_STACKSIZE,
				  NULL,
				  0);

  OSTaskCreateExt(link_task,
                  NULL,
                  (void *)&link_task_stk[TASK_STACKSIZE-1],
                  LINK_TASK_PRIORITY,
                  LINK_TASK_PRIORITY,
                  link_task_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);


  OSTaskCreateExt(c37dot94_task,
                  NULL,
                  (void *)&c3794_task_stk[TASK_STACKSIZE-1],
                  C3794_TASK_PRIORITY,
                  C3794_TASK_PRIORITY,
                  c3794_task_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSStart();
  return 0;
}



//*************************************************************************
//* Copyright (c) 2015 Greenlee Communications Vista, CA USA.			  *
//* All rights reserved.                                                  *
//*************************************************************************
