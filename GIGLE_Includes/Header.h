/*
 * Header.h
 *
 *  Created on: Feb 8, 2017
 *      Author: user
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <stdint.h>
//#include "PACKED"

enum GIGEConfig {
	PDAProbe = 0x00,
	DirtyPDABita = 0x80,
	DirtyModBita = 0x40,
	UpdateFlaga = 0x20,
	ReSyncFlaga = 0x10,
	ValidConfigMaska = 0xC0,
	ValidConfiga = 0x80,
};

/****************************************************************/
/********** Packet ID's by Function/PCB/Module type *************/
/****************************************************************/
enum ID {
	Packet_DS3a = 0x00,
	Packet_DS1Aa = 0x01,
	Packet_DS1Ba = 0x02,
	Packet_DS0a = 0x03,
	Packet_PRIa = 0x04,
	Packet_TIMSa = 0x05,
	Packet_DDSa = 0x06,
	Packet_DS3_2a = 0x07,
	Packet_TIMEa = 0x08,
	Packet_E1Aa = 0x09,
	Packet_E1Ba = 0x0A,
	Packet_E3a = 0x0B,
	Packet_E3_2a = 0x0C,
	Packet_DCOMa = 0x0D,

	Packet_SEEPROMTa = 0x0E,
	Packet_SEEPROMa = 0x0F,

	// Packet_AUTOTEST	0x80	// OR'd with Packet_TYPE to define AUTO TEST PACKET!
	Packet_DIGITSa = 0x10,	// Async packet sent to GUI when new DIGITs received

	Packet_GIGEAa = 0x20,
	Packet_GIGEA_MISCa = 0x21,
	Packet_GIGEBa = 0x28,
	Packet_GIGEB_MISCa = 0x29,

	Packet_GIGE10Aa = 0x2A,
	Packet_GIGE10A_MISCa = 0x2B,
	Packet_GIGE10Ba = 0x2C,
	Packet_GIGE10B_MISCa = 0x2D,

	Packet_100GA = 0x30,
	Packet_100GA_MISC = 0x31,
	Packet_100GB = 0x32,
	Packet_100GB_MISC = 0x33,
	Packet_100GC = 0x34,
	Packet_100GC_MISC = 0x35,

	Packet_MultiStreama = 0x75,
};

struct __attribute__ ((__packed__)) Header
{
	enum GIGEConfig	GIGEConfig;			//0
	enum ID	 		ID;					//1
	uint16_t		MsgLength;			//2-3
};


#endif /* HEADER_H_ */
