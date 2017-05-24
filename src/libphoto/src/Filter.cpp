
/**********************************************************************
 * @file Filter.cpp
 * @brief members and constructors for specific kernel based on its sub-classes
 * 
 * Kernel is a 2d floating points array used to sweep over image canvas
 * without using any mouse cursor.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "Filter.h"


Filter::Filter() {}

int Filter::getWidth()
{
	return m_width;
}

float** Filter::getKernel()
{
	return m_Kernel;
}


