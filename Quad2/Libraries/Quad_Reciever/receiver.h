/*
 * reciever.h
 *
 *  Created on: May 16, 2013
 *      Author: Ryler Hockenbury
 *
 * Library to read and process PPM input from stick
 * NOTE:  6 channel AR6210 receiver from Spektrum
 *
 */

#ifndef receiver_h
#define receiver_h

#include "../Quad_Gyroscope/ITG3200.h"
#include "../Quad_Accelerometer/ADXL345.h"
#include "../Quad_Compass/HMC5883L.h"

#define PPM_PIN 2    // digital pin 2 to support external interrupts (Arduino UNO)

#define MAX_CHANNELS 6

#define STICK_COMMAND_MIN 1000   // 1ms pulse length
#define STICK_COMMAND_MID 1500   // 1.5ms pulse length
#define STICK_COMMAND_MAX 2000	 // 2ms pulse length

#define STICK_MINCHECK (STICK_COMMAND_MIN + 100)
#define STICK_MAXCHECK (STICK_COMMAND_MAX - 100)

#define STICK_MINTHROTTLE (STICK_COMMAND_MIN + 100)

#define MIN_FRAME_WIDTH   (20000 - MAX_CHANNELS*2000)
#define MAX_CHANNEL_WIDTH 2000
#define MIN_CHANNEL_WIDTH 1000

class AR6210 {

 	 public:
    	AR6210();
    	void init();
    	void readChannels(); // interrupt driven on rising edge
    	void channelSync();
    	float smoothChannels();
    	void processInitCommands(ITG3200 *gyro, ADXL345 *accel, HMC5883L *comp); // TODO add motors for arming
    	void getStickCommands(float stickCommands[MAX_CHANNELS]); // every 20 ms
    	float mapStickCommandToAngle(float stickCommand);
    	bool mapStickCommandToBool(float stickCommand);

    	uint32_t getSyncCounter();
    	void setSmoothFactor(float factor[MAX_CHANNELS]);
    	void setScaleFactor(float factor[MAX_CHANNELS]);

 	 private:
    	unsigned int channelStartTime;
    	uint8_t currentChannel;
    	uint32_t syncCounter;

    	volatile int16_t rawChannelValue[MAX_CHANNELS]; // may not need volatile here
    	volatile float smoothChannelValue[MAX_CHANNELS];
    	float smoothFactor[MAX_CHANNELS];
    	float scaleFactor[MAX_CHANNELS];
};

#endif  /* receiver_h */
