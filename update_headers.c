#include "gigle.h"
#include "extvars.h"

//void update_headers=(short streamNo, short size_offset,short stream_offset)
void update_headers(short streamNo)
{

			 TRAFFIC_GEN_PKT_EMIXUSERSIZE5_ptr = (size_offset*streamNo)+(stream_offset + 0)	;
	     	 TRAFFIC_GEN_PKT_EMIXUSERSIZE4_ptr = (size_offset*streamNo)+(stream_offset + 1)	;

	     	 TRAFFIC_MON_STREAM_CNTL3_ptr	=(size_offset*streamNo)+(stream_offset + 2)	;	  // Packet ID for MODULE TYPE,	(eg DS3, DS1A, DS1B, PRI, DS0, etc.)
	         TRAFFIC_MON_STREAM_CNTL2_ptr	=(size_offset*streamNo)+(stream_offset + 3)	;	// ^0:Copper/Fiber 0=copper,1=fiber, //not used:^1&2=speed:00=10,01=100,10=1G,11=10G
	         TRAFFIC_MON_STREAM_CNTL1_ptr	=(size_offset*streamNo)+(stream_offset + 4)	;	// MON stream control (for independent capture)
	         TRAFFIC_MON_STREAM_CNTL0_ptr	=(size_offset*streamNo)+(stream_offset + 5)	;

	         TRAFFIC_RATE_TYPE		=(size_offset*streamNo)+(stream_offset + 6)	;	// Traffic Rate  0-FFFFFF:  valid%=F4240: 0-999999,1000000, div1e4 = 99.9999,100.0000%
	         TRAFFIC_RATE3_ptr    	=(size_offset*streamNo)+(stream_offset + 7)	;	// ie Gbps   0.0001-100.0G,  includes 99.9999 or 9.9999G or 0.9999G
	         TRAFFIC_RATE2_ptr     	=(size_offset*streamNo)+(stream_offset + 8)	;	//  ie Mbps   0.0001-100.0M, includes 99.9999 or 9.9999M or 0.9999M
	         TRAFFIC_RATE1_ptr     	=(size_offset*streamNo)+(stream_offset + 9)	;	// 2-byte spares for 100G
	         TRAFFIC_RATE0_ptr     	=(size_offset*streamNo)+(stream_offset + 10)	;

	         TRAFFIC_GEN_PKTPERCENT11_ptr	=(size_offset*streamNo)+(stream_offset + 11)	    ; //not in use
	         TRAFFIC_GEN_PKTPERCENT10_ptr	=(size_offset*streamNo)+(stream_offset + 12)	;
	         TRAFFIC_GEN_PKTPERCENT03_ptr	=(size_offset*streamNo)+(stream_offset + 13)	;
	         TRAFFIC_GEN_PKTPERCENT02_ptr	=(size_offset*streamNo)+(stream_offset + 14)	;
	         TRAFFIC_GEN_PKTPERCENT01_ptr	=(size_offset*streamNo)+(stream_offset + 15)	;
	         TRAFFIC_GEN_PKTPERCENT00_ptr	=(size_offset*streamNo)+(stream_offset + 16)	;

	         EGEN_PKT_TYPE_ptr 		=(size_offset*streamNo)+(stream_offset + 17)	;
	         EGEN_PKT_TYPE_ptr_1 	=(size_offset*streamNo)+(stream_offset + 18)	;
	         EGEN_PKT_TYPE_ptr_2 	=(size_offset*streamNo)+(stream_offset + 19)	;

	         TRAFFIC_GEN_CONFIG33_ptr		=(size_offset*streamNo)+(stream_offset + 20)	; //Traffic_gen_config 4
			 TRAFFIC_GEN_CONFIG32_ptr		=(size_offset*streamNo)+(stream_offset + 21)	;
			 TRAFFIC_GEN_CONFIG31_ptr		=(size_offset*streamNo)+(stream_offset + 22)	;
			 TRAFFIC_GEN_CONFIG30_ptr		=(size_offset*streamNo)+(stream_offset + 23)	;

	         TRAFFIC_GEN_CONFIG23_ptr	=(size_offset*streamNo)+(stream_offset + 24)	;
	         TRAFFIC_GEN_CONFIG22_ptr	=(size_offset*streamNo)+(stream_offset + 25)	;
	         TRAFFIC_GEN_CONFIG21_ptr	=(size_offset*streamNo)+(stream_offset + 26)	;
	         TRAFFIC_GEN_CONFIG20_ptr	=(size_offset*streamNo)+(stream_offset + 27)	;

	         TRAFFIC_GEN_CONFIG13_ptr	=(size_offset*streamNo)+(stream_offset + 28)	; // TRAFFIC_GEN_CONFIG: Payload: inc/prbs, Size:EMIX or fixed
	         TRAFFIC_GEN_CONFIG12_ptr	=(size_offset*streamNo)+(stream_offset + 29)	;
	         TRAFFIC_GEN_CONFIG11_ptr	=(size_offset*streamNo)+(stream_offset + 30)	; //ipv4_offset
	         TRAFFIC_GEN_CONFIG10_ptr	=(size_offset*streamNo)+(stream_offset + 31)	; //length_offset

	         TRAFFIC_GEN_CONFIG03_ptr	=(size_offset*streamNo)+(stream_offset + 32)	; //hdr_length	//TRAFFIC_GEN_CONFIG2
	         TRAFFIC_GEN_CONFIG02_ptr	=(size_offset*streamNo)+(stream_offset + 33)	; //hdr_length
	         TRAFFIC_GEN_CONFIG01_ptr	=(size_offset*streamNo)+(stream_offset + 34)	; //traffic_pkt_length
	         TRAFFIC_GEN_CONFIG00_ptr	=(size_offset*streamNo)+(stream_offset + 35)	; //traffic_pkt_length*/

	         TRAFFIC_GEN_PKT_EMIXUSERSIZE3_ptr	=(size_offset*streamNo)+(stream_offset + 36)	;
	         TRAFFIC_GEN_PKT_EMIXUSERSIZE2_ptr	=(size_offset*streamNo)+(stream_offset + 37)	;				//		0x00000001
	         TRAFFIC_GEN_PKT_EMIXUSERSIZE1_ptr	=(size_offset*streamNo)+(stream_offset + 38)	;				//		0x000000FE
	         TRAFFIC_GEN_PKT_EMIXUSERSIZE0_ptr	=(size_offset*streamNo)+(stream_offset + 39)	;				//		0x0003FF00

	         TRAFFIC_GEN_EMIXSEQ33_ptr  	=(size_offset*streamNo)+(stream_offset + 40)	;
	         TRAFFIC_GEN_EMIXSEQ32_ptr  	=(size_offset*streamNo)+(stream_offset + 41)	;
	         TRAFFIC_GEN_EMIXSEQ31_ptr  	=(size_offset*streamNo)+(stream_offset + 42)	;
	         TRAFFIC_GEN_EMIXSEQ30_ptr  	=(size_offset*streamNo)+(stream_offset + 43)	;
	         TRAFFIC_GEN_EMIXSEQ23_ptr		=(size_offset*streamNo)+(stream_offset + 44)	;
	         TRAFFIC_GEN_EMIXSEQ22_ptr  	=(size_offset*streamNo)+(stream_offset + 45)	;
	         TRAFFIC_GEN_EMIXSEQ21_ptr  	=(size_offset*streamNo)+(stream_offset + 46)	;
	         TRAFFIC_GEN_EMIXSEQ20_ptr 		=(size_offset*streamNo)+(stream_offset + 47)	;
	         TRAFFIC_GEN_EMIXSEQ13_ptr		=(size_offset*streamNo)+(stream_offset + 48)	;
	         TRAFFIC_GEN_EMIXSEQ12_ptr  	=(size_offset*streamNo)+(stream_offset + 49)	;
	         TRAFFIC_GEN_EMIXSEQ11_ptr		=(size_offset*streamNo)+(stream_offset + 50)	;
	         TRAFFIC_GEN_EMIXSEQ10_ptr		=(size_offset*streamNo)+(stream_offset + 51)	;
	         TRAFFIC_GEN_EMIXSEQ03_ptr 		=(size_offset*streamNo)+(stream_offset + 52)	;				//(good to set 1,2&3 def as 0xF0000000)
	         TRAFFIC_GEN_EMIXSEQ02_ptr 		=(size_offset*streamNo)+(stream_offset + 53)	;
	         TRAFFIC_GEN_EMIXSEQ01_ptr		=(size_offset*streamNo)+(stream_offset + 54)	;
	         TRAFFIC_GEN_EMIXSEQ00_ptr  	=(size_offset*streamNo)+(stream_offset + 55)	;

	         Y1564_CIR_3_ptr	=(size_offset*streamNo)+(stream_offset + 56)	;
	         Y1564_CIR_2_ptr	=(size_offset*streamNo)+(stream_offset + 57)	;		//Committed information rate
	         Y1564_CIR_1_ptr	=(size_offset*streamNo)+(stream_offset + 58)	;
	         Y1564_CIR_0_ptr	=(size_offset*streamNo)+(stream_offset + 59)	;

	         Y1564_EIR_3_ptr	=(size_offset*streamNo)+(stream_offset + 60)	;		 //Expected information rate
	         Y1564_EIR_2_ptr	=(size_offset*streamNo)+(stream_offset + 61)	;
	         Y1564_EIR_1_ptr	=(size_offset*streamNo)+(stream_offset + 62)	;
	         Y1564_EIR_0_ptr	=(size_offset*streamNo)+(stream_offset + 63)	;

	         Y1564_CIR_STEPS_ptr	=(size_offset*streamNo)+(stream_offset + 64)	;			//Steps for CIR – first 4 bits

	         Y1564_MAX_FTD_3_ptr 	=(size_offset*streamNo)+(stream_offset + 65)	;				//Expected information rate
	         Y1564_MAX_FTD_2_ptr 	=(size_offset*streamNo)+(stream_offset + 66)	;				//Expected information rate
	         Y1564_MAX_FTD_1_ptr 	=(size_offset*streamNo)+(stream_offset + 67)	;				//Expected information rate
	         Y1564_MAX_FTD_0_ptr 	=(size_offset*streamNo)+(stream_offset + 68)	;				//Expected information rate

	         Y1564_MAX_FDV_3_ptr 	=(size_offset*streamNo)+(stream_offset + 69)	;
	         Y1564_MAX_FDV_2_ptr 	=(size_offset*streamNo)+(stream_offset + 70)	;		//4 byte Float
	         Y1564_MAX_FDV_1_ptr 	=(size_offset*streamNo)+(stream_offset + 71)	;
	         Y1564_MAX_FDV_0_ptr 	=(size_offset*streamNo)+(stream_offset + 72)	;

	         Y1564_MAX_FRAME_LOSS_3_ptr 	=(size_offset*streamNo)+(stream_offset + 73)	;
	         Y1564_MAX_FRAME_LOSS_2_ptr 	=(size_offset*streamNo)+(stream_offset + 74)	;		//4 byte Float
	         Y1564_MAX_FRAME_LOSS_1_ptr 	=(size_offset*streamNo)+(stream_offset + 75)	;
	         Y1564_MAX_FRAME_LOSS_0_ptr 	=(size_offset*streamNo)+(stream_offset + 76)	;

	         Y1564_MAX_AVAIL_3_ptr 	=(size_offset*streamNo)+(stream_offset + 77)	;
	         Y1564_MAX_AVAIL_2_ptr 	=(size_offset*streamNo)+(stream_offset + 78)	;	//4 byte Float
	         Y1564_MAX_AVAIL_1_ptr 	=(size_offset*streamNo)+(stream_offset + 79)	;
	         Y1564_MAX_AVAIL_0_ptr 	=(size_offset*streamNo)+(stream_offset + 80)	;

	         SPARE_THRESHOLD_03	=(size_offset*streamNo)+(stream_offset + 81)	;
	         SPARE_THRESHOLD_02	=(size_offset*streamNo)+(stream_offset + 82)	;		//4 byte Float
	         SPARE_THRESHOLD_01	=(size_offset*streamNo)+(stream_offset + 83)	;
	         SPARE_THRESHOLD_00	=(size_offset*streamNo)+(stream_offset + 84)	;

	         TRAFFIC_GEN_SPARE_ptr	=(size_offset*streamNo)+(stream_offset + 85)	;
	         TRAFFIC_HDR_DEF_ptr	=(size_offset*streamNo)+(stream_offset + 86)	;

	         TRAFFIC_HDR0_3_ptr	=(size_offset*streamNo)+(stream_offset + 87)	; 	//MAC address
	         TRAFFIC_HDR0_2_ptr	=(size_offset*streamNo)+(stream_offset + 88)	;
	         TRAFFIC_HDR0_1_ptr	=(size_offset*streamNo)+(stream_offset + 89)	;
	         TRAFFIC_HDR0_0_ptr	=(size_offset*streamNo)+(stream_offset + 90)	;

	         TRAFFIC_HDR1_3_ptr	=(size_offset*streamNo)+(stream_offset + 91)	;			// additional instructional info for what is contained in HDR ptr's
	         TRAFFIC_HDR1_2_ptr	=(size_offset*streamNo)+(stream_offset + 92)	;
	         TRAFFIC_HDR1_1_ptr	=(size_offset*streamNo)+(stream_offset + 93)	;
	         TRAFFIC_HDR1_0_ptr	=(size_offset*streamNo)+(stream_offset + 94)	;

	         TRAFFIC_HDR2_3_ptr	=(size_offset*streamNo)+(stream_offset + 95)	;
	         TRAFFIC_HDR2_2_ptr	=(size_offset*streamNo)+(stream_offset + 96)	;
	         TRAFFIC_HDR2_1_ptr	=(size_offset*streamNo)+(stream_offset + 97)	;
	         TRAFFIC_HDR2_0_ptr	=(size_offset*streamNo)+(stream_offset + 98)	; //MAC address

	         TRAFFIC_HDR3_3_ptr	=(size_offset*streamNo)+(stream_offset + 99)	;
	         TRAFFIC_HDR3_2_ptr	=(size_offset*streamNo)+(stream_offset + 100)	;
	         TRAFFIC_HDR3_1_ptr	=(size_offset*streamNo)+(stream_offset + 101)	;
	         TRAFFIC_HDR3_0_ptr	=(size_offset*streamNo)+(stream_offset + 102)	;

	         TRAFFIC_HDR4_3_ptr	=(size_offset*streamNo)+(stream_offset + 103)	;
	         TRAFFIC_HDR4_2_ptr	=(size_offset*streamNo)+(stream_offset + 104)	;
	         TRAFFIC_HDR4_1_ptr	=(size_offset*streamNo)+(stream_offset + 105)	;
	         TRAFFIC_HDR4_0_ptr	=(size_offset*streamNo)+(stream_offset + 106)	;

	         TRAFFIC_HDR5_3_ptr	=(size_offset*streamNo)+(stream_offset + 107)	;
	         TRAFFIC_HDR5_2_ptr	=(size_offset*streamNo)+(stream_offset + 108)	;
	         TRAFFIC_HDR5_1_ptr	=(size_offset*streamNo)+(stream_offset + 109)	;
	         TRAFFIC_HDR5_0_ptr	=(size_offset*streamNo)+(stream_offset + 110)	;

	         TRAFFIC_HDR6_3_ptr	=(size_offset*streamNo)+(stream_offset + 111)	;
	         TRAFFIC_HDR6_2_ptr	=(size_offset*streamNo)+(stream_offset + 112)	;
	         TRAFFIC_HDR6_1_ptr	=(size_offset*streamNo)+(stream_offset + 113)	;
	         TRAFFIC_HDR6_0_ptr	=(size_offset*streamNo)+(stream_offset + 114)	;

	         TRAFFIC_HDR7_3_ptr	=(size_offset*streamNo)+(stream_offset + 115)	;
	         TRAFFIC_HDR7_2_ptr	=(size_offset*streamNo)+(stream_offset + 116)	;
	         TRAFFIC_HDR7_1_ptr	=(size_offset*streamNo)+(stream_offset + 117)	;
	         TRAFFIC_HDR7_0_ptr	=(size_offset*streamNo)+(stream_offset + 118)	;

	         TRAFFIC_HDR8_3_ptr	=(size_offset*streamNo)+(stream_offset + 119)	;
	         TRAFFIC_HDR8_2_ptr	=(size_offset*streamNo)+(stream_offset + 120)	;
	         TRAFFIC_HDR8_1_ptr	=(size_offset*streamNo)+(stream_offset + 121)	;
	         TRAFFIC_HDR8_0_ptr	=(size_offset*streamNo)+(stream_offset + 122)	;

	         TRAFFIC_HDR9_3_ptr	=(size_offset*streamNo)+(stream_offset + 123)	;
	         TRAFFIC_HDR9_2_ptr	=(size_offset*streamNo)+(stream_offset + 124)	;
	         TRAFFIC_HDR9_1_ptr	=(size_offset*streamNo)+(stream_offset + 125)	;
	         TRAFFIC_HDR9_0_ptr	=(size_offset*streamNo)+(stream_offset + 126)	;

	         TRAFFIC_HDR10_3_ptr	=(size_offset*streamNo)+(stream_offset + 127)	;
	         TRAFFIC_HDR10_2_ptr	=(size_offset*streamNo)+(stream_offset + 128)	;
	         TRAFFIC_HDR10_1_ptr	=(size_offset*streamNo)+(stream_offset + 129)	;
	         TRAFFIC_HDR10_0_ptr	=(size_offset*streamNo)+(stream_offset + 130)	;

	         TRAFFIC_HDR11_3_ptr	=(size_offset*streamNo)+(stream_offset + 131)	;
	         TRAFFIC_HDR11_2_ptr	=(size_offset*streamNo)+(stream_offset + 132)	;
	         TRAFFIC_HDR11_1_ptr	=(size_offset*streamNo)+(stream_offset + 133)	;
	         TRAFFIC_HDR11_0_ptr	=(size_offset*streamNo)+(stream_offset + 134)	;

	         TRAFFIC_HDR12_3_ptr	=(size_offset*streamNo)+(stream_offset + 135)	;
	         TRAFFIC_HDR12_2_ptr	=(size_offset*streamNo)+(stream_offset + 136)	;
	         TRAFFIC_HDR12_1_ptr	=(size_offset*streamNo)+(stream_offset + 137)	;
	         TRAFFIC_HDR12_0_ptr	=(size_offset*streamNo)+(stream_offset + 138)	;

	         TRAFFIC_HDR13_3_ptr	=(size_offset*streamNo)+(stream_offset + 139)	;
	         TRAFFIC_HDR13_2_ptr	=(size_offset*streamNo)+(stream_offset + 140)	;
	         TRAFFIC_HDR13_1_ptr	=(size_offset*streamNo)+(stream_offset + 141)	;
	         TRAFFIC_HDR13_0_ptr	=(size_offset*streamNo)+(stream_offset + 142)	;

	         TRAFFIC_HDR14_3_ptr	=(size_offset*streamNo)+(stream_offset + 143)	;
	         TRAFFIC_HDR14_2_ptr	=(size_offset*streamNo)+(stream_offset + 144)	;
	         TRAFFIC_HDR14_1_ptr	=(size_offset*streamNo)+(stream_offset + 145)	;
	         TRAFFIC_HDR14_0_ptr	=(size_offset*streamNo)+(stream_offset + 146)	;

	         TRAFFIC_HDR15_3_ptr	=(size_offset*streamNo)+(stream_offset + 147)	;
	         TRAFFIC_HDR15_2_ptr	=(size_offset*streamNo)+(stream_offset + 148)	;
	         TRAFFIC_HDR15_1_ptr	=(size_offset*streamNo)+(stream_offset + 149)	;
	         TRAFFIC_HDR15_0_ptr	=(size_offset*streamNo)+(stream_offset + 150)	;

	         TRAFFIC_HDR16_3_ptr	=(size_offset*streamNo)+(stream_offset + 151)	;
	         TRAFFIC_HDR16_2_ptr	=(size_offset*streamNo)+(stream_offset + 152)	;
	         TRAFFIC_HDR16_1_ptr	=(size_offset*streamNo)+(stream_offset + 153)	;
	         TRAFFIC_HDR16_0_ptr	=(size_offset*streamNo)+(stream_offset + 154)	;

	         TRAFFIC_HDR17_3_ptr	=(size_offset*streamNo)+(stream_offset + 155)	;
	         TRAFFIC_HDR17_2_ptr	=(size_offset*streamNo)+(stream_offset + 156)	;
	         TRAFFIC_HDR17_1_ptr	=(size_offset*streamNo)+(stream_offset + 157)	;
	         TRAFFIC_HDR17_0_ptr	=(size_offset*streamNo)+(stream_offset + 158)	;

	         TRAFFIC_HDR18_3_ptr	=(size_offset*streamNo)+(stream_offset + 159)	;
	         TRAFFIC_HDR18_2_ptr	=(size_offset*streamNo)+(stream_offset + 160)	;
	         TRAFFIC_HDR18_1_ptr	=(size_offset*streamNo)+(stream_offset + 161)	;
	         TRAFFIC_HDR18_0_ptr	=(size_offset*streamNo)+(stream_offset + 162)	;

	         TRAFFIC_HDR19_3_ptr	=(size_offset*streamNo)+(stream_offset + 163)	;
	         TRAFFIC_HDR19_2_ptr	=(size_offset*streamNo)+(stream_offset + 164)	;
	         TRAFFIC_HDR19_1_ptr	=(size_offset*streamNo)+(stream_offset + 165)	;
	         TRAFFIC_HDR19_0_ptr	=(size_offset*streamNo)+(stream_offset + 166)	;

	         TRAFFIC_HDR20_3_ptr	=(size_offset*streamNo)+(stream_offset + 167)	;
	         TRAFFIC_HDR20_2_ptr	=(size_offset*streamNo)+(stream_offset + 168)	;
	         TRAFFIC_HDR20_1_ptr	=(size_offset*streamNo)+(stream_offset + 169)	;
	         TRAFFIC_HDR20_0_ptr	=(size_offset*streamNo)+(stream_offset + 170)	;

	         TRAFFIC_HDR21_3_ptr	=(size_offset*streamNo)+(stream_offset + 171)	;
	         TRAFFIC_HDR21_2_ptr	=(size_offset*streamNo)+(stream_offset + 172)	;
	         TRAFFIC_HDR21_1_ptr	=(size_offset*streamNo)+(stream_offset + 173)	;
	         TRAFFIC_HDR21_0_ptr	=(size_offset*streamNo)+(stream_offset + 174)	;

	         TRAFFIC_HDR22_3_ptr	=(size_offset*streamNo)+(stream_offset + 175)	;
	         TRAFFIC_HDR22_2_ptr	=(size_offset*streamNo)+(stream_offset + 176)	;
	         TRAFFIC_HDR22_1_ptr	=(size_offset*streamNo)+(stream_offset + 177)	;
	         TRAFFIC_HDR22_0_ptr	=(size_offset*streamNo)+(stream_offset + 178)	;

	         TRAFFIC_HDR23_3_ptr	=(size_offset*streamNo)+(stream_offset + 179)	;
	         TRAFFIC_HDR23_2_ptr	=(size_offset*streamNo)+(stream_offset + 180)	;
	         TRAFFIC_HDR23_1_ptr	=(size_offset*streamNo)+(stream_offset + 181)	;
	         TRAFFIC_HDR23_0_ptr	=(size_offset*streamNo)+(stream_offset + 182)	;

	         TRAFFIC_HDR24_3_ptr	=(size_offset*streamNo)+(stream_offset + 183)	;
	         TRAFFIC_HDR24_2_ptr	=(size_offset*streamNo)+(stream_offset + 184)	;
	         TRAFFIC_HDR24_1_ptr	=(size_offset*streamNo)+(stream_offset + 185)	;
	         TRAFFIC_HDR24_0_ptr	=(size_offset*streamNo)+(stream_offset + 186)	;

	         TRAFFIC_HDR25_3_ptr	=(size_offset*streamNo)+(stream_offset + 187)	;
	         TRAFFIC_HDR25_2_ptr	=(size_offset*streamNo)+(stream_offset + 188)	;
	         TRAFFIC_HDR25_1_ptr	=(size_offset*streamNo)+(stream_offset + 189)	;
	         TRAFFIC_HDR25_0_ptr	=(size_offset*streamNo)+(stream_offset + 190)	;

	         TRAFFIC_HDR26_3_ptr	=(size_offset*streamNo)+(stream_offset + 191)	;
	         TRAFFIC_HDR26_2_ptr	=(size_offset*streamNo)+(stream_offset + 192)	;
	         TRAFFIC_HDR26_1_ptr	=(size_offset*streamNo)+(stream_offset + 193)	;
	         TRAFFIC_HDR26_0_ptr	=(size_offset*streamNo)+(stream_offset + 194)	;

	         TRAFFIC_HDR27_3_ptr	=(size_offset*streamNo)+(stream_offset + 195)	;
	         TRAFFIC_HDR27_2_ptr	=(size_offset*streamNo)+(stream_offset + 196)	;
	         TRAFFIC_HDR27_1_ptr	=(size_offset*streamNo)+(stream_offset + 197)	;
	         TRAFFIC_HDR27_0_ptr	=(size_offset*streamNo)+(stream_offset + 198)	;

	         TRAFFIC_HDR28_3_ptr	=(size_offset*streamNo)+(stream_offset + 199)	;
	         TRAFFIC_HDR28_2_ptr	=(size_offset*streamNo)+(stream_offset + 200)	;
	         TRAFFIC_HDR28_1_ptr	=(size_offset*streamNo)+(stream_offset + 201)	;
	         TRAFFIC_HDR28_0_ptr	=(size_offset*streamNo)+(stream_offset + 202)	;

	         TRAFFIC_HDR29_3_ptr	=(size_offset*streamNo)+(stream_offset + 203)	;
	         TRAFFIC_HDR29_2_ptr	=(size_offset*streamNo)+(stream_offset + 204)	;
	         TRAFFIC_HDR29_1_ptr	=(size_offset*streamNo)+(stream_offset + 205)	;
	         TRAFFIC_HDR29_0_ptr	=(size_offset*streamNo)+(stream_offset + 206)	;

	         TRAFFIC_HDR30_3_ptr	=(size_offset*streamNo)+(stream_offset + 207)	;
	         TRAFFIC_HDR30_2_ptr	=(size_offset*streamNo)+(stream_offset + 208)	;
	         TRAFFIC_HDR30_1_ptr	=(size_offset*streamNo)+(stream_offset + 209)	;
	         TRAFFIC_HDR30_0_ptr	=(size_offset*streamNo)+(stream_offset + 210)	;


	         TRAFFIC_HDR31_3_ptr	=(size_offset*streamNo)+(stream_offset + 211)	;
	         TRAFFIC_HDR31_2_ptr	=(size_offset*streamNo)+(stream_offset + 212)	;
	         TRAFFIC_HDR31_1_ptr	=(size_offset*streamNo)+(stream_offset + 213)	;
	         TRAFFIC_HDR31_0_ptr	=(size_offset*streamNo)+(stream_offset + 214)	;

	         TRAFFIC_MSK0_3_ptr	=(size_offset*streamNo)+(stream_offset + 215)	;
	         TRAFFIC_MSK0_2_ptr	=(size_offset*streamNo)+(stream_offset + 216)	;
	         TRAFFIC_MSK0_1_ptr	=(size_offset*streamNo)+(stream_offset + 217)	;
	         TRAFFIC_MSK0_0_ptr	=(size_offset*streamNo)+(stream_offset + 218)	;

	         TRAFFIC_MSK1_3_ptr	=(size_offset*streamNo)+(stream_offset + 219)	;
	         TRAFFIC_MSK1_2_ptr	=(size_offset*streamNo)+(stream_offset + 220)	;
	         TRAFFIC_MSK1_1_ptr	=(size_offset*streamNo)+(stream_offset + 221)	;
	         TRAFFIC_MSK1_0_ptr	=(size_offset*streamNo)+(stream_offset + 222)	;

	         TRAFFIC_MSK2_3_ptr	=(size_offset*streamNo)+(stream_offset + 223)	;
	         TRAFFIC_MSK2_2_ptr	=(size_offset*streamNo)+(stream_offset + 224)	;
	         TRAFFIC_MSK2_1_ptr	=(size_offset*streamNo)+(stream_offset + 225)	;
	         TRAFFIC_MSK2_0_ptr	=(size_offset*streamNo)+(stream_offset + 226)	;

	         TRAFFIC_MSK3_3_ptr	=(size_offset*streamNo)+(stream_offset + 227)	;
	         TRAFFIC_MSK3_2_ptr	=(size_offset*streamNo)+(stream_offset + 228)	;
	         TRAFFIC_MSK3_1_ptr	=(size_offset*streamNo)+(stream_offset + 229)	;
	         TRAFFIC_MSK3_0_ptr	=(size_offset*streamNo)+(stream_offset + 230)	;

	         TRAFFIC_MSK4_3_ptr	=(size_offset*streamNo)+(stream_offset + 231)	;
	         TRAFFIC_MSK4_2_ptr	=(size_offset*streamNo)+(stream_offset + 232)	;
	         TRAFFIC_MSK4_1_ptr	=(size_offset*streamNo)+(stream_offset + 233)	;
	         TRAFFIC_MSK4_0_ptr	=(size_offset*streamNo)+(stream_offset + 234)	;

	         TRAFFIC_MSK5_3_ptr	=(size_offset*streamNo)+(stream_offset + 235)	;
	         TRAFFIC_MSK5_2_ptr	=(size_offset*streamNo)+(stream_offset + 236)	;
	         TRAFFIC_MSK5_1_ptr	=(size_offset*streamNo)+(stream_offset + 237)	;
	         TRAFFIC_MSK5_0_ptr	=(size_offset*streamNo)+(stream_offset + 238)	;

	         TRAFFIC_MSK6_3_ptr	=(size_offset*streamNo)+(stream_offset + 239)	;
	         TRAFFIC_MSK6_2_ptr	=(size_offset*streamNo)+(stream_offset + 240)	;
	         TRAFFIC_MSK6_1_ptr	=(size_offset*streamNo)+(stream_offset + 241)	;
	         TRAFFIC_MSK6_0_ptr	=(size_offset*streamNo)+(stream_offset + 242)	;

	         TRAFFIC_MSK7_3_ptr	=(size_offset*streamNo)+(stream_offset + 243)	;
	         TRAFFIC_MSK7_2_ptr	=(size_offset*streamNo)+(stream_offset + 244)	;
	         TRAFFIC_MSK7_1_ptr	=(size_offset*streamNo)+(stream_offset + 245)	;
	         TRAFFIC_MSK7_0_ptr	=(size_offset*streamNo)+(stream_offset + 246)	;

	         TRAFFIC_MSK8_3_ptr	=(size_offset*streamNo)+(stream_offset + 247)	;
	         TRAFFIC_MSK8_2_ptr	=(size_offset*streamNo)+(stream_offset + 248)	;
	         TRAFFIC_MSK8_1_ptr	=(size_offset*streamNo)+(stream_offset + 249)	;
	         TRAFFIC_MSK8_0_ptr	=(size_offset*streamNo)+(stream_offset + 250)	;


	         TRAFFIC_MSK9_3_ptr	=(size_offset*streamNo)+(stream_offset + 251)	;
	         TRAFFIC_MSK9_2_ptr	=(size_offset*streamNo)+(stream_offset + 252)	;
	         TRAFFIC_MSK9_1_ptr	=(size_offset*streamNo)+(stream_offset + 253)	;
	         TRAFFIC_MSK9_0_ptr	=(size_offset*streamNo)+(stream_offset + 254)	;

	         TRAFFIC_MSK10_3_ptr	=(size_offset*streamNo)+(stream_offset + 255)	;
	         TRAFFIC_MSK10_2_ptr	=(size_offset*streamNo)+(stream_offset + 256)	;
	         TRAFFIC_MSK10_1_ptr	=(size_offset*streamNo)+(stream_offset + 257)	;
	         TRAFFIC_MSK10_0_ptr	=(size_offset*streamNo)+(stream_offset + 258)	;

	         TRAFFIC_MSK11_3_ptr	=(size_offset*streamNo)+(stream_offset + 259)	;
	         TRAFFIC_MSK11_2_ptr	=(size_offset*streamNo)+(stream_offset + 260)	;
	         TRAFFIC_MSK11_1_ptr	=(size_offset*streamNo)+(stream_offset + 261)	;
	         TRAFFIC_MSK11_0_ptr	=(size_offset*streamNo)+(stream_offset + 262)	;

	         TRAFFIC_MSK12_3_ptr	=(size_offset*streamNo)+(stream_offset + 263)	;
	         TRAFFIC_MSK12_2_ptr	=(size_offset*streamNo)+(stream_offset + 264)	;
	         TRAFFIC_MSK12_1_ptr	=(size_offset*streamNo)+(stream_offset + 265)	;
	         TRAFFIC_MSK12_0_ptr	=(size_offset*streamNo)+(stream_offset + 266)	;

	         TRAFFIC_MSK13_3_ptr	=(size_offset*streamNo)+(stream_offset + 267)	;
	         TRAFFIC_MSK13_2_ptr	=(size_offset*streamNo)+(stream_offset + 268)	;
	         TRAFFIC_MSK13_1_ptr	=(size_offset*streamNo)+(stream_offset + 269)	;
	         TRAFFIC_MSK13_0_ptr	=(size_offset*streamNo)+(stream_offset + 270)	;

	         TRAFFIC_MSK14_3_ptr	=(size_offset*streamNo)+(stream_offset + 271)	;
	         TRAFFIC_MSK14_2_ptr	=(size_offset*streamNo)+(stream_offset + 272)	;
	         TRAFFIC_MSK14_1_ptr	=(size_offset*streamNo)+(stream_offset + 273)	;
	         TRAFFIC_MSK14_0_ptr	=(size_offset*streamNo)+(stream_offset + 274)	;

	         TRAFFIC_MSK15_3_ptr	=(size_offset*streamNo)+(stream_offset + 275)	;
	         TRAFFIC_MSK15_2_ptr	=(size_offset*streamNo)+(stream_offset + 276)	;
	         TRAFFIC_MSK15_1_ptr	=(size_offset*streamNo)+(stream_offset + 277)	;
	         TRAFFIC_MSK15_0_ptr	=(size_offset*streamNo)+(stream_offset + 278)	;

	         TRAFFIC_MSK16_3_ptr	=(size_offset*streamNo)+(stream_offset + 279)	;
	         TRAFFIC_MSK16_2_ptr	=(size_offset*streamNo)+(stream_offset + 280)	;
	         TRAFFIC_MSK16_1_ptr	=(size_offset*streamNo)+(stream_offset + 281)	;
	         TRAFFIC_MSK16_0_ptr	=(size_offset*streamNo)+(stream_offset + 282)	;

	         TRAFFIC_MSK17_3_ptr	=(size_offset*streamNo)+(stream_offset + 283)	;
	         TRAFFIC_MSK17_2_ptr	=(size_offset*streamNo)+(stream_offset + 284)	;
	         TRAFFIC_MSK17_1_ptr	=(size_offset*streamNo)+(stream_offset + 285)	;
	         TRAFFIC_MSK17_0_ptr	=(size_offset*streamNo)+(stream_offset + 286)	;

	         TRAFFIC_MSK18_3_ptr	=(size_offset*streamNo)+(stream_offset + 287)	;
	         TRAFFIC_MSK18_2_ptr	=(size_offset*streamNo)+(stream_offset + 288)	;
	         TRAFFIC_MSK18_1_ptr	=(size_offset*streamNo)+(stream_offset + 289)	;
	         TRAFFIC_MSK18_0_ptr	=(size_offset*streamNo)+(stream_offset + 290)	;

	         TRAFFIC_MSK19_3_ptr	=(size_offset*streamNo)+(stream_offset + 291)	;
	         TRAFFIC_MSK19_2_ptr	=(size_offset*streamNo)+(stream_offset + 292)	;
	         TRAFFIC_MSK19_1_ptr	=(size_offset*streamNo)+(stream_offset + 293)	;
	         TRAFFIC_MSK19_0_ptr	=(size_offset*streamNo)+(stream_offset + 294)	;

	         TRAFFIC_MSK20_3_ptr	=(size_offset*streamNo)+(stream_offset + 295)	;
	         TRAFFIC_MSK20_2_ptr	=(size_offset*streamNo)+(stream_offset + 296)	;
	         TRAFFIC_MSK20_1_ptr	=(size_offset*streamNo)+(stream_offset + 297)	;
	         TRAFFIC_MSK20_0_ptr	=(size_offset*streamNo)+(stream_offset + 298)	;

	         TRAFFIC_MSK21_3_ptr	=(size_offset*streamNo)+(stream_offset + 299)	;
	         TRAFFIC_MSK21_2_ptr	=(size_offset*streamNo)+(stream_offset + 300)	;
	         TRAFFIC_MSK21_1_ptr	=(size_offset*streamNo)+(stream_offset + 301)	;
	         TRAFFIC_MSK21_0_ptr	=(size_offset*streamNo)+(stream_offset + 302)	;

	         TRAFFIC_MSK22_3_ptr	=(size_offset*streamNo)+(stream_offset + 303)	;
	         TRAFFIC_MSK22_2_ptr	=(size_offset*streamNo)+(stream_offset + 304)	;
	         TRAFFIC_MSK22_1_ptr	=(size_offset*streamNo)+(stream_offset + 305)	;
	         TRAFFIC_MSK22_0_ptr	=(size_offset*streamNo)+(stream_offset + 306)	;

	         TRAFFIC_MSK23_3_ptr	=(size_offset*streamNo)+(stream_offset + 307)	;
	         TRAFFIC_MSK23_2_ptr	=(size_offset*streamNo)+(stream_offset + 308)	;
	         TRAFFIC_MSK23_1_ptr	=(size_offset*streamNo)+(stream_offset + 309)	;
	         TRAFFIC_MSK23_0_ptr	=(size_offset*streamNo)+(stream_offset + 310)	;

	         TRAFFIC_MSK24_3_ptr	=(size_offset*streamNo)+(stream_offset + 311)	;
	         TRAFFIC_MSK24_2_ptr	=(size_offset*streamNo)+(stream_offset + 312)	;
	         TRAFFIC_MSK24_1_ptr	=(size_offset*streamNo)+(stream_offset + 313)	;
	         TRAFFIC_MSK24_0_ptr	=(size_offset*streamNo)+(stream_offset + 314)	;

	         TRAFFIC_MSK25_3_ptr	=(size_offset*streamNo)+(stream_offset + 315)	;
	         TRAFFIC_MSK25_2_ptr	=(size_offset*streamNo)+(stream_offset + 316)	;
	         TRAFFIC_MSK25_1_ptr	=(size_offset*streamNo)+(stream_offset + 317)	;
	         TRAFFIC_MSK25_0_ptr	=(size_offset*streamNo)+(stream_offset + 318)	;

	         TRAFFIC_MSK26_3_ptr	=(size_offset*streamNo)+(stream_offset + 319)	;
	         TRAFFIC_MSK26_2_ptr	=(size_offset*streamNo)+(stream_offset + 320)	;
	         TRAFFIC_MSK26_1_ptr	=(size_offset*streamNo)+(stream_offset + 321)	;
	         TRAFFIC_MSK26_0_ptr	=(size_offset*streamNo)+(stream_offset + 322)	;

	         TRAFFIC_MSK27_3_ptr	=(size_offset*streamNo)+(stream_offset + 323)	;
	         TRAFFIC_MSK27_2_ptr	=(size_offset*streamNo)+(stream_offset + 324)	;
	         TRAFFIC_MSK27_1_ptr	=(size_offset*streamNo)+(stream_offset + 325)	;
	         TRAFFIC_MSK27_0_ptr	=(size_offset*streamNo)+(stream_offset + 326)	;

	         TRAFFIC_MSK28_3_ptr	=(size_offset*streamNo)+(stream_offset + 327)	;
	         TRAFFIC_MSK28_2_ptr	=(size_offset*streamNo)+(stream_offset + 328)	;
	         TRAFFIC_MSK28_1_ptr	=(size_offset*streamNo)+(stream_offset + 329)	;
	         TRAFFIC_MSK28_0_ptr	=(size_offset*streamNo)+(stream_offset + 330)	;

	         TRAFFIC_MSK29_3_ptr	=(size_offset*streamNo)+(stream_offset + 331)	;
	         TRAFFIC_MSK29_2_ptr	=(size_offset*streamNo)+(stream_offset + 332)	;
	         TRAFFIC_MSK29_1_ptr	=(size_offset*streamNo)+(stream_offset + 333)	;
	         TRAFFIC_MSK29_0_ptr	=(size_offset*streamNo)+(stream_offset + 334)	;

	         TRAFFIC_MSK30_3_ptr	=(size_offset*streamNo)+(stream_offset + 335)	;
	         TRAFFIC_MSK30_2_ptr	=(size_offset*streamNo)+(stream_offset + 336)	;
	         TRAFFIC_MSK30_1_ptr	=(size_offset*streamNo)+(stream_offset + 337)	;
	         TRAFFIC_MSK30_0_ptr	=(size_offset*streamNo)+(stream_offset + 338)	;

	         TRAFFIC_MSK31_3_ptr	=(size_offset*streamNo)+(stream_offset + 339)	;
	         TRAFFIC_MSK31_2_ptr	=(size_offset*streamNo)+(stream_offset + 340)	;
	         TRAFFIC_MSK31_1_ptr	=(size_offset*streamNo)+(stream_offset + 341)	;
	         TRAFFIC_MSK31_0_ptr	=(size_offset*streamNo)+(stream_offset + 342)	;

}
