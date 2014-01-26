/*
 * flightcontrol.cpp
 *
 *  Created on: August 22, 2013
 *      Author: Ryler Hockenbury
 */

#include "flightControl.h"

float levelAdjust[2] = {0.0, 0.0};
//float motorAxisCommand[4] = {0.0, 0.0, 0.0, 0.0};
//float motorCommand[4] = {0.0, 0.0, 0.0, 0.0};

/*
 * Run control loops and command motors
 */
void processFlightControl(float motorAxisCommand[], float motorCommand[], float targetFlightAngle[], float currentFlightAngle[], Motors *motors, PID controller[], float gyroData[], float stickCommands[]) {

	// adjust for difference in target and current position
	//if(PID::getMode() == ON) {
		levelAdjust[ROLL_AXIS] = targetFlightAngle[ROLL_AXIS] - currentFlightAngle[ROLL_AXIS];
		levelAdjust[PITCH_AXIS] = targetFlightAngle[PITCH_AXIS] - currentFlightAngle[PITCH_AXIS];
	//}
	//else {
		//levelAdjust[ROLL_AXIS] = 0.0;
		//levelAdjust[PITCH_AXIS] = 0.0;
	//}

    // compute all axis commands

    float k = 6.0;
    motorAxisCommand[ROLL_AXIS] = controller[ROLL_AXIS].updatePid(1500.0 + gyroData[ROLL_AXIS], stickCommands[ROLL_CHANNEL] + k*levelAdjust[ROLL_AXIS]);
    motorAxisCommand[PITCH_AXIS] = controller[PITCH_AXIS].updatePid(1500.0 + gyroData[PITCH_AXIS], stickCommands[PITCH_CHANNEL] + k*levelAdjust[PITCH_AXIS]);
    motorAxisCommand[YAW_AXIS] = controller[YAW_AXIS].updatePid(1500.0 + gyroData[YAW_AXIS], stickCommands[YAW_CHANNEL]);

    //motorAxisCommand[ROLL_AXIS] = controller[ROLL_AXIS].updatePid(currentFlightAngle[ROLL_AXIS], targetFlightAngle[ROLL_AXIS]);
    //motorAxisCommand[PITCH_AXIS] = controller[PITCH_AXIS].updatePid(currentFlightAngle[PITCH_AXIS], targetFlightAngle[PITCH_AXIS]);
    //motorAxisCommand[YAW_AXIS] = stickCommands[YAW_CHANNEL];

    motorAxisCommand[THROTTLE_AXIS] = stickCommands[THROTTLE_CHANNEL];

    // neglect yaw, roll and pitch  commands before takeoff
    //if(motorAxisCommand[THROTTLE_AXIS] <= TAKEOFF_THROTTLE) {
    //	motorAxisCommand[YAW_AXIS] = 0.0;
    //  motorAxisCommand[ROLL_AXIS] = 0.0;
    //  motorAxisCommand[PITCH_AXIS] = 0.0;
    //}

    motorCommand[MOTOR_FRONTLEFT]= constrain(motorAxisCommand[THROTTLE_AXIS] - motorAxisCommand[PITCH_AXIS] +
    		motorAxisCommand[ROLL_AXIS] + motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    motorCommand[MOTOR_FRONTRIGHT] = constrain(motorAxisCommand[THROTTLE_AXIS] - motorAxisCommand[PITCH_AXIS] -
    		motorAxisCommand[ROLL_AXIS] - motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    motorCommand[MOTOR_BACKLEFT] = constrain(motorAxisCommand[THROTTLE_AXIS] + motorAxisCommand[PITCH_AXIS] +
    		motorAxisCommand[ROLL_AXIS] - motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    motorCommand[MOTOR_BACKRIGHT] = constrain(motorAxisCommand[THROTTLE_AXIS] + motorAxisCommand[PITCH_AXIS] -
    		motorAxisCommand[ROLL_AXIS] + motorAxisCommand[YAW_AXIS], MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);

    // fire up those motors
    if(motors->isArmed()) {
    	if(motorAxisCommand[THROTTLE_AXIS] >= 1200) {
    		motors->commandMotor(MOTOR_FRONTLEFT, motorAxisCommand[THROTTLE_AXIS]); //motorCommand[MOTOR_FRONTLEFT]);
    //		motors->commandMotor(MOTOR_FRONTRIGHT, motorCommand[MOTOR_FRONTRIGHT]);
    //		motors->commandMotor(MOTOR_BACKLEFT, motorCommand[MOTOR_BACKLEFT]);
    //		motors->commandMotor(MOTOR_BACKRIGHT, motorCommand[MOTOR_BACKRIGHT]);
    	}
    	else {
    		motors->commandAllMotors(1200);

    		// ensures motor do not stop, if receive turned off or low throttle
    	}
    }
}

