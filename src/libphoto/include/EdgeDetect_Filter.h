
/**********************************************************************
 * @file Edge_Filter.h
 * @brief A kernel constructor for the Filter Class
 * 
 * EdgeDetect_Filter.h:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the edge detect filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#ifndef EDGEDETECT_FILTER_H
#define EDGEDETECT_FILTER_H

#include "Filter.h"

class  EdgeDetect_Filter: public Filter 
{
	
public:
	// constructor:
	EdgeDetect_Filter();
	~EdgeDetect_Filter();
	
};
#endif
