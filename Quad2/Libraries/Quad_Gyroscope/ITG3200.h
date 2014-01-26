/*
 * ITG3200.h
 *
 *  Created on: April 20, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for ITG3200 3-channel gyroscope sensor
 * NOTE: Sensor is packaged with HMC5883L and ADXL345
 *
 */

#ifndef ITG3200_h
#define ITG3200_h

#include "I2Cdev.h"
#include <inttypes.h>

#define ITG3200_ADDR_LOW      0x68
#define ITG3200_ADDR_HIGH     0x69

#define ITG3200_WHOAMI_REGADDR    0x00    // contains I2C address of ITG3205
#define ITG3200_SMPLRT_REGADDR    0x15    // configures OSR divider 
#define ITG3200_LPFCFG_REGADDR    0x16    // configures range and low pass filter
#define ITG3200_TEMPH_REGADDR     0x1B    // contains temperature data
#define ITG3200_TEMPL_REGADDR     0x1C
#define ITG3200_XOUTH_REGADDR     0x1D    // contains X gyro data
#define ITG3200_XOUTL_REGADDR     0x1E
#define ITG3200_YOUTH_REGADDR     0x1F    // contains Y gyro data
#define ITG3200_YOUTL_REGADDR     0x20    
#define ITG3200_ZOUTH_REGADDR     0x21    // contains Z gyro data
#define ITG3200_ZOUTL_REGADDR     0x22
#define ITG3200_PWR_REGADDR       0x3E    // configures reset, power modes, and 
                                          //  clock source

#define ITG3200_FS_SEL        0x3         // full scale range +/- 2000 deg/sec

#define ITG3200_DLPF_256      0x0         // low pass filter bandwidth options
#define ITG3200_DLPF_188      0x1
#define ITG3200_DLPF_98       0x2
#define ITG3200_DLPF_42       0x3
#define ITG3200_DLPF_20       0x4
#define ITG3200_DLPF_10       0x5 
#define ITG3200_DLPF_5        0x6 

#define ITG3200_H_RESET_BIT   256         // power options
#define ITG3200_SLEEP_BIT     128
#define ITG3200_STBY_XG_BIT   64
#define ITG3200_STBY_YG_BIT   32
#define ITG3200_STBY_ZG_BIT   16

#define ITG3200_CLK_SEL_INTERN   0x0      // clock options
#define ITG3200_CLK_SEL_XGYRO    0x1
#define ITG3200_CLK_SEL_YGYRO    0x2
#define ITG3200_CLK_SEL_ZGRYO    0x3

#define ITG3200_SENSITIVITY  14.375

class ITG3200
{
  public:
    ITG3200(); 

    bool init();
    bool test();

    uint8_t getID();

    void setFullRange();
    void setSampleRate(uint8_t rate_div);
    void setLPF(uint8_t LPF);
    void setClockSource(uint8_t source);
    void setOffset(); 

    float getTemp();
    void getRawData();
    void getRate(float rate[]);
    
  
  private:
    uint8_t devAddr; 
    uint8_t buffer[6]; 
    
    float offset[3]; 
    int16_t data[3]; 
    bool gyroStatus;

};


#endif  /* ITG3200_h */
