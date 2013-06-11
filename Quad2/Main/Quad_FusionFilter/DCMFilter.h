/*
 * DCMFilter.h
 *
 *  Created on: June 8, 2013
 *      Author: Ryler Hockenbury
 *
 *
 *
 *
 */



// 25th August 2010
//=====================================================================================================
// Description:
//
// Quaternion implementation of the 'DCM filter' [Mayhony et al].  Incorporates the magnetic distortion
// compensation algorithms from my filter [Madgwick] which eliminates the need for a reference
// direction of flux (bx bz) to be predefined and limits the effect of magnetic distortions to yaw
// axis only.
//
// User must define 'halfT' as the (sample period / 2), and the filter gains 'Kp' and 'Ki'.
//
// Global variables 'q0', 'q1', 'q2', 'q3' are the quaternion elements representing the estimated
// orientation.  See my report for an overview of the use of quaternions in this application.
//
// User must call 'AHRSupdate()' every sample period and parse calibrated gyroscope ('gx', 'gy', 'gz'),
// accelerometer ('ax', 'ay', 'ay') and magnetometer ('mx', 'my', 'mz') data.  Gyroscope units are
// radians/second, accelerometer and magnetometer units are irrelevant as the vector is normalised.
//
//=====================================================================================================


//=====================================================================================================
// AHRS.h
// S.O.H. Madgwick
// 25th August 2010
//=====================================================================================================
//
// See AHRS.c file for description.
//
//=====================================================================================================
#ifndef AHRS_h
#define AHRS_h

//#define SERIAL_CHART

//----------------------------------------------------------------------------------------------------
// Variable declaration
unsigned int counter = 0;

//extern float q0, q1, q2, q3;	// quaternion elements representing the estimated orientation

//---------------------------------------------------------------------------------------------------
// Function declaration

//void getOrientation(float gyroAngle[3], float gyroData[3], float accelData[3], float compData[3]);

//void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

#endif
//=====================================================================================================
// End of file
//=============================








//----------------------------------------------------------------------------------------------------
// Header files

//#include "AHRS.h"
#include <math.h>
#include <arduino.h>

//----------------------------------------------------------------------------------------------------
// Definitions

#define Kp 25.0f  //2.0f			// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005f //0.005f		// integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f		// half the sample period

//---------------------------------------------------------------------------------------------------
// Variable definitions

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;	// scaled integral error

//====================================================================================================
// Function
//====================================================================================================

void getOrientation(float gyroAngle[3], float gyroData[3], float accelData[3], float compData[3])
{
//void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float gx = gyroData[0]*PI/180;
	float gy = gyroData[1]*PI/180;
	float gz = gyroData[2]*PI/180;

	float ax = accelData[0];
	float ay = accelData[1];
	float az = accelData[2];

	float mx = compData[0];
	float my = compData[1];
	float mz = compData[2];

	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;

	// auxiliary variables to reduce number of repeated operations
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;

	// normalise the measurements
	norm = sqrt(ax*ax + ay*ay + az*az);
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;
	norm = sqrt(mx*mx + my*my + mz*mz);
	mx = mx / norm;
	my = my / norm;
	mz = mz / norm;

	// compute reference direction of flux
	hx = 2*mx*(0.5 - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
	hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5 - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
	hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5 - q1q1 - q2q2);
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz;

	// estimated direction of gravity and flux (v and w)
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
	wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
	wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) + (my*wz - mz*wy);
	ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
	ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

	// integral error scaled integral gain
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	// integrate quaternion rate and normalise
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	// roll
	gyroAngle[0] = atan2(2*q2*q3 - 2*q0*q1, 2*q1*q1 + 2*q2*q2 - 1)*180/PI; // phi


	// pitch
	gyroAngle[1] = -asin(2*q1*q3 + 2*q0*q2)*180/PI; // theta

	// yaw
	gyroAngle[2] = atan2(2*q1*q2 - 2*q0*q3, 2*q2*q2 + 2*q3*q3 - 1)*180/PI; // psi



	  Serial.print(gyroAngle[0]);
	  Serial.print(",");
	  Serial.print(gyroAngle[1]);
	  Serial.print(",");
	  Serial.print(gyroAngle[2]);
	  Serial.print("\n");

}

//====================================================================================================
// END OF CODE
//====================================================================================================


void printOrientation(float currentPosition[3])
{
  //Format and print serial stream
  #ifdef SERIAL_CHART

  Serial.print(counter);
  Serial.print(",");

  Serial.print(currentPosition[0]);
  Serial.print(",");
  Serial.print(currentPosition[1]);
  Serial.print(",");
  Serial.print(currentPosition[2]);
  Serial.print("\n");

  //Serial.print(accelAngle[0]);
  //Serial.print(",");
  //Serial.print(accelAngle[1]);
  //Serial.print(",");
  //Serial.print(compAngle);
  //Serial.print("\n");

  #endif


  #ifdef SERIAL_MONITOR_DEBUG

  Serial.print(counter);
  Serial.print("\t\t");

  if(gyroAngle[1] >= 0) Serial.print(" ");
  Serial.print(gyroAngle[1]);
  Serial.print("\t\t");
  if(accelAngle[1] >= 0) Serial.print(" ");
  Serial.print(accelAngle[1]);
  Serial.print("\t\t");

  if(gyroAngle[0] >= 0) Serial.print(" ");
  Serial.print(gyroAngle[0]);
  Serial.print("\t\t");
  if(accelAngle[0] >= 0) Serial.print(" ");
  Serial.print(accelAngle[0]);
  Serial.print("\t\t");

  if(gyroAngle[2]>= 0) Serial.print(" ");
  Serial.print(gyroAngle[2]);
  Serial.print("\t\t");
  if(compAngle) >= 0) Serial.print(" ");
  Serial.print(compAngle);
  Serial.print("\n");

  #endif

  counter++;

}
