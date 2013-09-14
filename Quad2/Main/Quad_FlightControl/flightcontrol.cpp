/*
 * flightcontrol.cpp
 *
 *  Created on: August 22, 2013
 *      Author: Ryler Hockenbury
 */

#include "flightControl.h"

float levelAdjust[2] = {0.0, 0.0};
float motorAxisCommand[4] = {0.0, 0.0, 0.0, 0.0};

/*
void processFlightControl(targetFlightAngle[], currentFlightAngle[], controller[], gyroData[], stickCommands[]) {

    levelAdjust[ROLL_AXIS] = targetFlightAngle[ROLL_AXIS] - currentFlightAngle[ROLL_AXIS];
    levelAdjust[PITCH_AXIS] = targetFlightAngle[PITCH_AXIS] - currentFlightAngle[PITCH_AXIS];

    motorAxisCommand[ROLL_AXIS] = controller[ROLL_AXIS].updatePid(1500.0 + gyroData[1], stickCommands[ROLL_CHANNEL] + levelAdjust[ROLL_AXIS]);
    motorAxisCommand[PITCH_AXIS] = controller[PITCH_AXIS].updatePid(1500.0 + gyroData[0], stickCommands[PITCH_CHANNEL] + levelAdjust[PITCH_AXIS]);

    motorAxisCommand[THROTTLE_AXIS] = stickCommands[THROTTLE_CHANNEL];
    motorAxisCommand[YAW_AXIS] = stickCommands[YAW_CHANNEL];

    // TODO - constrain values
   // motorCommand1 = motorAxisCommand[THROTTLE_AXIS]/4 - motorAxisCommand[PITCH_AXIS]/4 - motorAxisCommand[ROLL_AXIS]/4 + motorAxisCommand[YAW_AXIS]/4;
   // motorCommand2 = motorAxisCommand[THROTTLE_AXIS]/4 + motorAxisCommand[PITCH_AXIS]/4 - motorAxisCommand[ROLL_AXIS]/4 - motorAxisCommand[YAW_AXIS]/4;
   // motorCommand3 = motorAxisCommand[THROTTLE_AXIS]/4 + motorAxisCommand[PITCH_AXIS]/4 + motorAxisCommand[ROLL_AXIS]/4 + motorAxisCommand[YAW_AXIS]/4;
   // motorCommand4 = motorAxisCommand[THROTTLE_AXIS]/4 - motorAxisCommand[PITCH_AXIS]/4 + motorAxisCommand[ROLL_AXIS]/4 - motorAxisCommand[YAW_AXIS]/4;

    //motor.commandMotors(MOTOR1, motorCommand1);
    //motor.commandMotors(MOTOR2, motorCommand2);
    //motor.commandMotors(MOTOR3, motorCommand3);
    //motor.commandMotors(MOTOR4, motorCommand4);
}
*/
