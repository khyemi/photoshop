

/**********************************************************************
 * @file MotionBlur_Filter.cpp
 * @brief A kernel constructor for the Filter Class
 * 
 * MotionBlur_Filter.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the motion blur filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "MotionBlur_Filter.h"
#include <cmath>

MotionBlur_Filter::MotionBlur_Filter(int direction, int width){
	
	m_width = width;


	m_Kernel = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_Kernel[i] = new float[m_width];
	}
	
		for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
				switch(direction){
					case 0:
					if(i == m_width/2)
						m_Kernel[i][j] = 1.0/m_width;
					else
						m_Kernel[i][j] = 0;
					break;
					case 1:
					if(j == m_width/2)
						m_Kernel[i][j] = 1.0/m_width;
					else
						m_Kernel[i][j] = 0;
					break;
					case 3:
					if(i+j == m_width-1)
						m_Kernel[i][j] = 1.0/m_width;
					else
						m_Kernel[i][j] = 0;
					break;
					case 2:
					if(i == j)
						m_Kernel[i][j] = 1.0/m_width;
					else
						m_Kernel[i][j] = 0;
					break;
					default:
					break;
				}
			}
		}
	
	

}

MotionBlur_Filter::~MotionBlur_Filter(){}

