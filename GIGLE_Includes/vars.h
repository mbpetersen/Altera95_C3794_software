/*************************************************************************
*		vars.h															 *
*				   														 *
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
#ifndef VARS_H_
#define VARS_H_

//#include "confstat.h"
// Receiver buffer
//unsigned char RxBuf[RXBUFSIZE];		// global variables unsigned char RxBuf [RXBUFSIZE];
//int RxStart =0;                  	// Receive Buffer index: Start ptr
//int RxEnd =0;                 		// Receive Buffer index: End ptr
//unsigned char TxBuf[TXBUFSIZE]; 	// Transmit Buffer
//int TxStart =0;                  	// Transmit Buffer index: Start ptr
//int TxEnd =0;                  		// Transmit Buffer index: End ptr

_Bool			gCUSTOM_TEST_START = NO, gCUSTOM_TEST_STOP = NO;
_Bool			gPACKET_TEST_TIMEOUT = NO; //old TSE gTEST_TIMED_OUT = NO,
_Bool			END_TO_END_TEST = NO;//, END_TO_END_TEST_10G = NO;	// standard Looped Test!
_Bool			GIGE_A = YES;	// default to PortA on boot
_Bool			SEND_SFPA_DATA = NO, SEND_SFPB_DATA = NO, SEND_SFP10A_DATA = NO ;
_Bool			READ_SFPA_DATA = NO, READ_SFPB_DATA = NO, READ_SFP10A_DATA = NO;
_Bool			SEND_LPBK_DATA = NO, SEND_LPBK10G_DATA = NO;
_Bool			SFP_10G_PRESENT = NO;
_Bool			EXTERNAL_PHY_BOOT_COMPLETE = NO;
_Bool 			SEND_MS_PACKET = NO, MS_PKT_RCVD = NO, SENDING_MS_CONFIG = NO, SENDING_STD_PKT = NO;


unsigned char	EMON_TIMEOUT=50;	// 50 (5s) for Looped-tests, 200 (10s) for end-to-end tests.
_Bool			RxBuffer_RDY=0;
unsigned char	RxBuffer[MaxConfig_ms];
//unsigned char	TxBuffer[MaxStatus2];
unsigned char	TxBuffer[MaxConfig_ms];
unsigned short	buff_ptr=0,tbuff_ptr=0;
unsigned short	RxTIMEOUT=0;
unsigned char	UART_CONTROL_REG=0;
unsigned char	TIMER1_CONTROL_REG=0;
/*
unsigned short	LOSCNT=0;			// was int
unsigned char	BERT_STATUS=0;
*/

unsigned char	Rx_PACKET_ID=0xFF;
/*unsigned char	new_bert=0,bert_ptr=2;
unsigned char	Misc_stat=0,Misc_statB=0;
unsigned char	BERT_STATE=0;
unsigned char	BERT=0, PAT_LENGTH=0;*/

unsigned char	ConfigStat[MaxStatus2], ConfigStatB[MaxStatus2], ConfigStat10G[MaxStatus2], ConfigStat10GB[MaxStatus2], ConfigMS[MaxConfig_ms];//ConfigStat[MaxStatus2]
unsigned char	SpecialStat[MaxStatus2], SpecialStatB[MaxStatus2];
unsigned char	Special10GStat[MaxStatus2], Special10GStatB[MaxStatus2];
unsigned char	LoopbackStat[MaxStatus2], LoopbackStatB[MaxStatus2], LoopbackStat10G[MaxStatus2];

//unsigned char	HDR[32][16];	// Future: 32-headers, 16-streams

//unsigned long	RTC=0;				// was long // internal BRICK RTC (copied from PDA on occasion, STORE & auto-test)
//unsigned long	RTD_measurement=0;	// was long
unsigned char	GIGE_STATUS=0; // passed into [GIGERESULT_ptr]

//_______________________________________
//	C3794 variables
//_______________________________________
unsigned char	ConfigStatC37[MaxStatus];
unsigned char	C3794_State=0, C3794_status=0;
signed short	PPM_offset=0;
_Bool 	BERT_SYNC = 0,BERT_INVERT=0;
_Bool	ERRORS_LED=0, HISTORY_LED=0;
unsigned int	CLKCTRL_register=0;

BERT_Patterns BERT=0;
unsigned char 	BERT_STATUS=0, incntr=0, outcntr=0, BERT_STATE=0, Misc_stat37=0;
unsigned long   USER_PAT=0;
unsigned short	LOSCNT=0, OOFCNT=0, YELCNT=0;	// periodically cleared, stored in status[]



unsigned short	PIO2_Content=0;			// default - both A&B at 10M and both 1G LED's OFF and
unsigned short	PIO3_Content=0x0002;	//C3794 lgoic in RESET at boot
//unsigned short	PIO_Direction = ALTERA_AVALON_PIO_DIRECTION_A_OUTPUT | ALTERA_AVALON_PIO_DIRECTION_B_OUTPUT;
volatile int 	pio_edge_capture;// A variable holding the value of the sfp pio edge capture register.

//======= tcl_def's ported to VARIABLES =================
unsigned int gen_number_seconds=30;				// default #secs for quick Test!
//unsigned long number_packet_lsb = 243823;		//# TOTAL Number of packets (for 30secs at def 100M) (1Mframes@1G=~18secs)  4250700300
unsigned long long number_packet_lsb = 243823;		//# TOTAL Number of packets (for 30secs at def 100M) (1Mframes@1G=~18secs)  4250700300


unsigned short number_packet_msb = 0;			// upper 8-bit of 40-bit TOTAL num_pkts
unsigned int MACA_COMMAND_CONFIG = 0x00000003;	// 0x02000013=ENA_10,ENA_TX/RX,PROMIS_on,HD_ENA=off,00:100M 0x00000013
unsigned int MACB_COMMAND_CONFIG = 0x00000003;	// 0x02000013=ENA_10,ENA_TX/RX,PROMIS_on,HD_ENA=off,00:100M 0x00000013
unsigned int MAC10A_COMMAND_CONFIG = 0x00000000;//^0=EnablePromiscuousMode, ^1=EnableRxPauseIgnore
unsigned int MAC_0 = 0;//0x22334450;	// MAC: 50-44-33-22-11-EE
unsigned int MAC_1 = 0;//0x0000EE11;
unsigned int MAC_0B = 0;//0x33445560;	// MAC: 60-55-44-33-11-BB
unsigned int MAC_1B = 0;//0x0000BB11;

