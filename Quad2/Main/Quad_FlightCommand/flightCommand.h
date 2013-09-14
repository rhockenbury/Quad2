/*
 * flightCommand.h
 *
 *  Created on: August 17, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef flightCommand_h
#define flightCommand_h

#include "../../Libraries/Quad_PID/PID.h"
#include "../../Libraries/Quad_Accelerometer/ADXL345.h"
#include "../../Libraries/Quad_Compass/HMC5883L.h"
#include "../../Libraries/Quad_Gyroscope/ITG3200.h"

void processFlightCommands(float stickCommands[], float targetFlightAngle[], PID controller[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp);
void processZeroThrottleCommands(float stickCommands[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp);

#endif /* flightCommand_h */
