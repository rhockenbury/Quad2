/*
globals.h
Defines system global #defines
Authors: Ryler Hockenbury
Date: 5/10/2013  12:05AM
*/

#ifndef globals_h
#define globals_h

#define X 0
#define Y 1
#define Z 2

#define TRUE 1     // these don't seem to be defined in arduino environment
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


#endif /* globals_h */