// initialized to MAC_0/1 in init.c
unsigned int destination_tse_addr0;
unsigned int destination_tse_addr1;
unsigned int source_tse_addr0;		// usually this IS the same as the MAC (sourcing the EGEN)
unsigned int source_tse_addr1;		// init to Port-A MAC addr (our EGEN)

unsigned char FlashQTX[128];
unsigned char FlashParameters[FlashParameterSize];//={
		/*****
		0x22,0x33,0x44,0x50,0x00,0x00,0xEE,0x11,	// MACA: MAC_0/1
		0x33,0x44,0x55,0x60,0x00,0x00,0xBB,0x11,	// MACB: MAC_0/1B
		0x01,0xA1,0x02,0xB2,0x03,0xC3,0x04,0xD4,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
		};
		******/
unsigned char OPTIONS_INSTALLED=0xFF,OPTIONS_ENABLED=0xFF;// init to all OFF but read from Flash & loaded on boot
//unsigned char SERNUM4=0; // MSbyte (5-bytes total)
//unsigned int  SERNUM3=0; // 3,2,1,0

//unsigned int PHY_COP_CONFIG1_ptr 60	// 16-bits: RESET, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG,
//unsigned int PHY_COP_CONFIG0_ptr 61	//			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
//unsigned int PHY_FBR_CONFIG1_ptr 63	// 16-bits: RESET, POWERDWN, SPEED-2, DPX, ANEG, RESTART_ANEG
//unsigned int PHY_FBR_CONFIG0_ptr 64	// 			ANEG_ADV, LPBK-2, MISC_1G_CNTL-2, MDIX
/****
unsigned int PHY_CONFIG_ETH_SPEED_MASK		0x0003	// 2-bits
	unsigned int speed_10M	0x00	// PHYx_ETH_SPEED: PHY_COP_CONFIG1_ptr & PHY_FBR_CONFIG1_ptr  bit.0-1
	unsigned int speed_100M	0x01
	unsigned int speed_1G	0x02
	unsigned int speed_10G   0x03
unsigned int PHY_CONFIG_FULLDUPLEX_MASK		0x0004	// 1-bit	1= FDX, 0=HDX
unsigned int PHY_CONFIG_ENABLE_AN_MASK		0x0008	// 1-bit	1=enabled
unsigned int PHY_CONFIG_RESTART_ANEG_MASK	0x0010	// 1-bit
unsigned int PHY_CONFIG_ENABLE_MDIX_MASK		0x0020	// 1-bit
unsigned int PHY_CONFIG_MISC_1G_CNTL_MASK	0x0060	// 2-bits
unsigned int PHY_CONFIG_RESET_MASK			0x0080	// 1-bit
unsigned int PHY_CONFIG_ANEG_ADV_MASK		0x0F00	// 4-bits	adv eth support (10,100,1G,FDX, etc)
unsigned int PHY_CONFIG_LOOPBACK_MASK		0x3000	// 2-bits
unsigned int PHY_CONFIG_PWR_DOWN_MASK		0x4000	// 1-bit
 *************/

unsigned char PHYA_ETH_SPEED = speed_100M;	// 0=10M  PHY_COP_CONFIG1_ptr & PHY_FBR_CONFIG1_ptr
unsigned char PHYB_ETH_SPEED = speed_100M;	// 0=10M
_Bool	 PHYA_COPPER_ENABLE_AN	= YES;		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
_Bool	 PHYA_FIBER_ENABLE_AN	= YES;		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
_Bool	 PHYA_COPPER_FULLDUPLEX = YES;		//# FDX = 1 and HDX = 0
_Bool	 PHYA_FIBER_FULLDUPLEX = YES;		//# FDX = 1 and HDX = 0
_Bool	 PHYB_COPPER_ENABLE_AN	= YES;		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
_Bool	 PHYB_FIBER_ENABLE_AN	= YES;		//# Enable PHY Auto-Negotiation (overwritten by EXT_LOOPBACK)
_Bool	 PHYB_COPPER_FULLDUPLEX	= YES;		//# FDX = 1 and HDX = 0
_Bool	 PHYB_FIBER_FULLDUPLEX = YES;		//# FDX = 1 and HDX = 0
unsigned short PHYA_CONFIG = speed_100M | PHY_CONFIG_FULLDUPLEX_MASK | PHY_CONFIG_ENABLE_AN_MASK;
unsigned short PHYB_CONFIG = speed_100M | PHY_CONFIG_FULLDUPLEX_MASK | PHY_CONFIG_ENABLE_AN_MASK;
//unsigned short PHYA_COPPER_CONFIG = speed_100M | PHY_CONFIG_FULLDUPLEX_MASK | PHY_CONFIG_ENABLE_AN_MASK;
//unsigned short PHYB_COPPER_CONFIG = speed_100M | PHY_CONFIG_FULLDUPLEX_MASK | PHY_CONFIG_ENABLE_AN_MASK;
//unsigned short PHYA_FIBER_CONFIG = speed_1G | PHY_CONFIG_FULLDUPLEX_MASK | PHY_CONFIG_ENABLE_AN_MASK; // ...for now
//unsigned short PHYB_FIBER_CONFIG = speed_1G | PHY_CONFIG_FULLDUPLEX_MASK | PHY_CONFIG_ENABLE_AN_MASK; // ...for now

//unsigned long  FPS_PAUSE_COUNT = 10249;
unsigned short MAC_FRM_LENGTH=9500;	//effectively our MTU size!  (was 1518)
//unsigned short egen_pkt_length=1518;	//pkt_length unsigned int <----old, now we uses combined [traffic_gen_pkt_length]
//_Bool	 length_sel=0;				//# Fixed = 0	& Random = 1
//======= TEST RESULT VARIABLES =================
/*_Bool	EGEN_TEST_RUNNING = 0;
double test_time_portA=0, test_time_portB=0;
double test_delaytime_portA=0,test_latencytime_min_portA=2e18,test_latencytime_max_portA=0;
double test_cycle_end_time_portA=0;
unsigned int test_jitter_time_portA=0;
unsigned int test_packets_sent_portA=0, test_packets_sent_portB=0;
double test_emon_bytes_received_portA=0, test_emon_bytes_received_portB=0;
unsigned int test_expected_packets_portA=0,test_valid_packets_portA=0, test_expected_packets_portB=0, test_valid_packets_portB=0;
unsigned int test_error_packets_portA=0, test_error_packets_portB=0;
//double test_data_received_portA=0, test_data_received_portB=0;
//double test_bps_portA=0, test_bps_portB=0;
//double test_linerate_portA=0, test_linerate_portB=0;
//double test_packets_per_second_portA=0, test_packets_per_second_portB=0;*/




