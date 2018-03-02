/*
 * badge.c
 *
 * Created: 23.02.2018 21:28:16
 * Author : a_l1
 */ 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "global.h"
#include "LEDs.h"
#include <avr/pgmspace.h>
#include "animation.h"



uint8_t currentAnimation=0;

// prototypes
void shutdown_chip(void);
void blip(void);

// should have taken a switch, sleep stuff is huge
ISR(INT0_vect) {
	sleep_disable();
	
	
	GIMSK &= ~(1 << INT0);
	GIMSK &= ~(1 << PCIE);
	currentAnimation=0;
	
}



int main(void)
{
	
    // this is for the button setup
    PORTB |= (1 << PB1);
    DDRB &= ~(1 << PB1);
	
	animation_init();
	animation_select(currentAnimation);
	sei();
	while(1)
	{
		if (sleep_flag)
		{
			sleep_flag=0;
			shutdown_chip();
		}
		if (isButFlag())
		{
			currentAnimation++;
			if (currentAnimation>7) currentAnimation=0;
			animation_select(currentAnimation);
		}
	}
}

void shutdown_chip()
{
	
	leds_off();

	/* Clear WDRF in MCUSR */
	MCUSR &= ~(1 << WDRF);
	/* Write logical one to WDCE and WDE */
	/* Keep old prescaler setting to prevent unintentional time-out */
	WDTCR |= (1 << WDCE) | (1 << WDE);
	/* Turn off WDT */
	WDTCR = 0x00;

	ADCSRA &= ~(1 << ADEN); // turn off adc

	GIMSK |= (1 << INT0);
	GIMSK |= (1 << PCIE);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei(); // Enable global interrupts
	sleep_cpu();
	sleep_disable();
}



void blip()
{
	charlie(11);
	// delay(100);
	//my_100ms_delay();
	charlie(255);
	// delay(100);
	//my_100ms_delay();
}



