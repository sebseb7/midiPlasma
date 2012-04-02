#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define LED_WIDTH	203
#define LED_HEIGHT	125

// multiple of 15
#define ZOOM 6

#define ATTRIBUTES     __attribute__((constructor));


typedef uint8_t (*tick_fun)(void);

void setLedXY(uint8_t x, uint8_t y, uint8_t red,uint8_t green, uint8_t blue);
void getLedXY(uint8_t x, uint8_t y, uint8_t* red,uint8_t* green, uint8_t* blue);
void registerAnimation(tick_fun tick, uint16_t t, uint16_t duration);
void registerApp(tick_fun tick, uint16_t t);
void pollKeyboard(void);
void button(uint8_t);
unsigned long long int get_clock(void);


uint8_t chan1 ;
uint8_t chan2 ;
uint8_t chan3 ;
uint8_t chan4 ;
uint8_t chan5 ;
uint8_t chan6 ;
uint8_t chan7 ;
uint8_t chan8 ;
uint8_t chan9 ;
uint8_t chana1 ;
uint8_t chana2 ;
uint8_t chana3 ;
uint8_t chana4 ;
uint8_t chana5 ;
uint8_t chana6 ;
uint8_t chana7 ;
uint8_t chana8 ;
uint8_t chana9 ;


#endif

