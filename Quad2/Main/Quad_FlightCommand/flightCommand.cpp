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
void processFlightCommands(float stickCommands[], float targetFlightAngle[], PID controller[], ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp) {
	// process zero throttle stick commands
    if(stickCommands[THROTTLE_CHANNEL] < STICK_MINCHECK) {
        processZeroThrottleCommands(stickCommands, gyro, accel, comp);
	}

    //if(!inFlight && motorArmed == true)

    // set some in flight check so we cannot disarm motors while flying


    // get flight angles from sticks
	targetFlightAngle[ROLL_AXIS] = AR6210::mapStickCommandToAngle(stickCommands[ROLL_CHANNEL]);
	targetFlightAngle[PITCH_AXIS] = AR6210::mapStickCommandToAngle(stickCommands[PITCH_CHANNEL]);

	// update controller mode
	controlMode = AR6210::mapStickCommandToBool(stickCommands[MODE_CHANNEL]);
	controller[ROLL_AXIS].setMode(controlMode);
	controller[PITCH_AXIS].setMode(controlMode);

	// update aux channel (currently unused)
	//auxMode = AR6210::mapStickCommandToBool(stickCommands[AUX1_CHANNEL]);
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
	}

	// arm motors
	// Left stick bottom right
	if(stickCommands[YAW_CHANNEL] < STICK_MINCHECK && SENSORS_ONLINE && motorArmed == false) {
        Serial.println("Warning: Arming motors");
        // arm motors here
        motorArmed = true;
        inFlight = true;
	}

	// disarm motors
	// Left stick bottom left
	if(stickCommands[YAW_CHANNEL] > STICK_MAXCHECK && motorArmed == ON /*&& safety*/) {
        Serial.println("Warning: Disarming motors");
        // disarm motors here
        motorArmed = false;
        inFlight = false;
	}

	//if(/* motionless or rotors not spinning*/    ) {
	//	safety = true;
	//}
}



/*
void AR6210::processInitCommands(ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp) {
    // We will keep polling the stick commands until
    // the operator initializes the sensors and motors.
    //Serial.println(SYSTEM_ONLINE);
    //while(!SYSTEM_ONLINE) {
        //if(!SENSORS_ONLINE) { LED::LEDBlink(RED_LED_PIN, 1, 1000); }
        //if(!MOTORS_ONLINE) { LED::LEDBlink(YELLOW_LED_PIN, 1, 1000); }

        // Initialize the sensors when right stick is in bottom right position, and
        // left stick is in bottom left position
        if(rawChannelValue[THROTTLE_CHANNEL] < STICK_MINCHECK &&
            rawChannelValue[YAW_CHANNEL] < STICK_MINCHECK &&
            rawChannelValue[PITCH_CHANNEL] < STICK_MINCHECK &&
            rawChannelValue[ROLL_CHANNEL] < STICK_MINCHECK) {

            // initialize IMU
            Serial.println("Initializing IMU");
            //gyro->init();   // zero instead of initing
            //accel->init();
            //comp->init();

            //Serial.println(SYSTEM_ONLINE);
            // initialize current / voltage sensor


            //if(SENSORS_ONLINE) { LED::turnLEDon(RED_LED_PIN); }
        }

        // Arm the motors when right stick is in bottom left position, and
        // left stick is in bottom right position
        if(rawChannelValue[THROTTLE_CHANNEL] < STICK_MINCHECK &&
              rawChannelValue[YAW_CHANNEL] > STICK_MAXCHECK &&
              rawChannelValue[PITCH_CHANNEL] < STICK_MINCHECK &&
              rawChannelValue[ROLL_CHANNEL] > STICK_MAXCHECK &&
              SENSORS_ONLINE) {

            // arm motors ->
        	// wait for beeps
        	// set motors to min values

        	if(MOTORS_ONLINE) { LED::turnLEDon(YELLOW_LED_PIN); }

            inFlight = TRUE;
        }


    }


}
*/

