/*
 * serial.cpp
 *
 *  Created on: June 10, 2013
 *      Author: Ryler Hockenbury
 *
 * Create and send a serial block of data.
 *
 */

#include "serial.h"

unsigned int serialCounter = 1;

void serialOpen()
{
  Serial.print(serialCounter);
  #ifdef SERIAL_CHART
    Serial.print(",");
  #endif

  #ifdef SERIAL_MONITOR
    Serial.print("\t\t");
  #endif

  serialCounter++;
}

void serialPrint(float value)
{
  serialPrint(&value, 1);

}

void serialPrint(float values[], int numValues)
{
  //Format for processing or serial chart (comma delimited)
  #ifdef SERIAL_CHART
  for(uint8_t count = 0; count < numValues; count++)
  {
    Serial.print(values[count]);
    if(count < numValues-1) Serial.print(",");
  }

  #endif

  //Format for serial monitor (tab delimited)
  #ifdef SERIAL_MONITOR
  for(uint8_t count = 0; count < numValues; count++)
  {
    if(values[count] >= 0) Serial.print(" ");
    Serial.print(values[count]);
    if(count < numValues-1) Serial.print("\t\t");
  }

  #endif

}

void serialClose()
{
  Serial.print("\n");
}
