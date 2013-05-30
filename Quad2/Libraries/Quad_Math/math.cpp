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
float math::LPF(float currentUnfiltered, float lastFiltered, float alpha)
{
  return (1-alpha)*currentUnfiltered + alpha*lastFiltered;
}


//float filter::LPF();

float math::getMagnitude(float value[3])
{
  return sqrt(value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
}

float math::getMagnitude(float xValue, float yValue, float zValue)
{
  return sqrt(xValue*xValue + yValue*yValue + zValue*zValue);
}
