/*
 * I2Cdev.h
 *
 *  Created on: April 20, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for I2C bus device interaction
 * WARNING:  Must include Wire.h in main sketch
 *
 */

#ifndef I2Cdev_h
#define I2Cdev_h

#include <arduino.h>
#include <inttypes.h>
#include <Wire.h>

class I2Cdev {
	public:
		I2Cdev();

		static bool writeBit(uint8_t devAddr, uint8_t regAddr,
    		uint8_t bitNum, uint8_t data);
		static bool writeBits(uint8_t devAddr, uint8_t regAddr,
    		uint8_t bitStart, uint8_t length, uint8_t data);
		static bool writeByte(uint8_t devAddr, uint8_t regAddr,
    		uint8_t data);
		static bool writeBytes(uint8_t devAddr, uint8_t regAddr,
    		uint8_t length, uint8_t *data);

		static int8_t readBit(uint8_t devAddr, uint8_t regAddr,
				uint8_t bitNum, uint8_t *data);
		static int8_t readBits(uint8_t devAddr, uint8_t regAddr,
				uint8_t bitStart, uint8_t length, uint8_t *data);
		static int8_t readByte(uint8_t devAddr, uint8_t regAddr,
				uint8_t *data);
		static int8_t readBytes(uint8_t devAddr, uint8_t regAddr,
				uint8_t length, uint8_t *data);

		static uint16_t readTimeout;
		static uint8_t errorCode;
		static char* errorMsg;
		static uint16_t errorCounter;

};


#endif  /* I2Cdev_h */
