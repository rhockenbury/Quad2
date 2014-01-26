/*
 * telemetry.h
 *
 *  Created on: June 10, 2013
 *      Author: Ryler Hockenbury
 *
 * Create and send a serial frame of data over
 * USB or 3DR radio.
 *
 */

#ifndef telemetry_h
#define telemetry_h

#include "receiver.h"
#include "PID.h"
#include "globals.h"
#include "conf.h"

#define INFO_TAG 	"INFO"
#define WARNING_TAG "WARNING"
#define ERROR_TAG 	"ERROR"

class Telemetry {
	public:
		Telemetry();

		void serialLineStart();
		void serialLineSend();

		void serialLineAddFloats(float values[], int numValues);
		void serialLineAddFloat(float value);

		void serialLineAddLong(long value);
		void serialLineAddLongs(long values[], int numValues);

		static void sendTaggedString(String tag, String message);
		static void sendString(String message);

		void processSerialCommand(char cmd, PID controller[], AR6210 receiver);

	private:

		void addDelimiter();
		float readFloatSerial();
		void readValueSerial(char *data, byte size);
		unsigned long serialCounter;
};

#endif /* telemetry_h */
