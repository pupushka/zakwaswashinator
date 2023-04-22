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
