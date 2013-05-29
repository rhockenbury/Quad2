/*
 * math.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for math utilities
 *
 */

#include "math.h"

// low pass filter
float filter::LPF(float currentUnfiltered, float lastFiltered, float alpha)
{
  return (1-alpha)*currentUnfiltered + alpha*lastFiltered;
}


//float filter::LPF();
