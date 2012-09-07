#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include <sys/time.h>

#include "main.h"
#include "keyboard.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <string.h>
int tty_fd;

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
	for(uint8_t i = 32;i <= 39;i++)
	{
		keyboard_send(176,i,0);
	}
	keyboard_send(176,32,127);

	struct termios tio;


	//memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	tty_fd=open("/dev/ttyACM1", O_RDWR | O_NONBLOCK);      
	cfsetospeed(&tio,B115200);            // 115200 baud
	cfsetispeed(&tio,B115200);            // 115200 baud

	tcsetattr(tty_fd,TCSANOW,&tio);

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


		//printf("%i\n",delay);
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
    
	// controller ranges:
	const int chan_begin= 0, chan_end= 7;
	const int chana_begin= 16, chana_end= 23;

	while(keyboard_poll(&e)) 
	{

		
		printf("%d %d %d\n", e.x, e.y, e.type);
//		fflush(stdout);
		
		if(e.type>>4 == 0x0b)	// control change
		{
			if( (e.x>=chan_begin) && (e.x<=chan_end) )
				chan[e.x-chan_begin]= e.y;
			else if( (e.x>=chana_begin) && (e.x<=chana_end) )
				chana[e.x-chana_begin]= e.y;
			else if((e.x >= 32)&&(e.x <= 39)&&(e.y == 127))
			{
		
				if(e.x < 32+apps)
				{
					for(uint8_t i = 32;i <= 39;i++)
					{
						keyboard_send(176,i,0);
					}
					keyboard_send(176,e.x,127);
				}
				
				
				button(e.x-32);
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
