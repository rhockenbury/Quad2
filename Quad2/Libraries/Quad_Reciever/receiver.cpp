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
#include "../Quad_Defines/globals.h"

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




