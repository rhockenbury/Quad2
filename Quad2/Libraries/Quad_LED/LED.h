/*
 * LED.h
 *
 *  Created on: May 30, 2013
 *      Author: Ryler Hockenbury
 *
 * Library to control onboard status LEDs. Planning to
 * have one red, one yellow and one green LED to indicate
 * system status as appropriate.
 *
 */

#ifndef LED_h
#define LED_h

#include <arduino.h>

#define RED_LED     13
#define YELLOW_LED  12
#define GREEN_LED   11

class LED {
  public:
	LED();

    static void LEDBlink(uint8_t pin, uint8_t numBlinks, int delay);
    static void turnLEDon(uint8_t pin);
    static void turnLEDoff(uint8_t pin);

  private:
    /* empty */

};



#endif /* LED_h*/
