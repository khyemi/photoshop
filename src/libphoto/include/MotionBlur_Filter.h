#ifndef MOTIONBLUR_FILTER_H
#define MOTIONBLUR_FILTER_H

#include "Filter.h"

class  MotionBlur_Filter: public Filter 
{
	
public:
	// constructor:
	MotionBlur_Filter(int direction, int width);
	~MotionBlur_Filter();
	
};
#endif
