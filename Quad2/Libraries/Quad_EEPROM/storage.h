/*
 * storage.h
 *
 *  Created on: Dec 10, 2013
 *      Author: Ryler Hockenbury
 *
 *  Read and write from EEPROM
 */

#ifndef STORAGE_h
#define STORAGE_h

#define EEPROM_START_ADDRESSS 0
#define EEPROM_END_ADDRESS    1023

#define PID_P_ADDRESS 0
#define PID_I_ADDRESS 4
#define PID_D_ADDRESS 8

#define TAKEOFF_THROTTLE_ADDRESS 5

#define RADIO_SMOOTH_FACTOR_ADDRESS 5
#define MAX_COMMAND_ANGLE_ADDRESS 5
#define MIN_COMMAND_ANGLE_ADDRESS 5


class Storage {

	public:

		Storage();

		static void writePidPValue(float value, int address=PID_P_ADDRESS);
		static void writePidIValue(float value, int address=PID_I_ADDRESS);
		static void writePidDValue(float value, int address=PID_D_ADDRESS);

		//void writeTakeoffThrottleValue(float value, int address);

		static float readPidPValue(int address=PID_P_ADDRESS);
		static float readPidIValue(int address=PID_I_ADDRESS);
		static float readPidDValue(int address=PID_D_ADDRESS);


	private:


};




#endif /* STORAGE_h */
