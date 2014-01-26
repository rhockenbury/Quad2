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

#include <arduino.h>

#define X 0
#define Y 1
#define Z 2

#define ROLL_AXIS     0
#define PITCH_AXIS    1
#define YAW_AXIS      2
#define THROTTLE_AXIS 3

#define ON 1
#define OFF 0

#define GYRO_READY   0x1
#define ACCEL_READY  0x2
#define MAG_READY    0x4
#define BATT_READY   0x8
#define RX_READY     0x10
#define TX_READY     0x20
#define MOTOR_READY  0x40
#define AUX_READY    0x80

extern bool inFlight;
extern uint8_t vehicleStatus;

#define SENSORS_ONLINE ((vehicleStatus & 0x7) == 0x7)
#define BATT_ONLINE    ((vehicleStatus & BATT_READY) == BATT_READY)
#define RX_ONLINE      ((vehicleStatus & RX_READY) == RX_READY)
#define MOTORS_ONLINE  ((vehicleStatus & MOTOR_READY) == MOTOR_READY)
#define SYSTEM_ONLINE  ((vehicleStatus & 0xFF) == 0xFF)

// 8 channels - roll, pitch, throttle, yaw, mode, aux1, aux2, aux3
#define ROLL_CHANNEL    	0
#define PITCH_CHANNEL   	1
#define THROTTLE_CHANNEL 	2
#define YAW_CHANNEL     	3
#define MODE_CHANNEL     	4
#define AUX1_CHANNEL   		5
#define AUX2_CHANNEL		6
#define AUX3_CHANNEL		7

#define MICROS  1000000.0  // 1 second = 1*10^6 microseconds
#define MILLIS  1000.0     // 1 second = 1*10^3 milliseconds

//#define PI  3.14159265359  // need to make this a double

//#define DEG_2_RAD
//#define RAD_2_DEG

#endif /* globals_h */
