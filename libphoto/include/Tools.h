
/**********************************************************************
 * @file Tools.h
 * @brief Virtual declarations for its sub-classes 
 * 
 * m_mask is 2d floating points array used to sweep over image canvas
 * by an interactive tool with the mouse cursor.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#ifndef TOOLS_H
#define TOOLS_H


class Tools
{
	
public:
	// constructor:
	Tools();
	virtual ~Tools(){};
	
	// routines
	 int getWidth();
	 float** getMask();
	
	
protected:
	//members variable.
	float** m_mask;
	int m_width;

	
};

#endif
