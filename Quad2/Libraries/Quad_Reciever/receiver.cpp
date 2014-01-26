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


#include "receiver.h"

AR6210::AR6210() {
    channelStartTime = 0;
    currentChannel = 0;
    syncCounter = 0;

    rawChannelValue[ROLL_CHANNEL] = STICK_COMMAND_MID;
    smoothChannelValue[ROLL_CHANNEL] = STICK_COMMAND_MID;

    rawChannelValue[PITCH_CHANNEL] = STICK_COMMAND_MID;
    smoothChannelValue[PITCH_CHANNEL] = STICK_COMMAND_MID;

    rawChannelValue[THROTTLE_CHANNEL] = STICK_COMMAND_MIN;
    smoothChannelValue[THROTTLE_CHANNEL] = STICK_COMMAND_MIN;

    rawChannelValue[YAW_CHANNEL] = STICK_COMMAND_MID;
    smoothChannelValue[YAW_CHANNEL] = STICK_COMMAND_MID;

    rawChannelValue[MODE_CHANNEL] = STICK_COMMAND_MAX;
    smoothChannelValue[MODE_CHANNEL] = STICK_COMMAND_MAX;

    rawChannelValue[AUX1_CHANNEL] = STICK_COMMAND_MIN;
    smoothChannelValue[AUX1_CHANNEL] = STICK_COMMAND_MIN;

    //unused auxiliary channels 7-8
    rawChannelValue[AUX2_CHANNEL] = 0;
    smoothChannelValue[AUX2_CHANNEL] = 0;

    rawChannelValue[AUX3_CHANNEL] = 0;
    smoothChannelValue[AUX3_CHANNEL] = 0;

    smoothFactor = Storage::readRadioSmoothFactorValue();
}


/*
 * Initialize receiver interrupts
 */
bool AR6210::init() {
	float stickCommands[MAX_CHANNELS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	getStickCommands(stickCommands);

	//TODO: Rebind rx setting channel 5 high (PIDs) and channel 6 low (aux)

	// check if failsafe values are active for each channel
	uint8_t readyCount = 0;
	if(stickCommands[ROLL_CHANNEL] < 1550.0 && stickCommands[ROLL_CHANNEL] > 1450.0) {
		readyCount++;
	}

	if(stickCommands[PITCH_CHANNEL] < 1550.0 && stickCommands[PITCH_CHANNEL] > 1450.0) {
		readyCount++;
	}

	if(stickCommands[THROTTLE_CHANNEL] < STICK_MINCHECK) {
		readyCount++;
	}

	if(stickCommands[YAW_CHANNEL] < 1550.0 && stickCommands[YAW_CHANNEL] > 1450.0) {
		readyCount++;
	}

	if(stickCommands[MODE_CHANNEL] > STICK_MAXCHECK) {
		readyCount++;
	}

	if(stickCommands[AUX1_CHANNEL] < STICK_MINCHECK) {
		readyCount++;
	}

	Serial.println(readyCount);

	//if(readyCount == 6) {
	//	vehicleStatus = vehicleStatus | RX_READY;
		return true;
	//}
	//else {
	//	return false;
	//}

}


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
 * Smooth stick inputs
 */
inline float AR6210::smoothChannels(uint16_t channelValue, uint8_t channelNum) {
	return filter::LPF( (float) channelValue, smoothChannelValue[channelNum], smoothFactor);
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
        if(channel != MODE_CHANNEL && channel != AUX1_CHANNEL) {
            smoothChannelValue[channel] = smoothChannels(stickCommands[channel], channel);
            stickCommands[channel] = smoothChannelValue[channel];
        }

    }
}


/*
 * Convert a raw stick value to a flight angle
 * The max/min pulse length should map to +/- angle
 */
float AR6210::mapStickCommandToAngle(float stickCommand) {

	float stickAngle = (stickCommand - STICK_COMMAND_MIN)*(MAX_COMMAND_ANGLE - MIN_COMMAND_ANGLE) /
			(STICK_COMMAND_MAX - STICK_COMMAND_MIN) + MIN_COMMAND_ANGLE;
    return constrain(stickAngle, MIN_COMMAND_ANGLE, MAX_COMMAND_ANGLE);
}


/*
 * Convert a raw stick value to a ON/OFF state
 */
bool AR6210::mapStickCommandToBool(float stickCommand) {

	if(stickCommand < 1300.0)
	    return false;
	else if(stickCommand > 1700.0) {
		return true;
	}
	else {
		Serial.println("WARNING: Stick command cannot be mapped to boolean type");
		return false;
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
 * Set and save the smooth factor
 */
void AR6210::setSmoothFactor(float factor) {
	smoothFactor = factor;
	Storage::writeRadioSmoothFactorValue(smoothFactor);

    for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++) {
	    smoothChannelValue[channel] = 0.0;  // clear weighted history
    }
}


