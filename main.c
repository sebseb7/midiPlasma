#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include <sys/time.h>

#include "main.h"
#include "keyboard.h"


#define MAX_NUMBER_OF_APPS 20

uint8_t (*appTick_fp[MAX_NUMBER_OF_APPS])(void);
uint16_t appInterval_duration[MAX_NUMBER_OF_APPS];
uint8_t apps = 0;
uint8_t current_app = 0;

unsigned long long int get_clock(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned long long int)tv.tv_usec + 1000000*tv.tv_sec;
}


int leds[LED_HEIGHT][LED_WIDTH][4];

SDL_Surface* screen;

void setLedXY(uint8_t x, uint8_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	leds[y][x][0] = red;
	leds[y][x][1] = green;
	leds[y][x][2] = blue;
	leds[y][x][3] = 1;
}
void getLedXY(uint8_t x, uint8_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LED_WIDTH) return;
	if (y >= LED_HEIGHT) return;
	*red = leds[y][x][0];
	*green = leds[y][x][1];
	*blue = leds[y][x][2];
}

void registerAnimation(tick_fun tick, uint16_t t, uint16_t ignore)
{
	assert(t > 0);
	if(apps < MAX_NUMBER_OF_APPS)
	{
		printf("reg\n");
		appTick_fp[apps] = tick;
		// 122Hz / tick
		appInterval_duration[apps]=10000000 / 122 * t;
	
		apps++;
	}
}


int main(int argc, char *argv[]) {
	int x, y;

	keyboard_init();


	for(x = 0; x < LED_WIDTH; x++) {
		for(y = 0; y < LED_HEIGHT; y++) {
			leds[y][x][0]=0;
			leds[y][x][1]=0;
			leds[y][x][2]=0;
			leds[y][x][3]=1;
		}
	}

	srand(time(NULL));

	screen = SDL_SetVideoMode(LED_WIDTH * ZOOM + ZOOM / 15, LED_HEIGHT * ZOOM + ZOOM / 15,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	SDL_Rect rect = { 0, 0, LED_WIDTH*ZOOM+(ZOOM/15), LED_HEIGHT*ZOOM+(ZOOM/15) };
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0x20,0x20,0x20));


	int running = 1;
	unsigned long long int startTime = get_clock();
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						default: break;
					}
				default: break;
			}
		}

		pollKeyboard();

		running &= !appTick_fp[current_app]();

		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {

				if(leds[y][x][3] == 1)
				{

					SDL_Rect rect = { ZOOM*x+(ZOOM/15), ZOOM*(LED_HEIGHT - y - 1)+(ZOOM/15), ZOOM-(ZOOM/15), ZOOM-(ZOOM/15) };
					SDL_FillRect(
						screen, 
						&rect, 
						SDL_MapRGB(screen->format, leds[y][x][0],leds[y][x][1],leds[y][x][2])
					);
					leds[y][x][3] = 0;

				}

			}
		}

		startTime+=appInterval_duration[current_app];
		int delay = startTime-get_clock();


		printf("%i\n",delay);
		if(delay > 0)
			usleep(delay);
		
		SDL_Flip(screen);
	}

	SDL_Quit();
	return 0;
}


void pollKeyboard(void)
{
	KeyboardEvent e;

	while(keyboard_poll(&e)) 
	{
		printf("%d %d %d\n", e.x, e.y,e.type);
//		fflush(stdout);
		
		if(e.type == 176)
		{
			if(e.x == 1)
			{
				chan1 = e.y;
			}
			else if(e.x == 2)
			{
				chan2 = e.y;
			}
			else if(e.x == 3)
			{
				chan3 = e.y;
			}
			else if(e.x == 4)
			{
				chan4 = e.y;
			}
			else if(e.x == 5)
			{
				chan5 = e.y;
			}
			else if(e.x == 6)
			{
				chan6 = e.y;
			}
			else if(e.x == 7)
			{
				chan7 = e.y;
			}
			else if(e.x == 8)
			{
				chan8 = e.y;
			}
			else if(e.x == 9)
			{
				chan9 = e.y;
			}
			else if(e.x == 11)
			{
				chana1 = e.y;
			}
			else if(e.x == 12)
			{
				chana2 = e.y;
			}
			else if(e.x == 13)
			{
				chana3 = e.y;
			}
			else if(e.x == 14)
			{
				chana4 = e.y;
			}
			else if(e.x == 15)
			{
				chana5 = e.y;
			}
			else if(e.x == 16)
			{
				chana6 = e.y;
			}
			else if(e.x == 17)
			{
				chana7 = e.y;
			}
			else if(e.x == 18)
			{
				chana8 = e.y;
			}
			else if(e.x == 19)
			{
				chana9 = e.y;
			}
			else if((e.x >= 21)&&(e.x <= 39)&&(e.y == 127))
			{
				button(e.x-21);
			}
		}
	}
}

void button(uint8_t nr)
{
	printf("button %i\n",nr);
	if(nr < apps)
	{
		current_app = nr;
	}
}
