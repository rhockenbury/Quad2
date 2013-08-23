/*
 * battMonitor.cpp
 *
 *  Created on: May 22, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for Attopilot current and voltage sensor
 *
 */

#include "battMonitor.h"
#include "../Quad_Defines/globals.h"
#include <Arduino.h>

// Constructor
battMonitor::battMonitor()
{
  Vraw = 0;
  Iraw = 0;
  Iprocessed = 0;
  Vprocessed = 0;

  Imax = 0;
  Imin = 0;

  previousTime = millis();
  capacity = INITIAL_CAPACITY;

  battMonitorStatus = ON;
  vehicleStatus = vehicleStatus | BATT_READY;

}



// Method to read raw battery voltage on analog pin
int battMonitor::getRawBattVoltage()
{
  Vraw = analogRead(VPIN);
  return Vraw;
}



// Method to read raw battery current draw on analog pin
int battMonitor::getRawBattCurrent()
{
  Iraw = analogRead(IPIN);
  return Iraw;
}



// Method to convert raw voltage data to volts and check voltage level
float battMonitor::getProcessedBattVoltage()
{
  Vprocessed = getRawBattVoltage() / I_SENSITIVITY;

  if(Vprocessed <= V_EMPTY)
  {
	  // alert pilot through alarm, flashing LED, telemtry
	  // enter auto descent mode
  }

  return Vprocessed; // in volts
}



// Method to convert raw current data to amps and monitor min and max current
float battMonitor::getProcessedBattCurrent()
{
  Iprocessed = getRawBattCurrent() / I_SENSITIVITY;

  if(Iprocessed > Imax) // update max current
	Imax = Iprocessed;

  if(Iprocessed < Imin) // update min current
    Imin = Iprocessed;

  return Iprocessed;  // in amps

}



// Method to estimate current MaH stored in battery
float battMonitor::getCurrentBattMaH()
{
  unsigned long currentTime = millis();
  unsigned int timestep = currentTime - previousTime;  // millseconds
  previousTime = currentTime;

  /*
  assume battery current draw is constant over timesteps
  need to ensure this function is called frequently enough to make this assumption
  maH = A *ms * 1hr/3600s
  */

  capacity = capacity - getProcessedBattCurrent()*timestep/3600;

  return capacity;
}



// Method to estimate remaining battery life in minutes
float battMonitor::getRemainingBattLife()
{
  return getCurrentBattMaH() / getProcessedBattCurrent()*60.0; // in minutes
}
