
//==============================================================================
//	TRAFFIC_CONTROLLER (Generator & Monitor) Map
//==============================================================================
#define TRAFFIC_GEN_NUM_PACKET	0x00
#define TRAFFIC_GEN_NUM_PACKET1	0x04 //0x01
#define TRAFFIC_GEN_STREAM_KEY		0x08 //0x02 (0=1 stream, 1=2streams, etc.)
#define TRAFFIC_GEN_START			0x0C //0x03 1-bit, 1=START generator!
#define TRAFFIC_GEN_STOP			0x10 //0x04 1-bit, 1=STOP generator!
#define TRAFFIC_GEN_PKT_TX_CNT    	0x24 //0x09 RD only (# Tx'd frames)
#define TRAFFIC_GEN_PKT_TX_CNT1	0x28 //0x0A

#define TRAFFIC_GEN_CONFIG	        0x200	//0x80 Config Fields for GENERATOR
// packet length will be changed by the generator if it is too small to fit required
//   header plus sequence_number/stream/timestamp.
//   for ipv4, the header checksum must also be changed.
// packet length is one value in a Y.1564 EMIX table for random length mode (see RNDUSR and RNDSEQ below)
//=================================================================================
//#define TRAFFIC_GEN_CONFIG	0x200	//0x80 Config Fields for GENERATOR
// 	Structure:     	bits
// CONFIG_PKLEN		0:15		16-bit packet length 64-9600: 0x40-0x2580 (or mask in RND len modes (e.g. 0xff - clamps size to 255)
// CONFIG_HDRLEN  	16:23		8-bit header length (header can be 0 to 255 bytes) (note MAC, IPv4, IPv6 overwritten with correct length by GEN)
// CONFIG_RNDLEN  	24		 	1-bit Random Length packets mode: 1=enable Random mode, 0=fixed length frames
// CONFIG_RNDPAY		25			1-bit Random Payload mode:   1=random payload contents, 0=incrementing payload
// CONFIG_FRCERR		26			1-bit Force Payload data error (field size bits) DEBUGGING ONLY
// CONFIG_CONTIN   	27		      1-bit Continuous Generate (ignore PACKET_NUM)
// CONFIG_BWRANK   	28:31		4-bits bandwidth rank: for streams set priority to higher BW above those with lower BW. 0=top, 15=lowest
//==================================================================================
		#define LEN_MASK			0xFFFF
		#define HDRLEN_ETH		0x0E0000		//14	0E
		#define HDRLEN_VLAN		0x120000		//18	12
		#define HDRLEN_SVLAN		0x160000		//22	16
		#define HDRLEN_IP4		0x220000		//34	22
		#define HDRLEN_VLANIP4	0x260000		//38	26
		#define HDRLEN_SVLANIP4	0x2A0000		//42	2A
		#define HDRLEN_IP6		0x360000		//54	36
		#define HDRLEN_VLANIP6	0x3A0000		//58	3A
		#define HDRLEN_SVLANIP6	0x3E0000		//62	3E
		#define HDRLEN_MASK		0xFF0000		//62	3E
		#define RNDLEN_ON	   	0x01000000
		#define RNDPAY_ON	   	0x02000000
		#define FRCERR_ON	   	0x04000000
		#define CONTINUOUS_ON 	0x08000000
		#define BWRANK_MASK 	0xF0000000


#define TRAFFIC_GEN_RNDSEED	  	0x204	//0x81 Random Seed Config Fields
		#define TRAFFIC_GEN_RNDSEED_DEF	0xF00D5EED	//using the old GEN verilog def
#define TRAFFIC_GEN_PKTPERCENT	 	0x208	//0x82 Packet Period Count (number of clks between packet starts)

