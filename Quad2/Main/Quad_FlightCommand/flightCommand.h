/*
 * flightCommand.h
 *
 *  Created on: August 17, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef flightCommand_h
#define flightCommand_h

#include "../../Libraries/Quad_PID/PID.h"

void processStickCommands(float stickCommands[], float targetFlightAngle[], PID controller[]);

void processZeroThrottleCommands(float stickCommands[]);

#endif /* flightCommand_h */
