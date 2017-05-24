
/**********************************************************************
 * @file Tools.cpp
 * @brief Virtual declarations for its sub-classes 
 * 
 * m_mask is a 2d floating points array used to sweep over image canvas
 * by an interactive tool with the mouse cursor.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "Tools.h"


Tools::Tools() {}

int Tools::getWidth()
{
	return m_width;
}

float** Tools::getMask()
{
	return m_mask;
}




















