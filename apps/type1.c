#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "main.h"
#include "sini.h"
#include "timesin.h"

static double pythagoras( double side1, double side2 )
{
	return sqrt(pow( side1, 2 ) + pow( side2, 2 ));
}




static uint8_t tick(void) {

	static uint16_t a = 0;
	uint8_t x, y;

	double x0 = (double)timesin(10,(LED_WIDTH-10)*10,chan[9-1])/10;
	double y0 = (double)timesin(10,(LED_HEIGHT-10)*10,chan[8-1])/10;
	double x1 = (double)timesin(10,(LED_WIDTH-10)*10,chan[7-1])/10;
	double y1 = (double)timesin(10,(LED_HEIGHT-10)*10,chan[6-1])/10;
	

	//~ printf("%f\n",x0);

	for(y = 0; y < LED_HEIGHT; y++) 
	{

		for(x = 0; x < LED_WIDTH; x++) 
		{


			uint16_t r = sini(pythagoras((x0-x)*chan[1],(y0-y)*chan[1-1]))>>1;
			r+= sini(pythagoras((x1-x)*chan[2],(y1-y)*chan[2-1]))>>1;


	//		printf("%i\n",r);

			setLedXY(
				x,y,
				sini((r)+a*0.1*chana[1]+x*2*chana[2-1])>>8,
				sini((r)+a*0.1*chana[3]+y*3*chana[4]+chana[6-1]*10)>>8,
				sini((r)+a*3*chana[5]+chana[7-1]*20)>>8
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




