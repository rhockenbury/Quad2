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

class PID {

	public:
		PID();
		void setTunings(double kp, double ki, double kd);
		double updatePid(double input, double setPoint);
		void setSampleTime(int newSampleTime);
		void setOutputLimits(double newMin, double newMax);
		void setMode(bool mode);
		void initializeMode();

	private:
		float input, output, setPoint;
		float iTerm;
		float lastInput;
		float kp, ki, kd;
		float outMax, outMin;
		bool inAuto;
		unsigned long sampleTime;
};

#endif /* pid_h */
