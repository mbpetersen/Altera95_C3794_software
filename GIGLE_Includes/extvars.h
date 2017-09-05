/*************************************************************************
 *		extvars.h														 *
 *				   														 *
 * Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
 * All rights reserved.                                                   *
 *************************************************************************/
#ifndef EXTVARS_H_
#define EXTVARS_H_

// Receiver buffer
//extern unsigned char RxBuf[RXBUFSIZE];		// global variables unsigned char RxBuf [RXBUFSIZE];
//extern int RxStart;                  		// Receive Buffer index: Start ptr
//extern int RxEnd;                 			// Receive Buffer index: End ptr
//extern unsigned char TxBuf[TXBUFSIZE]; 		// Transmit Buffer
//extern int TxStart;                  		// Transmit Buffer index: Start ptr
//extern int TxEnd;                  			// Transmit Buffer index: End ptr

extern _Bool gCUSTOM_TEST_START, gCUSTOM_TEST_STOP;
extern _Bool gTEST_TIMED_OUT, gPACKET_TEST_TIMEOUT;
extern _Bool END_TO_END_TEST;
extern _Bool GIGE_A;
extern _Bool SEND_SFPA_DATA, SEND_SFPB_DATA, SEND_SFP10A_DATA;
extern _Bool READ_SFPA_DATA, READ_SFPB_DATA, READ_SFP10A_DATA;
extern _Bool SEND_LPBK_DATA, SEND_LPBK10G_DATA;
extern _Bool SFP_10G_PRESENT;
//extern _Bool 			History,BERT_SYNC,BERT_ON;
//extern _Bool			NEW_RTDA;
//extern _Bool			TSE_B_LOOPBACK, TSE_B_SWAP_ADDR;
extern _Bool EXTERNAL_PHY_BOOT_COMPLETE;
extern _Bool SEND_MS_PACKET, MS_PKT_RCVD, SENDING_MS_CONFIG, SENDING_STD_PKT;

extern _Bool RxBuffer_RDY;
extern unsigned char EMON_TIMEOUT;
extern unsigned char RxBuffer[MaxConfig2];
extern unsigned char TxBuffer[MaxStatus2];
extern unsigned short buff_ptr, tbuff_ptr;
extern unsigned short	RxTIMEOUT;
extern unsigned char UART_CONTROL_REG;
/*extern unsigned short	LOSCNT;	// was int
 extern unsigned char	BERT_STATUS;*/

extern unsigned char Rx_PACKET_ID;
/*extern unsigned char	new_bert,bert_ptr;
 extern unsigned char	Misc_stat,Misc_statB;
 extern unsigned char	BERT_STATE;
 extern unsigned char	BERT, PAT_LENGTH;*/

extern unsigned char ConfigStat[MaxStatus2], ConfigStatB[MaxStatus2],
		ConfigStat10G[MaxStatus2], ConfigStat10GB[MaxStatus2],
		ConfigMS[MaxConfig_ms];
extern unsigned char SpecialStat[MaxStatus2], SpecialStatB[MaxStatus2];
extern unsigned char Special10GStat[MaxStatus2], Special10GStatB[MaxStatus2];
extern unsigned char LoopbackStat[MaxStatus2], LoopbackStatB[MaxStatus2],
		LoopbackStat10G[MaxStatus2];

//extern unsigned int		RTC;				// was long // internal BRICK RTC (copied from PDA on occasion, STORE & auto-test)
//extern unsigned int		RTD_measurement;	// was long
extern unsigned char GIGE_STATUS; // passed into [GIGERESULT_ptr]

//_______________________________________
//	C3794 variables
//_______________________________________
// BERT patterns
extern unsigned char	ConfigStatC37[MaxStatus];
extern unsigned char	C3794_Status;
extern signed short		PPM_offset;
extern unsigned int		CLKCTRL_register;
extern BERT_Patterns BERT;
extern unsigned char	BERT_STATUS, incntr, outcntr, BERT_STATE, Misc_stat37;
extern unsigned long	USER_PAT;
extern _Bool 	BERT_SYNC, BERT_INVERT;
extern _Bool	ERRORS_LED, HISTORY_LED;
extern unsigned short	LOSCNT, OOFCNT, YELCNT;


extern unsigned short PIO2_Content; // default - both A&B at 10M
extern unsigned short PIO3_Content;
extern volatile int pio_edge_capture;

//======= tcl_def's ported to VARIABLES =================
extern unsigned int gen_number_seconds;
extern unsigned long long number_packet_lsb;		//# Number of packets (1Mframes@1G=~18secs)  4250700300
extern unsigned short number_packet_msb;// upper 8-bit of 40-bit num_pkts
extern unsigned int MACA_COMMAND_CONFIG;// 0x02000013=ENA_10,ENA_TX/RX,PROMIS, 0x00000013=100M
extern unsigned int MACB_COMMAND_CONFIG;// 0x02000013=ENA_10,ENA_TX/RX,PROMIS, 0x00000013=100M
extern unsigned int MAC10A_COMMAND_CONFIG;//0x00000000; our custom MAC configurer
extern unsigned int MAC_0;// = 0x22334450;// MAC: 50-44-33-22-11-EE
extern unsigned int MAC_1;// = 0x0000EE11;
extern unsigned int MAC_0B;// = 0x33445560;// MAC: 60-55-44-33-11-BB
extern unsigned int MAC_1B;// = 0x0000BB11;


