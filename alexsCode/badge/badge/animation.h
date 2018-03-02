/*
 * animation.h
 *
 * Created: 25.02.2018 12:00:03
 *  Author: a_l1
 */ 


#ifndef ANIMATION_H_
#define ANIMATION_H_

	#include "global.h"
	
	extern volatile uint8_t sleep_flag;
	
	void animation_init(void);
	void animation_select(uint8_t animationNum);
	uint8_t isButFlag (void);


#endif /* ANIMATION_H_ */