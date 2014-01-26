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

#include "globals.h"
#include "storage.h"

class PID {

	public:
		PID();
		float updatePid(float input, float setPoint);
		void setPGain(float kp);
		float getPGain();
		void setIGain(float ki);
		float getIGain();
		void setDGain(float kd);
		float getDGain();
		static void setMode(int mode);
		static bool getMode();

	private:
		static int mode;
		float input, output, setPoint;
		float iTerm;
		float lastInput;
		float kp, ki, kd;
		float outMax, outMin;
		uint32_t sampleTime;
};

#endif /* pid_h */