extern unsigned int destination_tse_addr0;
extern unsigned int destination_tse_addr1;
extern unsigned int source_tse_addr0;
extern unsigned int source_tse_addr1;

extern unsigned char FlashQTX[128];
extern unsigned char FlashParameters[FlashParameterSize];
//extern unsigned int SYS_OPTIONS;
extern unsigned char OPTIONS_INSTALLED, OPTIONS_ENABLED;
//extern unsigned char SERNUM4; // MSbyte (5-bytes total)
//extern unsigned char  SERNUM3; // 3,2,1,0

//extern unsigned short PHYA_COPPER_CONFIG, PHYB_COPPER_CONFIG, PHYA_FIBER_CONFIG, PHYB_FIBER_CONFIG;
extern unsigned short PHYA_CONFIG, PHYB_CONFIG;
extern unsigned char PHYA_ETH_SPEED; // speed_10M;	// 0=10M
extern unsigned char PHYB_ETH_SPEED; // speed_10M;	// 0=10M
extern _Bool PHYA_COPPER_ENABLE_AN; //# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
extern _Bool PHYA_FIBER_ENABLE_AN; //# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
extern _Bool PHYA_COPPER_FULLDUPLEX;//# FDX = 1 and HDX = 0
extern _Bool PHYA_FIBER_FULLDUPLEX; //# FDX = 1 and HDX = 0
extern _Bool PHYB_COPPER_ENABLE_AN; //# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
extern _Bool PHYB_FIBER_ENABLE_AN; //# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
extern _Bool PHYB_COPPER_FULLDUPLEX;//# FDX = 1 and HDX = 0
extern _Bool PHYB_FIBER_FULLDUPLEX; //# FDX = 1 and HDX = 0

//extern unsigned long  FPS_PAUSE_COUNT;
extern unsigned short MAC_FRM_LENGTH;
//extern unsigned short egen_pkt_length;
extern float egen_line_load_rate; // 1 to 100 - def 100 (%).
extern unsigned long long total_num_packets;
extern unsigned int traffic_gen_config; //def:0x000E05EE:  noRND,fixedLEN,HDRLEN_ETH,1518 in length
extern unsigned int traffic_gen_config2;
extern unsigned int traffic_gen_config3;
extern unsigned int traffic_gen_config4;
/*extern _Bool	 length_sel;				//# Fixed = 0	& Random = 1
 //======= TEST RESULT VARIABLES =================
 extern _Bool	EGEN_TEST_RUNNING;
 extern double test_time_portA, test_time_portB;
 extern double test_delaytime_portA,test_latencytime_min_portA,test_latencytime_max_portA;
 extern double test_cycle_end_time_portA;
 extern unsigned int test_jitter_time_portA;
 extern unsigned int test_packets_sent_portA, test_packets_sent_portB;
 extern double test_emon_bytes_received_portA, test_emon_bytes_received_portB;
 extern unsigned int test_expected_packets_portA, test_valid_packets_portA, test_expected_packets_portB, test_valid_packets_portB;
 extern unsigned int test_error_packets_portA, test_error_packets_portB;
 extern double test_data_received_portA, test_data_received_portB;
 extern double test_bps_portA, test_bps_portB;
 extern double test_linerate_portA, test_linerate_portB;
 extern double test_packets_per_second_portA, test_packets_per_second_portB;*/

//========== 10G variables ===============
_Bool GIGE10; // default to TSE on boot
extern unsigned int MAC10G_0, MAC10G_0B;
extern unsigned int MAC10G_1, MAC10G_1B;

extern unsigned int destination_10g_addr0;//
extern unsigned int destination_10g_addr1;
extern unsigned int source_10g_addr0; // usually this IS the same as the MAC (sourcing the EGEN)
extern unsigned int source_10g_addr1; // init to Port-A MAC addr (our EGEN)


extern unsigned char stream_no;
extern unsigned char num_of_streams;
extern unsigned char stream_offset;
extern unsigned short size_offset;


extern unsigned long tot_hicycles, tot_locycles;
extern unsigned long tot_cycles_used, tot_traffic_mon_delay_min,
		tot_traffic_mon_delay_max, tot_traffic_mon_delay_sum, tot_lobytes,
		tot_hibytes;
extern unsigned long long tot_good_pkts_rcvd, tot_bad_pkts_rcvd,
		tot_good_pkts_rcvd1, tot_bad_pkts_rcvd1,tot_packets_rcvd;
extern unsigned int  tot_packets_to_transmit,
		tot_packets_act_transmit, tot_packets_to_transmit1,
		tot_packets_act_transmit1, tot_rx_broadcast_cnt, tot_rx_broadcast_cnt1;
extern double tot_sumbytes, tot_rcvd_bytes, tot_avg_pkt_size, tot_seconds;

