/**********************************************************************
 * @file main.cpp
 * @brief Specific behavior implementaion to run MedImageGUI/MedImageCML together.
 * 
 * MedImageGUI:
 * 	Features include the following:
 * 	A red arrow stamp and pen tool for circling locations in image.
 *	A set of filters to process MRI scans
 * 
 * MedImageCML:
 * 	MedImageCML is a commandline version of MedImageGUI. It is strictly 
 * 	optimized for commandline functionalities and therefore is a class
 * 	of its own, unlike being a sub-class of BaseGfxApp in MedImageGUI.
 * 
 * @author Henry Luong, Xiaoxun Meng, Hye Mi Kim
 * @bug No known bugs.
 * 
 * 
 * 
 * *******************************************************************/


#include "MedImageGUI.h"
#include "MedImageCML.h"
#include "ColorData.h"
#include "string.h"

int main(int argc, char* argv[])
{
	if(argc == 1){ // no argument -> start GUI
		MedImageGUI* app = new MedImageGUI(argc, argv,800,800, ColorData(1,1,0.95));
		app->runMainLoop();
	}
	else { // with arguments -> start CML
		MedImageCML* instance = new MedImageCML(argc, argv, ColorData(1,1,0.95));
		instance->argument_check(argc, argv);
	}
	exit(0);
}



