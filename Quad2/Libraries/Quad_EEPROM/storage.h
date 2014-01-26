/*
 * storage.h
 *
 *  Created on: Dec 10, 2013
 *      Author: Ryler Hockenbury
 *
 *  Read and write from EEPROM
 */

#ifndef storage_h
#define storage_h

#include <arduino.h>
#include <EEPROM.h>
#include "conf.h"

#define EEPROM_START_ADDRESSS 0
#define EEPROM_END_ADDRESS    1023

#define PID_P_ADDRESS 0
#define PID_I_ADDRESS 4
#define PID_D_ADDRESS 8

#define TAKEOFF_THROTTLE_ADDRESS 12

#define RADIO_SMOOTH_FACTOR_ADDRESS 16

#define MAX_COMMAND_ANGLE_ADDRESS 20
#define MIN_COMMAND_ANGLE_ADDRESS 24


class Storage {

	public:
		Storage();

		static void writePidPValue(float value, int address=PID_P_ADDRESS);
		static void writePidIValue(float value, int address=PID_I_ADDRESS);
		static void writePidDValue(float value, int address=PID_D_ADDRESS);
		static void writeTakeoffThrottleValue(float value, int address=TAKEOFF_THROTTLE_ADDRESS);
		static void writeRadioSmoothFactorValue(float value, int address=RADIO_SMOOTH_FACTOR_ADDRESS);
		static void writeMaxCommandAngleValue(float value, int address=MAX_COMMAND_ANGLE_ADDRESS);
		static void writeMinCommandAngleValue(float value, int address=MIN_COMMAND_ANGLE_ADDRESS);

		static float readPidPValue(int address=PID_P_ADDRESS);
		static float readPidIValue(int address=PID_I_ADDRESS);
		static float readPidDValue(int address=PID_D_ADDRESS);
		static float readTakeoffThrottleValue(int address=TAKEOFF_THROTTLE_ADDRESS);
		static float readRadioSmoothFactorValue(int address=RADIO_SMOOTH_FACTOR_ADDRESS);
		static float readMaxCommandAngleValue(int address=MAX_COMMAND_ANGLE_ADDRESS);
		static float readMinCommandAngleValue(int address=MIN_COMMAND_ANGLE_ADDRESS);

	private:

		static void writeFloat(float value, int address);
		static void writeLong(long value, int address);

		static float readFloat(int address);
		static long readLong(int address);

};

#endif /* storage_h */
