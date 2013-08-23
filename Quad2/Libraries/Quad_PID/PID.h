/*
 * PID.h
 *
 *  Created on: May 30, 2013
 *      Author: Ryler Hockenbury
 *
 * Implementation of PID controller for roll and
 * pitch control.
 *
 */

#ifndef pid_h
#define pid_h

#define MANUAL 0
#define AUTOMATIC 1

#include <arduino.h>

class PID {

	public:
		PID();
		void setTunings(float kp, float ki, float kd);
		float updatePid(float input, float setPoint);
		void setSampleTime(uint32_t newSampleTime);
		void setOutputLimits(float newMin, float newMax);
		void setMode(bool mode);
		bool getMode();
		void initializeMode();

	private:
		float input, output, setPoint;
		float iTerm;
		float lastInput;
		float kp, ki, kd;
		float outMax, outMin;
		bool inAuto;
		uint32_t sampleTime;
};

#endif /* pid_h */