extern _Bool MS_PKT, MS_TEST_ACTIVE, ETH_PKT;
extern _Bool SS_PKT;
extern _Bool RCVING_MS_PKT, ETH_PACKET;


//extern unsigned long  FPS_10G_PAUSE_COUNT;
extern unsigned short traffic_gen_pkt_length;

extern unsigned char SET_SWAP_A_TOGGLE;
extern unsigned char SET_SWAP_B_TOGGLE;
extern unsigned char SET_SWAP_10GA_TOGGLE;

extern unsigned long maxRxBustinessVal; // RxBurstiness==Incoming data jittering/bursting so as to overload our TxFIFO

extern unsigned char LOOK_FOR_RESPONSE;
extern _Bool loopup_prev;
extern _Bool loopdown_prev;
extern volatile int irq_aggr_capture;
extern short requestTimer;

extern _Bool DETECT_REMOTE_LOOPBACK_TSEA;
extern _Bool DETECT_REMOTE_LOOPBACK_TSEB;
extern _Bool DETECT_REMOTE_LOOPBACK_10GA;
extern unsigned char loopbackStatPtr;
extern char responseUnits;
extern char LOOPBACK_EVENT;
extern _Bool SWAP_IP;

// Keep track of frame configurations for port B
extern _Bool ipv4B_active;
extern _Bool ipv6B_active;
/*extern long portB_ipv6_address0;
 extern long portB_ipv6_address1;
 extern long portB_ipv6_address2;
 extern long portB_ipv6_address3;*/

extern unsigned char test_options;
extern unsigned char emix;
extern unsigned char vlanTags;
extern unsigned char mplsTags;
extern unsigned char rndPayload;
extern long long burst_packets;

extern unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr; //used to handle the case w
extern unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE4_ptr;

extern unsigned short TRAFFIC_MON_STREAM_CNTL3_ptr; // MON stream control (for independent capture)
extern unsigned short TRAFFIC_MON_STREAM_CNTL2_ptr;
extern unsigned short TRAFFIC_MON_STREAM_CNTL1_ptr;
extern unsigned short TRAFFIC_MON_STREAM_CNTL0_ptr;

extern unsigned short TRAFFIC_RATE_TYPE; // TBD: e.g. 0=FPS%, 1=Gbps, 2=Mbps, 3=Kbps
extern unsigned short TRAFFIC_RATE3_ptr;
extern unsigned short TRAFFIC_RATE2_ptr;// Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%	;
extern unsigned short TRAFFIC_RATE1_ptr;
extern unsigned short TRAFFIC_RATE0_ptr; //  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M	;

extern unsigned short TRAFFIC_GEN_PKTPERCENT11_ptr;
extern unsigned short TRAFFIC_GEN_PKTPERCENT10_ptr;
extern unsigned short TRAFFIC_GEN_PKTPERCENT03_ptr;
extern unsigned short TRAFFIC_GEN_PKTPERCENT02_ptr;
extern unsigned short TRAFFIC_GEN_PKTPERCENT01_ptr;
extern unsigned short TRAFFIC_GEN_PKTPERCENT00_ptr;
//_______________EGEN_PKT_TYPE_ptr____________________				;
// 0000,0000:  ^10:Ethernet, ^32:IP, ^54:TCP/UDP, ^76:FUTURE				;
// 10: 00=std Layer2-Ethernet, 01=Ethernet II, 10=802.3 LLC, 11=802.3SNAP				;
// 32: 00=none, 01=IPv4, 10=IPv6				;
// 54: 00=none, 01=TCP, 10=UDP				;
// 76: future				;
//_________________EGEN_PKT_TYPE_ptr_1____________________				;
// 0000,0000:  ^7654:MPLS, ^3210:VLAN 				;
//_________________EGEN_PKT_TYPE_ptr_2____________________				;
//				;
extern unsigned short EGEN_PKT_TYPE_ptr;
extern unsigned short EGEN_PKT_TYPE_ptr_1;
extern unsigned short EGEN_PKT_TYPE_ptr_2;
;
/*......for				;
 TRAFFIC_GEN_CONFIG				;
 TRAFFIC_GEN_CONFIG1				;
 TRAFFIC_GEN_CONFIG2				;
 ... in hardware_def.h copy the bit definitions & mask locations so the UI can set these 32-bit words				;
 as necessary per the FRAME CONFIGURED!				;
 ....the UI will be taking over all TRAFFIC_GEN & MON RELATED configurations associated with the following;				;
 ;
 and the associated frame type settings like how long the hdr is, where the IPv4 checksum is, where the payload starts, etc. etc.				;
 ...all these elements are within these 3 GEN config bytes (note there are some MON config ones as well..				;
 ;
 //   header plus sequence_number/stream/timestamp.				;
 //   for ipv4, the header checksum must also be changed.				;
 // packet length is one value in a Y.1564 EMIX table for random length mode (see RNDUSR and RNDSEQ below));				;
 //=================================================================================				;
 //public static  short  TRAFFIC_GEN_CONFIG				;
 // 				;
 // CONFIG_PKLEN				;
 // CONFIG_HDRLEN  			8-bit header length (header can be 0 to 255 bytes)); (note MAC, IPv4, IPv6 overwritten with correct length by GEN));	;
 // CONFIG_RNDLEN  			 	;
 // CONFIG_RNDPAY				;	1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
 // CONFIG_FRCERR				;	1-bit Force Payload data error (field size bits)); DEBUGGING ONLY
 // CONFIG_CONTIN   			      1-bit Continuous Generate (ignore PACKET_NUM));	;
 // CONFIG_BWRANK   			4-bits bandwidth rank: for streams set priority to higher BW above those with lower BW. 0=top, 15=lowest	;
 //==================================================================================				;
 //				;
 ;			// 7 bits: Length field byte offset in HDR
 ;
 ;			// 1 bit:  jumbo length to 0x8870 handling field size bits
 ;
 8:14	;	// 7-bits: ipv4 hcs byte offset field size bits
 15:31	;	// 17-bits: spare field size bits
 ;	// 802.3, 1518=payload len of 1500, 1500-64=1436, 0x5ee-0x40=0x059C
 ;			16-bit:  shortermediate length for random length recalc field size bits
 16:31	;		16-bit: shortermediate HCS for random length HCS recalc field size bits
 ;						*/

