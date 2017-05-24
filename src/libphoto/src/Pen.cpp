
/**********************************************************************
 * @file Pen.cpp
 * @brief A mask constructor for the Tools class
 * 
 * Pen.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values for the pen tool.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "Pen.h"

Pen::Pen(){
	
	m_width = 5;
	
	m_mask = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_mask[i] = new float[m_width];
	}
	
	
	for(int i = 0; i < m_width; i++){
	        for(int j = 0; j < m_width; j++){
			m_mask[i][j] = 1;
		}
	}
	
	m_mask[0][0] = 0;
	m_mask[0][m_width - 1] = 0;
	m_mask[m_width - 1][0] = 0;
	m_mask[m_width - 1][m_width -1] = 0;
}

Pen::~Pen(){}

