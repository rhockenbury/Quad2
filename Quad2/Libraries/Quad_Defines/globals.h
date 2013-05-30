/*
 * globals.h
 *
 *  Created on: May 10, 2013
 *      Author: Ryler Hockenbury
 *
 * System #defines
 *
 */

#ifndef globals_h
#define globals_h

#define X 0
#define Y 1
#define Z 2

#define TRUE  1
#define FALSE 0

#define ON 1
#define OFF 0

#define GYRO_READY   0x1
#define ACCEL_READY  0x2
#define MAG_READY    0x4
#define BATT_READY   0x8
#define RX_READY     0x16
#define TX_READY     0x32
#define MOTOR_READY  0x64

extern bool onGround;
extern uint8_t vehicleStatus;

// need to adjust these based on radio
#define THROTTLE 0
#define ROLL     1
#define PITCH    2
#define YAW      3
#define AUX1     4
#define AUX2     5

#define MICROS  1000000  // 1 second = 1*10^6 microseconds
#define MILLIS  1000     // 1 second = 1*10^3 milliseconds

#endif /* globals_h */
