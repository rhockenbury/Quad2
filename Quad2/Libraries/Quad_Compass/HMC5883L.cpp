/*
 * HMC5883L.cpp
 *
 *  Created on: May 11, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for HMC5883L 3-channel magnetometer sensor
 * NOTE: Sensor is packaged with ITG3200 and ADXL345
 *
 */

#include "HMC5883L.h"
#include "../Quad_Defines/globals.h"



HMC5883L::HMC5883L() {
  devAddr = HMC5883L_ADDR; 
  compStatus = OFF;
  
  offset[X] = 0.0; 
  offset[Y] = 0.0; 
  offset[Z] = 0.0; 

  data[X] = 0;
  data[Y] = 0; 
  data[Z] = 0; 
}


/*
 * Method to put sensor in default configuration
 * Range = +/- 1.3 Ga
 * Sample rate = 75 Hz
 * Continuous sample mode
 */
bool HMC5883L::init() {
  bool status = false;

  if( test() ) {
    compStatus = ON;
    setRange(HMC5883L_GAIN2);
    setSampleRate(HMC5883L_SMPLRT_75HZ);
    setMode(HMC5883L_CONTMODE);
    status = true;
  }
  else {
	  compStatus = OFF;
	  Serial.println("WARNING: compass not responding");
	  status = false;
  }

  return status;
}

/*
 * Test sensor connection
 */
bool HMC5883L::test() {
  I2Cdev::readBytes(devAddr, HMC5883L_IDENTA_REGADDR, 3, buffer); 
  return (buffer[0] == 'H' && buffer[1] == '4' && buffer[2] == '3');
}

/*
 * Set output sample rate
 */
void HMC5883L::setSampleRate(uint8_t rate) {
  I2Cdev::writeBits(devAddr, HMC5883L_CONFIGA_REGADDR, HMC5883L_SMPLRT_BIT, 
         HMC5883L_SMPLRT_LEN, rate); 
}

/*
 * Set the measurement range
 */
void HMC5883L::setRange(uint8_t range) {
  I2Cdev::writeBits(devAddr, HMC5883L_CONFIGB_REGADDR, HMC5883L_GAIN_BIT, 
         HMC5883L_GAIN_LEN, range); 
}

/*
 * Set the mode
 */
void HMC5883L::setMode(uint8_t mode) {
  I2Cdev::writeByte(devAddr, HMC5883L_MODE_REGADDR, mode);
}

/*
 * Set the offset values (zero voltage values)
 * WARNING: platform should be leveled first
 * Read 10 samples from sensor and average
 * Place offsets in offset variables
 */
void HMC5883L::setOffset() {
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
    // @ 75 Hz -> 0.014 sec
    delay(14); 
  }

  // TODO: fix this
  //offset[X] = (float) sumX / 10.0;
  //offset[Y] = (float) sumY / 10.0;
  //offset[Z] = 0.0;// (float) sumZ / 10.0 - (float) HMC5883L_SENSITIVITY2;
  vehicleStatus = vehicleStatus | MAG_READY;
}

/*
 * Read raw data from sensors
 */
void HMC5883L::getRawData() {
  I2Cdev::readBytes(devAddr, HMC5883L_XOUTH_REGADDR, 6, buffer);
  data[X] = (((int16_t)buffer[0]) << 8) | buffer[1];
  data[Y] = (((int16_t)buffer[4]) << 8) | buffer[5];
  data[Z] = (((int16_t)buffer[2]) << 8) | buffer[3];
} 

/*
 * Return heading
 */
void HMC5883L::getHeading(float *heading) {
  getRawData(); 
  
  heading[X] = ((float)data[X] - offset[X]) / (float) HMC5883L_SENSITIVITY2; 
  heading[Y] = ((float)data[Y] - offset[Y]) / (float) HMC5883L_SENSITIVITY2; 
  heading[Z] = ((float)data[Z] - offset[Z]) / (float) HMC5883L_SENSITIVITY2; 
}



