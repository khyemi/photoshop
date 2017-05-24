

/**********************************************************************
 * @file Sharpen_Filter.cpp
 * @brief A kernel constructor for the Filter Class
 * 
 * Sharpen_Filter.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the sharpen filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "Sharpen_Filter.h"
#include <cmath>

Sharpen_Filter::Sharpen_Filter(int width){
	
	m_width = width;


	m_Kernel = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_Kernel[i] = new float[m_width];
	}
	
	for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
				m_Kernel[i][j] = -1;
			}
	}
	m_Kernel[m_width/2][m_width/2] = m_width*m_width;
	
	
	
	

}

Sharpen_Filter::~Sharpen_Filter(){}

