/*
 * motors.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Ryler Hockenbury
 *
 */

#include "motors.h"
#include <servo.h>

Servo esc[4];


/*
 * Empty
 */
Motors::Motors() {

}


/*
 * Bind ESC variables to output pins
 */
void Motors::init() {
	esc[MOTOR_FRONTLEFT].attach(MOTOR1_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
	esc[MOTOR_FRONTRIGHT].attach(MOTOR2_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
	esc[MOTOR_BACKLEFT].attach(MOTOR3_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
	esc[MOTOR_BACKRIGHT].attach(MOTOR4_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);

	vehicleStatus = vehicleStatus | MOTOR_READY;
}


/*
 * Arm motors
 */
void Motors::armMotors() {
    commandAllMotors(MOTOR_COMMAND_MIN);
    delay(200);

    // delay long enough for start up tones, battery cell
    // count tones, and long beep tones
}

/*
 * Disarm motors
 */
void Motors::disarmMotors() {
	commandAllMotors(MOTOR_COMMAND_MIN);

}

/*
 * Disconnect motors
 */
void Motors::killMotors() {
	esc[MOTOR_FRONTLEFT].detach();
	esc[MOTOR_FRONTRIGHT].detach();
	esc[MOTOR_BACKLEFT].detach();
	esc[MOTOR_BACKRIGHT].detach();
}

/*
 * Pulse motors are low rotational speed
 */
void Motors::pulseMotors(uint8_t numPulses) {
    for(uint8_t pulseIndex; pulseIndex < numPulses; pulseIndex++) {
    	 commandAllMotors(MOTOR_COMMAND_MIN + 100);
    	 delay(200);
    }
}


/*
 * Command numMotor to input command
 */
void Motors::commandMotor(uint8_t numMotor, int command) {
    esc[numMotor].writeMicroseconds(command);
}


/*
 * Command all motors to input command
 */
void Motors::commandAllMotors(int command) {
	esc[MOTOR_FRONTLEFT].writeMicroseconds(command);
	esc[MOTOR_FRONTRIGHT].writeMicroseconds(command);
	esc[MOTOR_BACKLEFT].writeMicroseconds(command);
	esc[MOTOR_BACKRIGHT].writeMicroseconds(command);
}