extern unsigned short TRAFFIC_GEN_CONFIG33_ptr; //Traffic Gen Config 4
extern unsigned short TRAFFIC_GEN_CONFIG32_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG31_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG30_ptr;

extern unsigned short TRAFFIC_GEN_CONFIG23_ptr; //TRAFFIC_GEN_CONFIG3	;
extern unsigned short TRAFFIC_GEN_CONFIG22_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG21_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG20_ptr;

extern unsigned short TRAFFIC_GEN_CONFIG13_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG12_ptr; //15:31 // 17-bits: spare field size bits	;
extern unsigned short TRAFFIC_GEN_CONFIG11_ptr;//CONFG2_HCSOFS_SIZ	;
extern unsigned short TRAFFIC_GEN_CONFIG10_ptr;// 7 bits: Length field byte offset in HDR + 1 bit:  jumbo length to 0x8870 handling field size bits	;

extern unsigned short TRAFFIC_GEN_CONFIG03_ptr;// TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed	;
extern unsigned short TRAFFIC_GEN_CONFIG02_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG01_ptr;
extern unsigned short TRAFFIC_GEN_CONFIG00_ptr;

extern double DEF_EMIX_RANDOM;
extern double DEF_GEN_EMIXSEQ0;
extern double DEF_GEN_EMIXSEQ1;
extern double DEF_GEN_EMIXSEQ2;
extern double DEF_GEN_EMIXSEQ3;
extern double DEF_GEN_EMIXUSER;
extern double DEF_GEN_EMIXUSER1;

extern unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr;
extern unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE2_ptr;
extern unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr;
extern unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE0_ptr;

extern unsigned short TRAFFIC_GEN_EMIXSEQ33_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ32_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ31_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ30_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ23_ptr; //(good to set 1,2&3 def as 0xF0000000)
extern unsigned short TRAFFIC_GEN_EMIXSEQ22_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ21_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ20_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ13_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ12_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ11_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ10_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ03_ptr; //LSNibble sent last
extern unsigned short TRAFFIC_GEN_EMIXSEQ02_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ01_ptr;
extern unsigned short TRAFFIC_GEN_EMIXSEQ00_ptr;

/*
 extern unsigned short EGEN_PKT_EMIXUSERSIZE3_ptr				;
 extern unsigned short EGEN_PKT_EMIXUSERSIZE2_ptr				;
 extern unsigned short EGEN_PKT_EMIXUSERSIZE1_ptr				;
 extern unsigned short EGEN_PKT_EMIXUSERSIZE0_ptr				;

 extern unsigned short EGEN_EMIXSEQ33_ptr  				;
 extern unsigned short EGEN_EMIXSEQ32_ptr  				;
 extern unsigned short EGEN_EMIXSEQ31_ptr  				;
 extern unsigned short EGEN_EMIXSEQ30_ptr  				;
 extern unsigned short EGEN_EMIXSEQ23_ptr				;		//(good to set 1,2&3 def as 0xF0000000)
 extern unsigned short EGEN_EMIXSEQ22_ptr  				;
 extern unsigned short EGEN_EMIXSEQ21_ptr  				;
 extern unsigned short EGEN_EMIXSEQ20_ptr 				;
 extern unsigned short EGEN_EMIXSEQ13_ptr				;
 extern unsigned short EGEN_EMIXSEQ12_ptr  				;
 extern unsigned short EGEN_EMIXSEQ11_ptr				;
 extern unsigned short EGEN_EMIXSEQ10_ptr				;
 extern unsigned short EGEN_EMIXSEQ03_ptr 				;	//LSNibble sent last
 extern unsigned short EGEN_EMIXSEQ02_ptr 				;
 extern unsigned short EGEN_EMIXSEQ01_ptr				;
 extern unsigned short EGEN_EMIXSEQ00_ptr  				;

 */

