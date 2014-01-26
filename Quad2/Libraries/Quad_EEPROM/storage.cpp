/*
 * storage.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: Ryler Hockenbury
 *
 *  Read and write from EEPROM
 */

#include "storage.h"

void Storage::writePidPValue(float value, int address) {
	writeFloat(value, address);
}

void Storage::writePidIValue(float value, int address) {
	writeFloat(value, address);
}

void Storage::writePidDValue(float value, int address) {
	writeFloat(value, address);
}

void Storage::writeRadioSmoothFactorValue(float value, int address){
	writeFloat(value, address);
}

/*
 * Methods to read values will use default values if
 * the USE_DEFAULT_VALUES is set, or no value is stored
 */

float Storage::readPidPValue(int address) {
	float value;

	#ifdef USE_DEFAULT_VALUES
		value = PID_P;
		writePidPValue(value);
	#else
		value = readFloat(address);
		if(value == 0x0 || value == 0xFFFFFFFF) {
			value = PID_P;
			writePidPValue(value);
		}
	#endif

	return value;
}

float Storage::readPidIValue(int address) {
	float value;

	#ifdef USE_DEFAULT_VALUES
		value = PID_I;
		writePidPValue(value);
	#else
		value = readFloat(address);
		if(value == 0x0 || value == 0xFFFFFFFF) {
			value = PID_I;
			writePidIValue(value);
		}
	#endif

	return value;
}

float Storage::readPidDValue(int address) {
	float value;

	#ifdef USE_DEFAULT_VALUES
		value = PID_D;
		writePidPValue(value);
	#else
		value = readFloat(address);
		if(value == 0x0 || value == 0xFFFFFFFF) {
			value = PID_D;
			writePidDValue(value);
		}
	#endif

	return value;
}

float Storage::readRadioSmoothFactorValue(int address) {
	float value;

	#ifdef USE_DEFAULT_VALUES
		value = RADIO_SMOOTH_FACTOR;
		writePidPValue(value);
	#else
		value = readFloat(address);
		if(value == 0x0 || value == 0xFFFFFFFF) {
			value = PID_D;
			writePidDValue(value);
		}
	#endif

	return value;
}


/*
 * Utility method to read a float from an address
 */
float Storage::readFloat(int address) {
	union floatStore {
		byte floatByte[4];
		unsigned short floatUShort[2];
		float floatVal;
	} floatOut;

	for (int i = 0; i < 4; i++) {
		floatOut.floatByte[i] = EEPROM.read(address + i);
	}

	return floatOut.floatVal;
}

/*
 * Utility method to write a float to an address
 */
void Storage::writeFloat(float value, int address) {
	union floatStore {
		byte floatByte[4];
		unsigned short floatUShort[2];
		float floatVal;
	} floatIn;

	floatIn.floatVal = value;

	for (int i = 0; i < 4; i++) {
		EEPROM.write(address + i, floatIn.floatByte[i]);
	}
}

/*
 * Utility method to read a long from an address
 */
long Storage::readLong(int address) {
    union longStore {
    	byte longByte[4];
    	unsigned short longUShort[2];
    	long longVal;
    } longOut;

    for (byte i = 0; i < 4; i++) {
      longOut.longByte[i] = EEPROM.read(address + i);
    }

    return longOut.longVal;
}

/*
 * Utility method to write a long to an address
 */
void Storage::writeLong(long value, int address) {
    union longStore {
    	byte longByte[4];
    	unsigned short longUShort[2];
    	long longVal;
    } longIn;

    longIn.longVal = value;

    for (int i = 0; i < 4; i++) {
      EEPROM.write(address + i, longIn.longByte[i]);
    }
}








