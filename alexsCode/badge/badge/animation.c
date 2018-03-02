/*
 * animation.c
 *
 * Created: 25.02.2018 12:00:47
 *  Author: a_l1
 */

#include "animation.h"
#include "animationData.h"		// file to store all that animation data
#include <avr/interrupt.h>
#include "LEDs.h"

volatile uint16_t charlieBuffer=0;
volatile uint8_t charlieCOunt=0;
#define  charlieCountMAX  11

volatile uint8_t animationCounter;
volatile uint8_t animationCOunterMAX;
const __flash uint16_t *animationData;
volatile uint8_t subDiV=0;
volatile uint8_t  subDivMAX=50;
#define  subDivMAX_NORMAL 50
#define  subDivMAX_FAST 25

volatile uint8_t leds_enabled=0;

static volatile uint8_t butDiv=0;
#define  butDivMAX 5
static uint8_t butState=2;
static volatile uint8_t oldButState=2;
static volatile uint8_t butEventFlag=0;
static uint8_t butDownCounter=07;
#define  butDownCounterMAX 200
volatile uint8_t sleep_flag=1;

void animation_init(void)
{
	//init Timer
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS02)|(1<<CS00);
	TIMSK0 = (1<<OCIE0A);
	OCR0A = 1;
	
	charlieBuffer=0xFFFF;//alexsTest[0];
	charlieCOunt=0;
	animationCounter=0;
	animationCOunterMAX=alexsTestAnimation_length-1;
	animationData=&alexsTest[0];
	leds_enabled=1;
	
}

void animation_select(uint8_t animationNum)
{
	leds_enabled=0; 
	leds_off();
	animationCounter=0;
	charlieCOunt=0;
	subDivMAX=subDivMAX_NORMAL;
	
	switch(animationNum)
	{
		case 4: subDivMAX=subDivMAX_FAST;
		case 0:	animationCOunterMAX=alexsTestAnimation_length;
				animationData=&alexsTest[0];
				break;
		case 5: subDivMAX=subDivMAX_FAST;
		case 1: animationCOunterMAX=updown_length;
				animationData=&updown[0];
				break;
		case 6: subDivMAX=subDivMAX_FAST;
		case 2:	animationCOunterMAX=three_apart_length;
				animationData=&three_apart[0];
				break;
		case 7: subDivMAX=subDivMAX_FAST;
		case 3: animationCOunterMAX=two_apart_length;
				animationData=&two_apart[0];
				break;	
	}
	
	charlieBuffer=animationData[0];
	leds_enabled=1;
	
}

uint8_t isButFlag (void)
{
	uint8_t temp = butEventFlag;
	butEventFlag = 0;
	return temp;
}

ISR(TIM0_COMPA_vect)
{
	if (leds_enabled)
	{
		if (charlieBuffer&1)
		charlie(charlieCOunt);
		else
		charlie(255);
		charlieCOunt++;
		charlieBuffer = charlieBuffer>>1;
		if (charlieCOunt>charlieCountMAX)
		{
			subDiV++;
			charlieCOunt=0;
			if (subDiV>subDivMAX)
			{
				subDiV=0;
				animationCounter++;
				if (animationCounter>=animationCOunterMAX)
				{
					animationCounter=0;
					
				}
			}
			charlieBuffer=animationData[animationCounter];
		}
	}
	
	butDiv++;
	if (butDiv>butDivMAX)
	{
		butDiv=0;
		butState=(PINB & (1 << PB1));
		if (butState==0)
		{
			butDownCounter++;
			if (butDownCounter>254) butDownCounter=254;
		}
		if ((~oldButState) & butState)
		{
			if (butDownCounter>butDownCounterMAX) sleep_flag=1;
			butEventFlag=1;
			butDownCounter=0;
		}
		oldButState=butState;
		
	}
	
}


