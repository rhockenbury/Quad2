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

int PID::mode = ON;

PID::PID() {
	input = 0.0;
	output = 0.0;
	setPoint = 0.0;

	kp = Storage::readPidPValue();
	ki = Storage::readPidIValue();
	kd = Storage::readPidDValue();

	outMax = 3000.0;
	outMin = -3000.0;

	iTerm = 0.0;
	lastInput = 0.0;

	sampleTime = 10;
}


/*
 * Return new controller value
 */
float PID::updatePid(float input, float setPoint) {

	// compute error variables
	float error = setPoint - input;
	iTerm += (ki*error*sampleTime);

	// bound iTerm
	if(iTerm > outMax) {
		iTerm= outMax;
	}
	else if(iTerm < outMin) {
		iTerm= outMin;
	}

	float dInput = (input - lastInput)/sampleTime;

	// compute output
	output = kp * error + iTerm - kd * dInput;

	// bound outputs
	if(output > outMax) {
		output = outMax;
	}
	else if(output < outMin) {
		output = outMin;
	}

	lastInput = input;
	return output;
}

/*
 * Set and save proportional gain term
 */
void PID::setPGain(float Kp) {
	kp = Kp;
	Storage::writePidPValue(kp);
}

float PID::getPGain() {
	return kp;
}

/*
 * Set and save integral gain term
 */
void PID::setIGain(float Ki) {
	ki = Ki;
	Storage::writePidIValue(ki);
}

float PID::getIGain() {
	return ki;
}

/*
 * Set and save derivative gain term
 */
void PID::setDGain(float Kd) {
	kd = Kd;
	Storage::writePidDValue(kd);
}

float PID::getDGain() {
	return kd;
}

void PID::setMode(int newMode) {
	PID::mode = newMode;
}

bool PID::getMode() {
	return PID::mode;
}

/*
 * Set sample period, adjusting time dependent parameters
 */
/*
void PID::setSampleTime(uint32_t newSampleTime) {

	if(sampleTime > 0)
	   {
	      float ratio  = (float)newSampleTime / (float)sampleTime;
	      ki *= ratio;
	      kd /= ratio;
	      sampleTime = (uint32_t)newSampleTime;
	   }
}
*/

/*
 * Set min and max controller outputs
 */

/*
void PID::setOutputLimits(float Min, float Max) {
   if(Min > Max) return;
   outMin = Min;
   outMax = Max;

   if(output > outMax) output = outMax;
   else if(output < outMin) output = outMin;

   if(iTerm> outMax) iTerm= outMax;
   else if(iTerm< outMin) iTerm= outMin;
}
*/

/*
 * Change between auto and manual mode
 */
/*
void PID::setMode(bool Mode) {
    bool newAuto = (Mode == AUTOMATIC);

    // initialize when changing from manual to auto
    if(newAuto && !inAuto) {
        initializeMode();
    }
    inAuto = newAuto;
}
*/

/*
 * Return the current mode
 */
/*
bool PID::getMode() {
	return inAuto;
}
*/

/*
 *kkk
 */
/*
void PID::initializeMode() {
   lastInput = input;
   iTerm = output;
   if(iTerm > outMax) iTerm= outMax;
   else if(iTerm < outMin) iTerm= outMin;
}
*/
