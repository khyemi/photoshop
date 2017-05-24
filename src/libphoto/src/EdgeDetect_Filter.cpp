

/**********************************************************************
 * @file Edge_Filter.cpp
 * @brief A kernel constructor for the Filter Class
 * 
 * EdgeDetect_Filter.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the edge detect filter.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/




#include "EdgeDetect_Filter.h"
#include <cmath>

EdgeDetect_Filter::EdgeDetect_Filter(){
	
	m_width = 3;


	m_Kernel = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_Kernel[i] = new float[m_width];
	}
	
	for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
				m_Kernel[i][j] = -1;
			}
	}
	m_Kernel[1][1] = 8;
	
	
	
	

}

EdgeDetect_Filter::~EdgeDetect_Filter(){}

