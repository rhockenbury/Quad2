/*
 * telemetry.cpp
 *
 *  Created on: June 10, 2013
 *      Author: Ryler Hockenbury
 *
 * Create and send a serial frame of data over
 * USB or 3DR radio.
 *
 */

#include "telemetry.h"

Telemetry::Telemetry() {
	//Serial.begin(SERIAL_RATE);  // initialize serial link
	serialCounter = 1;
}

/*
 *
 */
void Telemetry::sendTaggedString(String tag, String message) {
	Serial.println(tag + ": " + message);
}

/*
 *
 */
void Telemetry::sendString(String message) {
	Serial.println(message);
}

/*
 *
 */
void Telemetry::serialLineStart() {
	Serial.print(millis());
	serialCounter++;

	addDelimiter();
	Serial.print(vehicleStatus, BIN);
	addDelimiter();
}

/*
 *
 */
void Telemetry::serialLineAddFloat(float value) {
	if(value >= 0) Serial.print(" ");
	Serial.print(value);
	addDelimiter();
}

/*
 *
 */
void Telemetry::serialLineAddLong(long value) {
	if(value >= 0) Serial.print(" ");
	Serial.print(value);
	addDelimiter();
}


/*
 *
 */
void Telemetry::serialLineAddFloats(float values[], int numValues) {
	for(uint8_t count = 0; count < numValues; count++) {
		if(values[count] >= 0) Serial.print(" ");
		Serial.print(values[count]);
		addDelimiter();
	}
}

/*
 *
 */
void Telemetry::serialLineAddLongs(long values[], int numValues) {
	for(uint8_t count = 0; count < numValues; count++) {
		if(values[count] >= 0) Serial.print(" ");
		Serial.print(values[count]);
		addDelimiter();
	}
}

/*
 * End telemtry serial line.
 */
void Telemetry::serialLineSend() {
  Serial.print("\n");
}

/*
 *
 */
void Telemetry::addDelimiter() {

	#ifdef SERIAL_CHART
		Serial.print(",");
 	#endif

 	#ifdef SERIAL_MONITOR
		Serial.print("\t\t");
 	#endif
}

/*
 * Process incoming serial commands.
 */
void Telemetry::processSerialCommand(char cmd, PID controller[], AR6210 receiver) {

	float gain;
	float smoothFactor;

	switch(cmd) {

	case 'P': // fetch PID P value
		Serial.print("Pgain=");
		Serial.print(controller[ROLL_AXIS].getPGain());
		Serial.print("/n");
		break;

	case 'p': // update PID P value
		gain = readFloatSerial();
		controller[ROLL_AXIS].setPGain(gain);
		controller[PITCH_AXIS].setPGain(gain);
		Serial.println("P gain changed");
		break;

	case 'I': // fetch PID I value
		Serial.print("Igain=");
		Serial.print(controller[ROLL_AXIS].getIGain());
		Serial.print("/n");
		break;

	case 'i': // update PID I value
		gain = readFloatSerial();
		controller[ROLL_AXIS].setIGain(gain);
		controller[PITCH_AXIS].setIGain(gain);
		Serial.println("I gain changed");
		break;

	case 'D': // fetch PID D value
		Serial.print("Dgain=");
		Serial.print(controller[ROLL_AXIS].getDGain());
		Serial.print("/n");
		break;

	case 'd': // update PID D values
		gain = readFloatSerial();
		controller[ROLL_AXIS].setDGain(gain);
		controller[PITCH_AXIS].setDGain(gain);
		Serial.println("D gain changed");
		break;

	case 's':
		smoothFactor = readFloatSerial();
		receiver.setSmoothFactor(smoothFactor);
		Serial.println("Smoothfactor changed");
		break;

	case 'e':

		break;

	default:

		break;
	}
}

/*
 * Read float from serial
 */
float Telemetry::readFloatSerial() {
  char data[15] = "";

  readValueSerial(data, sizeof(data));
  return atof(data);
}


/*
 * Read value from serial.
 */
void Telemetry::readValueSerial(char *data, byte size) {
	byte index = 0;
	byte timeout = 0;
	data[0] = '\0';

	do {
		if (Serial.available() == 0) {
			delay(1);
			timeout++;
		}
		else {
			data[index] = Serial.read();
			timeout = 0;
			index++;
		}
	} while ((index == 0 || data[index-1] != ';') && (timeout < 10) && (index < size-1));

	data[index] = '\0';
}




