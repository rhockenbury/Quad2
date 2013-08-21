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
#include "../Main/Quad_Conf/conf.h"

#define PPM_PIN 2    // digital pin 2 to support external interrupts (Arduino UNO)

#define MAX_CHANNELS RADIO_NUM_CHANNELS

#define STICK_COMMAND_MIN 1000   // 1ms pulse length
#define STICK_COMMAND_MID 1500   // 1.5ms pulse length
#define STICK_COMMAND_MAX 2000	 // 2ms pulse length

#define STICK_MINCHECK (STICK_COMMAND_MIN + 100)
#define STICK_MAXCHECK (STICK_COMMAND_MAX - 100)

#define STICK_MINTHROTTLE (STICK_COMMAND_MIN + 100)

#define MIN_FRAME_WIDTH (20000 - MAX_CHANNELS*2200)
#define MAX_CHANNEL_WIDTH (STICK_COMMAND_MAX + 100)
#define MIN_CHANNEL_WIDTH (STICK_COMMAND_MIN - 100)

class AR6210 {

    public:
        AR6210();
        void init();
        void readChannels();
        inline void channelSync();
        uint32_t getSyncCounter();
    	void getStickCommands(float stickCommands[MAX_CHANNELS]);
    	static float mapStickCommandToAngle(float stickCommand);
    	static bool mapStickCommandToBool(float stickCommand);
    	inline float smoothChannels(uint16_t channelValue, uint8_t channelNum);
    	void setSmoothFactor(float factor[MAX_CHANNELS]);
    	void setScaleFactor(float factor[MAX_CHANNELS]);

 	 private:
    	uint32_t channelStartTime;
    	uint8_t currentChannel;
    	volatile uint32_t syncCounter;
    	volatile int16_t rawChannelValue[MAX_CHANNELS]; // may not need volatile here
    	volatile float smoothChannelValue[MAX_CHANNELS];
    	float smoothFactor[MAX_CHANNELS];
    	float scaleFactor[MAX_CHANNELS];
};

#endif  /* receiver_h */
