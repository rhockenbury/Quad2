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

#define PPM_PIN 2    // digital pin 2 to support external interrupts (Arduino UNO)

#define MAX_CHANNELS 6

#define STICK_COMMAND_MIN 1000   // 1s pulse length
#define STICK_COMMAND_MID 1500
#define STICK_COMMAND_MAX 2000	 // 2s pulse length

#define STICK_MINCHECK (STICK_COMMAND_MIN + 100)
#define STICK_MAXCHECK (STICK_COMMAND_MAX - 100)

#define MIN_FRAME_WIDTH   (20000 - MAX_CHANNELS*2000)
#define MAX_CHANNEL_WIDTH 2000

class AR6210 {

  public:
    AR6210();

    void init();

    void readChannels(); // interrupt driven (every 20 ms? )
    float smoothChannels(float current, float previous, int factor);
    void processInitCommands(); // calibrate sensors, arm motors
    void convertToAngle();
    void channelSync();


  private:

    unsigned int channelStartTime;
    byte currentChannel;

    int16_t rawChannelValue[MAX_CHANNELS];
    float smoothChannelValue[MAX_CHANNELS];


    float smoothFactor[MAX_CHANNELS];
    float scaleFactor[MAX_CHANNELS];

};

#endif  /* receiver_h */
