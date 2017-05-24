
/**********************************************************************
 * @file Blur_Filter.h
 * @brief A kernel constructor for the Filter Class
 * 
 * Blur_Filter.h:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the blur filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/




#ifndef BLUR_FILTER_H
#define BLUR_FILTER_H

#include "Filter.h"

class  Blur_Filter: public Filter 
{
	
public:
	// constructor:
	Blur_Filter(int width);
	~Blur_Filter();
	

};
#endif
