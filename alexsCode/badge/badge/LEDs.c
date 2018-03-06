/*
 * LEDs.c
 *
 * Created: 23.02.2018 21:42:37
 *  Author: a_l1
 */ 

#include "LEDs.h"



const uint8_t __flash portMasks[12][2]={
	{ (1 << PB0) | (1 << PB2), (1 << PB0) },	//1
	{ (1 << PB0) | (1 << PB2), (1 << PB2) },	//2
	{ (1 << PB2) | (1 << PB3), (1 << PB2) },	//3
	{ (1 << PB2) | (1 << PB3), (1 << PB3) },	//4
	{ (1 << PB3) | (1 << PB4), (1 << PB3) },	//5
	{ (1 << PB3) | (1 << PB4), (1 << PB4) },	//6
	{ (1 << PB0) | (1 << PB3), (1 << PB0) },	//7
	{ (1 << PB0) | (1 << PB3), (1 << PB3) },	//8
	{ (1 << PB2) | (1 << PB4), (1 << PB2) },	//9
	{ (1 << PB2) | (1 << PB4), (1 << PB4) },	//10
	{ (1 << PB0) | (1 << PB4), (1 << PB0) },	//11
	{ (1 << PB0) | (1 << PB4), (1 << PB4) }		//12
	};
	
	
void leds_off()
{
	//DDRB &= ~(0b00011101);
	PORTB &= ~(0b00011101);
	DDRB=0;
	//PORTB=0;
}	

void charlie (byte var) 
{
	//if (var>11) return;
	leds_off();
	//nop();
	//PORTB=0;
	//if (var<12)
	{
		PORTB|=portMasks[var][1];
		DDRB=portMasks[var][0];
	}
	//nop();
}


/*
// this is probably already at it's best
void charlie (byte var) {
	leds_off();
	//PORTB=0;
	switch (var) {
		case 0: // 1
		DDRB = (1 << PB0) | (1 << PB2); // output
		PORTB |= (1 << PB0); // HIGH
		break;
		case 1: // 2
		DDRB = (1 << PB0) | (1 << PB2); // output
		PORTB |= (1 << PB2); // HIGH
		break;
		case 2: // 3
		DDRB = (1 << PB2) | (1 << PB3); // output
		PORTB |= (1 << PB2); // HIGH
		break;
		case 3: // 4
		DDRB = (1 << PB2) | (1 << PB3); // output
		PORTB |= (1 << PB3); // HIGH
		break;
		case 4: // 5
		DDRB = (1 << PB3) | (1 << PB4); // output
		PORTB |= (1 << PB3); // HIGH
		break;
		case 5: // 6
		DDRB = (1 << PB3) | (1 << PB4); // output
		PORTB |= (1 << PB4); // HIGH
		break;
		case 6: // 7
		DDRB = (1 << PB0) | (1 << PB3); // output
		PORTB |= (1 << PB0); // HIGH
		break;
		case 7: // 8
		DDRB = (1 << PB0) | (1 << PB3); // output
		PORTB |= (1 << PB3); // HIGH
		break;
		case 8: // 9
		DDRB = (1 << PB2) | (1 << PB4); // output
		PORTB |= (1 << PB2); // HIGH
		break;
		case 9: // 10
		DDRB = (1 << PB2) | (1 << PB4); // output
		PORTB |= (1 << PB4); // HIGH
		break;
		case 10: // 11
		DDRB = (1 << PB0) | (1 << PB4); // output
		PORTB |= (1 << PB0); // HIGH
		break;
		case 11: // 12
		DDRB = (1 << PB0) | (1 << PB4); // output
		PORTB |= (1 << PB4); // HIGH
		break;
		default:
		;

		nop();
	}
}
*/