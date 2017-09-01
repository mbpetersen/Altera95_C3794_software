#ifndef CONFSTAT_6400_H_
#define CONFSTAT_6400_H_

//****************************************************************
//**** Configuration Bytes from GUI to GIGE Module *****
//****************************************************************


#define MaxConfig 				75		// standard config
#define MaxConfig2 				445		// expanded TSE+10G Ethernet
//#define MaxConfig_ms 			5502		// further extended for TrafficEngine/MultiStreaming support

//extern unsigned int multistream_offset 		12
//extern unsigned int size_offset  			340
#define GIGEconfig_ptr 	0
#define ID_ptr			1
#define GIGEMODE_ptr	2
#define SYSCONFIG_ptr  	3
#define SPARE_ptr 		4



#define TRAFFIC_SPARE_12	5
#define TRAFFIC_SPARE_11	6
#define TRAFFIC_SPARE_10	7
#define TRAFFIC_SPARE_03	8
#define TRAFFIC_SPARE_02	9
#define TRAFFIC_SPARE_01	10
#define TRAFFIC_SPARE_00	11 //used to send the info of no of streams - as used by J





// Multi stream block
//------------------------------------------------------------------
// generic pointer bytes = (0-11) 12 bytes
//______________________________________________________________________
//________________________________________________________________________
//	Stream 0 -  Traffic bytes - numbers 12 to 352 (340+12)
//_________________________________________________________________________
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MON_STREAM_CNTL3_ptr)] [gconfig[(342*0)+(12 +0)]
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MON_STREAM_CNTL2_ptr)]	[gconfig[(342*0)+(12 +1)]
//
//							Stream 1
//
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MSK31_0_ptr)] [gconfig[(342*1)+(12 +342)]
//----------------------------------------------------------------------------
//	Stream 1 -  Traffic bytes - numbers 356 to 696 (342+356)
//----------------------------------------------------------------------------
//
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MON_STREAM_CNTL3_ptr)] [gconfig[(340*2)+(12 +0)]
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MON_STREAM_CNTL2_ptr)]	[gconfig[(340*2)+(12 +1)]
//
//							Stream 2
//
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MSK31_0_ptr)] [gconfig[(340*2)+(12 +340)]
//								.
//								.
//								.
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MON_STREAM_CNTL3_ptr)] [gconfig[(340*15)+(12 +0)]
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MON_STREAM_CNTL2_ptr)]	[gconfig[(34015)+(12 +1)]
//								.
//							    Stream 15
//								.
// gconfig[(size_offset*stream_no)+(multistream_offset + TRAFFIC_MSK31_0_ptr)] [gconfig[(340*15)+(12 +340)]
//---------------------------------------------------------------------------------------------
//		Stream 15 -  last byte number = 5488 (343*16)
//__________________________________________________________________
//__________________________________________________________________
//
// checksum1_ptr (5456 +1)
// checksum2_ptr (5456 +2)
//___________________________________________________________________________
// The size_offset being 343 (The number of bytes required for each stream = 0-342
// The total number of bytes is No of generic header type bytes = 	  12
//								No of btes in 16 streams = (343*16)	5488
//								No of bytes for checksum = 			   2
//								_________________________________________
//																	5502
//								_________________________________________
//----------------------------------------------------------------------------
/*
extern unsigned int TRAFFIC_MON_STREAM_CNTL3_ptr	(size_offset*stream_no)+(multistream_offset + 0)		  // Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
extern unsigned int TRAFFIC_MON_STREAM_CNTL2_ptr	(size_offset*stream_no)+(multistream_offset + 1)		// ^0:Copper/Fiber 0=copper,1=fiber, //not used:^1&2=speed:00=10,01=100,10=1G,11=10G
extern unsigned int TRAFFIC_MON_STREAM_CNTL1_ptr	(size_offset*stream_no)+(multistream_offset + 2)
extern unsigned int TRAFFIC_MON_STREAM_CNTL0_ptr	(size_offset*stream_no)+(multistream_offset + 3)
extern unsigned int TRAFFIC_RATE_TYPE				(size_offset*stream_no)+(multistream_offset + 4)			// MON stream control (for independent capture)
extern unsigned int TRAFFIC_RATE3_ptr          		(size_offset*stream_no)+(multistream_offset + 5)
extern unsigned int TRAFFIC_RATE2_ptr     			(size_offset*stream_no)+(multistream_offset + 6)
extern unsigned int TRAFFIC_RATE1_ptr     			(size_offset*stream_no)+(multistream_offset + 7	)
extern unsigned int TRAFFIC_RATE0_ptr     			(size_offset*stream_no)+(multistream_offset + 8	)			// TBD: e.g. 0=FPS%, 1=Gbps, 2=Mbps, 3=Kbps
extern unsigned int TRAFFIC_GEN_PKTPERCENT11_ptr	(size_offset*stream_no)+(multistream_offset + 9)
extern unsigned int TRAFFIC_GEN_PKTPERCENT10_ptr	(size_offset*stream_no)+(multistream_offset + 10)		  // Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%
extern unsigned int TRAFFIC_GEN_PKTPERCENT03_ptr	(size_offset*stream_no)+(multistream_offset + 11)		  // ie Gbps   0.0001-100.0G,  includes 99.9999 or 9.9999G or 0.9999G
extern unsigned int TRAFFIC_GEN_PKTPERCENT02_ptr	(size_offset*stream_no)+(multistream_offset + 12)		  //  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M
extern unsigned int TRAFFIC_GEN_PKTPERCENT01_ptr	(size_offset*stream_no)+(multistream_offset + 13)		 // 2-byte spares for 100G
extern unsigned int TRAFFIC_GEN_PKTPERCENT00_ptr	(size_offset*stream_no)+(multistream_offset + 14)
extern unsigned int EGEN_PKT_TYPE_ptr 				(size_offset*stream_no)+(multistream_offset + 15)
extern unsigned int EGEN_PKT_TYPE_ptr_1 			(size_offset*stream_no)+(multistream_offset + 16)
extern unsigned int EGEN_PKT_TYPE_ptr_2 			(size_offset*stream_no)+(multistream_offset + 17)

extern unsigned int TRAFFIC_GEN_CONFIG23_ptr	(size_offset*stream_no)+(multistream_offset + 18)
extern unsigned int TRAFFIC_GEN_CONFIG22_ptr	(size_offset*stream_no)+(multistream_offset + 19)
extern unsigned int TRAFFIC_GEN_CONFIG21_ptr	(size_offset*stream_no)+(multistream_offset + 20)
extern unsigned int TRAFFIC_GEN_CONFIG20_ptr	(size_offset*stream_no)+(multistream_offset + 21)
extern unsigned int TRAFFIC_GEN_CONFIG13_ptr	(size_offset*stream_no)+(multistream_offset + 22)		// TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed
extern unsigned int TRAFFIC_GEN_CONFIG12_ptr	(size_offset*stream_no)+(multistream_offset + 23)
extern unsigned int TRAFFIC_GEN_CONFIG11_ptr	(size_offset*stream_no)+(multistream_offset + 24)
extern unsigned int TRAFFIC_GEN_CONFIG10_ptr	(size_offset*stream_no)+(multistream_offset + 25)
extern unsigned int TRAFFIC_GEN_CONFIG03_ptr	(size_offset*stream_no)+(multistream_offset + 26)			//TRAFFIC_GEN_CONFIG2
extern unsigned int TRAFFIC_GEN_CONFIG02_ptr	(size_offset*stream_no)+(multistream_offset + 27)
extern unsigned int TRAFFIC_GEN_CONFIG01_ptr	(size_offset*stream_no)+(multistream_offset + 28)
extern unsigned int TRAFFIC_GEN_CONFIG00_ptr	(size_offset*stream_no)+(multistream_offset + 29)
extern unsigned int SPARE_GEN_CONFIG03_ptr		(size_offset*stream_no)+(multistream_offset + 30)
extern unsigned int SPARE_GEN_CONFIG02_ptr		(size_offset*stream_no)+(multistream_offset + 31)
extern unsigned int SPARE_GEN_CONFIG01_ptr		(size_offset*stream_no)+(multistream_offset + 32)
extern unsigned int SPARE_GEN_CONFIG00_ptr		(size_offset*stream_no)+(multistream_offset + 33)
extern unsigned int EGEN_PKT_EMIXUSERSIZE3_ptr	(size_offset*stream_no)+(multistream_offset + 34)
extern unsigned int EGEN_PKT_EMIXUSERSIZE2_ptr	(size_offset*stream_no)+(multistream_offset + 35)					//		0x00000001
extern unsigned int EGEN_PKT_EMIXUSERSIZE1_ptr	(size_offset*stream_no)+(multistream_offset + 36)					//		0x000000FE
extern unsigned int EGEN_PKT_EMIXUSERSIZE0_ptr	(size_offset*stream_no)+(multistream_offset + 37)					//		0x0003FF00
extern unsigned int EGEN_EMIXSEQ33_ptr  		(size_offset*stream_no)+(multistream_offset + 38)
extern unsigned int EGEN_EMIXSEQ32_ptr  		(size_offset*stream_no)+(multistream_offset + 39)
extern unsigned int EGEN_EMIXSEQ31_ptr  		(size_offset*stream_no)+(multistream_offset + 40)
extern unsigned int EGEN_EMIXSEQ30_ptr  		(size_offset*stream_no)+(multistream_offset + 41)
extern unsigned int EGEN_EMIXSEQ23_ptr			(size_offset*stream_no)+(multistream_offset + 42)			//LSNibble sent last
extern unsigned int EGEN_EMIXSEQ22_ptr  		(size_offset*stream_no)+(multistream_offset + 43)
extern unsigned int EGEN_EMIXSEQ21_ptr  	(size_offset*stream_no)+(multistream_offset + 44)
extern unsigned int EGEN_EMIXSEQ20_ptr 	(size_offset*stream_no)+(multistream_offset + 45)
extern unsigned int EGEN_EMIXSEQ13_ptr	(size_offset*stream_no)+(multistream_offset + 46)
extern unsigned int EGEN_EMIXSEQ12_ptr  	(size_offset*stream_no)+(multistream_offset + 47)
extern unsigned int EGEN_EMIXSEQ11_ptr	(size_offset*stream_no)+(multistream_offset + 48)
extern unsigned int EGEN_EMIXSEQ10_ptr	(size_offset*stream_no)+(multistream_offset + 49)
extern unsigned int EGEN_EMIXSEQ03_ptr 	(size_offset*stream_no)+(multistream_offset + 50)					//(good to set 1,2&3 def as 0xF0000000)
extern unsigned int EGEN_EMIXSEQ02_ptr 	(size_offset*stream_no)+(multistream_offset + 51)
extern unsigned int EGEN_EMIXSEQ01_ptr	(size_offset*stream_no)+(multistream_offset + 52)
extern unsigned int EGEN_EMIXSEQ00_ptr  	(size_offset*stream_no)+(multistream_offset + 53)
extern unsigned int Y1564_CIR_3_ptr	(size_offset*stream_no)+(multistream_offset + 54)			// MSNibble sent 1st   def for Random=0x0123456F
extern unsigned int Y1564_CIR_2_ptr	(size_offset*stream_no)+(multistream_offset + 55)			//Committed information rate
extern unsigned int Y1564_CIR_1_ptr	(size_offset*stream_no)+(multistream_offset + 56)
extern unsigned int Y1564_CIR_0_ptr	(size_offset*stream_no)+(multistream_offset + 57)
extern unsigned int Y1564_EIR_3_ptr	(size_offset*stream_no)+(multistream_offset + 58)				 //Expected information rate
extern unsigned int Y1564_EIR_2_ptr	(size_offset*stream_no)+(multistream_offset + 59)
extern unsigned int Y1564_EIR_1_ptr	(size_offset*stream_no)+(multistream_offset + 60)
extern unsigned int Y1564_EIR_0_ptr	(size_offset*stream_no)+(multistream_offset + 61)
extern unsigned int Y1564_CIR_STEPS_ptr	(size_offset*stream_no)+(multistream_offset + 62)				//Steps for CIR – first 4 bits
extern unsigned int Y1564_MAX_FTD_3_ptr 	(size_offset*stream_no)+(multistream_offset + 63)					//Expected information rate
extern unsigned int Y1564_MAX_FTD_2_ptr 	(size_offset*stream_no)+(multistream_offset + 64)					//Expected information rate
extern unsigned int Y1564_MAX_FTD_1_ptr 	(size_offset*stream_no)+(multistream_offset + 65)					//Expected information rate
extern unsigned int Y1564_MAX_FTD_0_ptr 	(size_offset*stream_no)+(multistream_offset + 66)					//Expected information rate
extern unsigned int Y1564_MAX_FDV_3_ptr 	(size_offset*stream_no)+(multistream_offset + 67)
extern unsigned int Y1564_MAX_FDV_2_ptr 	(size_offset*stream_no)+(multistream_offset + 68)			//4 byte Float
extern unsigned int Y1564_MAX_FDV_1_ptr 	(size_offset*stream_no)+(multistream_offset + 69)
extern unsigned int Y1564_MAX_FDV_0_ptr 	(size_offset*stream_no)+(multistream_offset + 70)
extern unsigned int Y1564_MAX_FRAME_LOSS_3_ptr 	(size_offset*stream_no)+(multistream_offset + 71)
extern unsigned int Y1564_MAX_FRAME_LOSS_2_ptr 	(size_offset*stream_no)+(multistream_offset + 72)			//4 byte Float
extern unsigned int Y1564_MAX_FRAME_LOSS_1_ptr 	(size_offset*stream_no)+(multistream_offset + 73)
extern unsigned int Y1564_MAX_FRAME_LOSS_0_ptr 	(size_offset*stream_no)+(multistream_offset + 74)
extern unsigned int Y1564_MAX_AVAIL_3_ptr 	(size_offset*stream_no)+(multistream_offset + 75)
extern unsigned int Y1564_MAX_AVAIL_2_ptr 	(size_offset*stream_no)+(multistream_offset + 76)		//4 byte Float
extern unsigned int Y1564_MAX_AVAIL_1_ptr 	(size_offset*stream_no)+(multistream_offset + 77)
extern unsigned int Y1564_MAX_AVAIL_0_ptr 	(size_offset*stream_no)+(multistream_offset + 78)
extern unsigned int SPARE_THRESHOLD_03	(size_offset*stream_no)+(multistream_offset + 79)
extern unsigned int SPARE_THRESHOLD_02	(size_offset*stream_no)+(multistream_offset + 80)			//4 byte Float
extern unsigned int SPARE_THRESHOLD_01	(size_offset*stream_no)+(multistream_offset + 81)
extern unsigned int SPARE_THRESHOLD_00	(size_offset*stream_no)+(multistream_offset + 82)
extern unsigned int TRAFFIC_GEN_SPARE_ptr	(size_offset*stream_no)+(multistream_offset + 83)
extern unsigned int TRAFFIC_HDR_DEF_ptr	(size_offset*stream_no)+(multistream_offset + 84)	 // additional instructional info for what is contained in HDR ptr's
extern unsigned int TRAFFIC_HDR0_3_ptr	(size_offset*stream_no)+(multistream_offset + 85)
extern unsigned int TRAFFIC_HDR0_2_ptr	(size_offset*stream_no)+(multistream_offset + 86)
extern unsigned int TRAFFIC_HDR0_1_ptr	(size_offset*stream_no)+(multistream_offset + 87)
extern unsigned int TRAFFIC_HDR0_0_ptr	(size_offset*stream_no)+(multistream_offset + 88)
extern unsigned int TRAFFIC_HDR1_3_ptr	(size_offset*stream_no)+(multistream_offset + 89)				// additional instructional info for what is contained in HDR ptr's
extern unsigned int TRAFFIC_HDR1_2_ptr	(size_offset*stream_no)+(multistream_offset + 90)
extern unsigned int TRAFFIC_HDR1_1_ptr	(size_offset*stream_no)+(multistream_offset + 91)
extern unsigned int TRAFFIC_HDR1_0_ptr	(size_offset*stream_no)+(multistream_offset + 92)
extern unsigned int TRAFFIC_HDR2_3_ptr	(size_offset*stream_no)+(multistream_offset + 93)
extern unsigned int TRAFFIC_HDR2_2_ptr	(size_offset*stream_no)+(multistream_offset + 94)
extern unsigned int TRAFFIC_HDR2_1_ptr	(size_offset*stream_no)+(multistream_offset + 95)
extern unsigned int TRAFFIC_HDR2_0_ptr	(size_offset*stream_no)+(multistream_offset + 96)
extern unsigned int TRAFFIC_HDR3_3_ptr	(size_offset*stream_no)+(multistream_offset + 97)
extern unsigned int TRAFFIC_HDR3_2_ptr	(size_offset*stream_no)+(multistream_offset + 98)
extern unsigned int TRAFFIC_HDR3_1_ptr	(size_offset*stream_no)+(multistream_offset + 99)
extern unsigned int TRAFFIC_HDR3_0_ptr	(size_offset*stream_no)+(multistream_offset + 100)
extern unsigned int TRAFFIC_HDR4_3_ptr	(size_offset*stream_no)+(multistream_offset + 101)
extern unsigned int TRAFFIC_HDR4_2_ptr	(size_offset*stream_no)+(multistream_offset + 102)
extern unsigned int TRAFFIC_HDR4_1_ptr	(size_offset*stream_no)+(multistream_offset + 103)
extern unsigned int TRAFFIC_HDR4_0_ptr	(size_offset*stream_no)+(multistream_offset + 104)
extern unsigned int TRAFFIC_HDR5_3_ptr	(size_offset*stream_no)+(multistream_offset + 105)
extern unsigned int TRAFFIC_HDR5_2_ptr	(size_offset*stream_no)+(multistream_offset + 106)
extern unsigned int TRAFFIC_HDR5_1_ptr	(size_offset*stream_no)+(multistream_offset + 107)
extern unsigned int TRAFFIC_HDR5_0_ptr	(size_offset*stream_no)+(multistream_offset + 108)
extern unsigned int TRAFFIC_HDR6_3_ptr	(size_offset*stream_no)+(multistream_offset + 109)
extern unsigned int TRAFFIC_HDR6_2_ptr	(size_offset*stream_no)+(multistream_offset + 110)
extern unsigned int TRAFFIC_HDR6_1_ptr	(size_offset*stream_no)+(multistream_offset + 111)
extern unsigned int TRAFFIC_HDR6_0_ptr	(size_offset*stream_no)+(multistream_offset + 112)
extern unsigned int TRAFFIC_HDR7_3_ptr	(size_offset*stream_no)+(multistream_offset + 113)
extern unsigned int TRAFFIC_HDR7_2_ptr	(size_offset*stream_no)+(multistream_offset + 114)
extern unsigned int TRAFFIC_HDR7_1_ptr	(size_offset*stream_no)+(multistream_offset + 115)
extern unsigned int TRAFFIC_HDR7_0_ptr	(size_offset*stream_no)+(multistream_offset + 116)
extern unsigned int TRAFFIC_HDR8_3_ptr	(size_offset*stream_no)+(multistream_offset + 117)
extern unsigned int TRAFFIC_HDR8_2_ptr	(size_offset*stream_no)+(multistream_offset + 118)
extern unsigned int TRAFFIC_HDR8_1_ptr	(size_offset*stream_no)+(multistream_offset + 119)
extern unsigned int TRAFFIC_HDR8_0_ptr	(size_offset*stream_no)+(multistream_offset + 120)
extern unsigned int TRAFFIC_HDR9_3_ptr	(size_offset*stream_no)+(multistream_offset + 121)
extern unsigned int TRAFFIC_HDR9_2_ptr	(size_offset*stream_no)+(multistream_offset + 122)
extern unsigned int TRAFFIC_HDR9_1_ptr	(size_offset*stream_no)+(multistream_offset + 123)
extern unsigned int TRAFFIC_HDR9_0_ptr	(size_offset*stream_no)+(multistream_offset + 124)
extern unsigned int TRAFFIC_HDR10_3_ptr	(size_offset*stream_no)+(multistream_offset + 125)
extern unsigned int TRAFFIC_HDR10_2_ptr	(size_offset*stream_no)+(multistream_offset + 126)
extern unsigned int TRAFFIC_HDR10_1_ptr	(size_offset*stream_no)+(multistream_offset + 127)
extern unsigned int TRAFFIC_HDR10_0_ptr	(size_offset*stream_no)+(multistream_offset + 128)
extern unsigned int TRAFFIC_HDR11_3_ptr	(size_offset*stream_no)+(multistream_offset + 129)
extern unsigned int TRAFFIC_HDR11_2_ptr	(size_offset*stream_no)+(multistream_offset + 130)
extern unsigned int TRAFFIC_HDR11_1_ptr	(size_offset*stream_no)+(multistream_offset + 131)
extern unsigned int TRAFFIC_HDR11_0_ptr	(size_offset*stream_no)+(multistream_offset + 132)
extern unsigned int TRAFFIC_HDR12_3_ptr	(size_offset*stream_no)+(multistream_offset + 133)
extern unsigned int TRAFFIC_HDR12_2_ptr	(size_offset*stream_no)+(multistream_offset + 134)
extern unsigned int TRAFFIC_HDR12_1_ptr	(size_offset*stream_no)+(multistream_offset + 135)
extern unsigned int TRAFFIC_HDR12_0_ptr	(size_offset*stream_no)+(multistream_offset + 136)
extern unsigned int TRAFFIC_HDR13_3_ptr	(size_offset*stream_no)+(multistream_offset + 137)
extern unsigned int TRAFFIC_HDR13_2_ptr	(size_offset*stream_no)+(multistream_offset + 138)
extern unsigned int TRAFFIC_HDR13_1_ptr	(size_offset*stream_no)+(multistream_offset + 139)
extern unsigned int TRAFFIC_HDR13_0_ptr	(size_offset*stream_no)+(multistream_offset + 140)
extern unsigned int TRAFFIC_HDR14_3_ptr	(size_offset*stream_no)+(multistream_offset + 141)
extern unsigned int TRAFFIC_HDR14_2_ptr	(size_offset*stream_no)+(multistream_offset + 142)
extern unsigned int TRAFFIC_HDR14_1_ptr	(size_offset*stream_no)+(multistream_offset + 143)
extern unsigned int TRAFFIC_HDR14_0_ptr	(size_offset*stream_no)+(multistream_offset + 144)
extern unsigned int TRAFFIC_HDR15_3_ptr	(size_offset*stream_no)+(multistream_offset + 145)
extern unsigned int TRAFFIC_HDR15_2_ptr	(size_offset*stream_no)+(multistream_offset + 146)
extern unsigned int TRAFFIC_HDR15_1_ptr	(size_offset*stream_no)+(multistream_offset + 147)
extern unsigned int TRAFFIC_HDR15_0_ptr	(size_offset*stream_no)+(multistream_offset + 148)
extern unsigned int TRAFFIC_HDR16_3_ptr	(size_offset*stream_no)+(multistream_offset + 149)
extern unsigned int TRAFFIC_HDR16_2_ptr	(size_offset*stream_no)+(multistream_offset + 150)
extern unsigned int TRAFFIC_HDR16_1_ptr	(size_offset*stream_no)+(multistream_offset + 151)
extern unsigned int TRAFFIC_HDR16_0_ptr	(size_offset*stream_no)+(multistream_offset + 152)
extern unsigned int TRAFFIC_HDR17_3_ptr	(size_offset*stream_no)+(multistream_offset + 153)
extern unsigned int TRAFFIC_HDR17_2_ptr	(size_offset*stream_no)+(multistream_offset + 154)
extern unsigned int TRAFFIC_HDR17_1_ptr	(size_offset*stream_no)+(multistream_offset + 155)
extern unsigned int TRAFFIC_HDR17_0_ptr	(size_offset*stream_no)+(multistream_offset + 156)
extern unsigned int TRAFFIC_HDR18_3_ptr	(size_offset*stream_no)+(multistream_offset + 157)
extern unsigned int TRAFFIC_HDR18_2_ptr	(size_offset*stream_no)+(multistream_offset + 158)
extern unsigned int TRAFFIC_HDR18_1_ptr	(size_offset*stream_no)+(multistream_offset + 159)
extern unsigned int TRAFFIC_HDR18_0_ptr	(size_offset*stream_no)+(multistream_offset + 160)
extern unsigned int TRAFFIC_HDR19_3_ptr	(size_offset*stream_no)+(multistream_offset + 161)
extern unsigned int TRAFFIC_HDR19_2_ptr	(size_offset*stream_no)+(multistream_offset + 162)
extern unsigned int TRAFFIC_HDR19_1_ptr	(size_offset*stream_no)+(multistream_offset + 163)
extern unsigned int TRAFFIC_HDR19_0_ptr	(size_offset*stream_no)+(multistream_offset + 164)
extern unsigned int TRAFFIC_HDR20_3_ptr	(size_offset*stream_no)+(multistream_offset + 165)
extern unsigned int TRAFFIC_HDR20_2_ptr	(size_offset*stream_no)+(multistream_offset + 166)
extern unsigned int TRAFFIC_HDR20_1_ptr	(size_offset*stream_no)+(multistream_offset + 167)
extern unsigned int TRAFFIC_HDR20_0_ptr	(size_offset*stream_no)+(multistream_offset + 168)
extern unsigned int TRAFFIC_HDR21_3_ptr	(size_offset*stream_no)+(multistream_offset + 169)
extern unsigned int TRAFFIC_HDR21_2_ptr	(size_offset*stream_no)+(multistream_offset + 170)
extern unsigned int TRAFFIC_HDR21_1_ptr	(size_offset*stream_no)+(multistream_offset + 171)
extern unsigned int TRAFFIC_HDR21_0_ptr	(size_offset*stream_no)+(multistream_offset + 172)
extern unsigned int TRAFFIC_HDR22_3_ptr	(size_offset*stream_no)+(multistream_offset + 173)
extern unsigned int TRAFFIC_HDR22_2_ptr	(size_offset*stream_no)+(multistream_offset + 174)
extern unsigned int TRAFFIC_HDR22_1_ptr	(size_offset*stream_no)+(multistream_offset + 175)
extern unsigned int TRAFFIC_HDR22_0_ptr	(size_offset*stream_no)+(multistream_offset + 176)
extern unsigned int TRAFFIC_HDR23_3_ptr	(size_offset*stream_no)+(multistream_offset + 177)
extern unsigned int TRAFFIC_HDR23_2_ptr	(size_offset*stream_no)+(multistream_offset + 178)
extern unsigned int TRAFFIC_HDR23_1_ptr	(size_offset*stream_no)+(multistream_offset + 179)
extern unsigned int TRAFFIC_HDR23_0_ptr	(size_offset*stream_no)+(multistream_offset + 180)
extern unsigned int TRAFFIC_HDR24_3_ptr	(size_offset*stream_no)+(multistream_offset + 181)
extern unsigned int TRAFFIC_HDR24_2_ptr	(size_offset*stream_no)+(multistream_offset + 182)
extern unsigned int TRAFFIC_HDR24_1_ptr	(size_offset*stream_no)+(multistream_offset + 183)
extern unsigned int TRAFFIC_HDR24_0_ptr	(size_offset*stream_no)+(multistream_offset + 184)
extern unsigned int TRAFFIC_HDR25_3_ptr	(size_offset*stream_no)+(multistream_offset + 185)
extern unsigned int TRAFFIC_HDR25_2_ptr	(size_offset*stream_no)+(multistream_offset + 186)
extern unsigned int TRAFFIC_HDR25_1_ptr	(size_offset*stream_no)+(multistream_offset + 187)
extern unsigned int TRAFFIC_HDR25_0_ptr	(size_offset*stream_no)+(multistream_offset + 188)
extern unsigned int TRAFFIC_HDR26_3_ptr	(size_offset*stream_no)+(multistream_offset + 189)
extern unsigned int TRAFFIC_HDR26_2_ptr	(size_offset*stream_no)+(multistream_offset + 190)
extern unsigned int TRAFFIC_HDR26_1_ptr	(size_offset*stream_no)+(multistream_offset + 191)
extern unsigned int TRAFFIC_HDR26_0_ptr	(size_offset*stream_no)+(multistream_offset + 192)
extern unsigned int TRAFFIC_HDR27_3_ptr	(size_offset*stream_no)+(multistream_offset + 193)
extern unsigned int TRAFFIC_HDR27_2_ptr	(size_offset*stream_no)+(multistream_offset + 194)
extern unsigned int TRAFFIC_HDR27_0_ptr	(size_offset*stream_no)+(multistream_offset + 196)
extern unsigned int TRAFFIC_HDR28_3_ptr	(size_offset*stream_no)+(multistream_offset + 197)
extern unsigned int TRAFFIC_HDR28_2_ptr	(size_offset*stream_no)+(multistream_offset + 198)
extern unsigned int TRAFFIC_HDR28_1_ptr	(size_offset*stream_no)+(multistream_offset + 199)
extern unsigned int TRAFFIC_HDR28_0_ptr	(size_offset*stream_no)+(multistream_offset + 200)
extern unsigned int TRAFFIC_HDR29_3_ptr	(size_offset*stream_no)+(multistream_offset + 201)
extern unsigned int TRAFFIC_HDR29_2_ptr	(size_offset*stream_no)+(multistream_offset + 202)
extern unsigned int TRAFFIC_HDR29_1_ptr	(size_offset*stream_no)+(multistream_offset + 203)
extern unsigned int TRAFFIC_HDR29_0_ptr	(size_offset*stream_no)+(multistream_offset + 204)
extern unsigned int TRAFFIC_HDR30_3_ptr	(size_offset*stream_no)+(multistream_offset + 205)
extern unsigned int TRAFFIC_HDR30_2_ptr	(size_offset*stream_no)+(multistream_offset + 206)
extern unsigned int TRAFFIC_HDR30_1_ptr	(size_offset*stream_no)+(multistream_offset + 207)
extern unsigned int TRAFFIC_HDR30_0_ptr	(size_offset*stream_no)+(multistream_offset + 208)
extern unsigned int TRAFFIC_HDR31_3_ptr	(size_offset*stream_no)+(multistream_offset + 209)
extern unsigned int TRAFFIC_HDR31_2_ptr	(size_offset*stream_no)+(multistream_offset + 210)
extern unsigned int TRAFFIC_HDR31_1_ptr	(size_offset*stream_no)+(multistream_offset + 211)
extern unsigned int TRAFFIC_HDR31_0_ptr	(size_offset*stream_no)+(multistream_offset + 212)
extern unsigned int TRAFFIC_MSK0_3_ptr	(size_offset*stream_no)+(multistream_offset + 213)
extern unsigned int TRAFFIC_MSK0_2_ptr	(size_offset*stream_no)+(multistream_offset + 214)
extern unsigned int TRAFFIC_MSK0_1_ptr	(size_offset*stream_no)+(multistream_offset + 215)
extern unsigned int TRAFFIC_MSK0_0_ptr	(size_offset*stream_no)+(multistream_offset + 216)
extern unsigned int TRAFFIC_MSK1_3_ptr	(size_offset*stream_no)+(multistream_offset + 217)
extern unsigned int TRAFFIC_MSK1_2_ptr	(size_offset*stream_no)+(multistream_offset + 218)
extern unsigned int TRAFFIC_MSK1_1_ptr	(size_offset*stream_no)+(multistream_offset + 219)
extern unsigned int TRAFFIC_MSK1_0_ptr	(size_offset*stream_no)+(multistream_offset + 220)
extern unsigned int TRAFFIC_MSK2_3_ptr	(size_offset*stream_no)+(multistream_offset + 221)
extern unsigned int TRAFFIC_MSK2_2_ptr	(size_offset*stream_no)+(multistream_offset + 222)
extern unsigned int TRAFFIC_MSK2_1_ptr	(size_offset*stream_no)+(multistream_offset + 223)
extern unsigned int TRAFFIC_MSK2_0_ptr	(size_offset*stream_no)+(multistream_offset + 224)
extern unsigned int TRAFFIC_MSK3_3_ptr	(size_offset*stream_no)+(multistream_offset + 225)
extern unsigned int TRAFFIC_MSK3_2_ptr	(size_offset*stream_no)+(multistream_offset + 226)
extern unsigned int TRAFFIC_MSK3_1_ptr	(size_offset*stream_no)+(multistream_offset + 227)
extern unsigned int TRAFFIC_MSK3_0_ptr	(size_offset*stream_no)+(multistream_offset + 228)
extern unsigned int TRAFFIC_MSK4_3_ptr	(size_offset*stream_no)+(multistream_offset + 229)
extern unsigned int TRAFFIC_MSK4_2_ptr	(size_offset*stream_no)+(multistream_offset + 230)
extern unsigned int TRAFFIC_MSK4_1_ptr	(size_offset*stream_no)+(multistream_offset + 231)
extern unsigned int TRAFFIC_MSK4_0_ptr	(size_offset*stream_no)+(multistream_offset + 232)
extern unsigned int TRAFFIC_MSK5_3_ptr	(size_offset*stream_no)+(multistream_offset + 233)
extern unsigned int TRAFFIC_MSK5_2_ptr	(size_offset*stream_no)+(multistream_offset + 234)
extern unsigned int TRAFFIC_MSK5_1_ptr	(size_offset*stream_no)+(multistream_offset + 235)
extern unsigned int TRAFFIC_MSK5_0_ptr	(size_offset*stream_no)+(multistream_offset + 236)
extern unsigned int TRAFFIC_MSK6_3_ptr	(size_offset*stream_no)+(multistream_offset + 237)
extern unsigned int TRAFFIC_MSK6_2_ptr	(size_offset*stream_no)+(multistream_offset + 238)
extern unsigned int TRAFFIC_MSK6_1_ptr	(size_offset*stream_no)+(multistream_offset + 239)
extern unsigned int TRAFFIC_MSK6_0_ptr	(size_offset*stream_no)+(multistream_offset + 240)
extern unsigned int TRAFFIC_MSK7_3_ptr	(size_offset*stream_no)+(multistream_offset + 241)
extern unsigned int TRAFFIC_MSK7_2_ptr	(size_offset*stream_no)+(multistream_offset + 242)
extern unsigned int TRAFFIC_MSK7_1_ptr	(size_offset*stream_no)+(multistream_offset + 243)
extern unsigned int TRAFFIC_MSK7_0_ptr	(size_offset*stream_no)+(multistream_offset + 244)
extern unsigned int TRAFFIC_MSK8_3_ptr	(size_offset*stream_no)+(multistream_offset + 245)
extern unsigned int TRAFFIC_MSK8_2_ptr	(size_offset*stream_no)+(multistream_offset + 246)
extern unsigned int TRAFFIC_MSK8_1_ptr	(size_offset*stream_no)+(multistream_offset + 247)
extern unsigned int TRAFFIC_MSK8_0_ptr	(size_offset*stream_no)+(multistream_offset + 248)
extern unsigned int TRAFFIC_MSK9_3_ptr	(size_offset*stream_no)+(multistream_offset + 249)
extern unsigned int TRAFFIC_MSK9_2_ptr	(size_offset*stream_no)+(multistream_offset + 250)
extern unsigned int TRAFFIC_MSK9_1_ptr	(size_offset*stream_no)+(multistream_offset + 251)
extern unsigned int TRAFFIC_MSK9_0_ptr	(size_offset*stream_no)+(multistream_offset + 252)
extern unsigned int TRAFFIC_MSK10_3_ptr	(size_offset*stream_no)+(multistream_offset + 253)
extern unsigned int TRAFFIC_MSK10_2_ptr	(size_offset*stream_no)+(multistream_offset + 254)
extern unsigned int TRAFFIC_MSK10_1_ptr	(size_offset*stream_no)+(multistream_offset + 255)
extern unsigned int TRAFFIC_MSK10_0_ptr	(size_offset*stream_no)+(multistream_offset + 256)
extern unsigned int TRAFFIC_MSK11_3_ptr	(size_offset*stream_no)+(multistream_offset + 257)
extern unsigned int TRAFFIC_MSK11_2_ptr	(size_offset*stream_no)+(multistream_offset + 258)
extern unsigned int TRAFFIC_MSK11_1_ptr	(size_offset*stream_no)+(multistream_offset + 259)
extern unsigned int TRAFFIC_MSK11_0_ptr	(size_offset*stream_no)+(multistream_offset + 260)
extern unsigned int TRAFFIC_MSK12_3_ptr	(size_offset*stream_no)+(multistream_offset + 261)
extern unsigned int TRAFFIC_MSK12_2_ptr	(size_offset*stream_no)+(multistream_offset + 262)
extern unsigned int TRAFFIC_MSK12_1_ptr	(size_offset*stream_no)+(multistream_offset + 263)
extern unsigned int TRAFFIC_MSK12_0_ptr	(size_offset*stream_no)+(multistream_offset + 264)
extern unsigned int TRAFFIC_MSK13_3_ptr	(size_offset*stream_no)+(multistream_offset + 265)
extern unsigned int TRAFFIC_MSK13_2_ptr	(size_offset*stream_no)+(multistream_offset + 266)
extern unsigned int TRAFFIC_MSK13_1_ptr	(size_offset*stream_no)+(multistream_offset + 267)
extern unsigned int TRAFFIC_MSK13_0_ptr	(size_offset*stream_no)+(multistream_offset + 268)
extern unsigned int TRAFFIC_MSK14_3_ptr	(size_offset*stream_no)+(multistream_offset + 269)
extern unsigned int TRAFFIC_MSK14_2_ptr	(size_offset*stream_no)+(multistream_offset + 270)
extern unsigned int TRAFFIC_MSK14_1_ptr	(size_offset*stream_no)+(multistream_offset + 271)
extern unsigned int TRAFFIC_MSK14_0_ptr	(size_offset*stream_no)+(multistream_offset + 272)
extern unsigned int TRAFFIC_MSK15_3_ptr	(size_offset*stream_no)+(multistream_offset + 273)
extern unsigned int TRAFFIC_MSK15_2_ptr	(size_offset*stream_no)+(multistream_offset + 274)
extern unsigned int TRAFFIC_MSK15_1_ptr	(size_offset*stream_no)+(multistream_offset + 275)
extern unsigned int TRAFFIC_MSK15_0_ptr	(size_offset*stream_no)+(multistream_offset + 276)
extern unsigned int TRAFFIC_MSK16_3_ptr	(size_offset*stream_no)+(multistream_offset + 277)
extern unsigned int TRAFFIC_MSK16_2_ptr	(size_offset*stream_no)+(multistream_offset + 278)
extern unsigned int TRAFFIC_MSK16_1_ptr	(size_offset*stream_no)+(multistream_offset + 279)
extern unsigned int TRAFFIC_MSK16_0_ptr	(size_offset*stream_no)+(multistream_offset + 280)
extern unsigned int TRAFFIC_MSK17_3_ptr	(size_offset*stream_no)+(multistream_offset + 281)
extern unsigned int TRAFFIC_MSK17_2_ptr	(size_offset*stream_no)+(multistream_offset + 282)
extern unsigned int TRAFFIC_MSK17_1_ptr	(size_offset*stream_no)+(multistream_offset + 283)
extern unsigned int TRAFFIC_MSK17_0_ptr	(size_offset*stream_no)+(multistream_offset + 284)
extern unsigned int TRAFFIC_MSK18_3_ptr	(size_offset*stream_no)+(multistream_offset + 285)
extern unsigned int TRAFFIC_MSK18_2_ptr	(size_offset*stream_no)+(multistream_offset + 286)
extern unsigned int TRAFFIC_MSK18_1_ptr	(size_offset*stream_no)+(multistream_offset + 287)
extern unsigned int TRAFFIC_MSK18_0_ptr	(size_offset*stream_no)+(multistream_offset + 288)
extern unsigned int TRAFFIC_MSK19_3_ptr	(size_offset*stream_no)+(multistream_offset + 289)
extern unsigned int TRAFFIC_MSK19_2_ptr	(size_offset*stream_no)+(multistream_offset + 290)
extern unsigned int TRAFFIC_MSK19_1_ptr	(size_offset*stream_no)+(multistream_offset + 291)
extern unsigned int TRAFFIC_MSK19_0_ptr	(size_offset*stream_no)+(multistream_offset + 292)
extern unsigned int TRAFFIC_MSK20_3_ptr	(size_offset*stream_no)+(multistream_offset + 293)
extern unsigned int TRAFFIC_MSK20_2_ptr	(size_offset*stream_no)+(multistream_offset + 294)
extern unsigned int TRAFFIC_MSK20_1_ptr	(size_offset*stream_no)+(multistream_offset + 295)
extern unsigned int TRAFFIC_MSK20_0_ptr	(size_offset*stream_no)+(multistream_offset + 296)
extern unsigned int TRAFFIC_MSK21_3_ptr	(size_offset*stream_no)+(multistream_offset + 297)
extern unsigned int TRAFFIC_MSK21_2_ptr	(size_offset*stream_no)+(multistream_offset + 298)
extern unsigned int TRAFFIC_MSK21_1_ptr	(size_offset*stream_no)+(multistream_offset + 299)
extern unsigned int TRAFFIC_MSK21_0_ptr	(size_offset*stream_no)+(multistream_offset + 300)
extern unsigned int TRAFFIC_MSK22_3_ptr	(size_offset*stream_no)+(multistream_offset + 301)
extern unsigned int TRAFFIC_MSK22_2_ptr	(size_offset*stream_no)+(multistream_offset + 302)
extern unsigned int TRAFFIC_MSK22_1_ptr	(size_offset*stream_no)+(multistream_offset + 303)
extern unsigned int TRAFFIC_MSK22_0_ptr	(size_offset*stream_no)+(multistream_offset + 304)
extern unsigned int TRAFFIC_MSK23_3_ptr	(size_offset*stream_no)+(multistream_offset + 305)
extern unsigned int TRAFFIC_MSK23_2_ptr	(size_offset*stream_no)+(multistream_offset + 306)
extern unsigned int TRAFFIC_MSK23_1_ptr	(size_offset*stream_no)+(multistream_offset + 307)
extern unsigned int TRAFFIC_MSK23_0_ptr	(size_offset*stream_no)+(multistream_offset + 308)
extern unsigned int TRAFFIC_MSK24_3_ptr	(size_offset*stream_no)+(multistream_offset + 309)
extern unsigned int TRAFFIC_MSK24_2_ptr	(size_offset*stream_no)+(multistream_offset + 310)
extern unsigned int TRAFFIC_MSK24_1_ptr	(size_offset*stream_no)+(multistream_offset + 311)
extern unsigned int TRAFFIC_MSK24_0_ptr	(size_offset*stream_no)+(multistream_offset + 312)
extern unsigned int TRAFFIC_MSK25_3_ptr	(size_offset*stream_no)+(multistream_offset + 313)
extern unsigned int TRAFFIC_MSK25_2_ptr	(size_offset*stream_no)+(multistream_offset + 314)
extern unsigned int TRAFFIC_MSK25_1_ptr	(size_offset*stream_no)+(multistream_offset + 315)
extern unsigned int TRAFFIC_MSK25_0_ptr	(size_offset*stream_no)+(multistream_offset + 316)
extern unsigned int TRAFFIC_MSK26_3_ptr	(size_offset*stream_no)+(multistream_offset + 317)
extern unsigned int TRAFFIC_MSK26_2_ptr	(size_offset*stream_no)+(multistream_offset + 318)
extern unsigned int TRAFFIC_MSK26_1_ptr	(size_offset*stream_no)+(multistream_offset + 319)
extern unsigned int TRAFFIC_MSK26_0_ptr	(size_offset*stream_no)+(multistream_offset + 320)
extern unsigned int TRAFFIC_MSK27_3_ptr	(size_offset*stream_no)+(multistream_offset + 321)
extern unsigned int TRAFFIC_MSK27_2_ptr	(size_offset*stream_no)+(multistream_offset + 322)
extern unsigned int TRAFFIC_MSK27_1_ptr	(size_offset*stream_no)+(multistream_offset + 323)
extern unsigned int TRAFFIC_MSK27_0_ptr	(size_offset*stream_no)+(multistream_offset + 324)
extern unsigned int TRAFFIC_MSK28_3_ptr	(size_offset*stream_no)+(multistream_offset + 325)
extern unsigned int TRAFFIC_MSK28_2_ptr	(size_offset*stream_no)+(multistream_offset + 326)
extern unsigned int TRAFFIC_MSK28_1_ptr	(size_offset*stream_no)+(multistream_offset + 327)
extern unsigned int TRAFFIC_MSK28_0_ptr	(size_offset*stream_no)+(multistream_offset + 328)
extern unsigned int TRAFFIC_MSK29_3_ptr	(size_offset*stream_no)+(multistream_offset + 329)
extern unsigned int TRAFFIC_MSK29_2_ptr	(size_offset*stream_no)+(multistream_offset + 330)
extern unsigned int TRAFFIC_MSK29_1_ptr	(size_offset*stream_no)+(multistream_offset + 331)
extern unsigned int TRAFFIC_MSK29_0_ptr	(size_offset*stream_no)+(multistream_offset + 332)
extern unsigned int TRAFFIC_MSK30_3_ptr	(size_offset*stream_no)+(multistream_offset + 333)
extern unsigned int TRAFFIC_MSK30_2_ptr	(size_offset*stream_no)+(multistream_offset + 334)
extern unsigned int TRAFFIC_MSK30_1_ptr	(size_offset*stream_no)+(multistream_offset + 335)
extern unsigned int TRAFFIC_MSK30_0_ptr	(size_offset*stream_no)+(multistream_offset + 336)
extern unsigned int TRAFFIC_MSK31_3_ptr	(size_offset*stream_no)+(multistream_offset + 337)
extern unsigned int TRAFFIC_MSK31_2_ptr	(size_offset*stream_no)+(multistream_offset + 338)
extern unsigned int TRAFFIC_MSK31_1_ptr	(size_offset*stream_no)+(multistream_offset + 339)
extern unsigned int TRAFFIC_MSK31_0_ptr	(size_offset*stream_no)+(multistream_offset + 340)
*/

/*
extern unsigned int TRAFFIC_MON_STREAM_CNTL3_ptr = 	101			;	// MON stream control (for independent capture)
        extern unsigned int TRAFFIC_MON_STREAM_CNTL2_ptr = 	102			;
        extern unsigned int TRAFFIC_MON_STREAM_CNTL1_ptr = 	103			;
        extern unsigned int TRAFFIC_MON_STREAM_CNTL0_ptr = 	104			;

        extern unsigned int TRAFFICRATE_TYPE	=		105	;			// TBD: e.g. 0=FPS%, 1=Gbps, 2=Mbps, 3=Kbps
        extern unsigned int TRAFFIC_RATE3_ptr =           	106			;
        extern unsigned int TRAFFIC_RATE2_ptr =      	107	;	 // Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%	;
        extern unsigned int TRAFFIC_RATE1_ptr =      	108			;
        extern unsigned int TRAFFIC_RATE0_ptr =      	109	;	  //  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M	;

        extern unsigned int TRAFFIC_GEN_PKTPERCENT11_ptr = 	110	; // 2-byte spares for 100G		;
        extern unsigned int TRAFFIC_GEN_PKTPERCENT10_ptr = 	111			;
        extern unsigned int TRAFFIC_GEN_PKTPERCENT03_ptr = 	112			;
        extern unsigned int TRAFFIC_GEN_PKTPERCENT02_ptr = 	113			;
        extern unsigned int TRAFFIC_GEN_PKTPERCENT01_ptr = 	114			;
        extern unsigned int TRAFFIC_GEN_PKTPERCENT00_ptr = 	115			;
//_______________EGEN_PKT_TYPE_ptr = ____________________				;
// 0000,0000:  ^10:Ethernet, ^32:IP, ^54:TCP/UDP, ^76:FUTURE				;
// 10: 00=std Layer2-Ethernet, 01=Ethernet II, 10=802.3 LLC, 11=802.3SNAP				;
// 32: 00=none, 01=IPv4, 10=IPv6				;
// 54: 00=none, 01=TCP, 10=UDP				;
// 76: future				;
//_________________EGEN_PKT_TYPE_ptr = _1____________________				;
// 0000,0000:  ^7654:MPLS, ^3210:VLAN 	(0=none, N=# of VLAN's or MPLS tags)			;
//_________________EGEN_PKT_TYPE_ptr = _2____________________				;
//	(8-bits OAM/802.3ag/Y.1731)			;
        extern unsigned int EGEN_PKT_TYPE_ptr =  		116		;
        extern unsigned int EGEN_PKT_TYPE_ptr_1 =	117			;
        extern unsigned int EGEN_PKT_TYPE_ptr_2 =	118			;

......for				;
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
		//#define JUMBO_MASK		;
		// CONFG2_HCSOFS_SIZ	8:14	;	// 7-bits: ipv4 hcs byte offset field size bits
		// CONFG2_SPARE2_SIZ	     15:31	;	// 17-bits: spare field size bits
	//	unsigned int  traffic_gen_config3 = 0x059C;		;	// 802.3, 1518=payload len of 1500, 1500-64=1436, 0x5ee-0x40=0x059C
		// CONFIG3_ILEN		;			16-bit:  intermediate length for random length recalc field size bits
		// CONFIG3_IHCS	     16:31	;		16-bit: intermediate HCS for random length HCS recalc field size bits
//unsigned short stream_no = 0;				;
extern unsigned int TRAFFIC_GEN_CONFIG23_ptr = 	119		;  //TRAFFIC_GEN_CONFIG3	;
extern unsigned int TRAFFIC_GEN_CONFIG22_ptr = 	120			;
extern unsigned int TRAFFIC_GEN_CONFIG21_ptr = 	121			;
extern unsigned int TRAFFIC_GEN_CONFIG20_ptr = 	122			;

extern unsigned int TRAFFIC_GEN_CONFIG13_ptr = 	123			;
extern unsigned int TRAFFIC_GEN_CONFIG12_ptr = 	124	;	 //15:31 // 17-bits: spare field size bits	;
extern unsigned int TRAFFIC_GEN_CONFIG11_ptr = 	125	;	//CONFG2_HCSOFS_SIZ	;
extern unsigned int TRAFFIC_GEN_CONFIG10_ptr = 	126	;	// 7 bits: Length field byte offset in HDR + 1 bit:  jumbo length to 0x8870 handling field size bits	;

extern unsigned int TRAFFIC_GEN_CONFIG03_ptr = 	127	;	// TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed	;
extern unsigned int TRAFFIC_GEN_CONFIG02_ptr = 	128			;
extern unsigned int TRAFFIC_GEN_CONFIG01_ptr = 	129			;
extern unsigned int TRAFFIC_GEN_CONFIG00_ptr = 	130			;

extern unsigned int SPARE_GEN_CONFIG03_ptr = 	131			;
extern unsigned int SPARE_GEN_CONFIG02_ptr = 	132			;
extern unsigned int SPARE_GEN_CONFIG01_ptr = 	133			;
extern unsigned int SPARE_GEN_CONFIG00_ptr = 	134			;

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
					#define LEN_MASK				0xFFFF
					#define HDRLEN_MASK				0xFF0000
					#define RNDLEN_ON				0x01000000
					#define RNDPAY_ON				0x02000000
					#define FRCERR_ON				0x04000000								//0xFFFF
					#define CONTINUOUS_ON 			0x08000000					//0xFF0000		//62	3E
					#define BWRANK_MASK 			0xF0000000		   			//0x01000000

					#define LENOFS_MASK				0x007F	// Payload 1st byte location (0 is 1st byte of HDR0)
					#define JUMBO_MASK				0x0080
					#define HCSOFF_MASK				0x7F00

					#define ILEN_MASK				0x0000FFFF	// ILEN = payload length - 64, (payload length=pkt-len-18)
					#define IHCS_MASK				0xFFFF0000

					#define STREAM_CNTL_ENABLE_MASK			0x00000001		// 1-bit:  Stream Enable bit
					#define STREAM_CNTL_SPARE1_MASK			0x000000FE		// 7-bit: spare - not used
					#define STREAM_CNTL_HDRLEN_MASK			0x0003FF00		//10-bit: Stream header length
					#define STREAM_CNTL_SPARE2_MASK			0xFFFC0000		//14-bit: spare - not used

//_______________________________________________________________________________________
//		a	  b		 c		 d		  e		  f		  g		        h		          u         invalid and
//	     64	128		256		512		1024	1280	usr_g	MTU(pktlen)	user_u     terminates_seq
//	      0      1          2          3           4           5          6               7                 8             9-F
//_______________________________________________________________________________________

#define EGEN_PKT_EMIXUSERSIZE3_ptr = 	135			;
#define EGEN_PKT_EMIXUSERSIZE2_ptr = 	136			;
#define EGEN_PKT_EMIXUSERSIZE1_ptr = 	137			;
#define EGEN_PKT_EMIXUSERSIZE0_ptr = 	138			;

#define EGEN_EMIXSEQ33_ptr =   	139			;
#define EGEN_EMIXSEQ32_ptr =   	140			;
#define EGEN_EMIXSEQ31_ptr =   	141			;
#define EGEN_EMIXSEQ30_ptr =   	142			;
#define EGEN_EMIXSEQ23_ptr = 		143		;		//(good to set 1,2&3 def as 0xF0000000)
#define EGEN_EMIXSEQ22_ptr =   	144			;
#define EGEN_EMIXSEQ21_ptr =   	145			;
#define EGEN_EMIXSEQ20_ptr =  		146		;
#define EGEN_EMIXSEQ13_ptr = 		147		;
#define EGEN_EMIXSEQ12_ptr =   	148			;
#define EGEN_EMIXSEQ11_ptr = 		149		;
#define EGEN_EMIXSEQ10_ptr = 		150		;
#define EGEN_EMIXSEQ03_ptr =  		151		;	//LSNibble sent last
#define EGEN_EMIXSEQ02_ptr =  		152		;
#define EGEN_EMIXSEQ01_ptr = 		153		;
#define EGEN_EMIXSEQ00_ptr =   	154			;
************************************************************************************************************************				;
// Config bytes related to Y.1564 & RFC2544 User selectable Test thresholds				;
***************************************************************************************************************************				;
extern unsigned int TEST_OPTIONS_ptr =  			XX // Test options to select between (quick test,multistream, y.1564, rfc..) - In the main Config is used to set the different type of test- service configuration/service performance/both)	;
...//...CIR & EIR are 4-bytes (they are rates - like TRAFFICRATE_TYPE above)				;
//RFC2544 will share/overlap these thresholds to allow threshold support for RFC2544 as well				;
extern unsigned int  Y1564_CIR_3_ptr = 	155			;	 //Committed information rate
        extern unsigned int  Y1564_CIR_2_ptr = 	156			;
        extern unsigned int  Y1564_CIR_1_ptr = 	157			;
        extern unsigned int  Y1564_CIR_0_ptr = 	158			;

        extern unsigned int  Y1564_EIR_3_ptr = 	159			;		//Expected information rate
        extern unsigned int  Y1564_EIR_2_ptr = 	160			;		//Expected information rate
        extern unsigned int  Y1564_EIR_1_ptr = 	161			;		//Expected information rate
        extern unsigned int  Y1564_EIR_0_ptr = 	162			;		//Expected information rate

        extern unsigned int  Y1564_CIR_STEPS_ptr = 	163			;	//Steps for CIR – first 4 bits

        extern unsigned int  Y1564_MAX_FTD_3_ptr =  	164			;
        extern unsigned int  Y1564_MAX_FTD_2_ptr =  	165			;
        extern unsigned int  Y1564_MAX_FTD_1_ptr =  	166			;
        extern unsigned int  Y1564_MAX_FTD_0_ptr =  	167			;

        extern unsigned int  Y1564_MAX_FDV_3_ptr =  	168			;
        extern unsigned int  Y1564_MAX_FDV_2_ptr =  	169			;
        extern unsigned int  Y1564_MAX_FDV_1_ptr =  	170			;
        extern unsigned int  Y1564_MAX_FDV_0_ptr =  	171			;

        extern unsigned int  Y1564_MAX_FRAME_LOSS_3_ptr =  	172	;	//4 byte Float	;
        extern unsigned int  Y1564_MAX_FRAME_LOSS_2_ptr =  	173			;
        extern unsigned int  Y1564_MAX_FRAME_LOSS_1_ptr =  	174			;
        extern unsigned int  Y1564_MAX_FRAME_LOSS_0_ptr =  	175			;

        extern unsigned int  Y1564_MAX_AVAIL_3_ptr =  	176			;
        extern unsigned int  Y1564_MAX_AVAIL_2_ptr =  	177			;
        extern unsigned int  Y1564_MAX_AVAIL_1_ptr =  	178			;
        extern unsigned int  Y1564_MAX_AVAIL_0_ptr =  	179			;

        extern unsigned int SPARE_THRESHOLD_03	=180			;
        extern unsigned int SPARE_THRESHOLD_02=	181			;
        extern unsigned int SPARE_THRESHOLD_01	=182			;
        extern unsigned int SPARE_THRESHOLD_00	=183			;

extern unsigned int TRAFFIC_GEN_SPARE_ptr = 	184			;
extern unsigned int TRAFFIC_HDR_DEF_ptr = 		185		;		// additional instructional info for what is contained in HDR ptr = 's


extern unsigned int TRAFFIC_HDR0_3_ptr = 	186			;
extern unsigned int TRAFFIC_HDR0_2_ptr = 	187			;
extern unsigned int TRAFFIC_HDR0_1_ptr = 	188			;
extern unsigned int TRAFFIC_HDR0_0_ptr = 	189			;
extern unsigned int TRAFFIC_HDR1_3_ptr = 	190			;
extern unsigned int TRAFFIC_HDR1_2_ptr = 	191			;
extern unsigned int TRAFFIC_HDR1_1_ptr = 	192			;
extern unsigned int TRAFFIC_HDR1_0_ptr = 	193			;
extern unsigned int TRAFFIC_HDR2_3_ptr = 	194			;
extern unsigned int TRAFFIC_HDR2_2_ptr = 	195			;
extern unsigned int TRAFFIC_HDR2_1_ptr = 	196			;
extern unsigned int TRAFFIC_HDR2_0_ptr = 	197			;
extern unsigned int TRAFFIC_HDR3_3_ptr = 	198			;
extern unsigned int TRAFFIC_HDR3_2_ptr = 	199			;
extern unsigned int TRAFFIC_HDR3_1_ptr = 	200			;
extern unsigned int TRAFFIC_HDR3_0_ptr = 	201			;
extern unsigned int TRAFFIC_HDR4_3_ptr = 	202			;
extern unsigned int TRAFFIC_HDR4_2_ptr = 	203			;
extern unsigned int TRAFFIC_HDR4_1_ptr = 	204			;
extern unsigned int TRAFFIC_HDR4_0_ptr = 	205			;
extern unsigned int TRAFFIC_HDR5_3_ptr = 	206			;
extern unsigned int TRAFFIC_HDR5_2_ptr = 	207			;
extern unsigned int TRAFFIC_HDR5_1_ptr = 	208			;
extern unsigned int TRAFFIC_HDR5_0_ptr = 	209			;
extern unsigned int TRAFFIC_HDR6_3_ptr = 	210			;
extern unsigned int TRAFFIC_HDR6_2_ptr = 	211			;
extern unsigned int TRAFFIC_HDR6_1_ptr = 	212			;
extern unsigned int TRAFFIC_HDR6_0_ptr = 	213			;
extern unsigned int TRAFFIC_HDR7_3_ptr = 	214			;
extern unsigned int TRAFFIC_HDR7_2_ptr = 	215			;
extern unsigned int TRAFFIC_HDR7_1_ptr = 	216			;
extern unsigned int TRAFFIC_HDR7_0_ptr = 	217			;
extern unsigned int TRAFFIC_HDR8_3_ptr = 	218			;
extern unsigned int TRAFFIC_HDR8_2_ptr = 	219			;
extern unsigned int TRAFFIC_HDR8_1_ptr = 	220			;
extern unsigned int TRAFFIC_HDR8_0_ptr = 	221			;
extern unsigned int TRAFFIC_HDR9_3_ptr = 	222			;
extern unsigned int TRAFFIC_HDR9_2_ptr = 	223			;
extern unsigned int TRAFFIC_HDR9_1_ptr = 	224			;
extern unsigned int TRAFFIC_HDR9_0_ptr = 	225			;
extern unsigned int TRAFFIC_HDR10_3_ptr = 	226			;
extern unsigned int TRAFFIC_HDR10_2_ptr = 	227			;
extern unsigned int TRAFFIC_HDR10_1_ptr = 	228			;
extern unsigned int TRAFFIC_HDR10_0_ptr = 	229			;
extern unsigned int TRAFFIC_HDR11_3_ptr = 	230			;
extern unsigned int TRAFFIC_HDR11_2_ptr = 	231			;
extern unsigned int TRAFFIC_HDR11_1_ptr = 	232			;
extern unsigned int TRAFFIC_HDR11_0_ptr = 	233			;
extern unsigned int TRAFFIC_HDR12_3_ptr = 	234			;
extern unsigned int TRAFFIC_HDR12_2_ptr = 	235			;
extern unsigned int TRAFFIC_HDR12_1_ptr = 	236			;
extern unsigned int TRAFFIC_HDR12_0_ptr = 	237			;
extern unsigned int TRAFFIC_HDR13_3_ptr = 	238			;
extern unsigned int TRAFFIC_HDR13_2_ptr = 	239			;
extern unsigned int TRAFFIC_HDR13_1_ptr = 	240			;
extern unsigned int TRAFFIC_HDR13_0_ptr = 	241			;
extern unsigned int TRAFFIC_HDR14_3_ptr = 	242			;
extern unsigned int TRAFFIC_HDR14_2_ptr = 	243			;
extern unsigned int TRAFFIC_HDR14_1_ptr = 	244			;
extern unsigned int TRAFFIC_HDR14_0_ptr = 	245			;
extern unsigned int TRAFFIC_HDR15_3_ptr = 	246			;
extern unsigned int TRAFFIC_HDR15_2_ptr = 	247			;
extern unsigned int TRAFFIC_HDR15_1_ptr = 	248			;
extern unsigned int TRAFFIC_HDR15_0_ptr = 	249			;
extern unsigned int TRAFFIC_HDR16_3_ptr = 	250			;
extern unsigned int TRAFFIC_HDR16_2_ptr = 	251			;
extern unsigned int TRAFFIC_HDR16_1_ptr = 	252			;
extern unsigned int TRAFFIC_HDR16_0_ptr = 	253			;
extern unsigned int TRAFFIC_HDR17_3_ptr = 	254			;
extern unsigned int TRAFFIC_HDR17_2_ptr = 	255			;
extern unsigned int TRAFFIC_HDR17_1_ptr = 	256			;
extern unsigned int TRAFFIC_HDR17_0_ptr = 	257			;
extern unsigned int TRAFFIC_HDR18_3_ptr = 	258			;
extern unsigned int TRAFFIC_HDR18_2_ptr = 	259			;
extern unsigned int TRAFFIC_HDR18_1_ptr = 	260			;
extern unsigned int TRAFFIC_HDR18_0_ptr = 	261			;
extern unsigned int TRAFFIC_HDR19_3_ptr = 	262			;
extern unsigned int TRAFFIC_HDR19_2_ptr = 	263			;
extern unsigned int TRAFFIC_HDR19_1_ptr = 	264			;
extern unsigned int TRAFFIC_HDR19_0_ptr = 	265			;
extern unsigned int TRAFFIC_HDR20_3_ptr = 	266			;
extern unsigned int TRAFFIC_HDR20_2_ptr = 	267			;
extern unsigned int TRAFFIC_HDR20_1_ptr = 	268			;
extern unsigned int TRAFFIC_HDR20_0_ptr = 	269			;
extern unsigned int TRAFFIC_HDR21_3_ptr = 	270			;
extern unsigned int TRAFFIC_HDR21_2_ptr = 	271			;
extern unsigned int TRAFFIC_HDR21_1_ptr = 	272			;
extern unsigned int TRAFFIC_HDR21_0_ptr = 	273			;
extern unsigned int TRAFFIC_HDR22_3_ptr = 	274			;
extern unsigned int TRAFFIC_HDR22_2_ptr = 	275			;
extern unsigned int TRAFFIC_HDR22_1_ptr = 	276			;
extern unsigned int TRAFFIC_HDR22_0_ptr = 	277			;
extern unsigned int TRAFFIC_HDR23_3_ptr = 	278			;
extern unsigned int TRAFFIC_HDR23_2_ptr = 	279			;
extern unsigned int TRAFFIC_HDR23_1_ptr = 	280			;
extern unsigned int TRAFFIC_HDR23_0_ptr = 	281			;
extern unsigned int TRAFFIC_HDR24_3_ptr = 	282			;
extern unsigned int TRAFFIC_HDR24_2_ptr = 	283			;
extern unsigned int TRAFFIC_HDR24_1_ptr = 	284			;
extern unsigned int TRAFFIC_HDR24_0_ptr = 	285			;
extern unsigned int TRAFFIC_HDR25_3_ptr = 	286			;
extern unsigned int TRAFFIC_HDR25_2_ptr = 	287			;
extern unsigned int TRAFFIC_HDR25_1_ptr = 	288			;
extern unsigned int TRAFFIC_HDR25_0_ptr = 	289			;
extern unsigned int TRAFFIC_HDR26_3_ptr = 	290			;
extern unsigned int TRAFFIC_HDR26_2_ptr = 	291			;
extern unsigned int TRAFFIC_HDR26_1_ptr = 	292			;
extern unsigned int TRAFFIC_HDR26_0_ptr = 	293			;
extern unsigned int TRAFFIC_HDR27_3_ptr = 	294			;
extern unsigned int TRAFFIC_HDR27_2_ptr = 	295			;
extern unsigned int TRAFFIC_HDR27_1_ptr = 	296			;
extern unsigned int TRAFFIC_HDR27_0_ptr = 	297			;
extern unsigned int TRAFFIC_HDR28_3_ptr = 	298			;
extern unsigned int TRAFFIC_HDR28_2_ptr = 	299			;
extern unsigned int TRAFFIC_HDR28_1_ptr = 	300			;
extern unsigned int TRAFFIC_HDR28_0_ptr = 	301			;
extern unsigned int TRAFFIC_HDR29_3_ptr = 	302			;
extern unsigned int TRAFFIC_HDR29_2_ptr = 	303			;
extern unsigned int TRAFFIC_HDR29_1_ptr = 	304			;
extern unsigned int TRAFFIC_HDR29_0_ptr = 	305			;
extern unsigned int TRAFFIC_HDR30_3_ptr = 	306			;
extern unsigned int TRAFFIC_HDR30_2_ptr = 	307			;
extern unsigned int TRAFFIC_HDR30_1_ptr = 	308			;
extern unsigned int TRAFFIC_HDR30_0_ptr = 	309			;
extern unsigned int TRAFFIC_HDR31_3_ptr = 	310			;
extern unsigned int TRAFFIC_HDR31_2_ptr = 	311			;
extern unsigned int TRAFFIC_HDR31_1_ptr = 	312			;
extern unsigned int TRAFFIC_HDR31_0_ptr = 	313			;
				;
extern unsigned int TRAFFIC_MSK0_3_ptr = 	314			;
extern unsigned int TRAFFIC_MSK0_2_ptr = 	315			;
extern unsigned int TRAFFIC_MSK0_1_ptr = 	316			;
extern unsigned int TRAFFIC_MSK0_0_ptr = 	317			;
extern unsigned int TRAFFIC_MSK1_3_ptr = 	318			;
extern unsigned int TRAFFIC_MSK1_2_ptr = 	319			;
extern unsigned int TRAFFIC_MSK1_1_ptr = 	320			;
extern unsigned int TRAFFIC_MSK1_0_ptr = 	321			;
extern unsigned int TRAFFIC_MSK2_3_ptr = 	322			;
extern unsigned int TRAFFIC_MSK2_2_ptr = 	323			;
extern unsigned int TRAFFIC_MSK2_1_ptr = 	324			;
extern unsigned int TRAFFIC_MSK2_0_ptr = 	325			;
extern unsigned int TRAFFIC_MSK3_3_ptr = 	326			;
extern unsigned int TRAFFIC_MSK3_2_ptr = 	327			;
extern unsigned int TRAFFIC_MSK3_1_ptr = 	328			;
extern unsigned int TRAFFIC_MSK3_0_ptr = 	329			;
extern unsigned int TRAFFIC_MSK4_3_ptr = 	330			;
extern unsigned int TRAFFIC_MSK4_2_ptr = 	331			;
extern unsigned int TRAFFIC_MSK4_1_ptr = 	332			;
extern unsigned int TRAFFIC_MSK4_0_ptr = 	333			;
extern unsigned int TRAFFIC_MSK5_3_ptr = 	334			;
extern unsigned int TRAFFIC_MSK5_2_ptr = 	335			;
extern unsigned int TRAFFIC_MSK5_1_ptr = 	336			;
extern unsigned int TRAFFIC_MSK5_0_ptr = 	337			;
extern unsigned int TRAFFIC_MSK6_3_ptr = 	338			;
extern unsigned int TRAFFIC_MSK6_2_ptr = 	339			;
extern unsigned int TRAFFIC_MSK6_1_ptr = 	340			;
extern unsigned int TRAFFIC_MSK6_0_ptr = 	341			;
extern unsigned int TRAFFIC_MSK7_3_ptr = 	342			;
extern unsigned int TRAFFIC_MSK7_2_ptr = 	343			;
extern unsigned int TRAFFIC_MSK7_1_ptr = 	344			;
extern unsigned int TRAFFIC_MSK7_0_ptr = 	345			;
extern unsigned int TRAFFIC_MSK8_3_ptr = 	346			;
extern unsigned int TRAFFIC_MSK8_2_ptr = 	347			;
extern unsigned int TRAFFIC_MSK8_1_ptr = 	348			;
extern unsigned int TRAFFIC_MSK8_0_ptr = 	349			;
extern unsigned int TRAFFIC_MSK9_3_ptr = 	350			;
extern unsigned int TRAFFIC_MSK9_2_ptr = 	351			;
extern unsigned int TRAFFIC_MSK9_1_ptr = 	352			;
extern unsigned int TRAFFIC_MSK9_0_ptr = 	353			;
extern unsigned int TRAFFIC_MSK10_3_ptr = 	354			;
extern unsigned int TRAFFIC_MSK10_2_ptr = 	355			;
extern unsigned int TRAFFIC_MSK10_1_ptr = 	356			;
extern unsigned int TRAFFIC_MSK10_0_ptr = 	357			;
extern unsigned int TRAFFIC_MSK11_3_ptr = 	358			;
extern unsigned int TRAFFIC_MSK11_2_ptr = 	359			;
extern unsigned int TRAFFIC_MSK11_1_ptr = 	360			;
extern unsigned int TRAFFIC_MSK11_0_ptr = 	361			;
extern unsigned int TRAFFIC_MSK12_3_ptr = 	362			;
extern unsigned int TRAFFIC_MSK12_2_ptr = 	363			;
extern unsigned int TRAFFIC_MSK12_1_ptr = 	364			;
extern unsigned int TRAFFIC_MSK12_0_ptr = 	365			;
extern unsigned int TRAFFIC_MSK13_3_ptr = 	366			;
extern unsigned int TRAFFIC_MSK13_2_ptr = 	367			;
extern unsigned int TRAFFIC_MSK13_1_ptr = 	368			;
extern unsigned int TRAFFIC_MSK13_0_ptr = 	369			;
extern unsigned int TRAFFIC_MSK14_3_ptr = 	370			;
extern unsigned int TRAFFIC_MSK14_2_ptr = 	371			;
extern unsigned int TRAFFIC_MSK14_1_ptr = 	372			;
extern unsigned int TRAFFIC_MSK14_0_ptr = 	373			;
extern unsigned int TRAFFIC_MSK15_3_ptr = 	374			;
extern unsigned int TRAFFIC_MSK15_2_ptr = 	375			;
extern unsigned int TRAFFIC_MSK15_1_ptr = 	376			;
extern unsigned int TRAFFIC_MSK15_0_ptr = 	377			;
extern unsigned int TRAFFIC_MSK16_3_ptr = 	378			;
extern unsigned int TRAFFIC_MSK16_2_ptr = 	379			;
extern unsigned int TRAFFIC_MSK16_1_ptr = 	380			;
extern unsigned int TRAFFIC_MSK16_0_ptr = 	381			;
extern unsigned int TRAFFIC_MSK17_3_ptr = 	382			;
extern unsigned int TRAFFIC_MSK17_2_ptr = 	383			;
extern unsigned int TRAFFIC_MSK17_1_ptr = 	384			;
extern unsigned int TRAFFIC_MSK17_0_ptr = 	385			;
extern unsigned int TRAFFIC_MSK18_3_ptr = 	386			;
extern unsigned int TRAFFIC_MSK18_2_ptr = 	387			;
extern unsigned int TRAFFIC_MSK18_1_ptr = 	388			;
extern unsigned int TRAFFIC_MSK18_0_ptr = 	389			;
extern unsigned int TRAFFIC_MSK19_3_ptr = 	390			;
extern unsigned int TRAFFIC_MSK19_2_ptr = 	391			;
extern unsigned int TRAFFIC_MSK19_1_ptr = 	392			;
extern unsigned int TRAFFIC_MSK19_0_ptr = 	393			;
extern unsigned int TRAFFIC_MSK20_3_ptr = 	394			;
extern unsigned int TRAFFIC_MSK20_2_ptr = 	395			;
extern unsigned int TRAFFIC_MSK20_1_ptr = 	396			;
extern unsigned int TRAFFIC_MSK20_0_ptr = 	397			;
extern unsigned int TRAFFIC_MSK21_3_ptr = 	398			;
extern unsigned int TRAFFIC_MSK21_2_ptr = 	399			;
extern unsigned int TRAFFIC_MSK21_1_ptr = 	400			;
extern unsigned int TRAFFIC_MSK21_0_ptr = 	401			;
extern unsigned int TRAFFIC_MSK22_3_ptr = 	402			;
extern unsigned int TRAFFIC_MSK22_2_ptr = 	403			;
extern unsigned int TRAFFIC_MSK22_1_ptr = 	404			;
extern unsigned int TRAFFIC_MSK22_0_ptr = 	405			;
extern unsigned int TRAFFIC_MSK23_3_ptr = 	406			;
extern unsigned int TRAFFIC_MSK23_2_ptr = 	407			;
extern unsigned int TRAFFIC_MSK23_1_ptr = 	408			;
extern unsigned int TRAFFIC_MSK23_0_ptr = 	409			;
extern unsigned int TRAFFIC_MSK24_3_ptr = 	410			;
extern unsigned int TRAFFIC_MSK24_2_ptr = 	411			;
extern unsigned int TRAFFIC_MSK24_1_ptr = 	412			;
extern unsigned int TRAFFIC_MSK24_0_ptr = 	413			;
extern unsigned int TRAFFIC_MSK25_3_ptr =  414				;
extern unsigned int TRAFFIC_MSK25_2_ptr = 	415			;
extern unsigned int TRAFFIC_MSK25_1_ptr =  416				;
extern unsigned int TRAFFIC_MSK25_0_ptr = 	417			;
extern unsigned int TRAFFIC_MSK26_3_ptr = 	418			;
extern unsigned int TRAFFIC_MSK26_2_ptr = 	419			;
extern unsigned int TRAFFIC_MSK26_1_ptr = 	420			;
extern unsigned int TRAFFIC_MSK26_0_ptr = 	421			;
extern unsigned int TRAFFIC_MSK27_3_ptr = 	422			;
extern unsigned int TRAFFIC_MSK27_2_ptr = 	423			;
extern unsigned int TRAFFIC_MSK27_1_ptr = 	424			;
extern unsigned int TRAFFIC_MSK27_0_ptr = 	425			;
extern unsigned int TRAFFIC_MSK28_3_ptr = 	426			;
extern unsigned int TRAFFIC_MSK28_2_ptr = 	427			;
extern unsigned int TRAFFIC_MSK28_1_ptr = 	428			;
extern unsigned int TRAFFIC_MSK28_0_ptr = 	429			;
extern unsigned int TRAFFIC_MSK29_3_ptr = 	430			;
extern unsigned int TRAFFIC_MSK29_2_ptr = 	431			;
extern unsigned int TRAFFIC_MSK29_1_ptr = 	432			;
extern unsigned int TRAFFIC_MSK29_0_ptr = 	433			;
extern unsigned int TRAFFIC_MSK30_3_ptr = 	434			;
extern unsigned int TRAFFIC_MSK30_2_ptr = 	435			;
extern unsigned int TRAFFIC_MSK30_1_ptr = 	436			;
extern unsigned int TRAFFIC_MSK30_0_ptr = 	437			;
extern unsigned int TRAFFIC_MSK31_3_ptr = 	438			;
extern unsigned int TRAFFIC_MSK31_2_ptr = 	439			;
extern unsigned int TRAFFIC_MSK31_1_ptr = 	440			;
extern unsigned int TRAFFIC_MSK31_0_ptr = 	441			;
*/

#define Spare_Checksum_ptr		5468
#define Checksum_ptr			5469					//(MaxConfig_ms-1)
//#define ConfigChecksum2			(MaxConfig_ms-1)		//5470	//(MaxConfig_ms-1)

#endif /* CONFSTAT_6400_H_ */