#define TRAFFIC_GEN_EMIXUSERSIZE	0x20C	//0x83 EMIX/random user packet length register (gen.v:ADDR_RNDUSR) - RNDUSR
#define TRAFFIC_GEN_EMIXSEQ0	  	0x210	//0x84EMIX/random sequence list register 0  MSNibble sent 1st - RNDSEQ0
#define TRAFFIC_GEN_EMIXSEQ1		0x214	//0x85							      - RNDSEQ1
#define TRAFFIC_GEN_EMIXSEQ2		0x218	//0x86  (up to 32 sizes)  (RNDSEQ0 = 0x01234F00 is a 5-size sequence: 64,128,256,512,1024) - RNDSEQ2
#define TRAFFIC_GEN_EMIXSEQ3		0x21C	//0x87	EMIX/random sequence list register 3  LSNibble sent last - RNDSEQ3
		// 0     a    64
		// 1     b    128
		// 2     c    256
		// 3     d    512
		// 4     e    1024
		// 5     f    1280
		// 6     g    RNDUSR_G that should be 1518, 1522, 1526 etc depending on vlan tags etc
		// 7     h    MTU uses PKTLEN
		// 8     u    RNDUSR_U
		#define DEF_EMIX_RANDOM		 (4782.0/7.0) // default EMIX average, 0x0123456F: 64,128,256,512,1024,1280,1518 = 4782/7
		#define DEF_GEN_EMIXUSER	0x258005EE		// U=2580, G=05EE
		#define DEBUG_GEN_EMIXUSER	0x05EF0041		// U=1519, G=65
		#define DEF_GEN_EMIXSEQ0	0x0123456F		//MEF: 0x01234567  7=MTU our's is 9600 ignore for now 64-1518
		#define DEF_GEN_EMIXSEQ1	0xF0000000
		#define DEF_GEN_EMIXSEQ2	0xF0000000
		#define DEF_GEN_EMIXSEQ3	0xF0000000
		#define TRAFFIC_GEN_CONFIG2	  		0X220 //0x88 -
// CONFG2_LENOFS_SIZ		0:6			// 7 bits: Length field byte offset in HDR
										// in EMIX mode set offset to 1st byte of HDR array (ie 802.3 frame frame_len starts at byte 12 so set this to 12).
// CONFG2_JUMBO_SIZ		   7			// 1 bit:  jumbo length to 0x8870 handling field size bits
// CONFG2_HCSOFS_SIZ	8:14		// 7-bits: ipv4 hcs byte offset field size bits
// CONFG2_SPARE2_SIZ	     15:31		// 17-bits: spare field size bits
		#define LENOFS_MASK		0x007F	// Payload 1st byte location (0 is 1st byte of HDR0)
		#define JUMBO_MASK		0x0080
		#define HCSOFF_MASK		0x7F00

#define TRAFFIC_GEN_CONFIG3		0X224 // 0x89
// CONFIG3_ILEN		0:15			16-bit:  intermediate length for random length recalc field size bits
// CONFIG3_IHCS	     16:31			16-bit: intermediate HCS for random length HCS recalc field size bits
		#define ILEN_MASK			0x0000FFFF	// ILEN = payload length - 64, (payload length=pkt-len-18)
		#define IHCS_MASK			0xFFFF0000

#define TRAFFIC_GEN_ERROR      	0x228 // 1-bit: 1=insert error
#define TRAFFIC_GEN_VERSION    	0X22C // RD only to see what version of GEN we're using

#define TRAFFIC_GEN_RUNNING	0x03C0 //0xF0 new: 1-bit: 1=RUNNING, 0=IDLE
//=================================================
   // the header length above determines how much of the header registers are used.
   // note that the length in MAC or ipv4 or ipv6 or etc. is only overwritten for random length packets.
   // for ipv4, the header checksum must also be overwritten, because the length changed.
   // the maximum header is 128 bytes = 32 words * 8 bytes per word.
