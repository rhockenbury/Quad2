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

//#define SERIAL_CHART           // format for serial chart
#define SERIAL_MONITOR_DEBUG     // format for serial monitor

int counter = 1;     // measurement counter

// time and integration variables
unsigned int prevTime = 0;
unsigned int currTime = 0;
unsigned int timeStep = 0;

// pitch, roll, yaw from gyro
float pitchGyro = 0.0;
float rollGyro = 0.0;
float yawGyro = 0.0;

// pitch, roll and yaw from accel
float pitchAccel = 0.0;
float rollAccel = 0.0;
float yawAccel = 0.0;

// component values from compass
float yawComp = 0.0;
float compNorm = 0.0;
float xComp = 0.0;
float yComp = 0.0;
float zComp = 0.0;

// fused and filtered pitch, roll, and yaw
float pitch = 0.0;
float roll = 0.0;
float yaw = 0.0;

// filter weight
float alpha = 0.02;

void getOrientation( float orientation[3], float gyroData[3], float accelData[3], float compData[3] )
{
  //Gyro angle calculation
  currTime = micros();
  timeStep = currTime - prevTime;

  pitchGyro = pitchGyro + gyroData[X]*timeStep/MICROS;
  rollGyro = rollGyro + gyroData[Y]*timeStep/MICROS;
  yawGyro = yawGyro + gyroData[Z]*timeStep/MICROS;

  prevTime = currTime;

  //Accel angle calculation
  pitchAccel = atan2(accelData[1], accelData[2])*180/PI;
  rollAccel = atan2(accelData[0], accelData[2])*180/PI;

  //Complementary filter (pitch and roll)
  pitch = alpha*pitchGyro + (1-alpha)*pitchAccel;
  roll = alpha*rollGyro + (1-alpha)*rollAccel;

  //Compass angle calculation
  float compNorm = vector::getMagnitude(compData);

  xComp = compData[0] / compNorm;
  yComp = compData[1] / compNorm;
  zComp = compData[2] / compNorm;

  yawComp = atan2( (-yComp*cos(roll) + zComp*sin(roll) ) ,
		     (xComp*cos(pitch) + yComp*sin(pitch)*sin(roll) + zComp*sin(pitch)*cos(roll)) )*180/PI;

  //Complementary filter (yaw)
  yaw = alpha*yawGyro + (1-alpha)*yawComp;

  //Format and print serial stream

  #ifdef SERIAL_CHART
  Serial.print(counter);
  Serial.print(",");

  Serial.print(pitchGyro);
  Serial.print(",");
  Serial.print(rollGyro);
  Serial.print(",");
  //Serial.print(yawGyro);
  //Serial.print(",");

  Serial.print(pitchAccel);
  Serial.print(",");
  Serial.print(rollAccel);
  Serial.print(",");
 // Serial.print(yawAccel);
 // Serial.print(",");

  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print("\n");

  #endif

  #ifdef SERIAL_MONITOR_DEBUG
  Serial.print(counter);
  Serial.print("\t");
  //Serial.print(timeStep);
  //Serial.print("\t");

  if(pitchGyro >= 0) Serial.print(" ");
  Serial.print(pitchGyro);
  Serial.print("\t\t");
  if(pitchAccel >= 0) Serial.print(" ");
  Serial.print(pitchAccel);
  Serial.print("\t\t");
  if(pitch >= 0) Serial.print(" ");
  Serial.print(pitch);
  Serial.print("\t\t");

  if(rollGyro >= 0) Serial.print(" ");
  Serial.print(rollGyro);
  Serial.print("\t\t");
  if(pitchAccel >= 0) Serial.print(" ");
  Serial.print(rollAccel);
  Serial.print("\t\t");
  if(roll >= 0) Serial.print(" ");
  Serial.print(roll);
  Serial.print("\n");

  //if(yawGyro >= 0) Serial.print(" ");
  //Serial.print(yawGyro);
  //Serial.print("\t\t");
  //if(pitchAccel >= 0) Serial.print(" ");
  //Serial.print(yawAccel);
  //Serial.print("\t\t");

  #endif

  counter++;

  orientation[0] = pitch;
  orientation[1] = roll;
  orientation[2] = yaw;

}

#endif /* compFilter_h */