//========== 10G variables ===============
_Bool	GIGE10 = NO;	// default to TSE on boot
unsigned int MAC10G_0 = 0, MAC10G_0B = 0;
unsigned int MAC10G_1 = 0, MAC10G_1B = 0;
// initialized to MAC10_0/1 in init.c
unsigned int destination_10g_addr0;//
unsigned int destination_10g_addr1;
unsigned int source_10g_addr0;		// usually this IS the same as the MAC (sourcing the EGEN)
unsigned int source_10g_addr1;		// init to Port-A MAC addr (our EGEN)

//unsigned long  FPS_10G_PAUSE_COUNT = 0;
//unsigned short egen_10G_pkt_length=1518;
unsigned short traffic_gen_pkt_length=1518;
//unsigned short egen_line_load_rate=100;	// 1 to 100 - def 100%.
float egen_line_load_rate=100.0;	// 1 to 100 - def 100%.
unsigned long long total_num_packets=0;
unsigned int  traffic_gen_config = 0x000E05EE;   // noRND,fixedLEN,HDRLEN_ETH,1518 in length
	//unsigned int TRAFFIC_GEN_CONFIG        0x200 //0x80 Config Fields for GENERATOR
	// Structure:     bits
	// CONFIG_PKLEN	 0:15		16-bit packet length 64-9600: 0x40-0x2580 (or mask in RND len modes (e.g. 0xff - clamps size to 255)
	// CONFIG_HDRLEN  16:23		 8-bit header length (header can be 0 to 255 bytes) (note MAC, IPv4, IPv6 overwritten with correct length by GEN)
	// CONFIG_RNDLEN  24		 1-bit Random Length packets mode: 1=enable Random mode, 0=fixed length frames
	// CONFIG_RNDPAY	25		 1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
	// CONFIG_FRCERR	26		 1-bit Force Payload data error (field size bits) DEBUGGING ONLY
	// CONFIG_SPARE   27:31		 5-bits Spares - not currently used
unsigned int  traffic_gen_config2 = 0x0000000C;	// non-IPv4, 802.3frame Length field at byte12
		// CONFG2_LENOFS_SIZ		0:6			// 7 bits: Length field byte offset in HDR
												// in EMIX mode set offset to 1st byte of HDR array (ie 802.3 frame frame_len starts at byte 12 so set this to 12).
		// CONFG2_JUMBO_SIZ		   7			// 1 bit:  jumbo length to 0x8870 handling field size bits
		//unsigned int JUMBO_MASK		0x80
		// CONFG2_HCSOFS_SIZ	8:14		// 7-bits: ipv4 hcs byte offset field size bits
		// CONFG2_SPARE2_SIZ	     15:31		// 17-bits: spare field size bits
unsigned int  traffic_gen_config3 = 0x059C;			// 802.3, 1518=payload len of 1500, 1500-64=1436, 0x5ee-0x40=0x059C
		// CONFIG3_ILEN		0:15			16-bit:  intermediate length for random length recalc field size bits
		// CONFIG3_IHCS	     16:31			16-bit: intermediate HCS for random length HCS recalc field size bits
unsigned int  traffic_gen_config4 = 0x000CFFEE;
// CONFIG4_ILEN		0:15		16 bits: same usage as CONFIG3_ILEN; intermediate length for random length recalc field size bits
		// CONFIG4_LENOFS		16:22		7 bits: same usage as CONFIG2_LENOFS_SIZ
unsigned char SET_SWAP_A_TOGGLE = 0x00;
unsigned char SET_SWAP_B_TOGGLE = 0x01; //B has MAC address swap ON by default.
unsigned char SET_SWAP_10GA_TOGGLE = 0x00;

unsigned long maxRxBustinessVal = 0; // RxBurstiness==Incoming data jittering/bursting so as to overload our TxFIFO

// Loopback variables
unsigned char LOOK_FOR_RESPONSE = 0;	// 3 = hello, 2 = loop-down, 1 = loop-up
_Bool loopup_prev = 0;
_Bool loopdown_prev = 0;
volatile int irq_aggr_capture;
short requestTimer = 5000;
_Bool DETECT_REMOTE_LOOPBACK_TSEA = 0;
_Bool DETECT_REMOTE_LOOPBACK_TSEB = 0;
_Bool DETECT_REMOTE_LOOPBACK_10GA = 0;
unsigned char loopbackStatPtr = 2;
char responseUnits = 0;
char LOOPBACK_EVENT = 0;
_Bool SWAP_IP = NO;
_Bool FRAME_TYPE_LLC = NO;
// Keep track of frame configurations for port B
_Bool ipv4B_active = 0;
//long portB_ipv4_address = 0;
_Bool ipv6B_active = 0;
/*
long portB_ipv6_address0 = 0;
long portB_ipv6_address1 = 0;
long portB_ipv6_address2 = 0;
long portB_ipv6_address3 = 0;
*/



 unsigned char stream_offset = 99; //multistream = 12, single stream = 99
 unsigned short size_offset  = 343; // same for multi and single stream
 unsigned char stream_no =0;	   // single:0 multi:>0
 unsigned char num_of_streams = 1;


 //sending consolidated results
 unsigned long tot_hicycles,tot_locycles=0;
 unsigned long tot_cycles_used,tot_traffic_mon_delay_min,tot_traffic_mon_delay_max,tot_traffic_mon_delay_sum,tot_lobytes,tot_hibytes=0;
 unsigned long long tot_good_pkts_rcvd,tot_bad_pkts_rcvd,tot_good_pkts_rcvd1,tot_bad_pkts_rcvd1,tot_packets_rcvd=0;
 unsigned int tot_packets_to_transmit,tot_packets_act_transmit,tot_packets_to_transmit1,tot_packets_act_transmit1,tot_rx_broadcast_cnt,tot_rx_broadcast_cnt1=0;
 double tot_sumbytes,tot_rcvd_bytes,tot_avg_pkt_size,tot_seconds=0;

unsigned char test_options = 0;
unsigned char emix = 0;
unsigned char vlanTags = 0;
unsigned char mplsTags = 0;
unsigned char rndPayload = 0;
//double burst_packets = 0;
unsigned long long burst_packets = 0;




 _Bool MS_PKT=NO, MS_TEST_ACTIVE=NO, ETH_PKT=NO;// multistream pkt. msconfig = 5470
