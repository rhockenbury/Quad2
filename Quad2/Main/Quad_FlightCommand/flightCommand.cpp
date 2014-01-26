/*
 * flightCommand.cpp
 *
 *  Created on: August 18, 2013
 *      Author: Ryler Hockenbury
 */

#include "flightCommand.h"
#include "globals.h"
#include "receiver.h"
#include "LED.h"
#include "conf.h"

bool controlMode = 0;
bool auxMode = 0;

/*
 * Distribute stick commands to system components
 */
void processFlightCommands(float stickCommands[], float targetFlightAngle[], Motors *motors,
		PID controller[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp) {

	// process zero throttle stick commands
    if(stickCommands[THROTTLE_CHANNEL] <= STICK_MINCHECK) {
        processZeroThrottleCommands(motors, stickCommands, gyro, accel, comp);
	}

    if(!inFlight && stickCommands[THROTTLE_CHANNEL] > TAKEOFF_THROTTLE && motors->isArmed()) {
    	inFlight = true;
    	LED::turnOn(RED_LED);
    }

    // get flight angles from sticks
	targetFlightAngle[ROLL_AXIS] = AR6210::mapStickCommandToAngle(stickCommands[ROLL_CHANNEL]);
	targetFlightAngle[PITCH_AXIS] = AR6210::mapStickCommandToAngle(stickCommands[PITCH_CHANNEL]);

	// update controller mode
	PID::setMode(AR6210::mapStickCommandToBool(stickCommands[MODE_CHANNEL]));
	//controller[ROLL_AXIS].setMode(controlMode);
	//controller[PITCH_AXIS].setMode(controlMode);
	//controller[YAW_AXIS].setMode(controlMode);

	// update aux channel (currently unused)
	//auxMode = AR6210::mapStickCommandToBool(stickCommands[AUX1_CHANNEL]);
}


/*
 * Process commands to zero sensors, arm and disarm motors.
 */
void processZeroThrottleCommands(Motors *motors, float stickCommands[], ITG3200 *gyro,
		ADXL345 *accel, HMC5883L *comp) {

	// zero sensors
	// Left stick bottom left, right stick bottom right
	if(stickCommands[PITCH_CHANNEL] < STICK_MINCHECK && stickCommands[ROLL_CHANNEL] > STICK_MAXCHECK &&
	   stickCommands[YAW_CHANNEL] > STICK_MAXCHECK && !SENSORS_ONLINE) {
		  Serial.println("Info: Zeroing sensors");
		  LED::turnOn(GREEN_LED);

          gyro->setOffset();
          accel->setOffset();
          comp->setOffset();
	}

	// arm motors
	// Left stick bottom right
	if(stickCommands[YAW_CHANNEL] < STICK_MINCHECK && SENSORS_ONLINE && !motors->isArmed()) {
        Serial.println("Warning: Arming motors");
        LED::turnOn(YELLOW_LED);

        motors->armMotors();
        motors->pulseMotors(3);
        motors->commandAllMotors(1200);
        //motors->setArmed(true);
	}

	// disarm motors
	// Left stick bottom left
	if(stickCommands[YAW_CHANNEL] > STICK_MAXCHECK && motors->isArmed()) {
        Serial.println("Warning: Disarming motors");
        LED::turnOff(YELLOW_LED);
        LED::turnOff(RED_LED);

        motors->disarmMotors();
        //motors->setArmed(false);
        inFlight = false;
	}

}

