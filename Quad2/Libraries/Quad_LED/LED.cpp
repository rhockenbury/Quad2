/*
 * LED.cpp
 *
 *  Created on: July 7, 2013
 *      Author: Ryler Hockenbury
 *
 * Library to control onboard status LEDs. Planning to
 * have one red, one yellow and one green LED to indicate
 * system status as appropriate.
 *
 */

#include "LED.h"
#include <arduino.h>

/*
 * Constructor
 */
LED::LED() {
	pinMode(RED_LED, OUTPUT);
	pinMode(YELLOW_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
}

/*
 * Blink LED
 */
void LED::LEDBlink(uint8_t pin, uint8_t numBlinks, int blinkDelay) {
	for(uint8_t blinks = 0; blinks < numBlinks; blinks++) {
		digitalWrite(pin, HIGH);
		delay(blinkDelay);
		digitalWrite(pin, LOW);
		delay(blinkDelay);
	}
}

/*
 * Turn LED on
 */
void LED::turnLEDon(uint8_t pin) {
	digitalWrite(pin, HIGH);
}

/*
 * Turn LED off
 */
void LED::turnLEDoff(uint8_t pin) {
	digitalWrite(pin, LOW);
}

