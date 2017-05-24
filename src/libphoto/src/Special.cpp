

/**********************************************************************
 * @file Special.cpp
 * @brief A kernel constructor for the Tools class
 * 
 * Special.cpp:
 * 	This class will generate the required two dimensional floating
 * 	point values mask for the colorful dust tool.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/

#include "Special.h"

Special::Special(){
	
	m_width = 71;
	
	m_mask = new float*[m_width];
	for(int i = 0; i < m_width; i++){
		m_mask[i] = new float[m_width];
	}
	
	for(int i = 0; i < m_width; i = i+2){
	        for(int j = 0; j < m_width; j = j+2){
			m_mask[i][j] = 0;
		}
	}
	
	
	for(int i = 5; i< m_width; i = i+3) {
			for(int j = 5; j < m_width; j = j+5){
				m_mask[i][j] = 0.2;
			}
		}
	
	for(int i = 3; i < m_width; i = i+5){
	        for(int j = 3; j < m_width; j = j+3){
			m_mask[i][j] = 0.8;
		}
	}
// Done making a square box.

// now making circle pattern. 

	for(int i = 0; i < (int) (m_width/5); i++){
		for(int j = 0; j < (int) m_width/5; j++){
			m_mask[i][j] = 0;
		}
	}
	
	
	for(int i = 0; i < (int) m_width/5; i++){
		for(int j = m_width - m_width/5; j< m_width; j++){
			m_mask[i][j] = 0;
		}
	}
	
	for(int i = m_width - m_width/5; i < m_width; i++){
		for(int j = 0; j < m_width/5; j++) {
			m_mask[i][j] = 0;
		}
	}
	
	for(int i = m_width - m_width/5; i < m_width; i++){
		for(int j = m_width - m_width/5; j < m_width; j++){
			m_mask[i][j] = 0;
		}
	}

}

Special::~Special(){}