#define TRAFFIC_GEN_HDR0	  0x0300  //new
#define TRAFFIC_GEN_HDR1	  0x0304  //new
#define TRAFFIC_GEN_HDR2	  0x0308  //new
#define TRAFFIC_GEN_HDR3	  0x030C  //new
#define TRAFFIC_GEN_HDR4	  0x0310  //new
#define TRAFFIC_GEN_HDR5	  0x0314  //new
#define TRAFFIC_GEN_HDR6	  0x0318  //new
#define TRAFFIC_GEN_HDR7	  0x031C  //new
#define TRAFFIC_GEN_HDR8	  0x0320  //new
#define TRAFFIC_GEN_HDR9	  0x0324  //new
#define TRAFFIC_GEN_HDR10	  0x0328  //new
#define TRAFFIC_GEN_HDR11	  0x032C  //new
#define TRAFFIC_GEN_HDR12	  0x0330  //new
#define TRAFFIC_GEN_HDR13	  0x0334  //new
#define TRAFFIC_GEN_HDR14	  0x0338  //new
#define TRAFFIC_GEN_HDR15	  0x033C  //new   
#define TRAFFIC_GEN_HDR16	  0x0340  //new
#define TRAFFIC_GEN_HDR17	  0x0344  //new
#define TRAFFIC_GEN_HDR18	  0x0348  //new
#define TRAFFIC_GEN_HDR19	  0x034C  //new
#define TRAFFIC_GEN_HDR20	  0x0350  //new
#define TRAFFIC_GEN_HDR21	  0x0354  //new
#define TRAFFIC_GEN_HDR22	  0x0358  //new
#define TRAFFIC_GEN_HDR23	  0x035C  //new
#define TRAFFIC_GEN_HDR24	  0x0360  //new
#define TRAFFIC_GEN_HDR25	  0x0364  //new
#define TRAFFIC_GEN_HDR26	  0x0368  //new
#define TRAFFIC_GEN_HDR27	  0x036C  //new
#define TRAFFIC_GEN_HDR28	  0x0370  //new
#define TRAFFIC_GEN_HDR29	  0x0374  //new
#define TRAFFIC_GEN_HDR30	  0x0378  //new
#define TRAFFIC_GEN_HDR31	  0x037C  //new
//======================================================================================
// Traffic Controller Monitor Registers Offset Definitions
//--------------------------------------------------------------------------------------
#define TRAFFIC_MON_NUM_PACKET    			  0x0000	//0x00
#define TRAFFIC_MON_PKT_RX_OK     			  0x0004	//0x01
#define TRAFFIC_MON_PKT_RX_ERR    			  0x0008	//0x02
#define TRAFFIC_MON_BYTE_RX_CNT0  			  0x000C	//0x03
#define TRAFFIC_MON_BYTE_RX_CNT1  			  0x0010	//0x04
#define TRAFFIC_MON_CYCLE_RX_CNT0 		  0x0014	//0x05
#define TRAFFIC_MON_CYCLE_RX_CNT1 		  0x0018	//0x06
#define TRAFFIC_MON_RX_CTRL_STAT			  0x001C  //0x07 ^0=1 clr cntrs & Start mon, ^1=Stop, ^2=1 rcv_done, ^3=error
		#define TRAFFIC_MON_START_BIT	                        				0x01 // start&clr bit
		#define TRAFFIC_MON_RECEIVE_START_AND_CLEAR_CNTRS_BIT	0x01 // start&clr bit
		#define TRAFFIC_MON_RECEIVE_STOP_BIT							0x02 // STOP bit
		#define TRAFFIC_MON_RECEIVE_CONTROL_DONE_STATUS_BIT		0x04 // DONE bit
		#define TRAFFIC_MON_RECEIVE_ERROR_STATUS_BIT				0x08 // rcv error flag
		// TRAFFIC_RX_CTRL_STAT_CODE		4:9			6-bit:  rcv error code
		#define ERROR_CODE_MASK									0x03F0
		// TRAFFIC_RX_CTRL_SPARE			10:14		6-bit: spare - not used
		#define TRAFFIC_MON_ACTIVE_BIT                          			0x0008000
		#define TRAFFIC_MON_RECEIVE_CONTINUOUS_BIT				0x0010000 // RCV Continuous (^16)
		#define TRAFFIC_MON_RESET_BIT	                        			0x80000000 //RESET bit

		// TRAFFIC_RX_CTRL_SPARE1			17:31		15-bit: spare - not used
