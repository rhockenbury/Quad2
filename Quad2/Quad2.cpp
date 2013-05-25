// Do not remove the include below
#include "Quad2.h"


// Quad2.cpp
// Test IMU functionallity through Serial Chart output
// Authors: Ryler Hockenbury

ITG3200 gyro;
ADXL345 accel;
//HMC5883L comp;

uint8_t vehicleStatus = 0x0;

float gyroData[3] = {0,0,0};
float accelData[3] = {0,0,0};
//float compDatag[3] = {0,0,0};



void setup()
{
  Wire.begin(); // join I2C bus
  Serial.begin(115200); // talk to computer

  Serial.println("Initializing Sensors");
  gyro.init();
  accel.init();

  delay(1000);

}

int counter = 1;  // Serial packet counter for serial chart

// integration variables
unsigned int prevTime = 0;
unsigned int currTime;
unsigned int timeStep;

// pitch, raw, yaw
float pitchGyro = 0.0;
float rollGyro = 0.0;
float yawGyro = 0.0;

float pitchAccel;
float rollAccel;

void loop()
{

  // some setup stuff -> stick inputs
  // getOrientation -> filter
  // getStickControlInputs -> reciever
  // Process orientation and inputs
  // tell motors what to do


  // wrap loop in clock

  currTime = micros();
  gyro.getRate(gyroData);
  accel.getValue(accelData);

  // print counter for serial chart
  Serial.print(counter);
  Serial.print(",");

  ///////////////////////////////////////
  /////// GYRO ANGLE CALCULAITON ////////
  ///////////////////////////////////////

  // update angles
  timeStep = currTime - prevTime;
  pitchGyro = pitchGyro + gyroData[X]*timeStep;
  rollGyro = rollGyro + gyroData[Y]*timeStep;
  yawGyro = yawGyro + gyroData[Z]*timeStep;

  // print angles for serial chart
  Serial.print(pitchGyro);
  Serial.print(",");
  Serial.print(rollGyro);
  Serial.print(",");
  Serial.print(yawGyro);
  Serial.print(",");


  Serial.print(timeStep);
  Serial.print(",");

  prevTime = currTime; // update

  ///////////////////////////////////////
  /////// ACCEL ANGLE CALCULAITON ///////
  ///////////////////////////////////////

  //Accelmagnitude = sqrt(accelData[0]*accelData[0] + accelData[1]*accelData[1] + accelData[2]*accelData[2]);
  pitchAccel = atan2(accelData[1], accelData[2])*180/PI;
  rollAccel = atan2(accelData[0], accelData[2])*180/PI;

  for(byte j= 0; j < 3; j++)
  {
    Serial.print(accelData[j]);
	if(j !=2 ) Serial.print(",");
  }

	// need to convert accelData to degrees

	Serial.print("\n");

	counter++;


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
	delay(1000);


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

}





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
