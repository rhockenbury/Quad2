/*
 * motors.h
 *
 *  Created on: May 30, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef motors_h
#define motors_h

#define MOTOR_FRONTLEFT  0
#define MOTOR_FRONTRIGHT 1
#define MOTOR_BACKLEFT   2
#define MOTOR_BACKRIGHT  3

#define MOTOR1_PIN 3
#define MOTOR2_PIN 4
#define MOTOR3_PIN 5
#define MOTOR4_PIN 6

#define MOTOR_COMMAND_MIN 1000
#define MOTOR_COMMAND_MAX 2000

#include <arduino.h>
#include <../Quad_Defines/globals.h>

class Motors {

    public:
	    Motors();
	    static void init();
    	static void armMotors();
    	static void disarmMotors();
    	static void killMotors();
	    static void pulseMotors(uint8_t numPulses);
	    static void commandMotor(uint8_t numMotor, int command);
        static void commandAllMotors(int command);

    private:


};

#endif /* motors_h */
