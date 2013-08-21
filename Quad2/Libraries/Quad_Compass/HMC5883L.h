/*
 * HMC5883L.h
 *
 *  Created on: May 11, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for HMC5883L 3-channel magnetometer sensor
 * NOTE: Sensor is packaged with ITG3200 and ADXL345
 *
 */

#ifndef HMC5883L_h
#define HMC5883L_h

#include "../Quad_I2C/I2Cdev.h"
#include <inttypes.h>

#define HMC5883L_ADDR   0x1E

#define HMC5883L_CONFIGA_REGADDR  0x00   // configure averging, OSR, and bias
#define HMC5883L_CONFIGB_REGADDR  0x01   // configure gain (range)
#define HMC5883L_MODE_REGADDR     0x02
#define HMC5883L_XOUTH_REGADDR    0x03   // contains X data
#define HMC5883L_XOUTL_REGADDR    0x04
#define HMC5883L_ZOUTH_REGADDR    0x05   // contains Z data
#define HMC5883L_ZOUTL_REGADDR    0x06
#define HMC5883L_YOUTH_REGADDR    0x07   // contains Y data
#define HMC5883L_YOUTL_REGADDR    0x08
#define HMC5883L_STATUS_REGADDR   0x09
#define HMC5883L_IDENTA_REGADDR   0x0A   // contains I2C address
#define HMC5883L_IDENTB_REGADDR   0x0B
#define HMC5883L_IDENTC_REGADDR   0x0C

#define HMC5883L_SMPLRT_BIT  4
#define HMC5883L_SMPLRT_LEN  3

#define HMC5883L_SMPLRT_15HZ 0x04  // output data rate options
#define HMC5883L_SMPLRT_30HZ 0x05
#define HMC5883L_SMPLRT_75HZ 0x06

#define HMC5883L_GAIN_BIT    7
#define HMC5883L_GAIN_LEN    3

#define HMC5883L_GAIN1     0b001   // +/- 1.3Ga
#define HMC5883L_GAIN2     0b010   // +/- 1.9Ga
#define HMC5883L_GAIN3     0b011   // +/- 2.5Ga

#define HMC5883L_CONTMODE  0x00
#define HMC5883L_SINGMODE  0x01

#define HMC5883L_SENSITIVITY1  1090
#define HMC5883L_SENSITIVITY2  820
#define HMC5883L_SENSITIVITY3  660


class HMC5883L
{
  public:
    HMC5883L(); 

    void init(); 
    inline bool test(); 

    inline uint8_t getID(); 

    inline void setMode(uint8_t mode);
    inline void setRange(uint8_t range); 
    inline void setSampleRate(uint8_t rate);

    void setOffset(); 
    void getRawData(); 
    void getHeading(float *heading); 

  private:
    uint8_t devAddr; 
    uint8_t buffer[6]; 

    float offset[3]; 
    int16_t data[3]; 

};

#endif  /* HMC5883L_h */




