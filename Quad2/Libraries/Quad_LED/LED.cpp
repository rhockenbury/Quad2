/*
 * LED.cpp
 *
 *  Created on: July 7, 2013
 *      Author: Ryler Hockenbury
 *
 * Library to control onboard status LEDs.
 */

#include "LED.h"

LED::LED() {
	/* empty */
}

/*
 * Turn LEDs on in circular loop
 */
void LED::circle(uint8_t numCircles, int delay) {
	setAllPinModesToOutput();
	for(uint8_t circles = 0; circles < numCircles; circles++) {
		blink(GREEN_LED, 1, delay);
		blink(YELLOW_LED, 1, delay);
		blink(RED_LED, 1, delay);
	}
}

/*
 * Send SOS morse code pattern
 */
void LED::morseSOS(uint8_t pin) {
	setPinModeToOutput(pin);
	blink(pin, 3, 200);
	blink(pin, 3, 400);
	blink(pin, 3, 200);
}

/*
 * Blink LED
 */
void LED::blink(uint8_t pin, uint8_t numBlinks, int blinkDelay) {
	setPinModeToOutput(pin);
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
void LED::turnOn(uint8_t pin) {
	setPinModeToOutput(pin);
	digitalWrite(pin, HIGH);
}

/*
 * Turn LED off
 */
void LED::turnOff(uint8_t pin) {
	int state = digitalRead(pin);
	if(state == HIGH) {
		digitalWrite(pin, LOW);
	}
	else {
		//Telemetry::sendTaggedString(INFO_TAG, "LED on pin " + pin + " is not turned on.");
	}
}

/*
 * Check if pin is set to output mode. If it is
 * not, then set it to output mode.
 */
void LED::setPinModeToOutput(uint8_t pin) {

	byte mode, mask, shift;

	switch(pin) {
	case GREEN_LED:
		mask = DDRB_GREENPIN_MASK;
		shift = 3;
		break;
	case YELLOW_LED:
		mask = DDRB_YELLOWPIN_MASK;
		shift = 4;
		break;
	case RED_LED:
		mask = DDRB_REDPIN_MASK;
		shift = 5;
		break;
	default:
		Telemetry::sendTaggedString(WARNING_TAG, "Invalid LED pin");
		break;
	}

	mode = (DDRB & mask) >> shift;

	if(mode != 1) {
		pinMode(pin, OUTPUT);
	}
}

/*
 *
 */
void LED::setAllPinModesToOutput() {
	setPinModeToOutput(GREEN_LED);
	setPinModeToOutput(YELLOW_LED);
	setPinModeToOutput(RED_LED);
}

