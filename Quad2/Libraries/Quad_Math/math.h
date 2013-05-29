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

class filter
{
  public:

    static float LPF(float currentUnfiltered, float lastFiltered, float alpha);
    static float HPF(float currentUnfiltered, float lastUnfiltered,
    		         float lastFiltered, float alpa);


  private:

    // empty for now

};






#endif /* math_h */


