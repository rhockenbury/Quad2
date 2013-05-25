/*
battMonitor.h
Defines functionality for Attopilot Current and Voltage Sensor
Authors: Ryler Hockenbury
Date: 5/22/2013 09:38 AM

Physical Connections:
-------------------------
Arduino  | Peripherals
-------- | --------------
Pin A0 --- AttoPilot "V"
Pin A1 --- AttoPilot "I"
GND ------ AttoPilot "GND"

Voltage and current read functions are to be called from timer
based ISR.

*/

#ifndef battMonitor_h
#define battMonitor_h

#include <inttypes.h>

#define VPIN A0
#define IPIN A1

#define V_SENSITIVITY 12.99  // for 180Amp board
#define I_SENSITIVITY 3.7

#define V_FULL    16.8 // volts (4S lipo)
#define V_EMPTY   13.2 // volts (4s lipo)

#define INITIAL_CAPACITY 4000  // 4 amp-hours

class battMonitor
{
  public:
	battMonitor();

	int getRawBattVoltage();
	int getRawBattCurrent();

	float getProcessedBattVoltage();
	float getProcessedBattCurrent();

	float getCurrentBattMaH();
    float getRemainingBattLife();

	void SerialPrintBattStats();

  private:
	int Vraw;
	int Iraw;

	float Vprocessed;
	float Iprocessed;

	float Imax;
	float Imin;

	unsigned long previousTime;
	float capacity;

	bool battMonitorStatus; // where to set this to OFF?

};


#endif  /* battMonitor_h */
