/*
 * complFilter.h
 *
 *  Created on: May 28, 2013
 *      Author: Ryler Hockenbury
 *
 * Data fusion and filter algorithm for gyroscope, accelerometer
 * and magnetometer using complementary filter
 *
 */

#ifndef compFilter_h
#define compFilter_h

#include "../../Libraries/Quad_Defines/globals.h"
#include "../../Libraries/Quad_Math/math.h"
#include <arduino.h>


//unsigned int counter = 1;     // measurement counter

//unsigned int previousTime = 0;
//unsigned int currentTime = 0;
//float timeStep = 0.0;

float accelAngle[2] = {0.0, 0.0};      // derived angles (roll, pitch) from accelerometer

float compAngle = 0.0;  // derived angle (yaw) from compass

float alpha = 0.98;     // complementary filter weight


/* Calculates roll, pitch and yaw from the gyroscope,
 * accelerometer, and magnetometer data
 *
 * Data is fused and filtered with complementary filter.
 *
 */
void getOrientation(float flightAngle[3], float gyroData[3], float accelData[3], float compData[3])
{
  //currentTime = micros();
  //timeStep = ((float)(currentTime - previousTime))/MICROS;
  //previousTime = currentTime;
  // this function is called 100 times a second, so timestep is somewhat guaraneteed

  //Accelerometer angle calculation
  accelAngle[ROLL_AXIS] = atan2(accelData[Y],
 		 vector::getMagnitude(accelData[X], 0.0, accelData[Z]))*180/PI;
  accelAngle[PITCH_AXIS] = atan2(accelData[X],
 		  vector::getMagnitude(0.0, accelData[Y], accelData[Z]))*180/PI;

  //Complementary filter (roll and pitch)
  flightAngle[ROLL_AXIS] = alpha*(flightAngle[ROLL_AXIS] + gyroData[ROLL_AXIS]*0.01)
  		                      + (1.0-alpha)*accelAngle[ROLL_AXIS];

  flightAngle[PITCH_AXIS] = alpha*(flightAngle[PITCH_AXIS] + gyroData[PITCH_AXIS]*0.01)
  		                      + (1.0-alpha)*accelAngle[PITCH_AXIS];

  //Compass angle calculation
  vector::normalize(compData);

  float cos_roll = cos(flightAngle[ROLL_AXIS]*PI/180);
  float sin_roll = sin(flightAngle[ROLL_AXIS]*PI/180);
  float cos_pitch = cos(flightAngle[PITCH_AXIS]*PI/180);
  float sin_pitch = sin(flightAngle[PITCH_AXIS]*PI/180);

  compAngle = atan2( (-compData[Y]*cos_roll + compData[Z]*sin_roll) ,
    (compData[X]*cos_pitch + compData[Y]*sin_pitch*sin_roll + compData[Z]*sin_pitch*cos_roll) )*180/PI;

  if(compAngle < 0.0)  // return yaw in  range 0 to 360
 	compAngle += 360.0;

   flightAngle[YAW_AXIS] = flightAngle[YAW_AXIS] + gyroData[YAW_AXIS]*0.01;

   // return yaw in range 0 to 360 degrees
   if(flightAngle[YAW_AXIS] > 360.0)
	   flightAngle[YAW_AXIS] = 0.0;

   if(flightAngle[YAW_AXIS] < 0.0)
	   flightAngle[YAW_AXIS] = 360.0;

   float diff = compAngle - flightAngle[YAW_AXIS];

   if(diff > 180.0)
	   diff -= 360.0;

   if(diff < -180.0)
	   diff += 360.0;

   flightAngle[YAW_AXIS] = flightAngle[YAW_AXIS] + diff*0.003;
}

#endif /* compFilter_h */
