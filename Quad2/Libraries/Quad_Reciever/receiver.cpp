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


AR6210::AR6210()
{
    channelStartTime = 0;
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

        if(channel == MODE_CHANNEL) {
        	rawChannelValue[channel] = STICK_COMMAND_MAX;
        }

        if(channel == AUX1_CHANNEL) {
        	rawChannelValue[channel] = STICK_COMMAND_MIN;
        }

        smoothFactor[channel] = RADIO_SMOOTH_FACTOR;  // configure in conf.h
        scaleFactor[channel] = RADIO_SCALE_FACTOR;
    }

    vehicleStatus = vehicleStatus | RX_READY;
}


/*
 * Initialize receiver interrupts
 */
//void AR6210::init() {
//    vehicleStatus = vehicleStatus | RX_READY;
//}


/*
 * Read receiver channels and synchronize
 * Interrupt driven on pin change
 */
void AR6210::readChannels() {
    uint32_t currentTime = micros();
    uint32_t channelWidth = currentTime - channelStartTime;
    channelStartTime = currentTime;

    // synchronize with framespace
    if(currentChannel == MAX_CHANNELS) {
        if(channelWidth < MIN_FRAME_WIDTH)
            channelSync();
        else
            currentChannel = 0;
    }
    // read channel or resync if glitch encountered
    else {
       if(channelWidth > MAX_CHANNEL_WIDTH || channelWidth < MIN_CHANNEL_WIDTH)
           channelSync();
       else {
            rawChannelValue[currentChannel] = channelWidth;
            currentChannel++;
       }
    }

}


/*
 * Force channel reader to synchronize with PPM pulses
 */
inline void AR6210::channelSync() {
	currentChannel = MAX_CHANNELS;
	syncCounter++;
}


/*
 * Smooth and scale stick inputs
 */
inline float AR6210::smoothChannels(uint16_t channelValue, uint8_t channelNum) {
	return filter::LPF( (float) channelValue, smoothChannelValue[channelNum], smoothFactor[channelNum]) / scaleFactor[channelNum];
}


/*
 * Safely access shared stick command values
 * To be called every 20 ms
 */
void AR6210::getStickCommands(float stickCommands[MAX_CHANNELS]) {
	uint8_t oldSREG = SREG;   // save interrupt flag
    cli();   // disable interrupts

    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++)
	    stickCommands[channel] = rawChannelValue[channel];

    SREG = oldSREG;   // restore the interrupt flag

    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++) {
    	// don't smooth boolean channel types
        if(channel != MODE_CHANNEL & channel != AUX1_CHANNEL) {
            smoothChannelValue[channel] = smoothChannels(stickCommands[channel], channel);
            stickCommands[channel] = smoothChannelValue[channel];
        }

    }
}


/*
 * Convert a raw stick value to a flight angle
 * The min.(max.) pulse length should map to -45(+45) degrees
 */
float AR6210::mapStickCommandToAngle(float stickCommand) {
    float stickAngle = 0.09*stickCommand - 135.0;
    return constrain(stickAngle, -45.0, 45.0);
}


/*
 * Convert a raw stick value to a ON/OFF state
 */
bool AR6210::mapStickCommandToBool(float stickCommand) {
    if(stickCommand < 1300)
        return FALSE;
    else if(stickCommand > 1700)
        return TRUE;
    else {
    	Serial.println("Warning: Stick command cannot be mapped to boolean type");
    	return FALSE;
    }
}


/*
 * Return the sync counter
 */
uint32_t AR6210::getSyncCounter() {
	uint8_t oldSREG = SREG;   // save interrupt flag
	cli();   // disable interrupts

	uint32_t tempSyncCounter = syncCounter;

	SREG = oldSREG;   // restore the interrupt flag
	return tempSyncCounter;
}


/*
 * Set the smooth factor
 */
void AR6210::setSmoothFactor(float factor[MAX_CHANNELS]) {
    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++) {
	    smoothFactor[channel] = factor[channel];
	    // guard mode and aux
	    smoothChannelValue[channel] = 0.0;  // clear weighted history
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


