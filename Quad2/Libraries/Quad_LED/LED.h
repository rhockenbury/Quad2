/*
 * LED.h
 *
 *  Created on: May 30, 2013
 *      Author: Ryler Hockenbury
 *
 * Library to control onboard status LEDs.
 */

#ifndef led_h
#define led_h

#include <arduino.h>
#include "telemetry.h"

#define GREEN_LED   11
#define YELLOW_LED  12
#define RED_LED     13

#define DDRB_GREENPIN_MASK  B00001000;
#define DDRB_YELLOWPIN_MASK B00010000;
#define DDRB_REDPIN_MASK 	B00100000;


class LED {
	public:
		LED();

		static void circle(uint8_t numCircles, int delay);
		static void morseSOS(uint8_t pin);
		static void blink(uint8_t pin, uint8_t numBlinks, int delay);
		static void turnOn(uint8_t pin);
		static void turnOff(uint8_t pin);


		static void setPinModeToOutput(uint8_t pin);
		static void setAllPinModesToOutput();

	private:

};


#endif /* led_h*/
