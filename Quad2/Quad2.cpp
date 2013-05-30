/*
 * Quad2.h
 *
 *  Created on: May 15, 2013
 *      Author: Ryler Hockenbury
 *
 * Quad2
 */

// Do not remove the include below
#include "Quad2.h"

uint8_t vehicleStatus = 0x0;
bool onGround = TRUE;

ITG3200 gyro;
ADXL345 accel;
HMC5883L comp;

AR6210 receiver;

float gyroData[3] = {0.0, 0.0, 0.0};  // x, y and z axis
float accelData[3] = {0.0, 0.0, 0.0};
float compData[3] = {0.0, 0.0, 0.0};

float currentPosition[3] = {0.0, 0.0, 0.0}; // roll, pitch, yaw
float targetPosition[3] = {0.0, 0.0, 0.0};

float stickCommands[6] = {1500, 1500, 1500, 1500, 1500, 1500};

unsigned int currentSystemTime = 0;
unsigned int lastSystemTime = 0;
unsigned int deltaSystemTime = 0;

unsigned int last100HzTime = 0;
unsigned int last50HzTime = 0;

void setup()
{
  Wire.begin(); // join I2C bus
  Serial.begin(115200); // talk to computer

  Serial.println("Initializing Sensors");
  gyro.init();
  accel.init();
  comp.init();

  delay(1000);
}

void loop()
{
  currentSystemTime = micros();
  deltaSystemTime = currentSystemTime - lastSystemTime;

  //Serial.println(deltaTime);

  /* 100 Hz Tasks
   * Sample critical sensors and calculate orientation by
   * fusing and filtering sensor measurements.
   *
   */

  if(currentSystemTime >= (last100HzTime + 10000)) // 100 Hz tasks
  {
    gyro.getRate(gyroData);
    accel.getValue(accelData);
    comp.getHeading(compData);
    getOrientation(currentPosition, gyroData, accelData, compData);

    last100HzTime = currentSystemTime;
  }

  /* 50 Hz Tasks
   * Read commands from radio and process.
   *
   */

  if(currentSystemTime >= (last50HzTime + 20000)) // 50 Hz tasks
  {
	if(onGround) receiver.processInitCommands();
    receiver.getStickCommands(stickCommands);

    // process stick commands
    //  - convert roll, pitch and yaw to angle (this will be the targetPosition)
    //  - convert throttle to
    //  - convert aux channels to ON/OFF, auto-level mode, autodescent mode ??

    last50HzTime = currentSystemTime;
  }

  // flight controller -> process target and current position

  // command motors 100 Hz or more

  lastSystemTime = currentSystemTime;

}



/////////////////////////////////////////////
// CODE TO READ SERIES OF SENSOR REGSITERS //
/////////////////////////////////////////////

/*
byte buffer[6];

Serial.print("\n");
Serial.println("Reading Initialized Bytes");
I2Cdev::readByte(0x53, 0x0, buffer);
    Serial.print("\n");
	Serial.print(buffer[0], HEX);
	Serial.print("\n");
I2Cdev::readByte(0x53, 0x1E, buffer);
    Serial.print("\n");
	Serial.print(buffer[0]);
	Serial.print("\n");
I2Cdev::readByte(0x53, 0x1F, buffer);
    Serial.print("\n");
	Serial.print(buffer[0]);
	Serial.print("\n");
I2Cdev::readByte(0x53, 0x20, buffer);
    Serial.print("\n");
	Serial.print(buffer[0]);
	Serial.print("\n");


I2Cdev::readByte(0x53, 0x2C, buffer);
    Serial.print("\n");
	Serial.print(buffer[0], BIN);
	Serial.print("\n");
I2Cdev::readByte(0x53, 0x2d, buffer);
    Serial.print("\n");
	Serial.print(buffer[0], BIN);
	Serial.print("\n");
I2Cdev::readByte(0x53, 0x31, buffer);
    Serial.print("\n");
	Serial.print(buffer[0], BIN);
	Serial.print("\n");
*/
//delay(20);


//Serial.print(buffer[0]);
//Serial.print("\n");

//delay(5000);
//gyro.setSampleRate(0x9);

//Serial.println("Writing Byte");
//byte status = I2Cdev::writeByte(0x68, 0x15, 0x9);
//Serial.print(status);

//delay(5000);
//I2Cdev::readBytes(0x68, 0x1D, 6, buffer);
//Serial.print("\n");
//Serial.println("Reading Byte");
//I2Cdev::readByte(0x68, 0x15, buffer);

//Serial.print(buffer[0]);
//Serial.print("\n");




//////////////////////////////////////////////////////////
// TEST LED BLINK PROGRAM
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
////int led = 13;

// the setup routine runs once when you press reset:
////void setup() {
  // initialize the digital pin as an output.
  ////pinMode(led, OUTPUT);
////}

// the loop routine runs over and over again forever:
///void loop() {
  ///digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  ///delay(1000);               // wait for a second
  ///digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  ///delay(1000);               // wait for a second
///}
