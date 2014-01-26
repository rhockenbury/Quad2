/*
 * ITG3200.cpp
 *
 *  Created on: April 21, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for ITG3200 3-channel gyroscope sensor
 * NOTE: Sensor is packaged with HMC5883L and ADXL345
 *
 */

#include "ITG3200.h"
#include "globals.h"

ITG3200::ITG3200() {
	devAddr = ITG3200_ADDR_LOW;
	gyroStatus = OFF;
  
	offset[X] = 0.0;
	offset[Y] = 0.0;
	offset[Z] = 0.0;

	data[X] = 0;
	data[Y] = 0;
	data[Z] = 0;
}

/*
 * Test connection to sensor
 * Returns true if responsive
 */
bool ITG3200::test(){
	return (getID() == devAddr);
}

/*
 * Put sensor in default sampling configuration
 * Range = +/- 2000 deg/sec
 * Sample rate divider = 9 -> sample rate = 100 Hz
 * LPF = 188 Hz bandwidth
 * Clock source = xGyro
 */
bool ITG3200::init() {
	bool status = false;
	if(test()) {
		gyroStatus = ON;
		setFullRange();
		setSampleRate(0x9);
		setLPF(ITG3200_DLPF_188);
		setClockSource(ITG3200_CLK_SEL_XGYRO);
		status = true;
	}
	else {
		Serial.println("WARNING: gyro not responding");
		status = false;
	}

	return status;
}

/*
 * Return ID of sensor
 */
uint8_t ITG3200::getID() {
	I2Cdev::readByte(devAddr, ITG3200_WHOAMI_REGADDR, buffer);
	return buffer[0];
}

/*
 * Set full range
 */
void ITG3200::setFullRange() {
	I2Cdev::writeBits(devAddr, ITG3200_LPFCFG_REGADDR, 4, 2, ITG3200_FS_SEL);
}

/*
 * Set output sample rate
 */
void ITG3200::setSampleRate(uint8_t rate_div) {
	I2Cdev::writeByte(devAddr, ITG3200_SMPLRT_REGADDR, rate_div);
}

/*
 * Set low pass filter
 */
void ITG3200::setLPF(uint8_t LPF) {
	I2Cdev::writeBits(devAddr, ITG3200_LPFCFG_REGADDR, 2, 3, LPF);
}

/*
 * Set the clock source
 */
void ITG3200::setClockSource(uint8_t source) {
	I2Cdev::writeBits(devAddr, ITG3200_PWR_REGADDR, 2, 3, source);
}

/*
 * Set the zero voltage values by averaging 10 samples
 */
void ITG3200::setOffset() {
	if(gyroStatus == ON) {
		int32_t sumX = 0;
		int32_t sumY = 0;
		int32_t sumZ = 0;

		for(uint8_t i = 0; i < 10; i++) {
			getRawData(); // read raw data
    
			sumX = sumX + (int32_t) data[X];
			sumY = sumY + (int32_t) data[Y];
			sumZ = sumZ + (int32_t) data[Z];

			// delay long enough for another sample
			// to be available on the sensor
			// @ 100 Hz -> 0.01 sec
			delay(10);
		}

		offset[X] = (float) sumX / 10.0;
		offset[Y] = (float) sumY / 10.0;
		offset[Z] = (float) sumZ / 10.0;

		// set global gyro status
		vehicleStatus = vehicleStatus | GYRO_READY;
	}
	else {
		Serial.println("WARNING: gyro is not online -> cannot be zeroed");
	}
}

/*
 * Read temperature on sensor
 */
float ITG3200::getTemp() {
	I2Cdev::readBytes(devAddr,ITG3200_TEMPH_REGADDR, 2, buffer);
	return (float) (((int16_t)buffer[0] << 8) | buffer[1]);
}

/*
 * Read raw data
 */
void ITG3200::getRawData() {
	I2Cdev::readBytes(devAddr, ITG3200_XOUTH_REGADDR, 6, buffer);
	data[X] = (((int16_t)buffer[0]) << 8) | buffer[1];
	data[Y] = (((int16_t)buffer[2]) << 8) | buffer[3];
	data[Z] = (((int16_t)buffer[4]) << 8) | buffer[5];
} 

/*
 * Convert raw data to rate in degrees/second
 */
void ITG3200::getRate(float rate[]) {
	getRawData();
  
	rate[X] = ((float)data[X] - offset[X]) / (float) ITG3200_SENSITIVITY;
	rate[Y] = ((float)data[Y] - offset[Y]) / (float) ITG3200_SENSITIVITY;
	rate[Z] = ((float)data[Z] - offset[Z]) / (float) ITG3200_SENSITIVITY;

  /*
      if(rate[X] >= 0) Serial.print(" ");
      Serial.print(rate[X]);
      Serial.print("\t\t");
      if(rate[Y] >= 0) Serial.print(" ");
      Serial.print(rate[Y]);
      Serial.print("\t\t");
      if(rate[Z] >= 0) Serial.print(" ");
      Serial.print(rate[Z]);
      Serial.print("\n");
   */


}



