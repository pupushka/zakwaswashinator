/*
 * utils.h
 *
 *  Created on: 22.04.2023 г.
 *      Author: dida
 */

#ifndef MAIN_UTILS_H_
#define MAIN_UTILS_H_


#include <stdint.h>

uint64_t ConvertASCItouint64( char *in );
void convertFloatToExpMantiss(float in, uint8_t& exponent, uint8_t& mantiss1,
                              uint8_t& mantiss2, uint8_t& mantiss3);
float convertExpMantissToFloat(uint8_t exponent, uint8_t mantiss1,
                               uint8_t mantiss2, uint8_t mantiss3);


#endif /* MAIN_UTILS_H_ */
