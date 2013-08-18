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
bool inFlight = FALSE;

ITG3200 gyro;
ADXL345 accel;
HMC5883L comp;
AR6210 receiver;
PID controller[2]; // pitch and roll controllers

float gyroData[3] = {0.0, 0.0, 0.0};  // x, y and z axis
float accelData[3] = {0.0, 0.0, 0.0};
float compData[3] = {0.0, 0.0, 0.0};

float currentFlightAngle[3] = {0.0, 0.0, 0.0}; // roll, pitch, yaw
float targetFlightAngle[3] = {0.0, 0.0}; // roll and pitch

float stickCommands[6] = {1500, 1500, 1500, 1000, 1000, 1000};  //raw throttle, roll, pitch, yaw

bool auxStatus[2] = {ON, OFF}; // controller mode

uint16_t currentSystemTime = 0; // millis() is a 16 bit timer
uint16_t lastSystemTime = 0;
uint16_t deltaSystemTime = 0;

uint16_t last100HzTime = 0;
uint16_t last50HzTime = 0;
uint16_t last10HzTime = 0;

void setup() {
  //Wire.begin();         // initialize I2C bus
  Serial.begin(57600);  // initialize serial link
  receiver.init();      // initialize receiver

  // process stick inputs to bring sensors and ESCs online
  //Serial.println("Entering process init commands");
  //receiver.processInitCommands(&gyro, &accel, &comp);

  // run system test
  // turn on green LED

  //Serial.println("Initializing Sensors");
  //gyro.init();
  //accel.init();
  //comp.init();

  delay(200);
  Serial.println("Starting main loop");
}


// TODO config file

void loop() {

	//TODO - test with micros()
  currentSystemTime = millis();
  deltaSystemTime = currentSystemTime - lastSystemTime;

  //Serial.println(currentSystemTime);
  //Serial.println(last100HzTime);
  //Serial.println(deltaSystemTime);   // loop time

  if(currentSystemTime < last100HzTime)
	  last100HzTime = 0;

  if(currentSystemTime < last50HzTime)
	  last50HzTime = 0;

  if(currentSystemTime < last10HzTime)
 	  last10HzTime = 0;


  // TODO there is a problem with timer overflow here, and maintaining frequency

  // TODO Can i make the assumption that these happend periodically as i would except?



  /* 100 Hz Tasks
   * Poll IMU sensors, calculate orientation, update controller and command motors.
   *
   */
  if(currentSystemTime >= (last100HzTime + 10)) {
	/*
    gyro.getRate(gyroData);
    accel.getValue(accelData);
    comp.getHeading(compData);
    getOrientation(currentFlightAngle, gyroData, accelData, compData);
	*/

    //levelAdjust[ROLL_AXIS] = targetFlightAngle[ROLL_AXIS] - currentFlightAngle[ROLL_AXIS];
    //levelAdjust[PITCH_AXIS] = targetFlightAngle[PITCH_AXIS] - currentFlightAngle[PITCH_AXIS];

    //motor.axisCommand[ROLL_AXIS] = controller[ROLL_AXIS].updatePid(1500.0 + gyroData[1], stickCommands[ROLL] + levelAdjust[ROLL_AXIS]);
    //motor.axisCommand[PITCH_AXIS] = controller[PITCH_AXIS].updatePid(1500.0 + gyroData[0], stickCommands[PITCH] + levelAdjust[PITCH_AXIS]);

    //motor.axisCommand[THROTTLE] = stickCommands[THROTTLE];
    //motor.axisCommand[YAW_AXIS] = stickCommands[YAW];

    //motor.command[MOTOR1] = throttle + roll + pitch + yaw commands;

    last100HzTime = currentSystemTime;
  }




  /* 50 Hz Tasks
   * Read and sanitize commands from radio, and monitor battery.
   *
   */
  if(currentSystemTime >= (last50HzTime + 20)) {
    receiver.getStickCommands(stickCommands);  //process radio commands

    //targetFlightAngle[ROLL_AXIS] = reciever.mapStickCommandToAngle(stickCommands[ROLL]);
    //targetFlightAngle[PITCH_AXIS] = reciever.mapStickCommandToAngle(stickCommands[PITCH]);

    //bool auxStatus[AUX1] = reciever.mapStickCommandToBool(stickCommands[AUX1]);
    //bool auxStatus[AUX2] = reciever.mapStickCommandToBool(stickCommands[AUX2]);

    //controller[ROLL_AXIS].setMode(aux1Status[AUX1]);
    //controller[PITCH_AXIS].setMode(aux1Status[AUX1]);


    //TODO - monitor battery health

    last50HzTime = currentSystemTime;
  }




  /* 10 Hz Tasks
   * Send serial stream to ground station.
   *
   */
  if(currentSystemTime >= (last10HzTime + 100))
  {
    serialOpen();
    //serialPrint(currentFlightAngle, 3); // must remove extra comma
    serialPrint(stickCommands, 6);
    //serialPrint(battVoltage);
    //serialPrint(battCurrent);
    serialClose();



    // TODO -  check for and process serial input
     //  pid gains
     //  stick scale/senstivitiy



    last10HzTime = currentSystemTime;
  }

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