// single stream packet. msconfig = 445
 _Bool RCVING_MS_PKT = NO, ETH_PACKET = NO;
 _Bool EXTERNAL_PHY_BOOT_COMPLETE;

 unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr = 99	  ; //used to handle the case w
 unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE4_ptr = 100	  ;

 unsigned short TRAFFIC_MON_STREAM_CNTL3_ptr = 	101			;	// MON stream control (for independent capture)
 unsigned short TRAFFIC_MON_STREAM_CNTL2_ptr = 	102			;
 unsigned short TRAFFIC_MON_STREAM_CNTL1_ptr = 	103			;
 unsigned short TRAFFIC_MON_STREAM_CNTL0_ptr = 	104			;

 unsigned short TRAFFIC_RATE_TYPE	=		105	;			// TBD: e.g. 0=FPS%, 1=Gbps, 2=Mbps, 3=Kbps
 unsigned short TRAFFIC_RATE3_ptr =          106;
 unsigned short TRAFFIC_RATE2_ptr =      	107;		 // Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%	;
 unsigned short TRAFFIC_RATE1_ptr =      	108;
 unsigned short TRAFFIC_RATE0_ptr =      	109	;	  //  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M	;

 unsigned short TRAFFIC_GEN_PKTPERCENT11_ptr = 	110			; // 2-byte spares for 100G		;
 unsigned short TRAFFIC_GEN_PKTPERCENT10_ptr = 	111			;
 unsigned short TRAFFIC_GEN_PKTPERCENT03_ptr = 	112			;
 unsigned short TRAFFIC_GEN_PKTPERCENT02_ptr = 	113			;
 unsigned short TRAFFIC_GEN_PKTPERCENT01_ptr = 	114			;
 unsigned short TRAFFIC_GEN_PKTPERCENT00_ptr = 	115			;

 //_______________EGEN_PKT_TYPE_ptr = ____________________				;
// 0000,0000:  ^10:Ethernet, ^32:IP, ^54:TCP/UDP, ^76:FUTURE				;
// 10: 00=std Layer2-Ethernet, 01=Ethernet II, 10=802.3 LLC, 11=802.3SNAP				;
// 32: 00=none, 01=IPv4, 10=IPv6				;
// 54: 00=none, 01=TCP, 10=UDP				;
// 76: future				;
//_________________EGEN_PKT_TYPE_ptr_1____________________				;
// 0000,0000:  ^7654:MPLS, ^3210:VLAN 	(0=none, N=# of VLAN's or MPLS tags)			;
//_________________EGEN_PKT_TYPE_ptr_2____________________				;
//	(8-bits OAM/802.3ag/Y.1731)			;
unsigned short EGEN_PKT_TYPE_ptr = 	116		;
unsigned short EGEN_PKT_TYPE_ptr_1= 	117			;
unsigned short EGEN_PKT_TYPE_ptr_2 =	118			;

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
// packet length will be changed by the generator if it is too small to fit required			;
//   header plus sequence_number/stream/timestamp.				;
//   for ipv4, the header checksum must also be changed.				;
// packet length is one value in a Y.1564 EMIX table for random length mode (see RNDUSR and RNDSEQ below));				;
//=================================================================================				;
//public static  short  TRAFFIC_GEN_CONFIG	0x200	//0x80 Config Fields for GENERATOR		;
// 	Structure:     	bits		;
// CONFIG_PKLEN		0:15		;
// CONFIG_HDRLEN  	16:23		8-bit header length (header can be 0 to 255 bytes)); (note MAC, IPv4, IPv6 overwritten with correct length by GEN));	;
// CONFIG_RNDLEN  	24		 	;
// CONFIG_RNDPAY		25		;	1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
// CONFIG_FRCERR		26		;	1-bit Force Payload data error (field size bits)); DEBUGGING ONLY
// CONFIG_CONTIN   	27		      1-bit Continuous Generate (ignore PACKET_NUM));	;
// CONFIG_BWRANK   	28:31:00		4-bits bandwidth rank: for streams set priority to higher BW above those with lower BW. 0=top, 15=lowest	;
//==================================================================================				;
//	unsigned int  traffic_gen_config2 = 12;	// non-IPv4, 802.3frame Length field at byte12		;
// CONFG2_LENOFS_SIZ		;			// 7 bits: Length field byte offset in HDR
// in EMIX mode set offset to 1st byte of HDR array (ie 802.3 frame frame_len starts at byte 12 so set this to 12).		;
// CONFG2_JUMBO_SIZ		;			// 1 bit:  jumbo length to 0x8870 handling field size bits
//unsigned int JUMBO_MASK		;
// CONFG2_HCSOFS_SIZ	8:14	;	// 7-bits: ipv4 hcs byte offset field size bits
// CONFG2_SPARE2_SIZ	     15:31	;	// 17-bits: spare field size bits
//	unsigned int  traffic_gen_config3 = 0x059C;		;	// 802.3, 1518=payload len of 1500, 1500-64=1436, 0x5ee-0x40=0x059C
// CONFIG3_ILEN		;			16-bit:  intermediate length for random length recalc field size bits
// CONFIG3_IHCS	     16:31	;		16-bit: intermediate HCS for random length HCS recalc field size bits
	;	*/

unsigned short TRAFFIC_GEN_CONFIG33_ptr = 	119			; //Traffic Gen Config 4
unsigned short TRAFFIC_GEN_CONFIG32_ptr = 	120			;
unsigned short TRAFFIC_GEN_CONFIG31_ptr = 	121			;
unsigned short TRAFFIC_GEN_CONFIG30_ptr = 	122			;

unsigned short TRAFFIC_GEN_CONFIG23_ptr = 	123	;	  //TRAFFIC_GEN_CONFIG3	;
unsigned short TRAFFIC_GEN_CONFIG22_ptr = 	124			;
unsigned short TRAFFIC_GEN_CONFIG21_ptr = 	125			;
unsigned short TRAFFIC_GEN_CONFIG20_ptr = 	126			;

unsigned short TRAFFIC_GEN_CONFIG13_ptr = 	127		;
unsigned short TRAFFIC_GEN_CONFIG12_ptr = 	128		; //15:31 // 17-bits: spare field size bits	;
unsigned short TRAFFIC_GEN_CONFIG11_ptr = 	129		;//CONFG2_HCSOFS_SIZ	;
unsigned short TRAFFIC_GEN_CONFIG10_ptr = 	130		;// 7 bits: Length field byte offset in HDR + 1 bit:  jumbo length to 0x8870 handling field size bits	;

