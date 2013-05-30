/*
 * math.h
 *
 *  Created on: May 28, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for math utility
 *
 */

#ifndef math_h
#define math_h

class math
{
  public:

    static float LPF(float currentUnfiltered, float lastFiltered, float alpha);
    static float HPF(float currentUnfiltered, float lastUnfiltered,
    		         float lastFiltered, float alpa);

    static float getMagnitude(float value[3]);
    static float getMagnitude(float xValue, float yValue, float zValue);

  private:

    // empty for now

};






#endif /* math_h */


