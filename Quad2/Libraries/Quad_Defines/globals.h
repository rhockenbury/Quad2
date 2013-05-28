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

#define onGround 0
#define inAir    1

#define GYRO_READY   0x1
#define ACCEL_READY  0x2
#define MAG_READY    0x4
#define BATT_READY   0x8
#define RX_READY     0x16
#define TX_READY     0x32

extern uint8_t vehicleStatus;

#define THROTTLE 1
#define ROLL     2
#define PITCH    3
#define YAW      4
#define AUX1     5
#define AUX2     6


#endif /* globals_h */
