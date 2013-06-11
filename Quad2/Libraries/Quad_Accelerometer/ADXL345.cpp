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



//Method to set the range, and enable full resolution
void ADXL345::setFormat(uint8_t range)
{
  I2Cdev::writeBits(devAddr, ADXL345_DATAFORMAT_REGADDR, ADXL345_RANGE_BIT,
         ADXL345_RANGE_LENGTH, range);

  I2Cdev::writeBit(devAddr, ADXL345_DATAFORMAT_REGADDR, ADXL345_FULLRES_BIT, ON);
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

  offset[X] = (float) sumX / 10.0;
  offset[Y] = (float) sumY / 10.0;
  offset[Z] = (float) sumZ / 10.0 - (float) 256;

  //Serial.println(offset[X]);
  //Serial.println(offset[Y]);
  //Serial.println(offset[Z]);

}



//Method to read raw data from sensor
void ADXL345::getRawData()
{
  I2Cdev::readBytes(devAddr, ADXL345_XDATA0_REGADDR, 6, buffer);
  data[X] = (((int16_t)buffer[1]) << 8) | buffer[0];
  data[Y] = (((int16_t)buffer[3]) << 8) | buffer[2];
  data[Z] = (((int16_t)buffer[5]) << 8) | buffer[4];

 /*
 Serial.println("X data");
 Serial.println(data[X]);
 Serial.println("Y data");
 Serial.println(data[Y]);
 Serial.println("Z data");
 Serial.println(data[Z]);
 */

}



//Method to return gyroscope rate in degrees/second
void ADXL345::getValue(float *value)
{
  getRawData();

  value[X] = -((float)data[X] - offset[X]) / (float) ADXL345_8GSENSITIVITY;
  value[Y] = ((float)data[Y] - offset[Y]) / (float) ADXL345_8GSENSITIVITY;
  value[Z] = ((float)data[Z] - offset[Z]) / (float) 256;

/*
    if(value[X] >= 0) Serial.print(" ");
    Serial.print(value[X]);
    Serial.print("\t\t");
    if(value[Y] >= 0) Serial.print(" ");
    Serial.print(value[Y]);
    Serial.print("\t\t");
    if(value[Z] >= 0) Serial.print(" ");
    Serial.print(value[Z]);
    Serial.print("\n");
*/

}
