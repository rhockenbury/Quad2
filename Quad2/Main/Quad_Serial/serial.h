/*
 * serial.h
 *
 *  Created on: June 10, 2013
 *      Author: Ryler Hockenbury
 *
 * Create and send a serial frame of data over
 * USB or 3DR radio.
 *
 */

#ifndef serial_h
#define serial_h

#include <arduino.h>

#define SERIAL_CHART          // format for serial chart and processing ground station
//#define SERIAL_MONITOR     // format for serial monitor

void serialOpen();
void serialClose();

void serialPrint(float values[], int numValues);
void serialPrint(float value);

#endif /* serial_h */