#define TRAFFIC_MON_CYCLE_START_RX_CNT0		0x0020	//0x08 new mbp
#define TRAFFIC_MON_CYCLE_START_RX_CNT1		0x0024	//0x09 new mbp
#define TRAFFIC_MON_CYCLE_END_RX_CNT0 		0x0028	//0x0A new mbp
#define TRAFFIC_MON_CYCLE_END_RX_CNT1 		0x002C	//0x0B new mbp
#define TRAFFIC_MON_BROADCAST_RX_CNT 		0x0030	//0x0C new mbp
#define TRAFFIC_MON_RX_DEST_MAC_ADDR0 		0x0034	//0x0D new mbp - this is what the Verilog mon captured
#define TRAFFIC_MON_RX_DEST_MAC_ADDR1 		0x0038	//0x0E new mbp
//================ New Addresses ========================= 
#define TRAFFIC_MON_NUM_MONINIT    	        	0x003C	//new:8'h0F; not used
#define TRAFFIC_MON_NUM_MONACTIVE  	        	0x0040	//new:8'h10; not used
#define TRAFFIC_MON_NUM_MONDONE    	       	0x0044	//new:8'h11; not used
#define TRAFFIC_MON_NUM_MONNONE    	        	0x0048	//new:8'h12; not used
#define TRAFFIC_MON_PKT_RX_MAC_ERR	        	0x004C  // count of packets with errors detected by the mac and
#define TRAFFIC_MON_PKT_RX_IPV4HCS_ERR      	0x0050  // count of packets with ipv4 hcs errors (unused)
#define TRAFFIC_MON_PKT_RX_EOP_ERR          		0x0054  // count of packets without a valid stream id from pkt
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR      	0x0058  // count of packets with a timeout error
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR      	0x005C  //0X17 X4: count of packets with no stream header match and not a
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR  0x0060  // count of packets with multiple stream header matches
#define TRAFFIC_MON_STREAM_KEY              		0x0064  // stream key, which stream is the nios accessing
		#define STREAM_KEY_INDEX_MASK				0x000000FF		//  8-bit: Stream index for HDR field
		#define STREAM_KEY_SPARE					0x00FFFF00		//16-bit: spare - not used
		#define STREAM_KEY_NUM_STREAMS_MASK		0xFF000000		//  8-bit: number of Streams configured (read only)

#define TRAFFIC_MON_NUM_PACKET1             		0x006c  // upper bits of the corresponding count
#define TRAFFIC_MON_STREAM_CNTL             		0x0068  // stream control bits, payload_start + stream_enable ;
		#define STREAM_CNTL_ENABLE_MASK			0x00000001		// 1-bit:  Stream Enable bit
		#define STREAM_CNTL_SPARE1_MASK			0x000000FE		// 7-bit: spare - not used
		#define STREAM_CNTL_HDRLEN_MASK			0x0003FF00		//10-bit: Stream header length
		#define STREAM_CNTL_SPARE2_MASK			0xFFFC0000		//14-bit: spare - not used

#define TRAFFIC_MON_PKT_RX_OK1              		0x0070  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_ERR1             		0x0074  // upper bits of the corresponding count
#define TRAFFIC_MON_BROADCAST_RX_CNT1       	0x0078  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_MAC_ERR1        	0x007c  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_EOP_ERR1        		0x0080  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_TIMEOUT_ERR1     	0x0084  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_NOMATCH_ERR1    	0x0088  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_MULTIMATCH_ERR1	0x008c  // upper bits of the corresponding count
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT     	0x0098  //26 number packets left in run
#define TRAFFIC_MON_PKT_RX_NUM_PKT_LEFT1    	0x0099  //27 upper bits of the corresponding

//===== currently defined ===================================    
#define TRAFFIC_MON_ADDR_RNDSEED   	       	0x0204	//0x81 random seed fields
//==========================================================================
		#define TRAFFIC_MON_RNDSEED_DEF	     	 0x5EED	// DBG and upper 15 bits all 0's