/*************************************************************************************************************************				;
 // Config bytes related to Y.1564 & RFC2544 User selectable Test thresholds				;
 ***************************************************************************************************************************				;
 extern unsigned short TEST_OPTIONS_ptr 			XX // Test options to select between (quick test,multistream, y.1564, rfc..) - In the main Config is used to set the different type of test- service configuration/service performance/both)	;
 ...//...CIR & EIR are 4-bytes (they are rates - like TRAFFICRATE_TYPE above)				;
 //RFC2544 will share/overlap these thresholds to allow threshold support for RFC2544 as well		*/
extern unsigned short Y1564_CIR_3_ptr; //Committed information rate
extern unsigned short Y1564_CIR_2_ptr;
extern unsigned short Y1564_CIR_1_ptr;
extern unsigned short Y1564_CIR_0_ptr;

extern unsigned short Y1564_EIR_3_ptr; //Expected information rate
extern unsigned short Y1564_EIR_2_ptr; //Expected information rate
extern unsigned short Y1564_EIR_1_ptr; //Expected information rate
extern unsigned short Y1564_EIR_0_ptr; //Expected information rate

extern unsigned short Y1564_CIR_STEPS_ptr; //Steps for CIR – first 4 bits

extern unsigned short Y1564_MAX_FTD_3_ptr;
extern unsigned short Y1564_MAX_FTD_2_ptr;
extern unsigned short Y1564_MAX_FTD_1_ptr;
extern unsigned short Y1564_MAX_FTD_0_ptr;

extern unsigned short Y1564_MAX_FDV_3_ptr;
extern unsigned short Y1564_MAX_FDV_2_ptr;
extern unsigned short Y1564_MAX_FDV_1_ptr;
extern unsigned short Y1564_MAX_FDV_0_ptr;

extern unsigned short Y1564_MAX_FRAME_LOSS_3_ptr;//4 byte Float	;
extern unsigned short Y1564_MAX_FRAME_LOSS_2_ptr;
extern unsigned short Y1564_MAX_FRAME_LOSS_1_ptr;
extern unsigned short Y1564_MAX_FRAME_LOSS_0_ptr;

extern unsigned short Y1564_MAX_AVAIL_3_ptr;
extern unsigned short Y1564_MAX_AVAIL_2_ptr;
extern unsigned short Y1564_MAX_AVAIL_1_ptr;
extern unsigned short Y1564_MAX_AVAIL_0_ptr;

extern unsigned short SPARE_THRESHOLD_03;
extern unsigned short SPARE_THRESHOLD_02;
extern unsigned short SPARE_THRESHOLD_01;
extern unsigned short SPARE_THRESHOLD_00;

extern unsigned short TRAFFIC_GEN_SPARE_ptr;
extern unsigned short TRAFFIC_HDR_DEF_ptr; // additional instructional info for what is contained in HDR ptr's

