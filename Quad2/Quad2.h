// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Quad2_H_
#define Quad2_H_
#include "Arduino.h"
//add your includes for the project Quad2 here


#include <Wire.h>
#include "Libraries/Quad_Defines/globals.h"
#include "Main/Quad_FusionFilter/compFilter.h"

#include "Libraries/Quad_Gyroscope/ITG3200.h"
#include "Libraries/Quad_Accelerometer/ADXL345.h"
#include "Libraries/Quad_Compass/HMC5883L.h"

#include "Libraries/Quad_Reciever/receiver.h"


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