//_________________________________________________________________________
//#define TRAFFIC_MON_ADDR_RNDSEED  0x0204	//0x81 random seed fields
// Structure:     	   bits
// RNDSEED_PAY_SIZ	    0:15		16-bit payload segment (sequence number is the remainder) field
// RNDSEED_FRCDLY_SIZ	  16		 1-bit force delay field size bits (for debugging)
// RNDSEED_SPARE_SIZ	17:31		15-bit - spare unused bits
//__________________________________________________________________
#define TRAFFIC_MON_DELAY_MIN 	0x0208	//0x82	packet delay minimum
#define TRAFFIC_MON_DELAY_MAX 	0x020C	//0x83	packet delay maximum
#define TRAFFIC_MON_DELAY_SUM0	0x0210	//0x84	packet delay sum LSB
#define TRAFFIC_MON_DELAY_SUM1	0x0214	//0x85	packet delay sum MSB
#define TRAFFIC_MON_DELAY_SUM2	0x0218  // packet delay sum MSB
//======= New addresses ======================================   
#define TRAFFIC_MON_HDR0		0x0404  //
#define TRAFFIC_MON_HDR1		0x0400  //
#define TRAFFIC_MON_HDR2		0x040C  //
#define TRAFFIC_MON_HDR3		0x0408  //
#define TRAFFIC_MON_HDR4		0x0414  //
#define TRAFFIC_MON_HDR5		0x0410  //
#define TRAFFIC_MON_HDR6		0x041C  //
#define TRAFFIC_MON_HDR7		0x0418  //
#define TRAFFIC_MON_HDR8		0x0424  //
#define TRAFFIC_MON_HDR9		0x0420  //
#define TRAFFIC_MON_HDR10		0x042C  //
#define TRAFFIC_MON_HDR11		0x0428  //
#define TRAFFIC_MON_HDR12		0x0434  //
#define TRAFFIC_MON_HDR13		0x0430  //
#define TRAFFIC_MON_HDR14		0x043C  //
#define TRAFFIC_MON_HDR15		0x0438  //
#define TRAFFIC_MON_HDR16		0x0444  //
#define TRAFFIC_MON_HDR17		0x0440  //
#define TRAFFIC_MON_HDR18		0x044C  //
#define TRAFFIC_MON_HDR19		0x0448  //
#define TRAFFIC_MON_HDR20		0x0454  //
#define TRAFFIC_MON_HDR21		0x0450  //
#define TRAFFIC_MON_HDR22		0x045C  //
#define TRAFFIC_MON_HDR23		0x0458  //
#define TRAFFIC_MON_HDR24		0x0464  //
#define TRAFFIC_MON_HDR25		0x0460  //
#define TRAFFIC_MON_HDR26		0x046C  //
#define TRAFFIC_MON_HDR27		0x0468  //
#define TRAFFIC_MON_HDR28		0x0474  //
#define TRAFFIC_MON_HDR29		0x0470  //
#define TRAFFIC_MON_HDR30		0x047C  //
#define TRAFFIC_MON_HDR31		0x0478  //
#define TRAFFIC_MON_HDR32		0x0484  //
#define TRAFFIC_MON_HDR33		0x0480  //
#define TRAFFIC_MON_HDR34		0x048C  //
#define TRAFFIC_MON_HDR35		0x0488  //
#define TRAFFIC_MON_HDR36		0x0494  //
#define TRAFFIC_MON_HDR37		0x0490  //
#define TRAFFIC_MON_HDR38		0x049C  //
#define TRAFFIC_MON_HDR39		0x0498  //
#define TRAFFIC_MON_HDR40		0x04A4  //
#define TRAFFIC_MON_HDR41		0x04A0  //
#define TRAFFIC_MON_HDR42		0x04AC  //
#define TRAFFIC_MON_HDR43		0x04A8  //
#define TRAFFIC_MON_HDR44		0x04B4  //
#define TRAFFIC_MON_HDR45		0x04B0  //
#define TRAFFIC_MON_HDR46		0x04BC  //
#define TRAFFIC_MON_HDR47		0x04B8  //
#define TRAFFIC_MON_HDR48		0x04C4  //
#define TRAFFIC_MON_HDR49		0x04C0  //
#define TRAFFIC_MON_HDR50		0x04CC  //
#define TRAFFIC_MON_HDR51		0x04C8  //
#define TRAFFIC_MON_HDR52		0x04D4  //
#define TRAFFIC_MON_HDR53		0x04D0  //
#define TRAFFIC_MON_HDR54		0x04DC  //
#define TRAFFIC_MON_HDR55		0x04D8  //
#define TRAFFIC_MON_HDR56		0x04E4  //
#define TRAFFIC_MON_HDR57		0x04E0  //
#define TRAFFIC_MON_HDR58		0x04EC  //
#define TRAFFIC_MON_HDR59		0x04E8  //
#define TRAFFIC_MON_HDR60		0x04F4  //
#define TRAFFIC_MON_HDR61		0x04F0  //
#define TRAFFIC_MON_HDR62		0x04FC  //
#define TRAFFIC_MON_HDR63		0x04F8  //