extern unsigned short TRAFFIC_HDR0_3_ptr;
extern unsigned short TRAFFIC_HDR0_2_ptr;
extern unsigned short TRAFFIC_HDR0_1_ptr;
extern unsigned short TRAFFIC_HDR0_0_ptr;
extern unsigned short TRAFFIC_HDR1_3_ptr;
extern unsigned short TRAFFIC_HDR1_2_ptr;
extern unsigned short TRAFFIC_HDR1_1_ptr;
extern unsigned short TRAFFIC_HDR1_0_ptr;
extern unsigned short TRAFFIC_HDR2_3_ptr;
extern unsigned short TRAFFIC_HDR2_2_ptr;
extern unsigned short TRAFFIC_HDR2_1_ptr;
extern unsigned short TRAFFIC_HDR2_0_ptr;
extern unsigned short TRAFFIC_HDR3_3_ptr;
extern unsigned short TRAFFIC_HDR3_2_ptr;
extern unsigned short TRAFFIC_HDR3_1_ptr;
extern unsigned short TRAFFIC_HDR3_0_ptr;
extern unsigned short TRAFFIC_HDR4_3_ptr;
extern unsigned short TRAFFIC_HDR4_2_ptr;
extern unsigned short TRAFFIC_HDR4_1_ptr;
extern unsigned short TRAFFIC_HDR4_0_ptr;
extern unsigned short TRAFFIC_HDR5_3_ptr;
extern unsigned short TRAFFIC_HDR5_2_ptr;
extern unsigned short TRAFFIC_HDR5_1_ptr;
extern unsigned short TRAFFIC_HDR5_0_ptr;
extern unsigned short TRAFFIC_HDR6_3_ptr;
extern unsigned short TRAFFIC_HDR6_2_ptr;
extern unsigned short TRAFFIC_HDR6_1_ptr;
extern unsigned short TRAFFIC_HDR6_0_ptr;
extern unsigned short TRAFFIC_HDR7_3_ptr;
extern unsigned short TRAFFIC_HDR7_2_ptr;
extern unsigned short TRAFFIC_HDR7_1_ptr;
extern unsigned short TRAFFIC_HDR7_0_ptr;
extern unsigned short TRAFFIC_HDR8_3_ptr;
extern unsigned short TRAFFIC_HDR8_2_ptr;
extern unsigned short TRAFFIC_HDR8_1_ptr;
extern unsigned short TRAFFIC_HDR8_0_ptr;
extern unsigned short TRAFFIC_HDR9_3_ptr;
extern unsigned short TRAFFIC_HDR9_2_ptr;
extern unsigned short TRAFFIC_HDR9_1_ptr;
extern unsigned short TRAFFIC_HDR9_0_ptr;
extern unsigned short TRAFFIC_HDR10_3_ptr;
extern unsigned short TRAFFIC_HDR10_2_ptr;
extern unsigned short TRAFFIC_HDR10_1_ptr;
extern unsigned short TRAFFIC_HDR10_0_ptr;
extern unsigned short TRAFFIC_HDR11_3_ptr;
extern unsigned short TRAFFIC_HDR11_2_ptr;
extern unsigned short TRAFFIC_HDR11_1_ptr;
extern unsigned short TRAFFIC_HDR11_0_ptr;
extern unsigned short TRAFFIC_HDR12_3_ptr;
extern unsigned short TRAFFIC_HDR12_2_ptr;
extern unsigned short TRAFFIC_HDR12_1_ptr;
extern unsigned short TRAFFIC_HDR12_0_ptr;
extern unsigned short TRAFFIC_HDR13_3_ptr;
extern unsigned short TRAFFIC_HDR13_2_ptr;
extern unsigned short TRAFFIC_HDR13_1_ptr;
extern unsigned short TRAFFIC_HDR13_0_ptr;
extern unsigned short TRAFFIC_HDR14_3_ptr;
extern unsigned short TRAFFIC_HDR14_2_ptr;
extern unsigned short TRAFFIC_HDR14_1_ptr;
extern unsigned short TRAFFIC_HDR14_0_ptr;
extern unsigned short TRAFFIC_HDR15_3_ptr;
extern unsigned short TRAFFIC_HDR15_2_ptr;
extern unsigned short TRAFFIC_HDR15_1_ptr;
extern unsigned short TRAFFIC_HDR15_0_ptr;
extern unsigned short TRAFFIC_HDR16_3_ptr;
extern unsigned short TRAFFIC_HDR16_2_ptr;
extern unsigned short TRAFFIC_HDR16_1_ptr;
extern unsigned short TRAFFIC_HDR16_0_ptr;
extern unsigned short TRAFFIC_HDR17_3_ptr;
extern unsigned short TRAFFIC_HDR17_2_ptr;
extern unsigned short TRAFFIC_HDR17_1_ptr;
extern unsigned short TRAFFIC_HDR17_0_ptr;
extern unsigned short TRAFFIC_HDR18_3_ptr;
extern unsigned short TRAFFIC_HDR18_2_ptr;
extern unsigned short TRAFFIC_HDR18_1_ptr;
extern unsigned short TRAFFIC_HDR18_0_ptr;
extern unsigned short TRAFFIC_HDR19_3_ptr;
extern unsigned short TRAFFIC_HDR19_2_ptr;
extern unsigned short TRAFFIC_HDR19_1_ptr;
extern unsigned short TRAFFIC_HDR19_0_ptr;
extern unsigned short TRAFFIC_HDR20_3_ptr;
extern unsigned short TRAFFIC_HDR20_2_ptr;
extern unsigned short TRAFFIC_HDR20_1_ptr;
extern unsigned short TRAFFIC_HDR20_0_ptr;
extern unsigned short TRAFFIC_HDR21_3_ptr;
extern unsigned short TRAFFIC_HDR21_2_ptr;
extern unsigned short TRAFFIC_HDR21_1_ptr;
extern unsigned short TRAFFIC_HDR21_0_ptr;
extern unsigned short TRAFFIC_HDR22_3_ptr;
extern unsigned short TRAFFIC_HDR22_2_ptr;
extern unsigned short TRAFFIC_HDR22_1_ptr;
extern unsigned short TRAFFIC_HDR22_0_ptr;
extern unsigned short TRAFFIC_HDR23_3_ptr;
extern unsigned short TRAFFIC_HDR23_2_ptr;
extern unsigned short TRAFFIC_HDR23_1_ptr;
extern unsigned short TRAFFIC_HDR23_0_ptr;
extern unsigned short TRAFFIC_HDR24_3_ptr;
extern unsigned short TRAFFIC_HDR24_2_ptr;
extern unsigned short TRAFFIC_HDR24_1_ptr;
extern unsigned short TRAFFIC_HDR24_0_ptr;
extern unsigned short TRAFFIC_HDR25_3_ptr;
extern unsigned short TRAFFIC_HDR25_2_ptr;
extern unsigned short TRAFFIC_HDR25_1_ptr;
extern unsigned short TRAFFIC_HDR25_0_ptr;
extern unsigned short TRAFFIC_HDR26_3_ptr;
extern unsigned short TRAFFIC_HDR26_2_ptr;
extern unsigned short TRAFFIC_HDR26_1_ptr;
extern unsigned short TRAFFIC_HDR26_0_ptr;
extern unsigned short TRAFFIC_HDR27_3_ptr;
extern unsigned short TRAFFIC_HDR27_2_ptr;
extern unsigned short TRAFFIC_HDR27_1_ptr;
extern unsigned short TRAFFIC_HDR27_0_ptr;
extern unsigned short TRAFFIC_HDR28_3_ptr;
extern unsigned short TRAFFIC_HDR28_2_ptr;
extern unsigned short TRAFFIC_HDR28_1_ptr;
extern unsigned short TRAFFIC_HDR28_0_ptr;
extern unsigned short TRAFFIC_HDR29_3_ptr;
extern unsigned short TRAFFIC_HDR29_2_ptr;
extern unsigned short TRAFFIC_HDR29_1_ptr;
extern unsigned short TRAFFIC_HDR29_0_ptr;
extern unsigned short TRAFFIC_HDR30_3_ptr;
extern unsigned short TRAFFIC_HDR30_2_ptr;
extern unsigned short TRAFFIC_HDR30_1_ptr;
extern unsigned short TRAFFIC_HDR30_0_ptr;
extern unsigned short TRAFFIC_HDR31_3_ptr;
extern unsigned short TRAFFIC_HDR31_2_ptr;
extern unsigned short TRAFFIC_HDR31_1_ptr;
extern unsigned short TRAFFIC_HDR31_0_ptr;

