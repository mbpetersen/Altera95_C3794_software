/*
 * SystemInfoData.h
 *
 *  Created on: Feb 8, 2017
 *      Author: user
 */

#ifndef SYSTEMINFODATA_H_
#define SYSTEMINFODATA_H_


#include "Constants.h"
#include <stdint.h>

struct __attribute__((packed)) SystemInfoData
{
	uint8_t HwVersion;  				// 0
	uint16_t FwVersion; 				// 1-2
	uint8_t FlashOptionInstalled; 		// 3
	uint8_t FlashOptionEnabled;   		// 4
	uint8_t unused[5];      			// 5-9
	uint8_t MacAddressA[MAC_ADDRESS_LEN]; 	// 10-15
	uint8_t MacAddressB[MAC_ADDRESS_LEN]; 	// 16-21
	uint8_t MacAddress10A[MAC_ADDRESS_LEN]; // 22-27
	uint8_t MacAddress10B[MAC_ADDRESS_LEN]; // 28-33
	uint8_t reserved[256];					// 34-289
};




#endif /* SYSTEMINFODATA_H_ */
