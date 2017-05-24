
/**********************************************************************
 * @file Sharpen_Filter.h
 * @brief A kernel constructor for the Filter Class
 * 
 * Sharpen_Filter.h:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the sharpen filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#ifndef SHARPEN_FILTER_H
#define SHARPEN_FILTER_H

#include "Filter.h"

class  Sharpen_Filter: public Filter 
{
	
public:
	// constructor:
	Sharpen_Filter(int width);
	~Sharpen_Filter();
	
};
#endif