unsigned short TRAFFIC_GEN_CONFIG03_ptr = 	131		;// TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed	;
unsigned short TRAFFIC_GEN_CONFIG02_ptr = 	132			;
unsigned short TRAFFIC_GEN_CONFIG01_ptr = 	133			;
unsigned short TRAFFIC_GEN_CONFIG00_ptr = 	134			;


double DEF_EMIX_RANDOM = (4782.0/7.0);
double DEF_GEN_EMIXSEQ0 = 0x0123456F;
double DEF_GEN_EMIXSEQ1 =	0xF0000000;
double DEF_GEN_EMIXSEQ2 = 0xF0000000;
double DEF_GEN_EMIXSEQ3 = 0xF0000000;
double DEF_GEN_EMIXUSER = 0x251C05EE; // U=0x251C, G=0x05EE   (9500 and 1518)
double DEF_GEN_EMIXUSER1 = 0x00000046;//w= 0x46/4E 70/78 //although we pass an int value

unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr = 	135			;//u
unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE2_ptr = 	136			;
unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr = 	137			;//g
unsigned short TRAFFIC_GEN_PKT_EMIXUSERSIZE0_ptr = 	138			;

unsigned short  TRAFFIC_GEN_EMIXSEQ33_ptr =   	139			;
unsigned short  TRAFFIC_GEN_EMIXSEQ32_ptr =   	140			;
unsigned short  TRAFFIC_GEN_EMIXSEQ31_ptr =   	141			;
unsigned short  TRAFFIC_GEN_EMIXSEQ30_ptr =   	142			;
unsigned short  TRAFFIC_GEN_EMIXSEQ23_ptr = 		143		;		//(good to set 1,2&3 def as 0xF0000000)
unsigned short  TRAFFIC_GEN_EMIXSEQ22_ptr =   	144			;
unsigned short  TRAFFIC_GEN_EMIXSEQ21_ptr =   	145			;
unsigned short  TRAFFIC_GEN_EMIXSEQ20_ptr =  		146		;
unsigned short  TRAFFIC_GEN_EMIXSEQ13_ptr = 		147		;
unsigned short  TRAFFIC_GEN_EMIXSEQ12_ptr =   	148			;
unsigned short  TRAFFIC_GEN_EMIXSEQ11_ptr = 		149		;
unsigned short  TRAFFIC_GEN_EMIXSEQ10_ptr = 		150		;
unsigned short  TRAFFIC_GEN_EMIXSEQ03_ptr =  		151		;	//LSNibble sent last
unsigned short  TRAFFIC_GEN_EMIXSEQ02_ptr =  		152		;
unsigned short  TRAFFIC_GEN_EMIXSEQ01_ptr = 		153		;
unsigned short  TRAFFIC_GEN_EMIXSEQ00_ptr =   	154			;


/**************************************************************************************************************************				;
// Config bytes related to Y.1564 & RFC2544 User selectable Test thresholds				;
***************************************************************************************************************************				;
unsigned short  TEST_OPTIONS_ptr =  			XX // Test options to select between (quick test,multistream, y.1564, rfc..) - In the main Config is used to set the different type of test- service configuration/service performance/both)	;
...//...CIR & EIR are 4-bytes (they are rates - like TRAFFICRATE_TYPE above)				;
//RFC2544 will share/overlap these thresholds to allow threshold support for RFC2544 as well	*/			;
unsigned short   Y1564_CIR_3_ptr = 	155			;	 //Committed information rate
unsigned short   Y1564_CIR_2_ptr = 	156			;
unsigned short   Y1564_CIR_1_ptr = 	157			;
unsigned short   Y1564_CIR_0_ptr = 	158			;

unsigned short   Y1564_EIR_3_ptr = 	159			;		//Expected information rate
unsigned short   Y1564_EIR_2_ptr = 	160			;		//Expected information rate
unsigned short   Y1564_EIR_1_ptr = 	161			;		//Expected information rate
unsigned short   Y1564_EIR_0_ptr = 	162			;		//Expected information rate

unsigned short   Y1564_CIR_STEPS_ptr = 	163			;	//Steps for CIR – first 4 bits
unsigned short   Y1564_MAX_FTD_3_ptr =  	164			;
unsigned short   Y1564_MAX_FTD_2_ptr =  	165			;
unsigned short   Y1564_MAX_FTD_1_ptr =  	166			;
unsigned short   Y1564_MAX_FTD_0_ptr =  	167			;

unsigned short   Y1564_MAX_FDV_3_ptr =  	168			;
unsigned short   Y1564_MAX_FDV_2_ptr =  	169			;
unsigned short   Y1564_MAX_FDV_1_ptr =  	170			;
unsigned short   Y1564_MAX_FDV_0_ptr =  	171			;

unsigned short   Y1564_MAX_FRAME_LOSS_3_ptr =  	172		;//4 byte Float	;
unsigned short   Y1564_MAX_FRAME_LOSS_2_ptr =  	173			;
unsigned short   Y1564_MAX_FRAME_LOSS_1_ptr =  	174			;
unsigned short   Y1564_MAX_FRAME_LOSS_0_ptr =  	175			;

unsigned short   Y1564_MAX_AVAIL_3_ptr =  	176			;
unsigned short   Y1564_MAX_AVAIL_2_ptr =  	177			;
unsigned short   Y1564_MAX_AVAIL_1_ptr =  	178			;
unsigned short   Y1564_MAX_AVAIL_0_ptr =  	179			;

unsigned short  SPARE_THRESHOLD_03 =	180			;
unsigned short  SPARE_THRESHOLD_02=	181			;
unsigned short  SPARE_THRESHOLD_01=	182			;
unsigned short  SPARE_THRESHOLD_00=	183			;

unsigned short  TRAFFIC_GEN_SPARE_ptr = 	    184	   ;
unsigned short  TRAFFIC_HDR_DEF_ptr = 		185		;		// additional instructional info for what is contained in HDR ptr = 's


