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
#include "../Quad_Defines/globals.h"

// Constructor
ADXL345::ADXL345()
{
  devAddr = ADXL345_ADDR;
  accelStatus = OFF;

  offset[X] = 0.0;
  offset[Y] = 0.0;
  offset[Z] = 0.0;

  data[X] = 0;
  data[Y] = 0;
  data[Z] = 0;
}



// Method to put sensor in default configuration
// *Sample rate = 100 Hz
// *Range = +/- 8g
// *Enable measurements
void ADXL345::init()
{
  if( test() ) // if sensor responds to ID test
  {
    accelStatus = ON;
	vehicleStatus = vehicleStatus | ACCEL_READY;
  }

  setSampleRate(ADXL345_RATE_100);
  setFormat(ADXL345_RANGE_8G);
  setPowerMode(ON);

  setOffset();
}



//Method to test sensor connection by reading ID value
bool ADXL345::test()
{
  return (getID() == devAddr);
}



//Method to get ID of I2C device
uint8_t ADXL345::getID()
{
  I2Cdev::readByte(devAddr, ADXL345_WHOAMI_REGADDR, buffer);
  return buffer[0];
}



//Method to set the output sample rate
void ADXL345::setSampleRate(uint8_t rate)
{
  I2Cdev::writeBits(devAddr, ADXL345_SMPLRT_REGADDR, ADXL345_BWRATE_BIT,
         ADXL345_BWRATE_LENGTH, rate);
}



//Method to set the range format
void ADXL345::setFormat(uint8_t range)
{
  I2Cdev::writeBits(devAddr, ADXL345_DATAFORMAT_REGADDR, ADXL345_RANGE_BIT,
         ADXL345_RANGE_LENGTH, range);
}




//Method to enable/disable measurement mode
void ADXL345::setPowerMode(bool mode)
{
  I2Cdev::writeBit(devAddr, ADXL345_PWRMODE_REGADDR, ADXL345_MEASURE_BIT,
         mode);
}




//Method to set the offset values (zero voltage values)
// *WARNING: the platform should be leveled first
//           using body mounted bubble level
// *Read 10 samples from sensor and average
// *Place offsets in offset registers
void ADXL345::setOffset()
{
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

  /*
  - Offset registers are 15.6 mg/LSB, and 4g resolution
     is 7.8 mg/LSB so have to divide by 2 before writing
  - Have to divide by ten to average the 10 samples
  - Offsets will be automatically added to data registers
  */

  offset[X] = (float) sumX / 10.0;
  offset[Y] = (float) sumY / 10.0;
  offset[Z] = (float) sumZ / 10.0 - 64;//ADXL345_8GSENSITIVITY;

  //Serial.println(offset[X]);
  //Serial.println(offset[Y]);
  //Serial.println(offset[Z]);

  //I2Cdev::writeByte(devAddr, ADXL345_XOFFSET_REGADDR, (int8_t) -round(sumX / 20.0) );
  //I2Cdev::writeByte(devAddr, ADXL345_YOFFSET_REGADDR, (int8_t) -round(sumY / 20.0) );
  //I2Cdev::writeByte(devAddr, ADXL345_ZOFFSET_REGADDR, (int8_t) -round(sumZ / 20.0) );

}



//Method to read raw data from sensor
void ADXL345::getRawData()
{
  I2Cdev::readBytes(devAddr, ADXL345_XDATA0_REGADDR, 6, buffer);
  data[X] = (((int16_t)buffer[1]) << 8) | buffer[0];
  data[Y] = (((int16_t)buffer[3]) << 8) | buffer[2];
  data[Z] = (((int16_t)buffer[5]) << 8) | buffer[4];


 //Serial.println("X data");
 //Serial.println(data[X]);
 //Serial.println("Y data");
 //Serial.println(data[Y]);
 //Serial.println("Z data");
 //Serial.println(data[Z]);

}



//Method to return gyroscope rate in degrees/second
void ADXL345::getValue(float *value)
{
  getRawData();

  value[X] = ((float)data[X] - offset[X]) / (float) ADXL345_8GSENSITIVITY;
  value[Y] = ((float)data[Y] - offset[Y]) / (float) ADXL345_8GSENSITIVITY;
  value[Z] = ((float)data[Z] - offset[Z]) / (float) 64; //ADXL345_8GSENSITIVITY;
}
