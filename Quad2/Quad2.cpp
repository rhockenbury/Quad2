/*
 * Quad2.h
 *
 *  Created on: May 15, 2013
 *      Author: Ryler Hockenbury
 *
 * Quad2
 */

#include "Quad2.h"

uint8_t vehicleStatus = 0x0;
bool inFlight = false;

ITG3200 gyro;
ADXL345 accel;
HMC5883L comp;
AR6210 receiver;
PID controller[2]; // pitch and roll controllers

float gyroData[3] = {0.0, 0.0, 0.0};  // x, y and z axis
float accelData[3] = {0.0, 0.0, 0.0};
float compData[3] = {0.0, 0.0, 0.0};

float currentFlightAngle[3] = {0.0, 0.0, 0.0}; // roll, pitch, yaw
float targetFlightAngle[2] = {0.0, 0.0}; // roll and pitch

float stickCommands[6] = {STICK_COMMAND_MID, STICK_COMMAND_MID,  // roll, pitch, throttle, yaw, mode, aux1
		                  STICK_COMMAND_MIN, STICK_COMMAND_MID,
		                  STICK_COMMAND_MAX, STICK_COMMAND_MIN };

uint32_t currentSystemTime = 0;
uint32_t lastSystemTime = 0;
uint32_t deltaSystemTime = 0;

uint32_t last100HzTime = 0;
uint32_t last50HzTime = 0;
uint32_t last10HzTime = 0;

/*
 * Helper routine to call radio ISR
 */
void handleReceiverInterruptHelper() {
    receiver.readChannels();
}

void setup() {
  //Wire.begin();         // initialize I2C bus
  Serial.begin(115200);  // initialize serial link 57600
  //receiver.init();      // initialize receiver

  Serial.println("INFO: attaching receiver interrupts");
  pinMode(PPM_PIN, INPUT);
  attachInterrupt(0, handleReceiverInterruptHelper, FALLING);
  delay(200);

  //Serial.println("INFO: initializing sensors");
  //gyro.init();
  //accel.init();
  //comp.init();

  // run system test
  // turn on green LED
  delay(200);
  Serial.println("INFO: entering flight processing loop");
}

void loop() {
  currentSystemTime = millis();
  deltaSystemTime = currentSystemTime - lastSystemTime;
  lastSystemTime = currentSystemTime;

  //Serial.println("current:" + currentSystemTime);
  //Serial.println("100Hz:" + last100HzTime);
  //Serial.println("50Hz:" + last50HzTime);
  //Serial.println("10Hz:" + last10HzTime);
  //Serial.println("last:" + deltaSystemTime);   // loop time


  //if(currentSystemTime < last100HzTime)
	//  last100HzTime = 0;

  //if(currentSystemTime < last50HzTime)
	//  last50HzTime = 0;

  //if(currentSystemTime < last10HzTime)
 	//  last10HzTime = 0;


  // TODO there is a problem with timer overflow here, and maintaining frequency
     /// this is fixed, i think, just need testing

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

	// processFlightControl(targetFlightAngle, currentFlightAngle, motors, controller    );

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
   * Read and process commands from radio, and monitor battery.
   *
   */
  if(currentSystemTime >= (last50HzTime + 20)) {
    receiver.getStickCommands(stickCommands);
    processStickCommands(stickCommands, targetFlightAngle, controller);

    //TODO - monitor battery health

    last50HzTime = currentSystemTime;
  }




  /* 10 Hz Tasks
   * Send serial stream to ground station.
   *
   */
  if(currentSystemTime >= (last10HzTime + 100)) {
    serialOpen();
    //serialPrint(currentFlightAngle, 3); // must remove extra comma

    serialPrint(stickCommands, 6);
    serialPrint(targetFlightAngle, 2);

    //serialPrint(battVoltage);
    //serialPrint(battCurrent);
    serialClose();



    // TODO -  check for and process serial input
     //  pid gains
     //  stick scale/senstivitiy



    last10HzTime = currentSystemTime;
  }
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
