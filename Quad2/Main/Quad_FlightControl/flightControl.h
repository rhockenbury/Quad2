/*
 * flightControl.h
 *
 *  Created on: August 17, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef flightControl_h
#define flightControl_h

#include "globals.h"
#include "motors.h"
#include "PID.h"
#include "conf.h"

void processFlightControl(float motorAxisCommand[], float motorCommand[], float targetFlightAngle[], float currentFlightAngle[],
		Motors *motors, PID controller[], float gyroData[], float stickCommands[]);

#endif /* flightControl_h */
