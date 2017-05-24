
/**********************************************************************
 * @file SprayCan.cpp
 * @brief A kernel constructor for the Tools class
 * 
 * SprayCan.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values kernel for the spray can tool.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "SprayCan.h"
#include <cmath>

SprayCan::SprayCan(){
	
	m_width = 41;
	int d = (m_width-1)/2;
	m_mask = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_mask[i] = new float[m_width];
	}
	for(int i = 0; i < m_width; i++){
		for(int j = 0; j < m_width; j++){
			float dis = sqrt((i - d)*(i - d) + (j - d)*(j - d));
			if(dis < d)
				m_mask[i][j] = 0.2*(1 - dis*1.0/d);
			else
				m_mask[i][j] = 0;
		}
	}
}

SprayCan::~SprayCan() {}