unsigned short  TRAFFIC_HDR0_3_ptr = 	186			; //MSB Src MAC Addr
unsigned short  TRAFFIC_HDR0_2_ptr = 	187			;
unsigned short  TRAFFIC_HDR0_1_ptr = 	188			;
unsigned short  TRAFFIC_HDR0_0_ptr = 	189			;
unsigned short  TRAFFIC_HDR1_3_ptr = 	190			; //LSB
unsigned short  TRAFFIC_HDR1_2_ptr = 	191			;
unsigned short  TRAFFIC_HDR1_1_ptr = 	192			; //MSB Dest MAC Addr
unsigned short  TRAFFIC_HDR1_0_ptr = 	193			;
unsigned short  TRAFFIC_HDR2_3_ptr = 	194			;
unsigned short  TRAFFIC_HDR2_2_ptr = 	195			;
unsigned short  TRAFFIC_HDR2_1_ptr = 	196			; //LSB
unsigned short  TRAFFIC_HDR2_0_ptr = 	197			;
unsigned short  TRAFFIC_HDR3_3_ptr = 	198			;
unsigned short  TRAFFIC_HDR3_2_ptr = 	199			;
unsigned short  TRAFFIC_HDR3_1_ptr = 	200			;
unsigned short  TRAFFIC_HDR3_0_ptr = 	201			;
unsigned short  TRAFFIC_HDR4_3_ptr = 	202			;
unsigned short  TRAFFIC_HDR4_2_ptr = 	203			;
unsigned short  TRAFFIC_HDR4_1_ptr = 	204			;
unsigned short  TRAFFIC_HDR4_0_ptr = 	205			;
unsigned short  TRAFFIC_HDR5_3_ptr = 	206			;
unsigned short  TRAFFIC_HDR5_2_ptr = 	207			;
unsigned short  TRAFFIC_HDR5_1_ptr = 	208			;
unsigned short  TRAFFIC_HDR5_0_ptr = 	209			;
unsigned short  TRAFFIC_HDR6_3_ptr = 	210			;
unsigned short  TRAFFIC_HDR6_2_ptr = 	211			;
unsigned short  TRAFFIC_HDR6_1_ptr = 	212			;
unsigned short  TRAFFIC_HDR6_0_ptr = 	213			;
unsigned short  TRAFFIC_HDR7_3_ptr = 	214			;
unsigned short  TRAFFIC_HDR7_2_ptr = 	215			;
unsigned short  TRAFFIC_HDR7_1_ptr = 	216			;
unsigned short  TRAFFIC_HDR7_0_ptr = 	217			;
unsigned short  TRAFFIC_HDR8_3_ptr = 	218			;
unsigned short  TRAFFIC_HDR8_2_ptr = 	219			;
unsigned short  TRAFFIC_HDR8_1_ptr = 	220			;
unsigned short  TRAFFIC_HDR8_0_ptr = 	221			;
unsigned short  TRAFFIC_HDR9_3_ptr = 	222			;
unsigned short  TRAFFIC_HDR9_2_ptr = 	223			;
unsigned short  TRAFFIC_HDR9_1_ptr = 	224			;
unsigned short  TRAFFIC_HDR9_0_ptr = 	225			;
unsigned short  TRAFFIC_HDR10_3_ptr = 	226			;
unsigned short  TRAFFIC_HDR10_2_ptr = 	227			;
unsigned short  TRAFFIC_HDR10_1_ptr = 	228			;
unsigned short  TRAFFIC_HDR10_0_ptr = 	229			;
unsigned short  TRAFFIC_HDR11_3_ptr = 	230			;
unsigned short  TRAFFIC_HDR11_2_ptr = 	231			;
unsigned short  TRAFFIC_HDR11_1_ptr = 	232			;
unsigned short  TRAFFIC_HDR11_0_ptr = 	233			;
unsigned short  TRAFFIC_HDR12_3_ptr = 	234			;
unsigned short  TRAFFIC_HDR12_2_ptr = 	235			;
unsigned short  TRAFFIC_HDR12_1_ptr = 	236			;
unsigned short  TRAFFIC_HDR12_0_ptr = 	237			;
unsigned short  TRAFFIC_HDR13_3_ptr = 	238			;
unsigned short  TRAFFIC_HDR13_2_ptr = 	239			;
unsigned short  TRAFFIC_HDR13_1_ptr = 	240			;
unsigned short  TRAFFIC_HDR13_0_ptr = 	241			;
unsigned short  TRAFFIC_HDR14_3_ptr = 	242			;
unsigned short  TRAFFIC_HDR14_2_ptr = 	243			;
unsigned short  TRAFFIC_HDR14_1_ptr = 	244			;
unsigned short  TRAFFIC_HDR14_0_ptr = 	245			;
unsigned short  TRAFFIC_HDR15_3_ptr = 	246			;
unsigned short  TRAFFIC_HDR15_2_ptr = 	247			;
unsigned short  TRAFFIC_HDR15_1_ptr = 	248			;
unsigned short  TRAFFIC_HDR15_0_ptr = 	249			;
unsigned short  TRAFFIC_HDR16_3_ptr = 	250			;
unsigned short  TRAFFIC_HDR16_2_ptr = 	251			;
unsigned short  TRAFFIC_HDR16_1_ptr = 	252			;
unsigned short  TRAFFIC_HDR16_0_ptr = 	253			;
unsigned short  TRAFFIC_HDR17_3_ptr = 	254			;
unsigned short  TRAFFIC_HDR17_2_ptr = 	255			;
unsigned short  TRAFFIC_HDR17_1_ptr = 	256			;
unsigned short  TRAFFIC_HDR17_0_ptr = 	257			;
unsigned short  TRAFFIC_HDR18_3_ptr = 	258			;
unsigned short  TRAFFIC_HDR18_2_ptr = 	259			;
unsigned short  TRAFFIC_HDR18_1_ptr = 	260			;
unsigned short  TRAFFIC_HDR18_0_ptr = 	261			;
unsigned short  TRAFFIC_HDR19_3_ptr = 	262			;
unsigned short  TRAFFIC_HDR19_2_ptr = 	263			;
unsigned short  TRAFFIC_HDR19_1_ptr = 	264			;
unsigned short  TRAFFIC_HDR19_0_ptr = 	265			;
unsigned short  TRAFFIC_HDR20_3_ptr = 	266			;
unsigned short  TRAFFIC_HDR20_2_ptr = 	267			;
unsigned short  TRAFFIC_HDR20_1_ptr = 	268			;
unsigned short  TRAFFIC_HDR20_0_ptr = 	269			;
unsigned short  TRAFFIC_HDR21_3_ptr = 	270			;
unsigned short  TRAFFIC_HDR21_2_ptr = 	271			;
unsigned short  TRAFFIC_HDR21_1_ptr = 	272			;
unsigned short  TRAFFIC_HDR21_0_ptr = 	273			;
unsigned short  TRAFFIC_HDR22_3_ptr = 	274			;
unsigned short  TRAFFIC_HDR22_2_ptr = 	275			;
unsigned short  TRAFFIC_HDR22_1_ptr = 	276			;
unsigned short  TRAFFIC_HDR22_0_ptr = 	277			;
unsigned short  TRAFFIC_HDR23_3_ptr = 	278			;
unsigned short  TRAFFIC_HDR23_2_ptr = 	279			;
unsigned short  TRAFFIC_HDR23_1_ptr = 	280			;
unsigned short  TRAFFIC_HDR23_0_ptr = 	281			;
unsigned short  TRAFFIC_HDR24_3_ptr = 	282			;
unsigned short  TRAFFIC_HDR24_2_ptr = 	283			;
unsigned short  TRAFFIC_HDR24_1_ptr = 	284			;
unsigned short  TRAFFIC_HDR24_0_ptr = 	285			;
unsigned short  TRAFFIC_HDR25_3_ptr = 	286			;
unsigned short  TRAFFIC_HDR25_2_ptr = 	287			;
unsigned short  TRAFFIC_HDR25_1_ptr = 	288			;
unsigned short  TRAFFIC_HDR25_0_ptr = 	289			;
unsigned short  TRAFFIC_HDR26_3_ptr = 	290			;
unsigned short  TRAFFIC_HDR26_2_ptr = 	291			;
unsigned short  TRAFFIC_HDR26_1_ptr = 	292			;
unsigned short  TRAFFIC_HDR26_0_ptr = 	293			;
unsigned short  TRAFFIC_HDR27_3_ptr = 	294			;
unsigned short  TRAFFIC_HDR27_2_ptr = 	295			;
unsigned short  TRAFFIC_HDR27_1_ptr = 	296			;
unsigned short  TRAFFIC_HDR27_0_ptr = 	297			;
unsigned short  TRAFFIC_HDR28_3_ptr = 	298			;
unsigned short  TRAFFIC_HDR28_2_ptr = 	299			;
unsigned short  TRAFFIC_HDR28_1_ptr = 	300			;
unsigned short  TRAFFIC_HDR28_0_ptr = 	301			;
unsigned short  TRAFFIC_HDR29_3_ptr = 	302			;
unsigned short  TRAFFIC_HDR29_2_ptr = 	303			;
unsigned short  TRAFFIC_HDR29_1_ptr = 	304			;
unsigned short  TRAFFIC_HDR29_0_ptr = 	305			;
unsigned short  TRAFFIC_HDR30_3_ptr = 	306			;
unsigned short  TRAFFIC_HDR30_2_ptr = 	307			;
unsigned short  TRAFFIC_HDR30_1_ptr = 	308			;
unsigned short  TRAFFIC_HDR30_0_ptr = 	309			;
unsigned short  TRAFFIC_HDR31_3_ptr = 	310			;
unsigned short  TRAFFIC_HDR31_2_ptr = 	311			;
unsigned short  TRAFFIC_HDR31_1_ptr = 	312			;
unsigned short  TRAFFIC_HDR31_0_ptr = 	313			;

