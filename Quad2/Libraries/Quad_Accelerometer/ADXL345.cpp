/*
 * ADXL345.cpp
 *
 *  Created on: May 10, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for ADXL345 3-channel accelerometer sensor
 * NOTE: Sensor is packaged with ITG3200 and HMC5883L
 *
 */

#include "ADXL345.h"
#include "globals.h"

ADXL345::ADXL345() {
    devAddr = ADXL345_ADDR;
    accelStatus = OFF;

    offset[X] = 0.0;
    offset[Y] = 0.0;
    offset[Z] = 0.0;

    data[X] = 0;
    data[Y] = 0;
    data[Z] = 0;
}


/*
 * Initialize responsive sensor to default configuration
 * Sample rate = 100 Hz
 * Range = +/- 8g
 * Enable measurements
 */
bool ADXL345::init() {
	if(test()) {
        accelStatus = ON;
        setSampleRate(ADXL345_RATE_100);
        setFormat(ADXL345_RANGE_8G);
        setPowerMode(ON);
        return true;
    }
    else {
	    accelStatus = OFF;
	    Serial.println("WARNING: accelerometer not responding");
	    return false;
    }
}


/*
 * Test sensor connection by checking device id register value
 */
bool ADXL345::test() {
    return (getID() == ADXL345_DEVID);
}


/*
 * Return sensor device identifier
 */
uint8_t ADXL345::getID() {
    I2Cdev::readByte(devAddr, ADXL345_WHOAMI_REGADDR, buffer);
    return buffer[0];
}


/*
 * Set the sample output rate
 */
void ADXL345::setSampleRate(uint8_t rate) {
    I2Cdev::writeBits(devAddr, ADXL345_SMPLRT_REGADDR, ADXL345_BWRATE_BIT,
         ADXL345_BWRATE_LENGTH, rate);
}


/*
 * Set the range, and enable full resolution
 */
void ADXL345::setFormat(uint8_t range) {
    I2Cdev::writeBits(devAddr, ADXL345_DATAFORMAT_REGADDR, ADXL345_RANGE_BIT,
         ADXL345_RANGE_LENGTH, range);

    I2Cdev::writeBit(devAddr, ADXL345_DATAFORMAT_REGADDR, ADXL345_FULLRES_BIT, ON);
}


/*
 * Set the measurement mode (on or off)
 */
void ADXL345::setPowerMode(bool mode) {
    I2Cdev::writeBit(devAddr, ADXL345_PWRMODE_REGADDR, ADXL345_MEASURE_BIT, mode);
}


/*
 * Set the offset values (zero voltage values)
 * WARNING: the platform should be leveled first
 *           using body mounted bubble level
 * Read 10 samples from sensor and average
 * Place offsets in offset registers
 */
void ADXL345::setOffset() {
    if(accelStatus == ON) {
	    int32_t sumX = 0;
        int32_t sumY = 0;
        int32_t sumZ = 0;

        for(uint8_t i = 0; i < 10; i++)
        {
            getRawData(); // read raw data

            sumX = sumX + (int32_t) data[X];
            sumY = sumY + (int32_t) data[Y];
            sumZ = sumZ + (int32_t) data[Z];

            // delay long enough for another sample to be available
            // @ 100 Hz -> 0.01 seconds
            delay(10);
        }

        offset[X] = (float) sumX / 10.0;
        offset[Y] = (float) sumY / 10.0;
        offset[Z] = (float) sumZ / 10.0 - (float) 256;

        vehicleStatus = vehicleStatus | ACCEL_READY;
    }
    else {
        Serial.println("WARNING: accel is not online -> cannot be zeroed");
    }
}


/*
 * Read raw data
 */
void ADXL345::getRawData() {
    I2Cdev::readBytes(devAddr, ADXL345_XDATA0_REGADDR, 6, buffer);
    data[X] = (((int16_t)buffer[1]) << 8) | buffer[0];
    data[Y] = (((int16_t)buffer[3]) << 8) | buffer[2];
    data[Z] = (((int16_t)buffer[5]) << 8) | buffer[4];
}


/*
 * Return accelerations in g's
 */
void ADXL345::getValue(float *value) {
    getRawData();

    // TODO - calibration work
    value[X] = -((float)data[X] - offset[X]) / (float) ADXL345_8GSENSITIVITY;
    value[Y] = ((float)data[Y] - offset[Y]) / (float) ADXL345_8GSENSITIVITY;
    value[Z] = ((float)data[Z] - offset[Z]) / (float) 263;

   //Serial.print(value[X]);
   //Serial.print(", ");
   //Serial.print(value[Y]);
   //Serial.print(", ");
   //Serial.print(value[Z]);
   //Serial.print("\n");
}
