/*
 * flightCommand.h
 *
 *  Created on: August 17, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef FLIGHTCOMMAND_H_
#define FLIGHTCOMMAND_H_

void processStickCommands(float stickCommands[], float targetFlightAngle[]);

void processZeroThrottleCommands(float stickCommands[]);

#endif /* FLIGHTCOMMAND_H_ */
