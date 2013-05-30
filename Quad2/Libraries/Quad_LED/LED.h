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

#define RED_LED_PIN
#define YELLOW_LED_PIN
#define GREEN_LED_PIN

class LED
{
  public:
    static void redLEDBlink(int numBlinks, int delay);
    static void yellowLEDBlink(int numBlinks, int delay);
    static void greenLEDBlink(int numBlinks, int delay);

  private:

    /* empty */

};



#endif /* LED_h*/
