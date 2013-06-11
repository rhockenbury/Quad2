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

// Constructor
AR6210::AR6210()
{
  channelStartTime = 0.0;
  currentChannel = 0;

  for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++)
  {
    rawChannelValue[channel] = STICK_COMMAND_MID;
    smoothChannelValue[channel] = STICK_COMMAND_MID;

    smoothFactor[channel] = 0.95;
    scaleFactor[channel] = 0.0;
  }
}



//Method to initialize receiver
void AR6210::init()
{
  pinMode(PPM_PIN, INPUT);
  //attachInterrupt(0, readChannels, RISING);

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
	else {
	  rawChannelValue[currentChannel] = channelWidth;
	  smoothChannels();
	  currentChannel++;
	}

  }

  channelStartTime = currentTime;

}



//Method to force synchronization
void AR6210::channelSync()
{
  currentChannel = MAX_CHANNELS;
}



//Method to run channels through low pass filter
float AR6210::smoothChannels()
{
  return filter::LPF( (float) rawChannelValue[currentChannel], smoothChannelValue[currentChannel],
		                 smoothFactor[currentChannel]) / scaleFactor[currentChannel];
}



//Method to process system setup stick commands
void AR6210::processInitCommands()
{
  // calibrate sensors
  // if(channelValue[THROTTLE] < STICK_MINCHECK && channelValue[YAW] < STICK_MINCHECK)
  {
       /// initialize IMU
		// initialize current / voltage sensor

  }

  // arm motors
  if(smoothChannelValue[PITCH_CHANNEL] < STICK_MINCHECK && smoothChannelValue[ROLL_CHANNEL] < STICK_MINCHECK)
  {
    // arm motors -> flash some LEDs to show craft is waiting for pilot
	//               arm the motors

	// set motors to min value
	onGround = FALSE;

  }

}


//Method to grab receiver stick commands
void AR6210::getStickCommands(float stickCommands[6])
{
  // disable interrupts
  // read smoothChannelValues

  for(uint8_t channel = 0; channel < MAX_CHANNELS; channel++)
    stickCommands[channel] = smoothChannelValue[channel];

  // enable interrupts

}



