/*
 * flightCommand.h
 *
 *  Created on: August 17, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef flightCommand_h
#define flightCommand_h

#include "PID.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "ITG3200.h"
#include "motors.h"

void processFlightCommands(float stickCommands[], float targetFlightAngle[], Motors *motors, PID controller[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp);
void processZeroThrottleCommands(Motors *motors, float stickCommands[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp);

#endif /* flightCommand_h */
