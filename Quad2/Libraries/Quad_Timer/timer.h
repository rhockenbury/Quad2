/*
 * timer.h
 *
 *  Created on: August 13, 2013
 *      Author: Ryler Hockenbury
 *
 * 32 bit timer
 *
 */

#ifndef timer_h
#define timer_h

extern volatile uint32_t ticks;
uint32_t getCurrentTime();
void initTimer();

#endif /* timer_h */
