/*
 * HMC5883L.cpp
 *
 *  Created on: April 21, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for ITG3200 3-channel gyroscope sensor
 * NOTE: Sensor is packaged with HMC5883L and ADXL345
 *
 */

#include "ITG3200.h"
#include "../Quad_Defines/globals.h"

bool gyroStatus = OFF;

ITG3200::ITG3200() 
{ 
  devAddr = ITG3200_ADDR_LOW; 
  
  offset[X] = 0.0; 
  offset[Y] = 0.0; 
  offset[Z] = 0.0; 

  data[X] = 0;
  data[Y] = 0; 
  data[Z] = 0; 
}


// Method to put sensor in default configuration
// *Range = +/- 2000 deg/sec
// *Sample rate divider = 9 -> sample rate = 100 Hz
// *LPF = 188 Hz bandwidth
// *Clock source = xGyro
void ITG3200::init()
{   
  if( test() ) // if sensor responds to ID test
  {
    gyroStatus = ON;
	vehicleStatus = vehicleStatus | GYRO_READY;
  }

  setFullRange(); 
  setSampleRate(0x9); 
  setLPF(ITG3200_DLPF_188); 
  setClockSource(ITG3200_CLK_SEL_XGYRO); 

  setOffset(); 
}



//Method to test sensor connection 
bool ITG3200::test()
{ 
  return (getID() == devAddr);
}



//Method to get ID of I2C device
uint8_t ITG3200::getID()
{
  I2Cdev::readByte(devAddr, ITG3200_WHOAMI_REGADDR, buffer);
  return buffer[0];
}



//Method to set the full data range 
void ITG3200::setFullRange() 
{
  I2Cdev::writeBits(devAddr, ITG3200_LPFCFG_REGADDR, 4, 2, ITG3200_FS_SEL); 
}



//Method to set the output sample rate 
void ITG3200::setSampleRate(uint8_t rate_div)
{
  I2Cdev::writeByte(devAddr, ITG3200_SMPLRT_REGADDR, rate_div); 
}



//Method to set the low pass filter
void ITG3200::setLPF(uint8_t LPF)
{
  I2Cdev::writeBits(devAddr, ITG3200_LPFCFG_REGADDR, 2, 3, LPF); 
}



//Method to set the clock source
void ITG3200::setClockSource(uint8_t source)
{
  I2Cdev::writeBits(devAddr, ITG3200_PWR_REGADDR, 2, 3, source);
}


//Method to set the offset values (zero voltage values) 
// *WARNING: the platform should be leveled first
//           using body mounted bubble level
// *Read 10 samples from sensor and average
// *Place offsets in offset variables 
void ITG3200::setOffset()
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

    // delay long enough for another sample
    // to be available on the sensor
    // @ 100 Hz -> 0.01 sec
    delay(10); 
  }

  offset[X] = (float) sumX / 10.0;  
  offset[Y] = (float) sumY / 10.0;
  offset[Z] = (float) sumZ / 10.0;

}




//Method to read raw data from sensor
void ITG3200::getRawData() 
{
  I2Cdev::readBytes(devAddr, ITG3200_XOUTH_REGADDR, 6, buffer);
  data[X] = (((int16_t)buffer[0]) << 8) | buffer[1];
  data[Y] = (((int16_t)buffer[2]) << 8) | buffer[3];
  data[Z] = (((int16_t)buffer[4]) << 8) | buffer[5];
} 



//Method to return gyroscope rate in degrees/second
void ITG3200::getRate(float rate[])
{ 
  getRawData(); 
  
  rate[X] = ((float)data[X] - offset[X]) / (float) ITG3200_SENSITIVITY;
  rate[Y] = ((float)data[Y] - offset[Y]) / (float) ITG3200_SENSITIVITY;
  rate[Z] = ((float)data[Z] - offset[Z]) / (float) ITG3200_SENSITIVITY;
}