#define TRAFFIC_MON_MSK1		0x0600  //
#define TRAFFIC_MON_MSK0		0x0604  //
#define TRAFFIC_MON_MSK3		0x0608  //
#define TRAFFIC_MON_MSK2		0x060C  //
#define TRAFFIC_MON_MSK5		0x0610  //
#define TRAFFIC_MON_MSK4		0x0614  //
#define TRAFFIC_MON_MSK7		0x0618  //
#define TRAFFIC_MON_MSK6		0x061C  //
#define TRAFFIC_MON_MSK9		0x0620  //
#define TRAFFIC_MON_MSK8		0x0624  //
#define TRAFFIC_MON_MSK10		0x062C  //
#define TRAFFIC_MON_MSK11		0x0628  //
#define TRAFFIC_MON_MSK12		0x0634  //
#define TRAFFIC_MON_MSK13		0x0630  //
#define TRAFFIC_MON_MSK14		0x063C  //
#define TRAFFIC_MON_MSK15		0x0638  //
#define TRAFFIC_MON_MSK16		0x0644  //
#define TRAFFIC_MON_MSK17		0x0640  //
#define TRAFFIC_MON_MSK18		0x064C  //
#define TRAFFIC_MON_MSK19		0x0648  //
#define TRAFFIC_MON_MSK20		0x0654  //
#define TRAFFIC_MON_MSK21		0x0650  //
#define TRAFFIC_MON_MSK22		0x065C  //
#define TRAFFIC_MON_MSK23		0x0658  //
#define TRAFFIC_MON_MSK24		0x0664  //
#define TRAFFIC_MON_MSK25		0x0660  //
#define TRAFFIC_MON_MSK26		0x066C  //
#define TRAFFIC_MON_MSK27		0x0668  //
#define TRAFFIC_MON_MSK28		0x0674  //
#define TRAFFIC_MON_MSK29		0x0670  //
#define TRAFFIC_MON_MSK30		0x067C  //
#define TRAFFIC_MON_MSK31		0x0678  //
#define TRAFFIC_MON_MSK32		0x0684  //
#define TRAFFIC_MON_MSK33		0x0680  //
#define TRAFFIC_MON_MSK34		0x068C  //
#define TRAFFIC_MON_MSK35		0x0688  //
#define TRAFFIC_MON_MSK36		0x0694  //
#define TRAFFIC_MON_MSK37		0x0690  //
#define TRAFFIC_MON_MSK38		0x069C  //
#define TRAFFIC_MON_MSK39		0x0698  //
#define TRAFFIC_MON_MSK40		0x06A4  //
#define TRAFFIC_MON_MSK41		0x06A0  //
#define TRAFFIC_MON_MSK42		0x06AC  //
#define TRAFFIC_MON_MSK43		0x06A8  //
#define TRAFFIC_MON_MSK44		0x06B4  //
#define TRAFFIC_MON_MSK45		0x06B0  //
#define TRAFFIC_MON_MSK46		0x06BC  //
#define TRAFFIC_MON_MSK47		0x06B8  //
#define TRAFFIC_MON_MSK48		0x06C4  //
#define TRAFFIC_MON_MSK49		0x06C0  //
#define TRAFFIC_MON_MSK50		0x06CC  //
#define TRAFFIC_MON_MSK51		0x06C8  //
#define TRAFFIC_MON_MSK52		0x06D4  //
#define TRAFFIC_MON_MSK53		0x06D0  //
#define TRAFFIC_MON_MSK54		0x06DC  //
#define TRAFFIC_MON_MSK55		0x06D8  //
#define TRAFFIC_MON_MSK56		0x06E4  //
#define TRAFFIC_MON_MSK57		0x06E0  //
#define TRAFFIC_MON_MSK58		0x06EC  //
#define TRAFFIC_MON_MSK59		0x06E8  //
#define TRAFFIC_MON_MSK60		0x06F4  //
#define TRAFFIC_MON_MSK61		0x06F0  //
#define TRAFFIC_MON_MSK62		0x06FC  //
#define TRAFFIC_MON_MSK63		0x06F8  //

#endif /*HARDWARE_DEF_H_*/