extern unsigned short TRAFFIC_MSK0_3_ptr;
extern unsigned short TRAFFIC_MSK0_2_ptr;
extern unsigned short TRAFFIC_MSK0_1_ptr;
extern unsigned short TRAFFIC_MSK0_0_ptr;
extern unsigned short TRAFFIC_MSK1_3_ptr;
extern unsigned short TRAFFIC_MSK1_2_ptr;
extern unsigned short TRAFFIC_MSK1_1_ptr;
extern unsigned short TRAFFIC_MSK1_0_ptr;
extern unsigned short TRAFFIC_MSK2_3_ptr;
extern unsigned short TRAFFIC_MSK2_2_ptr;
extern unsigned short TRAFFIC_MSK2_1_ptr;
extern unsigned short TRAFFIC_MSK2_0_ptr;
extern unsigned short TRAFFIC_MSK3_3_ptr;
extern unsigned short TRAFFIC_MSK3_2_ptr;
extern unsigned short TRAFFIC_MSK3_1_ptr;
extern unsigned short TRAFFIC_MSK3_0_ptr;
extern unsigned short TRAFFIC_MSK4_3_ptr;
extern unsigned short TRAFFIC_MSK4_2_ptr;
extern unsigned short TRAFFIC_MSK4_1_ptr;
extern unsigned short TRAFFIC_MSK4_0_ptr;
extern unsigned short TRAFFIC_MSK5_3_ptr;
extern unsigned short TRAFFIC_MSK5_2_ptr;
extern unsigned short TRAFFIC_MSK5_1_ptr;
extern unsigned short TRAFFIC_MSK5_0_ptr;
extern unsigned short TRAFFIC_MSK6_3_ptr;
extern unsigned short TRAFFIC_MSK6_2_ptr;
extern unsigned short TRAFFIC_MSK6_1_ptr;
extern unsigned short TRAFFIC_MSK6_0_ptr;
extern unsigned short TRAFFIC_MSK7_3_ptr;
extern unsigned short TRAFFIC_MSK7_2_ptr;
extern unsigned short TRAFFIC_MSK7_1_ptr;
extern unsigned short TRAFFIC_MSK7_0_ptr;
extern unsigned short TRAFFIC_MSK8_3_ptr;
extern unsigned short TRAFFIC_MSK8_2_ptr;
extern unsigned short TRAFFIC_MSK8_1_ptr;
extern unsigned short TRAFFIC_MSK8_0_ptr;
extern unsigned short TRAFFIC_MSK9_3_ptr;
extern unsigned short TRAFFIC_MSK9_2_ptr;
extern unsigned short TRAFFIC_MSK9_1_ptr;
extern unsigned short TRAFFIC_MSK9_0_ptr;
extern unsigned short TRAFFIC_MSK10_3_ptr;
extern unsigned short TRAFFIC_MSK10_2_ptr;
extern unsigned short TRAFFIC_MSK10_1_ptr;
extern unsigned short TRAFFIC_MSK10_0_ptr;
extern unsigned short TRAFFIC_MSK11_3_ptr;
extern unsigned short TRAFFIC_MSK11_2_ptr;
extern unsigned short TRAFFIC_MSK11_1_ptr;
extern unsigned short TRAFFIC_MSK11_0_ptr;
extern unsigned short TRAFFIC_MSK12_3_ptr;
extern unsigned short TRAFFIC_MSK12_2_ptr;
extern unsigned short TRAFFIC_MSK12_1_ptr;
extern unsigned short TRAFFIC_MSK12_0_ptr;
extern unsigned short TRAFFIC_MSK13_3_ptr;
extern unsigned short TRAFFIC_MSK13_2_ptr;
extern unsigned short TRAFFIC_MSK13_1_ptr;
extern unsigned short TRAFFIC_MSK13_0_ptr;
extern unsigned short TRAFFIC_MSK14_3_ptr;
extern unsigned short TRAFFIC_MSK14_2_ptr;
extern unsigned short TRAFFIC_MSK14_1_ptr;
extern unsigned short TRAFFIC_MSK14_0_ptr;
extern unsigned short TRAFFIC_MSK15_3_ptr;
extern unsigned short TRAFFIC_MSK15_2_ptr;
extern unsigned short TRAFFIC_MSK15_1_ptr;
extern unsigned short TRAFFIC_MSK15_0_ptr;
extern unsigned short TRAFFIC_MSK16_3_ptr;
extern unsigned short TRAFFIC_MSK16_2_ptr;
extern unsigned short TRAFFIC_MSK16_1_ptr;
extern unsigned short TRAFFIC_MSK16_0_ptr;
extern unsigned short TRAFFIC_MSK17_3_ptr;
extern unsigned short TRAFFIC_MSK17_2_ptr;
extern unsigned short TRAFFIC_MSK17_1_ptr;
extern unsigned short TRAFFIC_MSK17_0_ptr;
extern unsigned short TRAFFIC_MSK18_3_ptr;
extern unsigned short TRAFFIC_MSK18_2_ptr;
extern unsigned short TRAFFIC_MSK18_1_ptr;
extern unsigned short TRAFFIC_MSK18_0_ptr;
extern unsigned short TRAFFIC_MSK19_3_ptr;
extern unsigned short TRAFFIC_MSK19_2_ptr;
extern unsigned short TRAFFIC_MSK19_1_ptr;
extern unsigned short TRAFFIC_MSK19_0_ptr;
extern unsigned short TRAFFIC_MSK20_3_ptr;
extern unsigned short TRAFFIC_MSK20_2_ptr;
extern unsigned short TRAFFIC_MSK20_1_ptr;
extern unsigned short TRAFFIC_MSK20_0_ptr;
extern unsigned short TRAFFIC_MSK21_3_ptr;
extern unsigned short TRAFFIC_MSK21_2_ptr;
extern unsigned short TRAFFIC_MSK21_1_ptr;
extern unsigned short TRAFFIC_MSK21_0_ptr;
extern unsigned short TRAFFIC_MSK22_3_ptr;
extern unsigned short TRAFFIC_MSK22_2_ptr;
extern unsigned short TRAFFIC_MSK22_1_ptr;
extern unsigned short TRAFFIC_MSK22_0_ptr;
extern unsigned short TRAFFIC_MSK23_3_ptr;
extern unsigned short TRAFFIC_MSK23_2_ptr;
extern unsigned short TRAFFIC_MSK23_1_ptr;
extern unsigned short TRAFFIC_MSK23_0_ptr;
extern unsigned short TRAFFIC_MSK24_3_ptr;
extern unsigned short TRAFFIC_MSK24_2_ptr;
extern unsigned short TRAFFIC_MSK24_1_ptr;
extern unsigned short TRAFFIC_MSK24_0_ptr;
extern unsigned short TRAFFIC_MSK25_3_ptr;
extern unsigned short TRAFFIC_MSK25_2_ptr;
extern unsigned short TRAFFIC_MSK25_1_ptr;
extern unsigned short TRAFFIC_MSK25_0_ptr;
extern unsigned short TRAFFIC_MSK26_3_ptr;
extern unsigned short TRAFFIC_MSK26_2_ptr;
extern unsigned short TRAFFIC_MSK26_1_ptr;
extern unsigned short TRAFFIC_MSK26_0_ptr;
extern unsigned short TRAFFIC_MSK27_3_ptr;
extern unsigned short TRAFFIC_MSK27_2_ptr;
extern unsigned short TRAFFIC_MSK27_1_ptr;
extern unsigned short TRAFFIC_MSK27_0_ptr;
extern unsigned short TRAFFIC_MSK28_3_ptr;
extern unsigned short TRAFFIC_MSK28_2_ptr;
extern unsigned short TRAFFIC_MSK28_1_ptr;
extern unsigned short TRAFFIC_MSK28_0_ptr;
extern unsigned short TRAFFIC_MSK29_3_ptr;
extern unsigned short TRAFFIC_MSK29_2_ptr;
extern unsigned short TRAFFIC_MSK29_1_ptr;
extern unsigned short TRAFFIC_MSK29_0_ptr;
extern unsigned short TRAFFIC_MSK30_3_ptr;
extern unsigned short TRAFFIC_MSK30_2_ptr;
extern unsigned short TRAFFIC_MSK30_1_ptr;
extern unsigned short TRAFFIC_MSK30_0_ptr;
extern unsigned short TRAFFIC_MSK31_3_ptr;
extern unsigned short TRAFFIC_MSK31_2_ptr;
extern unsigned short TRAFFIC_MSK31_1_ptr;
extern unsigned short TRAFFIC_MSK31_0_ptr;

extern short ConfigChecksum2;

#endif /* EXTVARS_H_ */
/*************************************************************************
 * Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
 * All rights reserved.                                                   *
 *************************************************************************/
