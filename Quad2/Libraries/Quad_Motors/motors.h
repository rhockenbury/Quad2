/*
 * motors.h
 *
 *  Created on: May 30, 2013
 *      Author: Ryler Hockenbury
 */

#ifndef motors_h
#define motors_h

#define MOTOR1 0
#define MOTOR2 1
#define MOTOR3 2
#define MOTOR4 3

#define MOTOR1_PIN 10
#define MOTOR2_PIN 11
#define MOTOR3_PIN 12
#define MOTOR4_PIN 13

#define MOTOR_COMMAND_MIN 1000
#define MOTOR_COMMAND_MAX 2000

class Motors {

    public:
	    Motors();
	    void init();
    	void armMotors();
    	void disarmMotors();
	    void pulseMotors(uint8_t numPulses);
	    void commandMotor(uint8_t numMotor, float command);
        void commandAllMotors(float command);

    private:


};

#endif /* motors_h */
