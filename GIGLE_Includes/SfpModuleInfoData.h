/*
 * SfpModuleInfo.h
 *
 *  Created on: Feb 8, 2017
 *      Author: user
 */

#ifndef SFPMODULEINFODATA_H_
#define SFPMODULEINFODATA_H_

struct __attribute__((packed)) SfpModuleInfoData
{
	uint8_t		SFPCore[128]; 	//0-127
	uint8_t		SFPDiag[128];	//128-255
	uint8_t		unused[200];	//256-455
};



#endif /* SFPMODULEINFODATA_H_ */
