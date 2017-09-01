/*
 * Messages.h
 *
 *  Created on: Feb 8, 2017
 *      Author: user
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "Header.h"
#include "ConfigData.h"
#include "StatusData.h"
#include "SystemInfoData.h"
#include "SfpModuleInfoData.h"
#include "ConfigMsData.h"

struct __attribute__ ((__packed__)) ConfigMessage
{
	struct Header	Header;				//0-3
	struct ConfigData		Config;				//4-444
	uint16_t		Checksum;			//445-446
};

struct __attribute__ ((__packed__)) LinkDownMessage
{
	struct Header	Header;				//0-3
	struct ConfigData		Config;				//4-444
	struct SystemInfoData	SysInfo;			//445-734
	uint16_t		Checksum;			//735-736
};

struct __attribute__ ((__packed__)) LinkUpMessage
{
	struct Header		Header;				//0-3
	struct StatusData	Status;				//4-540
	struct SfpModuleInfoData	ModInfo;			//541-996
	uint16_t			Checksum;			//997-998
};


struct __attribute__ ((__packed__)) ConfigMsMessage
{
	struct Header	Header;				//0-3
	struct ConfigMsData	Config;				//4-5
	uint16_t		Checksum;			//445-446
};



#if __cplusplus
typedef std::shared_ptr<Header> HeaderPtr;
typedef std::shared_ptr<ConfigMessage> ConfigPtr;
typedef std::shared_ptr<ConfigMsMessage> ConfigMsPtr;
#endif

#endif /* MESSAGES_H_ */
