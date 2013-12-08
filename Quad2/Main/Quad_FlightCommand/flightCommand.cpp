/*
 * flightCommand.cpp
 *
 *  Created on: August 18, 2013
 *      Author: Ryler Hockenbury
 */

#include "flightCommand.h"
#include "../../Libraries/Quad_Defines/globals.h"
#include "../../Libraries/Quad_Reciever/receiver.h"

bool controlMode = 0;
bool auxMode = 0;
bool motorArmed  = false;


/*
 * Distribute stick commands to system components
 */
void processFlightCommands(float stickCommands[], float targetFlightAngle[], PID controller[],
		ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp) {

	// process zero throttle stick commands
    if(stickCommands[THROTTLE_CHANNEL] <= STICK_MINCHECK) {
        processZeroThrottleCommands(stickCommands, gyro, accel, comp);
	}

    if(!inFlight && stickCommands[THROTTLE_CHANNEL] > STICK_MINCHECK && motorArmed) {
    	inFlight = true;
    	LED::turnLEDon(RED_LED_PIN);
    }

    // get flight angles from sticks
	targetFlightAngle[ROLL_AXIS] = AR6210::mapStickCommandToAngle(stickCommands[ROLL_CHANNEL], ROLL_CHANNEL);
	targetFlightAngle[PITCH_AXIS] = AR6210::mapStickCommandToAngle(stickCommands[PITCH_CHANNEL], PITCH_CHANNEL);

	// update controller mode
	controlMode = AR6210::mapStickCommandToBool(stickCommands[MODE_CHANNEL], MODE_CHANNEL);
	controller[ROLL_AXIS].setMode(controlMode);
	controller[PITCH_AXIS].setMode(controlMode);

	// update aux channel (currently unused)
	auxMode = AR6210::mapStickCommandToBool(stickCommands[AUX1_CHANNEL], AUX1_CHANNEL);
}


/*
 * Process system setup and teardown commands
 */
void processZeroThrottleCommands(float stickCommands[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp) {

	// zero sensors - can only be zeroed once
	// Left stick bottom left, right stick bottom right
	if(stickCommands[PITCH_CHANNEL] < STICK_MINCHECK && stickCommands[ROLL_CHANNEL] < STICK_MINCHECK &&
	   stickCommands[YAW_CHANNEL] > STICK_MAXCHECK && !SENSORS_ONLINE) {
		  Serial.println("Info: Zeroing sensors");
          gyro->setOffset();
          accel->setOffset();
          comp->setOffset();

          LED::turnLEDon(GREEN_LED_PIN);
	}

	// arm motors
	// Left stick bottom right
	if(stickCommands[YAW_CHANNEL] < STICK_MINCHECK && SENSORS_ONLINE && motorArmed == false) {
        Serial.println("Warning: Arming motors");
        Motors::init();
        Motors::armMotors();
        Motors::pulseMotors(3);
        Motors::commandAllMotors(1100);
        motorArmed = true;

        LED::turnLEDon(YELLOW_LED_PIN);
	}

	// disarm motors
	// Left stick bottom left
	if(stickCommands[YAW_CHANNEL] > STICK_MAXCHECK && motorArmed == true /*&& safety*/) {
        Serial.println("Warning: Disarming motors");
        Motors::disarmMotors();
        motorArmed = false;
        inFlight = false;
	}

	//if(/* motionless or rotors not spinning*/    ) {
	//	safety = true;
	//}
}

