/*
 * PID.cpp
 *
 *  Created on: July 25, 2013
 *      Author: Ryler Hockenbury
 *
 * Implementation of PID controller for roll and
 * pitch control.
 *
 */

#include "PID.h"

PID::PID() {
	input = 0.0;
	output = 0.0;
	setPoint = 0.0;

	kp = 5.0;
	ki = 0.5;
	kd = 0.5;

	outMax = 2000.0;
	outMin = 1000.0;

	iTerm = 0.0;
	lastInput = 0.0;

	sampleTime = 20; // 20 ms

	inAuto = false;
}


/*
 * Return new controller value
 */
double PID::updatePid(double input, double setPoint) {

	if(!inAuto) return 0;   // should return value

	// compute error variables
	double error = setPoint - input;
	iTerm += (ki * error);
	if(iTerm> outMax) iTerm= outMax;
	else if(iTerm< outMin) iTerm= outMin;
	double dInput = (input - lastInput);

	// compute output
	output = kp * error + iTerm - kd * dInput;
	if(output > outMax) output = outMax;
	else if(output < outMin) output = outMin;

	lastInput = input;
	return output;
}


/*
 * Set controller constants
 */
void PID::setTunings(double Kp, double Ki, double Kd) {

	double SampleTimeInSec = ((double)sampleTime)/1000;
	kp = Kp;
	ki = Ki * SampleTimeInSec;
	kd = Kd / SampleTimeInSec;
}


/*
 * Set sample period, adjusting time dependent parameters
 */
void PID::setSampleTime(int newSampleTime) {

	if(sampleTime > 0)
	   {
	      double ratio  = (double)newSampleTime / (double)sampleTime;
	      ki *= ratio;
	      kd /= ratio;
	      sampleTime = (unsigned long)newSampleTime;
	   }
}


/*
 * Set min and max controller outputs
 */
void PID::setOutputLimits(double Min, double Max) {
   if(Min > Max) return;
   outMin = Min;
   outMax = Max;

   if(output > outMax) output = outMax;
   else if(output < outMin) output = outMin;

   if(iTerm> outMax) iTerm= outMax;
   else if(iTerm< outMin) iTerm= outMin;
}


/*
 * Change between auto and manual mode
 */
void PID::setMode(bool Mode) {
    bool newAuto = (Mode == AUTOMATIC);

    // initialize when changing from manual to auto
    if(newAuto && !inAuto) {
        initializeMode();
    }
    inAuto = newAuto;
}


/*
 *
 */
void PID::initializeMode() {
   lastInput = input;
   iTerm = output;
   if(iTerm > outMax) iTerm= outMax;
   else if(iTerm < outMin) iTerm= outMin;
}
