/*
 * conf.h
 *
 *  Created on: Aug 17, 2013
 *      Author: ryler
 */

#ifndef CONF_H_
#define CONF_H_

// PID parameters

// debug parameters

// serial chart or ground station
// serial rate

//

//#define I2CDEV_SERIAL_DEBUG

#define RADIO_NUM_CHANNELS  6     // number of radio channels
//#define RADIO_SMOOTH_FACTOR {0.9, 0.9, 0.9, 0.9, 0.9, 0.9}   // adjust stick responsiveness
//#define RADIO_SCALE_FACTOR  1.0   // adjust stick range (0.5 will double range, 2 will half range)
#define MAX_COMMAND_ANGLE 45.0  //


const float RADIO_SMOOTH_FACTOR[RADIO_NUM_CHANNELS] = {0.9, 0.9, 0.9, 0.9, 0.9, 0.9};
const float RADIO_SCALE_FACTOR[RADIO_NUM_CHANNELS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

#endif /* CONF_H_ */
