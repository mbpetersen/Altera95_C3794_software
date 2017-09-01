/*
 * ConfigMsData.h
 *
 *  Created on: Dec 9, 2016
 *      Author: dev
 */

#ifndef INCLUDE_CONFIGMSDATA_H_
#define INCLUDE_CONFIGMSDATA_H_

#include "ConfigData.h"


struct __attribute__ ((__packed__)) ConfigMsData
{
	struct ModeConfig		Mode;				//4-5
	uint8_t			spare[8];			//6-13
	struct StreamConfig	Streams[16];		//341 each, 14-354, 355-695...5129-5469
};


#endif /* INCLUDE_CONFIGMSDATA_H_ */
