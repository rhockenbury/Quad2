/*
 * conf.h
 *
 *  Created on: August 17, 2013
 *      Author: Ryler Hockenbury
 *
 *  List of default configuration values.  Modified
 *  values are written to EEPROM. Set USE_DEFAULT_VALUE
 *  to reset EEPROM to default values listed here.
 */

#ifndef conf_h
#define conf_h

// debug parameters
//#define I2CDEV_SERIAL_DEBUG
//#define RECEIVER_SERIAL_DEBUG
//#define CONTROLLER_SERIAL_DEBUG

// serial options
#define SERIAL_CHART
//#define SERIAL_MONITOR

/******************************
 *   Runtime Configurations   *
 ******************************/

#define USE_DEFAULT_VALUES

// PID parameters
#define PID_P 1.5
#define PID_I 0.0
#define PID_D 0.0

// throttle command for takeoff
#define TAKEOFF_THROTTLE 1300.0

// Stick responsiveness
#define RADIO_SMOOTH_FACTOR 0.9

// restricts roll/pitch to +/- angle
#define MAX_COMMAND_ANGLE 45.0
#define MIN_COMMAND_ANGLE -45.0


/********************************
 *  Compiletime Configurations  *
 ********************************/

// serial rate for telemetry
#define SERIAL_RATE 57600

// number of radio channels
#define RADIO_NUM_CHANNELS  8


#endif /* conf_h */
