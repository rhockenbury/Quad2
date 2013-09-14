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
 *
 */
Motors::Motors() {

}


/*
 *
 */
void Motors::init() {
    esc[MOTOR1].attach(MOTOR1_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    esc[MOTOR2].attach(MOTOR2_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    esc[MOTOR3].attach(MOTOR3_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
    esc[MOTOR4].attach(MOTOR4_PIN, MOTOR_COMMAND_MIN, MOTOR_COMMAND_MAX);
}


/*
 *
 */
void Motors::armMotors() {
    commandAllMotors(MOTOR_COMMAND_MIN);
    delay(200);
}

void Motors::disarmMotors() {

}

/*
 *
 */
void Motors::pulseMotors(uint8_t numPulses) {
    for(uint8_t pulseIndex; pulseIndex < numPulses; pulseIndex++) {
    	 commandAllMotors(1100);
    	 delay(200);
    }
}


/*
 *
 */
void Motors::commandMotor(uint8_t numMotor, float command) {
    esc[numMotor].writeMicroseconds(command);
}


/*
 *
 */
void Motors::commandAllMotors(float command) {
	esc[MOTOR1].writeMicroseconds(command);
	esc[MOTOR2].writeMicroseconds(command);
	esc[MOTOR3].writeMicroseconds(command);
	esc[MOTOR4].writeMicroseconds(command);
}


