/*
 * ADXL345.h
 *
 *  Created on: May 9, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for ADXL345 3-channel accelerometer sensor
 * NOTE: Sensor is packaged with ITG3200 and HMC5883L
 *
 */

#ifndef ADXL345_h
#define ADXL345_h

#include "../Quad_I2C/I2Cdev.h"
#include <inttypes.h>


#define ADXL345_ADDR    0x53

#define ADXL345_WHOAMI_REGADDR      0x00  // contains I2C address
#define ADXL345_XOFFSET_REGADDR     0x1E  // contains axis offset
#define ADXL345_YOFFSET_REGADDR     0x1F
#define ADXL345_ZOFFSET_REGADDR     0x20
#define ADXL345_SMPLRT_REGADDR      0x2C  // contains sample rate
#define ADXL345_PWRMODE_REGADDR     0x2D  // configures sleep and measure modes
#define ADXL345_DATAFORMAT_REGADDR  0x31  // configures range and resolution
#define ADXL345_XDATA0_REGADDR      0x32  // contains X data
#define ADXL345_XDATA1_REGADDR      0x33
#define ADXL345_YDATA0_REGADDR      0x34  // contains Y data
#define ADXL345_YDATA1_REGADDR      0x35
#define ADXL345_ZDATA0_REGADDR      0x36  // contains Z data
#define ADXL345_ZDATA1_REGADDR      0x37

#define ADXL345_BWRATE_BIT         3
#define ADXL345_BWRATE_LENGTH      4

#define ADXL345_RATE_3200           0b1111   // bandwidth options
#define ADXL345_RATE_1600           0b1110
#define ADXL345_RATE_800            0b1101
#define ADXL345_RATE_400            0b1100
#define ADXL345_RATE_200            0b1011
#define ADXL345_RATE_100            0b1010
#define ADXL345_RATE_50             0b1001
#define ADXL345_RATE_25             0b1000

#define ADXL345_MEASURE_BIT    3
#define ADXL345_RANGE_BIT      1
#define ADXL345_RANGE_LENGTH   2

#define ADXL345_RANGE_2G       0b00  // G-range options
#define ADXL345_RANGE_4G       0b01
#define ADXL345_RANGE_8G       0b10
#define ADXL345_RANGE_16G      0b11

#define ADXL345_2GSENSITIBITY   256  // G-sensitivity options
#define ADXL345_4GSENSITIVITY   128
#define ADXL345_8GSENSITIVITY   74
#define ADXL345_16GSENSITIVITY  32


class ADXL345 {
  public:
    ADXL345();

    void init();
    bool test();

    uint8_t getID();

    void setSampleRate(uint8_t rate);
    void setFormat(uint8_t range);
    void setPowerMode(bool mode);

    void setOffset();
    void getRawData();
    void getValue(float *value);


  private:
    uint8_t devAddr;
    uint8_t buffer[6];
    int16_t data[3];
    float offset[3];

    bool accelStatus;

};


#endif /* ADXL345_h */
