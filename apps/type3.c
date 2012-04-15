#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "main.h"
#include "sini.h"

static double pythagoras( double side1, double side2 )
{
	return sqrt(pow( side1, 2 ) + pow( side2, 2 ));
}




static uint8_t tick(void) {



	static uint16_t a = 0;
	
	uint8_t x, y;

	//uint16_t sin1 = sini(a);
	double x0 = (double)sini(a)/512;
	double y0 = (double)sini((a)+0x1000)/512;
	double x1 = (double)sini(a+0x1000)/512;
	double y1 = (double)sini((a)+0x2000)/512;
		
	for(y = 0; y < LED_HEIGHT; y++) 
	{
		//uint16_t y_part =  sini(sin1+y*5*chan8);


		for(x = 0; x < LED_WIDTH; x++) 
		{
			
			uint32_t dist = pow((double)sini(pythagoras(x0-x,y0-y)*chan1*10)/0xffff,4)*0xffff;
			uint32_t dist2 = sini(pythagoras(y1-x,x1-y)*chan2*10);


			uint32_t max = dist;
			if(dist2 > dist)
				max = dist2;

			dist = (dist>>1) + (dist2>>1);
			dist = (dist>>8) * (max >> 8);


	//			dist = (dist>>1)+(dist2>>1);
//			setLedXY(x,y,dist>>8,0,0);

//			uint16_t h = sini(sin1+x*5*chan9)+ y_part + sini(dist*3*chan6) + sini(dist2*chan7);
			setLedXY(
				x,y,
				sini((dist>>2)+a*0.1*chan6+x*2*chan3+(chan9*100))>>8,
				sini((dist>>2)+a*0.1*chan7+y*3*chan4+0x1555+(chan9*100))>>8,
				sini((dist>>2)+a*0.1*chan8+(x+y)*chan5+0x2aaa+(chan9*100))>>8
			);
		}
	}
	a+=chana5*2;
	if(a==0x4000)
	{
		a=0;
	}
	return 0;
}

static void init(void) ATTRIBUTES

void init(void) {
	registerAnimation(tick, 40000, 450);
}




