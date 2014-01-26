// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Quad2_H_
#define Quad2_H_
#include "Arduino.h"
//add your includes for the project Quad2 here

// external libraries
#include <Wire.h>

// main
#include "compFilter.h"
#include "flightCommand.h"
#include "flightControl.h"

// libraries
#include "globals.h"
#include "PID.h"
#include "ITG3200.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "receiver.h"
#include "telemetry.h"
#include "LED.h"


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Quad2 here




//Do not add code below this line
#endif /* Quad2_H_ */
