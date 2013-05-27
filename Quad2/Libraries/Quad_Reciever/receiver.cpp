/*
 * receiver.cpp
 *
 *  Created on: May 26, 2013
 *      Author: Ryler Hockenbury
 *
 * Read and process stick input from receiver
 * Note:  6 channel PPM with sync period
 *
 */

#include "../Quad_Defines/globals.h"
#include <arduino.h>
#include "receiver.h"

void AR6210::init()
{
	pinMode(PPM_PIN, INPUT);
	attachInterrupt(0, readChannels, RISING);

	vehicleStatus = vehicleStatus | RX_READY;
}



void AR6210::readChannels()
{
	unsigned int currentTime = micros();

	unsigned int channelWidth = currentTime - channelStartTime;

	if(currentChannel == NUM_CHANNELS)
	{

		if(channelWidth < MIN_FRAME_WIDTH)
			channelSync();

		else
			currentChannel = 0;
	}

	else
	{
		if( channelWidth > MAX_CHANNEL_WIDTH)
			channelSync();
		else
			channelValue[currentChannel++] = channelWidth;
	}

	channelStartTime = currentTime;

}




