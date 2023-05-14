/*
 * utils.cpp
 *
 *  Created on: 22.04.2023 г.
 *      Author: dida
 */

#include "utils.h"
#include <stdint.h>

uint64_t ConvertASCItouint64( char *in )
{
	char *b;
	uint64_t c=0;
	uint64_t multi=1;

	b = in;

	while ( ( (*b > 47) && (*b < 58 )) || ( *b == 32 ) || ( *b == 46 ) || ( *b ==44 ) )
	{
		b++;
	}

	while(b>=in)
	{
		if(( (*b > 47) && (*b < 58 )))
		{
			c=c+multi*(*b-'0');
			multi=multi*10;
		}

		b--;
	}
	return c;
}


float convertExpMantissToFloat(uint8_t exponent, uint8_t mantiss1,
                               uint8_t mantiss2, uint8_t mantiss3)
{
  float out;
  //just to avoid error in the checker value is assigned by pointer with cast lose
  out=0;

  void *addrTemp;

  addrTemp = static_cast<void *>(&out);
  uint8_t *addrConv;
  addrConv = static_cast<uint8_t *>(addrTemp);

  *(&addrConv[0])=mantiss3;
  *(&addrConv[1])=mantiss2;
  *(&addrConv[2])=mantiss1;
  *(&addrConv[3])=exponent;

  return out;
}

void convertFloatToExpMantiss(float in, uint8_t& exponent, uint8_t& mantiss1,
                              uint8_t& mantiss2, uint8_t& mantiss3)
{
  float tmpFloat;
  tmpFloat = in;

  void *addrTemp;
  uint8_t *addrConv;

  addrTemp= static_cast<void *>(&tmpFloat);

  addrConv= static_cast<uint8_t *>(addrTemp);

  mantiss3 = *(&addrConv[0]);
  mantiss2 = *(&addrConv[1]);
  mantiss1 = *(&addrConv[2]);
  exponent = *(&addrConv[3]);

}
