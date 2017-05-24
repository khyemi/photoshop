

/**********************************************************************
 * @file Blur.cpp
 * @brief A mask constructor for the Tools class
 * 
 * Blur.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values for the blur tool.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "Blur.h"
#include <cmath>

Blur::Blur(){
	
	m_width = 21;
	int center = m_width/2;
	int dist;

	m_mask = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_mask[i] = new float[m_width];
	}
	
	for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
			dist = sqrt((i-center)*(i-center)+(j-center)*(j-center));
			if(dist <= center)
				m_mask[i][j] = 1;
			else
				m_mask[i][j] = 0;				
		}
	}
	
}

Blur::~Blur(){}

