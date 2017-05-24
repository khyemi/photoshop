
/**********************************************************************
 * @file Highlight.cpp
 * @brief A kernel constructor for the Tools Class
 * 
 * Highlight.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values mask for the highlighter tool.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "Highlight.h"

Highlight::Highlight(){
	
	m_width = 15;
	
	m_mask = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_mask[i] = new float[m_width];
	}
	
	for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
			m_mask[i][j] = 0.2;
		}
	}
	
	
}

Highlight::~Highlight(){}


