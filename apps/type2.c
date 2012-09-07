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

	uint16_t sin1 = sini(a);
	double x0 = (double)sini(a*.1*chana[1-1])/256-64;
	double y0 = (double)sini((a*.1*chana[2-1])+0x1000)/256-64;
	double x1 = (double)sini(a*.1*chana[3-1])/128-128;
	double y1 = (double)sini((a*.1*chana[4-1])+0x1000)/128-128;
		
	for(y = 0; y < LED_HEIGHT; y++) 
	{
		uint16_t y_part =  sini(sin1+y*5*chan[8-1]);


		for(x = 0; x < LED_WIDTH; x++) 
		{
			
			double dist = pythagoras(x0-x,y0-y);
			double dist2 = pythagoras(y1-x,x1-y);


			uint16_t h = sini(sin1+x*5*chan[9])+ y_part + sini(dist*3*chan[6]) + sini(dist2*chan[7-1]);
			setLedXY(
				x,y,
				sini((h>>2)+a*0.1*chan[3]+x*2*chan[1-1])>>8,
				sini((h>>2)+a*0.1*chan[4]+y*3*chan[2-1]+0x1555)>>8,
				sini((h>>2)+a*3*chan[5-1]+0x2aaa)>>8
			);
		}
	}
	a+=chana[5-1]*2;
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




