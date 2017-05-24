
/**********************************************************************
 * @file Filter.h
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


#ifndef FILTER_H
#define FILTER_H


class Filter
{
	
public:
	// constructor:
	Filter();
	virtual ~Filter(){};
	
	// routines
	 int getWidth();
	 float** getKernel();
	
	
protected:
	//members variable.
	float** m_Kernel;
	int m_width;

	
};

#endif
