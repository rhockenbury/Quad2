
// read PPM

// convert PPM to angle

// take care of zero throttle inputs

#define PPM_PIN 2    // digital pin 2 to support external interrupts

#define NUM_CHANNELS 6

#define STICK_COMMAND_MIN 1000   // 1s pulse length
#define STICK_COMMAND_MID 1500
#define STICK_COMMAND_MAX 2000	 // 2s pulse length

#define STICK_MINCHECK (STICK_COMMAND_MIN + 100)
#define STICK_MAXCHECK (STICK_COMMAND_MAX - 100)

#define MIN_FRAME_WIDTH   (20000 - NUM_CHANNELS*2000)
#define MAX_CHANNEL_WIDTH 2000

// define stick input limits

class AR6210 {

  public:
    AR6210();

    void init();

    void readChannels(); // interrupt driven (every 20 ms? )
    void smoothChannels();
    void processInitCommands(); // calibrate sensors, arm motors
    void convertToAngle();
    void channelSync();


  private:

    unsigned int channelStartTime;
    byte currentChannel;

    int16_t channelValue[NUM_CHANNELS];

    // channels
    // time variables


};
