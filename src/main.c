/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include <cr_section_macros.h>
#include "driver_config.h"

#include <stdio.h>
#include "gpio.h"
#include "timer32.h"

#define LED_PORT 0
#define LED_BIT 7
#define SHORT_LEN 200
#define LONG_LEN (3 * SHORT_LEN)


typedef struct tuple{
       uint8_t value;
       uint8_t length;
} tuple;

static tuple LUT[16] = {{0x1F, 5}, {0x0F, 5}, {0x07, 5}, {0x03, 5}, {0x01, 5},
		{0x00, 5}, {0x10, 5}, {0x18, 5}, {0x1C, 5}, {0x1E, 5}, {0x01, 2}, {0x08, 4},
		{0x0A, 4}, {0x04, 3}, {0x00, 1}, {0x02, 4}};

static int L1;
static int L2;
static int L3;
static int total_len;

int translateFib(int fibNum);
extern int fibonacci();

int main(void) {

	// Force the counter to be placed into memory
	volatile static int i = 0 ;
	int currentFibString = 0;
	int topBitMask = 0x80000000;
	int j = 0;
	int currentIndex;
	/* Initialize GPIO (sets up clock) */
	GPIOInit();
	/* Set LED port pin to output */
	GPIOSetDir( LED_PORT, LED_BIT, 1 );

	/* init and enable timer */
	init_timer32( 0, TIME_INTERVAL );
	enable_timer32( 0 );

	// start with led off
	GPIOSetValue( LED_PORT, LED_BIT, 0 );

    // Enter an infinite loop, just incrementing a counter
    while(1) {
    	currentFibString = translateFib((i%20) + 1);
    	currentFibString = (currentFibString << (32 - total_len));
    	currentIndex = total_len;
    	for(j=0; j<total_len; ++j)
    	{
    		if(currentFibString & topBitMask)
    		{
    			timer32_0_counter = 0;
    			GPIOSetValue( LED_PORT, LED_BIT, 1 );
    			while(timer32_0_counter < LONG_LEN);
    			GPIOSetValue( LED_PORT, LED_BIT, 0 );
    		}
    		else
    		{
    			timer32_0_counter = 0;
    			GPIOSetValue( LED_PORT, LED_BIT, 1 );
    			while(timer32_0_counter < SHORT_LEN);
    			GPIOSetValue( LED_PORT, LED_BIT, 0 );
    		}

    		if(currentIndex == (L1+1) || currentIndex == (L2+1) || currentIndex == (L3+1))
    		{
    			timer32_0_counter = 0;
    			while(timer32_0_counter < LONG_LEN);
    		}
    		else
    		{
    			timer32_0_counter = 0;
    			while(timer32_0_counter < SHORT_LEN);
    		}

    		currentFibString <<= 1;
    		currentIndex -= 1;
    	}

    	timer32_0_counter = 0;
    	while(timer32_0_counter < (SHORT_LEN * 7));
        i++;

    }
    return 0;
}

int translateFib(int fibNum)
{
	// get the fibonacci number
	int fibResult = fibonacci(fibNum);
	int bitString = 0;
	int currentIndex = 0;
	tuple temp;
	static int mask = 0xF;

	// add first character
	temp = LUT[(mask & fibResult)];
	bitString |= temp.value;
	currentIndex += temp.length;
	// space between characters
	L1 = currentIndex;

	temp = LUT[(mask & (fibResult >> 4))];
	bitString |= (temp.value << currentIndex);
	currentIndex += temp.length;
	// space between characters
	L2 = currentIndex;

	temp = LUT[(mask & (fibResult >> 8))];
	bitString |= (temp.value << currentIndex);
	currentIndex += temp.length;
	// space between characters
	L3 = currentIndex;

	temp = LUT[(mask & (fibResult >> 12))];
	bitString |= (temp.value << currentIndex);
	total_len = currentIndex + temp.length;

	return bitString;
}

