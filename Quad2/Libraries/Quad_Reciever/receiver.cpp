/*
 * receiver.cpp
 *
 *  Created on: May 26, 2013
 *      Author: Ryler Hockenbury
 *
 * Library to read and process PPM input from stick
 * NOTE:  6 channel AR6210 receiver from Spektrum
 *
 */

#include <arduino.h>
#include "receiver.h"
#include "../Quad_Math/math.h"
#include "../Quad_Defines/globals.h"
#include "../Quad_LED/LED.h"


AR6210::AR6210()
{
    channelStartTime = 0.0;
    currentChannel = 0;
    syncCounter = 0;

    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++)
    {
        rawChannelValue[channel] = STICK_COMMAND_MID;
        smoothChannelValue[channel] = STICK_COMMAND_MID;

        if(channel == THROTTLE_CHANNEL) {
            rawChannelValue[channel] = STICK_COMMAND_MIN;
            smoothChannelValue[channel] = STICK_COMMAND_MIN;
        }

        // we need to set a safe value for aux1 and aux2

        smoothFactor[channel] = 0.95;
        scaleFactor[channel] = 0.0;
    }
}


/*
 * Initialize receiver interrupts
 */
void AR6210::init() {
    vehicleStatus = vehicleStatus | RX_READY;
    pinMode(PPM_PIN, INPUT);
    attachInterrupt(0, handleReceiverInterruptHelper, RISING);
}


/*
 * A workaround to force attachInterrupt to call a class method
 */
void handleReceiverInterruptHelper() {
    receiver.readChannels();
}


/*
 * Read receiver channels and synchronize
 */
void AR6210::readChannels() {
    LED::turnLEDon(13);
    LED::turnLEDoff(13);

    // TODO - test with micros()
    uint32_t currentTime = millis();
    uint32_t channelWidth = currentTime - channelStartTime;

    // synchronize with framespace
    if(currentChannel == MAX_CHANNELS) {
        if(channelWidth < MIN_FRAME_WIDTH)
            channelSync();
        else
            currentChannel = 0;
    }
    // read channel or resync if glitch encountered
    else {
        if(channelWidth > MAX_CHANNEL_WIDTH || channelWidth < MIN_CHANNEL_WIDTH) {
            channelSync();
            Serial.println("glitch");
        }
        else {
            rawChannelValue[currentChannel] = channelWidth;
            //smoothChannelValue = smoothChannels();
            currentChannel++;
        }
    }

    channelStartTime = currentTime;
}


/*
 * Force channel reader to synchronize with PPM pulses
 */
void AR6210::channelSync() {
	Serial.println("resyncing");
	currentChannel = MAX_CHANNELS;
	syncCounter++;
}


/*
 * Smooth and scale stick inputs
 */
float AR6210::smoothChannels() {
	return filter::LPF( (float) rawChannelValue[currentChannel], smoothChannelValue[currentChannel],
		                smoothFactor[currentChannel]) / scaleFactor[currentChannel];
}


/*
 * Process system setup stick commands
 */
void AR6210::processInitCommands(ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp) {
    // We will keep polling the stick commands until
    // the operator initializes the sensors and motors.
    Serial.println(SYSTEM_ONLINE);
    while(!SYSTEM_ONLINE) {
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

            Serial.println(SYSTEM_ONLINE);
            // initialize current / voltage sensor


            //if(SENSORS_ONLINE) { LED::turnLEDon(RED_LED_PIN); }
        }

        /*
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
        */

    }

    //TODO - should return value
}


/*
 * Safely access shared stick command values
 */
void AR6210::getStickCommands(float stickCommands[MAX_CHANNELS]) {
    uint8_t oldSREG = SREG;   // save interrupt flag
    cli();   // disable interrupts

    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++)
	    stickCommands[channel] = rawChannelValue[channel]; //smoothChannelValue[channel];

    SREG = oldSREG;   // restore the interrupt flag
}


/*
 * Convert a raw stick value to a flight angle
 * The min.(max.) pulse length should map to -45(+45) degrees
 */
float AR6210::mapStickCommandToAngle(float stickCommand) {
    return (0.09*stickCommand - 135.0);
}


/*
 * Convert a raw stick value to a ON/OFF state
 */
bool AR6210::mapStickCommandToBool(float stickCommand) {
    if(stickCommand < 1500)
        return FALSE;
    else
        return TRUE;
}


/*
 * Return the sync counter
 */
uint32_t AR6210::getSyncCounter() {
    return syncCounter;
}


/*
 * Set the smooth factor
 */
void AR6210::setSmoothFactor(float factor[MAX_CHANNELS]) {
    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++) {
	    smoothFactor[channel] = factor[channel];
    }
}


/*
 * Set the scale factor
 */
void AR6210::setScaleFactor(float factor[MAX_CHANNELS]) {
    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++) {
        scaleFactor[channel] = factor[channel];
    }
}