unsigned short  TRAFFIC_MSK0_3_ptr = 	314			;
unsigned short  TRAFFIC_MSK0_2_ptr = 	315			;
unsigned short  TRAFFIC_MSK0_1_ptr = 	316			;
unsigned short  TRAFFIC_MSK0_0_ptr = 	317			;
unsigned short  TRAFFIC_MSK1_3_ptr = 	318			;
unsigned short  TRAFFIC_MSK1_2_ptr = 	319			;
unsigned short  TRAFFIC_MSK1_1_ptr = 	320			;
unsigned short  TRAFFIC_MSK1_0_ptr = 	321			;
unsigned short  TRAFFIC_MSK2_3_ptr = 	322			;
unsigned short  TRAFFIC_MSK2_2_ptr = 	323			;
unsigned short  TRAFFIC_MSK2_1_ptr = 	324			;
unsigned short  TRAFFIC_MSK2_0_ptr = 	325			;
unsigned short  TRAFFIC_MSK3_3_ptr = 	326			;
unsigned short  TRAFFIC_MSK3_2_ptr = 	327			;
unsigned short  TRAFFIC_MSK3_1_ptr = 	328			;
unsigned short  TRAFFIC_MSK3_0_ptr = 	329			;
unsigned short  TRAFFIC_MSK4_3_ptr = 	330			;
unsigned short  TRAFFIC_MSK4_2_ptr = 	331			;
unsigned short  TRAFFIC_MSK4_1_ptr = 	332			;
unsigned short  TRAFFIC_MSK4_0_ptr = 	333			;
unsigned short  TRAFFIC_MSK5_3_ptr = 	334			;
unsigned short  TRAFFIC_MSK5_2_ptr = 	335			;
unsigned short  TRAFFIC_MSK5_1_ptr = 	336			;
unsigned short  TRAFFIC_MSK5_0_ptr = 	337			;
unsigned short  TRAFFIC_MSK6_3_ptr = 	338			;
unsigned short  TRAFFIC_MSK6_2_ptr = 	339			;
unsigned short  TRAFFIC_MSK6_1_ptr = 	340			;
unsigned short  TRAFFIC_MSK6_0_ptr = 	341			;
unsigned short  TRAFFIC_MSK7_3_ptr = 	342			;
unsigned short  TRAFFIC_MSK7_2_ptr = 	343			;
unsigned short  TRAFFIC_MSK7_1_ptr = 	344			;
unsigned short  TRAFFIC_MSK7_0_ptr = 	345			;
unsigned short  TRAFFIC_MSK8_3_ptr = 	346			;
unsigned short  TRAFFIC_MSK8_2_ptr = 	347			;
unsigned short  TRAFFIC_MSK8_1_ptr = 	348			;
unsigned short  TRAFFIC_MSK8_0_ptr = 	349			;
unsigned short  TRAFFIC_MSK9_3_ptr = 	350			;
unsigned short  TRAFFIC_MSK9_2_ptr = 	351			;
unsigned short  TRAFFIC_MSK9_1_ptr = 	352			;
unsigned short  TRAFFIC_MSK9_0_ptr = 	353			;
unsigned short  TRAFFIC_MSK10_3_ptr = 	354			;
unsigned short  TRAFFIC_MSK10_2_ptr = 	355			;
unsigned short  TRAFFIC_MSK10_1_ptr = 	356			;
unsigned short  TRAFFIC_MSK10_0_ptr = 	357			;
unsigned short  TRAFFIC_MSK11_3_ptr = 	358			;
unsigned short  TRAFFIC_MSK11_2_ptr = 	359			;
unsigned short  TRAFFIC_MSK11_1_ptr = 	360			;
unsigned short  TRAFFIC_MSK11_0_ptr = 	361			;
unsigned short  TRAFFIC_MSK12_3_ptr = 	362			;
unsigned short  TRAFFIC_MSK12_2_ptr = 	363			;
unsigned short  TRAFFIC_MSK12_1_ptr = 	364			;
unsigned short  TRAFFIC_MSK12_0_ptr = 	365			;
unsigned short  TRAFFIC_MSK13_3_ptr = 	366			;
unsigned short  TRAFFIC_MSK13_2_ptr = 	367			;
unsigned short  TRAFFIC_MSK13_1_ptr = 	368			;
unsigned short  TRAFFIC_MSK13_0_ptr = 	369			;
unsigned short  TRAFFIC_MSK14_3_ptr = 	370			;
unsigned short  TRAFFIC_MSK14_2_ptr = 	371			;
unsigned short  TRAFFIC_MSK14_1_ptr = 	372			;
unsigned short  TRAFFIC_MSK14_0_ptr = 	373			;
unsigned short  TRAFFIC_MSK15_3_ptr = 	374			;
unsigned short  TRAFFIC_MSK15_2_ptr = 	375			;
unsigned short  TRAFFIC_MSK15_1_ptr = 	376			;
unsigned short  TRAFFIC_MSK15_0_ptr = 	377			;
unsigned short  TRAFFIC_MSK16_3_ptr = 	378			;
unsigned short  TRAFFIC_MSK16_2_ptr = 	379			;
unsigned short  TRAFFIC_MSK16_1_ptr = 	380			;
unsigned short  TRAFFIC_MSK16_0_ptr = 	381			;
unsigned short  TRAFFIC_MSK17_3_ptr = 	382			;
unsigned short  TRAFFIC_MSK17_2_ptr = 	383			;
unsigned short  TRAFFIC_MSK17_1_ptr = 	384			;
unsigned short  TRAFFIC_MSK17_0_ptr = 	385			;
unsigned short  TRAFFIC_MSK18_3_ptr = 	386			;
unsigned short  TRAFFIC_MSK18_2_ptr = 	387			;
unsigned short  TRAFFIC_MSK18_1_ptr = 	388			;
unsigned short  TRAFFIC_MSK18_0_ptr = 	389			;
unsigned short  TRAFFIC_MSK19_3_ptr = 	390			;
unsigned short  TRAFFIC_MSK19_2_ptr = 	391			;
unsigned short  TRAFFIC_MSK19_1_ptr = 	392			;
unsigned short  TRAFFIC_MSK19_0_ptr = 	393			;
unsigned short  TRAFFIC_MSK20_3_ptr = 	394			;
unsigned short  TRAFFIC_MSK20_2_ptr = 	395			;
unsigned short  TRAFFIC_MSK20_1_ptr = 	396			;
unsigned short  TRAFFIC_MSK20_0_ptr = 	397			;
unsigned short  TRAFFIC_MSK21_3_ptr = 	398			;
unsigned short  TRAFFIC_MSK21_2_ptr = 	399			;
unsigned short  TRAFFIC_MSK21_1_ptr = 	400			;
unsigned short  TRAFFIC_MSK21_0_ptr = 	401			;
unsigned short  TRAFFIC_MSK22_3_ptr = 	402			;
unsigned short  TRAFFIC_MSK22_2_ptr = 	403			;
unsigned short  TRAFFIC_MSK22_1_ptr = 	404			;
unsigned short  TRAFFIC_MSK22_0_ptr = 	405			;
unsigned short  TRAFFIC_MSK23_3_ptr = 	406			;
unsigned short  TRAFFIC_MSK23_2_ptr = 	407			;
unsigned short  TRAFFIC_MSK23_1_ptr = 	408			;
unsigned short  TRAFFIC_MSK23_0_ptr = 	409			;
unsigned short  TRAFFIC_MSK24_3_ptr = 	410			;
unsigned short  TRAFFIC_MSK24_2_ptr = 	411			;
unsigned short  TRAFFIC_MSK24_1_ptr = 	412			;
unsigned short  TRAFFIC_MSK24_0_ptr = 	413			;
unsigned short  TRAFFIC_MSK25_3_ptr =  414				;
unsigned short  TRAFFIC_MSK25_2_ptr = 	415			;
unsigned short  TRAFFIC_MSK25_1_ptr =  416				;
unsigned short  TRAFFIC_MSK25_0_ptr = 	417			;
unsigned short  TRAFFIC_MSK26_3_ptr = 	418			;
unsigned short  TRAFFIC_MSK26_2_ptr = 	419			;
unsigned short  TRAFFIC_MSK26_1_ptr = 	420			;
unsigned short  TRAFFIC_MSK26_0_ptr = 	421			;
unsigned short  TRAFFIC_MSK27_3_ptr = 	422			;
unsigned short  TRAFFIC_MSK27_2_ptr = 	423			;
unsigned short  TRAFFIC_MSK27_1_ptr = 	424			;
unsigned short  TRAFFIC_MSK27_0_ptr = 	425			;
unsigned short  TRAFFIC_MSK28_3_ptr = 	426			;
unsigned short  TRAFFIC_MSK28_2_ptr = 	427			;
unsigned short  TRAFFIC_MSK28_1_ptr = 	428			;
unsigned short  TRAFFIC_MSK28_0_ptr = 	429			;
unsigned short  TRAFFIC_MSK29_3_ptr = 	430			;
unsigned short  TRAFFIC_MSK29_2_ptr = 	431			;
unsigned short  TRAFFIC_MSK29_1_ptr = 	432			;
unsigned short  TRAFFIC_MSK29_0_ptr = 	433			;
unsigned short  TRAFFIC_MSK30_3_ptr = 	434			;
unsigned short  TRAFFIC_MSK30_2_ptr = 	435			;
unsigned short  TRAFFIC_MSK30_1_ptr = 	436			;
unsigned short  TRAFFIC_MSK30_0_ptr = 	437			;
unsigned short  TRAFFIC_MSK31_3_ptr = 	438			;
unsigned short  TRAFFIC_MSK31_2_ptr = 	439			;
unsigned short  TRAFFIC_MSK31_1_ptr = 	440			;
unsigned short  TRAFFIC_MSK31_0_ptr = 	441			;
unsigned short ConfigChecksum2 = 444;
#endif /* VARS_H_ */
/*************************************************************************
* Copyright (c) 2012 GUISYS CORPORATION, Campton Hills, Illinois USA.    *
* All rights reserved.                                                   *
*************************************************************************/
