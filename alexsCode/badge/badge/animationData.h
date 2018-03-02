/*
 * animationData.h
 *
 * Created: 25.02.2018 12:02:07
 *  Author: a_l1
 */ 


#ifndef ANIMATIONDATA_H_
#define ANIMATIONDATA_H_

#include <stdint.h>

// animation where every third LED is glowing
// I could shift that continuosly and drop 4 bytes
#define three_apart_length  3
const __flash uint16_t three_apart[three_apart_length]  = {
	0b0000100100100100,
	0b0000010010010010,
	0b0000001001001001,
};

// this one is huge
// is there a math function to do that instead?
#define updown_length  7
const __flash uint16_t updown[updown_length]  = {
	0b0000000000100000,
	0b0000000001010000,
	0b0000000010001000,
	0b0000000100000100,
	0b0000001000000010,
	0b0000010000000001,
	0b0000100000000000,
};

#define alexsTestAnimation_length 12
const __flash uint16_t  alexsTest[alexsTestAnimation_length] = {
	0b0000000000000001,
	0b0000000000000010,
	0b0000000000000100,
	0b0000000000001000,
	0b0000000000010000,
	0b0000000000100000,
	0b0000000001000000,
	0b0000000010000000,
	0b0000000100000000,
	0b0000001000000000,
    0b0000010000000000,
	0b0000100000000000};

// that's just blinking each led
// maybe inverting the data bitwise does the trick
#define two_apart_length  2
const __flash uint16_t two_apart[two_apart_length]  = {
	0b0000101010101010,
	0b0000010101010101,
};




#endif /* ANIMATIONDATA_H_ */