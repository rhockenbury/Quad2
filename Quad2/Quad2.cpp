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

ITG3200 gyro;		// gyroscope
ADXL345 accel;		// accelerometer
HMC5883L comp;		// magnetometer
AR6210 receiver;	// command radio
PID controller[3];  // pitch, roll and yaw controllers
Motors motors;      // speed controllers
Telemetry com;		// serial radio
//BattMonitor batt;   // battery

float gyroData[3] = {0.0, 0.0, 0.0};  // x, y and z axis
float accelData[3] = {0.0, 0.0, 0.0};
float compData[3] = {0.0, 0.0, 0.0};

float currentFlightAngle[3] = {0.0, 0.0, 0.0};  // roll, pitch, yaw
float targetFlightAngle[2] = {0.0, 0.0}; 		// roll and pitch

float stickCommands[8] = {STICK_COMMAND_MID, STICK_COMMAND_MID,   // roll, pitch,
		                  STICK_COMMAND_MIN, STICK_COMMAND_MID,   // throttle, yaw,
		                  STICK_COMMAND_MAX, STICK_COMMAND_MIN,   // mode, aux 1,
		                  STICK_COMMAND_MID, STICK_COMMAND_MID }; // aux2, aux3

float motorCommand[4] = {0.0, 0.0, 0.0, 0.0};
float motorAxisCommand[4] = {0.0, 0.0, 0.0, 0.0};

uint32_t currentSystemTime = 0;
uint32_t lastSystemTime = 0;
uint32_t deltaSystemTime = 0;

uint32_t last100HzTime = 0;
uint32_t last50HzTime = 0;
uint32_t last10HzTime = 0;

/*
 * Helper routine to call radio ISR
 */
void inline handleReceiverInterruptHelper() {
    receiver.readChannels();
}

/*
 * Safe state
 */
void triggerFailSafe() {
	com.sendTaggedString(ERROR_TAG, "Entering fail safe mode");
	while(true) {
		LED::morseSOS(RED_LED);
	}
}

/*
 * Setup task
 */
void setup() {

  // allow buffer time for telemetry radio to sync
  delay(1000);

  Serial.begin(SERIAL_RATE);
  com.sendTaggedString(INFO_TAG, "Initializing I2C bus...");
  Wire.begin();  //TODO: should move to i2c constructor

  // attach receiver interrupts
  pinMode(PPM_PIN, INPUT);
  attachInterrupt(0, handleReceiverInterruptHelper, FALLING);
  delay(200);

  com.sendTaggedString(INFO_TAG, "Initializing stick receiver...");
  if(!receiver.init()) {
	  com.sendTaggedString(ERROR_TAG, "Could not initialize receiver.");
	  triggerFailSafe();
  }

  com.sendTaggedString(INFO_TAG, "Initializing sensors...");
  if(!gyro.init() || !accel.init() || !comp.init()) {
	  com.sendTaggedString(ERROR_TAG, "Could not initialize all sensors.");
	  triggerFailSafe();
  }

  com.sendTaggedString(INFO_TAG, "Initializing speed controllers");
  motors.init();

  com.sendTaggedString(INFO_TAG, "Entering flight processing loop...");
  LED::circle(3, 200);
}

/*
 * Flight task
 */
void loop() {
  currentSystemTime = millis();
  deltaSystemTime = currentSystemTime - lastSystemTime;
  lastSystemTime = currentSystemTime;

  //Serial.println("current: " + currentSystemTime);
  //Serial.println("100Hz: " + last100HzTime);
  //Serial.println("50Hz: " + last50HzTime);
  //Serial.println("10Hz: " + last10HzTime);
  //Serial.println("last: " + deltaSystemTime);   // loop time

  // TODO Can i make the assumption that these happend periodically as i would except?
  // am i missing deadlines?

  /* 100 Hz Tasks
   * Poll IMU sensors, calculate orientation, update controller and command motors.
   */
  if(currentSystemTime >= (last100HzTime + 10)) {

	if(SENSORS_ONLINE) {
		gyro.getRate(gyroData);
		accel.getValue(accelData);
		comp.getHeading(compData);  // these can go in get orientation

		getOrientation(currentFlightAngle, gyroData, accelData, compData);
	}

    if(RX_ONLINE && SENSORS_ONLINE) {
    	processFlightControl(motorAxisCommand, motorCommand, targetFlightAngle, currentFlightAngle,
    			&motors, controller, gyroData, stickCommands);
    }

    last100HzTime = currentSystemTime;
  }


  /* 50 Hz Tasks
   * Read and process commands from radio.
   */
  if(currentSystemTime >= (last50HzTime + 20)) {

	if(RX_ONLINE) {
		receiver.getStickCommands(stickCommands);
		processFlightCommands(stickCommands, targetFlightAngle, &motors, controller,
				&gyro, &accel, &comp);
	}

	//if(BATT_ONLINE) {
		//TODO - monitor battery health
	        //get voltage
	        //get remaining mah
	        //get remaining time
	//}

    last50HzTime = currentSystemTime;
  }


  /* 10 Hz Tasks
   * Send serial stream to ground station.
   */
  if(currentSystemTime >= (last10HzTime + 100)) {

	// build a serial line and send
    com.serialLineStart();
    //com.serialLineAddFloats(currentFlightAngle, 3);
    com.serialLineAddFloats(stickCommands, 6);
    //com.serialLineAddFloats(targetFlightAngle, 2);
    com.serialLineAddLong((long)receiver.getSyncCounter());
    com.serialLineAddFloats(motorCommand, 4);
    com.serialLineAddFloats(motorAxisCommand, 4);
    com.serialLineSend();


    // receive serial commands
    //if(Serial.available() > 0) {
     //  char cmd = Serial.read();
     //  com.processSerialCommand(cmd, controller);
    //}

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
