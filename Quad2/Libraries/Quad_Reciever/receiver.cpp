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

// Constructor
AR6210::AR6210()
{
   ///
}

//Method to initialize receiver
void AR6210::init()
{
	pinMode(PPM_PIN, INPUT);
	attachInterrupt(0, readChannels, RISING);

	vehicleStatus = vehicleStatus | RX_READY;
}


//Method to read receiver channels and synchronize
void AR6210::readChannels()
{
	unsigned int currentTime = micros();

	unsigned int channelWidth = currentTime - channelStartTime;

	if(currentChannel == MAX_CHANNELS)
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

//Method to run channels through low pass filter
float AR6210::smoothChannels(float current, float previous, int factor)
{
  return previous*(1-factor) + current*factor;
}

//Method to process system setup commands
void AR6210::processInitCommands()
{
	// calibrate sensors
	if(channelValue[THROTTLE] < STICK_MINCHECK && channelValue[YAW] < STICK_MINCHECK)
	{
       /// initialize IMU
		// initialize current / voltage sensor

	}

	// arm motors
	if(channelValue[PITCH] < STICK_MINCHECK && channelValue[ROLL] < STICK_MINCHECK)
	{


	}
}


//Method to map stick position to angle (1ms to 2ms -> -45deg to +45deg)
void AR6210::convertToAngle()
{


}



void AR6210::channelSync()
{


}



