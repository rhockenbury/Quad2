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
#define STICK_COMMAND_MID 1500   // 1.5s pulse length
#define STICK_COMMAND_MAX 2000	 // 2s pulse length

#define STICK_MINCHECK (STICK_COMMAND_MIN + 100)
#define STICK_MAXCHECK (STICK_COMMAND_MAX - 100)

#define STICK_MINTHROTTLE (STICK_COMMAND_MIN + 100)

#define MIN_FRAME_WIDTH   (20000 - MAX_CHANNELS*2000)
#define MAX_CHANNEL_WIDTH 2000

class AR6210 {

  public:
    AR6210();

    void init();

    void readChannels(); // interrupt driven (every 20 ms? )
    void channelSync();
    float smoothChannels();

    void processInitCommands();
    void getStickCommands(float stickCommands[6]);

  private:

    unsigned int channelStartTime;
    uint8_t currentChannel;

    int16_t rawChannelValue[MAX_CHANNELS];
    volatile float smoothChannelValue[MAX_CHANNELS];

    float smoothFactor[MAX_CHANNELS];
    float scaleFactor[MAX_CHANNELS];

};

#endif  /* receiver_h */
