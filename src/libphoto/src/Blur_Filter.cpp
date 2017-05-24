

/**********************************************************************
 * @file Blur_Filter.cpp
 * @brief A kernel constructor for the Filter Class
 * 
 * Blur_Filter.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the blur filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/




#include "Blur_Filter.h"
#include <cmath>
#include <iostream>

Blur_Filter::Blur_Filter(int width){
	
	m_width = width;


	m_Kernel = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_Kernel[i] = new float[m_width];
	}
	
	for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
				if(i+j <= (m_width+1)/2-2 || m_width-i+j <= 
				(m_width+1)/2-1 || 
				i+m_width-j <= (m_width+1)/2-1 || 
				m_width-i+m_width-j <= (m_width+1)/2)
					m_Kernel[i][j] = 0;
				else
					m_Kernel[i][j] = 
					1.0/((1+m_width)*(m_width/2+1)-m_width);	
			}
	}
	
	
	
	

}

Blur_Filter::~Blur_Filter(){}

