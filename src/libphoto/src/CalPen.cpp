
/**********************************************************************
 * @file CalPen.cpp
 * @brief A mask constructor for the Tools class
 * 
 * CalPen.h:
 * 	This class will generate the required two dimensional floating
 * 	point values for the caligraphy pen tool.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "CalPen.h"

CalPen::CalPen(){
	
	m_width = 15;
	
	m_mask = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_mask[i] = new float[m_width];
	}
	
	
	for(int i = 0; i < m_width; i++) {
		for (int j = 0; j<m_width; j++){
			m_mask[i][j] = 0;
		}
	}
	
	for(int i = 6; i < m_width-6; i++){
	        for(int j = 0; j < m_width; j++){
			m_mask[i][j] = 1;
		}
	}
	
}

CalPen::~CalPen(){}



