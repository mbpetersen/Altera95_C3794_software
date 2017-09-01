/*
 * Status.h
 *
 *  Created on: Dec 9, 2016
 *      Author: dev
 */

#ifndef INCLUDE_STATUSDATA_H_
#define INCLUDE_STATUSDATA_H_

#include <stdint.h>


struct __attribute__ ((__packed__)) PortPhyStatus
{
	uint32_t	OpMode;		//0-3
	uint16_t	OptPower;	//4-5
	uint16_t	MIIStat;	//6-7
	uint16_t	Misc;		//8-9
};

struct __attribute__ ((__packed__)) PhyStatus
{
	struct PortPhyStatus	A;			//0-9
	struct PortPhyStatus   B;			//10-19
	uint16_t		CableDiag;	//20-21
};



struct __attribute__ ((__packed__)) MacCounters
{
	uint64_t	FrameOk;		//0-7
	uint64_t	Unicast;		//8-15
	uint64_t	Multicast;		//16-23
	uint64_t	Broadcast;		//24-31
	uint64_t	Pause : 48;		//32-37
	uint64_t	Error : 48;		//38-43
	uint8_t		OctetsExtra[2];	//44-45
	uint64_t	Octets;			//46-53
	uint64_t 	Vlan : 48;		//54-59
	uint64_t	Extra : 48;		//60-65 // perhaps for UserPause later
};

struct __attribute__ ((__packed__)) MacRxCounters
{
	uint64_t	FrameOk;		//0-7
	uint64_t	Unicast;		//8-15
	uint64_t	Multicast;		//16-23
	uint64_t	Broadcast;		//24-31
	uint64_t	Pause : 48;		//32-37
	uint64_t	Error : 48;		//38-43
	uint8_t		OctetsExtra[2];	//44-45
	uint64_t	Octets;			//46-53
	uint64_t 	Vlan : 48;		//54-59
	uint64_t	Extra : 48;		//60-65 // perhaps for UserPause later
	uint64_t	Undersized : 48;	//66-71
	uint64_t	Oversized : 48;		//72-77
	uint64_t	_64 : 48;			//78-83
	uint64_t	_65_127 : 48;		//84-89
	uint64_t	_128_255 : 48;		//90-95
	uint64_t	_256_511 : 48;		//96-101
	uint64_t	_512_1023 : 48;		//102-107
	uint64_t	_1024_1518 : 48;	//108-113
	uint64_t	_1519_1522 : 48;	//114-119
	uint64_t	_1523_1548 : 48;	//120-125
	uint64_t	_1549_2047 : 48;	//126-131
	uint64_t	_2048_4095 : 48;	//132-137
	uint64_t	_4096_8191 : 48;	//138-143
	uint64_t	_8192_9215 : 48;	//144-149
	uint64_t	Jabbers : 48;		//150-155
	uint64_t	Fragments : 48;		//156-161
	uint64_t	BadFcs : 48;		//162-167
	uint64_t	StompedFcs : 48;	//168-173 // packets with bitwise inverse of the expected good FCS
	uint64_t	InRangeError : 48;	//174-179 // packets with Length field error but with good FCS
};


struct __attribute__ ((__packed__)) MacStatusGeneral
{
	uint32_t	SnapTxFrameCount;	//0-3
	uint32_t	SnapRxFrameCount; 	//4-7
	uint32_t	CollisionCount; 	//8-11
	uint32_t	VlanStatus; 		//12-15
};


struct __attribute__ ((__packed__)) MacStatusCounters
{
	struct MacCounters		Tx;						//0-65
	struct MacRxCounters	Rx;						//66-245
	uint64_t		FrameFcsErrors : 48;	//246-251
	uint64_t		AlignmentErrors : 48;	//252-257
	uint64_t		CycleCount : 48;		//258-263
};


struct __attribute__ ((__packed__)) MacStatusEthernet
{
	uint64_t	Drop : 48;			//0-5
	uint8_t		OctetsExtra[2];		//6-7
	uint64_t	Octets;				//8-15 Rx
	uint64_t	Packets : 48;		//16-21
};

struct __attribute__ ((__packed__)) MacStatus
{
	struct MacStatusGeneral 	General;		//0-15
	struct MacStatusCounters 	Counters;		//16-279
	struct MacStatusEthernet 	Ethernet;		//280-301
};

struct __attribute__ ((__packed__)) TrafficGen
{
	uint8_t		Running;			//0
	uint8_t		StreamKey;			//1
	uint8_t		Start;				//2
	uint8_t		Stop;				//3
	uint32_t	Error;				//4-7
	uint32_t	Version;			//8-11
	uint64_t	NumStreamPacket: 48;	//12-17
	uint64_t	TxPacketSentCount : 48;//18-23
};

struct __attribute__ ((__packed__)) TrafficMon
{
	uint64_t	NumRecvdPackets : 48;	//0-5
	uint32_t	RxControlStatus;		//6-9
	uint8_t		CycleRxCountExtra[2]; 	//10-11
	uint64_t	CycleRxCount;			//12-19
	uint32_t	PacketRxMacErrCount;	//20-23
	uint64_t	PacketRxEOPErr;			//24-31
	uint64_t	PacketRxTimeoutErr; 	//32-39
	uint64_t	PacketRxNoMatchErr;		//40-47
	uint64_t	PacketRxMultiMatchErr;	//48-55
	uint64_t	RxNumPacketsLeft;		//56-63
	uint32_t	StreamKey;				//64-67
	uint8_t		BroadcastRxExtra[2];	//68-69
	uint64_t	BroadcastRx;			//70-77
	uint8_t		PacketRxMacErrorExtra[2];//78-87
	uint64_t	PacketRxMacError;		//70-77
	uint32_t	Delay[3];				//88-99
	uint64_t	DelayMin : 40;			//100-104
	uint64_t	DelayMax : 40;			//105-109
	uint8_t		RxByteCountExtra[2];	//110-111
	uint64_t	RxByteCount;			//112-119
	uint64_t	RxPacketOkCount;		//120-127
	uint8_t		RxPacketErrorCountExtra[2];//128-139
	uint64_t	RxPacketErrorCount;		//130-137
};

struct __attribute__ ((__packed__)) SdtStatus
{
	uint16_t 	ShortestTime;		//0-1
	uint16_t 	LongestTime;		//2-3
	uint16_t 	AverageTime;		//4-5
	uint32_t 	TotalTime;			//6-9
};

struct __attribute__ ((__packed__)) StatusData
{
	uint8_t		GIGEStatus;		//0
	uint8_t		GIGEType;		//1
	uint8_t		GIGEResult;		//2
	uint32_t	TestElapsedSec;	//3-6
	uint8_t		MiscStatus;		//7
	uint8_t		SFPStatus;		//8

	struct PhyStatus	Phy;			//9-30
	struct MacStatus 	Mac;			//31-332
	struct TrafficGen  TGen;			//333-356
	struct TrafficMon 	TMon;			//357-494
	struct SdtStatus	Sdt;			//495-504
	uint8_t		reserved[32];	//505-537
};


#endif /* INCLUDE_STATUSDATA_H_ */
