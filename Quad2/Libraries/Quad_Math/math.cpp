/*
 * math.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for math utilities
 *
 */

#include <arduino.h>
#include "math.h"

// filter class members
float filter::LPF(float currentUnfiltered, float lastFiltered, float alpha)
{
  // y[n] = (1-alpha)*x[n] + alpha*y[n-1]
  return (1-alpha)*currentUnfiltered + alpha*lastFiltered;
}


float filter::HPF(float currentUnfiltered, float lastUnfiltered, float lastFiltered, float alpha )
{
  // y[n] = (1-alpha)*y[n-1] + (1-alpha)*(x[n] - x[n-1])
  return (1-alpha)*lastFiltered + (1-alpha)*(currentUnfiltered - lastUnfiltered);
}


// vector class members
float vector::getMagnitude(float value[3])
{
  return sqrt(value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
}

float vector::getMagnitude(float xValue, float yValue, float zValue)
{
  return sqrt(xValue*xValue + yValue*yValue + zValue*zValue);
}
