#include <stdint.h>
#include <stdio.h>

#include "sini.h"
#include "main.h"


double pi  = 3.141592653589793238462643;

uint16_t timesin(uint16_t from,uint16_t to, uint16_t duration)
{
	unsigned int span = (to-from);
	unsigned int center = (to+from)/2;

	duration = 128-duration;

	uint32_t counter = get_clock()*0.01*pi/duration;

	double sind = (double)sini(counter&0x3fff) / 0xffff;

	return (sind-0.5)*span+center;

//	printf("%llu\n",sini(counter)&0x3fff);
//	printf("%f\n",sin16i);

//	return (sini((uint32_t)(get_clock()*0.02*pi/duration)&0x3fff)/0xFFFF*span)+center;
}

