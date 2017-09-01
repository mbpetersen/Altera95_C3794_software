/*
 * FlashParameters.h
 *
 *  Created on: Dec 8, 2016
 *      Author: dev
 */

#ifndef INCLUDE_FLASHPARAMETERS_H_
#define INCLUDE_FLASHPARAMETERS_H_

#include "Constants.h"
#include <stdint.h>

struct FlashParameters
{
	uint8_t MacAddressA[6];
	uint8_t MacAddressB[6];
	uint8_t MacAddress10A[6];
	uint8_t MacAddress10B[6];
	uint8_t OptionsInstalled;
	uint8_t OptionsEnabled;
	uint8_t Length;
};



#endif /* INCLUDE_FLASHPARAMETERS_H_ */
