/*
 * flightcontrol.cpp
 *
 *  Created on: August 22, 2013
 *      Author: Ryler Hockenbury
 */

#include "flightControl.h"

float levelAdjust[2] = {0.0, 0.0};
float motorAxisCommand[4] = {0.0, 0.0, 0.0, 0.0};
float motorCommand[4] = {0.0, 0.0, 0.0, 0.0};

/*
 * Run control loops and command motors
 */
void processFlightControl(float targetFlightAngle[], float currentFlightAngle[], PID controller[], float gyroData[], float stickCommands[]) {

    levelAdjust[ROLL_AXIS] = targetFlightAngle[ROLL_AXIS] - currentFlightAngle[ROLL_AXIS];
    levelAdjust[PITCH_AXIS] = targetFlightAngle[PITCH_AXIS] - currentFlightAngle[PITCH_AXIS];

    // compute all axis commands
    motorAxisCommand[ROLL_AXIS] = controller[ROLL_AXIS].updatePid(1500.0 + gyroData[1], stickCommands[ROLL_CHANNEL] + levelAdjust[ROLL_AXIS]);
    motorAxisCommand[PITCH_AXIS] = controller[PITCH_AXIS].updatePid(1500.0 + gyroData[0], stickCommands[PITCH_CHANNEL] + levelAdjust[PITCH_AXIS]);
    motorAxisCommand[THROTTLE_AXIS] = stickCommands[THROTTLE_CHANNEL];
    motorAxisCommand[YAW_AXIS] = stickCommands[YAW_CHANNEL];

    motorCommand[MOTOR_FRONTLEFT]= constrain(motorAxisCommand[THROTTLE_AXIS] - motorAxisCommand[PITCH_AXIS] +
    		motorAxisCommand[ROLL_AXIS] + motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    motorCommand[MOTOR_FRONTRIGHT] = constrain(motorAxisCommand[THROTTLE_AXIS] - motorAxisCommand[PITCH_AXIS] -
    		motorAxisCommand[ROLL_AXIS] - motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    motorCommand[MOTOR_BACKLEFT] = constrain(motorAxisCommand[THROTTLE_AXIS] + motorAxisCommand[PITCH_AXIS] +
    		motorAxisCommand[ROLL_AXIS] - motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    motorCommand[MOTOR_BACKRIGHT] = constrain(motorAxisCommand[THROTTLE_AXIS] + motorAxisCommand[PITCH_AXIS] -
    		motorAxisCommand[ROLL_AXIS] + motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);

    if(motorArmed) {

    	if(motorAxisCommand[THROTTLE_AXIS] >= 1100) {
    		Motors::commandMotor(MOTOR_FRONTLEFT, motorCommand[MOTOR_FRONTLEFT]);
    		Motors::commandMotor(MOTOR_FRONTRIGHT, motorCommand[MOTOR_FRONTRIGHT]);
    		Motors::commandMotor(MOTOR_BACKLEFT, motorCommand[MOTOR_BACKLEFT]);
    		Motors::commandMotor(MOTOR_BACKRIGHT, motorCommand[MOTOR_BACKRIGHT]);
    	}
    	else {
    		Motors::commandAllMotors(1100);
    	}
    }
}

