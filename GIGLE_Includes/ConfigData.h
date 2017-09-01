/*
 * ConfigData.h
 *
 *  Created on: Dec 5, 2016
 *      Author: dev
 */

#ifndef INCLUDE_CONFIGDATA_H_
#define INCLUDE_CONFIGDATA_H_

#include "Constants.h"
#include <stdint.h>
//#include <memory>

//#include "PACKED"


enum SysConfig {
	SysConfig_None = 0x0,
	SysConfig_ReadFromFlash = 0x1,
	SysConfig_WriteToFlash = 0x2,
	SysConfig_Clear = 0x80,
	// QUESTION: ERASE???
};

struct __attribute__ ((__packed__)) ModeConfig
{
	uint8_t 		GIGEMode;			//0
	uint8_t 		SysConfig;			//1
};

struct __attribute__ ((__packed__)) OptionsConfig
{
	uint8_t 		Installed; 	//0
	uint8_t 		Enabled; 	//1
};


struct __attribute__ ((__packed__)) MacConfig
{
	//enum { MC_HASH_LEN = 8 };
	uint32_t 	Config;					//0-3
	uint16_t	MTU;					//4-5
	uint16_t	PauseQuant;				//6-7
	uint8_t 	InterPacketGap;			//8
	uint8_t		MiscControlFlag;		//9
	uint8_t		Addr[MAC_ADDRESS_LEN];	//10-15
	uint8_t		MultiCastHash[MC_HASH_LEN];			//16-23
	uint8_t		SupplementaryAddr[4][MC_HASH_LEN];	//24-55
};


enum PhyModuleControl {
	PhySfpA_TxOff = 0x0,
	PhySfpB_TxOff = 0x1,
	PhySfpPlusA_TxOff = 0x2,
	PhySfpPlusB_TxOff = 0x4,
	PhySfp2_TxOff = 0x8,
	PhySfp4_TxOff = 0x10,
	PhyQsfp_TxOff = 0x20,
	PhySgmiiA_TxOff = 0x40,
	PhySgmiiB_TxOff = 0x80,
};

enum PhySwitch
{
	Switch_Normal = 0x0,
	Switch_10G_LanWan = 0x2,
	Switch_EndToEnd = 0x10,
	Switch_Loopback = 0x20,
	Switch_TseB_MacSwap = 0x40,
};

struct __attribute__ ((__packed__)) PhyConfig
{
	uint16_t		Config;			//0-1
	uint8_t			ModuleControl;	//2
	uint8_t			CoreSigRouteMode;	//3
};


enum ClearFlags {
	ClearTestSeconds = 0x1,
	ClearFlagsMask = 0x1,
};

enum MiscFlags {
	DumpSfpA_Info = 0x1,
	DumpSfpB_Info = 0x2,
	DumpSfpPlusA_Info = 0x4,
	SendBigPacket = 0x40,
	SendMsResults = 0x80
};

enum StartStopFlags {
	TestNotRunning = 0x0,
	ContinueTest = 0x1,
	TestStart = 0x10,
	TestRunning = 0x20,
	TestStopped = 0x40,

};


struct __attribute__ ((__packed__)) Y1564Config
{
	uint32_t	CIR;				//0-3
	uint32_t	EIR;				//4-7
	uint8_t		CIR_Steps;			//8
	uint32_t	MaxFTD;				//9-12
	uint32_t	MaxFDV;				//13-16
	uint32_t	MaxFrameLoss;		//17-20
	uint32_t	MaxAvail;			//21-24
	uint32_t	spare;				//25-28
};

struct __attribute__ ((__packed__)) StreamConfig
{
	uint32_t 	TrafficMonStreamControl; 	//0-3
	uint8_t		TrafficRateType;			//4
	uint32_t	TrafficRate;				//5-8
	uint64_t	TrafficGenPacketPerc : 48;	//9-14
	uint8_t		EGenPacketType[3];			//15-17
	uint32_t	TrafficGenConfig[4];		//18-33
	uint32_t	TrafficGenPacketEmixUserSize; // 34-37
	uint32_t	TrafficGenEmixSequence[4];	//38-53
	struct Y1564Config	Y1564;						//54-82

	uint8_t		TrafficGenSpare;			//83
	uint8_t		TrafficHeaderDef;			//84
	uint32_t	TrafficHeader[32];			//85-212
	uint32_t	TrafficMask[32];			//213-340
};


struct __attribute__ ((__packed__)) ConfigData
{
	struct ModeConfig		Mode;				//0-1
	struct OptionsConfig	Optionsa; 			//2-3
	struct MacConfig   		Mac;				//4-59
	struct PhyConfig		Phy;				//60-63

	uint8_t			ClearFlags;			//64
	uint8_t			MiscFlags;			//65
	uint8_t			BERT;				//66
	uint8_t			TestOptions;		//67
	uint8_t			TrafficNumberStreams : 4; 	//68
	uint8_t			TrafficWhichStream : 4; 	//68

	uint32_t 		EGenNumSecs;				//69-72
	uint64_t		TrafficGenNumPackets : 40; 	//73-77
	uint8_t			reserved[21];				//78-98

	struct StreamConfig 	Stream;					//99-439
	uint8_t			StartStop;				//440
};

#endif /* INCLUDE_CONFIGDATA_H_ */

