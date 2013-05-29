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

//#define SERIAL_CHART           // serial chart utility for graphing serial data
#define SERIAL_MONITOR_DEBUG

int counter;      // measurement counter

// time and integration variables
unsigned int prevTime;
unsigned int currTime;
unsigned int timeStep;

// pitch, roll, yaw from gyro
float pitchGyro;
float rollGyro;
float yawGyro;

// pitch, roll and yaw from accel
float pitchAccel;
float rollAccel;
float yawAccel;

// fused and filtered pitch, roll, and yaw
float pitch;
float roll;
float yaw;

// filter weight
float alpha;

void initCompFilter();
void getOrientation(float orientation[3],float gyroData[3], float accelData[3], float compData[3]);


void initCompFilter()
{

  counter = 1;      // measurement counter

  // time and integration variables
  prevTime = 0;
  currTime = 0;
  timeStep = 0;

  // pitch, roll, yaw from gyro
  pitchGyro = 0.0;
  rollGyro = 0.0;
  yawGyro = 0.0;

  // pitch, roll and yaw from accel
  pitchAccel = 0.0;
  rollAccel = 0.0;
  yawAccel = 0.0;

  // fused and filtered pitch, roll, and yaw
  pitch = 0.0;
  roll = 0.0;
  yaw = 0.0;

  // filter weight
  alpha = 0.02;
}

void getOrientation( float orientation[3], float gyroData[3], float accelData[3], float compData[3] )
{
  //Gyro angle calculation
  currTime = micros();

  timeStep = currTime - prevTime; //*MICROS;
  pitchGyro = pitchGyro + gyroData[X]*timeStep/1000000.0;
  rollGyro = rollGyro + gyroData[Y]*timeStep/1000000.0;
  yawGyro = yawGyro + gyroData[Z]*timeStep/1000000.0;

  prevTime = currTime;

  //Accel angle calculation
  pitchAccel = atan2(accelData[1], accelData[2])*180/PI;
  rollAccel = atan2(accelData[0], accelData[2])*180/PI;

  /*
   * Compass angle calculation
   */

    //




  //Complementary filter
  pitch = alpha*pitchGyro + (1-alpha)*pitchAccel;
  roll = alpha*rollGyro + (1-alpha)*rollAccel;

  // fuse with compass data



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

  Serial.print(pitchGyro);
  Serial.print("\t\t");
  Serial.print(rollGyro);
  Serial.print("\t\t");
  Serial.print(yawGyro);
  Serial.print("\t\t");

  Serial.print(pitchAccel);
  Serial.print("\t\t");
  Serial.print(rollAccel);
  Serial.print("\t\t");
  Serial.print(yawAccel);
  Serial.print("\t\t");

  Serial.print(pitch);
  Serial.print("\t\t");
  Serial.print(roll);
  Serial.print("\n");

  #endif

  counter++;

  orientation[0] = pitch;
  orientation[1] = roll;
  orientation[2] = 0; // yaw;

}

#endif /* compFilter_h */
